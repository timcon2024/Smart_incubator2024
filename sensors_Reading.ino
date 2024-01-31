
/*
 * Функція перевірки стану і зчитування 
 * даних з сенсорів та портів
 */


void sensors_read()         {
  
 //читаємо дані з сенсорів формуємо структуру даних і флаги для подальшої їх опрацювання
  incubator_1.t1=(float)si7021.getTemperatureFromPreviousHumidityMeasurement(),1;
  
  
  incubator_1.h1=(float)si7021.measureHumidity(), 1;
  Serial.print(F("Humidity: ")); Serial.println(incubator_1.h1);
  // Serial.print (F("Температура округлена: " ));Serial.println (t);
  // Serial.print(F("Humidity:    ")); Serial.println(Incubatorincubator_1.h);
   
  incubator_1.tegg1= dsRead(0),2;
  incubator_1.tegg1=constrain(incubator_1.tegg1,1,60);
  Serial.print(F("Temperature egg: ")); Serial.println(incubator_1.tegg1);
  
 incubator_1.tw1= dsRead(1),2;
  Serial.print(F("Temperature water: ")); Serial.println(incubator_1.tw1);
  incubator_1.CO2=analogRead(SENSORGAZ_PIN);
  incubator_1.CO2 = constrain(incubator_1.CO2, 10, 3600);// рамки сенсора для уникнення фальшивого включення sensVal диапазоном от 10 до 150
   Serial.print(F("CO2: ")); Serial.println(incubator_1.CO2);
  delay(2000);
}
