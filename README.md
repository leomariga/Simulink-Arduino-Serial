# Simulink and Arduino connection

Welcome! You are here because you are trying to connect your Arduino board to Simulink to transmit some data (maybe some sensor output) but everything is going wrong and your world is falling apart (drama). 

In this tutorial, we are **NOT** using the [official Simulink Arduino Package](https://www.mathworks.com/help/supportpkg/arduino/examples/getting-started-with-arduino-hardware.html), which enables you to access directly Arduino pins and many other features. **What's the problem with this library?** It uses a specific Arduino code, so you can't customize the code inside the board (if you want to use the Arduino as a control device for a "Hardware in the loop" project, as example).

## The solution
To overcome the problem we can send and receive data using the serial blocks from Simulink without downloading any extra packages:

### Instrument Control Toolbox

* [Serial Configuration](https://www.mathworks.com/help/instrument/serialconfiguration.html)
* [Serial Send](https://www.mathworks.com/help/instrument/serialsend.html)
* [Serial Receive](https://www.mathworks.com/help/instrument/serialreceive.html)

Using this blocks you can send and receive bytes in Arduino and interpret it as ASCII, floats, ints or whatever you want!! \o/. 

In this example we are sending and receiving binary data, more specifically, an Arduino `float` (4 bytes) or Simulink  `single`. Though, you can use this same code to send other types of variables. 


## Configuring your Serial
Well, first of all, you need to configure the Serial Port you are using for this communication. To do that, simply put the [Serial Configuration](https://www.mathworks.com/help/instrument/serialconfiguration.html) block anywhere you want in your Simulink project. 

![configserial](https://github.com/leomariga/Simulink-Arduino/blob/master/images/configurationblock.png)

Make sure your Arduino is connected to your computer and select its **Communication port** (COM15 in my case). If you don't know which communication port the Arduino is using, open your Arduino IDE > Tools > Ports. 

Next, select the **Baud rate** to communicate with Arduino. Some standard values are 9600 and 115200. This is usually configured in the `setup()` function in your Arduino code. The other parameters in the block you can **simply use the same as the image above** or change it if you know what you are doing.


## Sending data from Arduino to Simulink

**IMPORTANT: You need to configure your serial port BEFORE sending or receiving data, so remember to [use the configuration block](#configuring-your-serial) before continuing.**


### Simulink setup - Receive
Use the [Serial Receive block](https://www.mathworks.com/help/instrument/serialreceive.html) to receive serial data in your Simulink project. You need to configure this block to make the communication correctly. You can see how to configure this block in the figure below. 

![seriareceiveblock](https://github.com/leomariga/Simulink-Arduino/blob/master/images/serialreceiveblock.png)

#### Configuring you Serial Receive block:

* **Communication port:** Use the same one you configured in the step above.
* **Header:** Makes Simulink understand when the message is starting. This is not strictly necessary to your communication, but **I highly recommend using it** because avoids all types of synchronization issues (Simulink crashes after some time receiving data). In this example I used the byte 'A', but you can use whatever you want.
* **Terminator:** Same as the header, but indicates the package end. I recommend using the end-line '\n'.
* **Data size:** If you are sending only one `float` from Arduino, use `[1 1]` but you can change it to `[1 2]` or `[1 N]` where N is the number of `float` you are receiving from Serial. 
* **Data type** If you are sending `float` from Arduino, make sure to select `single` in Simulink, since both type of variables are intrinsically the same (floating point number with 4 bytes of length). 

**Remember:** The *Data type* and *Data size* are correlated, so if you set Data size to `[1 3]` using Data type `single` Simulink will expect to receive 3 * 4 bytes = 12 bytes every step.

* **Enable blocking mode:** Make sure to check this option.
* Select **Output last received value** in the selection box.
* **Block sample time:** Period in which the Arduino is sending data (20 Hz rate in my case).

Well done! You can now receive the Arduino data. The only step left is to convert it to the variable you want to work, in my case, a `double`.

### Serial receive final block diagram:

![seriareceivediagramblock](https://github.com/leomariga/Simulink-Arduino/blob/master/images/simulinkReceiveblocks.png)

## Arduino setup - Send
To send binary data from Arduino you have to convert your `float` to an array of bytes `uint8_t`. An interesting strategy is to use an *union* type like the code below:

```c++
typedef union{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

FLOATUNION_t myValue;
```

To attribute a value for your `float`, you simply call:

```c++
myValue.number = 1.2;
```

To send this number just call `Serial.write()` for each `myValue.bytes[i]`. **Remember to also send the header and the terminator**. The code below illustrates this process:

```c++
Serial.write('A'); 

for (int i=0; i<4; i++){
	Serial.write(myValue.bytes[i]); 
}

Serial.print('\n');
```

 You need to setup the Serial in the `setup()` function and create a loop with the same time delay as configured in Simulink. [A simple example can be found here](https://github.com/leomariga/Simulink-Arduino/blob/master/Examples/Sigle_signal/Simulink_receive_Arduino_send/arduinoSend/arduinoSend.ino), where the Arduino sends a sinoid which is plotted in a scope. 

![seriareceivesignal](https://github.com/leomariga/Simulink-Arduino/blob/master/images/receive_result.png)


## Sending data from Simulink to Arduino

**IMPORTANT: As for the receive block, you need to configure your serial port BEFORE sending data, so remember to [use the configuration block](#configuring-your-serial) before continuing.**


### Simulink setup - Send
To send data from Simulink create a project similar to the image below. 

![serialsendsimulink](https://github.com/leomariga/Simulink-Arduino/blob/master/images/simulinkSendblocks.png)


Block | Function
------------ | -------------
Step| Generate the signal you want to send.
Zero-Order Hold | Set the simulation send rate.
Single | Convert this signal to a `single` (4 bytes).
Byte Pack | Convert `single` signal to byte. Use Byte alignment 4 and Data Type uint32.
Serial Send | Send the bytes. You can add a Header and a Terminator if you want, but I had no problem sending this data without them.


### Arduino Setup - Receive
To receive a `float` in Arduino, first use the same *union* as in [Arduino Send code](#arduino-setup---send). Then, just create a function called `getFloat()` as follows:

```c++
float getFloat(){
    int cont = 0;
    FLOATUNION_t f;
    while (cont < 4 ){
        f.bytes[cont] = Serial.read() ;
        cont = cont +1;
    }
    return f.number;
}
```

Then, in your loop, just call 

```c++
FLOATUNION_t myValue;

myValue.number = getFloat();
```

If you are sending more then one variable, remember to call `getFloat()` as many time as the number of `floats` you are sending. [A receive example can be found here](https://github.com/leomariga/Simulink-Arduino/blob/master/Examples/Sigle_signal/Simulink_send_Arduino_receive/arduinoReceive/arduinoReceive.ino)


## Send and receive (Hardware in the loop)
Now you have both the send and receive blocks working you can join everything into a single project. The idea here is to create a simulation where the Arduino receive some data, process it, and sends back to Simulink. To do that, the Simulink block diagram is shown below:

![sendreceive](https://github.com/leomariga/Simulink-Arduino/blob/master/images/SendAndReceive.png)


The Arduino code consists in joining both receive and send codes. [A fully working example of this code can be found here.](https://github.com/leomariga/Simulink-Arduino/blob/master/Examples/Sigle_signal/Simulink_SendReceive_Arduino_SendReceive/arduinoSendReceive/arduinoSendReceive.ino). In this example, Simulink sends a signal which passes through Arduino and sends back to Simulink. 

## Array of floats
The blocks used in this simulation enables transmitting more then one variable in each simulation step. To do so, you have to make some minor adjustments in your simulation. The figure below shows this communication.

![sendreceivemultiples](https://github.com/leomariga/Simulink-Arduino/blob/master/images/simulinkMultipleSendReceive.png)

Notice that nothing has changed in the sender diagram. The block *byte pack* allow converting an array of `float` into an array of `bytes`. When the array arrives in the *Serial Send*, it concatenates the array in order and sends the bytes in the serial. 

In the receiver diagram, the main change is in the *Serial Receive* block. You have to set the number of floats the Arduino is sending in order receive the data correctly. The figure bellow illustrates how to configure the block to receive 3 floats. 

![sendreceivemultiples](https://github.com/leomariga/Simulink-Arduino/blob/master/images/multipleDataSize.png)

This example shows the Simulink transmitting 6 different values to Arduino, and receiving back 3 values (Sum of each pair). [The Arduino code is right here.](https://github.com/leomariga/Simulink-Arduino/blob/master/Examples/Sigle_signal/Simulink_SendReceive_Arduino_SendReceive/arduinoSendReceive/arduinoSendReceive.ino).

![outputmultiple](https://github.com/leomariga/Simulink-Arduino/blob/master/images/multipleoutput.png)

## Conclusion
Hope I could help you in your work. If you have questions just e-mail me!

leomariga@gmail.com
Leonardo Mariga

As the legend says:

> Struggle with your heart, not with your code. 

Thank you!
