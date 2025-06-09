/*
Дисплей
*/

// обновление дисплея
void update_display_info() {
  if (preview_flag == true) return;  // при прокручивании списка станций обновлять экран текущей трансляции не надо
  update_flag = false;
  display_ready = false;
  oldVUlevel = VUlevel = 0;
  info_timer1 = millis();
  int16_t tx, ty;
  uint16_t tw, th;
  int16_t fix_x, fix_y;
  switch (radio_mode) {
    //Режим "РАДИО"
    case 0:
      {
        info_flag0 = false;
        tft.fillScreen(radio_color[0]);
        tft.fillRect(0, 0, 160, 18, radio_color[1]);
        tft.setTextWrap(false);
        tft.setFont(&mMediaSymbols);
        tft.setTextSize(1);
        tft.setCursor(0, 15);
        tft.setTextColor(radio_color[2]);
        // Пиктограмма радио
        tft.print("9");

        // Тип радиостанции
        tft.setFont();
        tft.setCursor(24, 1);
        tft.setTextColor(radio_color[3]);
        if (sys_font == false) {
          sys_font = true;
          tft.println(utf8rus(radio_type_title[cur_radio_type]));
          sys_font = false;
        } else {
          tft.println(utf8rus(radio_type_title[cur_radio_type]));
        }

        //Страна/континент
        tft.setCursor(24, 9);
        tft.setTextColor(radio_color[4]);
        if (sys_font == false) {
          sys_font = true;
          tft.print(utf8rus(radio_location_title[cur_radio_location]));
          sys_font = false;
        } else {
          tft.print(utf8rus(radio_location_title[cur_radio_location]));
        }

        // Название радиостанции
        if (station == "") station = "* * *";
        tft.setTextWrap(false);
        tft.setTextColor(radio_color[5]);
        fix_x = 0;
        fix_y = 36;
        station.trim();
        int len_t = station.length();
        if (len_t > 0) {
          if (sys_font == false) {
            tft.setFont(&mMediaBold10pt8b);
            tft.setTextSize(1);
            tft.getTextBounds(station, 0, 36, &tx, &ty, &tw, &th);
            if (tw > w) {
              tft.setFont(&mMediaBold9pt8b);
              tft.getTextBounds(station, 0, 36, &tx, &ty, &tw, &th);
              if (tw > w) {
                tft.setFont(&mMediaBold8pt8b);
                tft.getTextBounds(station, 0, 36, &tx, &ty, &tw, &th);
                if (tw > w) {
                  tft.setFont(&mMediaBold7pt8b);
                  if (tw > w) {
                    tft.setTextWrap(true);
                    tft.getTextBounds(station, 0, 36, &tx, &ty, &tw, &th);
                    tw = w;
                  }
                }
              }
            }
            fix_x = tx;
            fix_y = ty + th + 14;
            tft.setCursor((w - tw) / 2, 36);
          } else {
            tft.setTextSize(2);
            tft.setCursor((w - (len_t * 12)) / 2, 22);
            if (len_t < 9) {
              tft.setTextSize(3);
              tft.setCursor((w - (len_t * 18)) / 2, 22);
            }
            if (len_t > 13) {
              tft.setTextSize(1);
              tft.setCursor((w - (len_t * 6)) / 2, 22);
              if (len_t > 29) {
                tft.setTextWrap(true);
                tft.setCursor(0, 22);
              }
            }
            tft.getTextBounds(station, 0, 22, &tx, &ty, &tw, &th);
            fix_y = ty + th + 4;
          }
          tft.println(utf8rus(station));
        }

        // Композиция
        title.trim();
        if (title.length() > 0) {
          tft.setTextWrap(false);
          tft.setTextColor(radio_color[6]);
          tft.setTextSize(1);
          if (sys_font == false) {
            tft.setFont(&mMedia7pt8b);
            tft.getTextBounds(title, fix_x, fix_y, &tx, &ty, &tw, &th);
            if (tw > w) {
              tft.setFont(&mMedia6pt8b);
              tft.setTextWrap(true);
              tft.getTextBounds(title, fix_x, fix_y, &tx, &ty, &tw, &th);
              tw = w;
            } else {
              tft.getTextBounds(title, fix_x, fix_y, &tx, &ty, &tw, &th);
            }
            tft.setCursor((w - tw) / 2, fix_y);
            fix_y = ty + th + 4;
          } else {
            tft.setFont();
            tft.setTextWrap(true);
            tft.getTextBounds(title, fix_x, fix_y, &tx, &ty, &tw, &th);
            if (tw < (w - 2)) {
              tft.setCursor((w - tw) / 2, ty);
            }
            fix_y = ty + th + 2;
          }
          tft.print(utf8rus(title));
        }

        // URL
        tft.setFont();
        tft.setTextWrap(true);
        tft.setTextColor(radio_color[7]);
        tft.setTextSize(1);
        tft.setCursor(0, fix_y);
        tft.print(curl);

        tft.fillRect(0, 110, 160, 19, radio_color[1]);
        tft.setTextWrap(false);

        //Channels
        tft.setCursor(2, 111);
        tft.setTextColor(radio_color[2]);
        tft.print(F("CH:"));
        tft.setTextColor(radio_color[4]);
        tft.print(channels);

        //Station/Count of station
        tft.setCursor(33, 111);
        tft.setTextColor(radio_color[2]);
        tft.print("ST:");
        tft.setTextColor(radio_color[4]);
        tft.print(cur_radio_station);
        tft.setTextColor(radio_color[2]);
        tft.print("/");
        tft.setTextColor(radio_color[4]);
        tft.print(radio_station_count);

        //Volume
        if (preview_flag == false) {
          tft.setCursor(124, 111);
          tft.setTextColor(radio_color[2]);
          tft.print(F("Vol:"));
          tft.setTextColor(radio_color[4]);
          tft.print(volume);
        }

        // SampleRate
        tft.setCursor(2, 120);
        tft.setTextColor(radio_color[2]);
        tft.print(F("SR:"));
        tft.setTextColor(radio_color[3]);
        tft.print((sample_rate.toInt() / 1000));
        tft.print("K");

        // BitsPerSample
        tft.setTextColor(radio_color[2]);
        tft.print(F(" BPS:"));
        tft.setTextColor(radio_color[3]);
        tft.print(bits_per_sample);

        // BitRate
        tft.setTextColor(radio_color[2]);
        tft.print(F(" BR:"));
        tft.setTextColor(radio_color[3]);
        tft.print((bit_rate.toInt() / 1000));
        tft.print("K ");

        // Избранное
        if (favorites_flag == true) {
          tft.setTextColor(radio_color[3]);
          tft.setCursor(143, 10);
          tft.setFont(&mMediaSymbols);
          tft.print(F(";"));
          tft.setFont();
        }

        // Блокировка управления
        if (block_flag == true) {
          tft.setTextColor(radio_color[3]);
          tft.setCursor(133, 10);
          tft.setFont(&mMediaSymbols);
          tft.print(F(":"));
          tft.setFont();
        }

        info_flag0 = true;
      }
      break;
    // Режим "ЧАСЫ"
    case 1:
      {
        if (internet_flag == true) {
          if (rtc.getTime("%H:%M") != old_time) {
            if (old_time == "") {
              tft.fillScreen(clock_color[0]);
              tft.drawRect(0, 9, w, h - 21, clock_color[1]);
              tft.drawLine(0, 34, w, 34, clock_color[1]);
              tft.drawLine(0, 64, w, 64, clock_color[1]);
              tft.drawLine(0, 92, w, 92, clock_color[1]);
            }
            tft.setTextWrap(false);
            //Год
            tft.setFont(&mMediaDigit10pt8b);
            tft.setTextSize(1);
            tft.getTextBounds(utf8rus(String(rtc.getYear())), 0, 18, &tx, &ty, &tw, &th);
            tft.setCursor((w - tw) / 2, 18);
            if (rtc.getDay() != old_day) {
              tft.fillRect(1, ty - th, w - 2, th + 5, clock_color[0]);
              tft.drawLine(0, 9, w, 9, clock_color[1]);
            }
            tft.setTextColor(clock_color[8]);
            tft.print(utf8rus(String(rtc.getYear())));
            //Индикация настроек часов
            tft.setFont(&mMediaSymbols);
            tft.setCursor(11, 17);
            tft.setTextColor(clock_color[2]);
            if (kurant_flag == true) tft.setTextColor(clock_color[3]);
            tft.print(" ");
            tft.setCursor(115, 17);
            tft.setTextColor(clock_color[2]);
            for (int i = 0; i < MAX_ALARMS; i++) {
              if (sleep_flag[i] == true) tft.setTextColor(clock_color[3]);
            }
            tft.print(">");
            tft.setTextColor(clock_color[2]);
            for (int i = 0; i < MAX_ALARMS; i++) {
              if (waikup_flag[i] == true) tft.setTextColor(clock_color[3]);
            }
            tft.print("!");
            //Число, месяц
            tft.setFont(&mMediaDigit10pt8b);
            String cur_day = utf8rus(String(rtc.getDay()) + " " + String(months[rtc.getMonth()]));
            tft.setTextColor(clock_color[4]);
            tft.setTextSize(1);
            tft.getTextBounds(cur_day, 0, 43, &tx, &ty, &tw, &th);
            tft.setCursor((w - tw) / 2, 43);
            if (rtc.getDay() != old_day) {
              tft.fillRect(1, ty, w - 2, th, clock_color[0]);
              tft.drawLine(0, 34, w, 34, clock_color[1]);
            }
            tft.print(cur_day);
            //Время
            tft.setFont(&mMediaDigital32pt8b);
            tft.setTextSize(1);
            tft.getTextBounds("88:88", 0, ty + th + 90, &tx, &ty, &tw, &th);
            tft.setCursor((w - tw) / 2, ty);
            tft.setTextColor(clock_color[5]);
            tft.print("88:88");
            tft.setTextColor(clock_color[6]);
            tft.setCursor((w - tw) / 2, ty);
            tft.print(rtc.getTime("%H:%M"));
            tft.setFont(&mMediaDigitalC32pt8b);
            tft.setTextColor(clock_color[7]);
            tft.setCursor((w - tw) / 2, ty);
            tft.print(rtc.getTime("%H:%M"));
            old_time = rtc.getTime("%H:%M");
            //День недели
            tft.setFont(&mMediaDigit10pt8b);
            tft.setTextSize(1);
            tft.setTextColor(clock_color[4]);
            if (rtc.getDayofWeek() == 0 || rtc.getDayofWeek() == 6) tft.setTextColor(clock_color[7]);
            tft.getTextBounds(utf8rus(String(weekDays[rtc.getDayofWeek()])), 0, ty + th + 6, &tx, &ty, &tw, &th);
            tft.setCursor((w - tw) / 2, ty);
            if (rtc.getDay() != old_day) {
              tft.fillRect(1, ty - th + 4, w - 2, th + 1, clock_color[0]);
              tft.drawLine(0, h - 13, w, h - 13, clock_color[1]);
              old_day = rtc.getDay();
            }
            tft.print(utf8rus(String(weekDays[rtc.getDayofWeek()])));
          }
          update_timer = millis();
          update_flag = true;
          return;
        }
      }
      break;

    //Режим "ПРОИГРЫВАТЕЛЬ"
    case 2:
      {
        info_flag = false;
        tft.fillScreen(player_color[0]);
        tft.fillRect(0, 0, 160, 18, player_color[1]);
        tft.setTextWrap(false);

        // Пиктограмма проигрывателя
        tft.setFont(&mMediaSymbols);
        tft.setTextSize(1);
        tft.setCursor(0, 17);
        tft.setTextColor(player_color[2]);
        tft.print("'");

        // Тип музыки (жанр)
        tft.setFont();
        tft.setCursor(24, 1);
        tft.setTextColor(player_color[3]);
        if (sys_font == false) {
          sys_font = true;
          tft.println(utf8rus(music_type_title[cur_music_type]));
          sys_font = false;
        } else {
          tft.println(utf8rus(music_type_title[cur_music_type]));
        }

        // Название композиции
        station.trim();
        if (station == "") station = "* * *";
        tft.setTextWrap(false);
        tft.setTextColor(player_color[5]);
        fix_x = 0;
        fix_y = 36;
        station.trim();
        int len_t = station.length();
        if (len_t > 0) {
          if (sys_font == false) {
            tft.setFont(&mMediaBold10pt8b);
            tft.setTextSize(1);
            tft.getTextBounds(utf8rus(station), 0, 36, &tx, &ty, &tw, &th);
            if (tw > w || th > 48) {
              tft.setFont(&mMediaBold9pt8b);
              tft.getTextBounds(utf8rus(station), 0, 36, &tx, &ty, &tw, &th);
              if (tw > w || th > 48) {
                tft.setFont(&mMediaBold8pt8b);
                tft.getTextBounds(utf8rus(station), 0, 36, &tx, &ty, &tw, &th);
                if (tw > w || th > 48) {
                  tft.setFont(&mMediaBold7pt8b);
                  if (tw > w) {
                    tft.setTextWrap(true);
                    tft.getTextBounds(utf8rus(station), 0, 36, &tx, &ty, &tw, &th);
                    if (th > 48) {
                      tft.setFont(&mMedia7pt8b);
                      tft.getTextBounds(utf8rus(station), 0, 36, &tx, &ty, &tw, &th);
                      if (th > 48) {
                        tft.setFont(&mMedia6pt8b);
                        tft.getTextBounds(utf8rus(station), 0, 36, &tx, &ty, &tw, &th);
                      }
                    }
                    tw = w;
                  }
                }
              }
            }
            fix_x = tx;
            fix_y = ty + th + 14;
            tft.setCursor((w - tw) / 2, 36);
          } else {
            tft.setTextWrap(false);
            tft.setFont();
            tft.setTextSize(3);
            tft.getTextBounds(utf8rus(station), 0, 22, &tx, &ty, &tw, &th);
            if (tw / 2 > w) {
              tft.setTextSize(2);
              tft.getTextBounds(utf8rus(station), 0, 22, &tx, &ty, &tw, &th);
              if (tw / 2 > w) {
                tft.setTextSize(1);
                tft.getTextBounds(utf8rus(station), 0, 22, &tx, &ty, &tw, &th);
                if (tw / 2 > w) {
                  tft.setTextWrap(true);
                  tft.getTextBounds(utf8rus(station), 0, 22, &tx, &ty, &tw, &th);
                  tw = w;
                }
              }
            }
            fix_x = tx;
            fix_y = ty + th + 4;
            tft.setCursor((w - tw) / 2, 22);
          }
          tft.println(utf8rus(station));
        }

        // Исполнитель
        title.trim();
        if (title.length() > 0) {
          tft.setTextWrap(false);
          tft.setTextColor(player_color[6]);
          tft.setTextSize(1);
          if (sys_font == false) {
            tft.setFont(&mMedia7pt8b);
            tft.getTextBounds(utf8rus(title), fix_x, fix_y, &tx, &ty, &tw, &th);
            if (tw > w) {
              tft.setFont(&mMedia6pt8b);
              tft.setTextWrap(true);
              tft.getTextBounds(utf8rus(title), fix_x, fix_y, &tx, &ty, &tw, &th);
              tw = w;
            } else {
              tft.getTextBounds(utf8rus(title), fix_x, fix_y, &tx, &ty, &tw, &th);
            }
            tft.setCursor((w - tw) / 2, fix_y);
            tft.println(utf8rus(title));
            fix_y = ty + th + 12;
          } else {
            tft.setFont();
            tft.setTextWrap(false);
            tft.getTextBounds(utf8rus(title), fix_x, fix_y, &tx, &ty, &tw, &th);
            if (tw / 2 < (w - 2)) {
              tft.setCursor((w - tw) / 2, ty);
            }
            tft.setTextWrap(true);
            tft.getTextBounds(utf8rus(title), fix_x, fix_y, &tx, &ty, &tw, &th);
            tft.println(utf8rus(title));
            fix_y = ty + th + 4;
          }
        }

        //Дополнительная информация
        tft.setCursor(0, fix_y);
        mp3_info = "";
        if (sys_font == false) {
          tft.setFont(&mMedia6pt8b);
          tft.setTextSize(1);
        } else {
          tft.setFont();
        }
        tft.setTextWrap(true);
        tft.setTextSize(1);
        if (album != "-") {
          tft.setTextColor(player_color[7]);
          tft.print(utf8rus("Альбом: "));
          tft.setTextColor(player_color[8]);
          tft.println(utf8rus(album));
          mp3_info += "Альбом: " + album + "<BR>";
        }
        if (year != "-") {
          tft.setTextColor(player_color[7]);
          tft.print(utf8rus("Год: "));
          tft.setTextColor(player_color[8]);
          tft.println(utf8rus(year));
          mp3_info += "Год: " + year + "<BR>";
        }
        if (comment != "-") {
          tft.setTextColor(player_color[7]);
          tft.print(utf8rus("Доп.: "));
          tft.setTextColor(player_color[8]);
          tft.println(utf8rus(comment));
          mp3_info += "Доп.: " + comment + "<BR>";
        }
        tft.setTextColor(player_color[7]);
        tft.println(utf8rus(curl));

        if (mp3_info != "") mp3_info = "<BR>" + mp3_info;

        tft.setFont();
        tft.fillRect(0, 110, 160, 19, player_color[1]);
        tft.setCursor(2, 111);
        tft.setTextWrap(false);
        tft.setTextSize(1);

        //Каналы
        tft.setCursor(2, 111);
        tft.setTextColor(player_color[2]);
        tft.print(F("CH:"));
        tft.setTextColor(player_color[4]);
        tft.print(channels);

        //Текущая композиция/Количество композиций
        tft.setCursor(33, 111);
        tft.setTextColor(player_color[2]);
        tft.print("FL:");
        tft.setTextColor(player_color[4]);
        tft.print(cur_music_file);
        tft.setTextColor(player_color[2]);
        tft.print("/");
        tft.setTextColor(player_color[4]);
        tft.print(music_count);

        //Громкость
        if (preview_flag == false) {
          tft.setCursor(124, 111);
          tft.setTextColor(player_color[2]);
          tft.print(F("Vol:"));
          tft.setTextColor(player_color[4]);
          tft.print(volume);
        }

        tft.setCursor(2, 120);

        // SampleRate
        tft.setTextColor(player_color[2]);
        tft.print(F("SR:"));
        tft.setTextColor(player_color[3]);
        tft.print((sample_rate.toInt() / 1000));
        tft.print("K ");

        // BitsPerSample
        tft.setTextColor(player_color[2]);
        tft.print(F(" BPS:"));
        tft.setTextColor(player_color[3]);
        tft.print(bits_per_sample);

        // BitRate
        tft.setTextColor(player_color[2]);
        tft.print(F(" BR:"));
        tft.setTextColor(player_color[3]);
        tft.print((bit_rate.toInt() / 1000));
        tft.print("K ");

        // Избранное
        if (favorites_flag1 == true) {
          tft.setTextColor(player_color[3]);
          tft.setCursor(143, 10);
          tft.setFont(&mMediaSymbols);
          tft.print(F(";"));
          tft.setFont();
        }

        // Блокировка управления
        if (block_flag == true) {
          tft.setTextColor(player_color[3]);
          tft.setCursor(133, 10);
          tft.setFont(&mMediaSymbols);
          tft.print(F(":"));
          tft.setFont();
        }
        info_flag = true;
      }
      break;
    default:
      break;
  }
  aru_change = true;
  display_ready = true;
}

//Функция работы с таблицей русскоязычных символов
String utf8rus(String source) {
  int i, k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };
  k = source.length();
  i = 0;

  if (sys_font != true) {
    while (i < k) {
      n = source[i];
      i++;
      if (n >= 0xC0) {
        switch (n) {
          case 0xD0:
            {
              n = source[i];
              i++;
              if (n == 0x81) {
                n = 0xC5;
                break;
              }
              if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
              break;
            }
          case 0xD1:
            {
              n = source[i];
              i++;
              if (n == 0x91) {
                n = 0xE5;
                break;
              }
              if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
              break;
            }
        }
      }
      m[0] = n;
      target = target + String(m);
    }
  } else {
    while (i < k) {
      n = source[i];
      i++;

      if (n >= 0xBF) {
        switch (n) {
          case 0xD0:
            {
              n = source[i];
              i++;
              if (n == 0x81) {
                n = 0xA8;
                break;
              }
              if (n >= 0x90 && n <= 0xBF) n = n + 0x2F;
              break;
            }
          case 0xD1:
            {
              n = source[i];
              i++;
              if (n == 0x91) {
                n = 0xB7;
                break;
              }
              if (n >= 0x80 && n <= 0x8F) n = n + 0x6F;
              break;
            }
        }
      }
      m[0] = n;
      target = target + String(m);
    }
  }
  return target;
}

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File bmpFile;
  int bmpWidth, bmpHeight;             // W+H in pixels
  uint8_t bmpDepth;                    // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;             // Start of image data in file
  uint32_t rowSize;                    // Not always = bmpWidth; may have padding
  uint8_t sdbuffer[3 * BUFFPIXEL];     // pixel buffer (R+G+B per pixel)
  uint8_t buffidx = sizeof(sdbuffer);  // Current position in sdbuffer
  boolean goodBmp = false;             // Set to true on valid header parse
  boolean flip = true;                 // BMP is stored bottom-to-top
  int w, h, row, col;
  uint8_t r, g, b;
  uint32_t pos = 0, startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename, FILE_READ)) == NULL) {
    Serial.println(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) {  // BMP signature
    Serial.print(F("File size: "));
    Serial.println(read32(bmpFile));
    (void)read32(bmpFile);             // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);  // Start of image data
    Serial.print(F("Image Offset: "));
    Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: "));
    Serial.println(read32(bmpFile));
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) {    // # planes -- must be '1'
      bmpDepth = read16(bmpFile);  // bits per pixel
      Serial.print(F("Bit Depth: "));
      Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) {  // 0 = uncompressed

        goodBmp = true;  // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width()) w = tft.width() - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.startWrite();
        tft.setAddrWindow(x, y, w, h);

        for (row = 0; row < h; row++) {  // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip)  // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else  // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) {  // Need seek?
            tft.endWrite();
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer);  // Force buffer reload
          }

          for (col = 0; col < w; col++) {  // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) {  // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0;  // Set index to beginning
              tft.startWrite();
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r, g, b));
          }  // end pixel
        }    // end scanline
        tft.endWrite();
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(F(" ms"));
      }  // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println(F("BMP format not recognized."));
}


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();  // LSB
  ((uint8_t *)&result)[1] = f.read();  // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();  // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();  // MSB
  return result;
}

void setPalettes(int p) {
  for (int i; i < 9; i++) {
    if (i < 8) sys_color[i] = sys_colors[p][i];
    if (i < 8) radio_color[i] = radio_colors[p][i];
    player_color[i] = player_colors[p][i];
    clock_color[i] = clock_colors[p][i];
  }
}

//функция перевода 24 битного RGB в 16 битный 565 цвет
uint16_t R_G_B(byte r, byte g, byte b) {
  uint16_t code = 0;
  code = (g & 0x1f) << 11;
  code |= (r & 0x3f) << 5;
  code |= (b & 0x1f) << 0;
  return code;
}