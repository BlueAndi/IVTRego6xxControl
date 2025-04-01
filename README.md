# IVT Rego6xx Control <!-- omit in toc -->

A REST API server which provides control over IVT heatpumps, which uses a Rego6xx controller.
Its the successor project of [Rego6xxSrv](https://github.com/BlueAndi/Rego6xxSrv).

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://choosealicense.com/licenses/mit/)
[![Repo Status](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Release](https://img.shields.io/github/release/BlueAndi/IVTRego6xxControl.svg)](https://github.com/BlueAndi/IVTRego6xxControl/releases)

- [Motivation](#motivation)
- [Supported Hardware](#supported-hardware)
- [Software](#software)
  - [Installation](#installation)
  - [Build](#build)
  - [Flash to target and monitor](#flash-to-target-and-monitor)
  - [Usage](#usage)
- [Sensors](#sensors)
- [Binary Sensors](#binary-sensors)
- [Text Sensors](#text-sensors)
- [Buttons](#buttons)
- [API Endpoints and MQTT Topics](#api-endpoints-and-mqtt-topics)
- [SW-Architecture](#sw-architecture)
- [Details](#details)
  - [Command IDs](#command-ids)
  - [System Register Addresses](#system-register-addresses)
    - [Settings](#settings)
    - [Control Data](#control-data)
    - [Device Values](#device-values)
    - [Temperature Sensor Values](#temperature-sensor-values)
    - [Front Panel LED and Button Addresses](#front-panel-led-and-button-addresses)
- [Used Libraries](#used-libraries)
- [Issues, Ideas And Bugs](#issues-ideas-and-bugs)
- [License](#license)
- [Contribution](#contribution)

## Motivation

Unfortunately my Junkers heatpump (original build by IVT) has no smart interface out of the box. Therefore the service interface of the build-in rego6xx controller shall be used to read sensor data or to modify temperature threshold and etc.
The protocol for the Rego6xx controller was original derived from http://rago600.sourceforge.net/.

## Supported Hardware

- [Olimex ESP32-POE](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE/open-source-hardware)
- [Olimex ESP32-POE-ISO](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE-ISO/open-source-hardware)

## Software

ESPHome is used as the base of the software. The IVT reg6xx controller is implemented like a custom component.

### Installation

1. Install Python.
2. Setup [virtual environment](https://docs.python.org/3/library/venv.html).
    ```bash
    python -m venv .venv
    ```
3. Activate virtual environment.
   - Windows CMD: ```.venv\Scripts\activate.bat```
   - Windows PowerShell: ```.venv\Scripts\activate.ps1```
   - Linux: ```.venv/Scripts/activate```
4. Install packages.
    ```bash
    pip install -r requirements.txt
    ```

### Build

```bash
esphome compile IVTRego6xxCtrl.yaml
```

### Flash to target and monitor

```bash
esphome run IVTRego6xxCtrl.yaml
```

### Usage

## Sensors

| **Name**   | **Description**                | **Command ID** | **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** |
|------------|--------------------------------|----------------|------------------------------------------------|------------------------------------------------|
| **gt1**    | Radiator return temperature    | 0x02           | 0x0209                                         | 0x020B                                         |
| **gt2**    | Outdoor temperature            | 0x02           | 0x020A                                         | 0x020C                                         |
| **gt3**    | Hot water temperature          | 0x02           | 0x020B                                         | 0x020D                                         |
| **gt4**    | Forward temperature            | 0x02           | 0x020C                                         | 0x020E                                         |
| **gt5**    | Room temperature               | 0x02           | 0x020D                                         | 0x020F                                         |
| **gt6**    | Compressor temperature         | 0x02           | 0x020E                                         | 0x0210                                         |
| **gt8**    | Heat fluid out temperature     | 0x02           | 0x020F                                         | 0x0211                                         |
| **gt9**    | Heat fluid in temperature      | 0x02           | 0x0210                                         | 0x0212                                         |
| **gt10**   | Cold fluid in temperature      | 0x02           | 0x0211                                         | 0x0213                                         |
| **gt11**   | Cold fluid out temperature     | 0x02           | 0x0212                                         | 0x0214                                         |
| **gt3X**   | External hot water temperature | 0x02           | 0x0213                                         | 0x0215                                         |

## Binary Sensors

| **Name**   | **Description** | **Command ID** | **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** |
|------------|-----------------|----------------|------------------------------------------------|------------------------------------------------|
| **power**  | Power           | 0x00           | 0x0012                                         | 0x0012                                         |
| **pump**   | Pump            | 0x00           | 0x0013                                         | 0x0013                                         |
| **heating**| Heating         | 0x00           | 0x0014                                         | 0x0014                                         |
| **boiler** | Boiler          | 0x00           | 0x0015                                         | 0x0015                                         |
| **alarm**  | Alarm           | 0x00           | 0x0016                                         | 0x0016                                         |

## Text Sensors

| **Name**          | **Description** | **Command ID** | **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** |
|-------------------|-----------------|---------------|------------------------------------------------|------------------------------------------------|
| **display_row_1** | Display row 1   | 0x20          | 0x0000                                         | 0x0000                                         |
| **display_row_2** | Display row 2   | 0x20          | 0x0000                                         | 0x0000                                         |
| **display_row_3** | Display row 3   | 0x20          | 0x0000                                         | 0x0000                                         |
| **display_row_4** | Display row 4   | 0x20          | 0x0000                                         | 0x0000                                         |

## Buttons

| **Name**            | **Description**   | **Command ID** | **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** |
|---------------------|-------------------|----------------|------------------------------------------------|------------------------------------------------|
| **power_button**    | Power button      | 0x01           | 0x0008                                         | 0x0000                                         |
| **left_button**     | Left button       | 0x01           | 0x0009                                         | 0x0000                                         |
| **middle_button**   | Middle button     | 0x01           | 0x000A                                         | 0x0000                                         |
| **right_button**    | Right button      | 0x01           | 0x000B                                         | 0x0000                                         |
| **turn_wheel_left** | Turn wheel left   | 0x01           | 0x0044                                         | 0x01FF                                         |
| **turn_wheel_right**| Turn wheel right  | 0x01           | 0x0044                                         | 0x0001                                         |

## API Endpoints and MQTT Topics

| **Name**            | **Description**                | **REST API**                                   | **MQTT Topic**                                |
|---------------------|--------------------------------|------------------------------------------------|-----------------------------------------------|
| **gt1**             | Radiator return temperature    | `http://<IP-ADDRESS>/sensor/gt1`               | `heatpumpctrl/sensor/gt1/state`               |
| **gt2**             | Outdoor temperature            | `http://<IP-ADDRESS>/sensor/gt2`               | `heatpumpctrl/sensor/gt2/state`               |
| **gt3**             | Hot water temperature          | `http://<IP-ADDRESS>/sensor/gt3`               | `heatpumpctrl/sensor/gt3/state`               |
| **gt4**             | Forward temperature            | `http://<IP-ADDRESS>/sensor/gt4`               | `heatpumpctrl/sensor/gt4/state`               |
| **gt5**             | Room temperature               | `http://<IP-ADDRESS>/sensor/gt5`               | `heatpumpctrl/sensor/gt5/state`               |
| **gt6**             | Compressor temperature         | `http://<IP-ADDRESS>/sensor/gt6`               | `heatpumpctrl/sensor/gt6/state`               |
| **gt8**             | Heat fluid out temperature     | `http://<IP-ADDRESS>/sensor/gt8`               | `heatpumpctrl/sensor/gt8/state`               |
| **gt9**             | Heat fluid in temperature      | `http://<IP-ADDRESS>/sensor/gt9`               | `heatpumpctrl/sensor/gt9/state`               |
| **gt10**            | Cold fluid in temperature      | `http://<IP-ADDRESS>/sensor/gt10`              | `heatpumpctrl/sensor/gt10/state`              |
| **gt3X**            | External hot water temperature | `http://<IP-ADDRESS>/sensor/gt3x`              | `heatpumpctrl/sensor/gt3x/state`              |
| **power**           | Power                          | `http://<IP-ADDRESS>/binary_sensor/power`      | `heatpumpctrl/binary_sensor/power/state`      |
| **pump**            | Pump                           | `http://<IP-ADDRESS>/binary_sensor/pump`       | `heatpumpctrl/binary_sensor/pump/state`       |
| **heating**         | Heating                        | `http://<IP-ADDRESS>/binary_sensor/heating`    | `heatpumpctrl/binary_sensor/heating/state`    |
| **boiler**          | Boiler                         | `http://<IP-ADDRESS>/binary_sensor/boiler`     | `heatpumpctrl/binary_sensor/boiler/state`     |
| **alarm**           | Alarm                          | `http://<IP-ADDRESS>/binary_sensor/alarm`      | `heatpumpctrl/binary_sensor/alarm/state`      |
| **display_row_1**   | Display row 1                  | `http://<IP-ADDRESS>/text_sensor/display_row_1`| `heatpumpctrl/text_sensor/display_row_1/state`|
| **display_row_2**   | Display row 2                  | `http://<IP-ADDRESS>/text_sensor/display_row_2`| `heatpumpctrl/text_sensor/display_row_2/state`|
| **display_row_3**   | Display row 3                  | `http://<IP-ADDRESS>/text_sensor/display_row_3`| `heatpumpctrl/text_sensor/display_row_3/state`|
| **display_row_4**   | Display row 4                  | `http://<IP-ADDRESS>/text_sensor/display_row_4`| `heatpumpctrl/text_sensor/display_row_4/state`|
| **power_button**    | Power button                   | `http://<IP-ADDRESS>/button/power_button`      | `heatpumpctrl/button/power_button/state`      |
| **left_button**     | Left button                    | `http://<IP-ADDRESS>/button/left_button`       | `heatpumpctrl/button/left_button/state`       |
| **middle_button**   | Middle button                  | `http://<IP-ADDRESS>/button/middle_button`     | `heatpumpctrl/button/middle_button/state`     |
| **right_button**    | Right button                   | `http://<IP-ADDRESS>/button/right_button`      | `heatpumpctrl/button/right_button/state`      |
| **turn_wheel_left** | Turn wheel left                | `http://<IP-ADDRESS>/button/turn_wheel_left`   | `heatpumpctrl/button/turn_wheel_left/state`   |
| **turn_wheel_right**| Turn wheel right               | `http://<IP-ADDRESS>/button/turn_wheel_right`  | `heatpumpctrl/button/turn_wheel_right/state`  |

## SW-Architecture

![ClassDiagram](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/BlueAndi/IVTRego6xxControl/refs/heads/main/doc/sw-architecture/class_diagram.puml)

![Setup](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/BlueAndi/IVTRego6xxControl/refs/heads/main/doc/sw-architecture/setup.puml)

![Loop](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/BlueAndi/IVTRego6xxControl/refs/heads/main/doc/sw-architecture/loop.puml)

## Details

### Command IDs

| **Command ID** | **Description**                          |
|----------------|------------------------------------------|
| 0x00           | Read from front panel (register 0x093FF) |
| 0x01           | Write to front panel (register 0x09FF)   |
| 0x02           | Read system register (register 0x1345)   |
| 0x03           | Write system register (register 0x1345)  |
| 0x04           | Read timer register (register 0x1B45)    |
| 0x05           | Write timer register (register 0x1B45)   |
| 0x06           | Read register 1B61 (register 0x1B61)     |
| 0x07           | Write register 1B61 (register 0x1B61)    |
| 0x20           | Read display                             |
| 0x40           | Read last error line                     |
| 0x42           | Read previous error line                 |
| 0x7F           | Read REGO version                        |

### System Register Addresses

#### Settings

| **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** | **Description**                                |
|------------------------------------------------|------------------------------------------------|------------------------------------------------|
| 0x0000                                         | 0x0000                                         | Heat curve                                     |
| 0x0001                                         | 0x0001                                         | Heat curve fine adjustment                     |
| 0x0021                                         | 0x0021                                         | Indoor temperature settings                    |
| 0x0022                                         | 0x0022                                         | Curve influenced by indoor temperature         |
| 0x001E                                         | 0x001E                                         | Adjust curve at 20 degree outdoor temperature  |
| 0x001C                                         | 0x001C                                         | Adjust curve at 15 degree outdoor temperature  |
| 0x001A                                         | 0x001A                                         | Adjust curve at 10 degree outdoor temperature  |
| 0x0018                                         | 0x0018                                         | Adjust curve at 5 degree outdoor temperature   |
| 0x0016                                         | 0x0016                                         | Adjust curve at 0 degree outdoor temperature   |
| 0x0014                                         | 0x0014                                         | Adjust curve at -5 degree outdoor temperature  |
| 0x0012                                         | 0x0012                                         | Adjust curve at -10 degree outdoor temperature |
| 0x0010                                         | 0x0010                                         | Adjust curve at -15 degree outdoor temperature |
| 0x000E                                         | 0x000E                                         | Adjust curve at -20 degree outdoor temperature |
| 0x000C                                         | 0x000C                                         | Adjust curve at -25 degree outdoor temperature |
| 0x000A                                         | 0x000A                                         | Adjust curve at -30 degree outdoor temperature |
| 0x0008                                         | 0x0008                                         | Adjust curve at -35 degree outdoor temperature |
| 0x0002                                         | 0x0002                                         | Heat curve coupling difference                 |

#### Control Data

| **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** | **Description**                                |
|------------------------------------------------|------------------------------------------------|------------------------------------------------|
| 0x006E                                         | 0x006E                                         | GT1 target value                               |
| 0x006F                                         | 0x006F                                         | GT1 on value                                   |
| 0x0070                                         | 0x0070                                         | GT1 off value                                  |
| 0x002B                                         | 0x002B                                         | GT3 target value                               |
| 0x0073                                         | 0x0073                                         | GT3 on value                                   |
| 0x0074                                         | 0x0074                                         | GT3 off value                                  |
| 0x006D                                         | 0x006D                                         | GT4 target value                               |
| 0x006C                                         | 0x006C                                         | Additional heat power in percent               |

#### Device Values

| **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** | **Description**                                |
|------------------------------------------------|------------------------------------------------|------------------------------------------------|
| 0x01FD                                         | 0x01FF                                         | Ground loop pump P3                            |
| 0x01FE                                         | 0x0200                                         | Compressor                                     |
| 0x01FF                                         | 0x0201                                         | Additional heat 3KW                            |
| 0x0200                                         | 0x0202                                         | Additional heat 6KW                            |
| 0x0203                                         | 0x0205                                         | Radiator pump P1                               |
| 0x0204                                         | 0x0206                                         | Heat carrier pump P2                           |
| 0x0205                                         | 0x0207                                         | Tree-way valve VXV                             |
| 0x0206                                         | 0x0208                                         | Alarm                                          |

#### Temperature Sensor Values

| **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** | **Description**                                |
|------------------------------------------------|------------------------------------------------|------------------------------------------------|
| 0x0209                                         | 0x020B                                         | Radiator return GT1                            |
| 0x020A                                         | 0x020C                                         | Outdoor GT2                                    |
| 0x020B                                         | 0x020D                                         | Hot water GT3                                  |
| 0x020C                                         | 0x020E                                         | Forward GT4                                    |
| 0x020D                                         | 0x020F                                         | Room GT5                                       |
| 0x020E                                         | 0x0210                                         | Compressor GT6                                 |
| 0x020F                                         | 0x0211                                         | Heat fluid out GT8                             |
| 0x0210                                         | 0x0212                                         | Heat fluid in GT9                              |
| 0x0211                                         | 0x0213                                         | Cold fluid in GT10                             |
| 0x0212                                         | 0x0214                                         | Cold fluid out GT11                            |
| 0x0213                                         | 0x0215                                         | External hot water GT3X                        |

#### Front Panel LED and Button Addresses

| **Rego600-635**<br>**Address** | **Rego636-...**<br>**Address** | **Description**  |
|--------------------------------|--------------------------------|------------------|
| 0x0008                         | 0x0008                         | Power button     |
| 0x0009                         | 0x0009                         | Left button      |
| 0x000A                         | 0x000A                         | Middle button    |
| 0x000B                         | 0x000B                         | Right button     |
| 0x0012                         | 0x0012                         | Power LED        |
| 0x0013                         | 0x0013                         | Pump LED         |
| 0x0014                         | 0x0014                         | Heating LED      |
| 0x0015                         | 0x0015                         | Boiler LED       |
| 0x0016                         | 0x0016                         | Alarm LED        |
| 0x0044                         | 0x0044                         | Wheel            |

## Used Libraries

| Library | Description | License |
| - | - | - |
| [ESPHome](https://github.com/esphome/esphome) | ESPHome | MIT and GPLv3 |

## Issues, Ideas And Bugs

If you have further ideas or you found some bugs, great! Create a [issue](https://github.com/BlueAndi/IVTRego6xxControl/issues) or if you are able and willing to fix it by yourself, clone the repository and create a pull request.

## License

The whole source code is published under the [MIT license](http://choosealicense.com/licenses/mit/).
Consider the different licenses of the used third party libraries too!

## Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted for inclusion in the work by you, shall be licensed as above, without any
additional terms or conditions.
