/*
Аудио-плеер SD-карты
*/

// Подсчёт папок с музыкой
void ScanDir(String s_dir) {
  File dir;
  dir = SD.open("/" + s_dir);
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    if (entry.isDirectory() && dir_count < DIR_LIMIT) {
      music_type_title[dir_count] = entry.name();
      dir_count++;
    }
    entry.close();
    esp_task_wdt_reset();
  }
}

void ScanFiles(int l_folder) {
  music_count = 0;
  File dir, files_lst;
  Serial.println(music_type_title[l_folder]);
  files_lst = SD.open("/Music/" + music_type_title[l_folder] + "/files.lst", FILE_READ);
  // Если музыкальные файлы не проиндексированы
  if (!files_lst) {
    dir = SD.open("/Music/" + music_type_title[l_folder]);
    while (true) {
      File entry = dir.openNextFile();
      if (!entry) break;
      if (!entry.isDirectory()) {
        music_count++;
      }
      entry.close();
      esp_task_wdt_reset();
    }
    dir.close();
  } else {
    while (true) {
      if (files_lst.available()) {
        files_lst.readStringUntil('\n');
        music_count++;
      } else {
        break;
      }
      esp_task_wdt_reset();
    }
    files_lst.close();
  }
}

String LoadMusic(int l_file, int l_folder) {
  m_type_title = music_type_title[l_folder];
  int cur_pos = 0;
  File dir, files_lst, files_lst_w;
  dir = SD.open("/Music/" + music_type_title[l_folder]);
  //Serial.print(dir.lastAccessDate());
  files_lst = SD.open("/Music/" + music_type_title[l_folder] + "/files.lst", FILE_READ);
  // Если музыкальные файлы не проиндексированы
  if (!files_lst) {
    Serial.println(F("Музыкальные файлы НЕ проиндексированы."));
    files_lst_w = SD.open("/Music/" + music_type_title[l_folder] + "/files.lst", FILE_WRITE);
    audio.setVolume(0);
    while (true) {
      File entry = dir.openNextFile();
      if (!entry) break;
      if (!entry.isDirectory() && String(entry.name()) != "files.lst") {
        yield();
        esp_task_wdt_reset();
        //---------------------------------
        /*
        scanflag = false;
        music_file = "/Music/" + music_type_title[l_folder] + "/" + entry.name();
        audio.connecttoFS(SD, music_file.c_str());
        int ww = 0;
        while (scanflag == false && ww < 200) {
          yield();
          vTaskDelay(30);
          esp_task_wdt_reset();
          ww++;
        }
        */
        //---------------------------------
        Serial.println(entry.name());
        //Serial.println(station);
        //Serial.println(title);
        files_lst_w.println(entry.name());
      }
      entry.close();
    }
    files_lst.close();
    files_lst_w.close();
    dir.close();
  }
  //Музыкальные файлы проиндексированы. Берём имя из files.lst
  Serial.println(F("Музыкальные файлы проиндексированы."));
  String rs;
  files_lst = SD.open("/Music/" + music_type_title[l_folder] + "/files.lst", FILE_READ);
  while (cur_pos != l_file) {
    esp_task_wdt_reset();
    if (files_lst.available()) {
      rs = files_lst.readStringUntil('\n');
      rs.replace('\n', ' ');
      rs.trim();
      cur_pos++;
    } else {
      rs = "";
      cur_pos = l_file;
    }
  }
  files_lst.close();
  music_file = "/Music/" + music_type_title[l_folder] + "/" + rs;
  return music_file;
}

String LoadFavMusic(int l_file, int l_folder) {
  if (sd_flag == true) {
    file = SD.open("/Music/favorites.ini", FILE_READ);
    int cur_count = 0;
    if (file) {
      while (file.available()) {
        esp_task_wdt_reset();
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        if (l_folder >= 0) {
          if (rs.indexOf("/Music/" + music_type_title[l_folder] + "/", 0) != -1) cur_count++;
        } else {
          cur_count++;
        }
        if (cur_count == l_file) {
          rs.replace('\n', ' ');
          rs.trim();
          int s_pos = rs.indexOf("/", 0) + 7;
          int f_pos = rs.indexOf("/", s_pos + 1);
          m_type_title = rs.substring(s_pos, f_pos);
          Serial.println(rs);
          file.close();
          return rs;
        }
      }
      file.close();
    } else {
      Serial.print(F("SD-card: /Music/favorites.ini not found."));
    }
  }
  return "";
}

void PlayMusic() {
  if (cur_music_file == 0) cur_music_file = 1;
  audio.setVolume(0);
  while (audio.getVolume() != 0) {
    vTaskDelay(10);
  }
  title = " ";
  //Рандомизация
  if (rand_song == true) {
    cur_music_file = random(1, music_count + 1);
  }
  rand_song = old_rand_song;
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
  pre_music_file = cur_music_file;
  preview_flag = false;
}

void PlayMusicNext() {
  cur_music_file++;
  if (cur_music_file > music_count) cur_music_file = 1;
  play_music_flag = true;
}

void IndexMusicFiles() {
  if (indexMusic == false) {
    if (audio.isRunning()) audio.stopSong();
    int ct;
    while (audio.isRunning()) {
      esp_task_wdt_reset();
      yield();
      vTaskDelay(2);
      ct++;
      if (ct > 1000) break;
    }
    indexMusic = true;
  }
}

//Индексирование музыкальных файлов на SD-карте
void IndexMusicFilesProc() {
  File dir, files_lst;
  String cur_file_lst;
  for (int i = 0; i < dir_count; ++i) {
    dir = SD.open("/Music/" + music_type_title[i]);
    cur_file_lst = "/Music/" + music_type_title[i] + "/files.lst";
    if (SD.exists(cur_file_lst)) SD.remove(cur_file_lst);
    files_lst = SD.open(cur_file_lst, FILE_WRITE);
    vTaskDelay(100);
    index_info = "Индексирую кассету:<br>[" + music_type_title[i] + "]...";
    Serial.print(index_info);
    while (true) {
      File entry = dir.openNextFile();
      if (!entry) break;
      if (!entry.isDirectory() && String(entry.name()) != "files.lst") {
        yield();
        //---------------------------------
        /*
        scanflag = false;
        music_file = "/Music/" + music_type_title[i] + "/" + entry.name();
        audio.connecttoFS(SD, music_file.c_str());
        int ww = 0;
        while (scanflag == false && ww < 200) {
          yield();
          vTaskDelay(30);
          esp_task_wdt_reset();
          ww++;
        }
        */
        //---------------------------------
        Serial.println(entry.name());
        //Serial.println(station);
        //Serial.println(title);
        files_lst.println(entry.name());
        vTaskDelay(1);
        esp_task_wdt_reset();
        vTaskDelay(1);
      }
      entry.close();
    }
    files_lst.close();
    dir.close();
    index_info = F(" Готово!");
    Serial.println(index_info);
    vTaskDelay(1);
    esp_task_wdt_reset();
  }
  index_info = F("Индексирование всех папок завершено.");
  Serial.println(index_info);
}

int CountMusic(int l_folder) {
  if (sd_flag == true) {
    file = SD.open("/Music/favorites.ini", FILE_READ);
    int cur_count = 0;
    if (file) {
      while (file.available()) {
        String rs = file.readStringUntil('\n');
        //c_pos = rs.indexOf("/", 0);
        //if ( c_pos != -1) cur_count++;
        rs.replace('\n', ' ');
        rs.trim();
        if (l_folder >= 0) {
          if (rs.indexOf("/Music/" + music_type_title[l_folder] + "/", 0) != -1) cur_count++;
        } else {
          cur_count++;
        }
        esp_task_wdt_reset();
      }
      file.close();
      return cur_count;
    } else {
      Serial.print(F("SD-card: /Music/favorites.ini not found."));
      return 0;
    }
  } else {
    return 0;
  }
}

int AddMusicToFav() {
  File tmp_file;
  String rs;
  bool not_write = false;
  int cur_pos = 0, fix_pos = 0;
  if (sd_flag == true) {
    if (SD.exists("/Music/favorites.tmp")) SD.remove("/Music/favorites.tmp");
    tmp_file = SD.open("/Music/favorites.tmp", FILE_WRITE);
    file = SD.open("/Music/favorites.ini", FILE_READ);
    if (file) {
      int f_pos, c_pos;
      while (file.available()) {
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        Serial.println(rs);
        f_pos = rs.indexOf(music_file, 0);
        c_pos = rs.indexOf("/", 0);
        if (c_pos != -1) {
          tmp_file.println(rs);
          cur_pos++;
        }
        if (f_pos != -1) {
          not_write = true;
          fix_pos = cur_pos;
        }
        esp_task_wdt_reset();
      }
      file.close();
    } else {
      Serial.print(F("SD-card: /Music/favorites.ini not found."));
    }
    if (not_write == false) {
      tmp_file.println(music_file);
      fix_pos = cur_pos + 1;
    }
    //tmp_file.println(F("--------------eof--------------"));
    tmp_file.close();
    if (SD.exists("/Music/favorites.ini")) SD.remove("/Music/favorites.ini");
    SD.rename("/Music/favorites.tmp", "/Music/favorites.ini");
  }
  return fix_pos;
}

//Удаление станции из списка любимых
int DelMusicFromFav() {
  File tmp_file;
  String rs;
  int cur_count = 0;
  if (sd_flag == true) {
    if (SD.exists("/Music/favorites.tmp")) SD.remove("/Music/favorites.tmp");
    tmp_file = SD.open("/Music/favorites.tmp", FILE_WRITE);
    file = SD.open("/Music/favorites.ini", FILE_READ);
    if (file) {
      int f_pos, c_pos;
      while (file.available()) {
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        Serial.println(rs);
        f_pos = rs.indexOf(music_file, 0);
        c_pos = rs.indexOf("/", 0);
        if (f_pos == -1) {
          if (c_pos != -1) {
            tmp_file.println(rs);
            cur_count++;
          }
        }
        esp_task_wdt_reset();
      }
      file.close();
    } else {
      Serial.print(F("SD-card: /Radio/favorites.ini not found."));
    }
    //tmp_file.println(F("--------------eof--------------"));
    tmp_file.close();
    if (SD.exists("/Music/favorites.ini")) SD.remove("/Music/favorites.ini");
    SD.rename("/Music/favorites.tmp", "/Music/favorites.ini");
  }
  return cur_count;
}

void pause_control() {
  continue_flag = false;
  if (audio.isRunning() && curl.length() > 5 && curl.indexOf("/", 0) != -1) {
    fix_ptime = audio.getAudioCurrentTime();
    Serial.print(F("Fix time = "));
    Serial.println(fix_ptime);
    fix_music = music_file;
    continue_flag = true;
  }
}


void PreloadPlayer(int pos, int l_folder) {
  preview_flag = true;
  String rs, pre_rs_url, pre_rs_title;
  int16_t tx, ty;
  uint16_t tw, th;
  int16_t fix_x, fix_y;
  int fix_pos = pos;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    if (favorites_flag1 == true) {
      file = SD.open("/Music/favorites.ini", FILE_READ);
    } else {
      file = SD.open("/Music/" + music_type_title[l_folder] + "/files.lst", FILE_READ);
    }
    int cur_pos = 0;
    if (file) {
      while (cur_pos != pos) {
        vTaskDelay(1);
        if (file.available()) {
          rs = file.readStringUntil('\n');
          rs.replace('\n', ' ');
          rs.trim();
          int f_pos = 0;
          int s_pos;
          int col = 0;
          pre_rs_title = "";
          while (f_pos != -1) {
            s_pos = f_pos;
            f_pos = rs.indexOf("|", f_pos + 1);
            String column = rs.substring(s_pos, f_pos);
            column.replace('|', ' ');
            column.trim();
            if (col == 0) pre_rs_url = column;
            if (col == 1 && column != "") pre_rs_title = column;
            col++;
          }
          cur_pos++;
        } else {
          cur_pos = pos;
        }
        esp_task_wdt_reset();
      }
      file.close();

      tft.drawLine(0, 19, 160, 19, player_color[5]);
      tft.fillRect(0, 20, 160, 12, player_color[1]);
      tft.drawLine(0, 32, 160, 32, player_color[5]);
      tft.fillRect(0, 33, 160, 77, player_color[0]);
      tft.setTextWrap(false);
      tft.setFont();

      // Позиция в списке
      tft.setTextSize(1);
      tft.setCursor(2, 22);
      tft.setTextColor(player_color[2]);
      if (sys_font == false) {
        sys_font = true;
        tft.print(utf8rus(F("Трек ")));
        sys_font = false;
      } else {
        tft.print(utf8rus(F("Трек ")));
      }
      tft.setTextColor(player_color[3]);
      tft.print(String(fix_pos));
      tft.setTextColor(player_color[2]);
      if (sys_font == false) {
        sys_font = true;
        tft.print(utf8rus(F(" из ")));
        sys_font = false;
      } else {
        tft.print(utf8rus(F(" из ")));
      }
      tft.setTextColor(player_color[3]);
      tft.print(String(music_count));

      // Название трека
      if (pre_rs_title == "") pre_rs_title = pre_rs_url;
      tft.setTextWrap(false);
      tft.setTextColor(player_color[5]);
      fix_x = 0;
      fix_y = h;
      uint8_t fix_h = 88;
      pre_rs_title.trim();
      int len_t = pre_rs_title.length();
      if (len_t > 0) {
        if (sys_font == false) {
          tft.setFont(&mMediaBold10pt8b);
          tft.setTextSize(1);
          tft.getTextBounds(pre_rs_title, 0, fix_y, &tx, &ty, &tw, &th);
          if (tw > w) {
            tft.setFont(&mMediaBold9pt8b);
            tft.getTextBounds(pre_rs_title, 0, fix_y, &tx, &ty, &tw, &th);
            if (tw > w) {
              tft.setFont(&mMediaBold8pt8b);
              tft.getTextBounds(pre_rs_title, 0, fix_y, &tx, &ty, &tw, &th);
              if (tw > w) {
                tft.setFont(&mMediaBold7pt8b);
                if (tw > w) {
                  tft.setTextWrap(true);
                  tft.getTextBounds(pre_rs_title, 0, fix_y, &tx, &ty, &tw, &th);
                  tw = w;
                }
              }
            }
          }
          fix_x = tx;
          if (th < fix_h) {
            fix_y = ((fix_y - th) / 2) + 17;
          } else {
            fix_y = 34;
          }
          tft.setCursor((w - tw) / 2, fix_y);
        } else {
          tft.setTextSize(2);
          tft.setCursor((w - (len_t * 11)) / 2, 38);  //12
          if (len_t < 9) {
            tft.setTextSize(3);
            tft.setCursor((w - (len_t * 17)) / 2, 38);  //18
          }
          if (len_t > 13) {
            tft.setTextSize(1);
            tft.setCursor((w - (len_t * 5)) / 2, 38);  //6
            if (len_t > 29) {
              tft.setTextWrap(true);
              tft.setCursor(0, 38);
            }
          }
          tft.getTextBounds(pre_rs_title, 0, 34, &tx, &ty, &tw, &th);
          // Дописать центровку по вертикали!
        }
        tft.println(utf8rus(pre_rs_title));
      }

      tft.fillRect(0, 110, 160, 18, player_color[1]);
      tft.setTextWrap(false);
      tft.setFont();
      tft.setTextSize(1);

      // Кассета (папка)
      tft.setCursor(2, 111);
      if (sys_font == false) {
        sys_font = true;
        tft.setTextColor(player_color[2]);
        tft.print(utf8rus(F("Папка: ")));
        tft.setTextColor(player_color[3]);
        if (favorites_flag1 == true) {
          tft.print(utf8rus(F("Избранное")));
        } else {
          tft.print(utf8rus(music_type_title[l_folder]));
        }
        sys_font = false;
      } else {
        tft.setTextColor(player_color[2]);
        tft.print(utf8rus(F("Папка: ")));
        tft.setTextColor(player_color[3]);
        if (favorites_flag1 == true) {
          tft.print(utf8rus(F("Избранное")));
        } else {
          tft.print(utf8rus(music_type_title[l_folder]));
        }
      }

      tft.setFont(&mMediaSymbols);
      tft.setTextColor(player_color[6]);
      tft.setCursor(141, 127);
      printMenuText("?");

      file_flag = true;
    } else {
      if (favorites_flag == true) {
        Serial.print(F("SD-card: /Music/favorites.ini not found."));
      } else {
        Serial.print("SD-card: /Music/" + music_type_title[l_folder] + "/files.lst not found.");
      }
      pre_rs_url = "";
      file_flag = true;
    }
  }
}