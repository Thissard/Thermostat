

// NECESSARIO SCARICARE LIBRERIE
// BUTTON da https://github.com/JChristensen/Button
// ESPRotary da https://github.com/lennarthennigs/ESPRotary/
// non presenti nell'elenco delle librerie standard di Arduino IDE

#include <ESPRotary.h>

class UserCommands{
  public:
    UserCommands(uint8_t a_pin, uint8_t b_pin, uint8_t pushbutton_pin);  //COSTRUTTORE CLASSE
    
    void begin(void);  //DA USARE NEL SETUP 
    void update(void); //DA USARE NEL LOOP
    
    // I SEGUENTI METODI RESTITUISCONO TRUE SE E' STATA ESEGUITA L'AZIONE.
    // IL METODO UNA VOLTA USATO CANCELLA L'AZIONE ESEGUITA QUINDI
    // E' NECESSARIO PASSARE DA UN'ALTRO UPDATE PER VERIFICARE LE AZIONI DI NUOVO.
    
    bool turnedLeft(void); 
    bool turnedRight(void);
    bool buttonWasPressed(void);
  private:
    uint8_t _a;
    uint8_t _b;
    uint8_t _push;
    uint8_t _sensibility;
    bool _turnedLeft;
    bool _turnedRight;
    bool _buttonState;
    bool _buttonLastState;
    bool _bChanged;
    bool _buttonPressed;
    uint32_t _bLastChange;  // time of last state change (ms)
    uint32_t _bDebounce; //debounce time
    ESPRotary *_rotary;
    void rotate(void);
    
};

