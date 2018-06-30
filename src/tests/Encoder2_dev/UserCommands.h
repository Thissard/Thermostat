

// NECESSARIO SCARICARE LIBRERIE
// BUTTON da https://github.com/JChristensen/Button
// ESPRotary da https://github.com/lennarthennigs/ESPRotary/
// non presenti nell'elenco delle librerie standard
#include <JC_Button.h>
#include <ESPRotary.h>

class UserCommands{
  public:
    UserCommands(uint8_t a_pin, uint8_t b_pin, uint8_t pushbutton_pin);
    void begin(void);
    void update(void);
    bool turnedLeft(void);
    bool turnedRight(void);
    bool buttonPressed(void);
  private:
    uint8_t _a;
    uint8_t _b;
    uint8_t _push;
    uint8_t _sensibility;
    bool _turnedLeft;
    bool _turnedRight;
    ESPRotary *_rotary;
    Button *_button;
    void rotate(void);
    
};

