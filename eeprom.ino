/*
Сохранение и загрузка настроек через EEPROM
*/

// Контроль обновления настроек в энергонезависимой памяти
void eepromTick() {
  if (auto_save == true) {
    if (eeprom_flag == true)
      if (millis() - eeprom_timer > (save_time * 60000)) {  // save_time минут после последнего нажатия с пульта (кнопки или web-интерфейса)
        eeprom_flag = false;
        eeprom_timer = millis();
        updateEEPROM();
      }
  }
}

// Считывание настроек из энергонезависимой памяти
void readEEPROM() {
  EEPROM.begin(512);
  vTaskDelay(100);
  if (EEPROM.read(INIT_ADDR) != INIT_KEY) {  // первый запуск
    EEPROM.write(INIT_ADDR, INIT_KEY);       // записали ключ
    int address = 0;
    EEPROM.put(address, total_hours);
    address += sizeof(total_hours);
    EEPROM.put(address, radio_mode);
    address += sizeof(radio_mode);
    EEPROM.put(address, cur_radio_station);
    address += sizeof(cur_radio_station);
    EEPROM.put(address, volume);
    address += sizeof(volume);
    EEPROM.put(address, low_freq);
    address += sizeof(low_freq);
    EEPROM.put(address, mid_freq);
    address += sizeof(mid_freq);
    EEPROM.put(address, hi_freq);
    address += sizeof(hi_freq);
    EEPROM.put(address, balance);
    address += sizeof(balance);
    EEPROM.put(address, fix_sys_font);
    address += sizeof(fix_sys_font);
    EEPROM.put(address, auto_save);
    address += sizeof(auto_save);
    EEPROM.put(address, themas);
    address += sizeof(themas);
    EEPROM.put(address, cur_radio_type);
    address += sizeof(cur_radio_type);
    EEPROM.put(address, cur_music_type);
    address += sizeof(cur_music_type);
    EEPROM.put(address, cur_music_file);
    address += sizeof(cur_music_file);
    EEPROM.put(address, rand_song);
    address += sizeof(rand_song);
    EEPROM.put(address, cur_radio_location);
    address += sizeof(cur_radio_location);
    EEPROM.put(address, favorites_flag);
    address += sizeof(favorites_flag);
    EEPROM.put(address, kurant_flag);
    address += sizeof(kurant_flag);
    EEPROM.put(address, cur_kurant);
    address += sizeof(cur_kurant);
    EEPROM.put(address, mono);
    address += sizeof(mono);
    EEPROM.put(address, time_zone);
    address += sizeof(time_zone);
    EEPROM.put(address, voice_menu_flag);
    address += sizeof(voice_menu_flag);
    EEPROM.put(address, aru);
    address += sizeof(aru);
    EEPROM.put(address, cur_palette);
    address += sizeof(cur_palette);
    EEPROM.put(address, updateFirmware);
    address += sizeof(updateFirmware);
    EEPROM.put(address, cur_led);
    address += sizeof(cur_led);
    EEPROM.put(address, led_time);
    address += sizeof(led_time);
    EEPROM.put(address, voltage_mode);
    address += sizeof(voltage_mode);
    EEPROM.put(address, radio_per_page);
    address += sizeof(radio_per_page);
    EEPROM.put(address, preview_mode_flag);
    address += sizeof(preview_mode_flag);
    EEPROM.put(address, voice_sw_flag);
    address += sizeof(voice_sw_flag);

    EEPROM.put(address, cur_radio_station1);
    address += sizeof(cur_radio_station1);
    EEPROM.put(address, cur_radio_station2);
    address += sizeof(cur_radio_station2);
    EEPROM.put(address, cur_radio_station3);
    address += sizeof(cur_radio_station3);
    EEPROM.put(address, cur_music_file1);
    address += sizeof(cur_music_file1);
    EEPROM.put(address, cur_music_file2);
    address += sizeof(cur_music_file2);
    EEPROM.put(address, cur_music_file3);
    address += sizeof(cur_music_file3);
    EEPROM.put(address, sleeping);
    address += sizeof(sleeping);
    EEPROM.put(address, fix_mode);
    address += sizeof(fix_mode);
    EEPROM.put(address, volumea);
    address += sizeof(volumea);
    EEPROM.put(address, del_st_sw_flag);
    address += sizeof(del_st_sw_flag);
    EEPROM.put(address, del_st_slow_flag);
    address += sizeof(del_st_slow_flag);
    EEPROM.put(address, autoscan);
    address += sizeof(autoscan);

    for (int i = 0; i < MAX_ALARMS; i++) {
      EEPROM.put(address, sHour[i]);
      address += sizeof(sHour[i]);
      EEPROM.put(address, sMinute[i]);
      address += sizeof(sMinute[i]);
      EEPROM.put(address, wHour[i]);
      address += sizeof(wHour[i]);
      EEPROM.put(address, wMinute[i]);
      address += sizeof(wMinute[i]);
      EEPROM.put(address, sleep_flag[i]);
      address += sizeof(sleep_flag[i]);
      EEPROM.put(address, waikup_flag[i]);
      address += sizeof(waikup_flag[i]);
      EEPROM.put(address, alarm_mode[i]);
      address += sizeof(alarm_mode[i]);
      for (int d = 0; d < 8; d++) {
        EEPROM.put(address, waikup_day[i][d]);
        address += sizeof(waikup_day[i][d]);
        EEPROM.put(address, sleep_day[i][d]);
        address += sizeof(sleep_day[i][d]);
      }
    }
    //-----------------------
    EEPROM.put(address, lp);
    EEPROM.commit();
  }
  int address = 0;
  EEPROM.get(address, total_hours);
  address += sizeof(total_hours);
  EEPROM.get(address, radio_mode);
  address += sizeof(radio_mode);
  EEPROM.get(address, cur_radio_station);
  address += sizeof(cur_radio_station);
  EEPROM.get(address, volume);
  address += sizeof(volume);
  EEPROM.get(address, low_freq);
  address += sizeof(low_freq);
  EEPROM.get(address, mid_freq);
  address += sizeof(mid_freq);
  EEPROM.get(address, hi_freq);
  address += sizeof(hi_freq);
  EEPROM.get(address, balance);
  address += sizeof(balance);
  EEPROM.get(address, fix_sys_font);
  address += sizeof(fix_sys_font);
  EEPROM.get(address, auto_save);
  address += sizeof(auto_save);
  EEPROM.get(address, themas);
  address += sizeof(themas);
  EEPROM.get(address, cur_radio_type);
  address += sizeof(cur_radio_type);
  EEPROM.get(address, cur_music_type);
  address += sizeof(cur_music_type);
  EEPROM.get(address, cur_music_file);
  address += sizeof(cur_music_file);
  EEPROM.get(address, rand_song);
  address += sizeof(rand_song);
  EEPROM.get(address, cur_radio_location);
  address += sizeof(cur_radio_location);
  EEPROM.get(address, favorites_flag);
  address += sizeof(favorites_flag);
  EEPROM.get(address, kurant_flag);
  address += sizeof(kurant_flag);
  EEPROM.get(address, cur_kurant);
  address += sizeof(cur_kurant);
  EEPROM.get(address, mono);
  address += sizeof(mono);
  EEPROM.get(address, time_zone);
  address += sizeof(time_zone);
  EEPROM.get(address, voice_menu_flag);
  address += sizeof(voice_menu_flag);
  EEPROM.get(address, aru);
  address += sizeof(aru);
  EEPROM.get(address, cur_palette);
  address += sizeof(cur_palette);
  EEPROM.get(address, updateFirmware);
  address += sizeof(updateFirmware);
  EEPROM.get(address, cur_led);
  address += sizeof(cur_led);
  EEPROM.get(address, led_time);
  address += sizeof(led_time);
  EEPROM.get(address, voltage_mode);
  address += sizeof(voltage_mode);
  EEPROM.get(address, radio_per_page);
  address += sizeof(radio_per_page);
  EEPROM.get(address, preview_mode_flag);
  address += sizeof(preview_mode_flag);
  EEPROM.get(address, voice_sw_flag);
  address += sizeof(voice_sw_flag);

  EEPROM.get(address, cur_radio_station1);
  address += sizeof(cur_radio_station1);
  EEPROM.get(address, cur_radio_station2);
  address += sizeof(cur_radio_station2);
  EEPROM.get(address, cur_radio_station3);
  address += sizeof(cur_radio_station3);
  EEPROM.get(address, cur_music_file1);
  address += sizeof(cur_music_file1);
  EEPROM.get(address, cur_music_file2);
  address += sizeof(cur_music_file2);
  EEPROM.get(address, cur_music_file3);
  address += sizeof(cur_music_file3);
  EEPROM.get(address, sleeping);
  address += sizeof(sleeping);
  EEPROM.get(address, fix_mode);
  address += sizeof(fix_mode);
  EEPROM.get(address, volumea);
  address += sizeof(volumea);
  EEPROM.get(address, del_st_sw_flag);
  address += sizeof(del_st_sw_flag);
  EEPROM.get(address, del_st_slow_flag);
  address += sizeof(del_st_slow_flag);
  EEPROM.get(address, autoscan);
  address += sizeof(autoscan);

  for (int i = 0; i < MAX_ALARMS; i++) {
    EEPROM.get(address, sHour[i]);
    address += sizeof(sHour[i]);
    EEPROM.get(address, sMinute[i]);
    address += sizeof(sMinute[i]);
    EEPROM.get(address, wHour[i]);
    address += sizeof(wHour[i]);
    EEPROM.get(address, wMinute[i]);
    address += sizeof(wMinute[i]);
    EEPROM.get(address, sleep_flag[i]);
    address += sizeof(sleep_flag[i]);
    EEPROM.get(address, waikup_flag[i]);
    address += sizeof(waikup_flag[i]);
    EEPROM.get(address, alarm_mode[i]);
    address += sizeof(alarm_mode[i]);
    for (int d = 0; d < 8; d++) {
      EEPROM.get(address, waikup_day[i][d]);
      address += sizeof(waikup_day[i][d]);
      EEPROM.get(address, sleep_day[i][d]);
      address += sizeof(sleep_day[i][d]);
    }
    sleep_time[i] = GPtime(sHour[i], sMinute[i], 0);
    waikup_time[i] = GPtime(wHour[i], wMinute[i], 0);
  }
  //-----------------------
  EEPROM.get(address, lp);
  EEPROM.end();
  Serial.println(F("Настройки загружены из EEPROM."));
}

// Сохранение настроек в энергонезависимую память
void updateEEPROM() {
  int eget;
  EEPROM.begin(512);
  vTaskDelay(100);
  int address = 0;
  address += sizeof(total_hours);
  EEPROM.get(address, eget);
  if (eget != radio_mode) EEPROM.put(address, radio_mode);
  address += sizeof(radio_mode);
  EEPROM.get(address, eget);
  if (eget != cur_radio_station) EEPROM.put(address, cur_radio_station);
  address += sizeof(cur_radio_station);
  EEPROM.get(address, eget);
  if (eget != volume) EEPROM.put(address, volume);
  address += sizeof(volume);
  EEPROM.get(address, eget);
  if (eget != low_freq) EEPROM.put(address, low_freq);
  address += sizeof(low_freq);
  EEPROM.get(address, eget);
  if (eget != mid_freq) EEPROM.put(address, mid_freq);
  address += sizeof(mid_freq);
  EEPROM.get(address, eget);
  if (eget != hi_freq) EEPROM.put(address, hi_freq);
  address += sizeof(hi_freq);
  EEPROM.get(address, eget);
  if (eget != balance) EEPROM.put(address, balance);
  address += sizeof(balance);
  EEPROM.get(address, eget);
  if (eget != fix_sys_font) EEPROM.put(address, fix_sys_font);
  address += sizeof(fix_sys_font);
  EEPROM.get(address, eget);
  if (eget != auto_save) EEPROM.put(address, auto_save);
  address += sizeof(auto_save);
  EEPROM.get(address, eget);
  if (eget != themas) EEPROM.put(address, themas);
  address += sizeof(themas);
  EEPROM.get(address, eget);
  if (eget != cur_radio_type) EEPROM.put(address, cur_radio_type);
  address += sizeof(cur_radio_type);
  EEPROM.get(address, eget);
  if (eget != cur_music_type) EEPROM.put(address, cur_music_type);
  address += sizeof(cur_music_type);
  EEPROM.get(address, eget);
  if (eget != cur_music_file) EEPROM.put(address, cur_music_file);
  address += sizeof(cur_music_file);
  EEPROM.get(address, eget);
  if (eget != rand_song) EEPROM.put(address, rand_song);
  address += sizeof(rand_song);
  EEPROM.get(address, eget);
  if (eget != cur_radio_location) EEPROM.put(address, cur_radio_location);
  address += sizeof(cur_radio_location);
  EEPROM.get(address, eget);
  if (eget != favorites_flag) EEPROM.put(address, favorites_flag);
  address += sizeof(favorites_flag);
  EEPROM.get(address, eget);
  if (eget != kurant_flag) EEPROM.put(address, kurant_flag);
  address += sizeof(kurant_flag);
  EEPROM.get(address, eget);
  if (eget != cur_kurant) EEPROM.put(address, cur_kurant);
  address += sizeof(cur_kurant);
  EEPROM.get(address, eget);
  if (eget != mono) EEPROM.put(address, mono);
  address += sizeof(mono);
  EEPROM.get(address, eget);
  if (eget != time_zone) EEPROM.put(address, time_zone);
  address += sizeof(time_zone);
  EEPROM.get(address, eget);
  if (eget != voice_menu_flag) EEPROM.put(address, voice_menu_flag);
  address += sizeof(voice_menu_flag);
  EEPROM.get(address, eget);
  if (eget != aru) EEPROM.put(address, aru);
  address += sizeof(aru);
  EEPROM.get(address, eget);
  if (eget != cur_palette) EEPROM.put(address, cur_palette);
  address += sizeof(cur_palette);
  EEPROM.get(address, eget);
  if (eget != updateFirmware) EEPROM.put(address, updateFirmware);
  address += sizeof(updateFirmware);
  EEPROM.get(address, eget);
  if (eget != cur_led) EEPROM.put(address, cur_led);
  address += sizeof(cur_led);
  EEPROM.get(address, eget);
  if (eget != led_time) EEPROM.put(address, led_time);
  address += sizeof(led_time);
  EEPROM.get(address, eget);
  if (eget != voltage_mode) EEPROM.put(address, voltage_mode);
  address += sizeof(voltage_mode);
  EEPROM.get(address, eget);
  if (eget != radio_per_page) EEPROM.put(address, radio_per_page);
  address += sizeof(radio_per_page);
  EEPROM.get(address, eget);
  if (eget != preview_mode_flag) EEPROM.put(address, preview_mode_flag);
  address += sizeof(preview_mode_flag);
  EEPROM.get(address, eget);
  if (eget != voice_sw_flag) EEPROM.put(address, voice_sw_flag);
  address += sizeof(voice_sw_flag);

  EEPROM.get(address, eget);
  if (eget != cur_radio_station1) EEPROM.put(address, cur_radio_station1);
  address += sizeof(cur_radio_station1);
  EEPROM.get(address, eget);
  if (eget != cur_radio_station2) EEPROM.put(address, cur_radio_station2);
  address += sizeof(cur_radio_station2);
  EEPROM.get(address, eget);
  if (eget != cur_radio_station3) EEPROM.put(address, cur_radio_station3);
  address += sizeof(cur_radio_station3);
  EEPROM.get(address, eget);
  if (eget != cur_music_file1) EEPROM.put(address, cur_music_file1);
  address += sizeof(cur_music_file1);
  EEPROM.get(address, eget);
  if (eget != cur_music_file2) EEPROM.put(address, cur_music_file2);
  address += sizeof(cur_music_file2);
  EEPROM.get(address, eget);
  if (eget != cur_music_file3) EEPROM.put(address, cur_music_file3);
  address += sizeof(cur_music_file3);
  EEPROM.get(address, eget);
  if (eget != sleeping) EEPROM.put(address, sleeping);
  address += sizeof(sleeping);
  EEPROM.get(address, eget);
  if (eget != fix_mode) EEPROM.put(address, fix_mode);
  address += sizeof(fix_mode);
  EEPROM.get(address, eget);
  if (eget != volumea) EEPROM.put(address, volumea);
  address += sizeof(volumea);
  EEPROM.get(address, eget);
  if (eget != del_st_sw_flag) EEPROM.put(address, del_st_sw_flag);
  address += sizeof(del_st_sw_flag);
  EEPROM.get(address, eget);
  if (eget != del_st_slow_flag) EEPROM.put(address, del_st_slow_flag);
  address += sizeof(del_st_slow_flag);
  EEPROM.get(address, eget);
  if (eget != autoscan) EEPROM.put(address, autoscan);
  address += sizeof(autoscan);

  for (int i = 0; i < MAX_ALARMS; i++) {
    EEPROM.get(address, eget);
    if (eget != sHour[i]) EEPROM.put(address, sHour[i]);
    address += sizeof(sHour[i]);
    EEPROM.get(address, eget);
    if (eget != sMinute[i]) EEPROM.put(address, sMinute[i]);
    address += sizeof(sMinute[i]);
    EEPROM.get(address, eget);
    if (eget != wHour[i]) EEPROM.put(address, wHour[i]);
    address += sizeof(wHour[i]);
    EEPROM.get(address, eget);
    if (eget != wMinute[i]) EEPROM.put(address, wMinute[i]);
    address += sizeof(wMinute[i]);
    EEPROM.get(address, eget);
    if (eget != sleep_flag[i]) EEPROM.put(address, sleep_flag[i]);
    address += sizeof(sleep_flag[i]);
    EEPROM.get(address, eget);
    if (eget != waikup_flag[i]) EEPROM.put(address, waikup_flag[i]);
    address += sizeof(waikup_flag[i]);
    EEPROM.get(address, eget);
    if (eget != alarm_mode[i]) EEPROM.put(address, alarm_mode[i]);
    address += sizeof(alarm_mode[i]);
    for (int d = 0; d < 8; d++) {
      EEPROM.get(address, eget);
      if (eget != waikup_day[i][d]) EEPROM.put(address, waikup_day[i][d]);
      address += sizeof(waikup_day[i][d]);
      EEPROM.get(address, eget);
      if (eget != sleep_day[i][d]) EEPROM.put(address, sleep_day[i][d]);
      address += sizeof(sleep_day[i][d]);
    }
  }
  //---------------------------------
  EEPROM.get(address, lp_save);
  if (lp_save.ssid != lp.ssid || lp_save.pass != lp.pass) EEPROM.put(address, lp);

  EEPROM.commit();
  EEPROM.end();
  Serial.println(F("Настройки сохранены в EEPROM."));
}