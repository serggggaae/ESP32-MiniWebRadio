// ============= Управление через ИК-пульт ======================
// (вверх)(вниз)   - в режимах "Радио", "Проигрыватель" и "Часы" - увеличение / уменьшение громкости; в меню - перемещение курсора по вертикали
// (вправо)(влево) - в режимах "Радио", "Проигрыватель" и "Часы" - переключение (или предпросмотр) станций или переключение композиций; в меню - перемещение курсора по горизонтали или включение/выключение опций
// (OK)            - главное меню и подменю или переключение на выбранную станцию интернет-вещания в режиме предпросмотра станций
// (*)             - смена режима между "Радио" и "Проигрыватель"
// (#)             - включение выключение звука или выход из режима предпросмотра станций интернет-вещания
// (1)             - часы или радио / проигрыватель
// (2)             - переключение между списакми "Избранное" и "Всё" для радио и проигрывателя
// (3)             - случайная станция радио / случайная композиция проигрывателя
// (4)             - смена жанра назад
// (5)             - все жанры
// (6)             - смена жарна вперёд
// (7)             - смена локации радиостанции назад
// (8)             - все страны
// (9)             - смена локации радиостанции вперёд
// (0)             - включение / выключение

#if (PULT == 1)
void ir_tick() {

  if (IrReceiver.decode()) {
    uint a = IrReceiver.decodedIRData.command;
    vTaskDelay(50);
    IrReceiver.resume();
    if (a != IrReceiver.decodedIRData.command) {
      vTaskDelay(50);
      IrReceiver.resume();
      return;
    }
    vTaskDelay(50);
    IrReceiver.resume();
    if (a != IrReceiver.decodedIRData.command) {
      vTaskDelay(50);
      IrReceiver.resume();
      return;
    }
    vTaskDelay(50);
    IrReceiver.resume();
    if (a != IrReceiver.decodedIRData.command) {
      vTaskDelay(50);
      IrReceiver.resume();
      return;
    }
    vTaskDelay(50);
    IrReceiver.resume();
    if (a != IrReceiver.decodedIRData.command) {
      vTaskDelay(50);
      IrReceiver.resume();
      return;
    }
    if (sleeping && IrReceiver.decodedIRData.command != off) {
      vTaskDelay(100);
      IrReceiver.resume();
      return;
    }
    switch (IrReceiver.decodedIRData.command) {
      case skansss:
        skan = !skan;
        counter_st = 0;
        cur_radio_station++;
        notend();
        eeprom_timer = millis();
        eeprom_flag = true;
        break;
      default:
        break;
      case next:  // -> следующая станция/композиция
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
                clearInfo();
                repeatr1();
                cur_radio_station++;
                if (cur_radio_station > radio_station_count) cur_radio_station = 1;
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
          case 2:  //Следующая композиция
            if (music_count > 1) {
              if (preview_mode_flag == true) {
                pre_music_file++;
                if (pre_music_file > music_count) pre_music_file = 1;
                PreloadPlayer(pre_music_file, cur_music_type);
              } else {
                clearInfo();
                repeatm1();
                cur_music_file++;
                if (cur_music_file > music_count) cur_music_file = 1;
                repeatm3();
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
        break;
      case prev:  // <- предыдущая станция/композиция
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
                clearInfo();
                repeatr1();
                cur_radio_station--;
                if (cur_radio_station < 1) cur_radio_station = radio_station_count;
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
          case 2:  //Предыдущая композиция
            if (music_count > 1) {
              if (preview_mode_flag == true) {
                pre_music_file--;
                if (pre_music_file < 1) pre_music_file = music_count;
                PreloadPlayer(pre_music_file, cur_music_type);
              } else {
                clearInfo();
                repeatm1();
                cur_music_file--;
                if (cur_music_file < 1) cur_music_file = music_count;
                repeatm3();
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
        break;

      case up:  // (стрелка вверх) увеличить громкость
        if (LedOn()) return;
        volume++;
        if (volume > 21) volume = 21;
        eeprom_timer = millis();
        eeprom_flag = true;
        break;
      case down:  // (стрелка вниз) уменьшить громкость
        if (LedOn()) return;
        volume--;
        if (volume < 1) volume = 0;
        eeprom_timer = millis();
        eeprom_flag = true;
        break;
      case swap:  //(*) Смена режимов: радио - проигрыватель
        if (LedOn()) return;
        menu++;
        if (menu > 1) menu = 0;
        changeRadioMode(menu);
        break;
      case mute:  //(#) mute - unmute
        if (LedOn()) return;
        switch (fix_mode) {
          case 0:
            if (preview_mode_flag == true && preview_flag == true) {
              preview_flag = false;
              pre_radio_station = cur_radio_station;
              update_display_info();
            } else {
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
            }
            break;
          case 2:
            if (preview_mode_flag == true && preview_flag == true) {
              preview_flag = false;
              pre_music_file = cur_music_file;
              update_display_info();
            } else {
              if (music_count > 1) audio.pauseResume();
            }
            break;
        }
        break;
      case menus:  //(OK) Меню
        if (LedOn()) return;
        switch (radio_mode) {
          case 0:  //Случайное радио - перебор станций
            if (preview_mode_flag == true && preview_flag == true) {
              preview_flag = false;
              pre_radio_station = cur_radio_station;
              update_display_info();
              return;
            } else {
              IrReceiver.resume();
              mainRadioMenu();
              display_ready = false;
              update_flag = true;
              update_timer = millis();
              return;
            }
            break;
          case 1:  // В режиме часов - Озвучивание времени на часах
            audio_pause();
            speakTime(cHour, cMinute);
            audio_play();
            break;
          case 2:  //Случайная композиция - перебор треков
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
            }

            return;
            break;
        }
        break;
      case off:  //(0) Включение / выключение
        if (sleeping == false) {
          sleeping = true;
          set_led = 0;
          audio.setVolume(0);
          while (audio.getVolume() != 0) {
            vTaskDelay(10);
          }
          if (audio.isRunning()) audio.stopSong();
        } else {
          sleeping = false;
          set_led = cur_led;
          if (pause_flag == false) {
            if (fix_mode == 0) ReplayStation();
            else
              PlayMusic();
            update_flag = true;
          }
        }
        break;
      case clocks:  //(1) Переход в часы и обратно
        if (LedOn()) return;
        if (radio_mode != 1) {
          tft.fillScreen(sys_color[0]);
          VoiceMenu(F("Часы"));
          old_time = "";
          fix_mode = radio_mode;
          radio_mode = 1;
        } else {
          radio_mode = fix_mode;
        }
        update_flag = true;
        update_timer = millis();
        break;
      case fav:  //(2) Избранное / всё
        if (LedOn()) return;
        ShowInfoPanel();
        switch (radio_mode) {
          case 0:  //Радио
            audio.setVolume(0);
            cur_radio_location = 0;
            cur_radio_type = 0;
            mute_flag = true;
            favorites_flag = !favorites_flag;
            radio_station_count = CountRadioStation(cur_radio_type, cur_radio_location);
            cur_radio_station = FindRadioStation(station_ini_string);
            if (cur_radio_station == 0) cur_radio_station = 1;
            clearInfo();
            pre_radio_station = cur_radio_station;
            radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
            replay_station_flag = true;
            break;
          case 2:  //Проигрыватель
            if (favorites_flag1 == true) {
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
            } else {
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
            }
            break;
        }
        break;
      case randoms:  //(3)
        if (LedOn()) return;
        switch (fix_mode) {
          case 0:  //В режиме радио - Главное меню или загрузка выбранной станции трансляции
            if (preview_mode_flag == true && preview_flag == true) {
              repeatr1();
              audio.setVolume(0);
              cur_radio_station = pre_radio_station;
              radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
              repeatr3();
              preview_flag = false;
              display_ready = false;
            } else {
              audio.setVolume(0);
              mute_flag = true;
              repeatr1();
              cur_radio_station = random(1, radio_station_count + 1);
              if (cur_radio_station == 0 || cur_radio_station > radio_station_count) cur_radio_station = 1;
              pre_radio_station = cur_radio_station;
              repeatr3();
              if (radio_mode == 0) ShowInfoPanel();
              // вызывает PlayStation() из процесса Radio
              play_station_flag = true;
            }
            break;
          case 2:  // В режиме проигрывателя - Главное меню
            if (preview_mode_flag == true && preview_flag == true) {
              clearInfo();
              cur_music_file = pre_music_file;
              play_music_flag = true;
            } else {
              repeatm1();
              cur_music_file = random(1, music_count + 1);
              repeatm3();
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
        }
        break;
      case ganr:  //(4) Изменение жанра радио/музыки
        if (LedOn()) return;
        switch (radio_mode) {
          case 0:
            cur_radio_type--;
            if (cur_radio_type < 0) cur_radio_type = radio_type_count;
            changeRadioType();
            break;
          case 2:
            cur_music_type--;
            if (cur_music_type < 0) cur_music_type = dir_count - 1;
            changePlayerType();
            break;
        }
        break;
      case ganrall:  //(5) Все жанры радио/музыки
        if (LedOn()) return;
        switch (radio_mode) {
          case 0:
            if (cur_radio_type != 0) {
              cur_radio_type = 0;
              changeRadioType();
            }
            break;
          case 2:
            if (cur_music_type != 0) {
              cur_music_type = 0;
              changePlayerType();
            }
            break;
        }
        break;
      case ganrp:  //(6) Изменение жанра радио/музыки
        if (LedOn()) return;
        switch (radio_mode) {
          case 0:
            cur_radio_type++;
            if (cur_radio_type > radio_type_count) cur_radio_type = 0;
            changeRadioType();
            break;
          case 2:
            cur_music_type++;
            if (cur_music_type >= dir_count) cur_music_type = 0;
            changePlayerType();
            break;
        }
        break;
      case lok:  //(7) Изменение локации радио
        if (LedOn()) return;
        switch (radio_mode) {
          case 0:
            cur_radio_location--;
            if (cur_radio_location < 0) cur_radio_location = radio_location_count;
            changeRadioType();
            break;
        }
        break;
      case lokall:  //(8) Изменение локации радио - весь мир
        if (LedOn()) return;
        switch (radio_mode) {
          case 0:
            if (cur_radio_location != 0) {
              cur_radio_location = 0;
              changeRadioType();
            }
            break;
        }
        break;
      case lokp:  //(9) Изменение локации радио
        if (LedOn()) return;
        switch (radio_mode) {
          case 0:
            cur_radio_location++;
            if (cur_radio_location > radio_location_count) cur_radio_location = 0;
            changeRadioType();
            break;
        }
        break;
      case next10:  // -> следующая станция/композиция
        if (LedOn()) return;
        switch (fix_mode) {
          case 0:  //Следующая радиостанция
            if (radio_station_count > 10) {
              //Переключение станций с предпросмотром или без
              if (preview_mode_flag == true && radio_mode != 1) {
                pre_radio_station = pre_radio_station + 10;
                if (pre_radio_station > radio_station_count) pre_radio_station = 1;
                PreviewStation();
              } else {
                clearInfo();
                repeatr1();
                cur_radio_station = cur_radio_station + 10;
                if (cur_radio_station > radio_station_count) cur_radio_station = 1;
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
          case 2:  //Следующая композиция
            if (music_count > 10) {
              if (preview_mode_flag == true) {
                pre_music_file = pre_music_file + 10;
                if (pre_music_file > music_count) pre_music_file = 1;
                PreloadPlayer(pre_music_file, cur_music_type);
              } else {
                clearInfo();
                repeatm1();
                cur_music_file = cur_music_file + 10;
                if (cur_music_file > music_count) cur_music_file = 1;
                repeatm3();
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
        break;
      case prev10:  // <- предыдущая станция/композиция - 10
        if (LedOn()) return;
        switch (fix_mode) {
          case 0:  //Предыдущая радиостанция
            if (radio_station_count > 10) {
              //Переключение станций с предпросмотром или без
              if (preview_mode_flag == true && radio_mode != 1) {
                pre_radio_station = pre_radio_station - 10;
                if (pre_radio_station < 1) pre_radio_station = radio_station_count;
                PreviewStation();
              } else {
                clearInfo();
                repeatr1();
                cur_radio_station = cur_radio_station - 10;
                if (cur_radio_station < 1) cur_radio_station = radio_station_count;
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
          case 2:  //Предыдущая композиция
            if (music_count > 10) {
              if (preview_mode_flag == true) {
                pre_music_file = pre_music_file - 10;
                if (pre_music_file < 1) pre_music_file = music_count;
                PreloadPlayer(pre_music_file, cur_music_type);
              } else {
                clearInfo();
                repeatm1();
                cur_music_file = cur_music_file - 10;
                if (cur_music_file < 1) cur_music_file = music_count;
                repeatm3();
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
        break;
      case repeat:  // повтор
        if (LedOn()) return;
        switch (fix_mode) {
          case 0:  //повтор радиостанции
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
            break;
          case 2:  //повтор композиции
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
              //eeprom_timer = millis();
              //eeprom_flag = true;
            }
            break;
          default:
            break;
        }
        break;
    }
    vTaskDelay(100);
    IrReceiver.resume();
  }
}
int pult_tick() {
  if (IrReceiver.decode()) {
    LedOn();
    vTaskDelay(100);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      IrReceiver.resume();
      return 0;
    } else {
      IrReceiver.resume();
      return IrReceiver.decodedIRData.command;
    }
  } else
    return 0;
}
#else
int pult_tick() {
  return 0;
}
#endif
