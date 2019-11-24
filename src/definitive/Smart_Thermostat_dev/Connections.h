#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//#include <PubSubClient.h>


// NTP
#define NTP_PACKET_SIZE 48

class Connections{
  public:
    Connections(char* ssid, char* pass, IPAddress mqtt_broker);  //costruttore per DHCP
    Connections(char* ssid, char* pass, IPAddress ip_address,IPAddress dns_address, IPAddress net_mask, IPAddress ip_gateway, IPAddress mqtt_broker); //costruttore con indirizzo statico
    void begin(void); //da lanciare al setup
    void loop(void); //da lanciare in loop
    int connectionStatus(void); //restituisce lo stato della connessione

    //metodi NTP
    time_t NTPUpdateSystemTime(void); //setta data e ora dell'ESP
    void NTPSetServerName(char* NTPserverName);// setta NTP server
    void NTPBegin(unsigned int localPort); //da lanciare a connessione avvenuta
    IPAddress myIP(void);

    //metodi MQTT
//    void MQTTBegin(const char* id);
//    void MQTTBegin(const char* id, const char* user, const char* pass);
//    void MQTTBegin(const char* id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
//    void MQTTBegin(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
//    boolean publish(const char* topic, const char* payload);
//    boolean publish(const char* topic, const char* payload, boolean retained);
//    boolean publish(const char* topic, const uint8_t * payload, unsigned int plength);
//    boolean publish(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);
//    boolean subscribe(const char* topic);
//    boolean subscribe(const char* topic, uint8_t qos);
//    boolean unsubscribe(const char* topic);
//    void setCallback(MQTT_CALLBACK_SIGNATURE);
    
  private:
    char* _ssid;
    char* _pass;
    char* _NTPServerName;
    IPAddress _ip_address;
    IPAddress _dns_address;
    IPAddress _net_mask;
    IPAddress _ip_gateway;
    
    WiFiUDP Udp;
    byte packetBuffer[NTP_PACKET_SIZE];
    time_t getNTPTime();
    void sendNTPpacket(IPAddress &address);
    int DTSOffset (unsigned long unixTime);

//    IPAddress _mqtt_broker;
//    PubSubClient* _mqtt_client;
    
};

