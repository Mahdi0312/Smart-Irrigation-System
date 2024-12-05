# Smart-Irrigation-System
## 1. Libraries and Initialization:
### Imported Libraries:
DHT.h and DHT_U.h: To read temperature and humidity using a DHT11 sensor.
LiquidCrystal_I2C.h and Wire.h: To control an LCD screen via I2C.
### Pin Definitions:
DHTPIN, FAN_PIN, HEATER_LED_PIN, etc., define the connections between hardware components and the Arduino board.
### Sensor and Device Initialization:
A DHT11 sensor (dht) and an LCD screen (lcd) are configured.
## 2. setup Function:
Initializes serial communication at 9600 baud for the serial monitor.
Configures pins as input or output.
Turns on the light LED.
Initializes peripherals like the DHT sensor and the LCD screen.
## 3. loop Function (Main):
The main loop continuously performs several tasks:

### Sensor Readings:
Reads temperature and humidity using the DHT11 sensor.
Measures ambient light with a photoresistor (PHOTORESISTOR_PIN).
Measures soil moisture using a soil moisture sensor (SOIL_MOISTURE_PIN).
### LCD Display:
Displays temperature, humidity, light level, and soil moisture as percentages on the screen.
## 4. Automatic Control:
### Temperature Control:
If the temperature is >25°C: Activates the fan (FAN_PIN) and deactivates the heating LED.
If the temperature is <18°C: Activates the heating LED and deactivates the fan.
If the temperature is ideal: Deactivates both.
### Humidity Control:
If the humidity is <40%: Activates the fan at moderate speed (to let air in).
If the humidity is >60%: Activates the fan at maximum speed (to expel air).
### Day/Night Simulation:
If the light level is low (<300): Turns on the light LED.
Otherwise: Turns off the light LED.
### Automatic Irrigation:
If soil moisture is <50%: Activates the water pump for 10 seconds every 30 seconds.
If soil moisture is between 50% and 70%: Turns off the pump.
## 5. Sampling Frequency:
Adds a 1-second delay between each loop iteration to stabilize sensor readings.
Overall Operation:
This program turns an Arduino into an automated greenhouse controller. It monitors and regulates:

Temperature (ventilation/heating).
Air humidity (ventilation).
Light levels (artificial lighting).
Soil moisture (irrigation).
