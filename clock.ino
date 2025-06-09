/*
  Будильник, часы с боем
*/
void ClockControl() {
  clock_timer = millis();

  //Мигание двоеточием в режиме часов
  if (sleeping == false && radio_mode == 1 && old_time != "") {
    dot_flag = !dot_flag;
    tft.setTextColor(dot_flag ? clock_color[2] : clock_color[7]);
    tft.setFont(&mMediaDigitalC32pt8b);
    tft.setTextSize(1);
    tft.setCursor((w / 2) - 4, 94);
    tft.print(":");
  }

  cHour = rtc.getTime("%H").toInt();
  cMinute = rtc.getTime("%M").toInt();
  cSecond = rtc.getTime("%S").toInt();
  currentDay = rtc.getTime("%u").toInt() - 1;

  for (int i = 0; i < MAX_ALARMS; i++) {
    if (sleep_flag[i] && sHour[i] == cHour && sMinute[i] == cMinute && slowk == false && (sleep_day[i][currentDay] || sleep_day[i][7])) flag1();

    if (waikup_flag[i] && wHour[i] == cHour && wMinute[i] == cMinute && (waikup_day[i][currentDay] || waikup_day[i][7])) flag2();
  }

  //Куранты
  if (sleeping == false && kurant_flag == true && slowk == false && skan == false) {  // && radio_mode != 2) {
    //каждый час
    if (cMinute == 59 && cSecond == 59) {
      replay_station_flag = false;
      audio_pause();
      gong_flag = true;
      if (cHour == 23) {
        EveryHour(12);
      } else {
        if (cHour > 11) cHour = cHour - 12;
        EveryHour(cHour + 1);
      }
      audio_play();
    }
    // В 15 и 45 минут
    if ((cMinute == 14 || cMinute == 44) && cSecond > 58 && cSecond <= 60) {
      replay_station_flag = false;
      audio_pause();
      if (cur_kurant == 6) {
        gong_flag = true;
        String tmp_music = "Clock/13.mp3";
        gong_end = false;
        audio.connecttoFS(SD, tmp_music.c_str());
        int ct = 0;
        while (gong_end != true) {
          vTaskDelay(20);
          esp_task_wdt_reset();
          ct++;
          if (ct > 1000) break;
        }
      } else {
        if (cur_kurant == 7) {
          speakTime(cHour, cMinute + 1);
        } else {
          gong_flag = true;
          String tmp_music = "Clock/" + String(cur_kurant * 2 + 1) + ".mp3";
          gong_end = false;
          audio.connecttoFS(SD, tmp_music.c_str());
          int ct = 0;
          while (gong_end != true) {
            vTaskDelay(20);
            esp_task_wdt_reset();
            ct++;
            if (ct > 1000) break;
          }
        }
      }
      audio_play();
    }
    //В 30 минут
    if (cMinute == 29 && cSecond > 58 && cSecond <= 60) {
      replay_station_flag = false;
      audio_pause();
      if (cur_kurant == 6) {
        gong_flag = true;
        String tmp_music = "Clock/12.mp3";
        gong_end = false;
        audio.connecttoFS(SD, tmp_music.c_str());
        int ct = 0;
        while (gong_end != true) {
          vTaskDelay(20);
          esp_task_wdt_reset();
          ct++;
          if (ct > 1000) break;
        }
      } else {
        if (cur_kurant == 7) {
          speakTime(cHour, 30);
        } else {
          gong_flag = true;
          String tmp_music = "Clock/" + String(cur_kurant * 2 + 1) + ".mp3";
          gong_end = false;
          audio.connecttoFS(SD, tmp_music.c_str());
          int ct = 0;
          while (gong_end != true) {
            vTaskDelay(20);
            esp_task_wdt_reset();
            ct++;
            if (ct > 1000) break;
          }
          gong_end = false;
          ct = 0;
          audio.connecttoFS(SD, tmp_music.c_str());
          while (gong_end != true) {
            vTaskDelay(20);
            esp_task_wdt_reset();
            ct++;
            if (ct > 1000) break;
          }
        }
      }
      audio_play();
    }
  }
}

void EveryHour(int gong) {
  if (cur_kurant == 6) {
    audio_pause();
    gong_flag = true;
    String tmp_music = "Clock/MusicBox/" + String(random(1, mus_box_count + 1)) + ".mp3";
    gong_end = false;
    audio.connecttoFS(SD, tmp_music.c_str());
    int ct = 0;
    while (gong_end != true) {
      vTaskDelay(20);
      esp_task_wdt_reset();
      ct++;
      if (ct > 1000) break;
    }
    vTaskDelay(500);
    esp_task_wdt_reset();
    audio_play();
  } else {
    audio_pause();
    gong_flag = true;
    if (cur_kurant == 7) {
      speakTime(cHour + 1, 0);
    } else {
      String tmp_music = "Clock/" + String(cur_kurant * 2) + ".mp3";
      for (int i = 0; i < gong; i++) {
        gong_end = false;
        audio.connecttoFS(SD, tmp_music.c_str());
        int ct = 0;
        while (gong_end != true) {
          vTaskDelay(20);
          esp_task_wdt_reset();
          ct++;
          if (ct > 1000) break;
        }
        vTaskDelay(500);
        esp_task_wdt_reset();
      }
    }
    audio_play();
  }
}

void audio_pause() {
  audio.setVolume(0);
  if (audio.isRunning() && fix_mode == 2) {
    fix_ptime = audio.getAudioCurrentTime();
    Serial.print(F("Fix time = "));
    Serial.println(fix_ptime);
    fix_music = music_file;
  }
  vTaskDelay(1000);
  esp_task_wdt_reset();
  audio.setVolume(volume + vk);
}

void audio_play() {
  gong_flag = false;
  if ((fix_mode == 0 || fix_mode == 2) && cur_kurant == 7) {
    int cc;
    while (speek_end == false) {
      yield();
      esp_task_wdt_reset();
      vTaskDelay(10);
      cc++;
      if (cc > 800) speek_end = true;
    }
  }
  if (!audio.isRunning() && fix_mode == 0) {
    if (pause_flag == false) replay_station_flag = true;  //ReplayStation();
  }
  if (!audio.isRunning() && fix_mode == 2) {
    if (pause_flag == false) {
      //Serial.print(F("Fix time = "));
      //Serial.println(fix_ptime);
      audio.connecttoFS(SD, fix_music.c_str());
      vTaskDelay(200);
      audio.setVolume(0);
      vTaskDelay(200);
      audio.setAudioPlayPosition(fix_ptime);
      //play_music_flag = true;
    }
  }
  pik_volume = 0;
  c_volume = 0;
}

void speakTime(int cHour, int cMinute) {
  String tHour, tMinute;
  speek_end = false;
  //Час, часы, часов
  if (cHour == 1 || cHour == 21) {
    tHour = F(" час ");
  } else {
    if (cHour == 2 || cHour == 3 || cHour == 4 || cHour == 22 || cHour == 23 || cHour == 24) {
      tHour = F(" часа ");
    } else {
      tHour = F(" часов ");
    }
  }
  //минута, минуты, минут
  if (cMinute == 1 || cMinute == 21 || cMinute == 31 || cMinute == 41 || cMinute == 51) {
    tMinute = F(" минута");
  } else {
    if (cMinute == 2 || cMinute == 3 || cMinute == 4 || cMinute == 22 || cMinute == 23 || cMinute == 24 || cMinute == 32 || cMinute == 33 || cMinute == 34 || cMinute == 42 || cMinute == 43 || cMinute == 44 || cMinute == 52 || cMinute == 53 || cMinute == 54) {
      tMinute = F(" минуты");
    } else {
      tMinute = F(" минут");
    }
  }
  String time_text = " На часах " + String(cHour) + tHour + String(cMinute) + tMinute;
  if (cMinute == 0) time_text = " На часах " + String(cHour) + tHour + " ровно";
  audio.connecttospeech(time_text.c_str(), "ru");
}

void speakDate() {
  speek_end = false;
  String date_text = "Сегодня " + String(weekDays[rtc.getDayofWeek()]) + ", " + String(rtc.getDay()) + "-е " + String(months[rtc.getMonth()]) + " " + String(rtc.getYear()) + " года.";
  audio.connecttospeech(date_text.c_str(), "ru");
}
//Выключение (сон)
void flag1() {
  if (rtc.getTime("%S").toInt() <= 5 && sleeping == false) {
    sleeping = true;
    set_led = 0;
    audio.setVolume(0);
    while (audio.getVolume() != 0) {
      vTaskDelay(10);
    }
    if (audio.isRunning()) audio.stopSong();
  }
}
//Будильник
void flag2() {
  if (rtc.getTime("%S").toInt() <= 5 && sleeping == true) {
    for (int i = 0; i < MAX_ALARMS; i++) {
      if (alarm_mode[i] == 0) {
        radio_mode = 0;
        ReplayStation();
      } else {
        radio_mode = 2;
        play_music_flag = true;
      }
    }
    fix_mode = radio_mode;
    volume = volumea;
    sleeping = false;
    set_led = cur_led;
    update_flag = true;
  }
}