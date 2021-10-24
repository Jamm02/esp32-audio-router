# esp32-audio-router

Stream audio using bluetooth.

## Table of Contents

- [esp32-audio-router](#esp32-audio-router)
  - [Table of Contents](#table-of-contents)
  - [Demonstrations](#demonstrations)
  - [About the project](#about-the-project)
    - [Tech Stack](#tech-stack)
    - [File Structure](#file-structure)
    - [Data Processing:](#data-processing)
    - [Program Flow:](#program-flow)
  - [A2DP-Sink:](#a2dp-sink)
  - [Example Output](#example-output)
  - [WM8960:](#wm8960)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
  - [Usage](#usage)
    - [Configuration](#configuration)
  - [Contributors](#contributors)
  - [Acknowledgements and Resources](#acknowledgements-and-resources)


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
    │   ├──WM8960                            # Library for WM8960
    |       |──CMakeLists.txt
    |       |──wm8960.c
    |   |──a2dp_sink                         # a2dp sink source codes 
    |       |──bt_app_core.c
    |       |──bt_app_av.c
    ├── frontend                             # Frontend file
    │   ├── index.html                       # Code for frontend 
    ├── main                                 # Source files (alternatively `lib` or `app`)
    │   ├──main.c                            # Main Source code to be executed
    |   ├──tuning_http_server.c              # Source code for http server
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
   data:parameter
}  

```
* The data fetched is then decoded using cJSON and hence ESP32 gets to kmow about the input provided by the user.
  

### Program Flow:

![Flowchart]


## A2DP-Sink:

## Example Output

After the program is started, the example starts inquiry scan and page scan, awaiting being discovered and connected. Other bluetooth devices such as smart phones can discover a device named as providef by webpage from the user. A smartphone or another ESP-IDF example of A2DP source can be used to connect to the local device.

Once A2DP connection is set up, there will be a notification message with the remote device's bluetooth MAC address like the following:


I (106427) BT_AV: A2DP connection state: Connected, [64:a2:f9:69:57:a4]


If a smartphone is used to connect to local device, starting to play music with an APP will result in the transmission of audio stream. The transmitting of audio stream will be visible in the application log including a count of audio data packets, like this:


I (120627) BT_AV: A2DP audio state: Started
I (122697) BT_AV: Audio packet count 100
I (124697) BT_AV: Audio packet count 200
I (126697) BT_AV: Audio packet count 300
I (128697) BT_AV: Audio packet count 400


## WM8960:


The WM8960 is a low power, high quality stereo CODEC
designed for portable digital audio applications.

Stereo class D speaker drivers provide 1W per channel into 8 ohm
loads with a 5V supply. Low leakage, excellent PSRR and
pop/click suppression mechanisms also allow direct battery
connection to the speaker supply. Flexible speaker boost
settings allow speaker output power to be maximised while
minimising other analogue supply currents.

![WM8960]

A highly flexible input configuration for up to three stereo
sources is integrated, with a complete microphone interface.
External component requirements are drastically reduced as no
separate microphone, speaker or headphone amplifiers are
required. Advanced on-chip digital signal processing performs
automatic level control for the microphone or line input.

Stereo 24-bit sigma-delta ADCs and DACs are used with low
power over-sampling digital interpolation and decimation filters
and a flexible digital audio interface.
The master clock can be input directly or generated internally by
an onboard PLL, supporting most commonly-used clocking
schemes.

The WM8960 operates at analogue supply voltages down to
2.7V, although the digital supplies can operate at voltages down
to 1.71V to save power. The speaker supply can operate at up
to 5.5V, providing 1W per channel into 8 ohm loads. Unused
functions can be disabled using software control to save power.
The WM8960 is supplied in a very small and thin 5x5mm QFN
package, ideal for use in hand-held and portable systems.



## Getting Started

### Prerequisites
Install ESP-IDF : https://github.com/espressif/esp-idf

### Installation
Clone the project
```
https://github.com/Jamm02/esp32-audio-router.git

cd esp32-audio-router
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
* [Chinmay Lonkar](https://github.com/ChinmayLonkar)
  
## Acknowledgements and Resources
* [SRA VJTI](https://github.com/SRA-VJTI)
* [SRA Wall-E Workshop](https://github.com/SRA-VJTI/Wall-E_v2.2)
* Special thanks to [Gautam Agarwal](https://github.com/gautam-dev-maker)
* https://github.com/DaveGamble/cJSON
* https://github.com/espressif/esp-idf/tree/release/v4.2/examples/protocols/http_server



  
  
  
  
