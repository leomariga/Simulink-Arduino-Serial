# Simulink and Arduino connection

Welcome! You are here because you are trying to connect your Arduino board to Simulink to transmit some data (maybe some sensor output) but everything is going wrong and your world is falling apart (drama). 

In this tutorial, we are **NOT** using the [official Simulink Arduino Package](https://www.mathworks.com/help/supportpkg/arduino/examples/getting-started-with-arduino-hardware.html), which enables you to acess directely Arduino pins and many other features. **What's the problem with this library?** It uses a specific Arduino code, so you can't customize the code inside the board (if you want to use the Arduino as a control device for a "Hardware in the loop" project, as example).

## The solution
To overcome the problem we can send and receive data using the serial blocks from Simulink without downloading any extra packages:

### Instrument Control Toolbox

* [Serial Configuration](https://www.mathworks.com/help/instrument/serialconfiguration.html)
* [Serial Send](https://www.mathworks.com/help/instrument/serialsend.html)
* [Serial Receive](https://www.mathworks.com/help/instrument/serialreceive.html)

Using this blocks you can send and receive bytes in Arduino and interpret it as ASCII, floats, ints or whatever you want!! \o/. 

In this example we are sending and receiving binary data, more specificaly, an Arduino `float` (4 bytes) or Simulink  `single`. Though, you can use this same code to send other types of variables. 


## Configuring your Serial
Well, first of all, you need to configure the Serial Port you are using for this communication. To do that, simply put the [Serial Configuration](https://www.mathworks.com/help/instrument/serialconfiguration.html) block anywhere you want in your Simulink project. 

![configserial](https://github.com/leomariga/Simulink-Arduino/blob/master/images/configurationblock.png)

Make sure your Arduino is connected to your computer and select its **Communication port** (COM15 in my case). If you don't know which communication port the Arduino is using, open your Arduino IDE > Tools > Ports. 

Next, select the **Baud rate** to communicate with Arduino. Some standard values are 9600 and 115200. This is usually configured in the `setup()` function in your Arduino code. The other parameters in the block you can **simply use the same as the image above** or change it if you know what you are doing.


## Sending data from Arduino to Simulink

**IMPORTANT: You need to configure your serial port BEFORE sending or receiving data, so remember to [use the configuration block](#configuring-your-serial) before continuing.**


### Simulink setup
Use the [Serial Receive block](https://www.mathworks.com/help/instrument/serialreceive.html) to receive serial data in your Simulink project. You need to configure this block to make the communication correctly. You can see how to configure this block in the figure below. 

![seriareceiveblock](https://github.com/leomariga/Simulink-Arduino/blob/master/images/serialreceiveblock.png)

#### Configuring you Serial Receive block:

* **Communication port:** Use the same one you configured in the step above.
* **Header:** Makes Simulink understand when the message is starting. This is not strictly necessary to your communication, but **I highly recommend using it** because avoids all types of synchronization issues (Simulink crashes after some time receiving data). In this example I used the byte 'A', but you can use whatever you want.
* **Terminator:** Same as the header, but indicates the package end. I recomend using the end-line '\n'.
* **Data size:** If you are sending only one `float` from Arduino, use `[1 1]` but you can change it to `[1 2]` or `[1 N]` where N is the number of `float` you are receiving from Serial. 
* **Data type** If you are sending `float` from Arduino, make sure to select `single` in Simulink, since both type of variables are intrinsicaly the same (floating point number with 4 bytes of length). 

**Remember:** The *Data type* and *Data size* are correlated, so if you set Data size to `[1 3]` using Data type `single` Simulink will expect to receive 3 * 4 bytes = 12 bytes every step.

* **Enable blocking mode:** Make sure to check this option.
* Select **Output last received value** in the selection box.
* **Block sample time:** Period in which the arduino is sending data (20 Hz rate in my case).

Well done! You can now receive the Arduino data. The only step left is convert it to the variable you want to work, in my case, a `double`.

Serial receive final block diagram:

![seriareceivediagramblock](https://github.com/leomariga/Simulink-Arduino/blob/master/images/serialreceiveblock.png)


## Sending data from Simulink to Arduino


<!-- ```python
import foobar

foobar.pluralize('word') # returns 'words'
foobar.pluralize('goose') # returns 'geese'
foobar.singularize('phenomena') # returns 'phenomenon'
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/) -->