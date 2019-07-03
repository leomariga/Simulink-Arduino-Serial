# Simulink and Arduino connection

Welcome! You are here because you are trying to connect your Arduino board to Simulink to transmit some variables (maybe some sensor output) but everything is going wrong and your world is falling apart. 

In this tutorial, we are **NOT** using the [official Simulink Arduino Package](https://www.mathworks.com/help/supportpkg/arduino/examples/getting-started-with-arduino-hardware.html), which enables you to acess directely Arduino pins and many other features. **What's the problem with this library?** It uses a specific Arduino code, so you can't customize the code inside the board (if you want to use the Arduino as a control device for a "Hardware in the loop" project, as example).

## The solution
To overcome the problem we can transmit the data using the serial blocks from Simulink without downloading any extra packages:

### Instrument Control Toolbox

* [Serial Configuration](https://www.mathworks.com/help/instrument/serialconfiguration.html)
* [Serial Send](https://www.mathworks.com/help/instrument/serialsend.html)
* [Serial Receive](https://www.mathworks.com/help/instrument/serialreceive.html)

```bash
pip install foobar
```

## Usage

```python
import foobar

foobar.pluralize('word') # returns 'words'
foobar.pluralize('goose') # returns 'geese'
foobar.singularize('phenomena') # returns 'phenomenon'
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)