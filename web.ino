/*
WI-FI соединение
WEB-интерфейс
*/

// Логотип html-страницы (используется картинка в формате base64)
// Один из он-лайн сервисов, который позволяет преоборазовывать графические файлы в base64 - https://snipp.ru/tools/base64-img?
const char logotype[] = "<div style=\"padding:0px 0px;margin-top:0px;margin-bottom:0px;width:90%;max-width:400px;\">\n<img width=\"60\" height=\"60\" align=\"left\" hspace=4 src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADwAAAA8CAMAAAANIilAAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAyJpVFh0WE1MOmNvbS5hZG9iZS54bXAAAAAAADw/eHBhY2tldCBiZWdpbj0i77u/IiBpZD0iVzVNME1wQ2VoaUh6cmVTek5UY3prYzlkIj8+IDx4OnhtcG1ldGEgeG1sbnM6eD0iYWRvYmU6bnM6bWV0YS8iIHg6eG1wdGs9IkFkb2JlIFhNUCBDb3JlIDUuMy1jMDExIDY2LjE0NTY2MSwgMjAxMi8wMi8wNi0xNDo1NjoyNyAgICAgICAgIj4gPHJkZjpSREYgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIj4gPHJkZjpEZXNjcmlwdGlvbiByZGY6YWJvdXQ9IiIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RSZWY9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZVJlZiMiIHhtcDpDcmVhdG9yVG9vbD0iQWRvYmUgUGhvdG9zaG9wIENTNiAoV2luZG93cykiIHhtcE1NOkluc3RhbmNlSUQ9InhtcC5paWQ6MjFBMjdCQkNDOTBBMTFFRUFBMEVGRkY5RTI2Q0E5NjQiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6MjFBMjdCQkRDOTBBMTFFRUFBMEVGRkY5RTI2Q0E5NjQiPiA8eG1wTU06RGVyaXZlZEZyb20gc3RSZWY6aW5zdGFuY2VJRD0ieG1wLmlpZDoyMUEyN0JCQUM5MEExMUVFQUEwRUZGRjlFMjZDQTk2NCIgc3RSZWY6ZG9jdW1lbnRJRD0ieG1wLmRpZDoyMUEyN0JCQkM5MEExMUVFQUEwRUZGRjlFMjZDQTk2NCIvPiA8L3JkZjpEZXNjcmlwdGlvbj4gPC9yZGY6UkRGPiA8L3g6eG1wbWV0YT4gPD94cGFja2V0IGVuZD0iciI/Ps+Bta0AAABgUExURUo2KGpfVvzz5riHUgICAl1HNZxpLzIZC/DbvIttUaaJbzsuJseZYsGRWy8lHte9pRELBqx6Q9GhaauejiMbFZt7XRwTDI2BduS3eQkHBhcWFnpcQwwSHr6fgRMPDP///yDpiUwAAAAgdFJOU/////////////////////////////////////////8AXFwb7QAABYVJREFUeNrslu2WnCgQhkFABRTRRmykJfd/l/sWOD2TSfJjz+7PVNvqUR+qgPpiP/6DsL/wX/jfwJxEKWvZn0Qpzn8Hc2chjEDrnAuhlFh8gkgILgUPAsRZ/g3mTh0QTZoV8CauCSEnSaHRpCz8J5jb5VBNYYi34C40GYahqiSxSjkp+VfYLRxjerJrwAc0OaUyROOA0C09UsyGwFi66QpzpZO0+74bkp3kAwGU9483xlxGpcSYtJ+wO5xkRg2DG8jE4Q8SI/4DS0m5yN+w1b6Y7G3dDDK3X0nELGacxTzNggSPdD94692tmmDOVLKX6uv7vqejsqJe+nWGiBlD4P/sU2aFpTdslU15ySs+bsdaFYHAf14bNbfTlP2uiy2Of8A8+GtR/STmmyHoFjHNG2SapnmatmnrvXkMNnj2htOwNLiCnyypmhs64TLP27xGs8AZ5Q1zqxIbF7benz6fz41uJ9GMnbep/Uj7Jvy+sE8Yi5wU4J4+3cQqmH7WJb6Pm4Nq3MyAR/hKoklXOKQMGLMT01MN8F62Nq1THe+J6W7zbbyIDaZJV9gX3cyethXwS6a+30BXtdilrbIbzWmeq2Z2ygpzCxv2DjDePMXA3Ct5tZIiUkimb3Wd28TXuHeZMfcBu2RNhTH0k3mW2aDaNOsGfRhMy4XR/A0jrNkPhG1STTN9MmuV+57MJbLOuq7Y1nTTnDvNlPWREezCDdeN2Z6bwG7d1pJfiumL0FZ1O3wyyAqHWPLHnMUbqpqx10I0G252W73p9gGZA7HBOODzNnuurrW1HaIzrBT3OHNdvnmDh1XYSscJ9qXCPfxyJW4lZ6huXZF6ahfaS38BZs6mSKkSeVLRave3W1RLa0i9Z0vm1FfTMwM2zgZbJMI/QDMDrBC4on5SQ1m00Jy/CPnslocFMHOuwTG5bLou50lQHsDwVWn7tsZzW4Mabivru9G4bE+PXMZiTEzvI6nGrGsuoGTQEoIQX3IKpZl+HSvsSptzSWw34zguyJv1NZ1+IzSKufDdeFQ40Gp77whexq4bFxK6/yrdV6EHhmXn4Z+Akw/XNX57vzR5VEHCfjxo1Pv9g2mC4WHIw3FZ8OgyHC6LaI4uoqp5kvIuPpHqXWGcH2NnlA6+uidLHpmlm7xMkSoiQuyigpFRtRzckF/mghhe66X30o+LUhi7+XZJdhkxUq1G+LxIH04fzUTlxSwH6usJ7tSKw6OVVAuLWC8ymztYdy14J1PmqLJBSlTLk2mUMMX4YRNzoN2B+suVTN4uQwpw7ZoMUvFm9KeS3L3ci2kvg2IBZaRgIFycYTIkrl4uSdz6fBFsaw7DpNM+wlWck2pTiXNpj3TCIsWTPU5gCcWZSfYc7Ymp7xfioaWhCusRScmdgU0js2QCNMrErFQIPZtwKfGl++4plfPG2IBC27InVkyNKHzudCrrV4AOsjhZVSwg5hCzLpBT9CgP5TTanimwu2LAx7uMVToTyxqvvcjZaC00tqxW+n0XKOo2K2hIYdeIR9fmTG7iRi0t9Ssn8qFUgLJ6H9Ra5Fxker0w8xL2PBRKnh/wMBrJCrVMEoNOqO/ghdjF3iSjAlmZCvwsBJ1jlO8qGUtcLuxPTDGcsmQFp4NLloEO9BIDHBXZFh1RCS74XfnzDXNXAi23r/1atF4ej4Pc5cD5wfnjOB4PmMQijZmyKecZ7bsnwTqY0eSM9tE8L627X2W7lutRG6KLJdQY/m6lpHdFI+Yuer8cO4VldwfzfTcbrD+W3RzKsfjZ0JBqtEJwXGTD6ODBu94/RdemjHNWqPG0jKvq2J/tY0A8tq7UOqyUv6W873xtD1FcEOFJBv5T44q6kai3RRNZobek+3H6fBos/94y/7nL/qXn/l86/X8EGAChHfR2MO87rgAAAABJRU5ErkJggg==\">\n";

// Нижняя плашка html-страницы
const char copyright[] = "<div style=\"font-size: 11px;\">2023-2025, <a href=\"https://vk.com/olegpolyakov1812/\" target=\"_blank\">Поляков О.В.</a></div>";
//const char station_type_list[] = "все станции,классика,этно,джаз,рок,поп,психоделика,танцы,релакс,ретро,детское,аудиокниги,новости,";
const char language_list[] = "русский,английский,немецкий,французский";
String lang_list[4] = { "ru", "en", "de", "fr" };
const char palettes[] = { "тёмная,светлая,изумруд,чёрно-белая,ретро,рубин и...,калейдоскоп" };

String webDate;
GPtime wTime;
String webTime;
GPdate wDate;

bool wifi_flag = false;
bool connect_flag = true;
bool warrning_flag;

String cur_message;
String wfa, wfp;
String au_text = F("Здравствуйте, друзья!");

int add_station, try_connect;

String mp3_file;

void web_connect() {
  int16_t tx, ty;
  uint16_t tw, th;
  bool new_flag = false;
  tft.fillScreen(sys_color[0]);
  WiFi.disconnect();
  delay(1000);

  //Если зажата кнопка, то новый коннект. В противном случае - штатное подключение к сети
  if (digitalRead(PIN) == 1 || saved_connect_flag == true) {  // кнопка не нажата
    tft.setFont(&mMediaSymbols);
    tft.setTextSize(1);
    tft.setCursor(w / 2 - 14, h / 2);
    tft.setTextColor(sys_color[2]);
    tft.print("9");

    saved_connect_flag = false;
    WiFi.mode(WIFI_STA);
    delay(1000);
    WiFi.begin(lp.ssid, lp.pass);
    Serial.println(F("Подключение к wi-fi..."));

    tft.setFont();
    tft.setTextWrap(true);
    tft.setTextColor(sys_color[2]);
    tft.setTextSize(1);
    tft.setCursor(1, 40);
    tft.getTextBounds(utf8rus(F("Подключаюсь к")), 0, 10, &tx, &ty, &tw, &th);
    tft.setCursor((w - tw) / 2, 10);
    tft.println(utf8rus(F("Подключаюсь к")));

    tft.getTextBounds(lp.ssid, 0, ty + th + 5, &tx, &ty, &tw, &th);
    tft.setCursor((w - tw) / 2, ty);
    tft.setTextColor(sys_color[3]);
    tft.println(lp.ssid);

    int x = 0;
    tft.setTextSize(3);
    String Zx = "";
    while ((WiFi.status() != WL_CONNECTED) && x < 8) {  // (выполнять цикл, пока хотя бы одно из условий истинно)
      if (WiFi.status() == WL_CONNECTED) break;
      x++;
      delay(1500);
      esp_task_wdt_reset();
      Zx = Zx + ".";
      tft.getTextBounds(Zx, 0, ty, &tx, &ty, &tw, &th);
      tft.setCursor((w - tw) / 2, ty);
      tft.setTextColor(sys_color[4]);
      tft.print(Zx);
    }
    tft.fillRect((w - 32) / 2, (h - 32) / 2, 32, 32, sys_color[0]);
    tft.setTextSize(1);
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(F("Связь по wi-fi установлена."));
      Serial.print(F("IP-адрес: "));
      Serial.println(WiFi.localIP());
      tft.setTextColor(sys_color[2]);
      tft.getTextBounds(utf8rus(F("Есть контакт!")), 0, ty + th + 5, &tx, &ty, &tw, &th);
      tft.setCursor((w - tw) / 2, ty);
      tft.println(utf8rus(F("Есть контакт!")));
      tft.getTextBounds(utf8rus(F("IP-адрес:")), 0, ty + th + 5, &tx, &ty, &tw, &th);
      tft.setCursor((w - tw) / 2, ty);
      tft.print(utf8rus(F("IP-адрес:")));
      tft.setTextColor(sys_color[3]);
      tft.setTextSize(1);
      tft.getTextBounds(String(WiFi.localIP()) + "...", 0, ty + th + 5, &tx, &ty, &tw, &th);
      tft.setCursor((w - tw) / 2, ty);
      tft.print(WiFi.localIP());
      delay(2000);
      internet_flag = true;
      ready_flag = true;
      // Инициализация NTP-клиента для получения времени
      timeClient.begin();
      timeClient.setTimeOffset(time_zone);
      delay(300);
      // получение точного времени
      GetTimeDate();
    } else {
      internet_flag = false;
      ready_flag = false;
      WiFi.softAP(F("microMEDIA"), wifi_pass, 1, false);
      tft.getTextBounds(utf8rus(F("Связь не установлена")), 0, ty, &tx, &ty, &tw, &th);
      tft.setCursor((w - tw) / 2, ty + 3 * th + 2);
      tft.setTextColor(sys_color[1]);
      tft.println(utf8rus(F("Связь не установлена")));
      tft.getTextBounds(utf8rus(F("Связь не установлена")), 0, ty, &tx, &ty, &tw, &th);
      tft.setCursor((w - tw) / 2, ty + th + 5);
      delay(3000);
    }
  } else {
    internet_flag = false;
    WiFi.softAP(F("microMEDIA"), wifi_pass, 1, false);
    tft.fillRect(0, 0, 160, 16, sys_color[7]);
    tft.getTextBounds(utf8rus(F("Загрузочное меню")), 0, 4, &tx, &ty, &tw, &th);
    tft.setCursor((w - tw) / 2, 4);
    tft.setTextColor(sys_color[4]);
    tft.println(utf8rus(F("Загрузочное меню")));
    tft.setTextColor(sys_color[5]);
    tft.setCursor(1, 24);
    tft.println("*");
    for (int i = 0; i <= load_menu_count; ++i) {
      if (i > 0) tft.setTextColor(sys_color[2]);
      tft.setCursor(12, 24 + (14 * i));
      tft.print(utf8rus(load_menu_title[i]));
    }
    for (;;) {
      CheckLoadEncoder();
      if (push_flag == true) {
        push_flag = false;
        tft.fillScreen(sys_color[0]);
        tft.setTextColor(sys_color[4]);
        switch (cur_load_menu) {
          case 0:  //Новая точка доступа
            tft.getTextBounds(utf8rus(F("Немного терпения...")), 0, 60, &tx, &ty, &tw, &th);
            tft.setCursor((w - tw) / 2, 60);
            tft.println(utf8rus(F("Немного терпения...")));
            new_flag = true;
            break;
          case 1:  //Повторить попытку соединения
            web_connect();
            return;
            break;
          case 2:  //Выбрать сохранённую точку доступа
            tft.getTextBounds(utf8rus(F("Немного терпения...")), 0, 60, &tx, &ty, &tw, &th);
            tft.setCursor((w - tw) / 2, 60);
            tft.println(utf8rus(F("Немного терпения...")));
            saved_connect_flag = true;
            break;
          case 3:  //Без соединения с Интернетом
            tft.setCursor(1, 14);
            tft.println(utf8rus(F("Внимание!\nДоступен только музыкальный проигрыватель.\n")));
            tft.setTextColor(sys_color[2]);
            tft.print(utf8rus(F("1. Включите wi-fi на вашем\nсмартфоне и установите\nсоединение с ")));
            tft.setTextColor(sys_color[3]);
            tft.print(utf8rus(F("microMEDIA ")));
            tft.setTextColor(sys_color[2]);
            tft.print(utf8rus(F("\n(пароль: ")));
            tft.setTextColor(sys_color[3]);
            tft.print(wifi_pass);
            tft.setTextColor(sys_color[2]);
            tft.print(utf8rus(F(").\n\n2. Откройте web-браузер\nи введите url-адрес:\n")));
            tft.setTextColor(sys_color[3]);
            tft.print(local_IP);
            tft.setTextColor(sys_color[2]);
            tft.println(F(".\n"));
            tft.setTextColor(sys_color[4]);
            tft.println(utf8rus(F("Нажмите кнопку энкодера\nдля продолжения...")));
            //-------------------
            for (;;) {
              enc1.tick();
              butt1.tick();
              if (butt1.hasClicks() || enc1.isSingle()) break;
            }
            internet_flag = false;
            skip_flag = true;
            ready_flag = true;
            WiFi.mode(WIFI_AP);
            return;
            break;
        }
      }
      esp_task_wdt_reset();
      if (new_flag == true || skip_flag == true || saved_connect_flag == true) break;
    }
  }

  //список сохранённых и доступных точек доступа
  if (saved_connect_flag == true) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    String rs;
    if (sd_flag == true) {
      file = SD.open("/WiFi/wifi.ini", FILE_READ);
      if (file) {
        while (file.available()) {
          rs = file.readStringUntil('\n');
          rs.replace("\n", " ");
          rs.trim();
          int f_pos = rs.indexOf("|", 0);
          if (f_pos != -1) {
            int Networks_count = WiFi.scanNetworks();
            if (Networks_count > 20) Networks_count = 20;
            int k = 0;
            for (int i = 0; i < Networks_count; ++i) {
              if (rs.substring(0, f_pos) == WiFi.SSID(i)) {
                wifi_ssid[k] = WiFi.SSID(i);
                //Serial.print(F(">>>>>>"));
                //Serial.println(wifi_ssid[k]);
                k++;
              }
            }
          }
          esp_task_wdt_reset();
        }
        file.close();
      } else {
        Serial.println(F("SD-карта: файл /WiFi/wifi.ini - не найден."));
      }
    }
  } else {

    if (skip_flag == false) {
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println(F("Не могу присоединиться к wi-fi."));
        // Connect to Wi-Fi network with SSID and password
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);
        Serial.println(F("Сканирую wi-fi..."));
        int Networks_count = WiFi.scanNetworks();
        Serial.println(F("Сканирование wi-fi завершено."));
        if (Networks_count == 0) {
          Serial.println(F("Сети не найдены."));
        } else {
          Serial.print(Networks_count);
          Serial.println(F("Сети найдены."));
          if (Networks_count > 20) Networks_count = 20;
          for (int i = 0; i < Networks_count; ++i) {
            wifi_ssid[i] = WiFi.SSID(i);
            delay(10);
            //Пытаемся найти наш роутер в файле сохранённых точек доступа
            if (new_flag == false) {
              if (SearchWiFi(wifi_ssid[i]) == true) {
                wfa.toCharArray(lp.ssid, 20);
                wfp.toCharArray(lp.pass, 20);
                wfp = String(XORENC(lp.pass, password_key));
                wfp.toCharArray(lp.pass, 20);
                sys_font = fix_sys_font;
                updateEEPROM();
                sys_font = true;
                try_connect++;
                if (try_connect < 2) {
                  web_connect();
                }
              }
            }
          }
        }
      }
    }
  }

  //Если не удалось соединиться с роутером, то создаём точку доступа
  //и информируем об этом через экран.
  if (WiFi.status() != WL_CONNECTED) {
    internet_flag = false;
    WiFi.mode(WIFI_AP);
    tft.fillScreen(sys_color[0]);
    tft.setTextWrap(true);
    tft.setTextColor(sys_color[2]);
    tft.setTextSize(1);
    tft.setCursor(0, 20);
    tft.print(utf8rus(F("1. Включите wi-fi на вашем\nсмартфоне и установите\nсоединение с ")));
    tft.setTextColor(sys_color[3]);
    tft.print(utf8rus(F("microMEDIA ")));
    tft.setTextColor(sys_color[2]);
    tft.print(utf8rus(F("\n(пароль: ")));
    tft.setTextColor(sys_color[3]);
    tft.print(wifi_pass);
    tft.setTextColor(sys_color[2]);
    tft.print(utf8rus(F(").\n\n2. Откройте web-браузер\nи введите url-адрес:\n")));
    tft.setTextColor(sys_color[3]);
    tft.print(local_IP);
    tft.setTextColor(sys_color[2]);
    tft.println(".");
    tft.setTextColor(sys_color[4]);
    tft.println(utf8rus(F("\nНажмите кнопку энкодера\nдля продолжения...")));
    //-------------------
    for (;;) {
      enc1.tick();
      if (enc1.isSingle()) break;
    }
  }
  ready_flag = true;
}

void build() {
  LedOn();
  GP.BUILD_BEGIN();
  GP.PAGE_TITLE(F("micro-MEDIA"));
#ifndef BATTERY_OFF
  GP.UPDATE(F("vk,f_reload,radio_mode,VUlevel,slow,cur_volume,cur_radio_station,radio_station_count,station,title,sample_rate,bits_per_sample,bit_rate,aac_header_format,aac_codec,channels,themas,cur_radio_type,cur_music_type,cur_music_file,music_count,mp3_info,index_info,curl,m_type_title,cur_type_title,cur_location_title,cur_im,max_im,cur_volt,cur_led"), 300);
#else
  GP.UPDATE(F("vk,f_reload,radio_mode,VUlevel,slow,cur_volume,cur_radio_station,radio_station_count,station,title,sample_rate,bits_per_sample,bit_rate,aac_header_format,aac_codec,channels,themas,cur_radio_type,cur_music_type,cur_music_file,music_count,mp3_info,index_info,curl,m_type_title,cur_type_title,cur_location_title,cur_im,max_im,cur_led"), 300);
#endif
  GP.RELOAD_CLICK(F("themas"));
  if (themas == true) {
    GP.THEME(GP_DARK);
  } else {
    GP.THEME(GP_LIGHT);
  }
  GP.PAGE_BEGIN();
  GP.SEND(logotype);
  GP.TITLE(F("micro-MEDIA"));
  GP.LABEL(F("Интернет-радио"), "", GP_YELLOW, 16);
#ifndef BATTERY_OFF
  GP.LABEL(cur_volt, F("cur_volt"), GP_ORANGE, 12, 0, 0);
#endif

  if (internet_flag == true || skip_flag == true) {
    if (skip_flag == true) {
      GP.NAV_TABS_LINKS(F("/,/connect"), F("Плеер,Подключение к wi-fi"));
      //Плеер
      if (ui.uri() == "/") GP_PLAYER();
      if (ui.uri() == "/connect") GP_CONNECT();
    } else {
      GP.NAV_TABS_LINKS(F("/,/clock,/player,/au"), F("Радио,Часы,Плеер,Ау!"));
      //Радио
      if (ui.uri() == "/") GP_RADIO();
      //Часы
      if (ui.uri() == "/clock") GP_CLOCK();
      //Плеер
      if (ui.uri() == "/player") GP_PLAYER();
      //Ау!
      if (ui.uri() == "/au") GP_AU();
    }

    //Выбор типа радиостанции
    if (ui.uri() == "/radio_type") {
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Фильтр радиостанций"), GP_YELLOW);
      GP.LABEL(F("Жанр"), "", GP_GREEN_B, 17, 1, 1);
      for (int i = 0; i <= radio_type_count; ++i) {
        if (i == cur_radio_type) {
          GP.BUTTON_MINI_LINK("/", radio_type_title[i], GP_RED, F("98% !important;"), "radio_type/" + String(i));
        } else {
          GP.BUTTON_MINI_LINK("/", radio_type_title[i], GP_GREEN, F("98% !important;"), "radio_type/" + String(i));
        }
      }
      GP.BREAK();
      GP.LABEL(F("Локация"), "", GP_BLUE_B, 17, 1, 1);
      for (int i = 0; i <= radio_location_count; ++i) {
        if (i == cur_radio_location) {
          GP.BUTTON_MINI_LINK("/", radio_location_title[i], GP_YELLOW, F("98% !important;"), "radio_location/" + String(i));
        } else {
          GP.BUTTON_MINI_LINK("/", radio_location_title[i], GP_BLUE, F("98% !important;"), "radio_location/" + String(i));
        }
      }
      GP.BREAK();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Выбор типа музыки
    if (ui.uri() == "/music_type") {
      //pause_control();
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Музыкальный жанр"), GP_YELLOW);
      if (favorites_flag1 == true) {
        if (all_fav_flag == true) {
          GP.BUTTON_MINI_LINK(F("/player"), F("Все жанры"), GP_RED_B, F("98% !important;"), F("music_notype"));
        } else {
          GP.BUTTON_MINI_LINK(F("/player"), F("Все жанры"), GP_BLUE_B, F("98% !important;"), F("music_notype"));
        }
      }
      for (int i = 0; i < dir_count; ++i) {
        if (i == cur_music_type && all_fav_flag != true) {
          GP.BUTTON_MINI_LINK(F("/player"), music_type_title[i], GP_RED, F("98% !important;"), "music_type/" + String(i));
        } else {
          GP.BUTTON_MINI_LINK(F("/player"), music_type_title[i], GP_BLUE, F("98% !important;"), "music_type/" + String(i));
        }
      }
      GP.BREAK();
      GP.BUTTON_MINI_LINK(F("/player"), F("Вернуться в Плеер"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Добавление радиостанции к списку избранных станций
    if (ui.uri() == "/add_favorites") {
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Любимые радиостанции"), GP_YELLOW);
      M_BOX(GP.LABEL("Добавить текущую радиостанцию <b>«" + station + "»</b> в список любимых станций?", "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI("add_fav_btn", F("Да"), "", GP_GREEN, F("60px important!"), false, true););
      favorites_flag = true;
      int fav_station_count = CountRadioStation(0, 0);
      favorites_flag = false;
      if (fav_station_count > 0) {
        M_BOX(GP.LABEL(F("Загрузить список любимых радиостанций?"), "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI_LINK("/", F("Да"), GP_ORANGE, F("60px important!"), F("load_fav_btn")););
      } else {
        GP.BREAK();
        M_BOX(GP.LABEL(F("Список любимых радиостанций пока пуст."), "", GP_RED, 14, 0, 1););
      }
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Удаление радиостанции из списка избранных станций
    if (ui.uri() == "/del_favorites") {
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Любимые радиостанции"), GP_YELLOW);
      int fav_station_count = CountRadioStation(0, 0);
      if (fav_station_count > 0) {
        M_BOX(GP.LABEL("Удалить текущую радиостанцию <b>«" + station + "»</b> из списка любимых станций?", "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI("del_fav_btn", F("Да"), "", GP_RED, F("60px important!"), false, true););
      } else {
        GP.BREAK();
        M_BOX(GP.LABEL(F("Список любимых радиостанций пока пуст."), "", GP_RED, 14, 0, 1););
      }
      M_BOX(GP.LABEL(F("Загрузить список всех радиостанций?"), "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI_LINK("/", F("Да"), GP_ORANGE, F("60px important!"), F("load_all_btn")););
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    if (ui.uri() == "/radio_list") {
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Список станций"), GP_YELLOW);
      ListRadioStation(true, cur_radio_station, cur_radio_type, cur_radio_location);
      GP.HR();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }
    if (ui.uri() == "/find_radio") {
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Поиск станций"), GP_YELLOW);
      M_BOX(GP.TEXT(F("find_radio_text"), F("что ищем?"), find_radio_text); GP.BUTTON_MINI_LINK("/found_list", "🔎", GP_GREEN, F("40px !important;"), ""););
      GP.HR();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }
    if (ui.uri() == "/found_list") {
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Результаты поиска"), GP_YELLOW);
      Serial.println(find_radio_text);
      SearchRadioStation(0, cur_radio_type, cur_radio_location, find_radio_text);
      GP.HR();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }
    if (ui.uri() == "/radio_list_next") {
      int t_r = end_pos + (radio_per_page / 2);  // - 1;
      if (t_r > radio_station_count) t_r = radio_station_count;
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Список станций"), GP_YELLOW);
      ListRadioStation(false, t_r, cur_radio_type, cur_radio_location);
      GP.HR();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }
    if (ui.uri() == "/radio_list_prev") {
      int t_r = start_pos - (radio_per_page / 2);  // + 1;
      if (t_r < 1) t_r = 1;
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Список станций"), GP_YELLOW);
      ListRadioStation(false, t_r, cur_radio_type, cur_radio_location);
      GP.HR();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }
    if (ui.uri() == "/radio_find_next") {
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Результаты поиска"), GP_YELLOW);
      Serial.println(start_pos);
      SearchRadioStation(start_pos, cur_radio_type, cur_radio_location, find_radio_text);
      GP.HR();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }
    if (ui.uri() == "/radio_find_prev") {
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Результаты поиска"), GP_YELLOW);
      SearchRadioStation(0, cur_radio_type, cur_radio_location, find_radio_text);
      GP.HR();
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }
    if (ui.uri() == "/radio_edit") {
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Редактор станций"), GP_YELLOW);
      M_BOX(GP.LABEL(F("URI станции:"), "", GP_DEFAULT, 14, 0, 1); GP.TEXT(F("edit_radio_uri"), F("введите адрес"), edit_radio_uri););
      GP.BUTTON_MINI(F("url_btn"), F("Проверить"), "", GP_PINK, F("98% !important"));
      M_BOX(GP.LABEL(F("Жанр:"), "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI_LINK(F("/edit_radio_type"), radio_type_title[edit_radio_typ], GP_BLACK, F("98% !important;"), "edit_radio_typ"););
      M_BOX(GP.LABEL(F("Локация:"), "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI_LINK(F("/edit_radio_location"), radio_location_title[edit_radio_loc], GP_BLACK, F("98% !important;"), "edit_radio_loc"););
      GP.BUTTON_MINI_LINK("/", F("Сохранить"), GP_GREEN, F("98% !important;"), F("save_station"));
      GP.BUTTON_MINI_LINK("/", F("Вернуться в Радио"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Выбор типа радиостанции
    if (ui.uri() == "/edit_radio_type") {
      GP.BLOCK_BEGIN(GP_TAB, "100%", F("Жанр радиостанции"), GP_YELLOW);
      for (int i = 0; i <= radio_type_count; ++i) {
        if (i == edit_radio_typ) {
          GP.BUTTON_MINI_LINK(F("/radio_edit"), radio_type_title[i], GP_RED, F("98% !important;"), "edit_radio_type/" + String(i));
        } else {
          GP.BUTTON_MINI_LINK(F("/radio_edit"), radio_type_title[i], GP_GREEN, F("98% !important;"), "edit_radio_type/" + String(i));
        }
      }
      GP.BREAK();
      GP.BUTTON_MINI_LINK(F("/radio_edit"), F("Вернуться"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Выбор локации радиостанции
    if (ui.uri() == "/edit_radio_location") {
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Локация радио"), GP_YELLOW);
      for (int i = 0; i <= radio_location_count; ++i) {
        if (i == edit_radio_loc) {
          GP.BUTTON_MINI_LINK(F("/radio_edit"), radio_location_title[i], GP_YELLOW, F("98% !important;"), "edit_radio_location/" + String(i));
        } else {
          GP.BUTTON_MINI_LINK(F("/radio_edit"), radio_location_title[i], GP_BLUE, F("98% !important;"), "edit_radio_location/" + String(i));
        }
      }
      GP.BREAK();
      GP.BUTTON_MINI_LINK(F("/radio_edit"), F("Вернуться"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Добавление музыкальной композиции к списку избранных
    if (ui.uri() == "/add_favorites1") {
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Любимые композиции"), GP_YELLOW);
      M_BOX(GP.LABEL("Добавить текущую композицию <b>«" + station + "»</b> в список любимых станций?", "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI("add_fav1_btn", F("Да"), "", GP_GREEN, F("60px important!"), false, true););
      favorites_flag1 = true;
      int fav_station_count = CountMusic(-1);
      favorites_flag1 = false;
      if (fav_station_count > 0) {
        M_BOX(GP.LABEL(F("Загрузить список любимых композиций?"), "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI("load_fav1_btn", F("Да"), "", GP_ORANGE, F("60px important!")););
      } else {
        GP.BREAK();
        M_BOX(GP.LABEL(F("Список любимых композиций пока пуст."), "", GP_RED, 14, 0, 1););
      }
      GP.BUTTON_MINI_LINK(F("/player"), F("Вернуться в Плеер"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Удаление композиции из списка избранных
    if (ui.uri() == "/del_favorites1") {
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Любимые композиции"), GP_YELLOW);
      int fav_station_count = CountMusic(-1);
      if (fav_station_count > 0) {
        M_BOX(GP.LABEL("Удалить текущую композицию <b>«" + station + "»</b> из списка любимых?", "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI("del_fav1_btn", F("Да"), "", GP_RED, F("60px important!"), false, true););
      } else {
        GP.BREAK();
        M_BOX(GP.LABEL(F("Список любимых композиций пока пуст."), "", GP_RED, 14, 0, 1););
      }
      M_BOX(GP.LABEL(F("Загрузить все композиции?"), "", GP_DEFAULT, 14, 0, 1); GP.BUTTON_MINI("load_all1_btn", F("Да"), "", GP_ORANGE, F("60px important!")););
      GP.BUTTON_MINI_LINK(F("/player"), F("Вернуться в Плеер"), GP_GRAY, F("98% !important;"), "");
      GP.BLOCK_END();
    }

    //Настройки
    if (ui.uri() == "/settings") {
      for (int i = 0; i < MAX_ALARMS; i++) {
        sleep_time[i] = GPtime(sHour[i], sMinute[i], 0);
        waikup_time[i] = GPtime(wHour[i], wMinute[i], 0);
      }
      GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Настройки"), GP_GRAY_B);

      GP.SPOILER_BEGIN(F("Экран и шрифт"), GP_ORANGE);
      M_BOX(GP.LABEL(F("Тёмная тема WEB:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("themas"), themas, GP_ORANGE););
      M_BOX(GP.LABEL(F("Палитра:"), "", GP_DEFAULT, 16, 0, 0); GP.SELECT(F("palette"), palettes, cur_palette, 0););
      M_BOX(GP.LABEL(F("Яркость:"), "", GP_DEFAULT, 16, 0, 0); GP.SLIDER(F("cur_led"), cur_led, 1, 25, 1, 0, GP_ORANGE, 0););
      M_BOX(GP.LABEL(F("Выкл. (мин.):"), "", GP_DEFAULT, 16, 0, 0); GP.SLIDER(F("led_time"), led_time, 0, 30, 1, 0, GP_ORANGE, 0););
      M_BOX(GP.LABEL(F("Растровый шрифт:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("sys_font"), sys_font, GP_ORANGE););
#ifndef BATTERY_OFF
      M_BOX(GP.LABEL(F("Заряд аккум. в %:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("voltage_mode"), voltage_mode, GP_ORANGE););
#endif
      GP.SPOILER_END();
      GP.BREAK();

      GP.SPOILER_BEGIN(F("Звук"), GP_YELLOW);
      M_BOX(GP.LABEL(F("АРУ громкости:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("aru"), aru, GP_YELLOW););
      M_BOX(GP.LABEL(F("Моно:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("mono"), mono, GP_YELLOW););
      M_BOX(GP.LABEL(F("Озвучивание меню:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("voice_menu"), voice_menu_flag, GP_YELLOW););
      GP.SPOILER_END();
      GP.BREAK();

      GP.SPOILER_BEGIN(F("Часы"), GP_RED);
      M_BOX(GP.LABEL(F("Часовая зона:"), "", GP_DEFAULT, 16, 0, 1); GP.TEXT(F("time_zone"), F("число"), String(time_zone), F("50%"), 6, F("^[ 0-9]+$")););
      GP.LABEL(F("Например, GMT+1 = 3600. Москва находится в зоне GMT+3 = 10800."), "", GP_DEFAULT, 14, 0, 1);
      GP.BREAK();
      GP.BREAK();
      GP.LABEL(F("Куранты:"), "", GP_DEFAULT, 18, 0, 0);
      M_BOX(GP.SWITCH(F("kurant_flag"), kurant_flag, GP_CYAN_B); GP.SELECT(F("kurant_type"), kurant_type_list, cur_kurant););

      GP.SPOILER_BEGIN(F("Будильники на включение"), GP_GREEN);
      M_BOX(GP.LABEL(F("Гр.")); GP.SLIDER(F("cur_volumea"), volumea, 0, 21, 1, 0, GP_ORANGE, 0););
      for (int i = 0; i < MAX_ALARMS; i++) {
        M_BOX(
          GP.SWITCH("waikup_flag" + String(i), waikup_flag[i], GP_GREEN);
          GP.TIME("waikup_time" + String(i), waikup_time[i]););
        GP.SELECT("alarm_mode" + String(i), alarm_mode_list, alarm_mode[i], 0);
        bool active = waikup_flag[i];

        for (int section = 0; section < 2; section++) {
          int start = section * 5;
          int end = section == 0 ? 5 : 8;

          // Установка цветов в зависимости от активности и дня недели
          for (int d = start; d < end; d++) {
            if (active) {
              if (d < 5) GP.LABEL(days[d], "", GP_DEFAULT, 12, 0, 0);
              else if (d < 7) GP.LABEL(days[d], "", GP_YELLOW, 12, 0, 0);
              else if (d < 8) GP.LABEL(days[d], "", GP_CYAN_B, 12, 0, 0);
            } else {
              if (d < 7) GP.LABEL(days[d], "", GP_GRAY, 12, 0, 0);
              else if (d < 8) GP.LABEL(days[d], "", GP_GRAY_B, 12, 0, 0);
            }
          }
          GP.BREAK();
          for (int d = start; d < end; d++) {
            if (active) {
              if (d < 7) GP.CHECK("waikup_day_" + String(i) + String(d), waikup_day[i][d], GP_GREEN);
              else if (d < 8) GP.CHECK("waikup_day_" + String(i) + String(d), waikup_day[i][d], GP_CYAN_B);
            } else {
              if (d < 7) GP.CHECK("waikup_day_" + String(i) + String(d), waikup_day[i][d], GP_GRAY);
              else if (d < 8) GP.CHECK("waikup_day_" + String(i) + String(d), waikup_day[i][d], GP_GRAY_B);
            }
          }
          if (section == 0) GP.BREAK();
        }
        GP.HR(GP_GRAY);
      }
      GP.SPOILER_END();
      GP.SPOILER_BEGIN(F("Будильники на выключение"), GP_RED);
      for (int i = 0; i < MAX_ALARMS; i++) {
        M_BOX(
          GP.SWITCH("sleep_flag" + String(i), sleep_flag[i], GP_RED);
          GP.TIME("sleep_time" + String(i), sleep_time[i]););

        bool isActive = sleep_flag[i];

        // Обработка дней недели (0-4 и 5-7)
        for (int section = 0; section < 2; section++) {
          int startDay = section * 5;
          int endDay = section == 0 ? 5 : 8;

          // Отображение названий дней
          for (int d = startDay; d < endDay; d++) {
            if (isActive) {
              if (d < 5) GP.LABEL(days[d], "", GP_DEFAULT, 12, 0, 0);
              else if (d < 7) GP.LABEL(days[d], "", GP_YELLOW, 12, 0, 0);
              else if (d < 8) GP.LABEL(days[d], "", GP_PINK_B, 12, 0, 0);
            } else {
              if (d < 7) GP.LABEL(days[d], "", GP_GRAY, 12, 0, 0);
              else if (d < 8) GP.LABEL(days[d], "", GP_GRAY_B, 12, 0, 0);
            }
          }
          GP.BREAK();
          for (int d = startDay; d < endDay; d++) {
            if (isActive) {
              if (d < 7) GP.CHECK("sleep_day_" + String(i) + String(d), sleep_day[i][d], GP_RED);
              else if (d < 8) GP.CHECK("sleep_day_" + String(i) + String(d), sleep_day[i][d], GP_PINK_B);
            } else {
              if (d < 7) GP.CHECK("sleep_day_" + String(i) + String(d), sleep_day[i][d], GP_GRAY);
              else if (d < 8) GP.CHECK("sleep_day_" + String(i) + String(d), sleep_day[i][d], GP_GRAY_B);
            }
          }
          if (section == 0) GP.BREAK();
        }
        GP.HR(GP_GRAY);
      }
      GP.SPOILER_END();
      GP.SPOILER_END();
      GP.BREAK();
      GP.SPOILER_BEGIN(F("Радио"), GP_GREEN);
      M_BOX(GP.LABEL(F("Выбор из списка:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("preview_mode_flag"), preview_mode_flag, GP_GREEN););
      M_BOX(GP.LABEL(F("Способ выбора,<br>с кликом или без:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("voice_sw"), voice_sw_flag, GP_GREEN););
      M_BOX(GP.LABEL(F("Автовозврат:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("autoscan"), autoscan, GP_GREEN););
      M_BOX(GP.LABEL(F("Удалять<br>не рабочие?:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("del_st_sw"), del_st_sw_flag, GP_GREEN););
      M_BOX(GP.LABEL(F("Удалять<br>медленные?:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("del_slow_sw"), del_st_slow_flag, GP_GREEN););
      M_BOX(GP.LABEL(F("Сканирование...:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("skan"), skan, GP_GREEN););
      M_BOX(GP.LABEL(F("Станций<br>на странице:"), "", GP_DEFAULT, 16, 0, 0); GP.SLIDER(F("radio_per_page"), radio_per_page, 50, 500, 50, 0, GP_GREEN_B, 0););
      GP.SPOILER_END();
      GP.BREAK();

      GP.SPOILER_BEGIN(F("Проигрыватель"), GP_BLUE);
      M_BOX(GP.LABEL(F("Случайная композиция:"), "", GP_DEFAULT, 16, 0, 1); GP.SWITCH(F("rand_song"), rand_song, GP_BLUE););
      GP.BREAK();
      GP.LABEL(index_info, F("index_info"), GP_DEFAULT, 14, 0, 1);
      GP.BUTTON_MINI(F("index_btn"), F("Индексировать"), "", GP_BLUE_B, F("90% !important;"));
      GP.SPOILER_END();
      GP.BREAK();

      M_BOX(GP.LABEL(F("Автосохранение:"), "", GP_DEFAULT, 17, 1, 1); GP.SWITCH(F("auto_save"), auto_save, GP_GREEN););
      GP.ALERT("save_conf", F("Настройки сохранены"));
      GP.BUTTON_MINI("save_btn", F("Сохранить настройки"), "", GP_GREEN, F("90% !important;"));
      GP.UPDATE_CLICK(F("save_conf"), F("save_btn"));
      GP.BLOCK_END();
    }

    //Информация
    if (ui.uri() == "/info") {
      String info = F("\n<style>\n.info {font-size:12px;color:#b4b700;font-weight:bold;}\n</style>\n");
      info += F("Ревизия чипа: <span class='info'>");
      info += String(ESP.getChipRevision()) + "</span><br>";
      info += F("Версия SDK: <span class='info'>");
      info += String(ESP.getSdkVersion()) + "</span><br>";
      uint64_t chipid = ESP.getEfuseMac();
      char strBuf[50];
      info += F("ESP32 Chip ID: <span class='info'>");
      sprintf(strBuf, "%04X", (uint16_t)(chipid >> 32));
      info += strBuf;
      sprintf(strBuf, "%08X", (uint32_t)chipid);
      info += strBuf;
      info += F("</span><br>Размер скетча: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getSketchSize());
      info += strBuf;
      info += F("</span> байт<br>Размер свободной памяти под скетч: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getFreeSketchSpace());
      info += strBuf;
      info += F("</span> байт<br>Размер Flash памяти: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getFlashChipSize());
      info += strBuf;
      info += F("</span> байт<br>Частота Flash памяти: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getFlashChipSpeed());
      info += strBuf;
      info += F("</span> Гц<br>Объём свободной памяти: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getFreeHeap());
      info += strBuf;
      info += F("</span> байт<br>Общий объём памяти: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getHeapSize());
      info += strBuf;
      info += F("</span> байт<br>Размер самого большого блока свободной памяти: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getMaxAllocHeap());
      info += strBuf;
      info += F("</span> байт<br>Наименьший объём свободной памяти с начала загрузки: <span class='info'>");
      sprintf(strBuf, "%d", ESP.getMinFreeHeap());
      info += strBuf;
      info += F("</span> байт\n");

      String info1 = F("\n<style>\n.info1 {font-size:12px;color:#3b3ebd;font-weight:bold;}\n</style>\n");
      info1 += F("Время текущего воспроизведения: <br><span class='info1'>");
      unsigned long tpt = audio.getTotalPlayingTime() / 1000;
      sprintf(strBuf, "%d дн. %02d ч. %02d м. %02d с.", (tpt / (3600 * 24)), ((tpt / 3600) % 24), ((tpt / 60) % 60), (tpt % 60));
      info1 += strBuf;
      info1 += F("</span><br>Общее время воспроизведения: <br><span class='info1'>");
      sprintf(strBuf, "%d дн. %02d ч. %02d м. %02d с.", ((totalTime + tpt) / (3600 * 24)), (((totalTime + tpt) / 3600) % 24), (((totalTime + tpt) / 60) % 60), ((totalTime + tpt) % 60));
      info1 += strBuf;
      info1 += F("</span><br>Пробег (в часах): <span class='info1'>");
      info1 += String(total_hours);
      info1 += F("</span>");
      M_BLOCK_TAB(
        F("Об устройстве"),
        GP.SPOILER_BEGIN(F("«Железо»"), GP_YELLOW);
        GP.LABEL(info, "", GP_DEFAULT, 12, 0, 1);
        GP.SPOILER_END();
        GP.BREAK();
        GP.SPOILER_BEGIN(F("Пробег"), GP_GRAY_B);
        GP.LABEL(info1, "", GP_DEFAULT, 12, 0, 1);
        GP.SPOILER_END(););
    }
    GP.UPDATE(F("warrning"));
    GP.ALERT(F("warrning"));

    GP.NAV_TABS_LINKS(F("/settings,/info"), F("Настройки,Информация"));

    //-------------------------------------
    reload_flag = false;
    GP.SEND(F("<div style='display: none; visibility: hidden;'>\n"));
    GP.NUMBER(F("radio_mode"), "", radio_mode);
    GP.NUMBER(F("f_mode"), "", radio_mode);
    GP.NUMBER(F("f_reload"), "", reload_flag);
    GP.SEND(F("</div>\n"));
    GP.JS_BEGIN();
    GP.SEND(F("const r_mode = document.getElementById('radio_mode');\n"));
    GP.SEND(F("const f_mode = document.getElementById('f_mode');\n"));
    GP.SEND(F("const f_reload = document.getElementById('f_reload');\n"));
    GP.SEND(F("const timerId = setInterval(() => {\n"));
    GP.SEND(F("if (r_mode.value != f_mode.value) {\n"));
    GP.SEND(F("if (r_mode.value == '0') window.location = '/';\n"));
    GP.SEND(F("if (r_mode.value == '1') window.location = '/clock';\n"));
    GP.SEND(F("if (r_mode.value == '2') window.location = '/player';\n"));
    GP.SEND(F("}\n"));
    GP.SEND(F("if (f_reload.value == '1') {f_reload.value = '0'; window.location.reload();}\n"));
    GP.SEND(F("}, 1000);\n"));
    GP.JS_END();
  } else {
    GP_CONNECT();
  }
  GP.LABEL("Версия: <b>" + String(verMajor) + "." + String(verMinor) + "</b>", "", GP_YELLOW, 14);
  GP.LABEL(copyright, "", GP_DEFAULT, 11, 0, true);
  GP.PAGE_END();
  GP.BUILD_END();
}

void GP_RADIO() {
  if (old_radio_station != radio_station || radio_mode != 0) {
    audio.setVolume(0);
    preview_flag = false;
    pause_control();
    clearInfo();
    //вызывает ReplayStation() из процесса Radio
    replay_station_flag = true;
  }
  radio_mode = 0;
  fix_mode = 0;
  //menu = 0;
  update_flag = true;
  GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Радио"), GP_GRAY_B);
  GP.TABLE_BEGIN(F("40px,100%,40px"));
  GP.TR();
  GP.TD();
  if (favorites_flag == false) {
    GP.BUTTON_MINI_LINK(F("/add_favorites"), "❤", GP_GREEN, F("40px !important"), F("favorites"));
  } else {
    GP.BUTTON_MINI_LINK(F("/del_favorites"), "♬", GP_GREEN, F("40px !important"), F("favorites"));
  }
  GP.SEND(F("</td>"));
  GP.TD();
  if (favorites_flag == true) {
    GP.LABEL(F("Любимые станции"), "", GP_GREEN, 14, 0, 1);
  } else {
    GP.LABEL(F("Весь плейлист"), "", GP_GREEN, 14, 0, 1);
  }
  GP.BREAK();
  GP.LABEL(radio_type_title[cur_radio_type], "", GP_GREEN_B, 18, 1, 1);
  GP.BREAK();
  GP.LABEL("\n(" + radio_location_title[cur_radio_location] + ")", "", GP_GREEN, 16, 1, 1);
  GP.SEND(F("</td>"));
  GP.TD(GP_RIGHT);
  GP.BUTTON_MINI_LINK(F("/radio_type"), "▼", GP_GREEN, F("40px !important"), F("c_radio_type"));
  GP.SEND(F("</td></tr>"));
  GP.TABLE_END();
  GP.LABEL(F("Станция"), "", GP_DEFAULT, 14, 0, 0);
  GP.LABEL(String(cur_radio_station), F("cur_radio_station"), GP_ORANGE, 14, 1, 0);
  GP.LABEL("/", "", GP_DEFAULT, 14, 0, 0);
  GP.LABEL(String(radio_station_count), F("radio_station_count"), GP_ORANGE, 14, 0, 0);
  GP.BREAK();
  GP.SEND(F("<div class='VUlevel-container'><div class='VUlevel'></div></div>\n"));
  GP.TABLE_BEGIN();
  GP.TR();
  GP.TD();
  GP.BUTTON_MINI(F("prev10_st_btn"), "10◄", "", GP_GREEN, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("prev_st_btn"), "◄", "", GP_GREEN_B, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("pause_st_btn"), "⏻", "", GP_RED, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("next_st_btn"), "►", "", GP_GREEN_B, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("next10_st_btn"), "►10", "", GP_GREEN, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td></tr>"));
  GP.TABLE_END();
  if (cur_radio_type == 0) {
    GP.LABEL(cur_type_title, F("cur_type_title"), GP_ORANGE, 16, 1, 1);
    GP.BREAK();
  }
  if (cur_radio_location == 0) {
    GP.LABEL(cur_location_title, F("cur_location_title"), GP_ORANGE, 16, 1, 1);
    GP.BREAK();
  }
  GP.LABEL(station, F("station"), GP_YELLOW, 20, 1, 1);
  GP.BREAK();
  if (themas == true) {
    GP.LABEL(title, F("title"), GP_GREEN_B, 16, 0, 1);
  } else {
    GP.LABEL(title, F("title"), GP_GREEN, 16, 0, 1);
  }
  GP.BREAK();
  GP.LABEL(curl, F("curl"), GP_DEFAULT, 11, 0, 1);
  GP.SEND(F("<style>#curl {overflow-wrap: anywhere !important; white-space: pre-wrap !important; -webkit-hyphens: none !important;}</style>\n"));
  GP.HR();
  GP.LABEL(F("Connect quality:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(100 / (slow + 1)), F("slow"), GP_ORANGE_B, 13, 1, 0);
  GP.LABEL(F("%"), "", GP_DEFAULT, 13, 0, 0);
  GP.BREAK();
  if (aru == true) {
    GP.LABEL(F("Volume correction:"), "", GP_DEFAULT, 13, 0, 0);
    GP.LABEL(String(vk), F("vk"), GP_ORANGE_B, 13, 1, 0);
    GP.LABEL(F("dB"), "", GP_DEFAULT, 13, 0, 0);
    GP.BREAK();
  }
  GP.LABEL(F("Channels:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(channels), F("channels"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("Sample rate:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(sample_rate), F("sample_rate"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("Bits per sample:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(bits_per_sample), F("bits_per_sample"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("Bitrate:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(bit_rate), F("bit_rate"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("AAC Header Format:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(aac_header_format, F("aac_header_format"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("AAC Codec:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(aac_codec, F("aac_codec"), GP_ORANGE, 13, 1, 0);
  GP.HR();
  //Редактирование плейлиста
  GP.TABLE_BEGIN("", nullptr, F("120px;"));
  GP.TR();
  GP.TD();
  GP.BUTTON_MINI_LINK(F("/radio_list"), "&#9776;", GP_GREEN, F("37px !important; margin:4px 0px !important;"), F("radio_list"));
  GP.SEND("</td>");
  GP.TD();
  GP.BUTTON_MINI_LINK(F("/find_radio"), "🔎", GP_GREEN, F("37px !important; margin:4px 0px !important;"), F("find_radio"));
  GP.SEND("</td>");
  GP.TD();
  GP.BUTTON_MINI_LINK(F("/"), "🎲", GP_GREEN, F("37px !important; margin:4px 0px !important;"), F("radio_rnd"));
  GP.SEND("</td>");
  GP.TD();
  GP.BUTTON_MINI_LINK(F("/"), "⇆", GP_GREEN, F("37px !important; margin:4px 0px !important;"), F("repeat_radio"));
  GP.SEND("</td>");
  GP.TD();
  GP.BUTTON_MINI_LINK(F("/radio_edit"), "✚", GP_GREEN, F("37px !important; margin:4px 0px !important;"), F("radio_new"));
  GP.SEND("</td>");
  GP.TD();
  GP.BUTTON_MINI_LINK(F("/radio_edit"), "✎", GP_GREEN, F("37px !important; margin:4px 0px !important;"), F("radio_edit"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.CONFIRM(F("del"), F("Желаете безвозвратно удалить текущую радиостанцию?"));
  GP.BUTTON_MINI_LINK("/", "✘", GP_RED, F("37px !important; margin:4px 0px !important;"), F("radio_del"));
  GP.UPDATE_CLICK(F("del"), F("radio_del"));
  GP.SEND(F("</td></tr>"));
  GP.TABLE_END();
  GP.BLOCK_END();
  //Блок регулировки звука
  GP_VOLUME();
}

void GP_CLOCK() {
  radio_mode = 1;
  menu = 1;
  preview_flag = false;
  update_flag = true;
  old_time = "";

  GP.SEND(F("<style>\n"));
  GP.SEND(F("#rclock_panel {width: 212px; margin: 8px; padding: 14px; display: inline-block; -moz-border-radius: 8px; -khtml-border-radius: 8px; -webkit-border-radius: 8px; border: 8px solid #4e1002; outline: 8px solid #e3e3e3; border-radius: 0.25em; background: linear-gradient(to top, #d66a00 10%, #ff9023 50%, #d66a00 90%); font-family: monospace, sans-serif; background-color: #fb7c00; color: #4e1002; align-items: center; justify-content: center;}\n"));
  GP.SEND(F("#rdate {font-size: 26px;}\n"));
  GP.SEND(F("#rclock {font-size: 44px; font-weight: bold; transform: scaleY(1.5); transform-origin: 0 0; margin-bottom: 24px;}\n"));
  GP.SEND(F("#rday {letter-spacing: 2px; font-size: 26px; font-weight: normal;}\n"));
  GP.SEND(F("</style>\n\n"));

  GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Часы"), GP_GRAY_B);
  GP.SEND(F("<div id='rclock_panel'>\n"));
  GP.SEND(F("<div id='rdate'></div>\n"));
  GP.SEND(F("<div id='rclock'></div>\n"));
  GP.SEND(F("<div id='rday'></div>\n"));
  GP.SEND(F("</div>\n\n"));
  GP.BLOCK_END();

  GP.SEND(F("<script>\n"));
  GP.SEND(F("var rweek = ['ВОСКРЕСЕНЬЕ', 'ПОНЕДЕЛЬНИК', 'ВТОРНИК', 'СРЕДА', 'ЧЕТВЕРГ', 'ПЯТНИЦА', 'СУББОТА'];\n"));
  GP.SEND(F("var rdate;\n"));
  GP.SEND(F("function clock() {\n"));
  GP.SEND(F("rdate = new Date();\n"));
  GP.SEND(F("document.getElementById('rdate').innerText = rdate.toLocaleDateString();\n"));
  GP.SEND(F("document.getElementById('rclock').innerText = rdate.toLocaleTimeString();\n"));
  GP.SEND(F("document.getElementById('rday').innerText = rweek[rdate.getDay()];\n"));
  GP.SEND(F("setTimeout(clock, 1000);\n"));
  GP.SEND(F("}\n"));
  GP.SEND(F("clock();\n"));
  GP.SEND(F("</script>\n\n"));
}

void GP_PLAYER() {
  if (radio_mode == 0) {
    stop_song_Play();
    clearInfo();
  }
  radio_mode = 2;
  fix_mode = 2;
  preview_flag = false;
  s_afd = 0;
  info_flag = true;
  display_ready = false;
  add_station = 0;
  old_rand_song = rand_song;
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
    //clearInfo();
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
      if (!audio.isRunning()) {
        if (cur_music_file > music_count) cur_music_file = music_count;
        //clearInfo();
        play_music_flag = true;
      }
    } else {
      cur_music_file = 0;
      cur_message = F("Список музыкальных произведений пуст.");
      title = cur_message;
      warrning_flag = true;
    }
  }
  update_flag = true;
  update_timer = millis();
  continue_flag = false;
  GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Проигрыватель"), GP_GRAY_B);

  GP.TABLE_BEGIN(F("40px,100%,40px"));
  GP.TR();
  GP.TD();
  if (favorites_flag1 == false) {
    GP.BUTTON_MINI_LINK(F("/add_favorites1"), "❤", GP_BLUE, F("40px !important"), F("favorites1"));
  } else {
    GP.BUTTON_MINI_LINK(F("/del_favorites1"), "✘", GP_BLUE, F("40px !important"), F("favorites1"));
  }
  GP.SEND(F("</td>"));
  GP.TD();
  if (favorites_flag1 == true) {
    GP.LABEL(F("Любимые мелодии"), "", GP_BLUE, 14, 0, 1);
  } else {
    GP.LABEL(F("Весь плейлист"), "", GP_BLUE, 14, 0, 1);
  }
  GP.BREAK();
  GP.LABEL(m_type_title, F("m_type_title"), GP_BLUE_B, 18, 1, 1);
  GP.SEND(F("</td>"));
  GP.TD(GP_RIGHT);
  GP.BUTTON_MINI_LINK(F("/music_type"), "▼", GP_BLUE, F("40px !important"), F("c_music_type"));
  GP.SEND(F("</td></tr>"));
  GP.TABLE_END();
  GP.LABEL(F("Композиция"), "", GP_DEFAULT, 14, 0, 0);
  GP.LABEL(String(cur_music_file), F("cur_music_file"), GP_ORANGE, 14, 1, 0);
  GP.LABEL("/", "", GP_DEFAULT, 14, 0, 0);
  GP.LABEL(String(music_count), F("music_count"), GP_ORANGE, 14, 0, 0);
  GP.BREAK();
  GP.SEND(F("<div class='VUlevel-container'><div class='VUlevel'></div></div>\n"));
  GP.TABLE_BEGIN();
  GP.TR();
  GP.TD();
  GP.BUTTON_MINI(F("prev10_mus_btn"), "10◄", "", GP_BLUE, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("prev_mus_btn"), "◄", "", GP_BLUE_B, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("pause_mus_btn"), "⏻", "", GP_RED, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("next_mus_btn"), "►", "", GP_BLUE_B, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("next10_mus_btn"), "►10", "", GP_BLUE, F("100% !important; margin:4px 0px !important;"));
  GP.SEND(F("</td></tr>"));
  GP.TABLE_END();

  GP.LABEL(cur_im, F("cur_im"), GP_ORANGE, 14, 0, 0);
  GP.LABEL("-", "", GP_DEFAULT, 14, 0, 0);
  GP.LABEL(max_im, F("max_im"), GP_ORANGE, 14, 0, 0);
  GP.BREAK();

  GP.LABEL(station, F("station"), GP_YELLOW, 20, 1, 1);
  GP.BREAK();
  if (themas == true) {
    GP.LABEL(title, F("title"), GP_BLUE_B, 16, 0, 1);
  } else {
    GP.LABEL(title, F("title"), GP_BLUE, 16, 0, 1);
  }
  GP.LABEL(mp3_info, F("mp3_info"), GP_DEFAULT, 13, 0, 1);
  GP.BREAK();
  GP.LABEL(curl, F("curl"), GP_DEFAULT, 11, 0, 1);
  GP.SEND(F("<style>#curl {overflow-wrap: anywhere !important; white-space: pre-wrap !important;}</style>\n"));
  GP.HR();
  if (aru == true) {
    GP.LABEL(F("Volume correction:"), "", GP_DEFAULT, 13, 0, 0);
    GP.LABEL(String(vk), F("vk"), GP_ORANGE_B, 13, 1, 0);
    GP.LABEL(F("dB"), "", GP_DEFAULT, 13, 0, 0);
    GP.BREAK();
  }
  GP.LABEL(F("Channels:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(channels), F("channels"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("Sample rate:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(sample_rate), F("sample_rate"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("Bits per sample:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(bits_per_sample), F("bits_per_sample"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("Bitrate:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(String(bit_rate), F("bit_rate"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("AAC Header Format:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(aac_header_format, F("aac_header_format"), GP_ORANGE, 13, 1, 0);
  GP.BREAK();
  GP.LABEL(F("AAC Codec:"), "", GP_DEFAULT, 13, 0, 0);
  GP.LABEL(aac_codec, F("aac_codec"), GP_ORANGE, 13, 1, 0);
  GP.TABLE_BEGIN();
  GP.TR();
  GP.TD();
  GP.BUTTON_MINI(F("music_rnd"), "🎲", "", GP_BLUE, F("80px !important; margin:4px 0px !important;"));
  GP.SEND(F("</td>"));
  GP.TD();
  GP.BUTTON_MINI(F("repeat_play"), "⇆", "", GP_BLUE, F("80px !important; margin:4px 0px !important;"));
  GP.SEND(F("</td></tr>"));
  GP.TABLE_END();
  //Блок регулировки звука
  GP_VOLUME();
}

void GP_AU() {
  radio_mode = 3;
  //menu = 3;
  add_station = 0;
  clearInfo();
  update_flag = true;
  GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Ау!"), GP_GRAY_B);
  GP.LABEL(F("Текст для озвучивания:"), "", GP_DEFAULT, 16, 0, 0);
  GP.AREA(F("au_text"), 5, au_text, F("100% !important"));
  M_BOX(GP.LABEL(F("Язык:"), "", GP_DEFAULT, 16, 0, 0); GP.SELECT(F("lang_list"), language_list, cur_lang, 0););
  GP.BUTTON_MINI(F("au_play_btn"), F("Прочитать"), "", GP_GREEN_B, F("100% !important"));
  GP.BLOCK_END();
  //Блок регулировки звука
  GP_VOLUME();
}

void GP_CONNECT() {
  M_BLOCK_TAB(
    F("Подключение"),
    GP.LABEL(F("Для работы интернет-радио необходимо подключить устройство к сети Интернет через роутер или wi-fi модем вашего гаджета."), "", GP_DEFAULT, 16, 0, 1);
    GP.BREAK();
    GP.LABEL(F("Выберите точку подключения:"), "", GP_DEFAULT, 16, 0, 1);
    GP.SELECT("wifi", wifi_ssid, cur_wifi);
    GP.BREAK(); GP.BREAK();
    if (saved_connect_flag == false) {
      GP.LABEL(F("Введите пароль:"), "", GP_DEFAULT, 16, 0, 1);
      GP.BREAK();
      GP.PASS("wifi_pass", F("Пароль"), "", "100%", 0);
      GP.BREAK();
      GP.BREAK();
    } GP.LABEL(F("После нажатия на кнопку OK устройство перезагрузится и после успешного подключения к сети Интернет отобразит на своём экране ip-адрес, через который можно будет управлять интернет-радио через web-интерфейс."), "", GP_DEFAULT, 16, 0, 1);
    GP.BREAK();
    GP.BUTTON_MINI(F("ok_btn"), F("OK"), "", GP_GREEN_B, F("100% !important")););
}

void GP_VOLUME() {
  GP.BLOCK_BEGIN(GP_TAB, F("100%"), F("Звук"), GP_GRAY_B);
  //------------------------------
  GP.SEND(F("<div style='display: none; visibility: hidden;'>\n"));
  GP.NUMBER(F("VUlevel"), "", VUlevel);
  GP.SEND(F("</div>\n"));
  GP.JS_BEGIN();
  GP.SEND(F("const VUlevelbar = document.querySelector('.VUlevel');\n"));
  GP.SEND(F("const changeVUlevel = (VUlevel) => {VUlevelbar.style.width = `${VUlevel}%`;};\n"));
  GP.SEND(F("setInterval(() => changeVUlevel(VUlevel.value), 10);\n"));
  GP.JS_END();
  GP.SEND(F("<style>\n"));
  GP.SEND(F(".VUlevel-container {height:0.4rem;width:100%;border-radius:0.2rem;background:#000;margin:4px 0;}\n"));
  GP.SEND(F(".VUlevel-container .VUlevel {height:100%;width:0;border-radius:0.2rem;background:#d85904;transition:width 0.6s ease;}\n"));
  GP.SEND(F("</style>\n"));
  //------------------------------
  M_BOX(GP.LABEL(F("Гр.")); GP.SLIDER(F("cur_volume"), volume, 0, 21, 1, 0, GP_ORANGE, 0););

  GP.TABLE_BEGIN("", nullptr, F("100px;"));
  GP.TR();
  GP.TD();
  GP.BUTTON_MINI(F("vol_dn_btn"), "◄", "", GP_ORANGE, F("40px !important; margin-top: -6px !important; font-size: 13px !important; min-height: 24px !important"));
  GP.SEND("</td>");
  GP.TD();
  GP.BUTTON_MINI(F("mute_btn"), "🔇", "", GP_ORANGE, F("40px !important; margin-top: -6px !important; font-size: 13px !important; min-height: 24px !important"));
  GP.SEND("</td>");
  GP.TD();
  GP.BUTTON_MINI(F("vol_up_btn"), "►", "", GP_ORANGE, F("40px !important; margin-top: -6px !important; font-size: 13px !important; min-height: 24px !important"));
  GP.SEND(F("</td></tr>"));
  GP.TABLE_END();

  GP.LABEL(F("Баланс"));
  GP.BREAK();
  M_BOX(GP.LABEL(F("ЛП")); GP.SLIDER(F("balance"), balance, -16, 16, 1, 0, GP_YELLOW, 0););
  GP.LABEL(F("Эквалайзер"));
  GP.BREAK();
  M_BOX(GP.LABEL(F("НЧ")); GP.SLIDER(F("low_freq"), low_freq, 0, 20, 1, 0, GP_RED, 0););
  M_BOX(GP.LABEL(F("CЧ")); GP.SLIDER(F("mid_freq"), mid_freq, 0, 20, 1, 0, GP_GREEN, 0););
  M_BOX(GP.LABEL(F("BЧ")); GP.SLIDER(F("hi_freq"), hi_freq, 0, 20, 1, 0, GP_CYAN, 0););

  //GP.BUTTON_MINI(F("get_html_btn"), "Get HTML", "", GP_BLUE_B, F("100% !important; margin:4px 0px !important;"));
  GP.BLOCK_END();
}

void action() {
  if (ui.click()) {
    LedOn();
    if (ui.click("test_btn")) {
      audio_pause();
      speakTime(cHour, cMinute);
      audio_play();
    }
    if (ui.click("ok_btn")) {
      wifi_ssid[cur_wifi].toCharArray(lp.ssid, 20);
      if (saved_connect_flag == false) {
        password.toCharArray(lp.pass, 20);
        SaveWiFi(wifi_ssid[cur_wifi], String(XORENC(lp.pass, password_key)));
        password.toCharArray(lp.pass, 20);
      } else {
        wfa = "---";
        wfa.toCharArray(lp.ssid, 20);
        if (SearchWiFi(wifi_ssid[cur_wifi]) == true) {
          wfa.toCharArray(lp.ssid, 20);
          wfp.toCharArray(lp.pass, 20);
          wfp = String(XORENC(lp.pass, password_key));
          wfp.toCharArray(lp.pass, 20);
        }
      }
      updateEEPROM();
      web_connect();
    }
    if (ui.click("prev10_st_btn")) {
      if (radio_station_count > 10) {
        //clearInfo();
        repeatr1();
        cur_radio_station = cur_radio_station - 10;
        if (cur_radio_station < 1) cur_radio_station = radio_station_count;
        pre_radio_station = cur_radio_station;
        // вызывает PlayStation() из процесса Radio
        play_station_flag = true;
        repeatr3();
        eeprom_timer = millis();
        eeprom_flag = true;
      }
    }
    if (ui.click("prev_st_btn")) {
      if (radio_station_count > 1) {
        //clearInfo();
        repeatr1();
        cur_radio_station--;
        if (cur_radio_station < 1) cur_radio_station = radio_station_count;
        pre_radio_station = cur_radio_station;
        // вызывает PlayStation() из процесса Radio
        play_station_flag = true;
        repeatr3();
        eeprom_timer = millis();
        eeprom_flag = true;
      }
    }
    if (ui.click("pause_st_btn")) {
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
          ReplayStation();
          update_flag = true;
        }
      }
    }
    if (ui.click("next_st_btn")) {
      if (radio_station_count > 1) {
        //clearInfo();
        repeatr1();
        cur_radio_station++;
        if (cur_radio_station > radio_station_count) cur_radio_station = 1;
        pre_radio_station = cur_radio_station;
        // вызывает PlayStation() из процесса Radio
        play_station_flag = true;
        repeatr3();
        eeprom_timer = millis();
        eeprom_flag = true;
      }
    }
    if (ui.click("next10_st_btn")) {
      if (radio_station_count > 10) {
        //clearInfo();
        repeatr1();
        cur_radio_station = cur_radio_station + 10;
        if (cur_radio_station > radio_station_count) cur_radio_station = 1;
        pre_radio_station = cur_radio_station;
        // вызывает PlayStation() из процесса Radio
        play_station_flag = true;
        repeatr3();
        eeprom_timer = millis();
        eeprom_flag = true;
      }
    }
    if (ui.click("cur_volume")) {
      volume = ui.getInt();
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("vol_dn_btn")) {
      if (volume > 0) volume--;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("cur_volumea")) {
      volumea = ui.getInt();
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("mute_btn")) {
      switch (radio_mode) {
        case 0:
          if (radio_station_count > 1) {
            if (audio.isRunning()) {
              //audio.setVolume(0);
              //audio.stopSong();
              stop_song_Play();
              pause_flag = true;
            } else {
              //вызывает ReplayStation() из процесса Radio
              replay_station_flag = true;
              pause_flag = false;
            }
          }
          break;
        case 2:
          if (music_count > 1) audio.pauseResume();
          break;
      }
    }
    if (ui.click("vol_up_btn")) {
      if (volume < 21) volume++;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("balance")) {
      balance = ui.getInt();
      audio.setBalance(-balance);
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("low_freq")) {
      low_freq = ui.getInt();
      audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("mid_freq")) {
      mid_freq = ui.getInt();
      audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("hi_freq")) {
      hi_freq = ui.getInt();
      audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("sys_font")) {
      sys_font = !sys_font;
      s_afd = 0;
      update_flag = true;
      old_time = "";
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("aru")) {
      aru = !aru;
      if (aru == false) {
        vk = 0;
      }
      pik_volume = 0;
      update_flag = true;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("mono")) {
      mono = !mono;
      update_flag = true;
      audio.forceMono(mono);
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("voice_menu")) {
      voice_menu_flag = !voice_menu_flag;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("voice_sw")) {
      voice_sw_flag = !voice_sw_flag;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("del_st_sw")) {
      del_st_sw_flag = !del_st_sw_flag;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("del_slow_sw")) {
      del_st_slow_flag = !del_st_slow_flag;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("skan")) {
      skan = !skan;
      counter_st = 0;
      cur_radio_station++;
      notend();
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("autoscan")) {
      autoscan = !autoscan;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("rand_song")) {
      rand_song = !rand_song;
      old_rand_song = rand_song;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("auto_save")) {
      auto_save = !auto_save;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("index_btn")) {
      index_info = F("Индексирую...");
      vTaskDelay(1000);
      IndexMusicFiles();
    }
    if (ui.click("preview_mode_flag")) {
      preview_mode_flag = !preview_mode_flag;
      update_flag = true;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("save_btn")) {
      updateEEPROM();
    }
    if (ui.click("themas")) {
      themas = !themas;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("palette")) {
      cur_palette = ui.getInt();
      setPalettes(cur_palette);
      old_time = "";
      update_flag = true;
      update_timer = millis();
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("kurant_flag")) {
      kurant_flag = !kurant_flag;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("kurant_type")) {
      cur_kurant = ui.getInt();
      Serial.println(cur_kurant);
    }
    for (int i = 0; i < MAX_ALARMS; i++) {
      if (ui.click("alarm_mode" + String(i))) {
        alarm_mode[i] = ui.getInt();
        eeprom_timer = millis();
        eeprom_flag = true;
      }
      if (ui.click("sleep_flag" + String(i))) {
        sleep_flag[i] = !sleep_flag[i];
        eeprom_timer = millis();
        eeprom_flag = true;
      }
      if (ui.click("waikup_flag" + String(i))) {
        waikup_flag[i] = !waikup_flag[i];
        eeprom_timer = millis();
        eeprom_flag = true;
      }
      if (ui.click("sleep_time" + String(i))) {
        sleep_time[i] = ui.getTime("sleep_time" + String(i));
        sHour[i] = sleep_time[i].hour;
        sMinute[i] = sleep_time[i].minute;
      }
      if (ui.click("waikup_time" + String(i))) {
        waikup_time[i] = ui.getTime("waikup_time" + String(i));
        wHour[i] = waikup_time[i].hour;
        wMinute[i] = waikup_time[i].minute;
      }
      for (int d = 0; d < 8; d++) {
        if (ui.click("waikup_day_" + String(i) + String(d))) {
          waikup_day[i][d] = ui.getBool();
        }
        if (ui.click("sleep_day_" + String(i) + String(d))) {
          sleep_day[i][d] = ui.getBool();
        }
      }
    }
    //Проигрыватель
    if (ui.click("prev10_mus_btn")) {
      old_rand_song = rand_song;
      rand_song = false;
      if (music_count > 10) {
        clearInfo();
        repeatm1();
        cur_music_file = cur_music_file - 10;
        if (cur_music_file < 1) cur_music_file = music_count;
        repeatm3();
        play_music_flag = true;
      }
    }
    if (ui.click("prev_mus_btn")) {
      old_rand_song = rand_song;
      rand_song = false;
      if (music_count > 1) {
        clearInfo();
        repeatm1();
        cur_music_file--;
        if (cur_music_file < 1) cur_music_file = music_count;
        repeatm3();
        play_music_flag = true;
      }
    }
    if (ui.click("pause_mus_btn")) {
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
          PlayMusic();
          update_flag = true;
        }
      }
    }
    if (ui.click("next_mus_btn")) {
      old_rand_song = rand_song;
      rand_song = false;
      if (music_count > 1) {
        clearInfo();
        repeatm1();
        cur_music_file++;
        if (cur_music_file > music_count) cur_music_file = 1;
        repeatm3();
        play_music_flag = true;
      }
    }
    if (ui.click("next10_mus_btn")) {
      old_rand_song = rand_song;
      rand_song = false;
      if (music_count > 10) {
        clearInfo();
        repeatm1();
        cur_music_file = cur_music_file + 10;
        if (cur_music_file > music_count) cur_music_file = 1;
        repeatm3();
        play_music_flag = true;
      }
    }
    if (ui.clickSub("radio_type")) {
      int c_r_t = ui.clickNameSub(1).toInt();
      int r_s_c = CountRadioStation(c_r_t, cur_radio_location);
      if (r_s_c > 0) {
        radio_station_count = r_s_c;
        cur_radio_type = c_r_t;
        cur_radio_station = 1;
        pre_radio_station = cur_radio_station;
        radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
        if (radio_station != old_radio_station) clearInfo();
        eeprom_timer = millis();
        eeprom_flag = true;
      } else {
        cur_message = F("Станций этого типа в списке пока нет.");
        title = cur_message;
        warrning_flag = true;
      }
    }
    if (ui.clickSub("radio_location")) {
      int c_r_t = ui.clickNameSub(1).toInt();
      int r_s_c = CountRadioStation(cur_radio_type, c_r_t);
      if (r_s_c > 0) {
        radio_station_count = r_s_c;
        cur_radio_location = c_r_t;
        cur_radio_station = 1;
        pre_radio_station = cur_radio_station;
        radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
        if (radio_station != old_radio_station) clearInfo();
        eeprom_timer = millis();
        eeprom_flag = true;
      } else {
        cur_message = F("Станций этого типа в списке пока нет.");
        title = cur_message;
        warrning_flag = true;
      }
    }
    if (ui.clickSub("music_notype")) {
      all_fav_flag = true;
      music_count = CountMusic(-1);
      if (music_count > 0) {
        cur_music_file = 1;
        clearInfo();
        play_music_flag = true;
      } else {
        cur_music_file = 0;
        cur_message = F("В этой папке музыки нет.");
        title = cur_message;
        warrning_flag = true;
      }
    }
    if (ui.clickSub("music_type")) {
      all_fav_flag = false;
      cur_music_type = ui.clickNameSub(1).toInt();
      ScanFiles(cur_music_type);
      if (music_count > 0) {
        continue_flag = false;
        cur_music_file = 1;
        clearInfo();
        play_music_flag = true;
      } else {
        cur_music_file = 0;
        cur_message = F("В этой папке музыки нет.");
        title = cur_message;
        warrning_flag = true;
      }
    }
    if (ui.click("music_rnd")) {
      if (music_count > 1) {
        clearInfo();
        old_rand_song = rand_song;
        repeatm1();
        rand_song = true;
        play_music_flag = true;
        repeatm3();
      }
    }
    if (ui.click("repeat_play")) {
      old_rand_song = rand_song;
      rand_song = false;
      if (cur_music_file == cur_music_file2 && cur_music_file2 != cur_music_file1) cur_music_file = cur_music_file1;
      else if (cur_music_file != cur_music_file3) cur_music_file = cur_music_file3;
      else if (cur_music_file != cur_music_file2) cur_music_file = cur_music_file2;
      ShowInfoPanel();
      play_music_flag = true;
    }
    if (ui.clickSub("load_fav_btn")) {
      favorites_flag = true;
      cur_radio_location = 0;
      cur_radio_type = 0;
      radio_station_count = CountRadioStation(cur_radio_type, cur_radio_location);
      if (add_station == 0) {
        cur_radio_station = 1;
      } else {
        cur_radio_station = add_station;
      }
      pre_radio_station = cur_radio_station;
      radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
      //вызывает ReplayStation() из процесса Radio
      replay_station_flag = true;
      update_flag = true;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.clickSub("add_fav_btn")) {
      mute_flag = true;
      add_station = AddStationToFav();
      //вызывает ReplayStation() из процесса Radio
      replay_station_flag = true;
      cur_message = F("Радиостанция добавлена в список любимых станций.");
      warrning_flag = true;
    }
    if (ui.clickSub("del_fav_btn")) {
      audio.setVolume(0);
      radio_station_count = DelStationFromFav();
      cur_radio_location = 0;
      cur_radio_type = 0;
      if (cur_radio_station > radio_station_count) cur_radio_station = radio_station_count;
      pre_radio_station = cur_radio_station;
      //clearInfo();
      radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
      //вызывает ReplayStation() из процесса Radio
      replay_station_flag = true;
      update_flag = true;
      cur_message = F("Радиостанция удалена из списка любимых станций.");
      warrning_flag = true;
    }
    if (ui.clickSub("load_all_btn")) {
      favorites_flag = false;
      cur_radio_location = 0;
      cur_radio_type = 0;
      mute_flag = true;
      radio_station_count = CountRadioStation(cur_radio_type, cur_radio_location);
      cur_radio_station = FindRadioStation(station_ini_string);
      if (cur_radio_station == 0) cur_radio_station = 1;
      pre_radio_station = cur_radio_station;
      //clearInfo();
      radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
      //вызывает ReplayStation() из процесса Radio
      replay_station_flag = true;
      update_flag = true;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.clickSub("add_fav1_btn")) {
      mute_flag = true;
      cur_music_file = AddMusicToFav();
      play_music_flag = true;
      cur_message = F("Композиция добавлена в список любимых.");
      warrning_flag = true;
    }
    if (ui.clickSub("load_fav1_btn")) {
      favorites_flag1 = true;
      update_flag = true;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.clickSub("del_fav1_btn")) {
      audio.setVolume(0);
      music_count = DelMusicFromFav();
      clearInfo();
      play_music_flag = true;
      update_flag = true;
      cur_message = F("Композиция удалена из списка любимых.");
      warrning_flag = true;
    }
    if (ui.clickSub("load_all1_btn")) {
      favorites_flag1 = false;
      update_flag = true;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    ui.clickInt("wifi", cur_wifi);
    ui.clickString("wifi_pass", password);

    if (ui.click("radio_rnd")) {
      audio.setVolume(0);
      mute_flag = true;
      repeatr1();
      cur_radio_station = random(1, radio_station_count + 1);
      if (cur_radio_station == 0 || cur_radio_station > radio_station_count) cur_radio_station = 1;
      repeatr3();
      pre_radio_station = cur_radio_station;
      ShowInfoPanel();
      // вызывает PlayStation() из процесса Radio
      play_station_flag = true;
    }
    if (ui.click("repeat_radio")) {
      if (cur_radio_station == cur_radio_station2 && cur_radio_station2 != cur_radio_station1) cur_radio_station = cur_radio_station1;
      else if (cur_radio_station != cur_radio_station3) cur_radio_station = cur_radio_station3;
      else if (cur_radio_station != cur_radio_station2) cur_radio_station = cur_radio_station2;
      ShowInfoPanel();
      PlayStation();
    }

    if (ui.click("cur_led")) {
      cur_led = ui.getInt();
      eeprom_timer = millis();
      eeprom_flag = true;
    }

    if (ui.click("led_time")) {
      led_time = ui.getInt();
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.clickSub("radio_item")) {
      audio.setVolume(0);
      mute_flag = true;
      repeatr1();
      cur_radio_station = ui.clickNameSub(1).toInt();
      if (cur_radio_station == 0 || cur_radio_station > radio_station_count) cur_radio_station = 1;
      pre_radio_station = cur_radio_station;
      repeatr3();
      ShowInfoPanel();
      // вызывает PlayStation() из процесса Radio
      play_station_flag = true;
    }
    if (ui.click("radio_new")) {
      new_station = true;
      edit_radio_uri = "";
      edit_radio_typ = 0;
      edit_radio_loc = 0;
    }
    if (ui.click("radio_edit")) {
      new_station = false;
      edit_radio_uri = radio_station;
      edit_radio_typ = fix_radio_type;
      edit_radio_loc = fix_radio_location;
    }
    if (ui.click("edit_radio_uri")) {
      edit_radio_uri = ui.getString();
    }
    if (ui.clickSub("edit_radio_type")) {
      edit_radio_typ = ui.clickNameSub(1).toInt();
    }
    if (ui.clickSub("edit_radio_location")) {
      edit_radio_loc = ui.clickNameSub(1).toInt();
    }
    if (ui.click("find_radio_text")) {
      find_radio_text = ui.getString();
    }
    if (ui.click("save_station")) {
      edit_radio_uri.trim();
      if (edit_radio_uri.indexOf("http") != -1 && edit_radio_uri.length() > 10) {
        favorites_flag = false;
        cur_radio_type = 0;
        cur_radio_location = 0;
        radio_station_count = CountRadioStation(0, 0);
        Serial.print(F("Количество радиостанций: "));
        Serial.println(radio_station_count);
        if (new_station == true) {
          cur_radio_station = FindRadioStation(edit_radio_uri);
          if (cur_radio_station > 0) {
            //Станция уже есть - не добавляем
            cur_message = F("Эта станция уже есть в списке радиостанций.");
            Serial.println(cur_message);
            warrning_flag = true;
          } else {
            mute_flag = true;
            cur_radio_station = EditStation();
          }
          pre_radio_station = cur_radio_station;
        } else {
          mute_flag = true;
          favorites_flag = true;
          EditStation();  // в списке избранных станций
          favorites_flag = false;
          cur_radio_station = EditStation();  // в основном списке
        }
        if (cur_radio_station == 0 || cur_radio_station > radio_station_count) cur_radio_station = 1;  //radio_station_count;
        pre_radio_station = cur_radio_station;
        clearInfo();
        radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
        //вызывает ReplayStation() из процесса Radio
        replay_station_flag = true;
        update_flag = true;
      } else {
        cur_message = F("Некорректный адрес станции.");
        warrning_flag = true;
      }
    }
    if (ui.click("del")) {
      if (ui.getBool() == true) {
        DelStation();
        !favorites_flag;
        DelStation();
        !favorites_flag;
        radio_station_count--;
        cur_radio_station--;
        pre_radio_station = cur_radio_station;
        if (cur_radio_station > 0) {
          clearInfo();
          radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
          //вызывает ReplayStation() из процесса Radio
          replay_station_flag = true;
          update_flag = true;
        }
      }
    }
    if (ui.click("url_btn")) {
      radio_mode = -1;
      mute_flag = true;
      audio.connecttohost(edit_radio_uri.c_str());
      //audio.setVolume(volume + vk, 1);
    }
    if (ui.click("au_play_btn")) {
      SpeekText(au_text, lang_list[cur_lang]);
    }
    if (ui.click("au_text")) au_text = ui.getString();
    if (ui.click("lang_list")) cur_lang = ui.getInt();
    if (ui.click("time_zone")) {
      time_zone = ui.getInt();
      timeClient.setTimeOffset(time_zone);
      GetTimeDate();
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("radio_per_page")) {
      radio_per_page = ui.getInt();
      eeprom_timer = millis();
      eeprom_flag = true;
    }
    if (ui.click("get_html_btn")) {
      GetHTMLdocument("http://www.museum.ru/museum/1812/index.html");
    }
#ifndef BATTERY_OFF
    if (ui.click("voltage_mode")) {
      voltage_mode = !voltage_mode;
      update_flag = true;
      eeprom_timer = millis();
      eeprom_flag = true;
    }
#endif
  }

  if (ui.update()) {
    ui.updateString("m_type_title", m_type_title);
    if (ui.update("f_reload")) ui.updateInt("f_reload", reload_flag);
    if (ui.update("radio_mode")) ui.updateInt("radio_mode", radio_mode);
    if (ui.update("cur_volume")) ui.updateInt("cur_volume", volume);
    if (ui.update("cur_volumea")) ui.updateInt("cur_volumea", volumea);
    if (ui.update("alarm_mode")) ui.updateInt("alarm_mode", alarm_mode);
    if (ui.update("balance")) ui.updateInt("balance", balance);
    if (ui.update("low_freq")) ui.updateInt("low_freq", low_freq);
    if (ui.update("mid_freq")) ui.updateInt("mid_freq", mid_freq);
    if (ui.update("hi_freq")) ui.updateInt("hi_freq", hi_freq);
    if (ui.update("cur_radio_station")) ui.updateInt("cur_radio_station", cur_radio_station);
    if (ui.update("cur_type_title")) ui.updateString("cur_type_title", cur_type_title);
    if (ui.update("cur_location_title")) ui.updateString("cur_location_title", cur_location_title);
    if (ui.update("radio_station_count")) ui.updateInt("radio_station_count", radio_station_count);
    if (ui.update("station")) ui.updateString("station", station);
    if (ui.update("title")) ui.updateString("title", title);
    if (ui.update("mp3_info")) ui.updateString("mp3_info", mp3_info);
    if (ui.update("curl")) ui.updateString("curl", curl);
    if (ui.update("channels")) ui.updateString("channels", channels);
    if (ui.update("slow")) {
      String tmp = String(100 / (slow + 1));
      ui.updateString("slow", tmp);
    }
    if (ui.update("vk")) ui.updateInt("vk", vk);
    if (ui.update("sample_rate")) ui.updateString("sample_rate", sample_rate);
    if (ui.update("bits_per_sample")) ui.updateString("bits_per_sample", bits_per_sample);
    if (ui.update("bit_rate")) ui.updateString("bit_rate", bit_rate);
    if (ui.update("aac_header_format")) ui.answer(aac_header_format);
    if (ui.update("aac_codec")) ui.answer(aac_codec);
    if (ui.update("sys_font")) ui.answer(sys_font);
    if (ui.update("aru")) ui.answer(aru);
    if (ui.update("mono")) ui.answer(mono);
    if (ui.update("voice_menu")) ui.answer(voice_menu_flag);
    if (ui.update("voice_sw")) ui.answer(voice_sw_flag);
    if (ui.update("del_st_sw")) ui.answer(del_st_sw_flag);
    if (ui.update("del_slow_sw")) ui.answer(del_st_slow_flag);
    if (ui.update("skan")) ui.answer(skan);
    if (ui.update("autoscan")) ui.answer(autoscan);
    if (ui.update("auto_save")) ui.answer(auto_save);
    if (ui.update("themas")) ui.answer(themas);
    if (ui.update("rand_song")) ui.answer(rand_song);
    if (ui.update("save_conf")) ui.answer(1);
    if (warrning_flag && ui.update("warrning")) {
      warrning_flag = false;
      ui.answer(cur_message);
    }
    if (ui.update("music_count")) ui.updateInt("music_count", music_count);
    if (ui.update("cur_music_file")) ui.updateInt("cur_music_file", cur_music_file);
    if (ui.update("index_info")) ui.updateString("index_info", index_info);
    if (ui.update("cur_im")) ui.answer(cur_im);
    if (ui.update("max_im")) ui.answer(max_im);
    if (ui.update("VUlevel")) ui.answer(VUlevel);
    if (ui.update("del")) ui.answer(1);
    if (ui.update("preview_mode_flag")) ui.answer(preview_mode_flag);
#ifndef BATTERY_OFF
    if (ui.update("cur_volt")) ui.answer(cur_volt);
    if (ui.update("voltage_mode")) ui.answer(voltage_mode);
#endif
  }
}

//--- Получение точного времени и даты ---
void GetTimeDate() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];
  //Получение структуры времени
  struct tm* ptm = gmtime((time_t*)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  //String currentMonthName = months[currentMonth - 1];
  int currentYear = ptm->tm_year + 1900;
  rtc.setTime(currentSecond, currentMinute, currentHour, monthDay, currentMonth, currentYear);
  vTaskDelay(10);
}

bool SearchWiFi(String point) {
  String rs;
  bool result = false;
  if (sd_flag == true) {
    file = SD.open("/WiFi/wifi.ini", FILE_READ);
    if (file) {
      while (file.available()) {
        rs = file.readStringUntil('\n');
        rs.replace("\n", " ");
        rs.trim();
        int f_pos = rs.indexOf("|", 0);
        if (f_pos != -1) {
          if (rs.substring(0, f_pos) == point && point != lp.ssid) {
            wfa = rs.substring(0, f_pos);
            wfp = rs.substring(f_pos + 1, rs.length());
            result = true;
            break;
          }
        }
        esp_task_wdt_reset();
      }
      file.close();
    } else {
      Serial.print(F("SD-card: /WiFi/wifi.ini not found."));
    }
  }
  return result;
}

//Сохранение авторизации точки доступа в файл
void SaveWiFi(String point, String point1) {
  File tmp_file;
  String rs;
  bool not_write = false;
  if (sd_flag == true) {
    if (SD.exists("/WiFi/wifi.tmp")) SD.remove("/WiFi/wifi.tmp");
    tmp_file = SD.open("/WiFi/wifi.tmp", FILE_WRITE);
    file = SD.open("/WiFi/wifi.ini", FILE_READ);
    if (file) {
      int f_pos;
      while (file.available()) {
        String rs = file.readStringUntil('\n');
        rs.replace("\n", " ");
        rs.trim();
        Serial.println(rs);
        f_pos = rs.indexOf(point, 0);
        if (f_pos != -1) {
          //Serial.print(">>>>>>>>>>>>>>>>>>>>>>"); Serial.println(point1);
          not_write = true;
          tmp_file.println(point + "|" + point1);
        } else {
          f_pos = rs.indexOf("|", 0);
          if (f_pos != -1) tmp_file.println(rs);
        }
        esp_task_wdt_reset();
      }
      file.close();
    } else {
      Serial.print(F("SD-card: /WiFi/wifi.ini not found."));
    }
    if (not_write == false) tmp_file.println(point + "|" + point1);
    tmp_file.println(F("--------------eof--------------"));
    tmp_file.close();
    if (SD.exists("/WiFi/wifi.ini")) SD.remove("/WiFi/wifi.ini");
    SD.rename("/WiFi/wifi.tmp", "/WiFi/wifi.ini");
  }
}

char* XORENC(char* in, char* key) {
  // Использован код от Brad@pingturtle.com
  int insize = strlen(in);
  int keysize = strlen(key);
  for (int x = 0; x < insize; x++) {
    for (int i = 0; i < keysize; i++) {
      in[x] = (in[x] ^ key[i]) ^ (x * i);
    }
  }
  return in;
}

void SpeekText(String au_txt, String au_lang) {
  //audio.setVolume(0);
  //while(audio.getVolume() != 0) {
  //  vTaskDelay(10);
  //}
  int s_pos = 0;
  int str_len = au_txt.length() + 1;
  int i = 0;
  while (i <= str_len) {
    Serial.print(".");
    if ((i - s_pos) < 82) {
      if (au_txt[i] == '.' || au_txt[i] == '!' || au_txt[i] == '?' || au_txt[i] == ':' || au_txt[i] == ';' || au_txt[i] == ',' || au_txt[i] == '-') {
        String cur_text = au_txt.substring(s_pos, i + 1);
        cur_text.trim();
        s_pos = i + 1;
        int str_len1 = cur_text.length() + 1;
        if (str_len1 > 1) {
          Serial.println(cur_text);
          speek_end = false;
          audio.connecttospeech(cur_text.c_str(), au_lang.c_str());
          while (speek_end == false) {
            yield();
            vTaskDelay(10);
            esp_task_wdt_reset();
          }
        }
      } else {
        if (i == str_len) {
          String cur_text = au_txt.substring(s_pos, i + 1);
          cur_text.trim();
          s_pos = i + 1;
          int str_len1 = cur_text.length() + 1;
          if (str_len1 > 1) {
            Serial.println(cur_text);
            speek_end = false;
            audio.connecttospeech(cur_text.c_str(), au_lang.c_str());
            while (speek_end == false) {
              yield();
              vTaskDelay(10);
              esp_task_wdt_reset();
            }
          }
        }
      }
    } else {
      int j = s_pos + 82;
      while (j > 0) {
        Serial.print(".");
        if (au_txt[j] == ' ') {
          String cur_text = au_txt.substring(s_pos, j);
          cur_text.trim();
          s_pos = j;
          i = s_pos;
          int str_len1 = cur_text.length() + 1;
          if (str_len1 > 1) {
            Serial.println(cur_text);
            speek_end = false;
            audio.connecttospeech(cur_text.c_str(), au_lang.c_str());
            while (speek_end == false) {
              yield();
              vTaskDelay(10);
              esp_task_wdt_reset();
            }
            j = 0;
            break;
          }
        }
        j--;
      }
    }
    i++;
    esp_task_wdt_reset();
  }
  Serial.println("");
}

//====================
void GetHTMLdocument(char* host) {
  //const char* host = "fv.memorandum.ru";
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  //  "Подключение к "

  if (client.connect(host, 80)) {
    Serial.println(F("connected]"));
    //  " подключено"

    Serial.println(F("[Sending a request]"));
    //  "Отправка запроса"
    client.print(String("GET /") + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +
                 //  "Хост: "
                 "Connection: close\r\n" +
                 //  "Соединение: закрыто"
                 "\r\n");

    Serial.println(F("[Response:]"));
    //  "Ответ:"
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    client.stop();
    Serial.println(F("\n[Disconnected]"));
    //  "Отключено"
  } else {
    Serial.println(F("connection failed!]"));
    //  "подключиться не удалось!"
    client.stop();
  }
  delay(5000);
}
