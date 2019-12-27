#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// NTP
#define NTP_PACKET_SIZE 48

class Connections{
  public:
    Connections(char* ssid, char* pass);  //costruttore per DHCP
    Connections(char* ssid, char* pass, IPAddress ip_address,IPAddress dns_address, IPAddress net_mask, IPAddress ip_gateway); //costruttore con indirizzo statico
    void begin(void); //da lanciare al setup
    int connectionStatus(void); //restituisce lo stato della connessione
    time_t NTPUpdateSystemTime(void); //setta data e ora dell'ESP
    void NTPSetServerName(char* NTPserverName);// setta NTP server
    void NTPBegin(unsigned int localPort); //da lanciare a connessione avvenuta
    IPAddress myIP(void);
    bool isValid( IPAddress ip);
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
};

