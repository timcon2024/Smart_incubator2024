
/*
 * Функція перевірки стану і зчитування 
 * даних з сенсорів та портів
 */
bool checkSensorsPresence() {
  byte address[8];
  bool sensor1Present = false;
  bool sensor2Present = false;

  ds.reset_search(); // Починаємо пошук пристроїв на шині

  while (ds.search(address)) {
    if (OneWire::crc8(address, 7) == address[7]) {
      if (address[0] == 0x28) { // Перевіряємо тип пристрою (28 - тип DS18B20)
        if (!sensor1Present) {
          sensor1Present = true;
          Serial.println("Sensor 1 is present on the bus.");
        } else if (!sensor2Present) {
          sensor2Present = true;
          Serial.println("Sensor 2 is present on the bus.");
        }
      }
    }
  }

  if (!sensor1Present || !sensor2Present) {
    Serial.println("Not all sensors are present on the bus.");
    return false;
  }

  return true;
}

void sensors_read()         {
 if(!si7021.begin())
  {Serial.println("Did not find Si7021 sensor!");
  }
  else
  {
 //читаємо дані з сенсорів формуємо структуру даних і флаги для подальшої їх опрацювання
  incubator_1.t=(float)si7021.getTemperatureFromPreviousHumidityMeasurement(),1;
  incubator_1.h=(float)si7021.measureHumidity(), 1;
  
  Serial.print(F("Temperature: ")); Serial.println(incubator_1.t);
  Serial.print(F("Humidity: ")); Serial.println(incubator_1.h);
  }
 
  //перевірка наявності сенсорів на шині 
 checkSensorsPresence();
  
  // Зчитування температури з першого сенсора
  if (!checkSensorsPresence()) {Serial.println("Did not find DS18b20 sensor!"}
 
   else { 
    ds.reset();
  ds.select(ds.address(0)); // Вибір пристрою за його адресою
  ds.write(0x44); // Запускаємо конвертацію температури
  delay(750); // Затримка для завершення конвертації
  ds.reset();
  ds.select(ds.address(0));
  ds.write(0xBE); // Зчитування байтів з пам'яті сенсора
  temperature1 = ((ds.read() | ds.read() << 8) * 0.0625); // Розрахунок значення температури
 incubator_1.egg=temperature1;
  // Зчитування температури з другого сенсора
  ds.reset();
  ds.select(ds.address(1));
  ds.write(0x44);
  delay(750);
  ds.reset();
  ds.select(ds.address(1));
  ds.write(0xBE);
  temperature2 = ((ds.read() | ds.read() << 8) * 0.0625);
incubator_1.tw = temperature2;
  Serial.print("Temp. egg: ");
  Serial.println(incubator_1.egg);
  Serial.print("Temp. water ");
  Serial.println(incubator_1.tw);
  
   }
 
     
     
  incubator_1.CO2=analogRead(SENSORGAZ_PIN);
  incubator_1.CO2 = constrain(incubator_1.CO2, 10, 150);// рамки сенсора для уникнення фальшивого включення sensVal диапазоном от 10 до 150
   Serial.print(F("CO2: ")); Serial.println(incubator_1.CO2);
  delay(2000);
}


