# Walker

Flexible program for arduino based walker with variable number of legs and joints

## Getting Started

The main program is contained in "walker/". You need to setup your walker in "mainfunctions.cpp".

### Prerequisites

Arduino IDE
To use the debug QT version (walker.pro) you'll need Qt Creator with MinGW on windows, any version will do

Hardware side, you'll need an Arduino board and a PCA9685 linked via I2C to the controller to run the servos.

- TODO - Toggle between multiple Gait load system, might require more RAM than the arduino's capacity (ESP8266 ...)

### Installing

- TODO - Explain default position for servo calibration

## Deployment

Classic deployment on an arduino or other special board

## Built With

* [Adafruit-PWM-Servo-Driver-Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library) - Adafruit PCA9685 PWM Servo Driver Library

## Contributing

Just pull the damn thing and do stuff with it. Credit or not i don't really care.

## Author

* **Enguerrand Saminadin**

## License

This project is licensed under the Unlicensed License - see the [LICENSE.md](LICENSE.md) file for details
