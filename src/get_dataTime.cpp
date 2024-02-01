// перевірка роботи і отриманні часу з DS1307
#include<main.h>
#pragma once



/*
void GET_DateTime(){                    // отримуємо дані з годинника, дату і годину і хвилину
 
 Wire.beginTransmission(DS1307);  // начинаем обмен с DS1307
 Wire.write(byte(0x00));          // записуємо адрес регістра, з якого стартують дані дати і часу
 Wire.endTransmission();          // завершуємо передачу
 // byte dateTime[7];             // 7 байт для зберігання дати і часу з модуля
 int i = 0;                       // індекс даного елементу
 Wire.beginTransmission(DS1307);  // функція обміну з DS1307
 Wire.requestFrom(DS1307, 7);     // запит 7 байтів у DS1307

 while (Wire.available())         // цикл для передачі даних DS1307
  { dateTime[i] = Wire.read();    // читаем 1 байт и сохраняем в массив dateTime
    i += 1;
  }                       // инкрементуемo і додаємо індекс елемента масивуиндекс элемента массива
  
  Wire.endTransmission(); 
}
*/
void GET_DateTime() {
  Wire.beginTransmission(DS1307);  // початок обміну з DS1307
  Wire.write(byte(0x00));          // запис адреси регістра, з якого починаються дані дати і часу
  uint8_t status = Wire.endTransmission();  // завершення передачі та отримання статусу

  if (status == 0) {  // перевірка статусу зв'язку з DS1307
    int i = 0;                       // індекс для даного елементу
    Wire.requestFrom(DS1307, 7);     // запит 7 байтів з DS1307
     flag_Clock = 0;
    while (Wire.available()) {
      dateTime[i] = Wire.read();   // читання 1 байта та збереження в масив dateTime
      i++;
    }
  } else {
    flag_Clock = 1; // підняття флагу помилки у випадку відсутності відповіді від DS1307
  }
}