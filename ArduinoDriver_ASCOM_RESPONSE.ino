// Include Libraries
#include "Arduino.h"

// Pin Definitions
#define STEPPER1_PIN_STEP	3
#define STEPPER1_PIN_DIR	2
#define STEPPER2_PIN_STEP	5
#define STEPPER2_PIN_DIR	4



// Fixed track rate
// 1 sidereal day = 86'164'000 ms
// 1 day = 1 big wheel turn = 144 small screw turns (in RA)
// 1 screw turn = 598'361 ms
// 1 screw turn = 1 motor turn = 200 steps * 8 (microstepping)
// 1 motor step = 374 ms

int stepDelay = 374; // delay in ms for one step
int dur; // duration

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
    Serial.begin(57600);
    Serial.flush();

    // wait for serial port to connect. Needed for native USB
    while (!Serial) ; 
    Serial.println("Connection successful.");
    
}

void loop() 
{
  /* SerialEvent occurs whenever a new data comes in the hardware serial RX. The pulses are put out in the form of <direction#duration[in ms]#>. ( ex: "E#400#" ) */
  if(Serial.available()>0) 
  {
      String driver_cmd;
      driver_cmd = Serial.readStringUntil('#');
    
    
      if(driver_cmd == "I")   // Pulseguiding info
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
      
      if(driver_cmd == "N") {
        driver_cmd = "";
        driver_cmd = Serial.readStringUntil('#');
        dur = driver_cmd.toInt();             
        Move(0, dur);          
        isPulseGuiding = true;
      }
      
      // move motor 1 step south 
    
      if(driver_cmd == "S") {
        driver_cmd = "";
        driver_cmd = Serial.readStringUntil('#');
        dur = driver_cmd.toInt();                
        Move(1, dur);                 
        isPulseGuiding = true;
      }
      
      // move motor 1 step west
      
      if(driver_cmd == "W") {
        driver_cmd = "";
        driver_cmd = Serial.readStringUntil('#');
        dur = driver_cmd.toInt();
        Move(2, dur);  
        isPulseGuiding = true;
      }

      // move motor 1 step east
      
      if(driver_cmd == "E") {
        driver_cmd = "";
        driver_cmd = Serial.readStringUntil('#');
        dur = driver_cmd.toInt();              
        Move(3, dur);          
        isPulseGuiding = true; 
      }

      // stop motors

      if(driver_cmd == "H") {
        driver_cmd = "";
        Move(4, 0);  
        isPulseGuiding = false;
      }
    }
}

void Move(int dir, int duration)
{
if(duration < 374) // for test to get at least one step for each pulse, delete this for actual tracking
{
  duration = 374;
}

int steps = duration/stepDelay;
  
  if(dir == 0) // North
  {
    digitalWrite(STEPPER1_PIN_DIR, LOW);
    for ( int i=0; i<(steps); i++)
    {
      digitalWrite(STEPPER1_PIN_STEP, HIGH);
      delay(stepDelay);
      digitalWrite(STEPPER1_PIN_STEP, LOW);
    }
  }
  else if(dir == 1) // South
  {
    digitalWrite(STEPPER1_PIN_DIR, HIGH);
    for ( int i=0; i<(steps); i++)
    {
      digitalWrite(STEPPER1_PIN_STEP, HIGH);
      delay(stepDelay);
      digitalWrite(STEPPER1_PIN_STEP, LOW);
    }
  }
  else if(dir == 2) // West
  {
    digitalWrite(STEPPER2_PIN_DIR, LOW);
    for ( int i=0; i<steps; i++)
    {
      digitalWrite(STEPPER2_PIN_STEP, HIGH);
      delay(stepDelay);
      digitalWrite(STEPPER2_PIN_STEP, LOW);
    }
  }
  else if(dir == 3) // East
  {
    digitalWrite(STEPPER2_PIN_DIR, LOW);
    for ( int i=0; i<steps; i++)
    {
      digitalWrite(STEPPER2_PIN_STEP, HIGH);
      delay(stepDelay);
      digitalWrite(STEPPER2_PIN_STEP, LOW);
    }
  }
  else if(dir == 4) // STOP
  {
    digitalWrite(STEPPER1_PIN_STEP, LOW);
    digitalWrite(STEPPER2_PIN_STEP, LOW);
  }
}

