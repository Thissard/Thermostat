
enum{
  MANUAL_MODE,
  AUTOMATIC_MODE,
};

class Thermostat{
  public:
    Thermostat(unsigned char control_pin);
    Thermostat(unsigned char control_pin, unsigned char mode);
    
    void turn_heater_ON(void);
    void turn_heater_OFF(void);
  
    void set_mode(unsigned char mode);
    
    bool get_heater_state(void);
    void update(float room_temperature, float set_point);

  private:
    bool _heating;
    unsigned char _control_pin;
    unsigned char _mode;
};
