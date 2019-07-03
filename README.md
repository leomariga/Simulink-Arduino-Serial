# Simulink and Arduino connection

Welcome! You are here because you are trying to connect your Arduino board to Simulink to transmit some data (maybe some sensor output) but everything is going wrong and your world is falling apart (drama). 

In this tutorial, we are **NOT** using the [official Simulink Arduino Package](https://www.mathworks.com/help/supportpkg/arduino/examples/getting-started-with-arduino-hardware.html), which enables you to acess directely Arduino pins and many other features. **What's the problem with this library?** It uses a specific Arduino code, so you can't customize the code inside the board (if you want to use the Arduino as a control device for a "Hardware in the loop" project, as example).

## The solution
To overcome the problem we can send and receive data using the serial blocks from Simulink without downloading any extra packages:

### Instrument Control Toolbox

* [Serial Configuration](https://www.mathworks.com/help/instrument/serialconfiguration.html)
* [Serial Send](https://www.mathworks.com/help/instrument/serialsend.html)
* [Serial Receive](https://www.mathworks.com/help/instrument/serialreceive.html)

Using this blocks you can send and receive bytes in Arduino and interpret it - you can send ASCII, floats, ints or whatever you want!! \o/. 

In this example we are sending and receiving binary data, more specificaly, an Arduino `float` (4 bytes) or Simulink  `single`. Though, you can use this same code to send other types of variables. 


## Configuring your Serial
Well, first of all, you need to configure the Serial Port you are using for this communication. To do that, simply put the [Serial Configuration](https://www.mathworks.com/help/instrument/serialconfiguration.html) block in your Simulink project. 

![alt text](https://github.com/leomariga/Simulink-Arduino/blob/master/images/configurationblock.png)

Make sure your Arduino is connected select its **communication port** (COM6 in my case), if you don't know which communication port the Arduino is connected, open your Arduino IDE > Tools > Ports. 


## Sending data from Arduino to Simulink


**IMPORTANT: You need to configure your Serial port BEFORE sending or receiving data, so remember to [use the configuration block](#configuring-your-serial) before continuing.**


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