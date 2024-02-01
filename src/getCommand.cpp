//команди передані по каналах зв'язку
#include <main.h>
#pragma once
#include "send_Radio.cpp"
#include "send_I2C.cpp"

void read_routine_feeding() {
   switch (Incubator_1.routing){
  case 1: //  режим утримання бройлера
          Serial.println(myText[33]);
     break;
  case 2:// параметри утримання несучок
         //lcd.print(myText[40]); 
         Serial.println(myText[40]);
    break;
  case 3:// параметри утримання качок
         //lcd.print(myText[35]);
         Serial.println(myText[35]);
    break;
  case 4:// параметри утримання  індиків
         //lcd.print(myText[34]);
        Serial.println(myText[34]);
    break;
  case 5:// параметри утримання гуси 
        //lcd.print(myText[39]);
        Serial.println(myText[39]);
    break;
  case 6:// параметри утримання  перепелів 
        //lcd.print(myText[36]);
        Serial.println(myText[36]);
    break;
  case 7:// параметри утримання цесарки
        //lcd.print(myText[38]);
        Serial.println(myText[38]);
    break;
  case 8:// параметри утримання фазан
        //lcd.print(myText[37]);
        Serial.println(myText[37]);
    break;
  case 9:// параметри утримання ручна
        //lcd.print(myText[41]);
        Serial.println(myText[41]);
    break;
   }

  if ((bluetoothCommand==00)&&(NRF_message>0)&&(I2C_message==0))
         //відправляємо повідомлення, що отримали команду і встановили режим
          send_Radio_message(NRF_message);
  
  if ((bluetoothCommand==00)&&(NRF_message==0)&&(I2C_message>0))
        //відправляємо повідомлення, що отримали команду і встановили режим
        send_I2C_message(I2C_message);


}

void read_Comand() {
  //команди
  //кліматична карта, режим інкубації
  if (((bluetoothCommand>= 1)&&(bluetoothCommand<=7))||((NRF_message>=1)&&(NRF_message<=7))||((I2C_message>=1)&&(I2C_message<=7)))
      {Incubator_1.routing = (bluetoothCommand+NRF_message+I2C_message);// в структуру записуємо номер режиму інкубації
      EEPROM.put(201,Incubator_1.routing);
      delay(10);
      Serial.print(myText[20]); // пишемо в серіал "Routine incubation: "
      read_routine_feeding();
      }
  
  if ((bluetoothCommand == 10)||(NRF_message == 10)||(I2C_message==10))// якщо прийшла команда - 10 включаємо робочий стан інкубатора 
     {
          flag_Startincubation=1;
          EEPROM.put(0,flag_Startincubation);
           Serial.println(myText[31]); // пишемо в серіал "Start timer deys!"
           //Serial.print(myText[8]); // пишемо в серіал "Routine incubation: "
           //read_routine_feeding();
             
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
if (bluetoothCommand==77)
{
  /* виводимо в серіал стан усіх сенсорів і систем */
  if (flag_Clock)
  {Serial.println(myText[1]);}
  else
  {Serial.println(myText[49]);}
  if (flag_Radio)
  { Serial.println(myText[0]);}
  else
  {Serial.println(myText[48]);}
  
 
}
if (NRF_message==77){
// відправляємо по радіо стан усіх систем
}
if(I2C_message==77) {
// відправляємо по I2C стан усіх систем
}


bluetoothCommand=0;
NRF_message=0;
I2C_message=0;

}