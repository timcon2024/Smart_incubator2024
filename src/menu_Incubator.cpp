#pragma once
#include <main.h>


void read_readKey ()      {                  
        readKey = analogRead(A6);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(A6);
     }                
}
// збереження даних
void save_setting()       {
        button = 0;
        lcd.setCursor(0, 0);
        lcd.print(myText[25]);// виводимо теккст "записуємо дані"
        lcd.setCursor(0, 1);
        lcd.print(myText[53]);// виводимо теккст "дякую"
        delay(1000);
         button = 0;
         activeButton = 1; // вихід в меню
        
              
        
        }    
    //Функція, яка виводить на LCD(1602) два пункти меню, які будуть змінюватись відповідно до прокруткини будут менятся при прокрутке
void mainMenuDraw()       {
  Serial.print(menuPage);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}
    //Функція курсору на LCD,відповідно до пункту меню
void drawCursor()         {
  for (int x = 0; x < 2; x++) {
    lcd.setCursor(0, x);
    lcd.print(" ");
  }
 
  // The menu is set up to be progressive (menuPage 0 = Item 1 & Item 2, menuPage 1 = Item 2 & Item 3, menuPage 2 = Item 3 & Item 4), so
  // in order to determine where the cursor should be you need to see if you are at an odd or even menu page and an odd or even cursor position.
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is even and the cursor position is even that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is even and the cursor position is odd that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is odd and the cursor position is even that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is odd and the cursor position is odd that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}
//опрацювання команд в меню
void operateMainMenu()    {
  activeButton = 0;
  while (activeButton == 0) {
    
    read_readKey ();
    button = evaluateButton(readKey);
    switch (button) {
      case 0: //Если ничего не нажато
        break;
      case 1:  //Выполнять действие если нажата "назад"
        button = 0;
        switch (cursorPosition) { //Переходить в то меню, на котором стоит курсор
          case 0:
            menuItem1(); 
            break;
          case 1:
           menuItem2();
            break;
          case 2:
            menuItem3();
            break;
          case 3:
             menuItem4();
            break;
    //      case 4:
    //        menuItem5();
    //        break;
     //     case 5:
     //       menuItem6();
     //       break;
          }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
 
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
 
        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}
    // Функція, яка працює коли нажали кнопку на меню  и  шляхом зчитування напруги на порті A6, дає номер кнопки
int evaluateButton(int x) {
  int result = 0;
  if (x < 50) {//якщо напруга менше 50 
    result = 1; // кнопка Right
  } else if (x < 195) {
    result = 2; // кнопка Up
  } else if (x < 400) {
    result = 3; // кнопка Down
  } else if (x < 600){
    result = 4; //  кнопкаLeft
  } else if (x < 800) {
    result = 5; // кнопка Set-Ok
  }
  return result;
}
    //Встановлення стрілки і курсора на години, день місяця
void drawInstruction5()   {// стрілка на 5 символ нижнього рядка
  lcd.setCursor(0, 1);
  lcd.print(myText[37]);
  lcd.setCursor(5, 1);
  lcd.print(myText[38]);
}
    //Функції, встановлення указника(стрілки)при налаштуваннях параметрів через меню
    // курсор на хвилинах, 
void drawInstruction2()   { //на 10 символ нижнього рядка
  lcd.setCursor(0, 1);
 lcd.print(myText[37]);// чистимо нижню лінійку
  lcd.setCursor(10, 1);           // ставимо курсор на 10 ячейці щоб міняти хвилини або місяці
  lcd.print(myText[38]); // Up arrow
}
    //Установка курсора на рік,будильник
void drawInstruction14()   { //на 14 символ нижнього рядка
  lcd.setCursor(0, 1);
  lcd.print(myText[37]);
  lcd.setCursor(14, 1);
  lcd.print(myText[38]);
}
void Mane_menu() {        // Основне меню: виводить усю інформацію про стан об'єкту
  activeButton = 0;
  while (activeButton == 0) {
          read_readKey ();
          button = evaluateButton(readKey);
            switch (button) {
            case 5:  // Якщо натиснута кнопка "SELECT" в меню для налаштувань (але на останньому рядку циклу)
            button = 0;
            activeButton = 1;
            break;
        
        default:
      // усі функції контролю і моніторингу тут:
       
        if  (millis() - timer1 >= PERIOD1)          // кожних 5 хв перевіряємо стан
     {
        GET_DateTime();
        sensors_read();
        send_Lcd_Clock();
        send_LCD_sensors();
        parameter_control();
        send_incubator_1I2C();
      do {
       timer1 += PERIOD1;
      if (timer1 < PERIOD1) break;  // переполнение uint32_t
     } while (timer1 < millis() - PERIOD1); // защита от пропуска шага
      }
  
  if  (millis() - timer2 >= PERIOD2)          // кожних 5 хв перевіряємо стан
     {
        incubator_Control();
    
        //sent_Radio_Data();
        send_warningMessage();
  do {
       timer2 += PERIOD2;
      if (timer2 < PERIOD2) break;  // переполнение uint32_t
     } while (timer2 < millis() - PERIOD2); // защита от пропуска шага
      }
       
       
    
      if (Serial.available() > 0)// перевіряємо наявність нових команд від блютузу 
        {
    String bufString = Serial.readString();             // читаємо з порту як рядок символів
    byte dividerIndex = bufString.indexOf(';');           // шукаємо індекс кінця команди
    String buf = bufString.substring(0, dividerIndex);    // створюємо рядок чисел, від першого числадо індексу кінця комани строку с первым числом
    delay(10);
    bluetoothCommand=buf.toInt();                         // перетворюємо  (String  y int) команду в цифровукоманду
    delay(100);
    Serial.print(myText[50]); Serial.println(bluetoothCommand) ; // виводимо команду, для відладки 
    read_bluetoothComand();// запускаємо функцію читання блютуз команди
   }
     
      if((NRF_message>0)||(I2C_message>0)) // перевіряємо наявність нових команд від радіо, чи з модуля esp8266
      read_bluetoothComand();
      
      
     
  }
    
  }
  }
 void menuItem1() {        // Перше меню : налаштування годинника через кнопки з передачею у пам'ять годинника
  activeButton = 0;
  clockPos = 1;
  lcd.clear();            // чистимо екран
  send_Lcd_Clock();        //друкуємо дату і час
  lcd.setCursor(0, 1);    //курсор на нижній рядок на 10 ячейці
  lcd.print(myText[38]);        //  стрілка на 10 ячейці
 
                          // цикл для налаштування годинника (без секунд)
 while (activeButton == 0) {
    read_readKey();
    button = evaluateButton(readKey);
         //Позиція курсора, : 1-день року, 2-місяць року, 3-номер року,4- година, 5-хвилина 
  switch (button) {
   case 2: // якщо нажали "Up"
        if (clockPos==1)  //якщо курсор стоїть на першій клітинці
        {
            button = 0;
              if (dayOfMonth==31)dayOfMonth=0;
              else dayOfMonth = dayOfMonth + 1;// збільшуємо день місяця
                dayOfMonth = constrain(dayOfMonth,0,31);
                lcd.setCursor(0,0);
            
              if (dayOfMonth >=0 && dayOfMonth < 10) {lcd.print(F("0")); lcd.print(dayOfMonth);}
              else lcd.print(dayOfMonth);
        }
       
        else if (clockPos==2) {
                button = 0;
             if (month==12) month=0;
             else month = month + 1; // збільшуємо номер місяця
                month = constrain(month,0, 12);
              lcd.setCursor(3,0);
               if (month >=0 && month < 10) {lcd.print(F("0")); lcd.print(month);}
                else lcd.print(month);
        }
        
        else if (clockPos==3) {
             button = 0;
              if (year==99) year=21;
              else year = year + 1; // збільшуємо номер місяця
              year = constrain(year,21, 99);
              lcd.setCursor(6,0);
              lcd.print(year);
          }
         
        else if (clockPos==4) {
              button = 0;
              if (hour==23) hour=0;
              else hour = hour + 1;
              hour = constrain(hour,0, 24);
              lcd.setCursor(11,0);
           
                if (hour >=0 && hour < 10) {lcd.print(F("0")); lcd.print(hour);}
                else lcd.print(hour);// виводимо години
          }
      
        else if (clockPos==5) {
              button = 0;
              if (minute==59) minute=0;
              else minute = minute + 1;
              minute = constrain(minute,0,60);
              lcd.setCursor(14,0);
               if (minute >=0 && minute < 10) {lcd.print(F("0")); lcd.print(minute);}
                else lcd.print(minute);// виводимо хвилини на екрані 
          } 
          break;
       
   case 3: //якщо нажата кнопка DOWN - зменшуємо на 1 цифру хвилини
        if (clockPos==1)     {
            button = 0;
              if (dayOfMonth==31)dayOfMonth=0;
              else dayOfMonth = dayOfMonth - 1;// збільшуємо день місяця
                dayOfMonth = constrain(dayOfMonth,0,31);
                lcd.setCursor(0,0);
              
              if (dayOfMonth >=0 && dayOfMonth < 10) {lcd.print(F("0")); lcd.print(dayOfMonth);}
              else lcd.print(dayOfMonth);
        }
       
       else if (clockPos==2) {
            button = 0;
             if (month==12) month=0;
             else month = month - 1; // збільшуємо номер місяця
                month = constrain(month,0, 12);
              lcd.setCursor(3,0);
                if (month >=0 && month < 10) {lcd.print(F("0")); lcd.print(month);}
                else lcd.print(month);
        }
        
       else if (clockPos==3) {
             button = 0;
             if (year==21) year=99;
              else
              year = year - 1; // збільшуємо номер місяця
              year = constrain(year,21, 99);
              lcd.setCursor(6,0);
              lcd.print(year);
          }
         
       else if (clockPos==4) {
              button = 0;
              if (hour==23) hour=0;
              else hour = hour - 1;
              hour = constrain(hour,0, 24);
              lcd.setCursor(11,0);
              if (hour >=0 && hour < 10) {lcd.print(F("0")); lcd.print(hour);}
                else lcd.print(hour);// виводимо години
          }
      
       else if (clockPos==5) {
              button = 0;
              if (minute==59) minute=0;
              else minute = minute - 1;
              minute = constrain(minute,0,60);
              lcd.setCursor(14,0);
                if (minute >=0 && minute < 10) {lcd.print(F("0")); lcd.print(minute);}
                else lcd.print(minute);// виводимо хвилини на екрані 
          }
        break;  
            //Позиція курсора згідно clockPos: 1(0)-день року, 2(3)-місяць року, 3(6)-номер року,4(11)- година, 5(14)-хвилина 
   case 4: // Если нажата кнопка влево, смотрим где стоит курсор в данный момент и переставляем левее
        if (clockPos==5){ 
            lcd.setCursor(0, 1);
            lcd.print(myText[37]);
            lcd.setCursor(11, 1);
            lcd.print(myText[38]);
            clockPos=4;}
        else if (clockPos==4){
            lcd.setCursor(0, 1);
            lcd.print(myText[37]);
            lcd.setCursor(6, 1);
             lcd.print(myText[38]);
            clockPos=3;}
        else if (clockPos==3){
            lcd.setCursor(0, 1);
            lcd.print(myText[37]);
            lcd.setCursor(3, 1);
             lcd.print(myText[38]);
            clockPos=2;}
        else if (clockPos==2){
            lcd.setCursor(0, 1);
           lcd.print(myText[37]);
            lcd.setCursor(0, 1);
            lcd.print(myText[38]);
            clockPos=1;}
        else if (clockPos==1){
            button = 0;
         activeButton = 1;}
        break;
          
    
   case 1: // Если нажата кнопка вправо, смотрим где стоит курсор в данный момент и переставляем правее
       if (clockPos==1){
            lcd.setCursor(0, 1);
            lcd.print(myText[37]);
            lcd.setCursor(3, 1);
            lcd.print(myText[38]);
            clockPos=2;}
       else if (clockPos==2){
            lcd.setCursor(0, 1);
            lcd.print(myText[37]);
            lcd.setCursor(6, 1);
            lcd.print(myText[38]);
            clockPos=3;}
       else if (clockPos==3){
            lcd.setCursor(0, 1);
            lcd.print(myText[37]);
            lcd.setCursor(11, 1);
            lcd.print(myText[38]);
            clockPos=4;}
       else if (clockPos==4){
            lcd.setCursor(0, 1);
            lcd.print(myText[37]);
            lcd.setCursor(14, 1);
             lcd.print(myText[38]);
            clockPos=5;} 
       else if (clockPos==5){
            
          lcd.setCursor(0, 0);
          button = 0;
          activeButton = 1; // вихід в меню
            }

            
       break;
   case 5:  // Якщо нажата select  - зберігаємо установлені значення дати , години і хвилини 
       
        // записуємо дані в годинник
        Wire.beginTransmission(DS1307);
        Wire.write(0x1);
        Wire.write(decToBcd(minute)); // зберігаємо змінені годину і хвилину
        Wire.endTransmission();
        delay(10);
        
        Wire.beginTransmission(DS1307);
        Wire.write(0x2);
        Wire.write(decToBcd(hour)); // зберігаємо змінені годину і хвилину
        Wire.endTransmission();
        delay(10);
       
        Wire.beginTransmission(DS1307);
        Wire.write(0x4);
        Wire.write(decToBcd(dayOfMonth)); // зберігаємо змінені годину і хвилину
        Wire.endTransmission();
        delay(10);
        
        Wire.beginTransmission(DS1307);
        Wire.write(0x5);
        Wire.write(decToBcd(month)); // зберігаємо змінені годину і хвилину
        Wire.endTransmission();
        delay(10);
        
         Wire.beginTransmission(DS1307);
        Wire.write(0x6);
        Wire.write(decToBcd(year)); // зберігаємо змінені годину і хвилину
        Wire.endTransmission();
        delay(10);
        save_setting();
   
   break; // вихід у види меню після налаштування дати
  }
 }
}
void menuItem2() {        // Друге меню: активація роботи лічильника днів і параметрів утримання, задання ручних налаштувань,
  activeButton = 0;
  clockPos = 1;
 
  lcd.clear();            // чистимо екран
  
  lcd.setCursor(0, 0);    //
  lcd.print(F("Day:")); lcd.print(incubator_1.dey1);lcd.print(F("    "));
  read_routine_feeding();
  
  lcd.setCursor(4, 1);
 lcd.print(myText[52]);
  lcd.print(F("   next>"));
   while (activeButton == 0) {
    read_readKey();
      button = evaluateButton(readKey);
      switch (button) {
  case 2:  // якщо нажата кнопка "UP" - додавання до змінних
        if(clockPos == 1)      {  // коректуємо день утримання (день утримання при запуску таймеру рахунку днів утримання модуля)      
          button = 0;
          if (correctDey ==99) correctDey=1;
            else correctDey = correctDey + 1;              //  збільшуємо максимально допустиму корекцію на 1 день
            correctDey = constrain(correctDey,1,99);
             lcd.setCursor(4,0);
          lcd.print(myText[52]);
            lcd.setCursor(4,0);
            lcd.print(correctDey);
            }
        else if(clockPos == 2) {  // крутимо режими утримання по цифрах режимів на утриманні
              button = 0;
                if(incubator_1.routing1>9) incubator_1.routing1=1;
                incubator_1.routing1 = incubator_1.routing1 + 1;              
                incubator_1.routing1 = constrain(incubator_1.routing1,1,9);

                lcd.setCursor(9,0);
                read_routine_feeding();
                 
        }
        else if(clockPos == 3) {  // вибираємо к-сть птиці на утриманні
          button = 0;
         
            if (incubator_1.number1 ==999) incubator_1.number1=1;
                incubator_1.number1 = incubator_1.number1 + 1;              //  збільшуємо  на одиницю
                incubator_1.number1 = constrain(incubator_1.number1,1,999);
               
                lcd.setCursor(4,0);
                lcd.print(incubator_1.number1);
               }
        else if(clockPos == 4) {  // збільшуємо загальну к-сть днів утримання
       button = 0;
             if (all_feeding ==999) all_feeding=10;
                all_feeding = all_feeding + 1;              //  збільшуємо  на одиницю
                all_feeding = constrain(all_feeding,10,999);
               
                lcd.setCursor(13,0);
                lcd.print(all_feeding);
      }
     break;
  
  case 3:  // якщо нажата кнопка "Down" - віднімання від змінних
       if(clockPos == 1) { 
                button = 0;
              
              if (correctDey==1) incubator_1.dey1=99;
                correctDey = correctDey - 1;              //  зменшуємо на одиницю
                correctDey = constrain(correctDey,1,99);
                lcd.setCursor(4,0);
               lcd.print(myText[52]);
                lcd.setCursor(4,0);
                lcd.print(correctDey);
                }
       else if(clockPos == 2)  {   //змінюємо режими утримання
                button = 0;
                
              if (incubator_1.routing1==0) incubator_1.routing1=9;
                incubator_1.routing1 = incubator_1.routing1 - 1;              //  збільшуємо на одиницю
                incubator_1.routing1 = constrain(incubator_1.routing1,1,9);
               
                lcd.setCursor(9,0);
                 read_routine_feeding();
                               
         } 
       else if(clockPos == 3)  {   // зменшуємо к-сть птиці на утриманні
          button = 0;
         
            if ( incubator_1.number1 ==1)  incubator_1.number1=999;
                 incubator_1.number1 =  incubator_1.number1 - 1;              //  зменшуємо на одиницю
                 incubator_1.number1 = constrain(incubator_1.number1,1,999);
                lcd.setCursor(4,0);
               
                lcd.print(incubator_1.number1);
                }
       else if(clockPos == 4)  {   // зменшуємо загальну к-сть днів утримання
              button = 0;
                if (all_feeding ==999) all_feeding=10;
                all_feeding = all_feeding - 1;              //  зменшуємо на одиницю
                all_feeding = constrain(all_feeding,10,999);
              
                lcd.setCursor(13,0);
                lcd.print(all_feeding);
      }
     break;
  
  case 4: // якщо нажата кнопка "LEFT", дивимось де курсор в даний момент и переставляємо лівіше
       if  (clockPos==2)      {
            lcd.setCursor(0, 1);
           lcd.print(myText[51]);
            lcd.setCursor(4, 1);
           lcd.print(myText[52]);
            clockPos=1;}
       else if (clockPos==4)  {
            lcd.setCursor(0, 1);
           lcd.print(myText[51]);
            lcd.setCursor(4, 1);
            lcd.print(myText[52]);
            clockPos=3;}
       else if (clockPos==3)  {
            lcd.setCursor(0, 0);
          lcd.print(myText[51]);
            lcd.setCursor(0, 0);
            lcd.print("Day:"); lcd.print(incubator_1.dey1);lcd.print("    ");
            read_routine_feeding();
            lcd.setCursor(0, 1);
            lcd.print(myText[51]);
            lcd.setCursor(9, 1);
            lcd.print(myText[52]);
            clockPos=2;}
     break;
     
  case 1: //  якщо нажата кнопка "RIGTH", дивимось де курсор в даний момент и переставляємо вправо
       if (clockPos==1){
          lcd.setCursor(0, 1);
          lcd.print(myText[51]);
          lcd.setCursor(9, 1); // стаємо під назвою птахів
          lcd.print(myText[52]);
          clockPos=2;}
       else if (clockPos==2)  {
          lcd.setCursor(0, 0);
          lcd.print(F("Num ")); lcd.print(incubator_1.number1);lcd.setCursor(9, 0);
          lcd.print(F("All ")); lcd.print(all_feeding);lcd.print(F(" "));
          lcd.setCursor(0, 1);
         lcd.print(myText[51]);
          lcd.setCursor(4, 1);
          lcd.print(myText[52]);
          clockPos=3;}
       else if (clockPos==3)  {
          lcd.setCursor(0, 1);
          lcd.print(myText[51]);
          lcd.setCursor(13, 1);
         lcd.print(myText[52]);
          clockPos=4;
          }
       else if (clockPos==4)  {
          button=0;
         activeButton =1;
        
         }
    break;
    
      case 5:  // якщо нажата кнопка "select"
        
        flag_Startincubation=true;
       
       EEPROM.update(6, 1);//ставимо мітку що лічильник включено і запущено відрахунок днів
     //  EEPROM.put(280,routing1);
       EEPROM.put(42,correctDey);
       EEPROM.put(40,incubator_1.number1);  
       EEPROM.put(201,incubator_1.routing1);  
       save_setting();
       
       break;
   }
  }
}                        //читаємо дані з сенсорів і датчиків і їх обрізаємо та фільтруємо)
void menuItem3() {        // Третє меню: налаштування параметрів контролю  температури,(в тому числі коефіцієнтів PID -за замовчуванням Kp=2, Ki=5, Kd=1.5;)
  activeButton = 0;
  clockPos = 1;
  lcd.clear();            // чистимо екран
 // налаштовуємо задану температуру Setpoint, Kp=2, Ki=5, Kd=1.5;
lcd.setCursor(0, 0);    //
lcd.print(myText[41]); lcd.print(Setpoint);lcd.println(myText[13]);

lcd.setCursor(5, 1);
lcd.print(myText[38]);
lcd.print(F("   next>>"));
 while (activeButton == 0) {
    read_readKey();
    button = evaluateButton(readKey);
    switch (button) {
     
     case 2: // якщо нажали "Up"
         if (clockPos==1)      {
                button = 0;
                if (Setpoint ==39.00)Setpoint=36.30;
                Setpoint = Setpoint + 0.1;              //  збільшуємо максимально допустиму температуру на 1 градус
                Setpoint = constrain(Setpoint,36.30,39.00);
             
                lcd.setCursor(5,0);
                lcd.print(Setpoint);
                 }
        else if (clockPos==2) {
                button = 0;
                if (Kp ==0)Kp=10;
                Kp = Kp +0.1;              //  зменшуємо коефіцієнт  Кp на 0.1
                Kp = constrain(Kp,0,10);
              
                lcd.setCursor(14,0);
                 lcd.print(myText[51]);  lcd.print(Kp);
                }
        else if (clockPos==4) {
                button = 0;
                if (Ki ==1)Ki=10;
               Ki = Ki +0.1;              //  зменшуємо коефіцієнт  Кі на 0.1
                Ki = constrain(Ki,1,10);
              
                lcd.setCursor(14,0);
                 lcd.print(myText[52]);
                 lcd.print(Ki);
                }
        else if (clockPos==3) {
                 button = 0;
                if (Kd ==1)Kd=10;
                Kd = Kd +0.1;              //  зменшуємо коефіцієнт  Кі на 0.1
                Kd = constrain(Kd,1,10);
              
                lcd.setCursor(14,0);
                lcd.print(myText[53]);
                lcd.print(Kd);
                }
        else if (clockPos==5) {
                button = 0;
                if (twmax ==15)twmax=69;
                twmax = twmax - 1;              //  зменшуємо максимально допустиму температуру на 1 градус
                twmax = constrain(twmax,15,69);
                  lcd.setCursor(5,0);
                lcd.print(twmax);
                }
        else if (clockPos==6) {
                button = 0;
                if (gazmax <=101)gazmax=999;
                gazmax = gazmax - 50;              //  збільшуємо максимально допустиму температуру на 1 градус
                gazmax = constrain(gazmax,101,999);
                     lcd.setCursor(13,0);
                lcd.print(gazmax);
                }
       break;
     
    case 3: //якщо нажата кнопка DOWN - зменшуємо на 0,1 градус, корекція коефіцієнтів PID  Kp=2, Ki=5, Kd=1.5;
        if (clockPos==1)      {
                button = 0;
                if (Setpoint ==39.00)Setpoint=36.60;
                Setpoint = Setpoint - 0.1;              //  збільшуємо максимально допустиму температуру на 1 градус
                Setpoint = constrain(Setpoint,36.60,38.70);
             
                lcd.setCursor(5,0);
                lcd.print(Setpoint);
                 }
        else if (clockPos==2) {
                button = 0;
                if (Kp ==0)Kp=10;
                Kp = Kp -0.1;              //  зменшуємо коефіцієнт  Кp на 0.1
               // Kp = constrain(Kp,0,10);
              
                lcd.setCursor(14,0);
                 lcd.print(myText[51]);  lcd.print(Kp);
                }
        else if (clockPos==4) {
                button = 0;
                if (Ki ==1)Ki=10;
                Ki = Ki - 1.0;              //  зменшуємо коефіцієнт  Кі на 0.1
             //   Kі = constrain(Kі,1,10);
              
                lcd.setCursor(14,0);
                 lcd.print(myText[52]);
                lcd.print(Ki);
                }
        else if (clockPos==3) {
                 button = 0;
                if (Kd ==0)Kd=10;
                Kd = Kd -0.1;              //  зменшуємо коефіцієнт  Кd на 0.1
               // Kd = constrain(Kd,0,10);
              
                lcd.setCursor(14,0);
                 lcd.print(myText[53]);  lcd.print(Kd);
                }
        else if (clockPos==5) {
                button = 0;
                if (twmax ==15)twmax=69;
                twmax = twmax - 1;              //  зменшуємо максимально допустиму температуру на 1 градус
                twmax = constrain(twmax,15,69);
                  lcd.setCursor(5,0);
                lcd.print(twmax);
                }
        else if (clockPos==6) {
                button = 0;
                if (gazmax <=101)gazmax=999;
                gazmax = gazmax - 50;              //  збільшуємо максимально допустиму температуру на 1 градус
                gazmax = constrain(gazmax,101,999);
                     lcd.setCursor(13,0);
                lcd.print(gazmax);
                }
      break;
  
    case 1: // Если нажата кнопка RIGTH, вправо, смотрим где стоит курсор в данный момент и переставляем правее
        if (clockPos==1){
            drawInstruction14();
            clockPos=2;}
        else if (clockPos==2) {
            lcd.setCursor(0, 0);
            lcd.print(F("                "));
            lcd.setCursor(0, 0);
            lcd.print(myText[56]); lcd.print(Hmax);lcd.print(F("  "));
            lcd.print(myText[57]); lcd.print(Hmin);lcd.print(F("  "));
           
            drawInstruction5();
            
            clockPos=3;}
        else if (clockPos==3) {
            drawInstruction14();
            lcd.print(">>");
            clockPos=4;
            }
        else if (clockPos==4) {
            lcd.setCursor(0, 0);
           lcd.print(myText[51]);
            lcd.setCursor(0, 0);
            lcd.print(myText[58]); lcd.print(twmax);lcd.print(F(" "));
            lcd.print(myText[59]); lcd.print(gazmax);lcd.print(F("  "));
            drawInstruction5();
           
             clockPos=5;}
        else if (clockPos==5) {
            lcd.setCursor(0, 1);
            lcd.print(myText[51]);
            lcd.setCursor(13, 1);
            lcd.print(myText[52]);
            lcd.print("><");
            clockPos=6;
            }
        else if (clockPos==6) {
               save_setting();
             }
      break;
        
    case 4:  // Если нажата кнопка "left"
        if  (clockPos==2){
            drawInstruction5();
            clockPos=1;}
       else if (clockPos==4){
           drawInstruction5();
            clockPos=3;}
       else if (clockPos==6){
            drawInstruction5();
            clockPos=5;}
       else if (clockPos==5){
            lcd.setCursor(0, 0);
            lcd.print(myText[51]);
            lcd.setCursor(0, 0);
            lcd.print(myText[56]); lcd.print(Hmax);lcd.print("  ");
            lcd.print(myText[57]); lcd.print(Hmin);lcd.print("  ");
            
            drawInstruction14();
            clockPos=4;}
       else if (clockPos==3){
            lcd.setCursor(0, 0);
            lcd.print(myText[51]);
            lcd.setCursor(0, 0);
             lcd.print(myText[41]);lcd.print(Setpoint);lcd.print(F("  "));
             lcd.print(myText[40]); lcd.print(Input);lcd.print(F("  "));
            drawInstruction14();
            clockPos=2;}
       else if (clockPos==1){
            activeButton = 1;}
      
      break;
     
     case 5:  // Если нажата кнопка "set"
                   
           EEPROM.put(15,Setpoint);
           EEPROM.put(18, gazmax);                    // перезаписуємо в пам'ять (якщо там інші дані) старший байт в ячейку 1?
           EEPROM.put(20, Hmax);                   // перезаписуємо в пам'ять (якщо там інші дані) старший байт в ячейку 1?
           EEPROM.put(22,Hmin );                 // перезаписуємо в пам'ять (якщо там інші дані) старший байт в ячейку 1?
           EEPROM.put(24, twmax);                      // перезаписуємо в пам'ять (якщо там інші дані) старший байт в ячейку 1?
           EEPROM.put(26,twmin );                   // перезаписуємо в пам'ять (якщо там інші дані) старший байт в ячейку 1?
           
            save_setting();  
            break;
   }
 }
}
 void menuItem4() {        // четверте меню: налаштування таймерів для приладів перевороту актуаторів та приводних моторів
  activeButton = 0;
  clockPos = 1;
  lcd.clear();            // чистимо екран
 
lcd.setCursor(0, 0); 
lcd.print(F("Actuator on: ")); lcd.print(timer_work_ictuator/1000);// час роботи актуатора (час висунення і засунення штоку, прямий і ревесний хід актуатора для перевороту
lcd.setCursor(0, 1);
lcd.print(F("Actuator off: ")); lcd.print(timer_interval_incubator/1000);// час перерви між переворотами (час утримання лотка в положенні крайньому правому або лівому


lcd.setCursor(4, 1);
lcd.print(myText[38]);
lcd.print(F(" minute >>"));
 while (activeButton == 0) {
    read_readKey();
    button = evaluateButton(readKey);
    switch (button) {
     
     case 2: // якщо нажали "Up"
        if (clockPos==1)      {
                button = 0;
                timer_work_ictuator=timer_work_ictuator/1000/60; // виходимо на хвилини
                if (timer_work_ictuator ==120)timer_work_ictuator=10;
                timer_work_ictuator = timer_work_ictuator + 10;                 //  збільшуємо час спокою(перезив у перевороті на 10 хв
                timer_work_ictuator = constrain(timer_work_ictuator,10,120);
              
                lcd.setCursor(4,0);
                lcd.print(timer_work_ictuator);
                }
       
      //  else if (clockPos==2) {
      //         button = 0;
       //         if (timer4 ==1440)timer4=1;
       //         timer4 = timer4 + 1;                 //  збільшуємо максимально допустиму температуру на 1 градус
       //         timer4 = constrain(timer4,1,1440);
              
       //         lcd.setCursor(12,0);
       //         lcd.print(timer4);
        //        }
  
       break;
     
    case 3: //якщо нажата кнопка DOWN - за один натиск зменшуємо на 10 секунд (крок кнопки)
        if (clockPos==1)      {
                button = 0;
                if (timer_work_ictuator ==10)timer_work_ictuator=120;
                timer_work_ictuator = timer_work_ictuator - 10;                 //  збільшуємо максимально допустиму температуру на 1 градус
                timer_work_ictuator = constrain(timer_work_ictuator,10,120);
              
                lcd.setCursor(4,0);
                lcd.print(timer_work_ictuator);
                }
      //  else if (clockPos==2) {
       //          button = 0;
       //         if (timer4 ==1)timer4=1440;
       //         timer4 = timer4 - 1;                 //  збільшуємо максимально допустиму температуру на 1 градус
        //        timer4 = constrain(timer4,1,1440);
              
         //       lcd.setCursor(12,0);
          //      lcd.print(timer4);
          //      }
  
      break;
  
    case 1: // Если нажата кнопка RIGTH, вправо, смотрим где стоит курсор в данный момент и переставляем правее
        if (clockPos==1){
           lcd.setCursor(0, 1);
          lcd.print(myText[51]);
          lcd.setCursor(12, 1);
          lcd.print(myText[52]);
            clockPos=2;}
           
        else if (clockPos==2) {
            
           activeButton = 1;}
             
      break;
        
    case 4:  // Если нажата кнопка "left"
        if  (clockPos==2){
           lcd.setCursor(0, 1);
          lcd.print(myText[51]);
          lcd.setCursor(4, 1);
          lcd.print(myText[52]);
            clockPos=1;}
     
   
       else if (clockPos==1){
            activeButton = 1;}
      
      break;
     
     case 5:  // Если нажата кнопка "set"
                   
           EEPROM.put(100,timer_incubator);// перезаписуємо в пам'ять к-сть хвилин таймера перевороту яєць
           save_setting();  
            break;
   }
  }
}
