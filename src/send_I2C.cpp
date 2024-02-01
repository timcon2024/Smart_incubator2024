#pragma once
#include <main.h>

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
//якщо радіо не працює передаємо це на модульESP8266
