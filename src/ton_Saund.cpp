
#pragma once
#include <main.h>


void ton()      {
 

 if (flag_Startincubation=1) {
 // стартова мелодія
tone(tonePin,392, 350);
delay(350);
tone(tonePin,392, 350);
delay(350);
tone(tonePin,392, 350);
delay(350);
tone(tonePin,311, 250);
delay(250);
tone(tonePin,466, 100);
delay(100);
tone(tonePin,392, 350);

  tone(tonePin, 440, 250);
  delay(250);
  tone(tonePin, 165, 250);
  delay(250);
  tone(tonePin, 440, 250);
  delay(250);
  delay(250);
  
  tone(tonePin, 440, 250);
  delay(250);
  tone(tonePin, 165, 250);
  delay(250);
  tone(tonePin, 440, 250);
  delay(250);
  delay(250);
 tone(tonePin, 330, 125);
  delay(125);
  tone(tonePin, 294, 125);
  delay(125);
  tone(tonePin, 262, 125);
  delay(124);
  tone(tonePin, 494, 125);
  delay(125);
  tone(tonePin, 440, 125);
  delay(125);
  tone(tonePin, 494, 125);
  delay(125);
  tone(tonePin, 262, 125);
  delay(125);
  tone(tonePin, 294, 125);
  delay(125);
  
  tone(tonePin, 330, 250);
  delay(250);
  tone(tonePin, 165, 250);
  delay(250);
  tone(tonePin, 440, 250);
  delay(250);
 }
  else if (flag_power)
  {
    /* пікання довго 1 раз в циклі про відсутність 220% */
  tone(tonePin, 330, 1000);
  }
  
   else if (flag_sensor_Si7021) // поломка сенсора температури головного
  {
    /* пікання дуже швидке 3 рази в секуду в про поломку сенсора */
  tone(tonePin, 330, 330);
  delay(250);
   tone(tonePin, 330, 330);
  delay(250);
   tone(tonePin, 330, 330);
  delay(250);
   tone(tonePin, 330, 330);
  delay(250);
   tone(tonePin, 330, 330);
  delay(250);
  }
   else if (flag_Watter_Box) // відсутня вода в баці для зволоження
  {
    /* пікання середнє про відсутність води */
  tone(tonePin, 330, 530);
  delay(250);
   tone(tonePin, 330, 530);
  delay(250);
   tone(tonePin, 330, 530);
  delay(250);
  
  }
}