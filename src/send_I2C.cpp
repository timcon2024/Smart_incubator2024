#pragma once
#include <main.h>

void scanI2CDevices() {
  byte error, address;
  int deviceCount = 0;
  bool foundSensor = false;
  bool foundClock = false;

  Serial.println("Сканування шини I2C...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Знайдено пристрої з адресами 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);

      if (address == 0x40) {
        Serial.println(" - На шині I2C знайдено сенсор - Si7021");
        foundSensor = true;
      } else if (address == 0x68) {
        Serial.println(" - На шині I2C знайдено годинник реального часу: RTC DS1307");
        foundClock = true;
      } else {
        Serial.println(" - Знайдено пристрій на шині I2C");
      }

      deviceCount++;
    }
  }

  if (deviceCount == 0) {
    Serial.println("На шині I2C відсутні пристрої.");
  } else {
    if (!foundSensor) {
      Serial.println("Sensor Si7021 не знайдено.");
    flag_sensor_Si7021=true;
    }
    if (!foundClock) {
      Serial.println("RTC DS1307 не знайдено.");
    flag_Clock=true;
    }
  }

  Serial.print("Знайдено: ");
  Serial.print(deviceCount);
  Serial.println(" приладів на шині I2C.");

}

void send_I2C_message(int number) {
  Wire.beginTransmission(SLAVE_ADDRESS); // Початок обміну з ESP8266
  Wire.write(number); // ми відправляємо число-шифр повідомлення
  uint8_t status = Wire.endTransmission(); // Завершення передачі та отримання статусу

  if (status == 0) {
    Serial.println(myText[10]);
  } else {
    Serial.println(myText[60]);
  }
}
// функція перевірки чи немає нової команди на ESP8266
int receive_I2C_message() {
  Wire.requestFrom(SLAVE_ADDRESS, sizeof(int)); // Запит на отримання даних від ESP8266

  if (Wire.available()) {
    int number = Wire.read(); // Зчитування переданого числа
    I2C_message = number;
  } else {
    Serial.println("Помилка при отриманні повідомлення");
   
  }
}
// Відправлення даних на ESP8266
  void sent_Str_I2c() {
    Wire.beginTransmission(0x02);  // Встановлення зв'язку з ESP8266 за адресою 0x02
    Wire.write((byte*)&Incubator_1, sizeof(Incubator_1));  // Відправлення структури Incubator_1
    Wire.endTransmission();  // Завершення передачі
   delay(1000);  // Затримка між передачами
}
