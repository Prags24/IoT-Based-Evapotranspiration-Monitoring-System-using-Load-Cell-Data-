# Load Cell-Based IoT System for Evapotranspiration Measurement

## Overview

This project, developed for CE738: Hydrometry at IIT Kanpur, designs and implements a robust, field-ready IoT system to measure evapotranspiration (ET) rates using a load cell. The system monitors weight loss in a potted Tulsi plant sample to quantify water loss due to evaporation and transpiration. Data is logged locally on an SD card with timestamps, environmental readings (temperature and humidity), and transmitted wirelessly via ESP8266 to a remote PostgreSQL database using MQTT. Calibration, outlier detection (MAD method), and uncertainty analysis ensure measurement reliability. The final ET rate is calculated as 2.04 ± 1.34 mm/day.

The system addresses key challenges in water resource management, climate impact assessment, and agricultural irrigation by providing real-time, accurate ET data. Last updated: 05:45 PM IST, Sunday, October 26, 2025.

## Features

- **Real-Time Monitoring**: Measures weight changes every 15 seconds using a load cell, with temperature and humidity from DHT22.
- **Data Logging**: Stores data in CSV/TXT format on SD card (up to 1.5 MB, ~11,463 readings over 105 hours).
- **Wireless Transmission**: ESP8266 sends data to MQTT broker for remote storage and analysis.
- **Calibration**: Wheatstone bridge setup and hysteresis curve for load cell accuracy.
- **Outlier Detection**: Median Absolute Deviation (MAD) for robust anomaly identification.
- **Uncertainty Analysis**: Quantifies errors from load cell sensitivity, temperature (26.5–29.5°C), humidity (50–60%), and human factors; combined uncertainty ~32.77%.
- **Power Management**: Rechargeable lithium battery with protection circuit for remote deployment.
- **Sample Setup**: Tulsi plant in 16.994 cm diameter pot (60 cm height).

## Hardware Requirements

- **Load Cell**: Converts force/weight to electrical signal (resistances: R1=924Ω, R2=739Ω, R3=739Ω, R4=591Ω).
- **HX711 Amplifier**: Amplifies and digitizes load cell output.
- **RTC DS3231**: Battery-backed real-time clock for timestamps.
- **SD Card Module + MicroSD Card**: Local data storage.
- **DHT22 Sensor**: Temperature and humidity monitoring.
- **ESP8266 WiFi Module**: Wireless data transmission.
- **Rechargeable Lithium Battery + Protection Circuit**: Power supply.
- **Arduino Uno**: Main microcontroller for data acquisition.
- **Enclosure**: Weatherproof housing for field use.

## Software Requirements

- **Arduino IDE**: For compiling and uploading code.
- **Libraries**:
  - `HX711.h`: Load cell interface.
  - `SPI.h`, `SD.h`: SD card handling.
  - `DHT.h`: DHT22 sensor.
  - `Wire.h`, `RTClib.h`: RTC communication.
  - `ESP8266WiFi.h`, `PubSubClient.h`: WiFi and MQTT.
- **MQTT Explorer**: For monitoring transmitted data.
- **PostgreSQL**: Remote database (IP: 172.26.213.93, Port: 1883).

## Installation and Setup

1. **Hardware Assembly**:
   - Connect load cell to HX711 (DOUT: A0, SCK: Pin 4).
   - Wire DHT22 to Pin 9.
   - Connect RTC via I2C (SDA/SCL to A4/A5).
   - SD card CS to Pin 2.
   - Power via rechargeable battery; ESP8266 for WiFi.
   - Calibrate load cell with known weights (0–5402 g) to derive equation: `WEIGHT = 0.0043 * RAW - 912.04`.

2. **Software Setup**:
   - Install Arduino IDE and required libraries via Library Manager.
   - Update WiFi credentials (`ssid`, `password`) and MQTT broker (`mqtt_server`, `mqtt_port`) in ESP8266 code.
   - Set initial RTC time if needed: `rtc.adjust(DateTime(2024, 11, 6, 22, 0, 0));` (uncomment in Arduino code).

3. **Upload Code**:
   - Upload Arduino code to Uno for data collection/logging.
   - Upload ESP8266 code separately (or via serial bridge) for transmission.
   - Ensure Arduino serial output feeds into ESP8266 RX.

4. **Testing**:
   - Power on; verify serial output (temperature, humidity, weight).
   - Insert SD card; check `Project3.txt` for logged data.
   - Connect to WiFi; monitor MQTT topic `CE738/Project3/Group5/data`.

## Usage

1. **Data Collection**:
   - Place Tulsi plant pot on load cell.
   - Run the system; it logs every 15 seconds (timestamp, temp, humidity, raw weight, measured weight).
   - For continuous runs, connect battery/ESP8266 to adapter.

2. **Data Retrieval**:
   - Eject SD card; analyze `Project3.txt` (e.g., in Excel/Python for mass vs. time plots).
   - View remote data via MQTT Explorer or query PostgreSQL database.

3. **Analysis**:
   - **Outlier Removal**: Apply MAD on weight data.
   - **ET Calculation**: Fit mass decrease: `m = -5.2 × 10^{-4} g/s`; ET rate `e = m / (πD²/4)`.
   - **Uncertainty**: Use provided model; expanded ET = (8.50 ± 5.58) × 10^{-2} mm/hr or 2.04 ± 1.34 mm/day.

Example Serial Output:




## Code Structure

### Arduino Code (`data_logger.ino`)
- **Setup**: Initializes scale (HX711), RTC, DHT22, SD card.
- **Loop**: Reads weight, temp/humidity; logs to serial and SD every 15s.
- **Calibration**: Embedded equation for weight conversion.
- **File Handling**: Writes to `Project3.txt` with timestamps.

### ESP8266 Code (`mqtt_transmitter.ino`)
- **Setup**: Connects to WiFi and MQTT broker.
- **Loop**: Reconnects if needed; publishes serial input to MQTT topic.
- **Bridge**: Reads Arduino serial data line-by-line for transmission.

Full code available in project files.

## Results

- **Data Period**: Nov 6, 10 PM – Nov 11, 7 AM (105 hours, 30s intervals).
- **Hysteresis Curve**: Linear fit for loading/unloading (R² ≈ 0.99).
- **Mass Decrease**: Linear regression y = -1.879x + 3082.3 (g vs. hours).
- **ET Rate**: 2.04 ± 1.34 mm/day (95% confidence; mass uncertainty dominates).
- **Plots**: Hysteresis, mass vs. time, temp/humidity scatter, daily mass trends.

| Parameter              | Value                  | Uncertainty                  |
|------------------------|------------------------|------------------------------|
| Average ET Rate (e)    | 2.36 × 10^{-6} g/cm²s  | ±7.804 × 10^{-7} g/cm²s      |
| Evapotranspiration (E) | 8.50 × 10^{-2} mm/hr   | ±5.58 × 10^{-2} mm/hr        |
| Daily ET               | 2.04 mm/day            | ±1.34 mm/day                 |

## Limitations

- **Environmental Sensitivity**: Temperature/humidity variations (26.5–29.5°C, 50–60% RH) introduce ~33% uncertainty.
- **Field Durability**: Battery life limits unattended runtime; fan/sunlight affects evaporation.
- **Scalability**: Single-plant setup; extend to lysimeters for larger scales.
- **Data Gaps**: Potential outliers from electrical noise or human error.

## Future Improvements

- Integrate solar charging for extended field deployment.
- Add soil moisture sensor for full water balance.
- Use machine learning for advanced outlier detection and ET prediction.
- Develop a cloud dashboard for real-time visualization.

## Authors

- Khushi Dhiman (241030050)
- Pragya Singh (241030061)
- Kritika Bansal (208070524)

Submitted for CE738: Hydrometry, IIT Kanpur, 2024.



## Acknowledgments

- Guidance from CE738 course instructors at IIT Kanpur.
- Support from lab facilities for hardware setup and testing.
