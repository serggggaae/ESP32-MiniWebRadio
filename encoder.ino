/*
  Энкодер и кнопка управления
*/

// Обработка команд от кнопки
void CheckButton() {
  butt1.tick();

  if (butt1.hasClicks() && radio_mode != 1) {
    if (butt1.getClicks() == 5) {
      block_flag = !block_flag;
      update_flag = true;
      update_timer = millis();
    } else {
      //Сообщение о блокировке управления
      if (block_flag == true) {
        LedOn();
        showBlockMessage();
      }
    }
  }

  if (block_flag == false) {
    // проверка на наличие нажатий
    if (butt1.hasClicks()) {
      if (LedOn()) return;
      reload_flag = true;
      int butt1_clicks = butt1.getClicks();
      switch (butt1_clicks) {
          //Одно нажатие на кнопку
        case 1:  //Одно нажатие
          switch (radio_mode) {
            case 0:  //В режиме радио - Главное меню
              if (preview_mode_flag == true && preview_flag == true) {
                preview_flag = false;
                pre_radio_station = cur_radio_station;
                update_display_info();
                return;
              } else {
                if (radio_station_count > 1) {
                  clearInfo();
                  if (cur_radio_station == cur_radio_station2 && cur_radio_station2 != cur_radio_station1) cur_radio_station = cur_radio_station1;
                  else if (cur_radio_station != cur_radio_station3) cur_radio_station = cur_radio_station3;
                  else if (cur_radio_station != cur_radio_station2) cur_radio_station = cur_radio_station2;
                  ShowInfoPanel();
                  PlayStation();
                  if (radio_mode == 1) {
                    update_timer = millis();
                    update_flag = true;
                    old_time = "";
                  }
                  eeprom_timer = millis();
                  eeprom_flag = true;
                }
              }
              break;
            case 1:  // В режиме часов - Озвучивание времени на часах
              audio_pause();
              speakTime(cHour, cMinute);
              audio_play();
              break;
            case 2:  // В режиме проигрывателя - Главное меню
              if (preview_mode_flag == true && preview_flag == true) {
                preview_flag = false;
                pre_music_file = cur_music_file;
                update_display_info();
                return;
              } else {
                if (music_count > 1) {
                  clearInfo();
                  if (cur_music_file == cur_music_file2 && cur_music_file2 != cur_music_file1) cur_music_file = cur_music_file1;
                  else if (cur_music_file != cur_music_file3) cur_music_file = cur_music_file3;
                  else if (cur_music_file != cur_music_file2) cur_music_file = cur_music_file2;
                  ShowInfoPanel();
                  play_music_flag = true;
                  if (radio_mode == 1) {
                    update_timer = millis();
                    update_flag = true;
                    old_time = "";
                  }
                }
              }
          }
          break;
        case 2:  // Два нажатия
          switch (radio_mode) {
            case 0:  // Переход в режим часов
              if (radio_mode != 1) {
                tft.fillScreen(sys_color[0]);
                VoiceMenu(F("Часы"));
                old_time == "";
                radio_mode = 1;
              } else {
                radio_mode = fix_mode;
              }
              update_flag = true;
              update_timer = millis();
              break;
            case 1:
              switch (fix_mode) {
                case 0:  //Радио
                  VoiceMenu(F("Радио"));
                  break;
                case 2:  //Проигрыватель
                  VoiceMenu(F("Аудио плеер"));
                  break;
              }
              radio_mode = fix_mode;
              update_flag = true;
              update_timer = millis();
              break;
            case 2:  // Переход в режим часов
              if (radio_mode != 1) {
                tft.fillScreen(sys_color[0]);
                VoiceMenu(F("Часы"));
                old_time == "";
                radio_mode = 1;
              } else {
                radio_mode = fix_mode;
              }
              update_flag = true;
              update_timer = millis();
              break;
          }
          break;
        case 3:  //Три нажатия
          switch (radio_mode) {
            case 0:  //Радио - любимые станции - все станции
              ShowInfoPanel();
              mute_flag = true;
              favorites_flag = !favorites_flag;
              cur_radio_location = 0;
              cur_radio_type = 0;
              radio_station_count = CountRadioStation(cur_radio_type, cur_radio_location);
              if (favorites_flag == true) {
                VoiceMenu(F("Любимые радиостанции"));
                cur_radio_station = 1;
              } else {
                audio.stopSong();
                VoiceMenu(F("Все радиостанции"));
                cur_radio_station = FindRadioStation(station_ini_string);
                if (cur_radio_station == 0) cur_radio_station = 1;
              }
              clearInfo();
              if (radio_station_count > 0) {
                audio.setVolume(0);
                pre_radio_station = cur_radio_station;
                radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
                replay_station_flag = true;
                update_flag = true;
                eeprom_timer = millis();
                eeprom_flag = true;
              } else {
                title = F("Радиостанций этого типа в списке нет.");
                update_display_info();
                VoiceMenu(title);
              }
              break;
            case 1:  //Озвучивание даты и дня недели
              audio_pause();
              speakDate();
              audio_play();
              break;
            case 2:  //проигрыватель - любимые - все композиции
              favorites_flag1 = !favorites_flag1;
              clearInfo();
              if (favorites_flag1 == true) {
                VoiceMenu(F("Любимые записи"));
                if (all_fav_flag == true) {
                  music_count = CountMusic(-1);
                } else {
                  music_count = CountMusic(cur_music_type);
                }
              } else {
                VoiceMenu(F("Все записи"));
                ScanFiles(cur_music_type);
              }
              if (music_count > 0) {
                if (cur_music_file > music_count) cur_music_file = music_count;
                play_music_flag = true;
              } else {
                cur_music_file = 0;
                title = F("Список музыкальных произведений пуст.");
              }
              break;
          }
          break;
        case 4:  //Четыре нажатие
          switch (radio_mode) {
            case 0:  //В режиме радио - Главное меню
              if (preview_mode_flag == true && preview_flag == true) {
                preview_flag = false;
                pre_radio_station = cur_radio_station;
                update_display_info();
                return;
              } else {
                mainRadioMenu();
                display_ready = false;
                update_flag = true;
                update_timer = millis();
                return;
              }
              break;
            case 1:  //Настройки интернет-радио
              VoiceMenu(F("Настройки интернет радио"));
              radioSettings();
              //---------------------------------------
              switch (fix_mode) {
                case 0:  //Радио
                  pause_control();
                  VoiceMenu(F("Радио"));
                  clearInfo();
                  ReplayStation();
                  break;
                case 2:  //Проигрыватель
                  VoiceMenu(F("Аудио плеер"));
                  clearInfo();
                  if (favorites_flag1 == true) {
                    if (all_fav_flag == true) {
                      music_count = CountMusic(-1);
                    } else {
                      music_count = CountMusic(cur_music_type);
                    }
                  } else {
                    ScanFiles(cur_music_type);
                  }
                  if (continue_flag == true) {
                    clearInfo();
                    play_music_flag = true;
                    int ct;
                    while (!audio.isRunning()) {
                      vTaskDelay(10);
                      esp_task_wdt_reset();
                      ct++;
                      if (ct > 300) break;
                    }
                    vTaskDelay(100);
                    audio.setAudioPlayPosition(fix_ptime);
                    ct = 0;
                    while (!audio.isRunning()) {
                      vTaskDelay(10);
                      esp_task_wdt_reset();
                      ct++;
                      if (ct > 300) break;
                    }
                    vTaskDelay(300);
                  } else {
                    if (music_count > 0) {
                      if (!audio.isRunning()) {  ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Возможно, эта проверка не нужна!
                        if (cur_music_file > music_count) cur_music_file = music_count;
                        clearInfo();
                        play_music_flag = true;
                      }
                    } else {
                      cur_music_file = 0;
                    }
                  }
                  continue_flag = false;
                  break;
              }
              radio_mode = fix_mode;
              update_flag = true;
              update_timer = millis();
              //--------------------------------------
              break;
            case 2:  // В режиме проигрывателя - Главное меню
              if (preview_mode_flag == true && preview_flag == true) {
                preview_flag = false;
                pre_music_file = cur_music_file;
                update_display_info();
                return;
              } else {
                mainPlayerMenu();
                display_ready = false;
                update_flag = true;
                update_timer = millis();
                return;
              }
              break;
          }
          break;
        case 6:  //Настройки громкости
          if (radio_mode != 1) {
            soundMenu();
            display_ready = false;
            update_flag = true;
            update_timer = millis();
          }
          break;
      }
    }

    if (butt1.isHolded() && radio_mode != 1) {
      tft.setFont(&mMediaSymbols);
      tft.setCursor(40, 66);
      switch (radio_mode) {
        case 0:  //Случайное радио - начало
          tft.fillRect(35, 50, 90, 21, radio_color[1]);
          tft.drawRect(35, 50, 90, 21, radio_color[3]);
          tft.drawRect(34, 49, 92, 23, player_color[0]);
          tft.setTextColor(radio_color[5]);
          tft.setFont(&mMediaSymbols);
          tft.print("*");
          tft.setFont();
          break;
        case 2:  //Случайная композиция - начало
          tft.fillRect(35, 50, 90, 21, player_color[1]);
          tft.drawRect(35, 50, 90, 21, player_color[3]);
          tft.drawRect(34, 49, 92, 23, player_color[0]);
          tft.setTextColor(player_color[5]);
          tft.print("*");
          tft.setFont();
          break;
      }
    }

    if (butt1.isHold()) {
      switch (radio_mode) {
        case 0:  //Случайное радио - перебор станций
          repeatr1();
          rnd_music = random(1, radio_station_count + 1);
          tft.setTextColor(radio_color[5]);
          tft.fillRect(78, 51, 46, 19, radio_color[1]);
          break;
        case 2:  //Случайная композиция - перебор треков
          repeatm1();
          rnd_music = random(1, music_count + 1);
          tft.setTextColor(player_color[5]);
          tft.fillRect(78, 51, 46, 19, player_color[1]);
          break;
      }
      tft.setFont(&mMediaBold7pt8b);
      tft.setCursor(78, 64);
      tft.print(rnd_music);
    }

    if (butt1.isRelease() && rnd_music > 0) {
      vTaskDelay(1000);
      switch (radio_mode) {
        case 0:  //Случайное радио - фиксация
          audio.setVolume(0);
          mute_flag = true;
          repeatr2();
          cur_radio_station = rnd_music;
          ShowInfoPanel();
          PlayStation();
          break;
        case 2:  //Случайная композиция - фиксация
          cur_music_file = rnd_music;
          repeatm2();
          title = " ";
          if (favorites_flag1 == true) {
            if (all_fav_flag == true) {
              music_file = LoadFavMusic(cur_music_file, -1);
            } else {
              music_file = LoadFavMusic(cur_music_file, cur_music_type);
            }
          } else {
            music_file = LoadMusic(cur_music_file, cur_music_type);
          }
          audio.connecttoFS(SD, music_file.c_str());
          break;
      }
      rnd_music = 0;
    }
  }
}

// Обработка команд с энкодера
void CheckEncoder() {
  enc1.tick();
  if (enc1.isTurn()) {  // если был совершён поворот (индикатор поворота в любую сторону)
    if (block_flag == false) {
      if (sleeping == true) {
        sleeping = false;
        set_led = cur_led;
        switch (fix_mode) {
          case 0:  //Включаем радио
            ShowInfoPanel();
            ReplayStation();
            break;
          case 2:  //Включаем проигрыватель
            ShowInfoPanel();
            play_music_flag = true;
            break;
        }
        update_flag = true;
      }
    } else {
      //Сообщение о блокировке управления
      LedOn();
      showBlockMessage();
    }
  }
  if (voice_sw_flag) {
    rh = enc1.isRightH();
    lh = enc1.isLeftH();
    rl = enc1.isRight();
    ll = enc1.isLeft();
  } else {
    rh = enc1.isRight();
    lh = enc1.isLeft();
    rl = enc1.isRightH();
    ll = enc1.isLeftH();
  }
  if (rh) {
    if (block_flag == false) {
      if (LedOn()) return;
      switch (fix_mode) {
        case 0:  //Следующая радиостанция
          if (radio_station_count > 1) {
            //Переключение станций с предпросмотром или без
            if (preview_mode_flag == true && radio_mode != 1) {
              pre_radio_station++;
              if (pre_radio_station > radio_station_count) pre_radio_station = 1;
              PreviewStation();
            } else {
              repeatr1();
              clearInfo();
              cur_radio_station++;
              if (cur_radio_station > radio_station_count) cur_radio_station = 1;
              repeatr3();
              ShowInfoPanel();
              PlayStation();
            }
            if (radio_mode == 1) {
              update_timer = millis();
              update_flag = true;
              old_time = "";
            }
            eeprom_timer = millis();
            eeprom_flag = true;
          }
          break;
        case 2:  //Следующая композиция
          if (music_count > 1) {
            if (preview_mode_flag == true) {
              pre_music_file++;
              if (pre_music_file > music_count) pre_music_file = 1;
              PreloadPlayer(pre_music_file, cur_music_type);
            } else {
              repeatm1();
              cur_music_file++;
              if (cur_music_file > music_count) cur_music_file = 1;
              repeatm3();
              clearInfo();
              ShowInfoPanel();
              play_music_flag = true;
            }
            if (radio_mode == 1) {
              update_timer = millis();
              update_flag = true;
              old_time = "";
            }
            eeprom_timer = millis();
            eeprom_flag = true;
          }
          break;
        default:
          break;
      }
    } else {
      LedOn();
      showBlockMessage();
    }
  }

  if (lh) {
    if (block_flag == false) {
      if (LedOn()) return;
      switch (fix_mode) {
        case 0:  //Предыдущая радиостанция
          if (radio_station_count > 1) {
            //Переключение станций с предпросмотром или без
            if (preview_mode_flag == true && radio_mode != 1) {
              pre_radio_station--;
              if (pre_radio_station < 1) pre_radio_station = radio_station_count;
              PreviewStation();
            } else {
              repeatr1();
              clearInfo();
              cur_radio_station--;
              if (cur_radio_station < 1) cur_radio_station = radio_station_count;
              repeatr3();
              ShowInfoPanel();
              PlayStation();
              if (radio_mode == 1) {
                update_timer = millis();
                update_flag = true;
                old_time = "";
              }
              eeprom_timer = millis();
              eeprom_flag = true;
            }
          }
          break;
        case 2:  //Предыдущая композиция
          if (music_count > 1) {
            if (preview_mode_flag == true) {
              pre_music_file--;
              if (pre_music_file < 1) pre_music_file = music_count;
              PreloadPlayer(pre_music_file, cur_music_type);
            } else {
              repeatm1();
              cur_music_file--;
              if (cur_music_file < 1) cur_music_file = music_count;
              repeatm3();
              clearInfo();
              ShowInfoPanel();
              play_music_flag = true;
            }
            if (radio_mode == 1) {
              update_timer = millis();
              update_flag = true;
              old_time = "";
            }
            eeprom_timer = millis();
            eeprom_flag = true;
          }
          break;
        default:
          break;
      }
    } else {
      LedOn();
      showBlockMessage();
    }
  }

  if (rl) {
    //Увеличение громкости
    if (block_flag == false) {
      if (LedOn()) return;
      volume++;
      if (volume > 21) volume = 21;
      eeprom_timer = millis();
      eeprom_flag = true;
    } else {
      LedOn();
      showBlockMessage();
    }
  }

  if (ll) {
    //Уменьшение громкости
    if (block_flag == false) {
      if (LedOn()) return;
      volume--;
      if (volume < 1) volume = 0;
      eeprom_timer = millis();
      eeprom_flag = true;
    } else {
      LedOn();
      showBlockMessage();
    }
  }

  // Переключение режимов или переход на выбранную радиостанцию или аудио-трек
  if (enc1.isSingle() && skip_flag == false) {
    if (block_flag == false) {
      if (LedOn()) return;
      if (preview_mode_flag == true && preview_flag == true) {
        preview_flag = false;
        audio.setVolume(0);
        switch (radio_mode) {
          case 0:  //Режим "РАДИО"
            repeatr1();
            cur_radio_station = pre_radio_station;
            radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
            repeatr3();
            ReplayStation();
            break;
          case 1:
            break;
          case 2:
            clearInfo();
            cur_music_file = pre_music_file;
            play_music_flag = true;
            break;
        }
        update_flag = true;
        eeprom_timer = millis();
        eeprom_flag = true;
      } else {
        menu++;
        if (menu > 1) menu = 0;
        changeRadioMode(menu);
      }
    } else {
      LedOn();
      showBlockMessage();
    }
  }

  // двойной клик - часы
  if (enc1.isDouble()) {
    if (block_flag == false) {
      if (LedOn()) return;
      if (radio_mode != 1) {
        tft.fillScreen(ST77XX_BLACK);
        VoiceMenu(F("Часы"));
        old_time = "";
        fix_mode = radio_mode;
        radio_mode = 1;
      } else {
        radio_mode = fix_mode;
      }
      update_flag = true;
      update_timer = millis();
    } else {
      LedOn();
      showBlockMessage();
    }
  }

  // если была удержана и энкодер не поворачивался - пауза - снятие паузы с воспроизведения радио или проигрывателя
  if (enc1.isHolded()) {
    if (block_flag == false) {
      if (LedOn()) return;
      switch (radio_mode) {
        case 0:
          if (radio_station_count > 1) {
            if (audio.isRunning()) {
              audio.setVolume(0);
              audio.stopSong();
              pause_flag = true;
            } else {
              ReplayStation();
              pause_flag = false;
            }
          }
          break;
        case 2:
          if (music_count > 1) audio.pauseResume();
          break;
      }
    } else {
      LedOn();
      showBlockMessage();
    }
  }
}

// Обработка команд с энкодера при загрузке
void CheckLoadEncoder() {
  enc1.tick();
  if (enc1.isLeft()) {
    cur_load_menu++;
    if (cur_load_menu > load_menu_count) cur_load_menu = 0;
  }
  if (enc1.isRight()) {
    cur_load_menu--;
    if (cur_load_menu < 0) cur_load_menu = load_menu_count;
  }

  if (enc1.isSingle()) {
    push_flag = true;
  }
  if (enc1.isTurn()) {
    if (cur_load_menu != old_load_menu) {
      tft.setTextColor(sys_color[2]);
      tft.setCursor(12, 24 + (14 * old_load_menu));
      tft.print(utf8rus(load_menu_title[old_load_menu]));
      tft.setTextColor(sys_color[0]);
      tft.setCursor(1, 24 + (14 * old_load_menu));
      tft.print("*");
      tft.setTextColor(sys_color[5]);
      tft.setCursor(12, 24 + (14 * cur_load_menu));
      tft.print(utf8rus(load_menu_title[cur_load_menu]));
      tft.setCursor(1, 24 + (14 * cur_load_menu));
      tft.print("*");
    }
    old_load_menu = cur_load_menu;
  }
}

void VoiceMenu(String voiceText) {
  if (voice_menu_flag == true) {
    audio_pause();
    SpeekText(voiceText, "ru");
    audio_play();
  } else {
    //audio.setVolume(0);
    //while (audio.getVolume() != 0) {
    //  vTaskDelay(10);
    //}
  }
}

void ShowInfoPanel() {
  esp_task_wdt_reset();
  if (fix_mode == 0) {
    tft.fillRect(12, 50, 134, 22, radio_color[1]);
    tft.drawRect(12, 50, 134, 22, radio_color[3]);
    tft.setTextColor(radio_color[5]);
    tft.drawRect(11, 49, 136, 24, radio_color[0]);
  } else {
    tft.fillRect(12, 50, 134, 22, player_color[1]);
    tft.drawRect(12, 50, 134, 22, player_color[3]);
    tft.setTextColor(player_color[5]);
    tft.drawRect(11, 49, 136, 24, player_color[0]);
  }
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(20, 64);
  printMenuText(F("Немного терпения..."));
}

void showBlockMessage() {
  tft.fillRect(4, 50, 152, 36, ST77XX_ERED);
  tft.drawRect(4, 50, 152, 36, ST77XX_HRED);
  tft.drawRect(3, 49, 154, 38, ST77XX_HRED);
  //tft.setFont();
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(16, 64);
  tft.setTextColor(ST77XX_HRED);
  printMenuText(F("Для разблокирования"));
  tft.setCursor(16, 78);
  printMenuText(F("нажмите кнопку 5 раз"));
}

// Главное меню в режиме Радио
void mainRadioMenu() {
  tft.drawLine(0, 29, 160, 29, radio_color[0]);
  tft.fillRect(0, 30, 160, 72, radio_color[1]);
  tft.fillRect(1, 31, 158, 13, radio_color[5]);
  tft.drawRect(0, 30, 160, 72, radio_color[2]);
  tft.drawLine(1, 88, 158, 88, radio_color[2]);
  tft.drawLine(0, 103, 160, 103, radio_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(4, 41);
  tft.setTextColor(radio_color[1]);
  printMenuText(F("Главное меню"));

  tft.setTextColor(radio_color[0]);  //2
  tft.setFont(&mMediaSymbols);

  String menu_pic[7] = { "(", "+", "$", ",", "-", "#", "\"" };
  String menu_text[7] = { "Закрыть меню", "Смена режимов", "Жанры радио", "Локация радио", "Избранное", "Настройки звука", "Общие настройки" };
  int menu_size = sizeof(menu_pic) / sizeof(menu_pic[0]) - 1;
  int corr = 0;  // - 1 пункт меню

  if (main_menu_pos > menu_size) main_menu_pos = menu_size;
  for (int i = 0; i <= menu_size; i++) {
    tft.setCursor(3 + corr + (22 * i), 66);
    if (i == main_menu_pos) {
      tft.setTextColor(radio_color[5]);
      tft.print(menu_pic[i]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[i].length())) / 2, 81);
      printMenuText(menu_text[i]);
      tft.setFont(&mMediaSymbols);
      tft.setTextColor(radio_color[0]);  //2
    } else {
      tft.print(menu_pic[i]);
    }
  }
  tft.setFont();
  tft.setTextSize(1);
  tft.setCursor(4, 92);
  tft.setTextColor(radio_color[0]);  //2
  tft.print(F("web url: "));
  tft.setTextColor(radio_color[4]);
  tft.print(WiFi.localIP());
  VoiceMenu(F("Главное меню"));

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();
    if (enc1.isLeft() || pult_code == prev) {
      if (LedOn()) return;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(radio_color[0]);  //2
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      tft.setTextColor(radio_color[1]);
      printMenuText(menu_text[main_menu_pos]);
      main_menu_pos--;
      if (main_menu_pos < 0) main_menu_pos = menu_size;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(radio_color[5]);
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      printMenuText(menu_text[main_menu_pos]);
      VoiceMenu(menu_text[main_menu_pos]);
    }

    if (enc1.isRight() || pult_code == next) {
      if (LedOn()) return;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(radio_color[0]);  //2
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      tft.setTextColor(radio_color[1]);
      printMenuText(menu_text[main_menu_pos]);
      main_menu_pos++;
      if (main_menu_pos > menu_size) main_menu_pos = 0;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(radio_color[5]);
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      printMenuText(menu_text[main_menu_pos]);
      VoiceMenu(menu_text[main_menu_pos]);
    }

    if (butt1.hasClicks()) return;
    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      switch (main_menu_pos) {
        case 0:  // Выход из Главного меню
          return;
          break;
        case 1:  //Смена режимов
          if (skip_flag == false) {
            menu++;
            if (menu > 1) menu = 0;
            changeRadioMode(menu);
          }
          return;
          break;
        case 2:  //Выбор жанра радиостанции
          RadioTypeMenu();
          return;
          break;
        case 3:  //Выбор локации радиостанции
          RadioLocationMenu();
          return;
          break;
        case 4:  //Избранное
          if (favorites_flag == true) {
            RadioUnFavMenu();
          } else {
            RadioFavMenu();
          }
          return;
          break;
        case 5:  //Настройки звука
          soundMenu();
          return;
          break;
        case 6:  //Общие настройки
          radioSettings();
          return;
          break;
      }
    }
    if (butt1.hasClicks()) return;
    esp_task_wdt_reset();
  }
}

// Главное меню в режиме Проигрывателя
void mainPlayerMenu() {
  tft.drawLine(0, 29, 160, 29, player_color[0]);
  tft.fillRect(0, 30, 160, 72, player_color[1]);
  tft.fillRect(1, 31, 158, 13, player_color[5]);
  tft.drawRect(0, 30, 160, 72, player_color[2]);
  tft.drawLine(1, 88, 158, 88, player_color[2]);
  tft.drawLine(0, 103, 160, 103, player_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(4, 41);
  tft.setTextColor(player_color[1]);
  printMenuText(F("Главное меню"));

  tft.setTextColor(player_color[0]);  //2
  tft.setFont(&mMediaSymbols);

  String menu_pic[6] = { "(", "+", "'", "-", "#", "\"" };
  String menu_text[6] = { "Закрыть меню", "Смена режима", "Жанры музыки", "Избранное", "Настройки звука", "Общие настройки" };
  int menu_size = sizeof(menu_pic) / sizeof(menu_pic[0]) - 1;
  int corr = 8;  // - 1 пункт меню

  if (main_menu_pos > menu_size) main_menu_pos = menu_size;
  for (int i = 0; i <= menu_size; i++) {
    tft.setCursor(3 + corr + (22 * i), 66);
    if (i == main_menu_pos) {
      tft.setTextColor(player_color[5]);
      tft.print(menu_pic[i]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[i].length())) / 2, 81);
      printMenuText(menu_text[i]);
      tft.setFont(&mMediaSymbols);
      tft.setTextColor(player_color[0]);  //2
    } else {
      tft.print(menu_pic[i]);
    }
  }
  tft.setFont();
  tft.setTextSize(1);
  tft.setCursor(4, 92);
  tft.setTextColor(player_color[2]);
  tft.print(F("web url: "));
  tft.setTextColor(player_color[4]);
  tft.print(WiFi.localIP());
  VoiceMenu(F("Главное меню"));

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();
    if (enc1.isLeft() || pult_code == prev) {
      if (LedOn()) return;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(player_color[0]);  //2
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      tft.setTextColor(player_color[1]);
      printMenuText(menu_text[main_menu_pos]);
      main_menu_pos--;
      if (main_menu_pos < 0) main_menu_pos = menu_size;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(player_color[5]);
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      printMenuText(menu_text[main_menu_pos]);
      VoiceMenu(menu_text[main_menu_pos]);
    }

    if (enc1.isRight() || pult_code == next) {
      if (LedOn()) return;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(player_color[0]);  //2
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      tft.setTextColor(player_color[1]);
      printMenuText(menu_text[main_menu_pos]);
      main_menu_pos++;
      if (main_menu_pos > menu_size) main_menu_pos = 0;
      tft.setFont(&mMediaSymbols);
      tft.setCursor(3 + corr + (22 * main_menu_pos), 66);
      tft.setTextColor(player_color[5]);
      tft.print(menu_pic[main_menu_pos]);
      tft.setFont(&mMedia6pt8b);
      tft.setCursor((w - (4 * menu_text[main_menu_pos].length())) / 2, 81);
      printMenuText(menu_text[main_menu_pos]);
      VoiceMenu(menu_text[main_menu_pos]);
    }

    if (butt1.hasClicks()) return;
    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      switch (main_menu_pos) {
        case 0:  // Выход из Главного меню
          return;
          break;
        case 1:  //Смена режимов
          if (skip_flag == false) {
            menu++;
            if (menu > 1) menu = 0;
            changeRadioMode(menu);
          }
          return;
          break;
        case 2:  //Выбор жанра музыки
          PlayerTypeMenu();
          return;
          break;
        case 3:  //Избранное
          if (favorites_flag1 == true) {
            PlayerUnFavMenu();
          } else {
            PlayerFavMenu();
          }
          return;
          break;
        case 4:  //Настройки звука
          soundMenu();
          return;
          break;
        case 5:  //Общие настройки
          radioSettings();
          return;
          break;
      }
    }
    if (butt1.hasClicks()) return;
    esp_task_wdt_reset();
  }
}

void soundMenu() {
  if (radio_mode == 0) {
    tft.fillRect(6, 10, 146, 108, radio_color[1]);
    tft.fillRect(7, 11, 144, 13, radio_color[5]);
    tft.drawRect(6, 10, 146, 108, radio_color[2]);
    tft.drawRect(5, 9, 148, 110, radio_color[0]);
  } else {
    tft.fillRect(6, 10, 146, 108, player_color[1]);
    tft.fillRect(7, 11, 144, 13, player_color[5]);
    tft.drawRect(6, 10, 146, 108, player_color[2]);
    tft.drawRect(5, 9, 148, 110, player_color[0]);
  }
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 20);
  if (radio_mode == 0) {
    tft.setTextColor(radio_color[1]);
  } else {
    tft.setTextColor(player_color[1]);
  }
  printMenuText(F("Звук"));

  if (radio_mode == 0) {
    tft.setTextColor(radio_color[2]);
  } else {
    tft.setTextColor(player_color[2]);
  }
  tft.setCursor(22, 40);
  printMenuText(F("Громкость:"));
  tft.setCursor(41, 55);  // + 15
  printMenuText(F("Баланс:"));
  tft.setCursor(66, 70);
  printMenuText(F("НЧ:"));
  tft.setCursor(66, 85);
  printMenuText(F("СЧ:"));
  tft.setCursor(66, 100);
  printMenuText(F("ВЧ:"));

  tft.setFont(&mMedia6pt8b);
  if (radio_mode == 0) {
    tft.setTextColor(radio_color[5]);
  } else {
    tft.setTextColor(player_color[5]);
  }
  tft.setCursor(90, 40);
  tft.print(String(volume));
  tft.setCursor(90, 55);
  tft.print(String(balance));
  tft.setCursor(90, 70);
  tft.print(String(low_freq));
  tft.setCursor(90, 85);
  tft.print(String(mid_freq));
  tft.setCursor(90, 100);
  tft.print(String(hi_freq));

  tft.setFont(&mMediaSymbols);
  if (radio_mode == 0) {
    tft.setTextColor(radio_color[6]);
  } else {
    tft.setTextColor(player_color[6]);
  }
  tft.setCursor(110, 44);
  printMenuText("?");
  int menu_pos = 0;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isSingle() || butt1.hasClicks() || pult_code == randoms) {
      LedOn();
      return;
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) return;
      if (radio_mode == 0) {
        tft.fillRect(115, 29, 14, 77, radio_color[1]);
      } else {
        tft.fillRect(115, 29, 14, 77, player_color[1]);
      }
      menu_pos++;
      if (menu_pos > 4) menu_pos = 0;
      tft.setFont(&mMediaSymbols);
      if (radio_mode == 0) {
        tft.setTextColor(radio_color[6]);
      } else {
        tft.setTextColor(player_color[6]);
      }
      tft.setCursor(110, 44 + (15 * menu_pos));
      printMenuText("?");
    }

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) return;
      if (radio_mode == 0) {
        tft.fillRect(115, 29, 14, 77, radio_color[1]);
      } else {
        tft.fillRect(115, 29, 14, 77, player_color[1]);
      }
      menu_pos--;
      if (menu_pos < 0) menu_pos = 4;
      tft.setFont(&mMediaSymbols);
      if (radio_mode == 0) {
        tft.setTextColor(radio_color[6]);
      } else {
        tft.setTextColor(player_color[6]);
      }
      tft.setCursor(110, 44 + (15 * menu_pos));
      printMenuText("?");
    }

    if (enc1.isLeftH() || pult_code == prev) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      if (radio_mode == 0) {
        tft.setTextColor(radio_color[1]);
      } else {
        tft.setTextColor(player_color[1]);
      }
      switch (menu_pos) {
        case 0:  //Громкость
          tft.setCursor(90, 40);
          tft.print(String(volume));
          if (volume > 0) volume--;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 40);
          tft.print(String(volume));
          break;
        case 1:  //Баланс
          tft.setCursor(90, 55);
          tft.print(String(balance));
          if (balance > -16) balance--;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 55);
          tft.print(String(balance));
          audio.setBalance(balance);
          break;
        case 2:  //НЧ
          tft.setCursor(90, 70);
          tft.print(String(low_freq));
          if (low_freq > 0) low_freq--;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 70);
          tft.print(String(low_freq));
          audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
          break;
        case 3:  //СЧ
          tft.setCursor(90, 85);
          tft.print(String(mid_freq));
          if (mid_freq > 0) mid_freq--;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 85);
          tft.print(String(mid_freq));
          audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
          break;
        case 4:  //ВЧ
          tft.setCursor(90, 100);
          tft.print(String(hi_freq));
          if (hi_freq > 0) hi_freq--;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 100);
          tft.print(String(hi_freq));
          audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
          break;
      }
    }

    if (enc1.isRightH() || pult_code == next) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      if (radio_mode == 0) {
        tft.setTextColor(radio_color[1]);
      } else {
        tft.setTextColor(player_color[1]);
      }
      switch (menu_pos) {
        case 0:
          tft.setCursor(90, 40);
          tft.print(String(volume));
          if (volume < 21) volume++;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 40);
          tft.print(String(volume));
          break;
        case 1:  //Баланс
          tft.setCursor(90, 55);
          tft.print(String(balance));
          if (balance < 16) balance++;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 55);
          tft.print(String(balance));
          audio.setBalance(balance);
          break;
        case 2:  //НЧ
          tft.setCursor(90, 70);
          tft.print(String(low_freq));
          if (low_freq < 20) low_freq++;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 70);
          tft.print(String(low_freq));
          audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
          break;
        case 3:  //CЧ
          tft.setCursor(90, 85);
          tft.print(String(mid_freq));
          if (mid_freq < 20) mid_freq++;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 85);
          tft.print(String(mid_freq));
          audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
          break;
        case 4:  //ВЧ
          tft.setCursor(90, 100);
          tft.print(String(hi_freq));
          if (hi_freq < 20) hi_freq++;
          if (radio_mode == 0) {
            tft.setTextColor(radio_color[5]);
          } else {
            tft.setTextColor(player_color[5]);
          }
          tft.setCursor(90, 100);
          tft.print(String(hi_freq));
          audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
          break;
      }
    }
    esp_task_wdt_reset();
  }
}

void RadioTypeMenu() {
  tft.fillRect(6, 40, 146, 40, radio_color[1]);
  tft.fillRect(7, 41, 144, 13, radio_color[5]);
  tft.drawRect(6, 40, 146, 40, radio_color[2]);
  tft.drawRect(5, 39, 148, 42, radio_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 51);
  tft.setTextColor(radio_color[1]);
  printMenuText(F("Жанр"));
  tft.setTextColor(radio_color[3]);
  tft.setCursor(10, 70);
  printMenuText(radio_type_title[cur_radio_type]);
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(radio_color[6]);
  tft.setCursor(130, 74);
  printMenuText("?");
  int save_radio_type = cur_radio_type;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      tft.setTextColor(radio_color[1]);
      tft.setCursor(10, 70);
      printMenuText(radio_type_title[cur_radio_type]);
      cur_radio_type--;
      if (cur_radio_type < 0) cur_radio_type = radio_type_count;
      tft.setTextColor(radio_color[3]);
      tft.setCursor(10, 70);
      printMenuText(radio_type_title[cur_radio_type]);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      tft.setTextColor(radio_color[1]);
      tft.setCursor(10, 70);
      printMenuText(radio_type_title[cur_radio_type]);
      cur_radio_type++;
      if (cur_radio_type > radio_type_count) cur_radio_type = 0;
      tft.setTextColor(radio_color[3]);
      tft.setCursor(10, 70);
      printMenuText(radio_type_title[cur_radio_type]);
    }

    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      if (save_radio_type != cur_radio_type) changeRadioType();
      return;
    }

    if (butt1.hasClicks() || pult_code == prev) return;
    esp_task_wdt_reset();
  }
}

void PlayerTypeMenu() {
  tft.fillRect(6, 40, 146, 40, player_color[1]);
  tft.fillRect(7, 41, 144, 13, player_color[5]);
  tft.drawRect(6, 40, 146, 40, player_color[2]);
  tft.drawRect(5, 39, 148, 42, player_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 51);
  tft.setTextColor(player_color[1]);
  printMenuText(F("Жанр"));
  tft.setTextColor(player_color[3]);
  tft.setCursor(10, 70);
  printMenuText(music_type_title[cur_music_type]);
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(player_color[6]);
  tft.setCursor(130, 74);
  printMenuText("?");
  int save_music_type = cur_music_type;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      tft.setTextColor(player_color[1]);
      tft.setCursor(10, 70);
      printMenuText(music_type_title[cur_music_type]);
      cur_music_type--;
      if (cur_music_type < 0) cur_music_type = dir_count - 1;
      tft.setTextColor(player_color[3]);
      tft.setCursor(10, 70);
      printMenuText(music_type_title[cur_music_type]);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      tft.setTextColor(player_color[1]);
      tft.setCursor(10, 70);
      printMenuText(music_type_title[cur_music_type]);
      cur_music_type++;
      if (cur_music_type >= dir_count) cur_music_type = 0;
      tft.setTextColor(player_color[3]);
      tft.setCursor(10, 70);
      printMenuText(music_type_title[cur_music_type]);
    }

    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      if (save_music_type != cur_music_type) changePlayerType();
      return;
    }

    if (butt1.hasClicks() || pult_code == prev) return;
    esp_task_wdt_reset();
  }
}

void RadioLocationMenu() {
  tft.fillRect(6, 40, 146, 40, radio_color[1]);
  tft.fillRect(7, 41, 144, 13, radio_color[5]);
  tft.drawRect(6, 40, 146, 40, radio_color[2]);
  tft.drawRect(5, 39, 148, 42, radio_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 51);
  tft.setTextColor(radio_color[1]);
  printMenuText(F("Локация"));
  tft.setTextColor(radio_color[3]);
  tft.setCursor(10, 70);
  printMenuText(radio_location_title[cur_radio_location]);
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(radio_color[6]);
  tft.setCursor(130, 74);
  printMenuText("?");
  int save_radio_location = cur_radio_location;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      if (save_radio_location != cur_radio_location) changeRadioLocation();
      return;
    }

    if (enc1.isLeft() || pult_code == up) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      tft.setTextColor(radio_color[1]);
      tft.setCursor(10, 70);
      printMenuText(radio_location_title[cur_radio_location]);
      cur_radio_location--;
      if (cur_radio_location < 0) cur_radio_location = radio_location_count;
      tft.setTextColor(radio_color[3]);
      tft.setCursor(10, 70);
      printMenuText(radio_location_title[cur_radio_location]);
    }

    if (enc1.isRight() || pult_code == down) {
      if (LedOn()) return;
      tft.setFont(&mMedia6pt8b);
      tft.setTextColor(radio_color[1]);
      tft.setCursor(10, 70);
      printMenuText(radio_location_title[cur_radio_location]);
      cur_radio_location++;
      if (cur_radio_location > radio_location_count) cur_radio_location = 0;
      tft.setTextColor(radio_color[3]);
      tft.setCursor(10, 70);
      printMenuText(radio_location_title[cur_radio_location]);
    }
    if (butt1.hasClicks() || pult_code == prev) return;
    esp_task_wdt_reset();
  }
}

void RadioFavMenu() {
  int add_station = 0;
  tft.fillRect(6, 40, 146, 55, radio_color[1]);
  tft.fillRect(7, 41, 144, 13, radio_color[5]);
  tft.drawRect(6, 40, 146, 55, radio_color[2]);
  tft.drawRect(5, 39, 148, 57, radio_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 51);
  tft.setTextColor(radio_color[1]);
  printMenuText(F("Избранное"));
  tft.setTextColor(radio_color[3]);
  tft.setCursor(10, 70);
  printMenuText(F("Добавить в избр."));
  tft.setCursor(10, 85);
  printMenuText(F("Перейти в избр."));
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(radio_color[6]);
  tft.setCursor(130, 74);
  printMenuText("?");

  int cur_pos = 0;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (butt1.hasClicks() || pult_code == prev) {
      LedOn();
      return;
    }

    if (enc1.isLeft() || pult_code == down) {
      if (LedOn()) return;
      tft.setTextColor(radio_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos++;
      if (cur_pos > 1) cur_pos = 0;
      tft.setTextColor(radio_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isRight() || pult_code == up) {
      if (LedOn()) return;
      tft.setTextColor(radio_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos--;
      if (cur_pos < 0) cur_pos = 1;
      tft.setTextColor(radio_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      switch (cur_pos) {
        case 0:  //Добавить в избранное
          audio.setVolume(0);
          mute_flag = true;
          add_station = AddStationToFav();
          ReplayStation();
          break;
        case 1:  //Загрузить список избранных станций
          audio.setVolume(0);
          favorites_flag = true;
          cur_radio_location = 0;
          cur_radio_type = 0;
          radio_station_count = CountRadioStation(cur_radio_type, cur_radio_location);
          if (add_station == 0) {
            cur_radio_station = 1;
          } else {
            cur_radio_station = add_station;
          }
          clearInfo();
          radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
          ReplayStation();
          break;
      }
      return;
    }
    esp_task_wdt_reset();
  }
}

void PlayerFavMenu() {
  tft.fillRect(6, 40, 146, 55, player_color[1]);
  tft.fillRect(7, 41, 144, 13, player_color[5]);
  tft.drawRect(6, 40, 146, 55, player_color[2]);
  tft.drawRect(5, 39, 148, 57, player_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 51);
  tft.setTextColor(player_color[1]);
  printMenuText(F("Избранное"));
  tft.setTextColor(player_color[3]);
  tft.setCursor(10, 70);
  printMenuText(F("Добавить в избр."));
  tft.setCursor(10, 85);
  printMenuText(F("Перейти в избр."));
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(player_color[6]);
  tft.setCursor(130, 74);
  printMenuText("?");

  int cur_pos = 0;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (butt1.hasClicks() || pult_code == prev) {
      LedOn();
      return;
    }

    if (enc1.isLeft() || pult_code == down) {
      if (LedOn()) return;
      tft.setTextColor(player_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos++;
      if (cur_pos > 1) cur_pos = 0;
      tft.setTextColor(player_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isRight() || pult_code == up) {
      if (LedOn()) return;
      tft.setTextColor(player_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos--;
      if (cur_pos < 0) cur_pos = 1;
      tft.setTextColor(player_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      switch (cur_pos) {
        case 0:  //Добавить в избранное
          audio.setVolume(0);
          mute_flag = true;
          cur_music_file = AddMusicToFav();
          play_music_flag = true;
          break;
        case 1:  //Загрузить список избранных композиций
          audio.setVolume(0);
          favorites_flag1 = true;
          clearInfo();
          VoiceMenu(F("Любимые записи"));
          if (all_fav_flag == true) {
            music_count = CountMusic(-1);
          } else {
            music_count = CountMusic(cur_music_type);
          }
          if (music_count > 0) {
            if (cur_music_file == 0) cur_music_file = 1;
            if (cur_music_file > music_count) cur_music_file = music_count;
            play_music_flag = true;
          } else {
            cur_music_file = 0;
            title = F("Список музыкальных произведений пуст.");
          }
          break;
      }
      return;
    }
    esp_task_wdt_reset();
  }
}

void RadioUnFavMenu() {
  tft.fillRect(6, 40, 146, 55, radio_color[1]);
  tft.fillRect(7, 41, 144, 13, radio_color[5]);
  tft.drawRect(6, 40, 146, 55, radio_color[2]);
  tft.drawRect(5, 39, 148, 57, radio_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 51);
  tft.setTextColor(radio_color[1]);
  printMenuText(F("Избранное"));
  tft.setTextColor(radio_color[3]);
  tft.setCursor(10, 70);
  printMenuText(F("Удалить из избр."));
  tft.setCursor(10, 85);
  printMenuText(F("К полному списку"));
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(radio_color[6]);
  tft.setCursor(130, 74);
  printMenuText("?");

  int cur_pos = 0;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (butt1.hasClicks() || pult_code == prev) {
      LedOn();
      return;
    }

    if (enc1.isLeft() || pult_code == down) {
      if (LedOn()) return;
      tft.setTextColor(radio_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos++;
      if (cur_pos > 1) cur_pos = 0;
      tft.setTextColor(radio_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isRight() || pult_code == up) {
      if (LedOn()) return;
      tft.setTextColor(radio_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos--;
      if (cur_pos < 0) cur_pos = 1;
      tft.setTextColor(radio_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      switch (cur_pos) {
        case 0:  //Удалить станцию из избранного
          audio.setVolume(0);
          radio_station_count = DelStationFromFav();
          cur_radio_location = 0;
          cur_radio_type = 0;
          if (cur_radio_station > radio_station_count) cur_radio_station = radio_station_count;
          clearInfo();
          radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
          ReplayStation();
          break;
        case 1:  //Загрузить полный список станций
          audio.setVolume(0);
          favorites_flag = false;
          cur_radio_location = 0;
          cur_radio_type = 0;
          mute_flag = true;
          radio_station_count = CountRadioStation(cur_radio_type, cur_radio_location);
          cur_radio_station = FindRadioStation(station_ini_string);
          if (cur_radio_station == 0) cur_radio_station = 1;
          clearInfo();
          radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
          ReplayStation();
          break;
      }
      return;
    }
    esp_task_wdt_reset();
  }
}

void PlayerUnFavMenu() {
  tft.fillRect(6, 40, 146, 55, player_color[1]);
  tft.fillRect(7, 41, 144, 13, player_color[5]);
  tft.drawRect(6, 40, 146, 55, player_color[2]);
  tft.drawRect(5, 39, 148, 57, player_color[0]);
  tft.setFont(&mMedia6pt8b);
  tft.setTextSize(1);
  tft.setCursor(10, 51);
  tft.setTextColor(player_color[1]);
  printMenuText(F("Избранное"));
  tft.setTextColor(player_color[3]);
  tft.setCursor(10, 70);
  printMenuText(F("Удалить из избр."));
  tft.setCursor(10, 85);
  printMenuText(F("К полному списку"));
  tft.setFont(&mMediaSymbols);
  tft.setTextColor(player_color[6]);
  tft.setCursor(130, 74);
  printMenuText("?");

  int cur_pos = 0;

  for (;;) {
    enc1.tick();
    butt1.tick();
    ui.tick();
    int pult_code = pult_tick();

    if (butt1.hasClicks() || pult_code == prev) {
      LedOn();
      return;
    }

    if (enc1.isLeft() || pult_code == down) {
      if (LedOn()) return;
      tft.setTextColor(player_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos++;
      if (cur_pos > 1) cur_pos = 0;
      tft.setTextColor(player_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isRight() || pult_code == up) {
      if (LedOn()) return;
      tft.setTextColor(player_color[1]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
      cur_pos--;
      if (cur_pos < 0) cur_pos = 1;
      tft.setTextColor(player_color[6]);
      tft.setCursor(130, 74 + (15 * cur_pos));
      printMenuText("?");
    }

    if (enc1.isSingle() || pult_code == randoms) {
      if (LedOn()) return;
      switch (cur_pos) {
        case 0:  //Удалить композицию из избранного
          audio.setVolume(0);
          music_count = DelMusicFromFav();
          clearInfo();
          play_music_flag = true;
          break;
        case 1:  //Загрузить полный список композиций
          audio.setVolume(0);
          favorites_flag1 = false;
          clearInfo();
          VoiceMenu(F("Все записи"));
          ScanFiles(cur_music_type);
          if (music_count > 0) {
            if (cur_music_file == 0) cur_music_file = 1;
            if (cur_music_file > music_count) cur_music_file = music_count;
            play_music_flag = true;
          } else {
            cur_music_file = 0;
            title = F("Список музыкальных произведений пуст.");
          }
          break;
      }
      return;
    }
    esp_task_wdt_reset();
  }
}

void changeRadioType() {
  int r_s_c;
  ShowInfoPanel();
  mute_flag = true;
  r_s_c = CountRadioStation(cur_radio_type, cur_radio_location);
  VoiceMenu(radio_type_title[cur_radio_type] + ". " + radio_location_title[cur_radio_location]);
  clearInfo();
  if (r_s_c > 0) {
    radio_station_count = r_s_c;
    cur_radio_station = 1;
    audio.setVolume(0);
    radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
    eeprom_timer = millis();
    eeprom_flag = true;
  } else {
    // Предупреждение, что станций этого типа нет в списке
    if (audio.isRunning()) audio.stopSong();
    cur_radio_station = 0;
    radio_station_count = 0;
    title = F("Радиостанций этого типа в списке нет.");
    update_display_info();
    VoiceMenu(title);
  }
}

void changePlayerType() {
  if (cur_music_type >= dir_count) cur_music_type = 0;
  ScanFiles(cur_music_type);
  if (music_count > 0) {
    VoiceMenu(music_type_title[cur_music_type]);
    cur_music_file = 1;
    play_music_flag = true;
  } else {
    if (audio.isRunning()) audio.stopSong();
    cur_music_file = 0;
    music_count = 0;
    clearInfo();
    title = F("Папка с музыкой пуста.");
    update_display_info();
    VoiceMenu(title);
  }
}

void changeRadioLocation() {
  int r_s_c;
  ShowInfoPanel();
  mute_flag = true;
  r_s_c = CountRadioStation(cur_radio_type, cur_radio_location);
  VoiceMenu(radio_type_title[cur_radio_type] + ". " + radio_location_title[cur_radio_location]);
  clearInfo();
  if (r_s_c > 0) {
    radio_station_count = r_s_c;
    cur_radio_station = 1;
    audio.setVolume(0);
    radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
    eeprom_timer = millis();
    eeprom_flag = true;
  } else {
    // Предупреждение, что станций этого типа нет в списке
    if (audio.isRunning()) audio.stopSong();
    cur_radio_station = 0;
    radio_station_count = 0;
    title = F("Радиостанций этого типа в списке нет.");
    update_display_info();
    VoiceMenu(title);
  }
}

void changeRadioMode(int mode) {
  old_time = "";
  preview_flag = false;
  switch (mode) {
    case 0:  //Радио
      radio_mode = 0;
      fix_mode = 0;
      pause_control();
      VoiceMenu(F("Радио"));
      clearInfo();
      ReplayStation();
      break;
    case 1:  //Проигрыватель
      radio_mode = 2;
      fix_mode = 2;
      VoiceMenu(F("Аудио плеер"));
      clearInfo();
      if (favorites_flag1 == true) {
        if (all_fav_flag == true) {
          music_count = CountMusic(-1);
        } else {
          music_count = CountMusic(cur_music_type);
        }
      } else {
        ScanFiles(cur_music_type);
      }
      if (continue_flag == true) {
        clearInfo();
        play_music_flag = true;
        int ct;
        while (!audio.isRunning()) {
          vTaskDelay(10);
          esp_task_wdt_reset();
          ct++;
          if (ct > 300) break;
        }
        vTaskDelay(100);
        audio.setAudioPlayPosition(fix_ptime);
        ct = 0;
        while (!audio.isRunning()) {
          vTaskDelay(10);
          esp_task_wdt_reset();
          ct++;
          if (ct > 300) break;
        }
        vTaskDelay(300);
      } else {
        if (music_count > 0) {
          if (!audio.isRunning()) {  ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Возможно, эта проверка не нужна!
            if (cur_music_file == 0) cur_music_file = 1;
            if (cur_music_file > music_count) cur_music_file = music_count;
            clearInfo();
            play_music_flag = true;
          }
        } else {
          cur_music_file = 0;
        }
      }
      continue_flag = false;
      break;
  }
}

bool LedOn() {
  if (led_flag == true) {
    set_led = cur_led;
    led_flag = false;
    led_timer = millis();
    return true;
  }
  led_timer = millis();
  return false;
}
void repeatr1() {
  if (cur_radio_station != cur_radio_station2 && cur_radio_station != cur_radio_station3) {
    cur_radio_station3 = cur_radio_station2;
    cur_radio_station2 = cur_radio_station;
  }
}

void repeatm1() {
  if (cur_music_file != cur_music_file2 && cur_music_file != cur_music_file3) {
    cur_music_file3 = cur_music_file2;
    cur_music_file2 = cur_music_file;
  }
}

void repeatr2() {
  if (rnd_music != cur_radio_station2 && rnd_music != cur_radio_station3)
    cur_radio_station1 = rnd_music;
}

void repeatm2() {
  if (rnd_music != cur_music_file2 && rnd_music != cur_music_file3)
    cur_music_file1 = rnd_music;
}

void repeatr3() {
  if (cur_radio_station != cur_radio_station2 && cur_radio_station != cur_radio_station3)
    cur_radio_station1 = cur_radio_station;
}

void repeatm3() {
  if (cur_music_file != cur_music_file2 && cur_music_file != cur_music_file3)
    cur_music_file1 = cur_music_file;
}
