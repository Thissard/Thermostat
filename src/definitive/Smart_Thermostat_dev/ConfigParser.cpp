#include "ConfigParser.h"

String ConfigParser::loadConfiguration(void){
    SPIFFS.begin();
    configFile = SPIFFS.open("/config.json", "r");
    if (!configFile) {
        //Serial.println("Failed to open config file");
    }
    const size_t capacity = 7 * JSON_ARRAY_SIZE(24) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(4) + 2 * JSON_OBJECT_SIZE(7) + 330;
    DynamicJsonDocument doc(capacity);
    //DeserializationError error = deserializeJson(doc, configFile);
    //if (!error) deserialize(doc);
    configFile.close();
}

String ConfigParser::saveConfiguration(void){
  
}
/*
void ConfigParser::deserialize(DynamicJsonDocument doc) {
    
    JsonObject network = doc["network"];
    config.network.SSID = network["net-name"]; // "\"TIM-90267905\""
    config.network.password = network["net-pass"]; // "\"ufSzgF1bBGDlMWhhhG7OYrTC\""
    config.network.ip = network["ip"]; // "\"192.168.1.222\""
    config.network.subnet = network["sub"]; // "\"255.255.255.0\""
    config.network.dns = network["dns"]; // "\"192.168.1.1\""
    config.network.gateway = network["gateway"]; // "\"192.168.1.1\""
    config.network.NTPServerName = network["ntp-server"]; // "\"0.it.pool.ntp.org\""

    JsonObject setpoints = doc["setpoints"];
    config.chrono.setpoints.eco = setpoints["eco"]; // 18
    config.chrono.setpoints.normal = setpoints["normal"]; // 21
    config.chrono.setpoints.comfort= setpoints["comfort"]; // 23
    config.chrono.setpoints.comfort_p = setpoints["comfort+"]; // 25

    JsonObject chrono = doc["chrono"];

    JsonArray chrono_Luned_ = chrono["Luned�"];
    for (int i = 0; i < 24; i++)
    {
        config.chrono.calendar.LUN[i] = chrono_Luned_[i];
    }

    JsonArray chrono_Marted_ = chrono["Marted�"];
    for (int i = 0; i < 24; i++)
    {
        config.chrono.calendar.MAR[i] = chrono_Marted_[i];
    }

    JsonArray chrono_Mercoled_ = chrono["Mercoled�"];
    for (int i = 0; i < 24; i++)
    {
        config.chrono.calendar.MER[i] = chrono_Mercoled_[i];
    }
    
    JsonArray chrono_Gioved_ = chrono["Gioved�"];
    for (int i = 0; i < 24; i++)
    {
        config.chrono.calendar.GIO[i] = chrono_Gioved_[i];
    }

    JsonArray chrono_Venerd_ = chrono["Venerd�"];
    for (int i = 0; i < 24; i++)
    {
        config.chrono.calendar.GIO[i] = chrono_Venerd_[i];
    }

    JsonArray chrono_Sabato = chrono["Sabato"];
    for (int i = 0; i < 24; i++)
    {
        config.chrono.calendar.VEN[i] = chrono_Sabato[i];
    }

    JsonArray chrono_Domenica = chrono["Domenica"];
    for (int i = 0; i < 24; i++)
    {
        config.chrono.calendar.DOM[i] = chrono_Domenica[i];
    }

    int display_bright = doc["display"]["bright"]; // 100
}
*/
