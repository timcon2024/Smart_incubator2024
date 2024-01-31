


#include <Arduino.h>
#pragma once
// put function declarations here:
void idetenficstion_moidel(); //функція присвоєння ідентифікатора для TIMCON

void GET_DateTime();// читаємо свій годинник
void parameter_control (); // контроль встановлених параметрів
void read_Comand(); // команди передані по засобах зв'язку





void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}




void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}