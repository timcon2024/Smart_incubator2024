#include <Wire.h>
const byte SLAVE_ADDRESS = 0x02;    //  адреса  ESP8266 модуля (slive)в шині І2С  

#include <EEPROM.h>
#include "Arduino.h"

#include <Si7021.h>
Si7021 si7021;

#include <PID_v1.h>
double Setpoint, Input, Output; // змінні для PID регулювання
double Kp=2, Ki=5, Kd=1.5; // PID коефіцієнти
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
#define TEMP_MAX 38.8           // межі PID регулювання термостата 
#define TEMP_MIN 37.5         // межі PID регулювання термостата 
int WindowSize = 5000;        // вікно регулювання
unsigned long windowStartTime; // змінна вікна регулювання

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9, 10);                   // створюємо об'єкт з назваю-( "radio") NRF24L01 і порти підключення CE(9) і SCN(10) радіомодуля, крім того підключаємо SCK до 13, MO/MOSI до 11, MI/MISO до 12,
const uint64_t pipe2=0x6868686868LL; // адреса труби
int flag_Radio;
int flag_Startincubation;

#define DS1307 0x68                 // I2C адрес таймера DS1307
int flag_Clock; // флаг роботи годинника реального часу 0-працює 1 - не працює
byte bcdToDec(byte val){ return ( (val/16*10) + (val%16) );}//переводимо в просте десяткове число !byte
byte decToBcd(byte val){return ( (val/10*16) + (val%10) );}// переводимо в двоїчно-десяткове

byte flag_power; // флаг наявності (відсутності мережі 220)

byte minute, hour, dayOfMonth, month, year;
byte dateTime[7];                   // 7 байт для зберігання дати і часу з модуля

int readKey, // кнопка реле=analogRead(A6);
 clockPos, 
 menuPage,
 activeButton, 
 button,
 cursorPosition,
 //sensor1,
 //sensor2,
 //sensor3,
 q,
 lastYear,
 bluetoothCommand=0, //передана на модуль команда по блютуз
 NRF_message=0,//передана на модуль команда по радіо
 I2C_message=0,//передана на модуль команда по І2С
 
 gazmax,all_feeding, correctDey;

float Tmax,Tmin,Hmin,Hmax,twmax,twmin;
 String menuItems[] = {"Clock", "Start module", "Parametrs", "Device Timers",};
                       // налаштування годинника, встановлення таймеру нагадування, встановлення параметрів контролю, 
                       //встановлення таймерів роботи додаткового обладнання, "переведення сенсорів їх коректування)
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);  //Переменная для кнопок навигации
 
#include <OneWire.h>                // бібліотека для роботи з протоколом OneWire 
OneWire ds(14);                     // порт (А0=D14) сенсорів DS18b20
//#define PERIOD2 123000
uint32_t timer1 = 0;
uint32_t timer2 = 0;              // таймер 

#include <LiquidCrystal_I2C.h>        // бібліотека по роботі з lcd дисплеєм на шині 12С
LiquidCrystal_I2C lcd(0x27,16,2);     // I2C адресa LCD (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой 
#define PERIOD1 63000               // 1000*60*2період для виконання функції виведення даних на LCD 1602 і в Serial

#define SENSOR220V_PIN 2             // порт D2 - порт переривання наявність 220 вольт
#define tonePin 3                   // порт 3 - бузер
#define RELAY1_PIN 4                // порт D4  r1 - сигнальний провід до реле (5/220) (твердотільне) (в ванночці розпилючач або вентилятор-кулер)
#define RELAY2_PIN 5                //ШИМ// порт D5  r2 - сигнальний провід до реле(5/220) (твердотільне) (PID) - головний нагріваючий прилад (шнур 60 Om) шим опалення: тепловентилятор, калорифер або нагрів повітря карбоновий шнур
#define RELAY3_PIN 6                //ШИМ// порт D6  r3 - сигнальний провід до реле(5/220) (твердотільне) (ШИМ) - головний вентилятор змішування повітря шим циркуляція повітря: вентилятор для перемішування повітря (циркуляція)    
#define RELAY4_PIN 7                // порт D7  r4 - сигнальний провід до реле(5/220) (електромеханічне) прямий нагрів (для розігріву) додаткове опалення: тен або лампи накалювання(в т.ч. як страховка) розігріву інкубатора
#define RELAY5_PIN 8                // порт D8  r5 -  сигнальний провід до реле(5/220) (електромеханічне) витяжного вентилятора провітрювання і охолодження (скидання жари) вентилятори швидкого охородження
#define Water_level A1              // порт A1 - рівень води в зволожувачі аналоговий сигнал для шкали у відсотках       

#define RELAY6_PIN 16               //  порт A2-D16 - сигнальний провід до реле(5/220) порт для актуатора 
#define RELAY7_PIN 17               // порт A3-D16 сигнальний провід до реле(5/220) порт для мотора лотка  

volatile boolean flag_power,flag_Watter_Box,//флаги роботи електрики,  наявності води у зволожувачі
 flag_sensor_Si7021, // флаг наявності і справності сенсора Si7021
  flag_sensor_eggs,// флаг наявності і справності сенсора DS18b20 на яйці
  flag_sensorWatterBox,// флаг наявності і справності сенсора DS18b20 в ванні нагнітання вологості
  flag_sensor_module, // флаг наявності і справності сенсора DS18b20 на модулі реального часу DS1307 
  flag_RELAY1, flag_RELAY2,flag_RELAY3,flag_RELAY4,flag_RELAY5,flag_RELAY6; //флаги релейних модулів
unsigned long timer_interval_incubator; // перерва між поданням на реле актуатор (мотор) сигналу включення подачі електроенергії 
const char* warningMessage[]   =  {     // тривожні повідомлення
 "ClockRTC1307 OFF",         //   1       30
  "Sensor Si7021 OFF! ",      //   2       30 
  "Sensor eggs OFF! ",        //   3       30
  "Sensor modul OFF",         //   4       32
  "Sensor waterTenck OFF",    //   5       30
  "NO water in Tank",         //   6       32
  "Sensor СО2 OFF",           //   7       30
  "Warning! Door open! ",     //   8       30
  "Warning! Temp critical!",  //   9       30
  "Warning! Temp egg critical!", //   10    30
  "Warning! Hum critical!",   //   11       30
  "Warning! CO2 critical!",   //   12       30
  "Warning! 220! OFF!",       //   13       30
  "Warning! tmodule critical!" // 14       20
  
 };
const char* myText[]   =  {               // об'являємо масив рядків англійських команд-повідомлень
                                                  //   номер. байт.
  "RadioNRF24L01 OFF",         //  0       32
  "ClockRTC1307 OFF",         //   1       30
  "Sensor DHT OFF! ",         //   2       30 
  "Temp: ",                   //   3       30
  "Sensor modul OFF",         //   4      32
  "Sensor Water OFF",         //   5       30
  "NO water in Tank",         //   6      32
  "NO food in Feeder",         //  7      30
  "Warning! ALARM! ",         //   8       30
  "Warning! 220! ",           //   9       30
  "I2C sent: ",              //   10      28
  " TIM&Co.UA.2021 ",         //  11
  "IDE:" ,                    //  12
  "No init pcf8574 ",        //   13
  "Not connected..",          //  14  
  "Radiomes sent:",           //  15
  "Radiomes no sent",         //  16
  "Control devaises:",        //  17
  "TIMER dey open. ",         //  18
  "TIMER dey closed." ,       //  19
  "Routine incubation: ",        //  20
  "Feeding dey: ",            //  21
  "END for deys :" ,          //  22
  "Number eggs: ",          //  23
  "All dey incubation: ",        //  24
  "Current settings: ",       //  25 
  "%HR;",                     //  26
  "Hum: " ,                   //  27
  "Warning! tmodule: ",       //  28
  "END settings ..../",       //  29
  "SAVE settings...." ,       //  30
  "Start timer deys!" ,       //  31
 "C;",                        //  32
"Brooler ",                   //  33
"Tyrken  ",                   //  34
"Duck    ",                   //  35
"Quail   ",                   //  36
"Pheasan ",                   //  37
"Numida  ",                   //  38
"Anser   ",                   //  39
"Chicken  ",                  //  40
"Manual  ",                   //  41
"Restart module...",           // 42
"Warning! MOTOR!",            //  43
"Warning! Vaccination!",      //  44
"t water: ",                    //45
"t module: ",                   //46
"Temp sensors: ",               //47
"RadioNRF24L01 OK!:",           //48
"ClockRTC1307 OK!:",            //49
"bluetoothCommand:",            //50
"                ",             //51
"\1",                           //52
"   Thank you!,   ",            //53
"Tmax:",                        //54
"Tmin:",                        //55
"Hmax:",                        //56
"Hmin:",                        // 57
"twmax",                        //58
"gzmax",                        //59
"I2C no sent!"                  //60
};
struct Incubator {  //27 байти           //структура для передачі даних з курника на центропульт
  float t;          // температура
  float h;          // вологість
  float tegg;       // температура яйця
  int tw;           // температура води
  int CO2;          // рівеньCO2
  int routing;      // режим утримання
  int dey;          // к-сть днів утримання
  int number;       // к-сть штук
  byte w;           // наявність води в баці з водою
  byte v;           //стан мережі 220 Вольт
  byte r1;        //стан реле основного вентилятора-перемішувача курника
  byte r2;        // стан реле основного обігріву 
  byte r3;        // стан реле додаткового обігріву
  byte r4;        // стан реле вентилятори для нагнітання вологості
  byte r5;        // стан реле перевороту яць 
  byte r6;        // стан реле витяжної вентиляції охолодження
  byte radio;     // стан роботи  радіопередавача
  byte Si7021;    // стан роботи сенсора вологості і температури
  byte DS18b20;   // стан роботи  шини датчиків
  byte RTC_ds1307;   // стан роботи  годинника реального часу
};//typedef struct package Package;
Incubator Incubator_1;
byte downArrow[8] =  { //стрілка вниз
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};
byte upArrow[8] =    { //стрілка вверх
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};
byte menuCursor[8] = { //стрілка напрямку вправо 

  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};