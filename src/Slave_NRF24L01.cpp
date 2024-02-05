/* код для отримувача радіо каналом з інкубатора
*/
void receive_Radio_Data() {
  while (radio.available()) {
    byte messageType;
    radio.read(&messageType, sizeof(messageType));
    
    if (messageType == 1) {
      // Отримано число
      int receivedNumber;
      radio.read(&receivedNumber, sizeof(receivedNumber));
      
      // Обробка отриманого числа receivedNumber
      
      // Наприклад, виведення числа на серійний порт
      Serial.print("Отримане число: ");
      Serial.println(receivedNumber);
      
    } else if (messageType == 2) {
      // Отримано повідомлення
      char receivedMessage[32];
      radio.read(receivedMessage, sizeof(receivedMessage));
      
      // Обробка отриманого повідомлення receivedMessage
      
      // Наприклад, виведення повідомлення на серійний порт
      Serial.print("Отримане повідомлення: ");
      Serial.println(receivedMessage);
      
    } else if (messageType == 3) {
      // Отримано структуру Incubator
      byte dataBytes[sizeof(Incubator)];
      radio.read(dataBytes, sizeof(Incubator));
      
      Incubator receivedData;
      memcpy(&receivedData, dataBytes, sizeof(Incubator));
      
      // Обробка отриманої структури receivedData
      
      // Наприклад, виведення даних зі структури на серійний порт
      Serial.print("Отримані дані: t=");
      Serial.print(receivedData.t);
      Serial.print(", h=");
      Serial.print(receivedData.h);
      // Виведіть решту полів структури Incubator за аналогічним шаблоном
      
      // Відправка числа назад передавачу
      int message = 42; // вставляємо цифрову команду з шифруванням для інкубатора :
      
      // Використання radio.writeAckPayload() для передачі числа message
      radio.writeAckPayload(1, &message, sizeof(message));
      
    } else {
      // Невідомий тип повідомлення
      Serial.println("Невідомий тип повідомлення");
    }
  }
}
