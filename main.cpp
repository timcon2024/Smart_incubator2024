


#include <Arduino.h>
#pragma once
#include <main.h> 

void idetenficstion_moidel(); //функція присвоєння ідентифікатора для TIMCON
void GET_DateTime();// читаємо свій годинник
void parameter_control(); // контроль встановлених параметрів
void sensors_read(); // перевірка роботоздатності і зчитування показників з сенсорів
void read_Comand(); // команди передані по засобах зв'язку
void send_Lcd_Clock(); // виводимо час на LCD1602
void send_incubator_1I2C(); // відправка пакету даних на модуль контролю DEC
void ton(); // звуковий сигнал: налаштування, відкриті двері, відправка даних, прихід даних?
void Mane_menu(); // задаємо функцію для екрана доки не буде нажата кнопка з меню

// включити:
//перевірку чи це перше включення
//отримати дані з годинника
//налаштувати всі сенсори
//прочитати з пам'яті і відновити стан потрів останнього запису

void setup() {
Serial.begin(9600);
Wire.begin(); // ds18b20
si7021.begin(); // si7021
  
radio.begin();
radio.setPALevel(RF24_PA_MAX);                  // вибираємо потужність радіосигналу
radio.setDataRate( RF24_1MBPS );                 // вибираємо швидкість радіопередачі
radio.enableAckPayload();                       //дозволяємо відправку даних у відповіді на вхідний пакет на входящий  
radio.openWritingPipe(0x6868686868LL);          // відкриваємо трубу для зв'язку
radio.setChannel(115);                          //вибираємо канал для радіозв'язку (від 0 до 127)
  
lcd.begin();                           // ініціалізуємо lcd екран
lcd.backlight();                       // Підключаємо підсвітку екрана
lcd.setCursor(0, 0); lcd.print(myText[0]);
GET_DateTime();// читаємо годинник
send_Lcd_Clock();// виводимо дату і час з rtc ds1307
idetenficstion_moidel(); //перевірка першого включення і присвоєння власного коду IDE
    
  
  delay(1000);
 // пишемо в память lcd символи
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
 
  for (byte i = 4; i < 9; i++)
  {pinMode(i, OUTPUT);
  };           // усі піни з D4 по D8 робимо виходами
  
  pinMode(extendPin, OUTPUT); // порт для мотора-редуктора
  pinMode(retractPin, OUTPUT);// порт для штоку актуатора
    
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
if (!flag_Startincubation)//інкубатор не запущено в роботу, а лише включили панель управління і здійснюється нагрів до температури яйця! (для уникнення конденсату)
  {// читаємо дані з годинника реального часу
  GET_DateTime();
  //виводимо дані в серіал(щоб бачити по блютузу) і на LCD
  send_Lcd_Clock();
  //читаємо дані з сенсорів і виводимо в серіал-порт
  sensors_read();
  Serial.print(F("Temperature  incubator: ")); Serial.println(incubator_1.t);
lcd.clear();
lcd.setCursor (0,0);
lcd.print(myText[33]);lcd.print(incubator_1.t);lcd.print(myText[19]); //виводимо температуру

Serial.print(F("Humidity  incubator: ")); Serial.println(incubator_1.h);
lcd.setCursor (0,1);
lcd.print(myText[15]);lcd.print(incubator_1.h);lcd.print(myText[14]); //виводимо температуру
  
 }
 
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
 }
 //
 
 
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
// добавити блок відправки тривожних повідомлень
}
