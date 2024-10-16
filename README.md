# IVT Rego6xx Control
A REST API server which provides control over IVT heatpumps, which uses a Rego6xx controller.
Its the successor project of [Rego6xxSrv](https://github.com/BlueAndi/Rego6xxSrv).

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://choosealicense.com/licenses/mit/)
[![Repo Status](https://www.repostatus.org/badges/latest/inactive.svg)](https://www.repostatus.org/#inactive)
[![Release](https://img.shields.io/github/release/BlueAndi/IVTRego6xxControl.svg)](https://github.com/BlueAndi/IVTRego6xxControl/releases)
[![PlatformIO CI](https://github.com/BlueAndi/IVTRego6xxControl/workflows/PlatformIO%20CI/badge.svg?branch=master)](https://github.com/BlueAndi/IVTRego6xxControl/actions?query=workflow%3A%22PlatformIO+CI%22)

* [IVT Rego6xx Control](#ivt-rego6xx-control)
* [Motivation](#motivation)
* [Supported Hardware](#supported-hardware)
* [Software](#software)
  * [IDE](#ide)
  * [Installation](#installation)
  * [Build Project](#build-project)
  * [Update of the device](#update-of-the-device)
    * [Update via OTA](#update-via-ota)
    * [Update via USB](#update-via-usb)
* [REST API](#rest-api)
  * [Used Libraries](#used-libraries)
* [Issues, Ideas And Bugs](#issues-ideas-and-bugs)
* [License](#license)
* [Contribution](#contribution)

# Motivation
Unfortunately my Junkers heatpump (original build by IVT) has no smart interface out of the box. Therefore the service interface of the build-in rego6xx controller shall be used to read sensor data or to modify temperature threshold and etc.
The protocol for the Rego6xx controller was original derived from http://rago600.sourceforge.net/.

# Supported Hardware

* [Olimex ESP32-POE](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE/open-source-hardware)

# Software

## IDE
The [PlatformIO IDE](https://platformio.org/platformio-ide) is used for the development. Its build on top of Microsoft Visual Studio Code.

## Installation
1. Install [VSCode](https://code.visualstudio.com/).
2. Install PlatformIO IDE according to this [HowTo](https://platformio.org/install/ide?install=vscode).
3. Close and start VSCode again.
4. Recommended is to take a look to the [quick-start guide](https://docs.platformio.org/en/latest/ide/vscode.html#quick-start).

## Build Project
1. Load workspace in VSCode.
2. Change to PlatformIO toolbar.
3. _Project Tasks -> Build All_ or via hotkey ctrl-alt-b

## Update of the device

### Update via OTA
TODO

### Update via USB
TODO

# REST API
TODO

## Used Libraries
| Library | Description | License |
| - | - | - |
| [Arduino](https://github.com/platformio/platform-espressif32) | ESP32 Arduino framework v2.x.x | Apache-2.0 |
| [pioarduino](https://github.com/pioarduino/platform-espressif32) | ESP32 Arduino framework v3.x.x | Apache-2.0 |
| [PlatformIO](https://platformio.org) | PlatformIO is a cross-platform, cross-architecture, multiple framework, professional tool for embedded systems engineers and for software developers who write applications for embedded products. | Apache-2.0 |
| [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) | Webserver | LGPL-2.1 |
| [AsyncTCPSock](https://github.com/yubox-node-org/AsyncTCPSock) | TCP library, Reimplementation of the API of me-no-dev/AsyncTCP using high-level BSD sockets | MIT |
| [ArduinoJson](https://arduinojson.org/) | JSON handling | MIT |
| [StreamUtils](https://github.com/bblanchon/ArduinoStreamUtils) | Stream utilities | MIT |
| [Bootstrap](https://getbootstrap.com/) | CSS Framework | MIT |
| [POPPER JS](https://popper.js.org/) | POPPER JS | MIT |
| [jQuery](https://jquery.com/) | Javascript librariy for DOM handling | MIT |
| [JSZip](https://github.com/Stuk/jszip) | A library for creating, reading and editing .zip files with JavaScript, with a lovely and simple API. | MIT |
| [JSZipUtils](https://github.com/Stuk/jszip-utils) | A collection of cross-browser utilities to go along with JSZip. | MIT |
| [FileSaver.js](https://github.com/eligrey/FileSaver.js) | FileSaver.js is the solution to saving files on the client-side. | MIT |

# Issues, Ideas And Bugs
If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/IVTRego6xxControl/issues) or if you are able and willing to fix it by yourself, clone the repository and create a pull request.

# License
The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
Consider the different licenses of the used third party libraries too!

# Contribution
Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the work by you, shall be licensed as above, without any
additional terms or conditions.
