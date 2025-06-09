// Настройки радио
const char* menu_pic[6] = { "(", ")", "%", "#", "\'", "&" };
const char* menu_text[6] = { "Выход", "Экран и шрифт", "Часы", "Звук", "Воспроизведение", "Обновление" };
int menu_pos = 0;
int sub_menu_pos = 0;
int old_menu_pos = 0;
int new_palette;

void radioSettings() {
  new_palette = cur_palette;
  menu_pos = 0;
  tft.fillScreen(sys_color[0]);
  tft.drawLine(22, 0, 22, 180, sys_color[2]);
  tft.fillRect(22, 0, 240, 18, sys_color[2]);
  SettingsScreen(0);
  for (;;) {
    enc1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) break;
      menu_pos++;
      if (menu_pos > 5) menu_pos = 0;
    }

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) break;
      menu_pos--;
      if (menu_pos < 0) menu_pos = 5;
    }

    if (enc1.isTurn() || pult_code == down || pult_code == up) {
      if (LedOn()) break;
      SettingsScreen(menu_pos);
    }

    if (enc1.isSingle() || pult_code == randoms || pult_code == next) {
      if (LedOn()) break;
      if (menu_pos == 0) {
        // Выход из меню
        if (new_palette != cur_palette) {
          cur_palette = new_palette;
          setPalettes(cur_palette);
        }
        tft.fillScreen(ST77XX_BLACK);
        //radio_mode = 1;
        old_time = "";
        update_display_info();
        VoiceMenu(F("Часы"));
        eeprom_timer = millis();
        eeprom_flag = true;
        return;
      } else {
        sub_menu_pos = 1;
        if (menu_pos == 5) sub_menu_pos = 7;
        SettingsSubScreen(menu_pos, sub_menu_pos);
        SettingsSubMove();
      }
    }
    if (pult_code == prev || pult_code == menus) {
      if (LedOn()) break;
      menu_pos = 0;
      SettingsScreen(0);
    }
    esp_task_wdt_reset();
  }
}

void SettingsScreen(int cursor) {
  tft.setFont(&mMediaSymbols);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  tft.setTextColor(sys_color[2]);
  for (int i = 0; i <= 5; i++) {
    tft.setTextColor(sys_color[2]);
    if (i == cursor) tft.setTextColor(ST77XX_ORANGE);
    tft.setCursor(0, 16 + (22 * i));
    tft.print(menu_pic[i]);
  }
  tft.setFont(&mMediaBold7pt8b);
  tft.setTextSize(1);
  tft.setCursor(28, 13);
  tft.setTextColor(sys_color[2]);
  printMenuText(menu_text[old_menu_pos]);
  tft.setCursor(28, 13);
  tft.setTextColor(sys_color[0]);
  printMenuText(menu_text[menu_pos]);
  old_menu_pos = cursor;
  //-------------------
  SettingsSubScreen(menu_pos, 0);
}

void SettingsSubScreen(int cursor, int sub_cursor) {
  tft.fillRect(23, 19, 240, 180, sys_color[7]);
  if (sub_cursor > 0) tft.fillRect(23, 9 - sub_cursor + (15 * sub_cursor), 240, 15, sys_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextColor(sys_color[2]);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  switch (cursor) {
    case 0:  //Выход из меню
      tft.setCursor(28, 32);
      printMenuText(F("Нажмите кнопку или"));
      tft.setCursor(28, 44);  //+12
      printMenuText(F("энкодер для выхода"));
      tft.setCursor(28, 56);
      printMenuText(F("из меню настроек."));
      tft.drawLine(24, 65, 158, 65, sys_color[2]);
      tft.setCursor(28, 80);
      printMenuText(F("Пробег (в часах): "));
      tft.setTextColor(sys_color[3]);
      tft.setFont(&mMediaDigit10pt8b);
      tft.setTextSize(1);
      tft.setCursor((w + 28 - String(total_hours).length() * 11) / 2, 106);
      tft.print(String(total_hours));
      break;
    case 1:  //Экран и шрифт
      tft.setCursor(28, 34);
      printMenuText(F("Темная тема "));
      menuSwitch(themas);
      tft.setCursor(28, 48);
      printMenuText(F("Палитра "));
      printAddMenu();
      tft.setCursor(28, 62);  //+14
      printMenuText(F("Растровый шрифт "));
      menuSwitch(sys_font);
      tft.setCursor(28, 76);  //+14
      printMenuText(F("Яркость "));
      printAddMenu();
      tft.setCursor(28, 90);  //+14
      printMenuText(F("Автосохранение "));
      menuSwitch(auto_save);
      tft.setCursor(28, 104);  //+14
      printMenuText(F("[Сохранить настройки]"));
      break;
    case 2:  //Часы
      tft.setCursor(28, 34);
      printMenuText(F("Часовая зона "));
      printAddMenu();
      tft.setCursor(28, 48);  //+14
      printMenuText(F("Куранты "));
      menuSwitch(kurant_flag);
      printAddMenu();
      tft.setCursor(28, 62);
      printMenuText(F("Будильн.на вкл. "));
      printAddMenu();
      tft.setCursor(28, 76);
      printMenuText(F("Будильн.на выкл. "));
      printAddMenu();
      tft.setCursor(28, 90);  //+14
      printMenuText(F("Вкл. радио "));
      menuSwitch(waikup_flag[numAl]);
      printAddMenu();
      tft.setCursor(28, 104);  //+14
      printMenuText(F("Выкл. радио "));
      menuSwitch(sleep_flag[numAl2]);
      printAddMenu();
      break;
    case 3:  //Звук
      tft.setCursor(28, 34);
      printMenuText(F("Громкость "));
      printAddMenu();
      tft.setCursor(28, 48);
      printMenuText(F("Эквалайзер "));
      printAddMenu();
      tft.setCursor(28, 62);  //+14
      printMenuText(F("АРУ громкости "));
      menuSwitch(aru);
      tft.setCursor(28, 76);  //+14
      printMenuText(F("Моно "));
      menuSwitch(mono);
      tft.setCursor(28, 90);  //+14
      printMenuText(F("Голосовое меню "));
      menuSwitch(voice_menu_flag);
      break;
    case 4:  //Воспроизведение
      tft.setCursor(28, 34);
      printMenuText(F("Выбор из списка "));
      menuSwitch(preview_mode_flag);
      tft.setCursor(28, 48);
      printMenuText(F("С кликом / без "));
      menuSwitch(voice_sw_flag);
      tft.setCursor(28, 62);
      printMenuText(F("Автовозврат "));
      menuSwitch(autoscan);
      tft.setCursor(28, 76);
      printMenuText(F("Удалять не рабоч "));
      menuSwitch(del_st_sw_flag);
      tft.setCursor(28, 90);
      printMenuText(F("Удалять медленн. "));
      menuSwitch(del_st_slow_flag);
      tft.setCursor(28, 104);
      printMenuText(F("Случ. композиция "));
      menuSwitch(rand_song);
      tft.setCursor(28, 118);  //+14
      printMenuText(F("Индекс. файлов"));
      printAddMenu();
      break;
    case 5:  //Обновление
      tft.setCursor(28, 34);
      printMenuText(F("Текущая версия ПО: "));
      tft.setCursor(78, 48);  //+14
      tft.setTextColor(sys_color[4]);
      printMenuText(String(verMajor) + "." + String(verMinor));
      tft.setTextColor(sys_color[5]);
      if (CheckUpdate() == true) {
        tft.setCursor(28, 118);
        printMenuText(F("[ Обновить прошивку ]"));
      }
      break;
  }
}

void printAddMenu() {
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(ST77XX_BLUE);
  tft.print("<");
  tft.setFont(&mMedia6pt8b);
  tft.setTextColor(sys_color[2]);
}

void menuSwitch(bool val) {
  tft.setFont(&mMediaSymbols);
  if (val == true) {
    tft.setTextColor(ST77XX_GREEN);
    tft.print("/");
  } else {
    tft.setTextColor(ST77XX_HRED);
    tft.print(".");
  }
  tft.setFont(&mMedia6pt8b);
  tft.setTextColor(sys_color[2]);
}

void printMenuText(String mText) {
  if (sys_font == true) {
    sys_font = false;
    tft.print(utf8rus(mText));
    sys_font = true;
  } else {
    tft.print(utf8rus(mText));
  }
}

void SettingsSubMove() {
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) break;
      sub_menu_pos++;
      switch (menu_pos) {
        case 1:
          if (sub_menu_pos > 6) sub_menu_pos = 1;
          break;
        case 2:
          if (sub_menu_pos > 6) sub_menu_pos = 1;
          break;
        case 3:
          if (sub_menu_pos > 5) sub_menu_pos = 1;
          break;
        case 4:
          if (sub_menu_pos > 7) sub_menu_pos = 1;
          break;
        case 5:
          sub_menu_pos = 7;
          break;
      }
      SettingsSubScreen(menu_pos, sub_menu_pos);
    }

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) break;
      sub_menu_pos--;
      switch (menu_pos) {
        case 1:
          if (sub_menu_pos < 1) sub_menu_pos = 6;
          break;
        case 2:
          if (sub_menu_pos < 1) sub_menu_pos = 6;
          break;
        case 3:
          if (sub_menu_pos < 1) sub_menu_pos = 5;
          break;
        case 4:
          if (sub_menu_pos < 1) sub_menu_pos = 7;
          break;
        case 5:
          sub_menu_pos = 7;
          break;
      }
      SettingsSubScreen(menu_pos, sub_menu_pos);
    }

    if (butt1.hasClicks() || pult_code == prev || pult_code == menus) {
      if (LedOn()) break;
      sub_menu_pos = 0;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }

    if (enc1.isSingle() || pult_code == randoms || pult_code == next) {
      if (LedOn()) break;
      switch (menu_pos) {
        case 1:  //Экран и шрифт
          switch (sub_menu_pos) {
            case 1:  //Тёмная тема
              themas = !themas;
              break;
            case 2:  //Палитра
              showPaletteMenu();
              break;
            case 3:  //Растровый шрифт
              sys_font = !sys_font;
              break;
            case 4:  //Яркость экрана
              showLed();
              break;
            case 5:  //Автосохранение
              auto_save = !auto_save;
              break;
            case 6:  //Соханить настройки
              updateEEPROM();
              tft.setCursor(28, 118);  //+14
              tft.setTextColor(sys_color[3]);
              printMenuText(F("Сохранено."));
              vTaskDelay(2000);
              break;
          }
          break;
        case 2:  //Часы
          switch (sub_menu_pos) {
            case 1:  //Часовая зона
              showTimeZoneMenu();
              break;
            case 2:  //Куранты
              showKurantMenu();
              break;
            case 3:  //ном. будильн.
              showNnumMenu();
              break;
            case 4:  //ном. будильн.выкл
              showNnumMenuoff();
              break;
            case 5:  //Выкл. радио
              showOnMenu();
              break;
            case 6:  //Вкл. радио
              showOffMenu();
              break;
          }
          break;
        case 3:  //Звук
          switch (sub_menu_pos) {
            case 1:  //Громкость
              showVolumeMenu();
              break;
            case 2:  //Эквалайзер
              showSoundMenu();
              break;
            case 3:  //АРУ громкости
              aru = !aru;
              break;
            case 4:  //Моно
              mono = !mono;
              break;
            case 5:  //Голосовое меню
              voice_menu_flag = !voice_menu_flag;
              break;
          }
          break;
        case 4:  //Воспроизведение
          switch (sub_menu_pos) {
            case 1:  //Выбор из списка
              preview_mode_flag = !preview_mode_flag;
              break;
            case 2:  //voice_sw_flag
              voice_sw_flag = !voice_sw_flag;
              break;
            case 3:  //autoscan
              autoscan = !autoscan;
              break;
            case 4:  //del_st_sw_flag
              del_st_sw_flag = !del_st_sw_flag;
              break;
            case 5:  //del_st_slow_flag
              del_st_slow_flag = !del_st_slow_flag;
              break;
            case 6:  //Случ. композиция
              rand_song = !rand_song;
              break;
            case 7:  //Индексация файлов
              showIndexMenu();
              break;
          }
          break;
        case 5:  //Обновление прошивки
          updateFirmware = true;
          updateEEPROM();
          tft.fillRect(28, 60, 240, 68, sys_color[7]);
          tft.setCursor(28, 76);  //+14
          tft.setTextColor(sys_color[3]);
          printMenuText(F("После перезагрузки"));
          tft.setCursor(28, 90);  //+14
          printMenuText(F("ПО обновится."));
          vTaskDelay(3000);
          ESP.restart();
          return;
          break;
      }
      SettingsSubScreen(menu_pos, sub_menu_pos);
    }
    esp_task_wdt_reset();
  }
}

void showPaletteMenu() {
  showSubTitle(F("Палитра"));
  tft.setCursor(42, 60);  //+20
  tft.setTextColor(sys_color[3]);
  printMenuText("[ " + palette_type[new_palette] + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isRight() || pult_code == next) {
      if (LedOn()) break;
      tft.setTextColor(sys_color[7]);
      tft.setCursor(42, 60);
      printMenuText("[ " + palette_type[new_palette] + " ]");
      new_palette++;
      if (new_palette > 6) new_palette = 0;
      tft.setTextColor(sys_color[3]);
      tft.setCursor(42, 60);
      printMenuText("[ " + palette_type[new_palette] + " ]");
    }

    if (enc1.isLeft() || pult_code == prev) {
      if (LedOn()) break;
      tft.setTextColor(sys_color[7]);
      tft.setCursor(42, 60);
      printMenuText("[ " + palette_type[new_palette] + " ]");
      new_palette--;
      if (new_palette < 0) new_palette = 6;
      tft.setTextColor(sys_color[3]);
      tft.setCursor(42, 60);
      printMenuText("[ " + palette_type[new_palette] + " ]");
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == randoms || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

void showKurantMenu() {
  showSubTitle(F("Куранты"));
  printMenuText(F("состояние "));
  menuSwitch(kurant_flag);
  tft.setCursor(42, 80);  //+20
  tft.setTextColor(sys_color[3]);
  printMenuText("[ " + kurant_type[cur_kurant] + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == down) {
      if (LedOn()) break;
      sub_pos++;
      if (sub_pos > 1) sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (enc1.isRight() || pult_code == up) {
      if (LedOn()) break;
      sub_pos--;
      if (sub_pos < 0) sub_pos = 1;
      showSubItem(sub_pos);
    }

    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          kurant_flag = !kurant_flag;
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("состояние "));
          menuSwitch(kurant_flag);
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("[ " + kurant_type[cur_kurant] + " ]");
          cur_kurant++;
          if (cur_kurant > 7) cur_kurant = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("[ " + kurant_type[cur_kurant] + " ]");
          break;
      }
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          kurant_flag = !kurant_flag;
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("состояние "));
          menuSwitch(kurant_flag);
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("[ " + kurant_type[cur_kurant] + " ]");
          cur_kurant--;
          if (cur_kurant < 0) cur_kurant = 7;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("[ " + kurant_type[cur_kurant] + " ]");
          break;
      }
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == randoms || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

String getDaysString(bool days[7]) {
  const char* dayNames[7] = { "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс" };
  String result = "";

  for (int i = 0; i < 7; i++) {
    if (days[i]) {
      tft.setTextColor(ST77XX_GREEN);  // Активные дни - зеленые
    } else {
      tft.setTextColor(ST77XX_DARKGREY);  // Неактивные дни - серые
    }
    tft.print(utf8rus(dayNames[i]));  // Используем utf8rus для корректного отображения кириллицы
    if (i < 6) tft.print(" ");        // Пробел между днями, кроме последнего
  }
  return result;
}

void showNnumMenu() {
  for (int i = 0; i < MAX_ALARMS; i++) {
    if (waikup_day[i][7]) {
      waikup_day[i][7] = false;
      for (int y = 0; y < 7; y++) {
        waikup_day[i][y] = true;
      }
    }
  }
  showSubTitle("Будильник вкл. " + String(numAl + 1));

  // Отображаем состояние будильника
  tft.setCursor(42, 60);
  tft.setTextColor(sys_color[2]);
  printMenuText(F("Состояние: "));
  menuSwitch(waikup_flag[numAl]);

  // Отображаем время срабатывания
  char alarmTime[6];
  sprintf(alarmTime, "%02d:%02d", wHour[numAl], wMinute[numAl]);
  tft.setCursor(42, 80);
  tft.setTextColor(sys_color[2]);
  printMenuText("Время: [ " + String(alarmTime) + " ]");

  // Отображаем дни недели
  tft.setCursor(32, 100);
  bool oldpos;
  int sub_pos = 0;  // По умолчанию выбираем состояние будильника
  int day_pos = 0;  // Позиция выбранного дня недели (0-6)
  if (sub_pos != 2) showSubItem(sub_pos);
  clearDay(numAl, true);

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();
    // Навигация по пунктам меню
    if (sub_pos != 2) {
      if (enc1.isLeft() || pult_code == up) {
        if (LedOn()) break;
        sub_pos--;
        if (sub_pos < 0) {
          sub_pos = 2;
        }
        if (sub_pos != 2) {
          showSubItem(sub_pos);
          clearDay(numAl, true);
        } else {
          tft.fillRect(27, 53, 16, 8, sys_color[7]);
          day_pos = 6;
          oldpos = true;
          drawDayCursor(day_pos);
        }
      }
      if (enc1.isRight() || pult_code == down) {
        if (LedOn()) break;
        sub_pos++;
        if (sub_pos > 2) {
          sub_pos = 0;
        }
        if (sub_pos != 2) {
          showSubItem(sub_pos);
          clearDay(numAl, true);
        } else {
          tft.fillRect(27, 73, 16, 8, sys_color[7]);
          day_pos = 0;
          oldpos = true;
          drawDayCursor(day_pos);
        }
      }
    }
    // Переключение между днями недели (только когда sub_pos == 2)
    if (sub_pos == 2) {
      if (enc1.isRight() || pult_code == down) {
        if (LedOn()) break;
        tft.fillRect(35, 105, 122, 8, sys_color[7]);
        if (!oldpos)
          day_pos++;
        else
          oldpos = false;
        drawDayCursor(day_pos);
        if (day_pos > 6) {
          tft.fillRect(35, 105, 122, 8, sys_color[7]);
          sub_pos = 0;
          showSubItem(sub_pos);
          clearDay(numAl, true);
        }
      }
      if (enc1.isLeft() || pult_code == up) {
        if (LedOn()) break;
        tft.fillRect(35, 105, 122, 8, sys_color[7]);
        if (!oldpos)
          day_pos--;
        else
          oldpos = false;
        if (day_pos < 0) drawDayCursor(0);
        else
          drawDayCursor(day_pos);
        if (day_pos < 0) {
          tft.fillRect(35, 105, 122, 8, sys_color[7]);
          sub_pos = 1;
          showSubItem(sub_pos);
          clearDay(numAl, true);
        }
      }
      if (enc1.isLeftH() || enc1.isRightH() || pult_code == prev || pult_code == next) {
        if (LedOn()) break;
        switch (sub_pos) {
          case 2:  // Дни недели
            {
              // Переключаем текущий день недели
              waikup_day[numAl][day_pos] = !waikup_day[numAl][day_pos];

              // Обновляем отображение дней
              clearDay(numAl, true);
            }
            break;
        }
      }
    }
    // Изменение значений
    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:  // Состояние будильника
          waikup_flag[numAl] = !waikup_flag[numAl];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("Состояние: "));
          menuSwitch(waikup_flag[numAl]);
          break;

        case 1:  // Время будильника
          {
            char oldTime[6];
            sprintf(oldTime, "%02d:%02d", wHour[numAl], wMinute[numAl]);
            tft.setTextColor(sys_color[7]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(oldTime) + " ]");
            wMinute[numAl] += 5;
            if (wMinute[numAl] >= 60) {
              wMinute[numAl] = 0;
              wHour[numAl]++;
              if (wHour[numAl] >= 24) wHour[numAl] = 0;
            }

            char newTime[6];
            sprintf(newTime, "%02d:%02d", wHour[numAl], wMinute[numAl]);
            tft.setTextColor(sys_color[3]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(newTime) + " ]");
          }
          break;
      }
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:  // Состояние будильника
          waikup_flag[numAl] = !waikup_flag[numAl];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("Состояние: "));
          menuSwitch(waikup_flag[numAl]);
          break;

        case 1:  // Время будильника
          {
            char oldTime[6];
            sprintf(oldTime, "%02d:%02d", wHour[numAl], wMinute[numAl]);
            tft.setTextColor(sys_color[7]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(oldTime) + " ]");

            wMinute[numAl] -= 5;
            if (wMinute[numAl] < 0) {
              wMinute[numAl] = 55;
              wHour[numAl]--;
              if (wHour[numAl] < 0) wHour[numAl] = 23;
            }

            char newTime[6];
            sprintf(newTime, "%02d:%02d", wHour[numAl], wMinute[numAl]);
            tft.setTextColor(sys_color[3]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(newTime) + " ]");
          }
          break;
      }
    }

    // Переключение номера будильника через шапку
    if (enc1.isReleaseHold() || pult_code == randoms) {
      if (LedOn()) break;

      // Переключаем на следующий будильник
      numAl++;
      if (numAl >= MAX_ALARMS) numAl = 0;

      // Обновляем шапку с новым номером будильника
      showSubTitle("Будильник вкл. " + String(numAl + 1));

      // Обновляем все поля для нового будильника
      tft.setCursor(42, 60);
      tft.setTextColor(sys_color[2]);
      printMenuText(F("Состояние: "));
      menuSwitch(waikup_flag[numAl]);

      char newTime[6];
      sprintf(newTime, "%02d:%02d", wHour[numAl], wMinute[numAl]);
      tft.setCursor(42, 80);
      tft.setTextColor(sys_color[2]);
      printMenuText("Время: [ " + String(newTime) + " ]");

      // Обновляем индикатор выбранного дня
      day_pos = 0;  // Сбрасываем позицию дня при смене будильника
      if (sub_pos == 2) drawDayCursor(day_pos);

      showSubItem(sub_pos);
      clearDay(numAl, true);
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

void clearDay(int dayValue, bool isWakeUp) {
  tft.fillRect(27, 94, 128, 8, sys_color[7]);
  tft.setCursor(32, 100);  // 94 + 6 = 100
  if (isWakeUp) {
    getDaysString(waikup_day[dayValue]);
  } else {
    getDaysString(sleep_day[dayValue]);
  }
}

void showNnumMenuoff() {
  for (int i = 0; i < MAX_ALARMS; i++) {
    if (sleep_day[i][7]) {
      sleep_day[i][7] = false;
      for (int y = 0; y < 7; y++) {
        sleep_day[i][y] = true;
      }
    }
  }
  showSubTitle("Будильник выкл. " + String(numAl2 + 1));

  // Отображаем состояние будильника
  tft.setCursor(42, 60);
  tft.setTextColor(sys_color[2]);
  printMenuText(F("Состояние: "));
  menuSwitch(sleep_flag[numAl2]);

  // Отображаем время срабатывания
  char alarmTime[6];
  sprintf(alarmTime, "%02d:%02d", sHour[numAl2], sMinute[numAl2]);
  tft.setCursor(42, 80);
  tft.setTextColor(sys_color[2]);
  printMenuText("Время: [ " + String(alarmTime) + " ]");

  // Отображаем дни недели
  tft.setCursor(32, 100);
  bool oldpos;
  int sub_pos = 0;  // По умолчанию выбираем состояние будильника
  int day_pos = 0;  // Позиция выбранного дня недели (0-6)
  if (sub_pos != 2) showSubItem(sub_pos);
  clearDay(numAl2, false);

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();
    // Навигация по пунктам меню
    if (sub_pos != 2) {
      if (enc1.isLeft() || pult_code == up) {
        if (LedOn()) break;
        sub_pos--;
        if (sub_pos < 0) {
          sub_pos = 2;
        }
        if (sub_pos != 2) {
          showSubItem(sub_pos);
          clearDay(numAl2, false);
        } else {
          tft.fillRect(27, 53, 16, 8, sys_color[7]);
          day_pos = 6;
          oldpos = true;
          drawDayCursor(day_pos);
        }
      }
      if (enc1.isRight() || pult_code == down) {
        if (LedOn()) break;
        sub_pos++;
        if (sub_pos > 2) {
          sub_pos = 0;
        }
        if (sub_pos != 2) {
          showSubItem(sub_pos);
          clearDay(numAl2, false);
        } else {
          tft.fillRect(27, 73, 16, 8, sys_color[7]);
          day_pos = 0;
          oldpos = true;
          drawDayCursor(day_pos);
        }
      }
    }
    // Переключение между днями недели (только когда sub_pos == 2)
    if (sub_pos == 2) {
      if (enc1.isRight() || pult_code == down) {
        if (LedOn()) break;
        tft.fillRect(35, 105, 122, 8, sys_color[7]);
        if (!oldpos)
          day_pos++;
        else
          oldpos = false;
        drawDayCursor(day_pos);
        if (day_pos > 6) {
          tft.fillRect(35, 105, 122, 8, sys_color[7]);
          sub_pos = 0;
          showSubItem(sub_pos);
          clearDay(numAl2, false);
        }
      }
      if (enc1.isLeft() || pult_code == up) {
        if (LedOn()) break;
        tft.fillRect(35, 105, 122, 8, sys_color[7]);
        if (!oldpos)
          day_pos--;
        else
          oldpos = false;
        if (day_pos < 0) drawDayCursor(0);
        else
          drawDayCursor(day_pos);
        if (day_pos < 0) {
          tft.fillRect(35, 105, 122, 8, sys_color[7]);
          sub_pos = 1;
          showSubItem(sub_pos);
          clearDay(numAl2, false);
        }
      }
      if (enc1.isLeftH() || enc1.isRightH() || pult_code == prev || pult_code == next) {
        if (LedOn()) break;
        switch (sub_pos) {
          case 2:  // Дни недели
            {
              // Переключаем текущий день недели
              sleep_day[numAl2][day_pos] = !sleep_day[numAl2][day_pos];

              // Обновляем отображение дней
              clearDay(numAl2, false);
            }
            break;
        }
      }
    }
    // Изменение значений
    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:  // Состояние будильника
          sleep_flag[numAl2] = !sleep_flag[numAl2];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("Состояние: "));
          menuSwitch(sleep_flag[numAl2]);
          break;

        case 1:  // Время будильника
          {
            char oldTime[6];
            sprintf(oldTime, "%02d:%02d", sHour[numAl2], sMinute[numAl2]);
            tft.setTextColor(sys_color[7]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(oldTime) + " ]");

            sMinute[numAl2] += 5;
            if (sMinute[numAl2] >= 60) {
              sMinute[numAl2] = 0;
              sHour[numAl2]++;
              if (sHour[numAl2] >= 24) sHour[numAl2] = 0;
            }

            char newTime[6];
            sprintf(newTime, "%02d:%02d", sHour[numAl2], sMinute[numAl2]);
            tft.setTextColor(sys_color[1]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(newTime) + " ]");
          }
          break;
      }
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:  // Состояние будильника
          sleep_flag[numAl2] = !sleep_flag[numAl2];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("Состояние: "));
          menuSwitch(sleep_flag[numAl2]);
          break;

        case 1:  // Время будильника
          {
            char oldTime[6];
            sprintf(oldTime, "%02d:%02d", sHour[numAl2], sMinute[numAl2]);
            tft.setTextColor(sys_color[7]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(oldTime) + " ]");

            sMinute[numAl2] -= 5;
            if (sMinute[numAl2] < 0) {
              sMinute[numAl2] = 55;
              sHour[numAl2]--;
              if (sHour[numAl2] < 0) sHour[numAl2] = 23;
            }

            char newTime[6];
            sprintf(newTime, "%02d:%02d", sHour[numAl2], sMinute[numAl2]);
            tft.setTextColor(sys_color[1]);
            tft.setCursor(42, 80);
            printMenuText("Время: [ " + String(newTime) + " ]");
          }
          break;
      }
    }

    // Переключение номера будильника через шапку
    if (enc1.isReleaseHold() || pult_code == randoms) {
      if (LedOn()) break;

      // Переключаем на следующий будильник
      numAl2++;
      if (numAl2 >= MAX_ALARMS) numAl2 = 0;

      // Обновляем шапку с новым номером будильника
      showSubTitle("Будильник выкл. " + String(numAl2 + 1));

      // Обновляем все поля для нового будильника
      tft.setCursor(42, 60);
      tft.setTextColor(sys_color[2]);
      printMenuText(F("Состояние: "));
      menuSwitch(sleep_flag[numAl2]);

      char newTime[6];
      sprintf(newTime, "%02d:%02d", sHour[numAl2], sMinute[numAl2]);
      tft.setCursor(42, 80);
      tft.setTextColor(sys_color[2]);
      printMenuText("Время: [ " + String(newTime) + " ]");

      // Обновляем индикатор выбранного дня
      day_pos = 0;  // Сбрасываем позицию дня при смене будильника
      if (sub_pos == 2) drawDayCursor(day_pos);

      showSubItem(sub_pos);
      clearDay(numAl2, false);
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

// Функция для отрисовки курсора под днем недели
void drawDayCursor(int day_pos) {
  tft.setFont(&mMedia6pt8b);
  tft.setCursor(35 + (day_pos * 18), 113);
  tft.setTextColor(ST77XX_ORANGE);
  tft.print("^");
}

void showOffMenu() {
  char fHour[2], fMinute[2];
  showSubTitle("Будильник выкл. " + String(numAl2 + 1));
  printMenuText(F("состояние "));
  menuSwitch(sleep_flag[numAl2]);
  tft.setCursor(42, 80);  //+20
  sprintf(fHour, "%02d", sHour[numAl2]);
  printMenuText("часы: [ " + String(fHour) + " ]");
  tft.setCursor(42, 100);  //+20
  sprintf(fMinute, "%02d", sMinute[numAl2]);
  printMenuText("минуты: [ " + String(fMinute) + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) break;
      sub_pos--;
      if (sub_pos < 0) sub_pos = 2;
      showSubItem(sub_pos);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) break;
      sub_pos++;
      if (sub_pos > 2) sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          sleep_flag[numAl2] = !sleep_flag[numAl2];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("состояние "));
          menuSwitch(sleep_flag[numAl2]);
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", sHour[numAl2]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          sHour[numAl2]++;
          if (sHour[numAl2] > 23) sHour[numAl2] = 0;
          tft.setTextColor(sys_color[1]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", sHour[numAl2]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          break;
        case 2:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", sMinute[numAl2]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          sMinute[numAl2]++;
          if (sMinute[numAl2] > 59) sMinute[numAl2] = 0;
          tft.setTextColor(sys_color[1]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", sMinute[numAl2]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          break;
      }
    }
    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          sleep_flag[numAl2] = !sleep_flag[numAl2];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("состояние "));
          menuSwitch(sleep_flag[numAl2]);
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", sHour[numAl2]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          sHour[numAl2]--;
          if (sHour[numAl2] < 0) sHour[numAl2] = 23;
          tft.setTextColor(sys_color[1]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", sHour[numAl2]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          break;
        case 2:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", sMinute[numAl2]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          sMinute[numAl2]--;
          if (sMinute[numAl2] < 0) sMinute[numAl2] = 59;
          tft.setTextColor(sys_color[1]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", sMinute[numAl2]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          break;
      }
    }
    if (enc1.isReleaseHold() || pult_code == randoms) {
      if (LedOn()) break;

      // Очищаем старые данные

      // Переключаем на следующий будильник
      numAl2++;
      if (numAl2 >= MAX_ALARMS) numAl2 = 0;

      showSubTitle("Будильник выкл. " + String(numAl2 + 1));
      printMenuText(F("состояние "));
      menuSwitch(sleep_flag[numAl2]);
      tft.setCursor(42, 80);  //+20
      sprintf(fHour, "%02d", sHour[numAl2]);
      printMenuText("часы: [ " + String(fHour) + " ]");
      tft.setCursor(42, 100);  //+20
      sprintf(fMinute, "%02d", sMinute[numAl2]);
      printMenuText("минуты: [ " + String(fMinute) + " ]");
      int sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

void showOnMenu() {
  char fHour[2], fMinute[2];
  showSubTitle("Будильник вкл. " + String(numAl + 1));
  printMenuText(F("состояние "));
  menuSwitch(waikup_flag[numAl]);
  tft.setCursor(42, 80);  //+20
  sprintf(fHour, "%02d", wHour[numAl]);
  printMenuText("часы: [ " + String(fHour) + " ]");
  tft.setCursor(42, 100);  //+20
  sprintf(fMinute, "%02d", wMinute[numAl]);
  printMenuText("минуты: [ " + String(fMinute) + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) break;
      sub_pos--;
      if (sub_pos < 0) sub_pos = 2;
      showSubItem(sub_pos);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) break;
      sub_pos++;
      if (sub_pos > 2) sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          waikup_flag[numAl] = !waikup_flag[numAl];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("состояние "));
          menuSwitch(waikup_flag[numAl]);
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", wHour[numAl]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          wHour[numAl]++;
          if (wHour[numAl] > 23) wHour[numAl] = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", wHour[numAl]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          break;
        case 2:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", wMinute[numAl]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          wMinute[numAl]++;
          if (wMinute[numAl] > 59) wMinute[numAl] = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", wMinute[numAl]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          break;
      }
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          waikup_flag[numAl] = !waikup_flag[numAl];
          tft.setTextColor(sys_color[2]);
          tft.setCursor(42, 60);
          printMenuText(F("состояние "));
          menuSwitch(waikup_flag[numAl]);
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", wHour[numAl]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          wHour[numAl]--;
          if (wHour[numAl] < 0) wHour[numAl] = 23;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          sprintf(fHour, "%02d", wHour[numAl]);
          printMenuText("часы: [ " + String(fHour) + " ]");
          break;
        case 2:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", wMinute[numAl]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          wMinute[numAl]--;
          if (wMinute[numAl] < 0) wMinute[numAl] = 59;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 100);
          sprintf(fMinute, "%02d", wMinute[numAl]);
          printMenuText("минуты: [ " + String(fMinute) + " ]");
          break;
      }
    }
    if (enc1.isReleaseHold() || pult_code == randoms) {
      if (LedOn()) break;

      // Очищаем старые данные

      // Переключаем на следующий будильник
      numAl++;
      if (numAl >= MAX_ALARMS) numAl = 0;

      showSubTitle("Будильник вкл. " + String(numAl + 1));
      printMenuText(F("состояние "));
      menuSwitch(waikup_flag[numAl]);
      tft.setCursor(42, 80);  //+20
      sprintf(fHour, "%02d", wHour[numAl]);
      printMenuText("часы: [ " + String(fHour) + " ]");
      tft.setCursor(42, 100);  //+20
      sprintf(fMinute, "%02d", wMinute[numAl]);
      printMenuText("минуты: [ " + String(fMinute) + " ]");
      int sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

void showTimeZoneMenu() {
  int fTimeZone = time_zone / 3600;
  showSubTitle(F("Часовая зона"));
  tft.setTextColor(sys_color[3]);
  tft.setCursor(42, 60);
  printMenuText("[ GMT+" + String(fTimeZone) + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isRight() || pult_code == next) {
      if (LedOn()) break;
      tft.setTextColor(sys_color[7]);
      tft.setCursor(42, 60);
      printMenuText("[ GMT+" + String(fTimeZone) + " ]");
      fTimeZone++;
      if (fTimeZone > 23) fTimeZone = 0;
      tft.setTextColor(sys_color[3]);
      tft.setCursor(42, 60);
      printMenuText("[ GMT+" + String(fTimeZone) + " ]");
    }

    if (enc1.isLeft() || pult_code == prev) {
      if (LedOn()) break;
      tft.setTextColor(sys_color[7]);
      tft.setCursor(42, 60);
      printMenuText("[ GMT+" + String(fTimeZone) + " ]");
      fTimeZone--;
      if (fTimeZone < 0) fTimeZone = 23;
      tft.setTextColor(sys_color[3]);
      tft.setCursor(42, 60);
      printMenuText("[ GMT+" + String(fTimeZone) + " ]");
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == randoms || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      time_zone = fTimeZone * 3600;
      return;
    }
    esp_task_wdt_reset();
  }
}

void showIndexMenu() {
  showSubTitle(F("Индексирование"));
  tft.setCursor(42, 60);  //+20
  tft.setTextColor(sys_color[3]);
  printMenuText(F("[ запустить ]"));
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isSingle() || pult_code == randoms || pult_code == next) {
      if (LedOn()) break;
      tft.fillRect(27, 70, 130, 15, sys_color[7]);
      tft.setCursor(42, 80);
      tft.setTextColor(sys_color[2]);
      printMenuText(F("Индексирую..."));
      IndexMusicFiles();
      tft.setCursor(42, 80);
      tft.fillRect(27, 70, 130, 15, sys_color[7]);
      printMenuText(F("Процесс завершен"));
    }

    if (butt1.hasClicks() || pult_code == prev || pult_code == menus) {
      if (LedOn()) break;
      sub_menu_pos = 0;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

void showSubTitle(String sm_title) {
  tft.fillRect(26, 26, 132, 90, sys_color[7]);
  tft.fillRect(26, 26, 132, 15, sys_color[2]);
  tft.drawRect(26, 26, 132, 90, sys_color[2]);
  tft.setFont(&mMedia6pt8b);
  tft.setCursor(32, 36);
  tft.setTextColor(sys_color[0]);
  printMenuText(sm_title);
  tft.setTextColor(sys_color[2]);
  tft.setCursor(42, 60);
}

void showSubItem(int s_pos) {
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(sys_color[7]);
  for (int i = 0; i < 3; i++) {
    tft.setCursor(26, 60 + (i * 20));
    tft.print("=");
  }
  tft.setTextColor(ST77XX_ORANGE);
  tft.setCursor(26, 60 + (s_pos * 20));
  tft.print("=");
  tft.setFont(&mMedia6pt8b);
}

void showLed() {
  showSubTitle(F("Яркость экрана"));
  tft.setTextColor(sys_color[3]);
  tft.setCursor(42, 60);
  printMenuText("Яркость: [ " + String(cur_led) + " ]");
  tft.setCursor(42, 80);  //+20
  printMenuText("Выкл. (мин.): [ " + String(led_time) + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) break;
      sub_pos--;
      if (sub_pos < 0) sub_pos = 1;
      showSubItem(sub_pos);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) break;
      sub_pos++;
      if (sub_pos > 1) sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 60);
          printMenuText("Яркость: [ " + String(cur_led) + " ]");
          cur_led++;
          if (cur_led > 25) cur_led = 25;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 60);
          printMenuText("Яркость: [ " + String(cur_led) + " ]");
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("Выкл. (мин.): [ " + String(led_time) + " ]");
          led_time++;
          if (led_time > 30) led_time = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("Выкл. (мин.): [ " + String(led_time) + " ]");
          break;
      }
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 60);
          printMenuText("Яркость: [ " + String(cur_led) + " ]");
          cur_led--;
          if (cur_led < 1) cur_led = 1;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 60);
          printMenuText("Яркость: [ " + String(cur_led) + " ]");
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("Выкл. (мин.): [ " + String(led_time) + " ]");
          led_time--;
          if (led_time < 0) led_time = 30;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("Выкл. (мин.): [ " + String(led_time) + " ]");
          break;
      }
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == randoms || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

void showSoundMenu() {
  showSubTitle(F("Эквалайзер"));
  tft.setTextColor(sys_color[3]);
  tft.setCursor(42, 60);  //+20
  printMenuText("НЧ: [ " + String(low_freq) + " ]");
  tft.setCursor(42, 80);  //+20
  printMenuText("СЧ: [ " + String(mid_freq) + " ]");
  tft.setCursor(42, 100);  //+20
  printMenuText("ВЧ: [ " + String(hi_freq) + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) break;
      sub_pos--;
      if (sub_pos < 0) sub_pos = 2;
      showSubItem(sub_pos);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) break;
      sub_pos++;
      if (sub_pos > 2) sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 60);
          printMenuText("НЧ: [ " + String(low_freq) + " ]");
          low_freq++;
          if (low_freq > 20) low_freq = 20;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 60);
          printMenuText("НЧ: [ " + String(low_freq) + " ]");
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("СЧ: [ " + String(mid_freq) + " ]");
          mid_freq++;
          if (mid_freq > 20) mid_freq = 20;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("СЧ: [ " + String(mid_freq) + " ]");
          break;
        case 2:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 100);
          printMenuText("ВЧ: [ " + String(hi_freq) + " ]");
          hi_freq++;
          if (hi_freq > 20) hi_freq = 20;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 100);
          printMenuText("ВЧ: [ " + String(hi_freq) + " ]");
          break;
      }
      audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 60);
          printMenuText("НЧ: [ " + String(low_freq) + " ]");
          low_freq--;
          if (low_freq < 0) low_freq = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 60);
          printMenuText("НЧ: [ " + String(low_freq) + " ]");
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("СЧ: [ " + String(mid_freq) + " ]");
          mid_freq--;
          if (mid_freq < 0) mid_freq = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("СЧ: [ " + String(mid_freq) + " ]");
          break;
        case 2:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 100);
          printMenuText("ВЧ: [ " + String(hi_freq) + " ]");
          hi_freq--;
          if (hi_freq < 0) hi_freq = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 100);
          printMenuText("ВЧ: [ " + String(hi_freq) + " ]");
          break;
      }
      audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == randoms || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}

void showVolumeMenu() {
  showSubTitle(F("Громкость"));
  tft.setTextColor(sys_color[3]);
  tft.setCursor(42, 60);
  printMenuText("Громкость: [ " + String(volume) + " ]");
  tft.setCursor(42, 80);  //+20
  printMenuText("Баланс (Л-П): [ " + String(balance) + " ]");
  int sub_pos = 0;
  showSubItem(sub_pos);
  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) break;
      sub_pos--;
      if (sub_pos < 0) sub_pos = 1;
      showSubItem(sub_pos);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) break;
      sub_pos++;
      if (sub_pos > 1) sub_pos = 0;
      showSubItem(sub_pos);
    }

    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 60);
          printMenuText("Громкость: [ " + String(volume) + " ]");
          volume++;
          if (volume > 21) volume = 21;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 60);
          printMenuText("Громкость: [ " + String(volume) + " ]");
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("Баланс (Л-П): [ " + String(balance) + " ]");
          balance++;
          if (balance > 16) balance = 16;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("Баланс (Л-П): [ " + String(balance) + " ]");
          break;
      }
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) break;
      switch (sub_pos) {
        case 0:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 60);
          printMenuText("Громкость: [ " + String(volume) + " ]");
          volume--;
          if (volume < 0) volume = 0;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 60);
          printMenuText("Громкость: [ " + String(volume) + " ]");
          break;
        case 1:
          tft.setTextColor(sys_color[7]);
          tft.setCursor(42, 80);
          printMenuText("Баланс (Л-П): [ " + String(balance) + " ]");
          balance--;
          if (balance < -16) balance = -16;
          tft.setTextColor(sys_color[3]);
          tft.setCursor(42, 80);
          printMenuText("Баланс (Л-П): [ " + String(balance) + " ]");
          break;
      }
    }

    if (butt1.hasClicks() || enc1.isSingle() || pult_code == randoms || pult_code == menus) {
      if (LedOn()) break;
      SettingsSubScreen(menu_pos, sub_menu_pos);
      return;
    }
    esp_task_wdt_reset();
  }
}