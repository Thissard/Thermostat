#include "Display.h"
#include "Drop16p.h"

uint8_t prev_conn_status = -1;
uint8_t prev_day = -1;
uint8_t prev_hour = -1;
uint8_t prev_min = -1;
uint8_t prev_int_temp = -1;
uint8_t prev_dec_temp = -1;
float prev_humidity = -1;
uint8_t prev_index = -1;
uint8_t prev_brightness = -1;

SQUARE_COLOR square_colors;

int16_t getColorFromConfig(int hour_index, CONFIG configuration){
  int setpoint_index;
  int day_of_week = weekday();
  switch (day_of_week){
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
  switch (setpoint_index){
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

Display::Display(int8_t led, int8_t cs, int8_t dc, int8_t sdi_mosi, int8_t sck){
  this->_cs_pin = cs;
  this->_dc_pin = dc;
  this->_sdi_mosi_pin = sdi_mosi;
  this->_sck_pin = sck;
  this->_led_pin = led;
}

void Display::begin(void){
  //check if required software SPI interface
  if (this->_sdi_mosi_pin == -1 or this->_sck_pin == -1)
    tft = new Adafruit_ILI9341(_cs_pin, _dc_pin);
  else
    tft = new Adafruit_ILI9341(_cs_pin, _dc_pin, _sdi_mosi_pin, _sck_pin );
  tft->begin();
  clearScreen();
  tft->setCursor(0, 0);
  tft->setRotation(3);

  pinMode(_led_pin, OUTPUT);
  analogWrite(_led_pin, 1024);
  setBacklight(100);
}

void Display::clearScreen(void){
  prev_conn_status = -1;
  prev_day = -1;
  prev_min = -1;
  prev_int_temp = -1;
  prev_humidity = -1;
  prev_index = -1;
  prev_brightness = -1;
  prev_hour = -1;
  tft->fillScreen(ILI9341_BLACK);
}

void Display::setBacklight(int backlight){
  analogWrite(_led_pin, backlight*1024/100);
}

void Display::showSplashScreen(String project_version){
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

void Display::showMainScreen(float temperature, float humidity, uint8_t connection, String IP, CONFIG configuration){
  //IP
  tft->setFont();
  if (prev_conn_status != connection){
    tft->fillRect(0, 224, 200, 20, ILI9341_BLACK);
    tft->setCursor(0,224);
    tft->setTextSize(1);
    tft->setTextColor(ILI9341_WHITE);
    if (connection == 3){ //WL_CONNECTED = 3
      tft->println("CONNESSO");
      tft->print("IP:");
      tft->print(IP);
    }
    else
      tft->println("DISCONNESSO");
    prev_conn_status = connection;
  }

  //chrono Blu
  tft->setFont();
  tft->setTextSize(1);

  tft->fillRect(226,59,11,11, square_colors.ECO);
  tft->setCursor(240,62);
  tft->print("Eco      ");
  tft->printf("%.1f", configuration.chrono.setpoints.eco);

  tft->fillRect(226,73,11,11, square_colors.NORMAL);
  tft->setCursor(240,76);
  tft->print("Normal   ");
  tft->printf("%.1f",configuration.chrono.setpoints.normal);

  tft->fillRect(226,87,11,11, square_colors.COMFORT);
  tft->setCursor(240,90);
  tft->print("Comfort  ");
  tft->printf("%.1f",configuration.chrono.setpoints.comfort);

  tft->fillRect(226,101,11,11, square_colors.COMFORT_PLUS);
  tft->setCursor(240,104);
  tft->print("Comfort+ ");
  tft->printf("%.1f",configuration.chrono.setpoints.comfort_p);
  
  //Draw Chrono
  word start_x = 6;
  word start_y = 180;
  word space=13;

  word x_i = start_x;
  word l_y = start_y + space;
  word notch;

  //clear chrono progression
  if (prev_hour != hour()){
    tft->fillRect(0,173,3207,5, ILI9341_BLACK); 
    prev_hour = hour();
  }

  for (int i=0; i <= 24; i++){
    //Draw Squares

    if (i<24) //draw only 23 squares 
    {
      uint16_t color_config = getColorFromConfig(i, configuration);
      if (i <= hour() ){
      tft->fillRect(x_i,start_y-7,11,5, ILI9341_WHITE);
      }
      tft->fillRect(x_i,start_y,11,11, color_config);
    } 

    //draw 24 notches
    if (i ==0 || i == 12 || i == 24)
      notch = ILI9341_RED;
    else
      notch = ILI9341_WHITE;
    tft->fillRect(x_i-2,start_y + space,2,6, notch);
    
    //increase spacer for squares
    x_i=x_i+space;
   }

   //numeri
   tft->setFont();
   tft->setTextSize(1);
   tft->setCursor(12,start_y + space+8);
   tft->print("01");
   tft->setCursor(38,start_y + space+8);
   tft->print("03");
   tft->setCursor(64,start_y + space+8);
   tft->print("05");
   tft->setCursor(90,start_y + space+8);
   tft->print("07");
   tft->setCursor(116,start_y + space+8);
   tft->print("09");
   tft->setCursor(142,start_y + space+8);
   tft->print("11");
   tft->setCursor(168,start_y + space+8);
   tft->print("13");
   tft->setCursor(194,start_y + space+8);
   tft->print("15");
   tft->setCursor(220,start_y + space+8);
   tft->print("17");
   tft->setCursor(246,start_y + space+8);
   tft->print("19");
   tft->setCursor(272,start_y + space+8);
   tft->print("21");
   tft->setCursor(298,start_y + space+8);
   tft->print("23");

    //DATE
   if (prev_day != day()){
    tft->setFont();
    tft->fillRect(195, 224, 125, 20, ILI9341_BLACK);
    tft->setCursor(195,224);
    tft->setTextColor(ILI9341_WHITE);
    tft->setTextSize(2);
    tft->printf("%02d",day());
    tft->print("/");
    tft->printf("%02d",month());
    tft->print("/");
    tft->print(year());
    prev_day = day();
  }
  //TIME
  if(prev_min != minute()){
    tft->setFont();
    tft->setTextSize(2);
    tft->fillRect(255, 2, 125, 20, ILI9341_BLACK);
    tft->setCursor(255, 2);
    tft->printf("%02d",hour());
    tft->print(":");
    tft->printf("%02d",minute());
    prev_min = minute();
  }
  
  //TEMPERATURE
  int int_temperature=temperature;
  int dec_temperature=(temperature*10)-(int_temperature*10);
  if (prev_int_temp != int_temperature || prev_dec_temp != dec_temperature){
    tft->fillRect(5, 50, 150, 80, ILI9341_BLACK);
    tft->setFont(&Gameplay20pt7b);
    tft->setCursor(5,125);
    tft->setTextColor(ILI9341_WHITE);
    tft->setTextSize(2);
    tft->print(int_temperature);
    tft->setCursor(125,125);
    tft->setTextSize(1);
    tft->print(dec_temperature);
    prev_int_temp = int_temperature;
    prev_dec_temp = dec_temperature;
  }
  
  //HUMIDITY
  if (prev_humidity != humidity){
    tft->drawBitmap(10, 143, Drop16p, 16, 16, ILI9341_WHITE);
    tft->fillRect(30, 135, 100, 25, ILI9341_BLACK);
    tft->setCursor(30,150);
    tft->setFont(&Classic8pt7b);
    tft->setTextSize(1);
    tft->printf("%2.1f",humidity);
    tft->print("%");
    prev_humidity = humidity;
  }
}



void Display::showMenuScreen(int selection){
  tft->setFont();
  tft->setTextSize(2);
  //arrow based on index
  if (prev_index != selection){
    tft->fillRect(10, 50, 22, 100, ILI9341_BLACK);
    tft->setCursor(10, 60+20*selection);
    tft->print("->");
    prev_index = selection;
  }
  tft->setCursor(30,60);
  tft->print(" - PROGRAMMAZIONE");
  tft->setCursor(30,80);
  tft->print(" - LUMINOSITA'");
  tft->setCursor(30,100);
  tft->print(" - INDIETRO");
  
}

void Display::showBrightness(int selection){
  
  if (prev_brightness != selection){
    tft->setFont();
    tft->setTextSize(4);
    tft->setCursor(20,60);
    tft->print("LUMINOSITA':");
    tft->fillRect(100, 110, 110, 50, ILI9341_BLACK);
    tft->setCursor(110,120);
    tft->print(selection);
    tft->print("%");
    //int i=0;
    for (int i = 0; i<10; i++)
      tft->fillRect(32*i,200,32,32,tft->color565(25*i,25*i,25*i));
  }  
  
}
