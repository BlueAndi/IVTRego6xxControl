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
- [API Endpoints and MQTT Topics](#api-endpoints-and-mqtt-topics)
- [SW-Architecture](#sw-architecture)
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

TODO

## Sensors

| **Name** | **Description**                  | **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** |
|----------|----------------------------------|------------------------------------------------|------------------------------------------------|
| **gt1**  | Radiator return temperature      | 0x0209                                         | 0x020B                                         |
| **gt2**  | Outdoor temperature              | 0x020A                                         | 0x020C                                         |
| **gt3**  | Hot water temperature            | 0x020B                                         | 0x020D                                         |
| **gt4**  | Forward temperature              | 0x020C                                         | 0x020E                                         |
| **gt5**  | Room temperature                 | 0x020D                                         | 0x020F                                         |
| **gt6**  | Compressor temperature           | 0x020E                                         | 0x0210                                         |
| **gt8**  | Heat fluid out temperature       | 0x020F                                         | 0x0211                                         |
| **gt9**  | Heat fluid in temperature        | 0x0210                                         | 0x0212                                         |
| **gt10** | Cold fluid in temperature        | 0x0211                                         | 0x0213                                         |
| **gt11** | Cold fluid out temperature       | 0x0212                                         | 0x0214                                         |
| **gt3X** | External hot water temperature   | 0x0213                                         | 0x0215                                         |

## Binary Sensors

| **Name**   | **Description** | **Rego600-635**<br>**System Register Address** | **Rego636-...**<br>**System Register Address** |
|------------|-----------------|------------------------------------------------|------------------------------------------------|
| **power**  | Power           | 0x0012                                         | 0x0012                                         |
| **pump**   | Pump            | 0x0013                                         | 0x0013                                         |
| **heating**| Heating         | 0x0014                                         | 0x0014                                         |
| **boiler** | Boiler          | 0x0015                                         | 0x0015                                         |
| **alarm**  | Alarm           | 0x0016                                         | 0x0016                                         |

## API Endpoints and MQTT Topics

The following table provides an overview of the REST API endpoints and MQTT topics for each sensor:

| **Name**    | **Description**                | **REST API**                               | **MQTT Topic**                            |
|-------------|--------------------------------|--------------------------------------------|-------------------------------------------|
| **gt1**     | Radiator return temperature    | `http://<IP-ADDRESS>/sensor/gt1`           | `heatpumpctrl/sensor/gt1/state`           |
| **gt2**     | Outdoor temperature            | `http://<IP-ADDRESS>/sensor/gt2`           | `heatpumpctrl/sensor/gt2/state`           |
| **gt3**     | Hot water temperature          | `http://<IP-ADDRESS>/sensor/gt3`           | `heatpumpctrl/sensor/gt3/state`           |
| **gt4**     | Forward temperature            | `http://<IP-ADDRESS>/sensor/gt4`           | `heatpumpctrl/sensor/gt4/state`           |
| **gt5**     | Room temperature               | `http://<IP-ADDRESS>/sensor/gt5`           | `heatpumpctrl/sensor/gt5/state`           |
| **gt6**     | Compressor temperature         | `http://<IP-ADDRESS>/sensor/gt6`           | `heatpumpctrl/sensor/gt6/state`           |
| **gt8**     | Heat fluid out temperature     | `http://<IP-ADDRESS>/sensor/gt8`           | `heatpumpctrl/sensor/gt8/state`           |
| **gt9**     | Heat fluid in temperature      | `http://<IP-ADDRESS>/sensor/gt9`           | `heatpumpctrl/sensor/gt9/state`           |
| **gt10**    | Cold fluid in temperature      | `http://<IP-ADDRESS>/sensor/gt10`          | `heatpumpctrl/sensor/gt10/state`          |
| **gt3X**    | External hot water temperature | `http://<IP-ADDRESS>/sensor/gt3x`          | `heatpumpctrl/sensor/gt3x/state`          |
| **power**   | Power                          | `http://<IP-ADDRESS>/binary_sensor/power`  | `heatpumpctrl/binary_sensor/power/state`  |
| **pump**    | Pump                           | `http://<IP-ADDRESS>/binary_sensor/pump`   | `heatpumpctrl/binary_sensor/pump/state`   |
| **heating** | Heating                        | `http://<IP-ADDRESS>/binary_sensor/heating`| `heatpumpctrl/binary_sensor/heating/state`|
| **boiler**  | Boiler                         | `http://<IP-ADDRESS>/binary_sensor/boiler` | `heatpumpctrl/binary_sensor/boiler/state` |
| **alarm**   | Alarm                          | `http://<IP-ADDRESS>/binary_sensor/alarm`  | `heatpumpctrl/binary_sensor/alarm/state`  |

## SW-Architecture

![ClassDiagram](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/BlueAndi/IVTRego6xxControl/refs/heads/main/doc/sw-architecture/class_diagram.puml)

![Setup](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/BlueAndi/IVTRego6xxControl/refs/heads/main/doc/sw-architecture/setup.puml)

![Loop](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/BlueAndi/IVTRego6xxControl/refs/heads/main/doc/sw-architecture/loop.puml)

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
