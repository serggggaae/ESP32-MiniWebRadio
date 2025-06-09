#ifndef BATTERY_OFF
void Battery() {
  if (cur_reads < 10) return;
  //  Коррекция результата и получение напряжения батареи в вольтах
  Volt = (mVolt + 0.0028 * mVolt * mVolt + 0.0096 * mVolt - 0.051) / (ADC_R2 / (ADC_R1 + ADC_R2)) + DELTA;
  tft.setTextWrap(false);
  tft.setFont();
  tft.setTextSize(1);
  if (radio_mode == 0) {
    tft.setTextColor(radio_color[5]);
    tft.fillRect(114, 1, 46, 8, radio_color[1]);
  } else if (radio_mode == 2) {
    tft.setTextColor(player_color[5]);
    tft.fillRect(114, 10, 46, 8, player_color[1]);
  }
  if (Charging) {
    tft.setTextColor(sys_color[3]);
  }
  if (voltage_mode == false) {
    cur_volt = "&#128267;" + String(Volt) + "в";
    if (radio_mode == 0) {
      tft.setCursor(114, 1);
    } else if (radio_mode == 2) {
      tft.setCursor(114, 10);
    }
    tft.print(String(Volt) + "v");
  } else {
    int tpc = map((Volt * 100), 240, 424, 0, 100);
    if (tpc < 0) tpc = 0;
    if (tpc > 100) tpc = 100;
    cur_volt = "&#128267;" + String(tpc) + "%";
    if (radio_mode == 0) {
      tft.setCursor(122, 1);
    } else if (radio_mode == 2) {
      tft.setCursor(122, 10);
    }
    tft.print(String(tpc) + "%");
  }
  if (millis() - batt3_timer > 5000) {
    batt3_timer = millis();
    if (Volt3 > 0) { Volt = (Volt + Volt1 + Volt2 + Volt3) / 4; }
    Volt4 += Volt;
    Volt3 = Volt2;
    Volt2 = Volt1;
    Volt1 = Volt;
    t++;
  }

  if (millis() - batt2_timer > 50000) {
    batt2_timer = millis();
    t -= 1;
    Volt4 = Volt4 / t;
    if ((Volt5 > 0) && ((Volt4 - Volt5) > 0.001)) {
      Charging = true;
    } else {
      Charging = false;
    }
    Volt5 = Volt4;
    Volt4 = 0;
    t = 1;
  }

  if (radio_mode == 0) {
    tft.setCursor(141, 1);
  } else if (radio_mode == 2) {
    tft.setCursor(141, 10);
  }
  tft.setFont(&mMediaSymbols);
  tft.setTextSize(1);
  if (Charging) {
    if (g == 1) ChargeLevel = 0;
    else if (g == 2) ChargeLevel = 1;
    else if (g == 3) ChargeLevel = 2;
    else if (g == 4) ChargeLevel = 3;
    else {
      ChargeLevel = 4;
      g = 0;
    }
    g++;
  } else {
    if (Volt >= 4.1)
      ChargeLevel = 4;
    else if (Volt >= 3.7)
      ChargeLevel = 3;
    else if (Volt >= 3.3)
      ChargeLevel = 2;
    else if (Volt >= 2.9)
      ChargeLevel = 1;
    else if (Volt >= 2.7)
      ChargeLevel = 0;
    else {
      if (e == 1) {
        ChargeLevel = 4;
        tft.setTextColor(sys_color[1]);
      } else {
        ChargeLevel = 0;
        e = 0;
        tft.setTextColor(sys_color[4]);
      }
      e++;
    }
  }
  tft.print(String(ChargeLevel));
  battery_timer = millis();
}

void checkBattery() {
  tempmVolt += esp_adc_cal_raw_to_voltage(adc1_get_raw(USER_ADC_CHAN), &adc1_chars);
  cur_reads++;
  mVolt = (tempmVolt / cur_reads) / 1000;  //  Получаем средний результат в вольтах
  if (cur_reads > reads) {
    tempmVolt = 0;
    cur_reads = 0;
  }
}
#endif