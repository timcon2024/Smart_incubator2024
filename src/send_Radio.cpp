/*
 *функція відправки структур даних і тривожних повідомлень
 *в системі TIMCON
 *
 *NRF24L01
 *- створюємо у кожному передавачі структуру даних
 *- створюємо у кожному передавачі String - тривожні повідомлення
 *- створюємо такод масив даних з портів 
 *
 *готуємо відправку
 *циклічно - через кожних 5 хвилин 
 *відправляємо структуру, якщо запущено роботу модуля в 
 *робочому режимі
 *
 *- у разі винекнення помилок чи проблем - 
 *відправляємо тривожні повідомлення 
 *
 *отримуємо у зворотньому пакеті команди у вигляді шифрованих чисел
  */
 #include <main.h>
 #pragma once
 //відправляємо дані типу int



void send_Radio_message(int number) {

  radio.begin(); // Ініціалізація модуля NRF24L01
  bool success = radio.write(&number, sizeof(number)); // Відправляємо число
  if (success) {
    Serial.println(myText[15]);
  } else {
    Serial.println(myText[16]);
  }
  radio.powerDown(); // Вимикаємо модуль NRF24L01
}

   
   

    // Передавати тривожні повідомлення за потреби
    
void sendAlertMessage(const char* message) {
  radio.begin(); // Ініціалізація модуля NRF24L01
  delay (50);
  // Відправляємо повідомлення
  bool result = radio.write(message, strlen(message) + 1);
  
  if (result) {
   Serial.print(myText[15]);
  } else {
   Serial.println(myText[16]);
  }
  
  radio.powerDown(); // Вимикаємо модуль NRF24L01
}

void sent_Radio_Data() {
  radio.powerUp();
  delay(75);
  
  if (!radio.isPVariant()) {
    flag_Radio = true;
  } else {
    flag_Radio = false;
  }
  
  byte dataBytes[sizeof(Incubator)];
  memcpy(dataBytes, &Incubator, sizeof(Incubator));
  
  if (radio.write(dataBytes, sizeof(Incubator))) {
    if (radio.isAckPayloadAvailable()) {
      radio.read(&NRF_message, sizeof(NRF_message));
      delay(75);
      Serial.print(myText[15]);
      Serial.println(NRF_message);
      radio.powerDown();
    }
  } else {
    Serial.println(myText[16]);
  }
}


 /*void sent_warningMessage (){
  if ((Incubator_1.t > 38.9)||(Incubator_1.t < 37.3))//якщо  температури в інкубаторі вище або нище норми)
  {
    //String warningMessage = warningMessage[9];
    // Відправляємо тривожне повідомлення через модуль NRF24L01
    radio.write(&warningMessage[9], sizeof(warningMessage[9]));

    Serial.println(myText[43]);
  }
  if ((Incubator_1.tegg >37.8)||(Incubator_1.tegg <36.5))//якщо температура яйця вище або нище)
  {
    //String warningMessage = "Warning!Temp tegg critical!";

    // Відправляємо тривожне повідомлення через модуль NRF24L01
    //radio.write(warningMessage.c_str(), warningMessage.length() + 1);
    radio.write(&warningMessage[3], sizeof(warningMessage[3]));
    Serial.println(myText[43]);
  }
  if ((Incubator_1.h > 77)||(Incubator_1.h < 40)) //якщо вологість нище або вище в інкубаторі)
  {
    //String warningMessage = "Warning!Hum critical!";

    // Відправляємо тривожне повідомлення через модуль NRF24L01
    //radio.write(warningMessage.c_str(), warningMessage.length() + 1);
  radio.write(&warningMessage[11], sizeof(warningMessage[11]));
   Serial.println(myText[43]);
  }
  if (Incubator_1.CO2 >500)//якщо перевищення рівня СО2 в інкубаторі)
  {
    //String warningMessage = "Warning!CO2 critical!";
    // Відправляємо тривожне повідомлення через модуль NRF24L01
    //radio.write(warningMessage.c_str(), warningMessage.length() + 1);
     radio.write(&warningMessage[12], sizeof(warningMessage[12]));
    
    Serial.println(myText[43]);
  }

  
      
  }
  */
  
