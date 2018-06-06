// Include Libraries
#include "Arduino.h"

// Pin Definitions
#define STEPPER1_PIN_STEP	3
#define STEPPER1_PIN_DIR	2
#define STEPPER2_PIN_STEP	5
#define STEPPER2_PIN_DIR	4



/*/Fixed track rate
// 1 day = 86'400'000 ms
// 1 day = 1 big wheel turn = 100 small screw turns
// 1 screw turn = 864'000 ms
// 1 screw turn = 1 motor turn = 400 steps * 16 (microstepping)
// 1 motor step = 135 ms
// Timer1 OCR : 1000ms = 15625 units
// 15.625*135 = 2109.375 = 2109 (value for 1.0x sidereal rate)
int trackRate = 2109;
int trackWest = trackRate/2;
int trackEast = 10*trackRate;*/

boolean isPulseGuiding = false;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // assign pin modes
    pinMode(STEPPER1_PIN_DIR, OUTPUT);
    pinMode(STEPPER1_PIN_STEP, OUTPUT);
    pinMode(STEPPER2_PIN_DIR, OUTPUT);
    pinMode(STEPPER2_PIN_STEP, OUTPUT);

    // no power to pins
    digitalWrite(STEPPER1_PIN_STEP, LOW);
    digitalWrite(STEPPER2_PIN_STEP, LOW);
    
    // initialize serial connection:
    Serial.begin(9600);
    Serial.flush();

    // wait for serial port to connect. Needed for native USB
    while (!Serial) ; 
    Serial.println("Connection successful.");
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    /* SerialEvent occurs whenever a new data comes in the hardware serial RX.
 This routine is run between each time loop() runs, so using delay inside
 the loop() can delay response.  Multiple bytes of data may be available. */
  if(Serial.available()>0) 
  {
      String driver_cmd;
      driver_cmd = Serial.readStringUntil('#');
    
    
      if(driver_cmd == ": I ")   // Pulseguiding info
      {
        if(isPulseGuiding)
        {
          Serial.println("TRUE#");
        }
        else
        {
          Serial.println("FALSE#");
        }  
      }

      // move motor 1 step north
      
      if(driver_cmd == ": N ") {
        driver_cmd = "";               
        Move(0);          
        isPulseGuiding = true;
      }
      
      // move motor 1 step south 
    
      if(driver_cmd == ": S ") {
        driver_cmd = "";                
        Move(1);                 
        isPulseGuiding = true;
      }
      
      // move motor 1 step west
      
      if(driver_cmd == ": W ") {
        driver_cmd = "";
        Move(2);  
        isPulseGuiding = true;
      }

      // move motor 1 step east
      
      if(driver_cmd == ": E ") {
        driver_cmd = "";               
        Move(3);          
        isPulseGuiding = true; 
      }

      // stop motors

      if(driver_cmd == ": H ") {
        driver_cmd = "";
        Move(4);  
        isPulseGuiding = false;
      }
    }
}

void Move(int dir)
{
  if(dir == 0) // North
  {
    digitalWrite(STEPPER1_PIN_DIR, LOW);
   
    digitalWrite(STEPPER1_PIN_STEP, HIGH);
    delay(100);
    digitalWrite(STEPPER1_PIN_STEP, LOW);
  }
  else if(dir == 1) // South
  {
    digitalWrite(STEPPER1_PIN_DIR, HIGH);

    digitalWrite(STEPPER1_PIN_STEP, HIGH);
    delay(100);
    digitalWrite(STEPPER1_PIN_STEP, LOW);
  }
  else if(dir == 2) // West
  {
    digitalWrite(STEPPER2_PIN_DIR, LOW);

    digitalWrite(STEPPER2_PIN_STEP, HIGH);
    delay(100);
    digitalWrite(STEPPER2_PIN_STEP, LOW);
  }
  else if(dir == 3) // East
  {
    digitalWrite(STEPPER2_PIN_DIR, HIGH);

    digitalWrite(STEPPER2_PIN_STEP, HIGH);
    delay(100);
    digitalWrite(STEPPER2_PIN_STEP, LOW);
  }
  else if(dir == 4) // STOP
  {
    digitalWrite(STEPPER1_PIN_STEP, LOW);
    digitalWrite(STEPPER2_PIN_STEP, LOW);
  }
}

