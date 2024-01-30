#include "funshield.h"
const int buttonCount = 3;
int buttons [buttonCount] = {button1_pin, button2_pin, button3_pin};

class Button{
  private: 
    int pin;
    bool down = false;
    bool modeChanged = false;
        
  public:
    int getPin(){
       return pin;  
    }

    void changePin(int p){
        pin = p;
    }
    bool getDown(){
      return down;
    }

    void changeDown(bool n){
      down = n;
    }

    void changeVal(bool a){
              modeChanged = a;
        }

    bool getVal(){
      return modeChanged;
    }

    bool pressedOnce(){
        if(digitalRead(pin) == false && down == false) return true;        
        else return false;  
        
    }
    bool released(){
         if(digitalRead(pin) == true && down == true) return true;
         else return false;  
        
    }
 };

Button button[buttonCount];

class RandomGenerator{
   private:
      int lastRandom = 0;

   public:
      int getRandom(){
          return lastRandom; 
      }

      void generateRandom(int tr, int dType){
          lastRandom = 0;
          for(int i = 0; i < tr; i++){ 
                lastRandom += (rand() % dType + 1);
          }
      }
};

RandomGenerator randG; 
  
class DiceParam{
  private:
        //General
        bool configMode = true;
      
        //Configuration Mode
        int diceTypeArr[7] = {4, 6, 8, 10, 12, 20, 100};
        int diceIndex = 0;
        int numThrows = 1; 
        

  public:
        bool getMode(){
           return configMode;  
        }
        void changeMode(){
            configMode = !configMode;   
        }
        int getDiceType(){
            return diceTypeArr[diceIndex];  
        }

        int getNumThrows(){
            return numThrows;  
        }
        void incrementNumThrows(){
            numThrows = (numThrows + 1 + 10) % 10;
            if(numThrows == 0) numThrows+=1;   
       
        }
        void incrementDiceType(){
            diceIndex = (diceIndex + 1 + 7) % 7;         
        }

        
};

DiceParam diceP;

class Display{
    private:
        //Display 
        byte d = 0b10100001;
        byte EMPTY_GLYPH = 0b11111111;
        
        const int displaySize = 4;
        int digit_muxpo[4] = { 0x08, 0x04, 0x02, 0x01 };
        
        byte displayArr[4] = {EMPTY_GLYPH , EMPTY_GLYPH, EMPTY_GLYPH, EMPTY_GLYPH}; 
        int index = 4;  
        
    public:            
          
        void displayDigit(byte digit, byte pos){
             shiftOut(data_pin, clock_pin, MSBFIRST, digit);
             shiftOut(data_pin, clock_pin, MSBFIRST, pos);
             digitalWrite(latch_pin, ON);
             digitalWrite(latch_pin, OFF);
        
        }

        void updateConfig(int tr, int dType){
           int copyT = dType;
           displayArr[3] = digits[tr];
           displayArr[2] = d;
           if(dType < 10) {
            displayArr[1] = digits[dType];
            displayArr[0] = EMPTY_GLYPH;
           }
           else{
              for(int i = 0; i < 2; i++){
                  displayArr[i] = digits[copyT % 10];
                  copyT = copyT / 10;
              }
           }
        }

        void updateNormal(int randNum){
            int copyR = randNum;
            for(int i = 0; i < 4; i++){
                  if(copyR <= 0) displayArr[i] = EMPTY_GLYPH;
                  else displayArr[i] = digits[copyR % 10];
                  copyR = copyR / 10;
                  
            }
        }

        void displayReset(){
           int index = 4;
           for(int i = 0; i < 4; i++){
                    displayArr[i] = EMPTY_GLYPH;
                   
            }
        }
          
        void displayNum(){
             index = (index - 1 + displaySize) % displaySize;  
             displayDigit(displayArr[index], digit_muxpo[index]);  
        }

};

Display disp;

bool buttonHandle(Button &button){
     if(button.pressedOnce() == true){
            button.changeDown(true);
            return true;
        }
     else if(button.released() == true){
              button.changeDown(false);
              button.changeVal(false);
              return false;        
     }
     return false;
}

      
void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < buttonCount; i++){
      button[i].changePin(buttons[i]);
      pinMode(buttons[i],INPUT);
  }
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);
  disp.updateConfig(diceP.getNumThrows(), diceP.getDiceType());
}

void loop() {
  // put your main code here, to run repeatedly:
      if(buttonHandle(button[0]) || button[0].getDown() == true){
        if(diceP.getMode()){
            disp.displayReset();
            diceP.changeMode();
            button[0].changeVal(true);     
      }
      else if(!button[0].getVal()){
          randG.generateRandom(diceP.getNumThrows(), diceP.getDiceType());
      }
      disp.updateNormal(randG.getRandom());
      }             
      else if (buttonHandle(button[1])){
          if(!diceP.getMode()) diceP.changeMode();
          else diceP.incrementNumThrows();
          disp.displayReset();
          disp.updateConfig(diceP.getNumThrows(), diceP.getDiceType());
      }
      else if (buttonHandle(button[2])){
          if(!diceP.getMode()) diceP.changeMode();
          else diceP.incrementDiceType();
          disp.displayReset();
          disp.updateConfig(diceP.getNumThrows(), diceP.getDiceType());
      }
    
      disp.displayNum();
}
