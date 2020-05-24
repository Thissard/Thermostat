#include "Display.h"
#include "Drop16p.h"

uint8_t prev_conn_status = -1;
uint8_t prev_day = -1;
uint8_t prev_hour = -1;
uint8_t prev_min = -1;
uint8_t prev_int_temp = -1;
uint8_t prev_dec_temp = -1;
float prev_humidity = -1;
float prev_temperature_setpoint = -1;
uint8_t prev_index = -1;
uint8_t prev_brightness = -1;
uint8_t prev_day_index = -1;
uint8_t prev_navigation_index = -1;
uint8_t prev_hour_index = -1;
uint8_t prev_temperature_index = -1;
uint8_t prev_setpoint_index = -1;

SQUARE_COLOR square_colors;

int16_t getColorFromConfig(int day_index, int hour_index, CONFIG configuration)
{
  int setpoint_index;
  switch (day_index)
  {
  case DOMENICA: //SUNDAY
    setpoint_index = configuration.chrono.calendar.DOM[hour_index];
    break;
  case LUNEDI: //MONDAY
    setpoint_index = configuration.chrono.calendar.LUN[hour_index];
    break;
  case MARTEDI: //TUESDAY
    setpoint_index = configuration.chrono.calendar.MAR[hour_index];
    break;
  case MERCOLEDI: //WEDNESDAY
    setpoint_index = configuration.chrono.calendar.MER[hour_index];
    break;
  case GIOVEDI: //THURSDAY
    setpoint_index = configuration.chrono.calendar.GIO[hour_index];
    break;
  case VENERDI: //FRIDAY
    setpoint_index = configuration.chrono.calendar.VEN[hour_index];
    break;
  case SABATO: //SATURDAY
    setpoint_index = configuration.chrono.calendar.SAB[hour_index];
    break;
  }
  switch (setpoint_index)
  {
  case OFF:
    return square_colors.OFF;
    break;
  case ECO:
    return square_colors.ECO;
    break;
  case NORMAL:
    return square_colors.NORMAL;
    break;
  case COMFORT:
    return square_colors.COMFORT;
    break;
  case COMFORT_PLUS:
    return square_colors.COMFORT_PLUS;
    break;
  }
}

Display::Display(int8_t led, int8_t cs, int8_t dc, int8_t sdi_mosi, int8_t sck)
{
  this->_cs_pin = cs;
  this->_dc_pin = dc;
  this->_sdi_mosi_pin = sdi_mosi;
  this->_sck_pin = sck;
  this->_led_pin = led;
}

void Display::begin(void)
{
  //check if required software SPI interface
  if (this->_sdi_mosi_pin == -1 or this->_sck_pin == -1)
    tft = new Adafruit_ILI9341(_cs_pin, _dc_pin);
  else
    tft = new Adafruit_ILI9341(_cs_pin, _dc_pin, _sdi_mosi_pin, _sck_pin);
  tft->begin();
  clearScreen();
  tft->setCursor(0, 0);
  tft->setRotation(3);

  pinMode(_led_pin, OUTPUT);
  analogWrite(_led_pin, 1024);
  setBacklight(100);
}

void Display::clearScreen(void)
{
  prev_conn_status = -1;
  prev_day = -1;
  prev_min = -1;
  prev_int_temp = -1;
  prev_humidity = -1;
  prev_temperature_setpoint = -1;
  prev_index = -1;
  prev_brightness = -1;
  prev_hour = -1;
  prev_day_index = -1;
  prev_navigation_index = -1;
  prev_hour_index = -1;
  prev_temperature_index = -1;
  prev_setpoint_index = -1;
  tft->fillScreen(ILI9341_BLACK);
}

void Display::setBacklight(int backlight)
{
  analogWrite(_led_pin, backlight * 1024 / 100);
}

void Display::showSplashScreen(String project_version)
{
  tft->fillScreen(0x4A5A);
  tft->setFont();
  tft->setCursor(102, 80);
  tft->setTextSize(4);
  tft->println("MQTT");
  tft->setFont();
  tft->setCursor(60, 130);
  tft->setTextSize(2);
  tft->println("Smart Thermostat");
  tft->setFont();
  tft->setCursor(115, 180);
  tft->print(project_version);
}

void Display::showMainScreen(float temperature, float humidity, uint8_t connection, String IP, CONFIG configuration)
{
  //IP
  tft->setFont();
  if (prev_conn_status != connection)
  {
    tft->fillRect(0, 224, 200, 20, ILI9341_BLACK);
    tft->setCursor(0, 224);
    tft->setTextSize(1);
    tft->setTextColor(ILI9341_WHITE);
    if (connection == 3)
    { //WL_CONNECTED = 3
      tft->println("CONNESSO");
      tft->print("IP:");
      tft->print(IP);
    }
    else
      tft->println("DISCONNESSO");
    prev_conn_status = connection;
  }

  //CHRONO TEMPERATURES SETPOINTS
  tft->setFont();
  tft->setTextSize(1);

  tft->fillRect(226, 59, 11, 11, square_colors.ECO);
  tft->setCursor(240, 62);
  tft->print("Eco      ");
  tft->printf("%.1f", configuration.chrono.setpoints.eco);

  tft->fillRect(226, 73, 11, 11, square_colors.NORMAL);
  tft->setCursor(240, 76);
  tft->print("Normal   ");
  tft->printf("%.1f", configuration.chrono.setpoints.normal);

  tft->fillRect(226, 87, 11, 11, square_colors.COMFORT);
  tft->setCursor(240, 90);
  tft->print("Comfort  ");
  tft->printf("%.1f", configuration.chrono.setpoints.comfort);

  tft->fillRect(226, 101, 11, 11, square_colors.COMFORT_PLUS);
  tft->setCursor(240, 104);
  tft->print("Comfort+ ");
  tft->printf("%.1f", configuration.chrono.setpoints.comfort_p);

  //Draw Chrono
  word start_x = 6;
  word start_y = 180;
  word space = 13;

  word x_i = start_x;
  word l_y = start_y + space;
  word notch;

  //clear chrono progression
  if (prev_hour != hour())
  {
    tft->fillRect(0, 173, 3207, 20, ILI9341_BLACK);
    prev_hour = hour();

    for (int i = 0; i <= 24; i++)
    {
      //Draw Squares

      if (i < 24) //draw only 23 squares
      {
        uint16_t color_config = getColorFromConfig(weekday(), i, configuration);
        if (i <= hour())
        {
          tft->fillRect(x_i, start_y - 7, 11, 5, ILI9341_WHITE);
        }
        tft->fillRect(x_i, start_y, 11, 11, color_config);
      }

      //draw 24 notches
      if (i == 0 || i == 12 || i == 24)
        notch = ILI9341_RED;
      else
        notch = ILI9341_WHITE;
      tft->fillRect(x_i - 2, start_y + space, 2, 6, notch);

      //increase spacer for squares
      x_i = x_i + space;
    }
  }

  //numeri
  tft->setFont();
  tft->setTextSize(1);
  tft->setCursor(12, start_y + space + 8);
  tft->print("01");
  tft->setCursor(38, start_y + space + 8);
  tft->print("03");
  tft->setCursor(64, start_y + space + 8);
  tft->print("05");
  tft->setCursor(90, start_y + space + 8);
  tft->print("07");
  tft->setCursor(116, start_y + space + 8);
  tft->print("09");
  tft->setCursor(142, start_y + space + 8);
  tft->print("11");
  tft->setCursor(168, start_y + space + 8);
  tft->print("13");
  tft->setCursor(194, start_y + space + 8);
  tft->print("15");
  tft->setCursor(220, start_y + space + 8);
  tft->print("17");
  tft->setCursor(246, start_y + space + 8);
  tft->print("19");
  tft->setCursor(272, start_y + space + 8);
  tft->print("21");
  tft->setCursor(298, start_y + space + 8);
  tft->print("23");

  //DATE
  if (prev_day != day())
  {
    tft->setFont();
    tft->fillRect(195, 224, 125, 20, ILI9341_BLACK);
    tft->setCursor(195, 224);
    tft->setTextColor(ILI9341_WHITE);
    tft->setTextSize(2);
    tft->printf("%02d", day());
    tft->print("/");
    tft->printf("%02d", month());
    tft->print("/");
    tft->print(year());
    prev_day = day();
  }
  //TIME
  if (prev_min != minute())
  {
    tft->setFont();
    tft->setTextSize(2);
    tft->fillRect(255, 2, 125, 20, ILI9341_BLACK);
    tft->setCursor(255, 2);
    tft->printf("%02d", hour());
    tft->print(":");
    tft->printf("%02d", minute());
    prev_min = minute();
  }

  //TEMPERATURE
  int int_temperature = temperature;
  int dec_temperature = (temperature * 10) - (int_temperature * 10);
  if (prev_int_temp != int_temperature || prev_dec_temp != dec_temperature)
  {
    tft->fillRect(5, 50, 150, 80, ILI9341_BLACK);
    tft->setFont(&Gameplay20pt7b);
    tft->setCursor(5, 125);
    tft->setTextColor(ILI9341_WHITE);
    tft->setTextSize(2);
    tft->print(int_temperature);
    tft->setCursor(125, 125);
    tft->setTextSize(1);
    tft->print(dec_temperature);
    prev_int_temp = int_temperature;
    prev_dec_temp = dec_temperature;
  }

  //HUMIDITY
  if (prev_humidity != humidity)
  {
    tft->drawBitmap(10, 143, Drop16p, 16, 16, ILI9341_WHITE);
    tft->fillRect(30, 135, 100, 25, ILI9341_BLACK);
    tft->setCursor(30, 150);
    tft->setFont(&Classic8pt7b);
    tft->setTextSize(1);
    tft->printf("%2.1f", humidity);
    tft->print("%");
    prev_humidity = humidity;
  }
}

void Display::showMenuScreen(int selection)
{
  tft->setFont();
  tft->setTextSize(2);
  //arrow based on index
  if (prev_index != selection)
  {
    tft->fillRect(10, 50, 22, 100, ILI9341_BLACK);
    tft->setCursor(10, 60 + 20 * selection);
    tft->print("->");
    prev_index = selection;
  }
  tft->setCursor(30, 60);
  tft->print(" - PROGRAMMAZIONE");
  tft->setCursor(30, 80);
  tft->print(" - TEMPERATURE");
  tft->setCursor(30, 100);
  tft->print(" - LUMINOSITA'");
  tft->setCursor(30, 120);
  tft->print(" - INDIETRO");
}

void Display::showBrightness(int selection)
{

  if (prev_brightness != selection)
  {
    tft->setFont();
    tft->setTextSize(4);
    tft->setCursor(20, 60);
    tft->print("LUMINOSITA':");
    tft->fillRect(100, 110, 110, 50, ILI9341_BLACK);
    tft->setCursor(110, 120);
    tft->print(selection);
    tft->print("%");
    //int i=0;
    for (int i = 0; i < 10; i++)
      tft->fillRect(32 * i, 200, 32, 32, tft->color565(25 * i, 25 * i, 25 * i));
  }
}

void Display::showProgrammation(int navigation_index, int day_index, int hour_index, int temperature_index, CONFIG configuration)
{
  //DAY
  tft->setFont();
  tft->setTextSize(2);
  tft->setCursor(64, 30);
  if (navigation_index == CHOOSE_DAY)
  {
    tft->print("-> "); //selectd based on navigation_index
  }
  else
  {
    tft->fillRect(64, 0, 36, 60, ILI9341_BLACK);
    tft->print("   ");
  }
  if (day_index != prev_day_index)
  {
    //UPDATE DAY
    //clear day area
    tft->fillRect(100, 0, 200, 60, ILI9341_BLACK);
    //clear chrono bar
    tft->fillRect(0, 180, 320, 60, ILI9341_BLACK);
    switch (day_index)
    {
    case LUNEDI:
      tft->print("LUNEDI'"); //selectd based on day_index
      break;
    case MARTEDI:
      tft->print("MARTEDI'"); //selectd based on day_index
      break;
    case MERCOLEDI:
      tft->print("MERCOLEDI'"); //selectd based on day_index
      break;
    case GIOVEDI:
      tft->print("GIOVEDI'"); //selectd based on day_index
      break;
    case VENERDI:
      tft->print("VENERDI'"); //selectd based on day_index
      break;
    case SABATO:
      tft->print("SABATO"); //selectd based on day_index
      break;
    case DOMENICA:
      tft->print("DOMENICA"); //selectd based on day_index
      break;
    case NO_DAY:
      tft->print("TORNA INDIETRO"); //selectd based on day_index
      break;
    default:
      break;
    }
    prev_day_index = day_index;
  }

  //HOUR
  if (navigation_index >= CHOOSE_TIME)
  {
    tft->setFont();
    tft->setTextSize(2);
    tft->setCursor(64, 90);
    if (navigation_index == CHOOSE_TIME)
    {
      tft->print("-> ");
    }
    else
    {
      tft->fillRect(64, 60, 36, 60, ILI9341_BLACK);
      tft->print("   ");
    }

    if (hour_index != prev_hour_index)
    {
      //clear hour area
      tft->fillRect(100, 60, 200, 60, ILI9341_BLACK);
      prev_hour_index = hour_index;
      //clear chrono bar
      tft->fillRect(0, 180, 320, 60, ILI9341_BLACK);

      if (hour_index < 24)
      {
        if (hour_index < 10)
          tft->print("0");
        tft->print(String(hour_index, DEC) + "-");
        if (hour_index < 9)
          tft->print("0");
        tft->print(String(hour_index + 1, DEC)); //selectd based on hours_index
      }
      else
      {
        tft->print("TORNA INDIETRO");
      }
    }
  }
  else
  {
    tft->fillRect(0, 60, 320, 60, ILI9341_BLACK);
  }

  if (navigation_index >= CHOOSE_TEMPERATURE)
  {
    //CHRONO TEMPERATURES SETPOINTS
    tft->setFont();
    tft->setTextSize(2);
    tft->setCursor(64, 150);
    tft->print("-> ");

    if (temperature_index != prev_temperature_index)
    {
      //clear temperature area
      tft->fillRect(100, 120, 200, 60, ILI9341_BLACK);
      //clear chrono bar
      tft->fillRect(0, 180, 320, 60, ILI9341_BLACK);
      tft->setCursor(114, 150);
      switch (temperature_index)
      {
      case OFF:
        tft->print("OFF");
        tft->fillRect(94, 149, 15, 15, square_colors.OFF);
        break;
      case ECO:
        tft->print("ECO");
        tft->fillRect(94, 149, 15, 15, square_colors.ECO);
        break;
      case NORMAL:
        tft->print("NORMAL");
        tft->fillRect(94, 149, 15, 15, square_colors.NORMAL);
        break;
      case COMFORT:
        tft->print("COMFORT");
        tft->fillRect(94, 149, 15, 15, square_colors.COMFORT);
        break;
      case COMFORT_PLUS:
        tft->print("COMFORT+");
        tft->fillRect(94, 149, 15, 15, square_colors.COMFORT_PLUS);
        break;
      default:
        break;
      }
      prev_temperature_index = temperature_index;
    }
  }
  else
  {
    prev_temperature_index = -1;
    tft->fillRect(0, 120, 320, 60, ILI9341_BLACK);
  }

  //CHRONO VIEW
  word start_x = 6;
  word start_y = 205;
  word space = 13;
  word x_i = start_x;
  word l_y = start_y + space;
  word notch;

  for (int i = 0; i <= 24; i++)
  {
    //Draw Squares
    if (i < 24) //draw only 23 squares
    {
      uint16_t color_config = getColorFromConfig(day_index, i, configuration);
      if (i == hour_index)
      {
        tft->fillRect(x_i, start_y - 7, 11, 5, ILI9341_WHITE);
      }
      tft->fillRect(x_i, start_y, 11, 11, color_config);
    }

    //draw 24 notches
    if (i == 0 || i == 12 || i == 24)
      notch = ILI9341_RED;
    else
      notch = ILI9341_WHITE;
    tft->fillRect(x_i - 2, start_y + space, 2, 6, notch);

    //increase spacer for squares
    x_i = x_i + space;
  }

  //numeri
  tft->setFont();
  tft->setTextSize(1);
  tft->setCursor(12, start_y + space + 8);
  tft->print("01");
  tft->setCursor(38, start_y + space + 8);
  tft->print("03");
  tft->setCursor(64, start_y + space + 8);
  tft->print("05");
  tft->setCursor(90, start_y + space + 8);
  tft->print("07");
  tft->setCursor(116, start_y + space + 8);
  tft->print("09");
  tft->setCursor(142, start_y + space + 8);
  tft->print("11");
  tft->setCursor(168, start_y + space + 8);
  tft->print("13");
  tft->setCursor(194, start_y + space + 8);
  tft->print("15");
  tft->setCursor(220, start_y + space + 8);
  tft->print("17");
  tft->setCursor(246, start_y + space + 8);
  tft->print("19");
  tft->setCursor(272, start_y + space + 8);
  tft->print("21");
  tft->setCursor(298, start_y + space + 8);
  tft->print("23");
}

void Display::showTempSetpointsSettings(int navigation_index, int setpoint_index, float temperature, CONFIG configuration)
{
  if (navigation_index != prev_navigation_index)
  {
    tft->fillRect(44, 35, 22, 180, ILI9341_BLACK);
    prev_navigation_index = navigation_index;
    prev_temperature_setpoint = -1;
  }

  if (setpoint_index != prev_setpoint_index)
  {
    tft->fillRect(212, 35, 22, 145, ILI9341_BLACK);
    prev_setpoint_index = navigation_index;
  }

  if (temperature != prev_temperature_setpoint)
  {
    tft->fillRect(236, 35, 70, 145, ILI9341_BLACK);
    prev_temperature_setpoint = temperature;
  }

  switch (navigation_index)
  {
  case ECO:
    tft->setCursor(44, 40);
    tft->print("-> ");
  case NORMAL:
    tft->setCursor(44, 80);
    tft->print("-> ");
    break;
  case COMFORT:
    tft->setCursor(44, 120);
    tft->print("-> ");
    break;
  case COMFORT_PLUS:
    tft->setCursor(44, 160);
    tft->print("-> ");
    break;
  case OFF:
    tft->setCursor(44, 200);
    tft->print("-> ");
    break;
  default:
    break;
  }

  switch (setpoint_index)
  {
  case ECO:
    tft->setCursor(212, 40);
    tft->print("->");
  case NORMAL:
    tft->setCursor(212, 80);
    tft->print("->");
    break;
  case COMFORT:
    tft->setCursor(212, 120);
    tft->print("->");
    break;
  case COMFORT_PLUS:
    tft->setCursor(212, 160);
    tft->print("->");
    break;
  default:
    break;
  }

  //SETPOINTS
  tft->setFont();
  tft->setTextSize(2);

  tft->fillRect(71, 39, 15, 15, square_colors.ECO);
  tft->setCursor(90, 40);
  tft->print("ECO");
  tft->setCursor(200, 40);
  tft->print(":");
  tft->setCursor(236, 40);
  tft->printf("%.1f", configuration.chrono.setpoints.eco);
  tft->print(" C");
  tft->setTextSize(1);
  tft->setCursor(287, 38);
  tft->print("o");

  tft->setTextSize(2);
  tft->fillRect(71, 79, 15, 15, square_colors.NORMAL);
  tft->setCursor(90, 80);
  tft->print("NORMAL");
  tft->setCursor(200, 80);
  tft->print(":");
  tft->setCursor(236, 80);
  tft->printf("%.1f", configuration.chrono.setpoints.normal);
  tft->print(" C");
  tft->setTextSize(1);
  tft->setCursor(287, 78);
  tft->print("o");

  tft->setTextSize(2);
  tft->fillRect(71, 119, 15, 15, square_colors.COMFORT);
  tft->setCursor(90, 120);
  tft->print("COMFORT");
  tft->setCursor(200, 120);
  tft->print(":");
  tft->setCursor(236, 120);
  tft->printf("%.1f", configuration.chrono.setpoints.comfort);
  tft->print(" C");
  tft->setTextSize(1);
  tft->setCursor(287, 118);
  tft->print("o");

  tft->setTextSize(2);
  tft->fillRect(71, 159, 15, 15, square_colors.COMFORT_PLUS);
  tft->setCursor(90, 160);
  tft->print("COMFORT+");
  tft->setCursor(200, 160);
  tft->print(":");
  tft->setCursor(236, 160);
  tft->printf("%.1f", configuration.chrono.setpoints.comfort_p);
  tft->print(" C");
  tft->setTextSize(1);
  tft->setCursor(287, 158);
  tft->print("o");

  tft->setTextSize(2);
  tft->setCursor(90, 200);
  tft->print("INDIETRO");
}