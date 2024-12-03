#include <DHT.h>
#include <DHT_U.h>
#include "LiquidCrystal_I2C.h"
#include <Wire.h>
 
// Définir les broches
#define DHTPIN 9
#define DHTTYPE DHT11
#define FAN_PIN 3
#define HEATER_LED_PIN 4
#define LIGHT_LED_PIN 5
#define PUMP_PIN 6
#define PHOTORESISTOR_PIN A0
#define SOIL_MOISTURE_PIN A1
 
// Initialisation des capteurs et de l'écran LCD
DHT dht = DHT(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
// Variables pour la gestion de l'irrigation
unsigned long previousPumpMillis = 0;
const unsigned long pumpOnTime = 10000; // 10 secondes
const unsigned long pumpCycleTime = 60000/2; // 60 secondes
 
void setup() {
  Serial.begin(9600);
  // Initialiser les broches
  pinMode(FAN_PIN, OUTPUT);
  pinMode(HEATER_LED_PIN, OUTPUT);
  pinMode(LIGHT_LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
 
  digitalWrite(LIGHT_LED_PIN, HIGH); // Allumer la lumière
 
  // Initialisation des périphériques
  dht.begin();
  lcd.begin();
  //lcd.init();
  lcd.backlight();
  Serial.println("Config Done");
}
 
void loop() {
  //int chk = dht.read(DHTPIN);
  // Lecture des capteurs
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightLevel = analogRead(PHOTORESISTOR_PIN);
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
 
  //Print temp and humidity values to serlal monitor
  Serial.print("Humidite: ");
  Serial.print(humidity);
  Serial.print("%, Temperature: ");
  Serial.print(temperature);
  Serial.println(" Celsius");
  Serial.print("lightLevel ="); Serial.print(String(lightLevel*100/1023)+"\n"); Serial.print("soilMoisture ="); Serial.print(String((soilMoisture*100)/1023)+"\n");
  Serial.println("------------------------------------");
  delay(1000);
 
  // Affichage des données sur l'écran LCD
  lcd.clear();
 
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(int(temperature));
  lcd.print("C H: ");
  lcd.print(int(humidity));
  lcd.print("%");
 
  lcd.setCursor(0, 1);
  lcd.print("L: ");
  lcd.print(map(lightLevel, 0, 1023, 0, 100)); // Lumière en %
  lcd.print("% S: ");
  lcd.print(map(soilMoisture, 0, 1023, 0, 100)); // Humidité du sol en %
  lcd.print("%");
 
  // Maintien de la température
  if (temperature > 25) {
    digitalWrite(FAN_PIN, HIGH); // Activer le ventilateur pour expulser l'air chaud
    digitalWrite(HEATER_LED_PIN, LOW); // Éteindre la LED chauffante
  } else if (temperature < 18) {
    digitalWrite(FAN_PIN, LOW); // Désactiver le ventilateur
    digitalWrite(HEATER_LED_PIN, HIGH); // Allumer la LED chauffante
  } else {
    digitalWrite(FAN_PIN, LOW); // Température dans la plage idéale
    digitalWrite(HEATER_LED_PIN, LOW);
  }
 
  // Régulation de l'humidité
  if (humidity < 40) {
    // Activer le ventilateur pour faire entrer de l'air
    analogWrite(FAN_PIN, 128); // Vitesse modérée pour l'entrée d'air
  } else if (humidity > 60) {
    // Activer le ventilateur pour expulser l'air
    analogWrite(FAN_PIN, 255); // Vitesse maximale pour l'expulsion d'air
  }
 
  // Cycle jour/nuit
  if (lightLevel < 300) { // Seuil pour simuler l'obscurité
    digitalWrite(LIGHT_LED_PIN, HIGH); // Allumer la lumière
  } else {
    digitalWrite(LIGHT_LED_PIN, LOW); // Éteindre la lumière
  }
 
  // Irrigation automatique
  int soilMoisturePercent = map(soilMoisture, 0, 1023, 0, 100);
  unsigned long currentMillis = millis();
 
  if (soilMoisturePercent < 50) {
    if (currentMillis - previousPumpMillis >= pumpCycleTime) {
      digitalWrite(PUMP_PIN, HIGH); // Activer la pompe
      delay(pumpOnTime); // Maintenir la pompe activée pendant 10 secondes
      digitalWrite(PUMP_PIN, LOW); // Désactiver la pompe
      previousPumpMillis = currentMillis;
    }
  } else if (soilMoisturePercent >= 50 && soilMoisturePercent <= 70) {
    digitalWrite(PUMP_PIN, LOW); // Désactiver la pompe
  }
 
  delay(1000); // Fréquence d'échantillonnage de 1 Hz
}
 