/* функція виведення статусу усіх флагів (сенсорів, релейних модулів та налаштувань), стан портів та дані, що перебувають в пам'яті МК 

*/

#include <main.h>
 #pragma once

 void general_status(){

Serial.println ("Діючі налаштування і стани:");
Serial.print ("   робочий стан інкубатора: ");
    if(!flag_Startincubation) //якщо не активовано
        {Serial.print ("холостий режим. - ");}
    else
         {Serial.print ("режим інкубування. - ");
         }
    Serial.println (flag_Startincubation);


Serial.print ("стан модуля RTC DS1307: ");
    if(!flag_Clock)//якщо не активовано
        {Serial.print ("підключено - .");
         }

    else
         {Serial.print ("не підключено - ");}

    Serial.println (flag_Clock);      


Serial.print ("стан радіомодуля NRF24L01: ");
    if(!flag_Radio)//якщо не активовано
        {Serial.print ("підключено - .");
         }

    else
         {Serial.print ("не підключено - ");}

    Serial.println (flag_Clock);  



Serial.println ("Статуси флагів портів:");


Serial.println ("Статуси флагів сенсорів:");


Serial.println ("Статуси флагів девайсів:");



 }

