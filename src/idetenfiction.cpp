/*
 * функція створення унікального ідентифікаційного коду для кожного модуля timCon
 * генерується першим запуском модуля при умові налагодження годинника реального часу
 * якщо годинник не працює - код не буде генеровано
 * при поверненні до заводських налаштувань код зтирається з пам'яті і можна вважати модуль новим
 */

#include <main.h>
 void idetenficstion_moidel(){       

            
    if (((EEPROM.read(0))!=1)&&(!flag_Clock)) // якщо ячейка пам'яті 0 містить не "1", а годинник реального часу уже налаштований то це перше включення модуля  і запускаємо очищення всієї пам'яті
         { 
           for (int i = 1; i < 300; i++) // чистимо всі ячейки до 300 -ї 
                  { EEPROM.update(i, 0);}
                    //записуємо в пам'ять момент першого запуску модуля для генерації унікального номера
                    GET_DateTime();// читаємо годинник
                    EEPROM.update(300,bcdToDec(dateTime[1]));
                    EEPROM.update(301,bcdToDec(dateTime[2]));
                    EEPROM.update(302,bcdToDec(dateTime[4]));
                    EEPROM.update(303,bcdToDec(dateTime[5]));
                    EEPROM.update(304,bcdToDec(dateTime[6]));
                    EEPROM.update(0,1); // пишемо в ячейку 0 одиницю, щоб при перезагрузці не стерлись дані в пам'яті
                   };
               if (flag_Clock) // якщо годинник не налаштований
                  {Serial.println(F("Налаштуйте годинник ds1307!!!!"));
                 
                      lcd.setCursor(0, 1);
                      lcd.print(myText[37]);
                      delay(100);
                      lcd.setCursor(0, 1);
                      lcd.print("RTC no found"); 
                  }
                 
                 else
                 {     //виводимо серійний номер модуля
                       
                        byte m=EEPROM.read(300); 
                        byte H=EEPROM.read(301); 
                        byte D=EEPROM.read(302); 
                        byte M=EEPROM.read(303); 
                        byte Y=EEPROM.read(304); 
                        String IDE = String(Y) + "-" + String(M) + "-" + String(D) + " " + String(H) + ":" + String(m);

                        
                        Serial.print(myText[1]);Serial.println(IDE);
                        
                       lcd.clear();
                      lcd.setCursor(0, 1);
                      lcd.print("IDE:"); lcd.print(IDE);
                        }
         }
