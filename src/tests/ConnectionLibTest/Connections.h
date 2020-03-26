#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// NTP
#define NTP_PACKET_SIZE 48

class Connections{
  public:
    Connections(void);  //costruttore per DHCP
    void begin(String ssid, String pass); //da lanciare al setup
    void begin(String ssid, String pass, String ip_address,String dns_address, String net_mask, String ip_gateway); //costruttore con indirizzo statico
    int connectionStatus(void); //restituisce lo stato della connessione
    time_t NTPUpdateSystemTime(void); //setta data e ora dell'ESP
    void NTPSetServerName(String NTPserverName);// setta NTP server
    void NTPBegin(unsigned int localPort); //da lanciare a connessione avvenuta
    IPAddress myIP(void);
    bool isValid( IPAddress ip);
  private:
    String _ssid;
    String _pass;
    String _NTPServerName;
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
