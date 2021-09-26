# WiFi Controlled Bot

WiFi Controlled Bot - Moves the bot in desired direction at press of a button.

## Table of Contents

* [Demonstartions](#demonsrations)
* [About the Project](#about-the-project)
  * [Tech Stack](#tech-stack)
  * [File Structure](#file-structure)
  * [Data Processing](#data-processing)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
  * [Configuration](#configuration)
* [Usage](#usage)
* [Contributors](#contributors)
* [Acknowledgements and Resources](#acknowledgements-and-resources)
* [License](#license)


<!-- Demonstrations -->
## Demonstrations


![Controls](https://user-images.githubusercontent.com/84293091/120165747-81a55880-c219-11eb-8082-56b194670d9b.JPG)

![Bot](https://user-images.githubusercontent.com/84293091/120164530-4e15fe80-c218-11eb-8234-84aeb8ff666e.gif)
<!-- ABOUT THE PROJECT -->
## About the project
### Tech Stack
The Technologies used for this project are
* [FreeRTOS](https://www.freertos.org/openrtos.html)
* [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)

### File Structure
    .
    ├── Components                           # Contains files of specific library of functions or Hardware used
    │   ├──sra-board-component               # Library for motor drivers
    ├── frontend                             # Frontend file
    │   ├── index.htnl                       # Code for frontend 
    ├── main                                 # Source files (alternatively `lib` or `app`)
    │   ├──main.c                            # Main Source code to be executed
    |   ├──tuning_http_server.c              # Source code for http server
    |   ├──motion.c                          # Source code for motion of the bot
    |   ├──wifi_handler.c                    # Source code for handling wifi operations
    │   ├──kconfig.projbuild                 # defines the entries of the menu for configuration
    │   ├──CMakeLists.txt                    # contains commands to include the bluetooth library and main.c in esp-idf
    ├── CmakeLists.txt                       # contains commands to include Components and main folder while executing
    ├── LICENSE
    └── README.md 
 
### Data Processing:
* Data of button press is relayed from the frontend to the server(ESP32) using JSON.
```
JSON format:
{
   motion:parameter
}  
parameters: forward,backward,left,right
```
* The data fetched is then decoded using cJSON and hence ESP32 gets to kmow about the input provided by the user.

## Getting Started

### Prerequisites
Install ESP-IDF : https://github.com/espressif/esp-idf

### Installation
Clone the project
```
https://github.com/Jamm02/Wall-E-WiFi-Controlled-Bot.git

cd Wall-E-WiFi-Controlled-Bot
```
## Usage

Build
```
idf.py build
```
Flash
```
idf.py -p (PORT) flash monitor

```
### Configuration

```
idf.py menuconfig
```
* `Example Connection Configuration`
  * `WiFi SSID` - Set wifi SSID
  * `WiFi PASSWORD` - Set wifi Password
  

## Contributors
* [Moteen Shah](https://github.com/Jamm02)

## Acknowledgements and Resources
* [SRA VJTI](https://github.com/SRA-VJTI)
* [SRA Wall-E Workshop](https://github.com/SRA-VJTI/Wall-E_v2.2)
* Special thanks to [Gautam Agarwal](https://github.com/gautam-dev-maker)
* https://github.com/DaveGamble/cJSON
* https://github.com/espressif/esp-idf/tree/release/v4.2/examples/protocols/http_server
* https://youtu.be/44Kqyphvlxk


  
## License
The [License](https://github.com/Jamm02/Wall-E-WiFi-Controlled-Bot/blob/master/LICENSE) Used for this Project.
  
  
  
