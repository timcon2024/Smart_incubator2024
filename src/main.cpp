


#include <Arduino.h>
#include <main.h>
#pragma once
// put function declarations here:
void idetenficstion_moidel(); //функція присвоєння ідентифікатора для TIMCON
void GET_DateTime();// читаємо свій годинник
void parameter_control(); // контроль встановлених параметрів
void sensors_read(); // перевірка роботоздатності і зчитування показників з сенсорів
void read_Comand(); // команди передані по засобах зв'язку
void send_Lcd_Clock(); // виводимо час на LCD1602
void send_incubator_1I2C(); // відправка пакету даних на модуль контролю DEC
void ton(); // звуковий сигнал: налаштування, відкриті двері, відправка даних, прихід даних?
void Mane_menu(); // задаємо функцію для екрана доки не буде нажата кнопка з меню
void send_Radio_massege(int number);// відправка повідомлення-шифру на шлюз-модуль D
void sendAlertMessage(const char* message); // відправка по радіо каналу тривожних повідомлень
void sent_Radio_Data(); // код відправки структури даних по радіоканалу
void receive_I2C_message(); //запит на ESP8266 чи немає команди
void ton(); // функція озвучки подій
void check_power(); //функція обробки переривання INT4 (наявність 220 вольт)
void check_Stop_motor(); // функція обробки переривання INT5 (зупинка і ручне управління мотором перевороту і актуатором)
void check_Light(); // функція обробки переривання INT2 (ручного управління світлом)
void toggleMotor(); // функція ручного управління рухом мотора(актуатора)
void manualControl_Light(); // функція ручного управління світлом
void general_status(); // функція виведення усіх статусів і стані пам'яті
void scanI2CDevices(); // функція пошуку девайсів на шині I2C
void checkLCDData(); // функція перевірки чи записані дані символів для LCD1602? щоб не перезаписувати постійно
// включити:
//перевірку чи це перше включення
//отримати дані з годинника
//налаштувати всі сенсори
//прочитати з пам'яті і відновити стан потрів останнього запису
// віддалене і ручне управління світлом, моторами, дизенфекцією (лампою для кварцування повітря рециркуляцією)

void setup() {
  Serial.begin(9600);
  Wire.begin(); // запуск шини I2C
  scanI2CDevices(); // шукаємо і виводимо сенсори

  si7021.begin(); // si7021
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);                  // вибираємо потужність радіосигналу
  radio.setDataRate( RF24_1MBPS );                 // вибираємо швидкість радіопередачі
  radio.enableAckPayload();                       //дозволяємо відправку даних у відповіді на вхідний пакет на входящий  
  radio.openWritingPipe(0x6868686868LL);          // відкриваємо трубу для зв'язку
  radio.setChannel(115);                          //вибираємо канал для радіозв'язку (від 0 до 127)
  
  lcd.begin(0X27,16,2);                           // ініціалізуємо lcd екран
  lcd.backlight();                       // Підключаємо підсвітку екрана
  lcd.setCursor(0, 0); lcd.print(myText[0]);
  
  If(!flag_Clock)
  {GET_DateTime();// читаємо годинник
  send_Lcd_Clock();// виводимо дату і час з rtc ds1307
  idetenficstion_moidel(); //перевірка першого включення і присвоєння власного коду IDE
  } 
  
  delay(1000);
 // пишемо в память lcd символи
 checkLCDData();

 //налаштування портів для релейних модулів
  for (byte i = 34; i < 47; i++)
  {pinMode(i, OUTPUT);
  };           // усі піни з D34 по D46 робимо виходами
  
  pinMode(RELAY2_PIN, OUTPUT); // порт для ШИМ нагріву
  pinMode(RELAY3_PIN, OUTPUT);// порт для ШИМ міксу повітря
  
  pinMode(SENSOR220V_PIN, INPUT_PULLUP);     // (переривання )вхід для сигналу про наявність/відсутність електропостачання(для МК Otmega8, 328 - PD2),
  attachInterrupt(4, check_power, CHANGE);    //задаємо назву функції і умову її спрацювання (при зміні сигналу в обидва боки)
   
   pinMode(ButtonSTOP_MOTOR_PIN, INPUT_PULLUP); // порт преривання від кнопки ручного управління переворотом
  // Налаштування преривання для кнопки
  attachInterrupt(5, check_Stop_motor, FALLING);
  
   pinMode(ButtonLIGHT_PIN, INPUT_PULLUP); // порт преривання від кнопки ручного управління переворотом
  // Налаштування преривання для кнопки тла в інкубаторі
  attachInterrupt(2, check_Light, FALLING);



  myPID.SetOutputLimits(10, WindowSize);
  myPID.SetMode(AUTOMATIC);
  if (EEPROM.read(10) == 255 && EEPROM.read(4) == 255){
        EEPROM.write(10, 10);
        EEPROM.write(11, 0);
        EEPROM.write(12, 240);
        EEPROM.write(13, 65);
   }
   EEPROM.get(10, Setpoint);
   delay(200);
   Serial.println(F("INCUBATOR TIMСON...Start module."));
  void ton();
  void Mane_menu(); // задаємо функцію для екрана доки не буде нажата кнопка з меню
}



void loop() {
  // холостий цикл - попередній нагрів інкубатора
  if (!flag_Startincubation) {//інкубатор не запущено в роботу, а лише включили панель управління і здійснюється нагрів до температури яйця! (для уникнення конденсату)
      // читаємо дані з годинника реального часу
      GET_DateTime();
      //виводимо дані в серіал(щоб бачити по блютузу) і на LCD
      send_Lcd_Clock();
      //читаємо дані з сенсорів і виводимо в серіал-порт
      sensors_read();
      Serial.print(F("Temperature  incubator: ")); Serial.println(Incubator_1.t);
      lcd.clear();
      lcd.setCursor (0,0);
      lcd.print(myText[33]);lcd.print(Incubator_1.t);lcd.print(myText[19]); //виводимо температуру
      Serial.print(F("Humidity  incubator: ")); Serial.println(Incubator_1.h);
      lcd.setCursor (0,1);
      lcd.print(myText[15]);lcd.print(Incubator_1.h);lcd.print(myText[14]); //виводимо температуру
  
 }
 if (flag_Radio) { // якщо радіо не працює відправляємо повідомлення в ESP8266 по I2C
  Serial.println(warningMessage[0]); // виводимо в серіал для відладки
  Wire.beginTransmission(SLAVE_ADDRESS); // Адреса ESP8266 на шині I2C
        Wire.write(warningMessage[0]); // Передача повідомлення за вказаним індексом
        Wire.endTransmission();}
    // контролюємо параметр температури PID
 if (millis() - windowStartTime > WindowSize)
  {  Input=(float)si7021.getTemperatureFromPreviousHumidityMeasurement(),1;
  Serial.print(F("Input: "));Serial.println(Input);
  myPID.Compute();
  windowStartTime += WindowSize;
  }
  if (Output < millis() - windowStartTime)
  {digitalWrite(RELAY2_PIN, HIGH);
   
  }
  else {digitalWrite(RELAY2_PIN, LOW);}
 
 
 if (flag_power) {
  
   sendAlertMessage(warningMessage[13]); // Відправляємо тривожне повідомлення про відсутність електроенергії
   Serial.println(warningMessage[13]); // пишемо в серіал, що спрацювала відсутність світла
   ton(); // звукове сповіщення
 } 
 
 if(manualControl_overturning) {
  toggleMotor(); // включаємо ручний режим переворотом
 }
 // відправляємо дані по радіоканалу
  if  (millis() - timer1 >= PERIOD1)          // кожних 5 хв відправляємо дані по радіо і по I2C
     {
       if (!radio.isPVariant())
          {flag_Radio=true;
         }
        else { // якщо радіо працює то шлемо
      sensors_read(); // Отримуємо дані для передачі (наприклад, з датчиків або інших джерел)
    if(radio.write(&incubator_1, sizeof(incubator_1))); //відправляємо пакет структури даних
  Serial.println("Структура даних відправлена по радіоканалу");
 if (radio.isAckPayloadAvailable()) {             // чекаємо відповіді в пакеті...
  radio.read(&NRF_message,sizeof(NRF_message));   //... и имеем переменную message с числом ...0-256 от приемника.
  delay(75);
  Serial.print(myText[3]); Serial.println(NRF_message);
    }
      send_incubator_1I2C(); // відправляємо дані по шині на супутній модульESP8266
             do {
       timer1 += PERIOD1;
      if (timer1 < PERIOD1) break;  // переполнение uint32_t
     } while (timer1 < millis() - PERIOD1); // защита от пропуска шага
      }
     }
 if (Serial.available() > 0) // перевіряємо чи є команди передані по bluetooth
        {
    String bufString = Serial.readString();             // читаємо з порту як рядок символів
    byte dividerIndex = bufString.indexOf(';');           // шукаємо індекс кінця команди
    String buf = bufString.substring(0, dividerIndex);    // створюємо рядок чисел, від першого числадо індексу кінця комани строку с первым числом
    delay(10);
    bluetoothCommand=buf.toInt();                         // перетворюємо  (String  y int) команду в цифровукоманду
    delay(100);
    Serial.print(myText[50]); Serial.println(bluetoothCommand) ; // виводимо команду, для відладки 
    read_Comand();// запускаємо функцію перевірки на прихід нових команд
   }
// добавити перевірку шини І2С чи не прийшли команди выд модуля DEC_контролю
receive_I2C_message();
}

// інші функції
// обробка переривання світло
 void check_power()          {   //функція обробки першого переривання (відключення електроенергії
  flag_power = true;
}
// обробка переривання ручного управління переворотом
void check_Stop_motor()          {   //функція обробки першого переривання (відключення електроенергії
  Serial.println(warningMessage[15]); // пишемо в серіал, що спрацювала відсутність світла
  flag_manualControl_overturning = true;
}
// обробка периривання включення світла
void check_Light (){
  Serial.println(warningMessage[19]); // пишемо в серіал, що спрацювала відсутність світла
  flag_manual_light = true;
}
// ручне управління переворотом
void toggleMotor() {
  // Зупинка або відновлення руху мотора та зміна флагу ручного управління
  
  digitalWrite(motorRelay_PIN, !digitalRead(motorRelay_PIN));
  digitalWrite(actuatorRelay_PIN), !digitalRead(actuatorRelay_PIN);
  digitalWrite (reservReleMOTOR_PIN), !digitalRead(reservReleMOTOR_PIN);
  flag_manualControl_overturning = !flag_manualControl_overturning;
}
//ручне управління світлом
void manualControl_Light() {
  // Включення виключення світла в інкубаторі для ручного управління  
  digitalWrite(RELAY6_PIN, !digitalRead(RELAY6_PIN));
   flag_manual_light = !flag_manual_light;
}