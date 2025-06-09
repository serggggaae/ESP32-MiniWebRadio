/*==================--------------- Интернет-радио microMedia на ESP32-S3 ---------------==================
                                              Версия: 2.46
Схемотехника и программирование: Олег Поляков (https://community.alexgyver.ru/members/oleg-poljakov.44456/)
Индикация заряда аккумулятора: Сергей aka sergggg (https://community.alexgyver.ru/members/sergggg.75421/)
Описание проекта, инструкция по сборке, исходные коды и дискуссия:
https://community.alexgyver.ru/threads/internet-radio-micromedia-na-esp32-s3.13089/
-----------------------------------------------------------------------------------------------------------
!!!ВАЖНО!!! Используйте в настройках платы следующую схему распределения памяти "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"

Версия 2.23 от 28.04.2025
1. Добавлена новая цветовая схема интерфейса "Калейдоскоп".
2. Сделано обновление прошивки с sd-карты. Для этого необходимо откомпилировать скетч с Partition Scheme "Minimal SPIFFS". Файл прошивки firmware.bin выложить в корень sd-карты.
3. Оптимизирована процедура индексации аудио-файлов в папках Проигрывателя.

Версия 2.40 от 07.05.2025
1. Улучшена стабильность работы.
2. Изменён формат хранения списков радиостанций в ini-файлах папки Radio: добавлено поле названия станции вещания, которое определяется и сохраняется автоматически для каждой станции.
   ini-файлы предыдущей версии совместимы с новой версией прошивки и будут автоматически преобразовываться в новый формат в процессе соединения с радиостанций для прослушивания.
3. Добавлена возможность постраничного просмотра списка интернет-станций в режиме "Радио" через web-интерфейс.
4. В раздел "Настройки" web-интерфейса добавлен блок настроек "Радио", где можно установить желаемое количество станций для постраничного просмотра. См. п.3.

Версия 2.41 от 08.05.2025
1. Исправлена коллизия с перезагрузкой ESP при нахождении в меню настроек. Спасибо участнику форума с ником VILUKAS (https://community.alexgyver.ru/members/vilukas.100909/)

Версия 2.42 от 09.05.2025
1. В web-интерфейс добавлен поиск станций интернет-вещания по общему списку и списку любимых станций.
2. Улучшена стабильность работы без перезагрузок ESP.

Версия 2.43 от 11.05.2025
1. Реализован альтернативный способ выбора станций интернет-вещания через энкодер: поворот энкодера - просмотр информации о следующей/предыдущей станции без перехода на неё; нажатие на кнопку энкодера - переход на выбранную станцию вещания; нажатие на кнопку - отмена выбора и возвращение в окно "Радио".
2. В раздел "Настройки / Воспроизведение" интернет-радио и в "Настройки - Радио" web-интерфейса добавлена возможность установить способ выбора станций: классический или альтернативный.

Версия 2.44 от 16.05.2025
1. Убраны мелкие недочёты с отображением информации в разных режимах.
2. Управление через кнопку и энкодер приведено к единому стилю.
3. В управлении через ИК-пульт реализовано управление в режиме предпросмотра станций интернет-вещания. 
За следующие улучшения спасибо участнику форума с ником sergggg (https://community.alexgyver.ru/members/sergggg.75421/)
4. Улучшен алгоритм плавного уменьшения/увеличения громкости при переключениях станций/треков.
5. Улучшен алгоритм записи, считывания данных из EEPROM.
6. Улучшен алгоритм обработки сигналов с ИК-пульта.

Версия 2.45 от 30.05.2025
1. Все надписи на дисплее и в мониторе порта приведены к литературному русскому языку.
2. Исправлена досадная ошибка, когда при сохранённом режиме Проигрыватель при включении запускалась сохранённая радио-трансляция.
3. Добавлен целый ряд поясняющих сообщений, транслируемых в серийный порт во время запуска и работы прибора.
4. Реализован альтернативный способ выбора треков Проигрывателя через энкодер и ИК-пульт: поворот энкодера - просмотр информации о следующем/предыдущем звуковом файле без его воспроизведения; нажатие на кнопку энкодера - запуск воспроизведения файла; нажатие на кнопку - отмена выбора и возвращение в окно "Проигрыватель".
5. Вызовы и отработка функции воспроизведения звуковых файлов PlayMusic и связанных с ней функций полностью перенесены на нулевое ядро, что позволило оптимизировать загрузку ядер процессора и высвободить память.
6. Меньше перезагрузок на "висящих" станциях интернет-вещания (но это не точно, надо тестировать, тестировать, тестировать!)

Версия 2.46 от 01.06.2025
1. Исправлена ситуация с перезагрузкой прибора во время боя курантов, выявленная участником форума VILUKAS (https://community.alexgyver.ru/members/vilukas.100909/).
2. Добавлен контроль wi-fi соединения и оповещение в случае его потери.
3. Исправлен ряд минорных погрешностей.
*/

//===== Версия ПО =============
int verMajor = 2;
int verMinor = 46;
#define FIRMWARE_VERSION 2.46
#include <Update.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <WiFi.h>
// Библиотека работы с аудио DAC: https://github.com/schreibfaul1/ESP32-audioI2S
#include <Audio.h>
#include <SD.h>
#include <FS.h>
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 48  // "Сторожевая собака" перезагрузит ESP-32 через 48 сек. после зависания
unsigned long timeout = 0;
bool timeout_flag = false;
bool audio_flag = false;

//Настройка работы с EEPROM
#include <EEPROM.h>
#define INIT_ADDR 500  // номер резервной ячейки
#define INIT_KEY 69    // ключ первого запуска. 0-254, на выбор

//Энкодер
#define CLK 41
#define DT 40
#define SW 39
#include <GyverEncoder.h>
Encoder enc1(CLK, DT, SW);

#define PIN 1  // кнопка подключена сюда (PIN --- КНОПКА --- GND)
#include <GyverButton.h>
GButton butt1(PIN);
//Кнопка Reset подключается к RST и GND

//ИК-пульт управления
#define PULT 1    // ИК-пульт: 0 - нет; 1 - есть
#define IR_PIN 2  // пин подключения ИК-датчика
#if (PULT == 1)
#define DECODE_NEC
#include <IRremote.h>  // Используйте версию библиотеки от 3.x до 3.5.
#endif
//int ir_protocol = 8;  //Тип пульта

// DAC I/O used
#define I2S_DOUT 38  //DIN
#define I2S_BCLK 21  //BSK
#define I2S_LRC 47   //LCK
//-----------------
Audio audio;

//SD-card reader
#define SD_CS 11
#define SPI_MOSI 12
#define SPI_MISO 13
#define SPI_SCK 14

//TFT-дисплей
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

//TFT-screen pin for ESP32
#define TFT_CS 3     //case select connect to pin 3
#define TFT_RST 8    //reset connect to pin 8
#define TFT_DC 18    //AO connect to pin 32  (not sure that this is really used)  try pin 18
#define TFT_MOSI 17  //Data = SDA connect to pin 17
#define TFT_SCLK 16  //Clock = SCK connect to pin 16
#define MAX_ALARMS 5
//Пин управления яркостью подсветки дисплея
#define TFT_LED 42
// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Он-лайн калькулятор для преобразование цветов из RGB в RGB565: https://trolsoft.ru/ru/articles/rgb565-color-picker
String palette_type[7] = { "темная", "светлая", "изумрудная", "черно-белая", "ретро", "рубин и янтарь", "калейдоскоп" };
int sys_color[8], radio_color[8], player_color[9], clock_color[9];
int sys_colors[][8] = {
  { 0x0000, 0xF800, 0xC618, 0x07E0, 0xFFE0, 0x3A4E, 0x07FF, 0x0809 },  //Палитра по-умолчанию (тёмная)
  { 0xFFFF, 0xF800, 0x7BEF, 0x3240, 0xA3A1, 0xA3A1, 0x3A4E, 0x0804 },  //Палитра светлая
  { 0x0000, 0xE960, 0x5C22, 0xFFFF, 0xFFFF, 0x7BEF, 0xAF25, 0x1981 },  //Палитра изумрудная
  { 0x0000, 0xFFFF, 0xC618, 0xFFFF, 0xFFFF, 0x7BEF, 0xC618, 0x39E7 },  //Палитра чёрно-белая
  { 0x0000, 0xFF57, 0xC54D, 0xF4C5, 0xFF57, 0xDD23, 0xC54D, 0x4120 },  //Палитра ретро
  { 0x2020, 0xF960, 0xFDC9, 0xFBC4, 0xF6C4, 0xDC23, 0xFDC9, 0x78C0 },  //Рубин и янтарь
  { 0x4A26, 0xFB63, 0xDEB0, 0xE703, 0xFC42, 0xFC80, 0xBD91, 0x3164 }   //Калейдоскоп 9DF0
  //фон меню красн  текст   яркий   жёл-бел меню    серый   фон основного окна
  //        фиаско          успех   ярк.инф
};
int radio_colors[][8] = {
  { 0x0000, 0x3240, 0xAFE5, 0xFFE0, 0xFFFF, 0xFD20, 0xC618, 0x7BEF },
  { 0xE7B7, 0x3240, 0xAFE5, 0xFFE0, 0xFFFF, 0xC380, 0x5580, 0x4A49 },
  { 0x0000, 0x3240, 0x5C22, 0x8D86, 0xb72b, 0xAF25, 0xAE10, 0x7C4C },
  { 0x0000, 0x3166, 0x5AAB, 0x738E, 0x8C71, 0xDEFB, 0xB5B6, 0x8C71 },
  { 0x1880, 0x4120, 0x7A40, 0xB3A0, 0xD4A3, 0xFE68, 0xDE4E, 0xD4A3 },
  { 0x2020, 0x4060, 0xAC67, 0xEBA0, 0xD4A3, 0xE980, 0xA100, 0x7265 },
  { 0x2163, 0x3AA4, 0xAF24, 0xDF2C, 0xCFED, 0xDF67, 0xA6C2, 0xA5EF }
  //фон     плашки  иконка  тип     локация титл    исполн  url
};
int player_colors[][9] = {
  { 0x0000, 0x3A4E, 0x07FF, 0xFFE0, 0xFFFF, 0xF81F, 0xFFFF, 0x7BEF, 0xC618 },
  { 0xFFFD, 0x3A4E, 0x07FF, 0xFFE0, 0xFFFF, 0xF81F, 0x31DF, 0x18E3, 0x4A49 },
  { 0x0000, 0x10A6, 0x124E, 0x33B4, 0x5D59, 0x763F, 0x2438, 0x122D, 0x1B11 },
  { 0x0000, 0x3166, 0x5AAB, 0x738E, 0x8C71, 0xDEFB, 0xB5B6, 0x6B4D, 0x8C71 },
  { 0x1880, 0x4120, 0x7A40, 0xB3A0, 0xD4A3, 0xFE68, 0xDE4E, 0x9B20, 0xD4A3 },
  { 0x2020, 0x4060, 0xAC67, 0xEBA0, 0xD4A3, 0xEC60, 0xBAA0, 0x7265, 0xA204 },
  { 0x0104, 0x0A09, 0x2C1A, 0xE70E, 0xBEBB, 0xFFFF, 0x8E1A, 0x640F, 0xD6DA }
  //фон     плашки  иконка  тип     счетчик титл    исполн  url     под. инфо
};
int clock_colors[][9] = {
  { 0x0000, 0x3081, 0x4102, 0xA3A1, 0x7425, 0x4102, 0xFB64, 0xF221, 0x3240 },
  { 0xD733, 0xB630, 0xB630, 0xA3A1, 0x7425, 0xB613, 0xEB60, 0xAA40, 0x3240 },
  { 0x41C3, 0x3162, 0x3162, 0xBFE0, 0x7D65, 0x3162, 0x5D64, 0xBFE0, 0x7D65 },
  { 0x0000, 0x2925, 0x31A6, 0x8C71, 0x7BCF, 0x39E7, 0xBDf7, 0xDEFB, 0x7BCF },
  { 0x3880, 0x40A0, 0x40A0, 0xB1C0, 0xB1C0, 0x60E0, 0xD302, 0xFBA3, 0xB1C0 },
  { 0x3880, 0x40A0, 0x40A0, 0xB1C0, 0xB1C0, 0x60E0, 0xD302, 0xFBA3, 0xB1C0 },
  { 0x3880, 0x40A0, 0x40A0, 0xB1C0, 0xB1C0, 0x60E0, 0xD302, 0xFBA3, 0xB1C0 }
  //фон  затемнения вык.ик  иконки  буквы   тёмн.ц. цифры   ярк.ц.  год
};
#define ST77XX_BLACK 0x0000        //   0,   0,   0  +
#define ST77XX_HARDGREY 0x2925     //                ?
#define ST77XX_MIDDLEGREY 0x4A49   //                ?
#define ST77XX_NAVY 0x000F         //   0,   0, 123  -
#define ST77XX_DARKGREEN 0x3240    //   0, 125,   0  +
#define ST77XX_DARKCYAN 0x3A4E     //   0, 125, 123  +
#define ST77XX_MAROON 0x7800       // 123,   0,   0  +
#define ST77XX_PURPLE 0x780F       // 123,   0, 123  -
#define ST77XX_OLIVE 0x7BE0        // 123, 125,   0  -
#define ST77XX_LIGHTGREY 0xC618    // 198, 195, 198  +
#define ST77XX_DARKGREY 0x7BEF     // 123, 125, 123  +
#define ST77XX_EDARKYELLOW 0x3942  //   7,  10,   2  -
#define ST77XX_BLUE 0x001F         //   0,   0, 255  ?
#define ST77XX_GREEN 0x07E0        //   0, 255,   0  +
#define ST77XX_MGREEN 0x7425       //   0, 255,   0  ? часы
#define ST77XX_CYAN 0x07FF         //   0, 255, 255  +
#define ST77XX_RED 0xF800          // 255,   0,   0  ? инициализация программы, связь
#define ST77XX_HRED 0xF221         //                ? блокировка экрана, часы,
#define ST77XX_ERED 0x4102         //
#define ST77XX_URED 0x3081         //
#define ST77XX_MAGENTA 0xF81F      // 255,   0, 255
#define ST77XX_YELLOW 0xFFE0       // 255, 255,   0
#define ST77XX_UYELLOW 0xA3A1
#define ST77XX_WHITE 0xFFFF        // 255, 255, 255
#define ST77XX_ORANGE 0xFD20       // 255, 165,   0
#define ST77XX_RORANGE 0xFB64      // 255, 165,   0
#define ST77XX_GREENYELLOW 0xAFE5  // 173, 255,  41
#define ST77XX_PINK 0xFC18         // 255, 130, 198
#define ST77XX_PAPER 0XDE32

//===== ШРИФТЫ ==============================================================================
// 1. В папку библиотек Arduino\libraries\Adafruit_GFX_Library перепишите файл glcdfont.c из архива скетча.
// 2. Туда же обязательно перепишите из архива скетча папку FontsRus, даже если не планируете использовать.
#include "FontsRus/mMedia6.h"
#include "FontsRus/mMedia7.h"
#include "FontsRus/mMediaBold7.h"
#include "FontsRus/mMediaBold8.h"
#include "FontsRus/mMediaBold9.h"
#include "FontsRus/mMediaBold10.h"
#include "FontsRus/mMediaDigit10.h"     // для отображения даты, месяца, года и дня недели
#include "FontsRus/mMediaDigital32.h"   // для отображения времени сегментными цифрами
#include "FontsRus/mMediaDigitalC32.h"  // для отображения времени сегментными цифрами
#include "FontsRus/mMediaSymbols.h"     // пиктограммы
bool sys_font = false;                  // (true - использовать системный руссифицированный шрифт; false - использовать другого начертания шрифт из набора)
bool fix_sys_font;

//Индикация заряда аккумулятора
//#define BATTERY_OFF // для выключения индикации раскомментируйте эту строку и пересоберите скетч
bool voltage_mode = false;  // false - индикация напряжения в вольтах, true - в процентах
#ifndef BATTERY_OFF
#include "driver/adc.h"   // Подключение необходимого драйвера
#include "esp_adc_cal.h"  // Подключение необходимой библиотеки
#define ADC_PIN 4
#define USER_ADC_CHAN ADC1_CHANNEL_3
#ifndef R1
#define R1 200  // Номинал резистора на плюс (+)
#endif
#ifndef R2
#define R2 100  // Номинал резистора на минус (-)
#endif
#ifndef DELTA_BAT
#define DELTA_BAT 0  // Величина коррекции напряжения батареи
#endif
float ADC_R1 = R1;        // Номинал резистора на плюс (+)
float ADC_R2 = R2;        // Номинал резистора на минус (-)
float DELTA = DELTA_BAT;  // Величина коррекции напряжения батареи
uint8_t reads = 100;      // Количество замеров в одном измерении
float Volt = 0;           // Напряжение на батарее
//uint8_t oldvolt;
static esp_adc_cal_characteristics_t adc1_chars;
uint8_t ChargeLevel, slows = 0;
float Volt1, Volt2, Volt3, Volt4, Volt5 = 0;
uint8_t g, e, t = 1;
bool rh, lh, rl, ll, Charging;
unsigned long battery_timer;
uint8_t cur_reads;
float tempmVolt;
float mVolt;
String cur_volt;
unsigned long batt2_timer;
unsigned long batt3_timer;
#endif

// WEB-интерфейс
// ========== ДЕФАЙНЫ НАСТРОЕК ==========
// объявлять ДО ПОДКЛЮЧЕНИЯ БИБЛИОТЕКИ GyverPortal
#define GP_NO_MDNS      // убрать поддержку mDNS из библиотеки (вход по хосту в браузере)
#define GP_NO_DNS       // убрать поддержку DNS из библиотеки (для режима работы как точка доступа)
#define GP_NO_OTA       // убрать поддержку OTA обновления прошивки
#define GP_NO_UPLOAD    // убрать поддержку загрузки файлов на сервер
#define GP_NO_DOWNLOAD  // убрать поддержку скачивания файлов с сервера

int wifi_mode = 0;  // 0 - прямой доступ; 1 - AP
char wifi_pass[20] = "12345678";
String def_ssid = "Asus_home";
String def_pass = "11111111";
String wifi_info = "";
char password_key[11] = "Never_die!";
struct LoginPass {
  char ssid[20];
  char pass[20];
};
LoginPass lp, lp_save;
String password;
String local_IP = "192.168.4.1";
String wifi_ssid[20] = {};
int cur_wifi;

#include <GyverPortal.h>
GyverPortal ui;
bool themas = true;  // тема web-интерфейса (true - тёмная, false - светлая)

//===== Часовой пояс ========================================================================
#include <ESP32Time.h>  // библиотека работы со встроенными часами
int time_zone = 10800;  // часовой пояс
ESP32Time rtc(0);
#include <NTPClient.h>  // Библиотека получения точного времени и даты через интернет службу NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");  // Адрес сервиса точного времени
//Дни недели
int8_t sHour[MAX_ALARMS], sMinute[MAX_ALARMS], wHour[MAX_ALARMS], wMinute[MAX_ALARMS];
String weekDays[7] = { "воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота" };
//Месяца
String months[12] = { "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря" };
String old_time;
int old_day;
bool sleep_flag[MAX_ALARMS], waikup_flag[MAX_ALARMS], sleep_day[MAX_ALARMS][8] = { false }, waikup_day[MAX_ALARMS][8] = { false };
String days[8] = { "Пнд", "Втр", "Срд", "Чтв", "Птн", "Сбт", "Вск", "Все дни" };
GPtime sleep_time[MAX_ALARMS], waikup_time[MAX_ALARMS];
bool sleeping = false, gong_flag = false, gong_end = false;

bool preview_mode_flag;  // true - при поворотах энкодера пролистывает станции, а фиксация выбора осуществляется нажатием кнопки энкодера; false - ьрадиционное переключение станций поворотом энкодера
bool preview_flag = false;
bool favorites_flag = false, favorites_flag1 = false, favorites_flag4 = false;
String edit_radio_title, edit_radio_uri;
String find_radio_text;
String station_ini_string;
String rs_title;  // Название текущей радиостанции
String index_info = F("Если вы добавили папки и файлы музыки на SD-карту, то для ускорения доступа к файлам произведите разовую индексацию.");
#define DIR_LIMIT 40  // максимальное количество папок с музыкальными файлами
String music_type_title[DIR_LIMIT];
String m_type_title;
int dir_count = 0;
bool indexMusic = false;
int rl_count[20];          // массив для хранения количества радиостанций в каждом INI файле
int radio_per_list = 500;  // максимальное количество радиостанций в каждом INI файле
int radio_per_page = 200;  // по сколько станций выдавать на странице списком через web-интерфейс
// При rl_count = 20 и radio_per_list = 500 получается, что мы можем хранить и осуществлять быстрый доступ к 10 000 радиостанций
int total_radio_ini = 1;  //фактическое количество INI файлов радиостанций
int radio_location_count = 6;
String radio_location_title[7] = { "весь мир", "Россия", "Европа", "Азия", "Африка", "Австралия", "Америка" };
int cur_radio_location = 0;
int fix_radio_type, fix_radio_location;
bool new_station;
int radio_type_count = 20;
String radio_type_title[21] = { "ВСЕ СТАНЦИИ", "КЛАССИКА", "ЭТНО", "ДЖАЗ", "РОК", "ПОП", "ПСИХОДЕЛИКА", "ТАНЦЫ", "РЕЛАКС", "РЕТРО", "ДЕТСКОЕ", "АУДИОКНИГИ", "НОВОСТИ", "СПОРТ", "СССР", "ДИСКО", "РАЗГОВОРНОЕ", "РЭП", "КАНТРИ", "БЛЮЗ", "ШАНСОН" };
String display = "", add_info = "", mp3_info = "";
String cur_type_title, cur_location_title;
String station, curl, title, channels, sample_rate, bits_per_sample, bit_rate, aac_header_format, aac_codec, album = "-", year = "-", comment = "-";
int radio_mode = 0;
int fix_mode = 0;
int edit_radio_typ, edit_radio_loc;
int volumea = 5, volume = 12;
bool volume_flag = true;
bool mono = false;
int low_freq = 20;
int mid_freq = 20;
int hi_freq = 20;
int balance = 0;
char cur_im[6], max_im[6];
int VUlevel, oldVUlevel, maxVUlevel = 100000;
int w, h;
bool update_flag = false, skan = false;
bool pause_flag = false;
bool sd_flag = true;
File file;
String radio_station, old_radio_station;
int radio_station_count;
int cur_radio_station = 1;
int pre_radio_station = 1;
int cur_radio_station1 = 1;
int cur_radio_station2 = 1;
int cur_radio_station3 = 1;
int cur_radio_station4 = 1;
int cur_radio_type = 3;
int menu = 0;
String music_file, fix_music;
uint32_t fix_ptime;
int cur_music_type, cur_music_file, pre_music_file, cur_music_file1, cur_music_file2, cur_music_file3, music_count;
bool internet_flag = false;
bool eeprom_flag = false;
unsigned long eeprom_timer, update_timer, info_timer, info_timer1, slow_timer, retry_timer, total_timer;
bool auto_save = true;
int save_time = 5;  // Интервал проверки необходимости сохранение настроек в минутах
bool rand_song = false, old_rand_song;
bool all_fav_flag = false;
bool alarm_mode[MAX_ALARMS] = { 0 };  // 0 - радио, 1 - проигрыватель
const char alarm_mode_list[] = "радио,проигрыватель";
uint16_t counter_st = 0;

#define off 0x40
#define repeat 0x42
#define next 0x50
#define prev 0x51
#define up 0x16
#define down 0x1a
#define swap 0x4c
#define mute 0x5a
#define menus 0x44
#define clocks 0x11
#define fav 0x19
#define randoms 0x13
#define ganr 0x4e
#define ganrall 0x0d
#define ganrp 0x0c
#define lok 0x4a
#define lokall 0x09
#define lokp 0x08
#define next10 0x54
#define prev10 0x55
#define skansss 0x43

//Куранты
bool slowk = false, autoscan = false;
bool kurant_flag = true;
const char kurant_type_list[] = "камин. часы,кукушка,куранты,склянки,армия,сонар,муз. шкатулка,голос";
String kurant_type[8] = { "каминные часы", "часы с кукушкой", "куранты", "морские склянки", "армия", "сонар", "муз. шкатулка", "время голосом" };
int cur_kurant = 0;
int mus_box_count = 46;

//Счётчик попыток восстановления связи со станцией
int retry = 0;

//Ау!
int cur_lang = 0;
bool speek_end = false;

bool info_flag0, info_flag, voice_menu_flag = false, voice_sw_flag = false, del_st_sw_flag = false, del_st_slow_flag = false;

int slow, old_slow;
bool mute_flag = false;
unsigned long totalTime;

int load_menu_count = 3;
String load_menu_title[4] = { "Новое соединение", "Повтор соединения", "Использовать сохранённые", "Пропустить wi-fi" };
int cur_load_menu, old_load_menu;
bool push_flag = false;
int cHour, cMinute, cSecond, currentDay;
bool ready_flag = false;
bool skip_flag = false;
bool retry_flag = false;
bool reload_flag = true;
int rnd_music = 0;
bool play_music_flag = false;
int pik_volume = 0;  // Максимальный пик громкости текущей трансляции звука
int middle_volume = 9;
int vk = 0;
int c_volume = 0;
bool aru = true;  //АРУ громкости
bool aru_change = false;
unsigned long aru_timer;
bool saved_connect_flag = false;
bool continue_flag = true;
bool na_flag = false;
bool file_flag = true;
unsigned long radiometer;
uint32_t s_afd;
bool display_ready = false;
bool dot_flag = false;
unsigned long clock_timer;
bool block_flag = false;

int cur_palette = 0;
int main_menu_pos = 0;

bool updateFirmware = false;
int total_hours;

int cur_led = 12, set_led = 12, fix_led = 12, save_led = 12;
int led_time = 0;
bool led_flag = false;
unsigned long led_timer, led_timer1;
bool update_now_flag = false;
uint8_t old_progress = 0;
int8_t numAl = 0, numAl2 = 0;
int cur_radio_ini = 0;
bool play_station_flag = false;
bool replay_station_flag = false;
int start_pos, end_pos;

//================ РАСПРЕДЕЛЕНИЕ ЗАДАЧ НА 2 ЯДРА ПРОЦЕССОРА ESP32 =========================
// Необходимость задействовать второе ядро процессора связана с необходимостью:
// a) иметь необходимые вычислительные ресурсы для обработки звука;
// б) иметь стабильный информационный обмен по wi-fi через web-интерфейс и отрисовку на экране.
TaskHandle_t Radio_task;
TaskHandle_t Comm_task;
//--- Размер статической памяти, резервируемый под 2 задачи ---
#define RADIO_STACK_SIZE 10000  //12000 //13000  //12000  //8800 байт
#define COMM_STACK_SIZE 22000   //28000   //21000 //22000  //19200 байт

//-------------------------------------
void setup() {
  //Управление подсветкой дисплея
  // от TFT_LED-пина через резистор 300 Ом идёт подключение к базе PNP транзистора (2SA708),
  // коллектор подключён на +3 в, а эммитер - на пин LED экрана
  pinMode(TFT_LED, OUTPUT);
  analogWrite(TFT_LED, 255);

  def_ssid.toCharArray(lp.ssid, 20);
  def_pass.toCharArray(lp.pass, 20);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  Serial.begin(115200);

  //Информация о плате
  GetSysInfo();

  //Энкодер
  Serial.println(F("Инициализирую энкодер."));
  enc1.setType(TYPE2);

  // ИК-пульт
#if (PULT == 1)
  Serial.println(F("Инициализирую датчик ИК-пульта."));
  IrReceiver.begin(IR_PIN, false, 0);
#endif

  Serial.println(F("Инициализирую кнопку."));
  butt1.setDebounce(50);       // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(300);       // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(600);  // настройка таймаута между кликами (по умолчанию 300 мс)
  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);

  // Считывание сохранённых настроек из EEPROM
  Serial.println(F("Считываю данные из EEPROM."));
  readEEPROM();
  sys_font = true;

  // Установка палитры для экрана в разных режимах
  Serial.println(F("Инициализирую дисплей."));
  setPalettes(cur_palette);

  //инициализация tft-дисплея
  tft.initR(INITR_BLACKTAB);  // Init ST7735S chip, black tab
  tft.setSPISpeed(8000000);   // set 8 MHz
  Serial.println(F("TTF-display initialized"));
  tft.setRotation(3);
  tft.fillScreen(sys_color[0]);
  w = tft.width();
  h = tft.height();
  tft.setFont();

  // инициализация и настройка аудио
  Serial.println(F("Инициализирую аудио."));
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(0);
  audio.setBalance(balance);
  audio.forceMono(mono);
  audio.setTone(low_freq - 15, mid_freq - 15, hi_freq - 15);

  analogWrite(TFT_LED, 255 - 10 * cur_led);
  fix_led = cur_led;

  //Инициализация и проверка sd-карты
  Serial.println(F("Инициализирую и проверяю sd-карту."));
  if (!SD.begin(SD_CS)) {
    title = F("SD-карта неисправна\nили не вставлена!\n");
    Serial.println(title);
    tft.setCursor(1, 24);
    tft.setTextColor(sys_color[1]);
    tft.println(utf8rus(title));
    title = F("Пожалуйста, вставьте\nsd-карту и\nперезагрузите устройство.");
    Serial.println(title);
    tft.println(utf8rus(title));
    tft.setTextColor(sys_color[2]);
    sd_flag = false;
    for (;;) {
      esp_task_wdt_reset();
    }
  } else {
    // Проверка флага обновления прошивки - обновление прошивки
    if (updateFirmware == true && SD.exists("/firmware.bin")) {
      Serial.println(F("Обновление прошивки."));
      update_now_flag = true;
      updateFirmware = false;
      sys_font = fix_sys_font;
      updateEEPROM();
      //-----------------------------------
      sys_font = false;
      bmpDraw("/System/update.bmp", 0, 0);
      tft.setFont(&mMediaBold7pt8b);
      tft.setTextColor(ST77XX_BLUE);
      tft.setCursor(58, 12);
      tft.print(utf8rus(F("Обновление")));
      tft.setCursor(68, 26);
      tft.print(utf8rus(F("прошивки")));
      sys_font = true;
      tft.setTextSize(1);
      //--------------------------------
      File updateBin = SD.open("/firmware.bin");
      size_t updateSize = updateBin.size();
      Serial.print("Размер firmware.bin = ");
      Serial.println(updateSize);
      if (updateBin) {
        Update.onProgress(progresscallback);
        if (Update.begin(updateBin.size(), U_FLASH)) {
          Update.writeStream(updateBin);
          if (Update.end()) {
            Serial.println(F("Обновление закончено!"));
          } else {
            Serial.println(F("ОШИБКА обновления!"));
            Serial.println(Update.getError());
          }
        } else {
          Serial.println(F("Файл firmware.bin слишком большой!"));
        }
      }
      updateBin.close();
      if (SD.exists("/firmware.bak")) SD.remove("/firmware.bak");
      if (SD.rename("/firmware.bin", "/firmware.bak")) {
        Serial.println(F("Файл прошивки успешно переименован!"));
      } else {
        Serial.println(F("ОШИБКА переименования файла прошивки!"));
      }
      delay(1000);
      //---Для тестов-----------------------------------
      //SD.rename("/firmware.bak", "/firmware.bin");
      //delay(1000);
      //------------------------------------------------
      ESP.restart();
    }
    // Заставка
    bmpDraw("/System/logo.bmp", 0, 0);
    delay(3000);

    //--------- Проверка целостности INI-файлов ----------------
    Serial.println(F("Проверка целостности ini-файлов в папке Radio."));
    String r_file = "";
    for (int i = 0; i < total_radio_ini; i++) {
      r_file = "/Radio/radio" + String(i);
      if (SD.exists(r_file + ".tmp")) {
        //Предыдущее изменение файла radio(i).ini НЕ закончилось успешно
        SD.remove(r_file + ".tmp");
        if (SD.exists(r_file + ".bak")) {
          if (SD.exists(r_file + ".ini")) SD.remove(r_file + ".ini");
          SD.rename(r_file + ".bak", r_file + ".ini");
        }
      }
    }
    if (SD.exists("/Radio/favorites.tmp")) {
      //Предыдущее изменение файла favorites.ini НЕ закончилось успешно
      SD.remove("/Radio/favorites.tmp");
      if (SD.exists("/Radio/favorites.bak")) {
        if (SD.exists("/Radio/favorites.ini")) SD.remove("/Radio/favorites.ini");
        SD.rename("/Radio/favorites.bak", "/Radio/favorites.ini");
      }
    }

    // Подключение к Сети
    web_connect();
    clearInfo();
    // Считывание количества радиостанций и url сохранённой радиостанции
    radio_station_count = CountRadioStation(cur_radio_type, cur_radio_location);
    if (radio_station_count == 0) cur_radio_type = 0;
    Serial.print(F("Количество радиостанций: "));
    Serial.println(radio_station_count);
    if (cur_radio_station > radio_station_count || cur_radio_station == 0) cur_radio_station = 1;
    Serial.print(F("Сохранённая радиостанция станция №"));
    Serial.println(cur_radio_station);
    //Загрузка сохранённой радиостанции
    radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
    Serial.print(F("URL станции: "));
    Serial.println(radio_station);

    if (internet_flag == false) {
      radio_mode = 2;
      fix_mode = 2;
    }
    // Считывание папок с музыкой на SD-карте
    ScanDir("Music");
  }

  sys_font = fix_sys_font;

  // подключаем конструктор web-сервера и запускаем
  Serial.println(F("Запуск web-сервера."));
  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
  ui.uploadAuto(0);    // выключить автозагрузку
  ui.deleteAuto(0);    // выключить автоудаление
  ui.downloadAuto(0);  // выключить автоскачивание
  ui.renameAuto(0);    // выключить автопереименование

  switch (radio_mode) {
    case 0:
      // Включаем сохранённое радио
      if (internet_flag == true) {
        play_music_flag = false;
        play_station_flag = true;
      }
      break;
    case 1:
      tft.fillScreen(clock_color[0]);
      update_flag = true;
      old_time = "";
      break;
    case 2:
      ScanFiles(cur_music_type);
      if (music_count > 0) {
        fix_music = music_file;
        old_radio_station = radio_station;
        clearInfo();
        play_music_flag = true;
        play_station_flag = false;
        replay_station_flag = false;
      }
      break;
  }
  menu = radio_mode;
  //  fix_mode = radio_mode;

// Инициализация АЦП для контроля заряда аккумулятора
#ifndef BATTERY_OFF
  // Настройка и инициализация АЦП
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(USER_ADC_CHAN, ADC_ATTEN_DB_12);

  // Расчет характеристик АЦП т.е. коэффициенты усиления и смещения
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 0, &adc1_chars);
#endif

  if (update_now_flag == false) {
    //Основные задачи (Radio) будут выполняться на ядре 0 с максимальным приоритетом (4)
    //Толковая инструкция по организации и настройки многозадачности: https://kotyara12.ru/iot/esp32_tasks/?ysclid=lepk1u09t0551245100
    //xTaskCreatePinnedToCore(
    static StaticTask_t Radio_task;
    static StackType_t Radio_stack[RADIO_STACK_SIZE];
    xTaskCreateStaticPinnedToCore(
      Radio_task_proc,   // Функция задачи
      "Radio task",      // Ее имя
      RADIO_STACK_SIZE,  // Размер стека функции
      NULL,              // Параметры задачи
      20,                // Приоритет
      Radio_stack,       // Стек выделенной статической памяти под задачу
      &Radio_task,       // Дескриптор задачи для отслеживания
      0);                // Указываем пин для данного ядра
    delay(500);

    //Коммуникационная задача (wi-fi, web) будет выполняться на ядре 1 с средним приоритетом (3)
    //Толковая инструкция по организации и настройки многозадачности: https://kotyara12.ru/iot/esp32_tasks/?ysclid=lepk1u09t0551245100
    static StaticTask_t Comm_task;
    static StackType_t Comm_stack[COMM_STACK_SIZE];
    xTaskCreateStaticPinnedToCore(
      Comm_task_proc,        // Функция задачи
      "Communication task",  // Имя задачи
      COMM_STACK_SIZE,       // Размер стека
      NULL,                  // Параметры задачи
      2,                     // Приоритет
      Comm_stack,            // Стек выделенной статической памяти под задачу
      &Comm_task,            // Дескриптор задачи для отслеживания
      1);                    // Указываем пин для этой задачи
    delay(500);
  }
  old_rand_song = rand_song;
  initOTA();
  if (led_time > 0) led_timer = millis();
  if (sleeping == true) {
    set_led = 0;
    audio.setVolume(0);
    while (audio.getVolume() != 0) {
      vTaskDelay(10);
    }
    if (audio.isRunning()) audio.stopSong();
  }
}

// Основной цикл пуст, т.к. все задачи распределены по двум параллельным процессам
void loop() {
  if (update_now_flag == false) {
    vTaskDelete(NULL);
  } else {
    esp_task_wdt_reset();
  }
}

//=== На первом ядре запущены процессы обработки и визуализации звука ===
void Radio_task_proc(void *pvParameters) {
  Serial.print(F("Процесс Radio запущен на ядре № "));
  Serial.println(xPortGetCoreID());

  //Watch dog
  Serial.println(F("Конфигурируется WDT..."));
  esp_task_wdt_init(WDT_TIMEOUT, true);  //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                //add current thread to WDT watch

  for (;;) {
    CheckConnection();  // проверка wi-fi соединения
    if (internet_flag == true) {
      if (radio_mode == 0 && ((millis() - timeout) / 1000) + (WDT_TIMEOUT / 8) > (WDT_TIMEOUT / 4) && audio_flag == false && timeout_flag == true) {
        timeout_flag = false;
        na_flag = true;
        audio.stopSong();
        title = F("Интернет-радиостанция\nочень долго\nне отвечает.");
        Serial.println(title);
        vTaskDelay(300);
        if (del_st_sw_flag) notfound();
        else if (!autoscan) cur_radio_station++;
        notend();
      }
      if (play_station_flag == true) {
        play_station_flag = false;
        clearInfo();
        PlayStation();
      }
      if (replay_station_flag == true) {
        clearInfo();
        ReplayStation();
        replay_station_flag = false;
      }
    }

    if (indexMusic == true) {
      IndexMusicFilesProc();
      indexMusic = false;
    }
    if (play_music_flag == true) {
      play_music_flag = false;
      PlayMusic();
    }

    if (ready_flag == true) {
      CheckLed();
      if (sleeping == false) {
        vTaskDelay(1);
        audio.loop();
        CheckVolume();
        // ввод URL станции через com-порт
        if (Serial.available() && internet_flag == true) {
          if (audio.isRunning()) audio.stopSong();
          String r = Serial.readString();
          r.trim();
          if (r.length() > 5) audio.connecttohost(r.c_str());
          log_i("free heap=%i", ESP.getFreeHeap());
        }
        if (fix_mode == 0 && old_radio_station != radio_station) {
          Serial.println(F("Перезапуск станции."));
          ReplayStation();
          old_radio_station = radio_station;
        }
        CheckSlow();
        //        CheckRadiometer();
      } else {
        int i;
        while (i < 1000) {
          i++;
          vTaskDelay(10);
          esp_task_wdt_reset();
          yield();
        }
      }
    }
    vTaskDelay(1);
    esp_task_wdt_reset();
    eepromTick();                   // проверка не пора ли сохранить настройки
    if (!sleeping) retryControl();  // проверка на возобновление трансляции при её обрыве
    totalHours();                   // счётчик пробега
  }
  // Процесс непредвиденно прервался
  Serial.print(F("Процесс Radio непредвиденно прервался..."));
  vTaskDelete(NULL);
}

//=== На нулевом ядре запущены коммуникационные модули ===
void Comm_task_proc(void *pvParameters) {
  Serial.print(F("Wi-fi и web запущены на ядре № "));
  Serial.println(xPortGetCoreID());

  //Watch dog
  Serial.println(F("Конфигурируется WDT..."));
  esp_task_wdt_init(WDT_TIMEOUT, true);  //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                //add current thread to WDT watch

  for (;;) {
    // опрос web-портала
    ui.tick();

    if (ready_flag == true) {
      // Опрос энкодера
      CheckEncoder();
      // Опрос кнопки
      CheckButton();
#if (PULT == 1)
      // опрос и обработка команд от ИК-пульта
      ir_tick();
#endif
      if (update_flag == true) {
        if (millis() - update_timer > 1000) update_display_info();
      }
      // Контроль будильника
      if (millis() - clock_timer > 500) ClockControl();
      if (display_ready == true) {
        GetPlayInfo();
#ifndef BATTERY_OFF
        // Отображение заряда аккумулятора
        if (radio_mode != 1) {
          if (Charging || Volt < 2.7) {
            if (millis() - battery_timer > 1000) Battery();
          } else {
            if (millis() - battery_timer > 5000) Battery();
          }
        }
#endif
      }
    }
#ifndef BATTERY_OFF
    // Замеры напряжения аккумулятора
    if (radio_mode != 1) checkBattery();
#endif
    ArduinoOTA.handle();
    vTaskDelay(2);
    esp_task_wdt_reset();
  }
  // Процесс непредвиденно прервался
  Serial.print(F("Процесс 'Wi-fi и web' непредвиденно прервался..."));
  vTaskDelete(NULL);
}

String getInfo(String info, String tag) {
  String buff = info;
  int pos = buff.indexOf(tag);
  if (pos == 0) return buff.substring(tag.length());
  return "";
}

void audio_eof_mp3(const char *info) {
  if (gong_flag == true) {
    gong_end = true;
  } else {
    info_flag = false;
    PlayMusicNext();
  }
}

// optional
void audio_info(const char *info) {
  Serial.print(F("info        "));
  Serial.println(info);
  String buff = info;

  if (buff.indexOf(F("End of speech: ")) != -1) return;
  if (audio.isRunning()) slowk = false;
  add_info = getInfo(info, F("Channels: "));
  if (add_info != "") {
    pik_volume = 0;
    c_volume = 0;
    channels = add_info;
    update_timer = millis();
    if (skan && radio_mode == 0) {
      counter_st++;
      cur_radio_station++;
      if (counter_st >= radio_station_count) {
        skan = false;
        counter_st = 0;
      }
      notend();
    }
    update_flag = true;
    if (channels == "1") {
      audio.setBalance(0);
      audio.forceMono(true);
    } else {
      audio.setBalance(balance);
      audio.forceMono(mono);
    }
  }
  add_info = getInfo(info, F("SampleRate: "));
  if (add_info != "") {
    audio_flag = true;
    sample_rate = add_info;
    update_timer = millis();
    update_flag = true;
  }
  add_info = getInfo(info, F("BitsPerSample: "));
  if (add_info != "") {
    audio_flag = true;
    bits_per_sample = add_info;
    update_timer = millis();
    update_flag = true;
  }
  add_info = getInfo(info, F("BitRate: "));
  if (add_info != "") {
    audio_flag = true;
    bit_rate = add_info;
    update_timer = millis();
    update_flag = true;
  }

  add_info = getInfo(info, F("AAC HeaderFormat: "));
  if (add_info != "") {
    audio_flag = true;
    aac_header_format = add_info;
    update_timer = millis();
    update_flag = true;
  }

  add_info = getInfo(info, F("AAC Codec: "));
  if (add_info != "") {
    audio_flag = true;
    aac_codec = add_info;
    update_timer = millis();
    update_flag = true;
  }

  add_info = getInfo(info, F("Reading file: "));
  if (add_info != "" && add_info.indexOf("/", 0) != -1) {
    curl = add_info;
    update_timer = millis();
    update_flag = true;
  }

  //Воспроизведение прервалось -> подождать и повторить соединение
  if (buff.indexOf(F("Request ")) != -1 && buff.indexOf(F(" failed!")) > 0 && internet_flag == true) {
    slowk = true;
    //Очень короткий и возможно пустой url радиостанции
    if (buff.length() < 26) {
      old_radio_station = radio_station;
      na_flag = true;
      stop_song_Play();
      title = F("Некорректный url радиостанции.");
      Serial.println(title);
      esp_task_wdt_reset();
      vTaskDelay(300);
      update_flag = true;
      retry = 4;
    }

    retry++;
    Serial.print("retry = ");
    Serial.println(retry);
    int ssp;
    while (ssp < 1000) {
      esp_task_wdt_reset();
      vTaskDelay(2);
      yield();
      ssp++;
    }
    if (retry < 4) {
      title = "Попытка установить связь\nсо станцией #" + String(retry);
      Serial.println(title);
      update_timer = millis();
      update_flag = true;
      int ssp;
      while (ssp < 400) {  // 4 секунды задержки
        esp_task_wdt_reset();
        vTaskDelay(10);
        yield();
        ssp++;
      }
      //ReplayStation();
      replay_station_flag = true;
    } else {
      old_radio_station = radio_station;
      title = F("Попытки соединения\nне увенчались успехом.\nСтанция не отвечает.");
      Serial.println(title);
      na_flag = true;
      update_flag = true;
      update_timer = millis();
      stop_song_Play();
      vTaskDelay(300);
      if (del_st_sw_flag) notfound();
      else if (!autoscan) cur_radio_station++;
      notend();
    }
  }

  if (buff.indexOf(F("could not be initialized")) != -1 && internet_flag == true) {
    old_radio_station = radio_station;
    stop_song_Play();
    title = F("Трансляция прервалaсь\nи не может быть восстановлена.");
    update_timer = millis();
    if (del_st_sw_flag) notfound();
    else if (!autoscan) cur_radio_station++;
    notend();
  }

  if (buff.indexOf(F("unknown content found at:")) != -1) {
    old_radio_station = radio_station;
    stop_song_Play();
    na_flag = true;
    title = F("Интернет-станция\nне ведёт трансляцию.\nВыберите другую станцию.");
    update_timer = millis();
    if (del_st_sw_flag) notfound();
    else if (!autoscan) cur_radio_station++;
    notend();
  }

  if (buff.indexOf(F("slow stream, dropouts are possible")) != -1 && internet_flag == true) {
    slow_timer = millis();
    slow++;
  } else slows = 0;

  if (buff.indexOf(F("Stream lost -> try new connection")) != -1 && internet_flag == true) {
    //title = F("Низкая скорость.\nНовое соединение.");
    //update_timer = millis();
    //update_flag = true;
    stop_song_Play();
  }
}

void audio_id3data(const char *info) {  //id3 metadata
  Serial.print(F("id3data     "));
  Serial.println(info);
  String add_info;

  add_info = getInfo(info, F("Title: "));
  if (add_info != "") {
    station = add_info;
    station.trim();
    update_timer = millis();
    update_flag = true;
  }
  add_info = getInfo(info, F("Artist: "));
  if (add_info != "") {
    title = add_info;
    title.trim();
    update_timer = millis();
    update_flag = true;
  }
  add_info = getInfo(info, F("Album: "));
  if (add_info != "") {
    album = add_info;
    album.trim();
    update_timer = millis();
    update_flag = true;
  }
  add_info = getInfo(info, F("Year: "));
  if (add_info != "") {
    year = add_info;
    year.trim();
    update_timer = millis();
    update_flag = true;
  }
  add_info = getInfo(info, F("Comment: "));
  if (add_info != "") {
    comment = add_info;
    comment.trim();
    update_timer = millis();
    update_flag = true;
  }
  //scanflag = true;
}

void audio_showstation(const char *info) {
  Serial.print(F("station     "));
  Serial.println(info);
  //if (info == "") info = "* * *";
  if (rs_title != info && strlen(info) > rs_title.length()) {
    new_station = false;
    edit_radio_typ = fix_radio_type;
    edit_radio_loc = fix_radio_location;
    edit_radio_title = info;
    edit_radio_uri = radio_station;
    cur_radio_station = EditStation();
  }
  if (station != info && retry < 4) {
    station = info;
    update_timer = millis();
    update_flag = true;
  }
}

void audio_showstreamtitle(const char *info) {
  Serial.print(F("streamtitle "));
  Serial.println(info);
  if (title != info && retry < 4) {
    title = info;
    update_timer = millis();
    update_flag = true;
    pik_volume = 0;
    c_volume = 0;
  }
  String buff = info;
  if (buff.indexOf(F("HTTP/1.1 400 Bad Request")) != -1) {
    esp_task_wdt_reset();
    vTaskDelay(300);
    if (del_st_sw_flag) notfound();
    else if (!autoscan) cur_radio_station++;
    notend();
    speek_end = true;
  }
  if (buff.indexOf(F("404 Not Available")) != -1) {
    old_radio_station = radio_station;
    na_flag = true;
    stop_song_Play();
    title = F("Интернет-радиостанция\nне отвечает.");
    esp_task_wdt_reset();
    vTaskDelay(300);
    if (del_st_sw_flag) notfound();
    else if (!autoscan) cur_radio_station++;
    notend();
  }
  if (buff.indexOf(F("404 Not Found")) != -1 || buff.indexOf(F("404 File Not Found")) != -1) {
    old_radio_station = radio_station;
    na_flag = true;
    stop_song_Play();
    title = F("Интернет-радиостанция\nне найдена.");
    esp_task_wdt_reset();
    vTaskDelay(300);
    if (del_st_sw_flag) notfound();
    else if (!autoscan) cur_radio_station++;
    notend();
  }
}

void audio_bitrate(const char *info) {
  Serial.print(F("bitrate     "));
  Serial.println(info);
}

void audio_commercial(const char *info) {  //duration in sec
  Serial.print(F("commercial  "));
  Serial.println(info);
}

void audio_icyurl(const char *info) {  //homepage
  Serial.print(F("icyurl      "));
  Serial.println(info);


  if (info == "" && na_flag == false && internet_flag == true) {
    esp_task_wdt_reset();
    Serial.println(F("Трансляция прервалась."));
    title = F("Трансляция прервалась.");
    update_flag = true;
    update_timer = millis();
    vTaskDelay(500);
    slow_timer = millis();
    slow = 6;
    esp_task_wdt_reset();
  }
}

void audio_lasthost(const char *info) {  //stream URL played
  Serial.print(F("lasthost    "));
  Serial.println(info);
  if (curl != info) {
    curl = info;
    update_timer = millis();
    update_flag = true;
    pik_volume = 0;
    c_volume = 0;
  }
}

void audio_eof_speech(const char *info) {
  Serial.print(F("eof_speech  "));
  Serial.println(info);
  esp_task_wdt_reset();
  vTaskDelay(600);
  speek_end = true;
}

void GetPlayInfo() {
  //Отображение текущей позиции и длительности композиции в режиме Проигрывателя
  if (radio_mode == 2) {
    if (millis() - info_timer > 1000 && info_flag == true) {
      info_timer = millis();
      uint32_t act = audio.getAudioCurrentTime();
      uint32_t afd = audio.getAudioFileDuration();
      sprintf(max_im, "%i:%02d", (afd / 60), (afd % 60));
      sprintf(cur_im, "%i:%02d", (act / 60), (act % 60));
      int16_t tx, ty;
      uint16_t tw, th;
      tft.setFont();
      tft.getTextBounds(cur_im, 24, 9, &tx, &ty, &tw, &th);
      tft.fillRect(24, 9, tw, th, player_color[1]);
      tft.setCursor(24, 10);
      tft.setTextColor(player_color[4]);
      tft.print(String(cur_im));
      tft.setTextColor(player_color[2]);
      tft.print(F(" - "));
      tft.getTextBounds(F(" - "), tx, ty, &tx, &ty, &tw, &th);
      tft.setTextColor(player_color[4]);
      tft.getTextBounds(F("000:00"), tx, ty, &tx, &ty, &tw, &th);
      if (afd != s_afd) {
        s_afd = afd;
        tft.fillRect(tx + tw, 8, tw, th, player_color[1]);
      }
      tft.print(String(max_im));
    }
  }
  // Отображение шкалы громкости звука
  int iVUlevel = map(audio.getVUlevel(), 0, maxVUlevel, 0, 100);
  if (iVUlevel > 100) {
    VUlevel = 100;
    iVUlevel = 100;
    maxVUlevel = audio.getVUlevel();
  } else {
    VUlevel = iVUlevel * 2;
  }
  if (millis() - info_timer1 > 100 && (info_flag0 == true || info_flag == true) && (radio_mode == 0 || radio_mode == 2)) {
    info_timer1 = millis();
    iVUlevel = (iVUlevel + oldVUlevel) / 2;
    iVUlevel = iVUlevel * 1.64;  //1.68;
    if (iVUlevel > oldVUlevel) {
      if (radio_mode == 0) {
        tft.drawRect(0, 108, iVUlevel, 2, radio_color[5]);
      } else {
        tft.drawRect(0, 108, iVUlevel, 2, player_color[5]);
      }
    } else {
      if (radio_mode == 0) {
        tft.drawRect(iVUlevel, 108, oldVUlevel, 2, radio_color[0]);
      } else {
        tft.drawRect(iVUlevel, 108, oldVUlevel, 2, player_color[0]);
      }
    }
    oldVUlevel = iVUlevel;

    if (aru == true && aru_change == true && preview_flag == false) {
      aru_change = false;
      tft.setFont();
      tft.setTextSize(1);
      tft.setCursor(136, 120);
      if (radio_mode == 0) {
        tft.setTextColor(radio_color[4]);
      } else {
        tft.setTextColor(player_color[4]);
      }
      tft.print("A");
      if (radio_mode == 0) {
        tft.setTextColor(radio_color[3]);
        tft.fillRect(142, 119, 18, 10, radio_color[1]);
      } else {
        tft.setTextColor(player_color[3]);
        tft.fillRect(142, 119, 18, 10, player_color[1]);
      }
      if (pik_volume > middle_volume) {
        tft.print(String(vk));
      }
      if (pik_volume <= middle_volume) {
        tft.print("+");
        tft.print(String(vk));
      }
    }

    // Отображение текущей громкости
    int current_volume = audio.getVolume();
    int set_volume = volume + vk;
    if (set_volume > 21) set_volume = 21;

    if (current_volume != set_volume) {
      volume_flag = true;
      showVolume(current_volume + vk);
    } else {
      if (volume_flag == true) {
        volume_flag = false;
        showVolume(set_volume);
      }
    }

    // Качество связи с радиостанцией
    if (radio_mode == 0 && slow != old_slow) {
      old_slow = slow;
      tft.setFont(&mMediaSymbols);
      tft.setTextSize(1);
      tft.setCursor(0, 15);
      tft.setTextColor(radio_color[1]);
      // Пиктограмма радио
      tft.print("9");
      tft.setCursor(0, 15);
      tft.setTextColor(radio_color[2]);
      if (slow < 5) tft.print(String(9 - slow));
      else {
        slows++;
        if (slows > 5) {
          if (del_st_slow_flag) notfound();
          else if (!autoscan) cur_radio_station++;
          notend();
        }
      }
    }
  }
}

void showVolume(int sVolume) {
  if (preview_flag == true) return;
  if (radio_mode != 1) {
    switch (radio_mode) {
      case 0:
        tft.fillRect(146, 110, 160, 10, radio_color[1]);
        tft.setTextColor(radio_color[4]);
        break;
      case 2:
        tft.fillRect(146, 110, 160, 10, player_color[1]);
        tft.setTextColor(player_color[4]);
        break;
      default:
        break;
    }
    tft.setFont();
    tft.setTextSize(1);
    tft.setCursor(148, 111);
    tft.print(sVolume);
  }
}

void clearInfo() {
  display_ready = false;
  s_afd = 0;
  pik_volume = 0;
  c_volume = 0;
  channels = "-";
  sample_rate = "-";
  bits_per_sample = "-";
  bit_rate = "-";
  aac_header_format = "-";
  aac_codec = "-";
  station = " ";
  title = " ";
  curl = " ";
  album = "-";
  year = "-";
  comment = "-";
  mp3_info = " ";
  update_timer = millis();
  update_flag = true;
}

void GetSysInfo() {
  Serial.println(F("==== Информация о плате ==="));
  Serial.print(F("Ревизия чипа: "));
  Serial.println(ESP.getChipRevision());
  Serial.print(F("Версия SDK: "));
  Serial.println(ESP.getSdkVersion());
  uint64_t chipid = ESP.getEfuseMac();
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32));
  Serial.printf("%08X\n", (uint32_t)chipid);
  Serial.printf("Размер скетча:\t%d байт\n", ESP.getSketchSize());
  Serial.printf("Размер свободной памяти под скетч:\t%d байт\n", ESP.getFreeSketchSpace());
  Serial.printf("Размер Flash памяти:\t%d байт\n", ESP.getFlashChipSize());
  Serial.printf("Частота Flash памяти:\t%d Гц\n", ESP.getFlashChipSpeed());
  Serial.printf("Объём свободной памяти:\t%d байт\n", ESP.getFreeHeap());
  Serial.printf("Общий объём памяти:\t%d байт\n", ESP.getHeapSize());
  Serial.printf("Размер самого большого блока свободной памяти:\t%d байт\n", ESP.getMaxAllocHeap());
  Serial.printf("Наименьший объём свободной памяти с начала загрузки:\t%d байт\n", ESP.getMinFreeHeap());
}

void CheckSlow() {
  if (internet_flag == false) return;
  if (millis() - slow_timer > 2000) {
    slow_timer = millis();
    if (slow > 5) {
      title = F("Низкая скорость связи.\nТрансляция прервана.\nПодождите...");
      slowk = true;
      update_timer = millis();
      update_flag = true;
      stop_song_Play();
      slow = 0;
      retry_flag = true;
      retry_timer = millis();
      return;
    }
    if (slow > 0) slow--;
  }
}

void CheckVolume() {
  if (mute_flag == true) {
    stop_song_Play();
    mute_flag = false;
  } else {
    //Автоматическая регулировка уровня громкости
    if (aru == true) {
      //Serial.print(pik_volume);
      c_volume = (c_volume + map(audio.getVUlevel(), 0, 50000, 0, 21)) / 2;
      if (c_volume > pik_volume) {
        pik_volume = c_volume;
        vk = 0;
        if (pik_volume > middle_volume) {
          vk = 0 - int(trunc(pik_volume - middle_volume) / ((22 / c_volume + 3) / 2));
        }
        if (pik_volume < middle_volume) {
          vk = int(trunc(middle_volume - pik_volume) / ((25 / c_volume + 3) / 2));
        }
        aru_change = true;
      }
    }
  }
  if (!audio.isRunning()) maxVUlevel = 100000;
  aruVolume();
}

// Плавное выравнивание громкости под установленный уровень
void aruVolume() {
  if (millis() - aru_timer > 250) {
    aru_timer = millis();
    int current_volume = audio.getVolume();
    if (volume <= -vk) vk = -volume / 4;
    if (volume == 0) vk = 0;
    if (current_volume > (volume + vk)) audio.setVolume(current_volume - 1, 1);
    if (current_volume < (volume + vk)) audio.setVolume(current_volume + 1, 1);
  }
}

void retryControl() {
  if (retry_flag == true && fix_mode == 0) {
    if (millis() - retry_timer > 5000) {  // 20 сек.
      retry_timer = millis();
      retry_flag = false;
      Serial.println(F("Пытaюсь возобновить трансляцию..."));
      //Флаг запуска ReplayStation()
      replay_station_flag = true;
      pause_flag = false;
    }
  }
}

void CheckConnection() {
  if (WiFi.status() != WL_CONNECTED && internet_flag == true) {
    title = F("Wi-fi соединение потеряно.\nПерезагрузите устройство\nили дождитесь\nвосстановления связи.");
    Serial.println(title);
    internet_flag = false;
    update_flag = true;
  }
  if (WiFi.status() == WL_CONNECTED && internet_flag == false) {
    title = F("Wi-fi соединение восстановлено.");
    Serial.println(title);
    internet_flag = true;
    update_flag = true;
    if (fix_mode == 0 || radio_mode == 0) replay_station_flag = true;
  }
}

void CheckRadiometer() {
  /*
  if (millis() - radiometer > 10 * 60000) {  // 10 минут
    radiometer = millis();
    File rad_file;
    int s_radiometer;
    if (SD.exists("/Radio/radiometer.info")) {
      rad_file = SD.open("/Radio/radiometer.info", FILE_READ);
      //String rs = rad_file.readStringUntil('\n');
      String rs = rad_file.read();
      s_radiometer = rs.toInt();
      rad_file.close();
    }
    s_radiometer += audio.getTotalPlayingTime() / 1000;
    rad_file = SD.open("/Radio/radiometer.info", FILE_WRITE);

  }
  */
}

void stop_song_Play() {
  int ssp;
  audio.setVolume(0);
  vTaskDelay(100);
  if (audio.isRunning()) audio.stopSong();
  while (audio.isRunning()) {
    esp_task_wdt_reset();
    vTaskDelay(1);
    if (ssp > 2000) break;
    ssp++;
    Serial.print("-");
  }
}

void totalHours() {
  if (millis() - total_timer > 3600000) {  // 1 час
    total_timer = millis();
    total_hours++;
    int eget;
    EEPROM.begin(512);
    vTaskDelay(100);
    EEPROM.get(0, eget);
    if (eget != total_hours) EEPROM.put(0, total_hours);
    EEPROM.commit();
    EEPROM.end();
  }
}

void CheckLed() {
  if (set_led != fix_led && millis() - led_timer1 > 100) {
    if (set_led > fix_led) {
      fix_led++;
    } else {
      fix_led--;
    }
    analogWrite(TFT_LED, 255 - 10 * fix_led);
    led_timer1 = millis();
    return;
  }

  if (millis() - led_timer > led_time * 60000 && led_time > 0 && led_flag == false) {
    set_led = 0;
    led_flag = true;
  }
  if (cur_led != save_led && led_flag == false) {
    save_led = cur_led;
    set_led = cur_led;
  }
}
void initOTA() {
  ArduinoOTA.setHostname("InternetRadio");
  // ArduinoOTA.setPassword("admin");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else  // U_SPIFFS
      type = "filesystem";
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    esp_task_wdt_reset();
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}
void notfound() {
  slows = 0;
  slowk = true;
  if (favorites_flag == favorites_flag4) {
    if (favorites_flag) {
      audio.setVolume(0);
      radio_station_count = DelStationFromFav();
      cur_radio_location = 0;
      cur_radio_type = 0;
      if (cur_radio_station > radio_station_count) cur_radio_station = radio_station_count;
      //clearInfo();
      radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
    } else {
      DelStation();
      !favorites_flag;
      DelStation();
      !favorites_flag;
      radio_station_count--;
      if (cur_radio_station > 0) {
        clearInfo();
        radio_station = LoadRadioStation(cur_radio_station, cur_radio_type, cur_radio_location);
      }
    }
    if (cur_radio_station < cur_radio_station2) cur_radio_station2 = cur_radio_station2--;
    if (cur_radio_station < cur_radio_station3) cur_radio_station3 = cur_radio_station3--;
  }
}
void notend() {
  slows = 0;
  slowk = true;
  if (autoscan) {
    if (cur_radio_station != cur_radio_station2 && cur_radio_station != cur_radio_station3) cur_radio_station = cur_radio_station2;
    else if (cur_radio_station != cur_radio_station3) cur_radio_station = cur_radio_station3;
    else {
      cur_radio_station = cur_radio_station + 3;
      cur_radio_station2 = cur_radio_station2 + 3;
      cur_radio_station3 = cur_radio_station3 + 3;
    }
  }

  if (cur_radio_station > radio_station_count) cur_radio_station = 1;
  if (cur_radio_station < 1) cur_radio_station = radio_station_count;

  if (autoscan) PlayStation();
  else {
    pre_radio_station = cur_radio_station;
    play_station_flag = true;
    eeprom_flag = true;
  }
  update_flag = true;
}