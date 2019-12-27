#ifndef CONFIG_H
  #define CONFIG_H

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //MACHINE STATES
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  enum SCREENS{
    SPLASH_SCREEN,
    MAIN_SCREEN,
    MENU_SCREEN,
    BRIGHTNESS_SCREEN,
    CHRONO_SCREEN
  };
  
  enum MENU_INDEX{
    PROGRAMS,
    BRIGHTNESS,
    BACK  //LEAVE 'BACK' AS LAST
  };
  
  void SensorTaskCallback();
  void DisplayTaskCallback();
  void ThermostatTaskCallback();
  void UserCommandsTaskCallback();
  void SerialDiagnosticCallback();
  
  int MACHINE_STATE = SPLASH_SCREEN;
  int INDEX = PROGRAMS;
  
  //////////////////////////////////////
  //WIFI
  //////////////////////////////////////

  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //SENSOR
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  #define IN_DHT D2
  
  // Uncomment the type of sensor in use:
  //#define DHTTYPE           DHT11     // DHT 11 
  #define DHTTYPE           DHT22     // DHT 22 (AM2302)
  //#define DHTTYPE           DHT21     // DHT 21 (AM2301)
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //THERMOSTAT
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  #define OUT_RELAY D0
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //ENCODER
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  #define IN_ENC_BUTTON D6
  #define IN_ENC_A D3
  #define IN_ENC_B D1
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //DISPLAY
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  #define TFT_SCLK D5
  #define TFT_DATA D7
  #define TFT_CD D4
  #define TFT_CS D8
  #define TFT_LED RX
  //#define TFT_RESET D11

#endif
