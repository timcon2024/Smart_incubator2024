//команди передані по каналах зв'язку
#include <main.h>
#pragma once


void read_Comand() {
  
  if (((bluetoothCommand>= 1)&&(bluetoothCommand<=9))||((NRF_message>=1)&&(NRF_message<=9)))
      {Incubatordata.routing=(bluetoothCommand+NRF_message);// в структуру записуємо номер режиму інкубації
      EEPROM.put(201,Incubatordata.routing);
      delay(10);
      Serial.print(myText[20]);
      read_routine_feeding();
      }
  if ((bluetoothCommand == 11)||(NRF_message == 11)||(I2C_message==11))// якщо прийшла команда - 11 та реле не активоване то його включаємо 
      { if (digitalRead(RELAY1_PIN)==LOW)
      {digitalWrite(RELAY1_PIN, HIGH );// включаємо вентилятор для нагнітання вологості
      Serial.println(F("ON humidity-fan.")); 
      flag_RELAY1 == true;
      }
       else
      {digitalWrite(RELAY1_PIN, LOW);
      Serial.println(F("OFF humidity-fan."));
      flag_RELAY1 == false;}
     }

if ((bluetoothCommand == 12)||(NRF_message == 12)||(I2C_message==12))// якщо прийшла команда - 12 та реле не активоване то його включаємо реле 
      { if (digitalRead(RELAY2_PIN)==LOW)
      {digitalWrite(RELAY2_PIN, HIGH );// включаємо головний нагрівальний кабель інкубатора
      Serial.println(F("ON top-heating.")); 
      flag_RELAY2 == true;
      }
       else
      {digitalWrite(RELAY2_PIN, LOW);
      Serial.println(F("OFF top-heating."));//виключаємо головний нагрівальний кабель інкубатора
      flag_RELAY2 == false;}
     }

if ((bluetoothCommand == 13)||(NRF_message == 13)||(I2C_message==13))// якщо прийшла команда - 13 та реле не активоване то його включаємо 
      { if (digitalRead(RELAY3_PIN)==LOW)
      {digitalWrite(RELAY3_PIN, HIGH );// включаємо головний вентилятор перемішування повітря
      Serial.println(F("ON top-fan.")); 
      flag_RELAY3 == true;
      }
       else
      {digitalWrite(RELAY3_PIN, LOW);//виключаємо головний вентилятор перемішування повітря
      Serial.println(F("OFF top-fan."));
      flag_RELAY3 == false;}
     }
if ((bluetoothCommand == 14)||(NRF_message == 14)||(I2C_message==14))// якщо прийшла команда - 15 та реле не активоване то його включаємо 
      { if (digitalRead(RELAY4_PIN)==LOW)
      {digitalWrite(RELAY4_PIN, HIGH );// включаємо додатковий нагрів для пришвидшення нагріву
      Serial.println(F("ON second-heating.")); 
      flag_RELAY4 == true;
      }
       else
      {digitalWrite(RELAY4_PIN, LOW);// виключаємо додатковий нагрів для пришвидшення нагріву
      Serial.println(F("OFF second-heating."));
      flag_RELAY4 == false;}
     }
if ((bluetoothCommand == 15)||(NRF_message == 15)||(I2C_message==15))// якщо прийшла команда - 15 та реле не активоване то його включаємо 
      { if (digitalRead(RELAY5_PIN)==LOW)
      {digitalWrite(RELAY5_PIN, HIGH );// включаємо вентилятор провітрювання (охолодження)
      Serial.println(F("ON air-fan.")); 
      flag_RELAY5 == true;
      }
       else
      {digitalWrite(RELAY5_PIN, LOW);// виключаємо вентилятор провітрювання (охолодження)
      Serial.println(F("OFF air-fan."));
      flag_RELAY5 == false;}
     }

if ((bluetoothCommand == 16)||(NRF_message == 16)||(I2C_message==16))// якщо прийшла команда - 16 та реле не активоване то його включаємо 
      { if (digitalRead(RELAY6_PIN)==LOW)
      {digitalWrite(RELAY6_PIN, HIGH );// включаємо переворот (подаємо напругу на актуатор перевороту)
      Serial.println(F("ON actuator.")); 
      flag_RELAY6 == true;
      }
       else
      {digitalWrite(RELAY6_PIN, LOW);// виключаємо переворот (неподаємо напругу на актуатор перевороту)
      Serial.println(F("OFF actuator."));
      flag_RELAY6 == false;}
     }
}