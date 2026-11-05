# IoT-Based Weather and Atmospheric Pressure Monitoring Station

An IoT-based weather monitoring system developed using **ESP32, DHT22, BMP180, Wokwi and ThingSpeak**. The system collects temperature, relative humidity and atmospheric pressure readings, then uploads the sensor data to the ThingSpeak cloud platform for remote monitoring and visualization.

This project is implemented using the Wokwi online simulator, allowing anyone to interact with virtual sensors and observe the resulting cloud data without requiring physical hardware.

## Live Project

| Resource | Link |
|---|---|
| Wokwi Simulation | [Run the Weather Monitoring System](https://wokwi.com/projects/479875277963187201) |
| ThingSpeak Dashboard | [View Live Cloud Data](https://thingspeak.mathworks.com/channels/3567943) |

**Try it yourself:** Start the Wokwi simulation, change the virtual sensor values and observe the updated readings on the ThingSpeak dashboard.

## Project Overview

Weather monitoring involves measuring environmental parameters such as temperature, humidity and atmospheric pressure.

This project demonstrates how an ESP32-based IoT system can acquire environmental data from sensors and transmit it to a cloud platform over Wi-Fi.

The ThingSpeak cloud platform stores the uploaded readings and visualizes them through graphs, making remote monitoring possible.

## Components and Technologies

| Component / Technology | Purpose |
|---|---|
| ESP32 DevKit | Main microcontroller |
| DHT22 | Temperature and humidity measurement |
| BMP180 | Atmospheric pressure measurement |
| Wokwi | Online hardware simulation |
| ThingSpeak | Cloud data storage and visualization |
| Arduino C++ | Firmware development |
| Wi-Fi | Wireless data transmission |

## System Architecture

```text
       DHT22 Sensor
     (Temperature &
        Humidity)
            |
            |
       BMP180 Sensor
        (Pressure)
            |
            v
      +-------------+
      |    ESP32    |
      |             |
      | Data Reading|
      | Processing  |
      +-------------+
            |
            v
          Wi-Fi
            |
            v
      +-------------+
      | ThingSpeak  |
      |    Cloud    |
      +-------------+
            |
            v
      +-------------+
      | Data Graphs |
      |Visualization|
      +-------------+
```

## Working Principle

1. The DHT22 sensor provides temperature and relative humidity readings.
2. The BMP180 sensor measures atmospheric pressure.
3. The ESP32 reads data from both sensors.
4. The ESP32 connects to the Wokwi virtual Wi-Fi network.
5. Sensor readings are transmitted to ThingSpeak using an HTTPS POST request.
6. ThingSpeak stores the received values and displays them through graphs.
7. Users can monitor the uploaded readings through the public ThingSpeak channel.

The firmware is configured to upload readings approximately every **16 seconds**.

## ThingSpeak Channel Configuration

The ThingSpeak channel contains three data fields:

| Channel Field | Parameter | Unit |
|---|---|---|
| Field 1 | Temperature | °C |
| Field 2 | Humidity | % |
| Field 3 | Atmospheric Pressure | hPa |

**Public dashboard:** [Weather Monitoring System – ThingSpeak](https://thingspeak.mathworks.com/channels/3567943)

## Circuit Connections

### ESP32 and DHT22

| DHT22 Pin | ESP32 Pin |
|---|---|
| VCC | 3V3 |
| SDA / DATA | GPIO 15 |
| GND | GND |

### ESP32 and BMP180

| BMP180 Pin | ESP32 Pin |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

## Repository Structure

```text
IoT-Weather-Monitoring-Station/
│
├── README.md
│
├── wokwi/
│   ├── sketch.ino
│   ├── diagram.json
│   └── libraries.txt
│
├── screenshots/
│   ├── wokwi-circuit.png
│   ├── serial-monitor.png
│   ├── thingspeak-dashboard.png
│   └── sensor-controls.png
│
└── docs/
    ├── project-report.pdf
    └── system-architecture.png
```

## How to Run the Project

1. Open the [Wokwi Simulation](https://wokwi.com/projects/479875277963187201).
2. Click the Play button to start the simulation.
3. Open the Serial Monitor to observe sensor readings and Wi-Fi connection status.
4. Use the DHT22 controls to change temperature and humidity values.
5. Use the BMP180 controls to change atmospheric pressure.
6. Open the [ThingSpeak Dashboard](https://thingspeak.mathworks.com/channels/3567943).
7. Observe the uploaded sensor readings and updated graphs.

**Note:** The virtual sensors generate simulated readings. They do not measure actual outdoor weather conditions.

## Features

- Temperature monitoring using DHT22.
- Relative humidity monitoring using DHT22.
- Atmospheric pressure monitoring using BMP180.
- ESP32-based sensor data acquisition.
- Wireless data transmission using Wi-Fi.
- Cloud-based data storage using ThingSpeak.
- Graphical visualization of uploaded sensor readings.
- Interactive virtual sensor testing through Wokwi.
- Basic atmospheric pressure trend indication based on consecutive readings.

## Limitations

- The project currently uses Wokwi simulation rather than physical hardware.
- Virtual sensor readings do not represent actual environmental measurements.
- Internet connectivity is provided through Wokwi's virtual Wi-Fi environment.
- Pressure trend analysis compares consecutive readings; it is not a weather forecasting model.
- The public demo ThingSpeak channel is shared. Multiple visitors may upload readings to the same channel.
- The public simulation source exposes its demo Write API Key, so the demo channel should not contain private or sensitive data.

## Future Improvements

- Implement the system using physical ESP32 hardware and sensors.
- Add additional environmental sensors.
- Introduce historical weather data analysis.
- Implement machine learning-based weather prediction.
- Add threshold-based alerts for unusual environmental conditions.
- Develop a dedicated web or mobile monitoring dashboard.

## Learning Outcomes

This project demonstrates practical concepts involving:

- Embedded systems programming.
- Sensor interfacing using I2C and digital communication.
- ESP32 Wi-Fi connectivity.
- HTTPS-based cloud communication.
- IoT architecture and cloud integration.
- Real-time data visualization.
- Hardware simulation using Wokwi.


## License

This project is available for educational and learning purposes. You may use and modify the source code with appropriate attribution.
