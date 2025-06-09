/*
Интернет-радио
*/
bool lrs_flag;

//Получение uri радиостанции из ini-файла
String LoadRadioStation(int pos, int r_type, int r_location) {
  pre_radio_station = cur_radio_station;
  preview_flag = false;
  lrs_flag = false;
  na_flag = false;
  totalTime += audio.getTotalPlayingTime() / 1000;
  retry = 0;
  slow = 0;
  String rs, rs_url;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    if (favorites_flag == true) {
      file = SD.open("/Radio/favorites.ini", FILE_READ);
    } else {
      int t_r = 0;
      for (int i = 0; i < total_radio_ini; i++) {
        t_r = t_r + rl_count[i];
        if (t_r >= pos) {
          pos = pos - t_r + rl_count[i];
          file = SD.open("/Radio/radio" + String(i) + ".ini", FILE_READ);
          break;
        }
      }
    }
    int cur_pos = 0;
    int r_t_t = 0;
    if (file) {
      while (cur_pos != pos) {
        vTaskDelay(1);
        if (file.available()) {
          rs = file.readStringUntil('\n');
          rs.replace('\n', ' ');
          rs.trim();
          station_ini_string = rs;
          int f_pos = 0;
          int s_pos;
          int col = 0;
          bool f_flag = false;
          rs_title = "";
          while (f_pos != -1) {
            s_pos = f_pos;
            f_pos = rs.indexOf("|", f_pos + 1);
            String column = rs.substring(s_pos, f_pos);
            column.replace('|', ' ');
            column.trim();
            if (col == 0) r_t_t = column.toInt();
            if (col == 0 && (column.toInt() == r_type || r_type == 0)) f_flag = true;
            if (col == 1) fix_radio_location = column.toInt();
            if (col == 1 && (column.toInt() == r_location || r_location == 0) && f_flag == true) cur_pos++;
            if (col == 2) rs_url = column;
            if (col == 3) rs_title = column;
            col++;
          }
        } else {
          rs_url = "";
          cur_pos = pos;
        }
        esp_task_wdt_reset();
      }
      file.close();
      if (r_t_t == 0) {
        cur_type_title = "* * *";
      } else {
        cur_type_title = radio_type_title[r_t_t];
      }
      fix_radio_type = r_t_t;
      if (fix_radio_location == 0) {
        cur_location_title = "* * *";
      } else {
        cur_location_title = radio_location_title[fix_radio_location];
      }
      if (rs_title != "") {
        station = rs_title;
      } else {
        station = "";
      }
      file_flag = true;
      lrs_flag = true;
      return rs_url;
    } else {
      if (favorites_flag == true) {
        Serial.print(F("SD-card: /Radio/favorites.ini not found."));
      } else {
        Serial.print(F("SD-card: /Radio/radio(i).ini not found."));
      }
      rs_url = "";
      file_flag = true;
      lrs_flag = true;
      return rs_url;
    }
  }
  lrs_flag = true;
}

//Подсчёт количества радиостанций в ini-файлах
int CountRadioStation(int r_type, int r_location) {
  int r_count = 0;
  int i = 0;
  if (favorites_flag == true) {
    r_count = CurCountRadioStation("/Radio/favorites", r_type, r_location);
  } else {
    while (SD.exists("/Radio/radio" + String(i) + ".ini")) {
      rl_count[i] = CurCountRadioStation("/Radio/radio" + String(i), r_type, r_location);
      r_count = r_count + rl_count[i];
      i++;
      esp_task_wdt_reset();
    }
    total_radio_ini = i;
  }
  return r_count;
}

//Подсчёт количества радиостанций в отдельном ini-файлe
int CurCountRadioStation(String r_file, int r_type, int r_location) {
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    file = SD.open(r_file + ".ini", FILE_READ);
    int cur_count = 0;
    if (file) {
      while (file.available()) {
        vTaskDelay(1);
        String rs = file.readStringUntil('\n');
        int f_pos = 0;
        int s_pos;
        int col = 0;
        bool f_flag = false;
        bool comma_flag = false;
        while (f_pos != -1) {
          s_pos = f_pos;
          f_pos = rs.indexOf("|", f_pos + 1);
          String column = rs.substring(s_pos, f_pos);
          column.replace('|', ' ');
          column.trim();
          if (col == 0 && (column.toInt() == r_type || r_type == 0)) f_flag = true;
          if (col == 1 && (column.toInt() == r_location || r_location == 0) && f_flag == true) comma_flag = true;
          if (comma_flag == true && col == 2) cur_count++;
          col++;
        }
        esp_task_wdt_reset();
      }
      file.close();
      file_flag = true;
      return cur_count;
    } else {
      Serial.print("SD-card: " + r_file + ".ini not found.");
      file_flag = true;
      return 0;
    }
  }
}

//Загрузка радиотрансляции
void PlayStation() {
  favorites_flag4 = favorites_flag;
  cur_radio_station4 = cur_radio_station;
  retry = 0;
  slow = 0;
  na_flag = false;
  if (lrs_flag == true) {
    station = " ";
    title = " ";
    radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
    audio.setVolume(0);
    int ct = 0;
    while (audio.getVolume() != 0) {
      esp_task_wdt_reset();
      yield();
      vTaskDelay(2);
      ct++;
      if (ct > 1000) break;
    }
  }
}

//Возобновление проигрывания радиотрансляции
void ReplayStation() {
  favorites_flag4 = favorites_flag;
  cur_radio_station4 = cur_radio_station;
  if (retry > 3) return;
  if (audio.isRunning()) audio.stopSong();
  int ct;
  while (audio.isRunning()) {
    esp_task_wdt_reset();
    yield();
    vTaskDelay(2);
    ct++;
    if (ct > 1000) break;
  }
  timeout = millis();
  timeout_flag = true;
  audio_flag = false;
  audio.connecttohost(radio_station.c_str());
  ct = 0;
  while (!audio.isRunning()) {
    esp_task_wdt_reset();
    yield();
    vTaskDelay(2);
    ct++;
    if (ct > 1000) break;
  }
  audio.setVolume(0);
  ct = 0;
  while (audio.getVolume() != 0) {
    esp_task_wdt_reset();
    yield();
    vTaskDelay(2);
    ct++;
    if (ct > 1000) break;
  }
  Serial.printf("Объём свободной памяти:\t%d байт\n", ESP.getFreeHeap());
}

//Поиск радиостанции по uri в ini-файлах
int FindRadioStation(String f_station) {
  Serial.print("STATION = ");
  Serial.println(f_station);
  if (favorites_flag == true) {
    return CurFindRadioStation("/Radio/favorites", f_station);
  } else {
    int r_count = 0;
    for (int i = 0; i < total_radio_ini; i++) {
      int result = CurFindRadioStation("/Radio/radio" + String(i), f_station);
      if (result != 0) {
        return (r_count + result);
        break;
      }
      r_count = r_count + rl_count[i];
    }
  }
  return 0;
}

//Поиск радиостанции по uri в ini-файле
int CurFindRadioStation(String r_file, String f_station) {
  int cur_pos = 0;
  int find_pos = 0;
  bool find_flag = false;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    file = SD.open(r_file + ".ini", FILE_READ);
    if (file) {
      while (file.available()) {
        vTaskDelay(1);
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        if (rs.indexOf("|", 0) != -1) cur_pos++;
        if (rs.indexOf(f_station, 0) != -1) {
          find_flag = true;
          find_pos = cur_pos;
        }
        esp_task_wdt_reset();
      }
      file.close();
    } else {
      Serial.print("SD-card: " + r_file + ".ini not found.");
    }
  }
  file_flag = true;
  if (find_flag == true) {
    return find_pos;
  } else {
    return 0;
  }
}

//Добавление станции к списку любимых
int AddStationToFav() {
  File tmp_file;
  String rs;
  bool not_write = false;
  int cur_pos = 0, fix_pos = 0;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    if (SD.exists("/Radio/favorites.tmp")) SD.remove("/Radio/favorites.tmp");
    tmp_file = SD.open("/Radio/favorites.tmp", FILE_WRITE);
    file = SD.open("/Radio/favorites.ini", FILE_READ);
    if (file) {
      int f_pos, c_pos;
      while (file.available()) {
        vTaskDelay(1);
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        f_pos = rs.indexOf(station_ini_string, 0);
        c_pos = rs.indexOf("|", 0);
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
      Serial.print(F("SD-card: /Radio/favorites.ini not found."));
    }
    if (not_write == false) {
      tmp_file.println(station_ini_string);
      fix_pos = cur_pos + 1;
    }
    tmp_file.println(F("--------------eof--------------"));
    tmp_file.close();
    if (SD.exists("/Radio/favorites.ini")) SD.remove("/Radio/favorites.ini");
    SD.rename("/Radio/favorites.tmp", "/Radio/favorites.ini");
  }
  file_flag = true;
  return fix_pos;
}

//Удаление станции из списка любимых
int DelStationFromFav() {
  File tmp_file;
  String rs;
  int cur_count = 0;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    if (SD.exists("/Radio/favorites.tmp")) SD.remove("/Radio/favorites.tmp");
    tmp_file = SD.open("/Radio/favorites.tmp", FILE_WRITE);
    file = SD.open("/Radio/favorites.ini", FILE_READ);
    if (file) {
      int f_pos, c_pos;
      while (file.available()) {
        vTaskDelay(1);
        esp_task_wdt_reset();
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        f_pos = rs.indexOf(station_ini_string, 0);
        c_pos = rs.indexOf("|", 0);
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
    tmp_file.println(F("--------------eof--------------"));
    tmp_file.close();
    if (SD.exists("/Radio/favorites.ini")) SD.remove("/Radio/favorites.ini");
    SD.rename("/Radio/favorites.tmp", "/Radio/favorites.ini");
  }
  file_flag = true;
  vTaskDelay(300);
  return cur_count;
}

int EditStation() {
  if (favorites_flag == true) {
    return CurEditStation("/Radio/favorites");
  } else {
    if (new_station == true) {
      int tmp_p = 0;
      Serial.println("Новая станция");
      //Внесение в список новой радиостанции
      if (rl_count[total_radio_ini - 1] < radio_per_list) {
        tmp_p = CurEditStation("/Radio/radio" + String(total_radio_ini - 1));
        Serial.println(tmp_p);
        rl_count[total_radio_ini - 1] = rl_count[total_radio_ini - 1] + 1;
      } else {
        tmp_p = 1;
        File tmp_file = SD.open("/Radio/radio" + String(total_radio_ini) + ".ini", FILE_WRITE);
        tmp_file.println(String(edit_radio_typ) + "|" + String(edit_radio_loc) + "|" + edit_radio_uri);
        tmp_file.println(F("--------------eof--------------"));
        tmp_file.close();
        rl_count[total_radio_ini] = 1;
      }
      if (tmp_p > 0) radio_station_count++;
      Serial.println(radio_station_count);
      return radio_station_count;
    } else {
      //Изменение существующей в списке радиостанции
      int t_r = 0;
      for (int i = 0; i < total_radio_ini; i++) {
        t_r = t_r + rl_count[i];
        if (t_r >= cur_radio_station) {
          CurEditStation("/Radio/radio" + String(i));
          return cur_radio_station;
          break;
        }
      }
    }
  }
}

//Добавление новой станции к общему списку радиостанций или редактирование существующей
int CurEditStation(String r_file) {
  File tmp_file;
  String rs;
  bool not_write = false;
  int cur_pos = 0, fix_pos = 0;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    if (SD.exists(r_file + ".tmp")) SD.remove(r_file + ".tmp");
    tmp_file = SD.open(r_file + ".tmp", FILE_WRITE);
    file = SD.open(r_file + ".ini", FILE_READ);
    if (file) {
      int f_pos, c_pos;
      while (file.available()) {
        vTaskDelay(1);
        esp_task_wdt_reset();
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        f_pos = rs.indexOf(edit_radio_uri, 0);
        c_pos = rs.indexOf("|", 0);
        if (c_pos != -1 && f_pos == -1) {
          tmp_file.println(rs);
          cur_pos++;
        }
        if (f_pos != -1) {
          not_write = true;
          cur_pos++;
          fix_pos = cur_pos;
          tmp_file.println(String(edit_radio_typ) + "|" + String(edit_radio_loc) + "|" + edit_radio_uri + "|" + edit_radio_title);
        }
        esp_task_wdt_reset();
      }
      file.close();
    } else {
      Serial.print("SD-card: " + r_file + ".ini not found.");
    }
    if (not_write == false) {
      tmp_file.println(String(edit_radio_typ) + "|" + String(edit_radio_loc) + "|" + edit_radio_uri);
      fix_pos = radio_station_count;
    }
    tmp_file.println(F("--------------eof--------------"));
    tmp_file.close();
    if (SD.exists(r_file + ".bak")) SD.remove(r_file + ".bak");
    if (SD.exists(r_file + ".ini")) SD.rename(r_file + ".ini", r_file + ".bak");
    SD.rename(r_file + ".tmp", r_file + ".ini");
  }
  file_flag = true;
  return fix_pos;
}

//Удаление станции из списка
int DelStation() {
  File tmp_file;
  String rs;
  String r_file = "";
  int cur_count = 0;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;

    if (favorites_flag == true) {
      r_file = "/Radio/favorites";
      tmp_file = SD.open(r_file + ".tmp", FILE_WRITE);
      file = SD.open(r_file + ".ini", FILE_READ);
    } else {
      int t_r = 0;
      int pos = cur_radio_station4;
      for (int i = 0; i < total_radio_ini; i++) {
        t_r = t_r + rl_count[i];
        if (t_r >= pos) {
          pos = pos - t_r + rl_count[i];
          r_file = "/Radio/radio" + String(i);
          tmp_file = SD.open(r_file + ".tmp", FILE_WRITE);
          file = SD.open(r_file + ".ini", FILE_READ);
          rl_count[i] = rl_count[i] - 1;
          break;
        }
      }
    }

    if (file) {
      int f_pos, c_pos;
      while (file.available()) {
        vTaskDelay(1);
        esp_task_wdt_reset();
        String rs = file.readStringUntil('\n');
        rs.replace('\n', ' ');
        rs.trim();
        f_pos = rs.indexOf(station_ini_string, 0);
        c_pos = rs.indexOf("|", 0);
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
      Serial.print("SD-card: " + r_file + ".ini not found.");
    }
    tmp_file.println(F("--------------eof--------------"));
    tmp_file.close();
    if (SD.exists(r_file + ".bak")) SD.remove(r_file + ".bak");
    if (SD.exists(r_file + ".ini")) SD.rename(r_file + ".ini", r_file + ".bak");
    SD.rename(r_file + ".tmp", r_file + ".ini");
  }
  file_flag = true;
  return cur_count;
}

//Получение списка радиостанций для web-интерфейса из ini-файла
void ListRadioStation(bool pos_flag, int pos, int r_type, int r_location) {
  lrs_flag = false;
  na_flag = false;
  int fl_pos = pos;
  int add_to = 0;
  retry = 0;
  slow = 0;
  String rs, rs_url;
  int cur_pos = 0;
  int r_t_t = 0;
  bool g_flag = false;
  bool fix_start_flag = false;
  int fix_start_pos;
  int start_ini = 0;
  int end_ini = 0;

  start_pos = pos - (radio_per_page / 2);
  if (start_pos < 0) {
    add_to = start_pos;
    start_pos = 1;
  } else {
    add_to = 1;
  }
  end_pos = pos + (radio_per_page / 2) - add_to;  // - 1;
  if (end_pos > radio_station_count) end_pos = radio_station_count;
  int smesh = 0;
  if (favorites_flag == false) {
    start_ini = FindIniFile(start_pos);
    end_ini = FindIniFile(end_pos);
    for (int i = 0; i < start_ini; i++) {
      smesh = smesh + rl_count[i];
    }
    if ((pos - (radio_per_page / 2)) > smesh) smesh = (pos - smesh) - (radio_per_page / 2) - 1;
  }

  if (favorites_flag == true) {
    GP.LABEL(F("Любимые станции"), "", GP_ORANGE, 19, 1, 1);
  } else {
    GP.LABEL(F("Полный список"), "", GP_ORANGE, 19, 1, 1);
  }
  GP.BREAK();
  GP.LABEL("Жанр:", "", GP_GRAY_B, 16, 0, 1);
  GP.LABEL(radio_type_title[r_type], "", GP_GREEN_B, 16, 1, 1);
  GP.BREAK();
  GP.LABEL("Локация:", "", GP_GRAY_B, 16, 0, 1);
  GP.LABEL(radio_location_title[r_location], "", GP_GREEN_B, 16, 1, 1);
  GP.BREAK();
  GP.SEND("<div style='color:#9c9ea1;font-size:14px;font-weight:normal;white-space:normal;'>c <span style='color:#b4b700;font-size:14px;font-weight:bold;white-space:normal;'>" + String(start_pos) + "</span> по <span style='color:#b4b700;font-size:14px;font-weight:bold;white-space:normal;'>" + String(end_pos) + "</span> из <span style='color:#b4b700;font-size:14px;font-weight:bold;white-space:normal;'>" + String(radio_station_count) + "</span></div>");
  if (start_ini + end_ini >= 0) {
    if (sd_flag == true && file_flag == true) {
      file_flag = false;
      if (favorites_flag == true) {
        file = SD.open("/Radio/favorites.ini", FILE_READ);
        if (file) {
          while (file.available()) {
            vTaskDelay(1);
            esp_task_wdt_reset();
            rs = file.readStringUntil('\n');
            rs.replace('\n', ' ');
            rs.trim();
            int f_pos = 0;
            int s_pos;
            int col = 0;
            bool f_flag = false;
            rs_title = "";
            rs_url = "";
            fix_radio_location = -1;
            while (f_pos != -1) {
              s_pos = f_pos;
              f_pos = rs.indexOf("|", f_pos + 1);
              String column = rs.substring(s_pos, f_pos);
              column.replace('|', ' ');
              column.trim();
              if (col == 0) r_t_t = column.toInt();
              if (col == 0 && (column.toInt() == r_type || r_type == 0)) f_flag = true;
              if (col == 1) fix_radio_location = column.toInt();
              if (col == 1 && (column.toInt() == r_location || r_location == 0) && f_flag == true) {
                cur_pos++;
                g_flag = true;
                if (fix_start_flag == false && cur_pos >= start_pos && cur_pos <= end_pos) {
                  fix_start_flag = true;
                  fix_start_pos = cur_pos;
                }
              }
              if (col == 2) rs_url = column;
              if (col == 3) rs_title = column;
              col++;
            }
            if (cur_pos >= start_pos && cur_pos <= end_pos && g_flag == true) {
              g_flag = false;
              if (fix_radio_location >= 0) {
                if (pos == cur_pos && pos_flag == true) {
                  if (rs_title == "") {
                    GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                  } else {
                    GP.BUTTON_MINI_LINK("/", rs_title, GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                  }
                } else {
                  if (rs_title == "") {
                    GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_GRAY, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                  } else {
                    GP.BUTTON_MINI_LINK("/", rs_title, GP_GREEN, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                  }
                }
                GP.HINT("radio_item/" + String(cur_pos), "[" + radio_type_title[r_t_t] + "]-[" + radio_location_title[fix_radio_location] + "]");
              }
            }
            if (cur_pos > end_pos) break;
            esp_task_wdt_reset();
          }
          if (pos_flag == true) GP.SEND("<script>\nvar trg = document.getElementById('radio_item/" + String(pos) + "');\ntrg.scrollIntoView();\</script>\n");
          file.close();
          file_flag = true;
          lrs_flag = true;
        } else {
          Serial.print(F("SD-card: /Radio/favorites.ini not found."));
          rs_url = "";
          file_flag = true;
          lrs_flag = true;
          return;
        }
        start_pos = fix_start_pos;
        end_pos = cur_pos;
        GP.HR();
        GP.TABLE_BEGIN();
        GP.TR();
        GP.SEND(F("<td width='50%'>"));
        if (fl_pos > (radio_per_page / 2)) {
          GP.BUTTON_MINI_LINK("/radio_list_prev", "◄", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_list_prev"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td>"));
        GP.SEND(F("<td width='50%'>"));
        if (end_pos < radio_station_count) {
          GP.BUTTON_MINI_LINK("/radio_list_next", "►", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_list_next"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td></tr>"));
        GP.TABLE_END();
        //-----------------------------------------------
      } else {
        cur_pos = start_pos - 1 - smesh;
        for (int j = start_ini; j <= end_ini; j++) {
          file = SD.open("/Radio/radio" + String(j) + ".ini", FILE_READ);
          if (file) {
            while (file.available()) {
              vTaskDelay(1);
              esp_task_wdt_reset();
              rs = file.readStringUntil('\n');
              rs.replace('\n', ' ');
              rs.trim();
              int f_pos = 0;
              int s_pos;
              int col = 0;
              bool f_flag = false;
              rs_title = "";
              rs_url = "";
              fix_radio_location = -1;
              while (f_pos != -1) {
                s_pos = f_pos;
                f_pos = rs.indexOf("|", f_pos + 1);
                String column = rs.substring(s_pos, f_pos);
                column.replace('|', ' ');
                column.trim();
                if (col == 0) r_t_t = column.toInt();
                if (col == 0 && (column.toInt() == r_type || r_type == 0)) f_flag = true;
                if (col == 1) fix_radio_location = column.toInt();
                if (col == 1 && (column.toInt() == r_location || r_location == 0) && f_flag == true) {
                  cur_pos++;
                  g_flag = true;
                  if (fix_start_flag == false && cur_pos >= start_pos && cur_pos <= end_pos) {
                    fix_start_flag = true;
                    fix_start_pos = cur_pos;
                  }
                }
                if (col == 2) rs_url = column;
                if (col == 3) rs_title = column;
                col++;
              }
              if (cur_pos >= start_pos && cur_pos <= end_pos && g_flag == true) {
                g_flag = false;
                if (fix_radio_location >= 0) {
                  if (pos == cur_pos && pos_flag == true) {
                    if (rs_title == "") {
                      GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                    } else {
                      GP.BUTTON_MINI_LINK("/", rs_title, GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                    }
                  } else {
                    if (rs_title == "") {
                      GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_GRAY, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                    } else {
                      GP.BUTTON_MINI_LINK("/", rs_title, GP_GREEN, F("98% !important; white-space: normal"), "radio_item/" + String(cur_pos));
                    }
                  }
                  GP.HINT("radio_item/" + String(cur_pos), "[" + radio_type_title[r_t_t] + "]-[" + radio_location_title[fix_radio_location] + "]");
                }
              }
              if (cur_pos > end_pos) break;
              esp_task_wdt_reset();
            }
            file.close();
          } else {
            Serial.print(F("SD-card: /Radio/radio*.ini not found."));
            rs_url = "";
            file_flag = true;
            lrs_flag = true;
            return;
          }
        }
        file_flag = true;
        lrs_flag = true;
        if (pos_flag == true) GP.SEND("<script>\nvar trg = document.getElementById('radio_item/" + String(pos) + "');\ntrg.scrollIntoView();\</script>\n");
        start_pos = fix_start_pos;
        end_pos = cur_pos;
        GP.HR();
        GP.TABLE_BEGIN();
        GP.TR();
        GP.SEND(F("<td width='50%'>"));
        if (fl_pos > (radio_per_page / 2)) {
          GP.BUTTON_MINI_LINK("/radio_list_prev", "◄", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_list_prev"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td>"));
        GP.SEND(F("<td width='50%'>"));
        if (end_pos < radio_station_count) {
          GP.BUTTON_MINI_LINK("/radio_list_next", "►", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_list_next"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td></tr>"));
        GP.TABLE_END();
      }
    }
  }
}

//определяет в каком файле радиостанция
int FindIniFile(int pos) {
  int t_r = 0;
  for (int i = 0; i < total_radio_ini; i++) {
    t_r = t_r + rl_count[i];
    if (t_r >= pos) return i;
  }
  return -1;
}

//=====================================================

// Поиск радиостанций для web-интерфейса из ini-файла
void SearchRadioStation(int pos, int r_type, int r_location, String find_text) {
  String rs_lc_title;
  lrs_flag = false;
  na_flag = false;
  retry = 0;
  slow = 0;
  String rs, rs_url;
  int cur_pos = 0;
  int r_t_t = 0;
  bool g_flag = false;
  int cur_id = 0;
  int start_ini = 0;
  int end_ini = 0;

  start_pos = pos;
  if (start_pos < 0) {
    start_pos = 0;
  }
  end_pos = radio_station_count;
  int smesh = 0;
  if (favorites_flag == false) {
    start_ini = FindIniFile(start_pos + 1);
    end_ini = FindIniFile(radio_station_count);
    for (int i = 0; i < start_ini; i++) {
      smesh = smesh + rl_count[i];
    }
    if ((pos - (radio_per_page / 2)) > smesh) smesh = (pos - smesh) - (radio_per_page / 2) - 1;
  }
  GP.LABEL("«" + find_text + "»", "", GP_ORANGE, 19, 1, 1);
  GP.BREAK();
  if (favorites_flag == true) {
    GP.LABEL(F("Любимые станции"), "", GP_DEFAULT, 16, 1, 1);
  } else {
    GP.LABEL(F("Полный список"), "", GP_DEFAULT, 16, 1, 1);
  }
  GP.BREAK();
  GP.LABEL("Жанр:", "", GP_GRAY_B, 16, 0, 1);
  GP.LABEL(radio_type_title[r_type], "", GP_GREEN_B, 16, 1, 1);
  GP.BREAK();
  GP.LABEL("Локация:", "", GP_GRAY_B, 16, 0, 1);
  GP.LABEL(radio_location_title[r_location], "", GP_GREEN_B, 16, 1, 1);
  GP.BREAK();

  find_text.toLowerCase();
  find_text = cyrToLowerCase(find_text);
  Serial.println(find_text);

  if (start_ini + end_ini >= 0) {
    if (sd_flag == true && file_flag == true) {
      file_flag = false;
      if (favorites_flag == true) {
        file = SD.open("/Radio/favorites.ini", FILE_READ);
        if (file) {
          while (file.available()) {
            vTaskDelay(1);
            esp_task_wdt_reset();
            rs = file.readStringUntil('\n');
            rs.replace('\n', ' ');
            rs.trim();
            int f_pos = 0;
            int s_pos;
            int col = 0;
            bool f_flag = false;
            rs_title = "";
            rs_url = "";
            fix_radio_location = -1;
            while (f_pos != -1) {
              s_pos = f_pos;
              f_pos = rs.indexOf("|", f_pos + 1);
              String column = rs.substring(s_pos, f_pos);
              column.replace('|', ' ');
              column.trim();
              if (col == 0) r_t_t = column.toInt();
              if (col == 0 && (column.toInt() == r_type || r_type == 0)) f_flag = true;
              if (col == 1) fix_radio_location = column.toInt();
              if (col == 2) rs_url = column;
              if (col == 3) rs_title = column;
              if (col == 3 && (fix_radio_location == r_location || r_location == 0) && f_flag == true) {
                if (find_text == "") {
                  g_flag = true;
                } else {
                  rs_lc_title = rs_title;
                  rs_lc_title.toLowerCase();
                  rs_lc_title = cyrToLowerCase(rs_lc_title);
                  if (rs_lc_title.indexOf(find_text) > -1) g_flag = true;
                }
                cur_id++;
              }
              col++;
            }
            if (cur_pos >= start_pos && cur_pos <= end_pos && g_flag == true) {
              cur_pos++;
              g_flag = false;
              if (fix_radio_location >= 0) {
                if (cur_id == cur_radio_station) {
                  if (rs_title == "") {
                    GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                  } else {
                    GP.BUTTON_MINI_LINK("/", rs_title, GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                  }
                } else {
                  if (rs_title == "") {
                    GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_GRAY, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                  } else {
                    GP.BUTTON_MINI_LINK("/", rs_title, GP_GREEN, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                  }
                }
                GP.HINT("radio_item/" + String(cur_id), "[" + radio_type_title[r_t_t] + "]-[" + radio_location_title[fix_radio_location] + "]");
              }
            }
            if (cur_pos > radio_per_page) break;
            esp_task_wdt_reset();
          }
          if (cur_id == cur_radio_station) GP.SEND("<script>\nvar trg = document.getElementById('radio_item/" + String(cur_id) + "');\ntrg.scrollIntoView();\</script>\n");
          start_pos = cur_id;
          file.close();
          file_flag = true;
          lrs_flag = true;
        } else {
          Serial.print(F("SD-card: /Radio/favorites.ini not found."));
          rs_url = "";
          file_flag = true;
          lrs_flag = true;
          return;
        }
        GP.HR();
        GP.TABLE_BEGIN();
        GP.TR();
        GP.SEND(F("<td width='50%'>"));
        if (pos > 1) {
          GP.BUTTON_MINI_LINK("/radio_find_prev", "◄◄", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_find_prev"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td>"));
        GP.SEND(F("<td width='50%'>"));
        if (cur_id < radio_station_count) {
          GP.BUTTON_MINI_LINK("/radio_find_next", "►", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_find_next"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td></tr>"));
        GP.TABLE_END();
      } else {
        cur_pos = start_pos - 1 - smesh;
        for (int j = start_ini; j <= end_ini; j++) {
          file = SD.open("/Radio/radio" + String(j) + ".ini", FILE_READ);
          if (file) {
            while (file.available()) {
              vTaskDelay(1);
              esp_task_wdt_reset();
              rs = file.readStringUntil('\n');
              rs.replace('\n', ' ');
              rs.trim();
              int f_pos = 0;
              int s_pos;
              int col = 0;
              bool f_flag = false;
              rs_title = "";
              rs_url = "";
              fix_radio_location = -1;
              while (f_pos != -1) {
                s_pos = f_pos;
                f_pos = rs.indexOf("|", f_pos + 1);
                String column = rs.substring(s_pos, f_pos);
                column.replace('|', ' ');
                column.trim();
                if (col == 0) r_t_t = column.toInt();
                if (col == 0 && (column.toInt() == r_type || r_type == 0)) f_flag = true;
                if (col == 1) fix_radio_location = column.toInt();
                if (col == 2) rs_url = column;
                if (col == 3) rs_title = column;
                if (col == 3 && (fix_radio_location == r_location || r_location == 0) && f_flag == true) {
                  if (find_text == "") {
                    g_flag = true;
                  } else {
                    rs_lc_title = rs_title;
                    rs_lc_title.toLowerCase();
                    rs_lc_title = cyrToLowerCase(rs_lc_title);
                    if (rs_lc_title.indexOf(find_text) > -1) g_flag = true;
                  }
                  cur_id++;
                }
                col++;
              }
              if (g_flag == true) {
                cur_pos++;
                g_flag = false;
                if (fix_radio_location >= 0) {
                  if (cur_id == cur_radio_station) {
                    if (rs_title == "") {
                      GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                    } else {
                      GP.BUTTON_MINI_LINK("/", rs_title, GP_RED, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                    }
                  } else {
                    if (rs_title == "") {
                      GP.BUTTON_MINI_LINK("/", F("- без названия -"), GP_GRAY, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                    } else {
                      GP.BUTTON_MINI_LINK("/", rs_title, GP_GREEN, F("98% !important; white-space: normal"), "radio_item/" + String(cur_id));
                    }
                  }
                  GP.HINT("radio_item/" + String(cur_id), "[" + radio_type_title[r_t_t] + "]-[" + radio_location_title[fix_radio_location] + "]");
                }
              }
              if (cur_pos > radio_per_page) break;
              esp_task_wdt_reset();
            }
            //if (cur_id == cur_radio_station) GP.SEND("<script>\nvar trg = document.getElementById('radio_item/" + String(cur_id) + "');\ntrg.scrollIntoView();\</script>\n");
            file.close();
          } else {
            Serial.print(F("SD-card: /Radio/radio*.ini not found."));
            rs_url = "";
            file_flag = true;
            lrs_flag = true;
            return;
          }
        }
        file_flag = true;
        lrs_flag = true;
        if (cur_id == cur_radio_station) GP.SEND("<script>\nvar trg = document.getElementById('radio_item/" + String(cur_id) + "');\ntrg.scrollIntoView();\</script>\n");
        start_pos = cur_id + 1;
        Serial.println(start_pos);
        GP.HR();
        GP.TABLE_BEGIN();
        GP.TR();
        GP.SEND(F("<td width='50%'>"));
        if (pos > 1) {
          GP.BUTTON_MINI_LINK("/radio_find_prev", "◄◄", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_find_prev"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td>"));
        GP.SEND(F("<td width='50%'>"));
        if (cur_id < radio_station_count) {
          Serial.print("cur_id = ");
          Serial.println(cur_id);
          Serial.print("radio_station_count = ");
          Serial.println(radio_station_count);
          GP.BUTTON_MINI_LINK("/radio_find_next", "►", GP_YELLOW, F("98% !important; margin:0px 0px !important;"), F("radio_find_next"));
        } else {
          GP.SEND(F("&nbsp;"));
        }
        GP.SEND(F("</td></tr>"));
        GP.TABLE_END();
      }
    }
  }
}

void PreviewStation() {
  retry = 0;
  slow = 0;
  na_flag = false;
  preview_flag = true;
  if (lrs_flag == true) PreloadRadioStation(pre_radio_station, cur_radio_type, cur_radio_location);
}

void PreloadRadioStation(int pos, int r_type, int r_location) {
  String rs, pre_rs_url, pre_rs_title;
  int16_t tx, ty;
  uint16_t tw, th;
  int16_t fix_y;
  lrs_flag = false;
  int fix_pos = pos;
  if (sd_flag == true && file_flag == true) {
    file_flag = false;
    if (favorites_flag == true) {
      file = SD.open("/Radio/favorites.ini", FILE_READ);
    } else {
      int t_r = 0;
      for (int i = 0; i < total_radio_ini; i++) {
        t_r = t_r + rl_count[i];
        if (t_r >= pos) {
          pos = pos - t_r + rl_count[i];
          file = SD.open("/Radio/radio" + String(i) + ".ini", FILE_READ);
          break;
        }
      }
    }
    int cur_pos = 0;
    int r_t_t = 0;
    int r_t_l = 0;
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
          bool f_flag = false;
          pre_rs_title = "";
          while (f_pos != -1) {
            s_pos = f_pos;
            f_pos = rs.indexOf("|", f_pos + 1);
            String column = rs.substring(s_pos, f_pos);
            column.replace('|', ' ');
            column.trim();
            if (col == 0) r_t_t = column.toInt();
            if (col == 0 && (column.toInt() == r_type || r_type == 0)) f_flag = true;
            if (col == 1) r_t_l = column.toInt();
            if (col == 1 && (column.toInt() == r_location || r_location == 0) && f_flag == true) cur_pos++;
            if (col == 2) pre_rs_url = column;
            if (col == 3) pre_rs_title = column;
            col++;
          }
        } else {
          pre_rs_url = "";
          cur_pos = pos;
        }
        esp_task_wdt_reset();
      }
      file.close();

      tft.drawLine(0, 19, 160, 19, radio_color[5]);
      tft.fillRect(0, 20, 160, 12, radio_color[1]);
      tft.drawLine(0, 32, 160, 32, radio_color[5]);
      tft.fillRect(0, 33, 160, 77, radio_color[0]);
      tft.setTextWrap(false);
      tft.setFont();

      // Позиция в списке
      tft.setTextSize(1);
      tft.setCursor(2, 22);
      tft.setTextColor(radio_color[2]);
      if (sys_font == false) {
        sys_font = true;
        tft.print(utf8rus(F("Станция ")));
        sys_font = false;
      } else {
        tft.print(utf8rus(F("Станция ")));
      }
      tft.setTextColor(radio_color[3]);
      tft.print(String(fix_pos));
      tft.setTextColor(radio_color[2]);
      if (sys_font == false) {
        sys_font = true;
        tft.print(utf8rus(F(" из ")));
        sys_font = false;
      } else {
        tft.print(utf8rus(F(" из ")));
      }
      tft.setTextColor(radio_color[3]);
      tft.print(String(radio_station_count));

      // Название радиостанции
      if (pre_rs_title == "") pre_rs_title = "* * *";
      tft.setTextWrap(false);
      tft.setTextColor(radio_color[5]);
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
          if (th < fix_h) {
            fix_y = ((fix_y - th) / 2) + 17;
          } else {
            fix_y = 34;
          }
          tft.setCursor((w - tw) / 2, fix_y);
        } else {
          tft.setTextSize(2);
          tft.setCursor((w - (len_t * 12)) / 2, 38);
          if (len_t < 9) {
            tft.setTextSize(3);
            tft.setCursor((w - (len_t * 18)) / 2, 38);
          }
          if (len_t > 13) {
            tft.setTextSize(1);
            tft.setCursor((w - (len_t * 6)) / 2, 38);
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

      tft.fillRect(0, 110, 160, 18, radio_color[1]);
      tft.setTextWrap(false);
      tft.setFont();
      tft.setTextSize(1);

      // Тип радиостанции
      tft.setCursor(2, 111);
      if (sys_font == false) {
        sys_font = true;
        tft.setTextColor(radio_color[2]);
        tft.print(utf8rus(F("Тип: ")));
        tft.setTextColor(radio_color[3]);
        tft.print(utf8rus(radio_type_title[r_t_t]));
        sys_font = false;
      } else {
        tft.setTextColor(radio_color[2]);
        tft.print(utf8rus(F("Тип станции: ")));
        tft.setTextColor(radio_color[3]);
        tft.print(utf8rus(radio_type_title[r_t_t]));
      }

      //Страна/континент
      tft.setCursor(2, 120);
      if (sys_font == false) {
        sys_font = true;
        tft.setTextColor(radio_color[2]);
        tft.print(utf8rus(F("Локация: ")));
        tft.setTextColor(radio_color[3]);
        tft.print(utf8rus(radio_location_title[r_t_l]));
        sys_font = false;
      } else {
        tft.setTextColor(radio_color[2]);
        tft.print(utf8rus(F("Локация: ")));
        tft.setTextColor(radio_color[3]);
        tft.print(utf8rus(radio_location_title[r_t_l]));
      }

      tft.setFont(&mMediaSymbols);
      tft.setTextColor(radio_color[6]);
      tft.setCursor(141, 127);
      printMenuText("?");

      file_flag = true;
      lrs_flag = true;
    } else {
      if (favorites_flag == true) {
        Serial.print(F("SD-card: /Radio/favorites.ini not found."));
      } else {
        Serial.print(F("SD-card: /Radio/radio(i).ini not found."));
      }
      pre_rs_url = "";
      file_flag = true;
      lrs_flag = true;
    }
  }
  lrs_flag = true;
}


String cyrToLowerCase(String str) {
  char* uppercaseCyrillics[] = { "А", "Б", "В", "Г", "Д", "Е", "Ж", "З",
                                 "И", "Й", "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У",
                                 "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я" };
  char* lowercaseCyrillics[] = { "а", "б", "в", "г", "д", "е", "ж", "з",
                                 "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у",
                                 "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я" };
  for (int i = 0; i < 32; i++) {
    str.replace(uppercaseCyrillics[i], lowercaseCyrillics[i]);
  }
  return str;
}
