# BluetoothClient

This project is an ESP32-based Bluetooth client that receives data over Bluetooth Serial, displays it on a TFT screen and changes led color. The project uses the `BluetoothSerial` library for Bluetooth communication and the `TFT_eSPI` library for displaying text on the screen.

## Features

- Connects to a Bluetooth device using Classic Bluetooth (SPP).
- Displays received key presses on a TFT screen.
- Changes the color of an RGB LED based on received key presses.
- Clears the screen and resets the message after a timeout.

## Hardware Configuration

- ESP32 WiFi Bluetooth development board with integrated 1.9-Inch LCD screen
- Keyes RGB LED module (KY-016 or similar)
- Jumper wires (Breadboard optional)

- RGB LED module is connected to the ESP32 as follows:
  - Red pin to GPIO 14
  - Green pin to GPIO 12
  - Blue pin to GPIO 13

## Software

This project was developed and built using the following:
- PlatformIO: Core `v.6.1.16`, Home `v.3.4.4`
- Platform: Espressif 32 `v.6.10.0`
- Python `v.3.12.1`

External libraries used:
- TFT_eSPI `v.2.5.34`
- ArduinoJson `latest (v.7.3.0)`

## Installation

1. Clone this repository to your local machine.
2. Open the project in PlatformIO.
3. Check configuration:
    - `platformio.ini`: If using different board than in the project, change `platfomio.ini` file to reflect your board
    - `User_Setup.h`: If using different board than in the project, change `User_Setup.h` and/or `User_Setup_Select.h` files found in `TFT_eSPI` library to reflect your lcd-screen driver and pins it is connected
4. Upload the code to your ESP32 board.

## Usage

1. Power on the ESP32 board.
2. Figure out the Bluetooth MAC address of your ESP32 device and in `keylogger.py` change it to reflect your ESP32 device. Otherwise the script will use the default MAC address of the ESP32 device used in development
3. Open terminal to the project and using `pip` install required Python libraries using the command `pip install -r requirements.txt`
4. Turn on your computer's bluetooth adapter and run `keylogger.py` using Python and your computer will connect to the ESP32 device
5. Now your computer will send every key press to the ESP32 device which will display them on the lcd-screen
6. To stop the `keylogger.py` script, simply press the esc-key
