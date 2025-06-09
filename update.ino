/*
Обновление прошивки через SD-карту
*/
bool CheckUpdate() {
  uint8_t cardType;
  if (!SD.begin()) {
    tft.setCursor(28, 68);  //+14
    tft.setTextColor(sys_color[2]);
    printMenuText(F("SD-карта не вставлена."));
    return false;
  }
  cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    tft.setCursor(28, 68);  //+14
    tft.setTextColor(sys_color[2]);
    printMenuText(F("SD-карта не вставлена."));
    return false;
  } else {
    bool ufs = updateFromFS(SD);
    return ufs;
  }
  return true;
}

// Проверка на наличие обновления прошивки
bool updateFromFS(fs::FS &fs) {
  File updateBin = fs.open("/firmware.bin");
  if (updateBin) {
    if (updateBin.isDirectory()) {
      tft.setCursor(28, 68);  //+14
      tft.setTextColor(sys_color[2]);
      printMenuText(F("Ошибка!"));
      tft.setCursor(28, 82);  //+14
      printMenuText(F("firmware.bin не является"));
      tft.setCursor(28, 104);  //+14
      printMenuText(F("файлом."));
      updateBin.close();
      return false;
    }

    size_t updateSize = updateBin.size();
    Serial.print("Размер firmware.bin = ");
    Serial.println(updateSize);
    if (updateSize > 0) {
      tft.setCursor(28, 68);  //+14
      tft.setTextColor(sys_color[2]);
      printMenuText(F("Новая прошивка"));
      tft.setCursor(28, 82);  //+14
      printMenuText(F("готова к установке."));
      updateBin.close();
      return true;
    } else {
      tft.setCursor(28, 68);  //+14
      tft.setTextColor(sys_color[2]);
      printMenuText(F("Ошибка!"));
      tft.setCursor(28, 82);  //+14
      printMenuText(F("Файл firmware.bin пуст."));
      updateBin.close();
      return false;
    }
  } else {
    Serial.println(F("firmware.bin not found on the root of the sd-card"));
    tft.setCursor(28, 68);  //+14
    tft.setTextColor(sys_color[2]);
    printMenuText(F("Новой прошивки"));
    tft.setCursor(28, 82);  //+14
    printMenuText(F("на SD-карте нет."));
    return false;
  }
}

// Update progress status
void progresscallback(size_t currSize, size_t totalSize) {
  Serial.printf("Обновление прошивки: %d из %d байт...\n", currSize, totalSize);
  int cur_progress = (currSize * 100) / totalSize;
  if (old_progress != cur_progress) {
    tft.setCursor((w / 2) - 20, 94);
    tft.setFont(&mMediaDigital32pt8b);
    tft.setTextColor(0xB121);
    tft.print(old_progress, DEC);
    tft.setCursor((w / 2) - 20, 94);
    tft.setFont(&mMediaDigitalC32pt8b);
    tft.setTextColor(0xFCA1);
    tft.print(cur_progress, DEC);
    old_progress = cur_progress;
  }
}

void rebootEspWithReason(String reason) {
  Serial.println(reason);
  delay(1000);
  ESP.restart();
}
