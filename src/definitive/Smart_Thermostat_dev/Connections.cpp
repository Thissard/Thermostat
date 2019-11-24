#include "Connections.h"

Connections::Connections(char* ssid, char* pass, IPAddress mqtt_broker){
  this->_ssid = ssid;
  this->_pass = pass;
//  this->_mqtt_broker = mqtt_broker;
}

Connections::Connections(char* ssid, char* pass, IPAddress ip_address, IPAddress dns_address, IPAddress net_mask, IPAddress ip_gateway, IPAddress mqtt_broker){
  this->_ssid = ssid;
  this->_pass = pass;
  this->_ip_address = ip_address;
  this->_dns_address = dns_address;
  this->_net_mask = net_mask;
  this->_ip_gateway = ip_gateway;
//  this->_mqtt_broker = mqtt_broker;
}

void Connections::begin(void){
  if (this->_ip_address != NULL && this->_dns_address != NULL && this->_net_mask != NULL && this->_ip_gateway != NULL)
    WiFi.config(_ip_address, _dns_address, _net_mask, _ip_gateway);
  WiFi.begin(_ssid, _pass);
  
//  WiFiClient espClient;
//  _mqtt_client = new PubSubClient(espClient);
//  _mqtt_client->setServer(_mqtt_broker, 1883);
}
/*
boolean Connections::publish(const char* topic, const char* payload){
  return _mqtt_client->publish(topic,payload);
}
boolean Connections::publish(const char* topic, const char* payload, boolean retained){
  return _mqtt_client->publish(topic,payload,retained);
}
boolean Connections::publish(const char* topic, const uint8_t * payload, unsigned int plength) {
  return _mqtt_client->publish(topic,payload,plength);
}
boolean Connections::publish(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained){
  return _mqtt_client->publish(topic,payload,plength,retained);
}
boolean Connections::subscribe(const char* topic){
  return _mqtt_client->subscribe(topic);
}
boolean Connections::subscribe(const char* topic, uint8_t qos){
  return _mqtt_client->subscribe(topic,qos);
}
boolean Connections::unsubscribe(const char* topic){
  return _mqtt_client->unsubscribe(topic);
}

void Connections::setCallback(MQTT_CALLBACK_SIGNATURE) {
 _mqtt_client->setCallback(callback);
}
*/
int Connections::connectionStatus(void){
  return WiFi.status();
}

IPAddress Connections::myIP(void){
  return WiFi.localIP();
}

time_t Connections::NTPUpdateSystemTime(void){
  time_t t = 0;
  if (_NTPServerName != NULL)
    t = getNTPTime();
  setTime(t);
  return t;
}

void Connections::NTPBegin(unsigned int localPort){
  Udp.begin(localPort);
}

void Connections::NTPSetServerName( char* NTPServerName){
  this->_NTPServerName = NTPServerName;
}

int Connections::DTSOffset (unsigned long unixTime)
{
  //Receives unix epoch time and returns GMT offset for local DST
  //Valid thru 2099 for IT only, Calculations from "http://www.webexhibits.org/daylightsaving/i.html"
  //DST update wont be reflected until the next time sync
  time_t t = unixTime;
  int beginDSTDay = (14 - (1 + year(t) * 5 / 4) % 7); 
  int beginDSTMonth=3;
  int endDSTDay = (7 - (1 + year(t) * 5 / 4) % 7);
  int endDSTMonth=11;
  if (((month(t) > beginDSTMonth) && (month(t) < endDSTMonth))
    || ((month(t) == beginDSTMonth) && (day(t) > beginDSTDay))
    || ((month(t) == beginDSTMonth) && (day(t) == beginDSTDay) && (hour(t) >= 2))
    || ((month(t) == endDSTMonth) && (day(t) < endDSTDay))
    || ((month(t) == endDSTMonth) && (day(t) == endDSTDay) && (hour(t) < 1)))
    return (2); //GMT+2
  else
    return (1); //GMT+1
}

void Connections::sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t Connections::getNTPTime(){
  IPAddress ntpServerIP; // NTP server's ip address
  while (Udp.parsePacket() > 0) ; // discard any previously received packets

  // get a random server from the pool
  WiFi.hostByName(_NTPServerName, ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 2500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + DTSOffset(secsSince1900) * SECS_PER_HOUR;
    }
  }
  return 0; // return 0 if unable to get the time
}

