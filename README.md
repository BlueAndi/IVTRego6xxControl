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
- [REST API](#rest-api)
  - [Sensor API Endpoints](#sensor-api-endpoints)
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
| **GT1**  | Radiator return temperature      | 0x0209                                         | 0x020B                                         |
| **GT2**  | Outdoor temperature              | 0x020A                                         | 0x020C                                         |
| **GT3**  | Hot water temperature            | 0x020B                                         | 0x020D                                         |
| **GT4**  | Forward temperature              | 0x020C                                         | 0x020E                                         |
| **GT5**  | Room temperature                 | 0x020D                                         | 0x020F                                         |
| **GT6**  | Compressor temperature           | 0x020E                                         | 0x0210                                         |
| **GT8**  | Heat fluid out temperature       | 0x020F                                         | 0x0211                                         |
| **GT9**  | Heat fluid in temperature        | 0x0210                                         | 0x0212                                         |
| **GT10** | Cold fluid in temperature        | 0x0211                                         | 0x0213                                         |
| **GT11** | Cold fluid out temperature       | 0x0212                                         | 0x0214                                         |
| **GT3X** | External hot water temperature   | 0x0213                                         | 0x0215                                         |

## REST API

### Sensor API Endpoints

The following table provides an overview of the REST API endpoints and MQTT topics for each sensor:

| **Name**   | **Description**                | **REST API**                                         | **MQTT Topic**                                      |
|------------|--------------------------------|------------------------------------------------------|-----------------------------------------------------|
| **GT1**    | Radiator return temperature    | `http://<IP-ADDRESS>/sensor/radiator_return_gt1`     | `heatpumpctrl/sensor/radiator_return_gt1/state`     |
| **GT2**    | Outdoor temperature            | `http://<IP-ADDRESS>/sensor/outdoor_gt2`             | `heatpumpctrl/sensor/outdoor_gt2/state`             |
| **GT3**    | Hot water temperature          | `http://<IP-ADDRESS>/sensor/hot_water_gt3`           | `heatpumpctrl/sensor/hot_water_gt3/state`           |
| **GT4**    | Forward temperature            | `http://<IP-ADDRESS>/sensor/forward_gt4`             | `heatpumpctrl/sensor/forward_gt4/state`             |
| **GT5**    | Room temperature               | `http://<IP-ADDRESS>/sensor/room_gt5`                | `heatpumpctrl/sensor/room_gt5/state`                |
| **GT6**    | Compressor temperature         | `http://<IP-ADDRESS>/sensor/compressor_gt6`          | `heatpumpctrl/sensor/compressor_gt6/state`          |
| **GT8**    | Heat fluid out temperature     | `http://<IP-ADDRESS>/sensor/heat_fluid_out_gt8`      | `heatpumpctrl/sensor/heat_fluid_out_gt8/state`      |
| **GT9**    | Heat fluid in temperature      | `http://<IP-ADDRESS>/sensor/heat_fluid_in_gt9`       | `heatpumpctrl/sensor/heat_fluid_in_gt9/state`       |
| **GT10**   | Cold fluid in temperature      | `http://<IP-ADDRESS>/sensor/cold_fluid_in_gt10`      | `heatpumpctrl/sensor/cold_fluid_in_gt10/state`      |
| **GT11**   | Cold fluid out temperature     | `http://<IP-ADDRESS>/sensor/cold_fluid_out_gt11`     | `heatpumpctrl/sensor/cold_fluid_out_gt11/state`     |
| **GT3X**   | External hot water temperature | `http://<IP-ADDRESS>/sensor/external_hot_water_gt3x` | `heatpumpctrl/sensor/external_hot_water_gt3x/state` |

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
