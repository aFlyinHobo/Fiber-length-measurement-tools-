//Andrew Rhodes and Christian Sharpe
// The University of Tennessee, Knoxville
// 6/25/2022
//Fiber Plug Maker

#include <AccelStepper.h>
#include <IRremote.h>

#define motorInterfaceType 1

#define EN_PINb   6  //defines the stepper enable pin for the motor connected to the top platform
#define EN_PINt   7 //defines the stepper enable pin for the motor connected to the bottom platform
/////////////////////////////////////////////DEFINE PARAMETERS HERE////////////////////////////////////////////////////
int needleDist = 15; //Distance the needle travels upwards was 10
int plungDist = 3; //Distance the plunger travels relative to the needle was 2. Ratio of 0.2 

int motorSpeed = 3; // Sets the speed of the motors

int motorSpeed = motorSpeed *60; // converts to mm/min. 

// Number of steps per output rotation
const int stepsPerRevolution = 200; // defines the steps per rev for our motors. NEMA 17 = 200
const int stepsPermm = 200; // defines the steps per mm. This motor + leadscrew combo = 200 steps/mm
/////////////////////////////////////////////DEFINE PARAMETERS HERE////////////////////////////////////////////////////

int go=0;
int deposit=0;
int sig=0;
int receiver = 12; // Signal Pin of IR receiver to Arduino Digital Pin 12

// Create Instance of Stepper library

//Stepper topMotor(stepsPerRevolution, 8, 9, 10, 11);
//Stepper bottomMotor(stepsPerRevolution, 4, 5, 6, 7);

//Library motorName(Driver Type, Step Pin, Direction Pin)
AccelStepper topMotor(motorInterfaceType, 8, 9);
AccelStepper bottomMotor(motorInterfaceType, 10, 11);

//decode_results results;     // create instance of 'decode_results'

//Switches
int Tswitch = 2;
int Bswitch = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(Tswitch, INPUT); 
  pinMode(Bswitch, INPUT); 

  // set the speed at 60 rpm:
  //topMotor.setSpeed(18.75);
  //bottomMotor.setSpeed(15);
  
    pinMode(EN_PINt, OUTPUT); // writes this pin as an output
  digitalWrite(EN_PINt, LOW); // sets pin to off (motors disabled) 
      pinMode(EN_PINb, OUTPUT);
  digitalWrite(EN_PINb, LOW); 

  topMotor.setMaxSpeed(1000); // might be in mm/min, not 100% sure
  topMotor.setAcceleration(50000); // set to a high number so motor accels as fast as it can 
  topMotor.setSpeed(motorSpeed); 

  bottomMotor.setMaxSpeed(1000);
  bottomMotor.setAcceleration(50000);
  bottomMotor.setSpeed(motorSpeed);
 
  // initialize the serial port:
  Serial.begin(9600);
  
IrReceiver.begin(receiver, ENABLE_LED_FEEDBACK);

}

void loop() {
  // put your main code here, to run repeatedly:



   if (IrReceiver.decode())
   {
      Serial.println(IrReceiver.decodedIRData.command, HEX);
      IrReceiver.resume();
   }
 // Serial.println(go);
if (IrReceiver.decodedIRData.command==0x45 && go==0) // On Button

  {

    go=1;

  }

if (go==1) //Run Programs

  {



     if (IrReceiver.decode()) // have we received an IR signal?

      {
        sig=IrReceiver.decodedIRData.command;
        
        switch(sig)
    
        {
          
          case 0xFF629D: // VOL+ button pressed
                          
                          //delay(100); 
                          break;
    
          case 0xFFA857: // VOL- button pressed
                         
                         // delay(100); 
                          break;
    
          case 0x45:   //power button pressed  (OFF)
                           Serial.println("on");
                          go=0;  
                          delay(100);        
                          break;
           case 0xD: //St button pressed | Calibrate
                      calibrate();
                      break;
                          
           case 0x40:   //play pressed
    
                     insert();
                     depositing();
                      Serial.println("go");
    
                          break;
                          
           case 0xC:   //1 button pressed | top up

                      topMotor.setSpeed(3000);
                      topMotor.move(-100);//(stepsPerRevolution);
      
                      //delay(50);
                      break;
                      
            case 0x42:   //7 button pressed | top down
                      topMotor.setSpeed(3000);
                      topMotor.move(100);//(-stepsPerRevolution);

                      //delay(50);
                        break;
                        
           case 0x5E:   //3 button pressed | bottom up

                      bottomMotor.setSpeed(3000);
                      bottomMotor.move(100);
 
                      //delay(1000);
                          break;
                          
           case 0x4A:   //9 button pressed | bottom down

                      bottomMotor.setSpeed(3000);
                      bottomMotor.move(-100);

                      //delay(500);
                          break;     
          case 0x18:
                      //2 button pressed | both up
                      topMotor.setSpeed(3000);
                      topMotor.move(-100);
                      bottomMotor.setSpeed(3000);
                      bottomMotor.move(100);
          break;  
          case 0x52:
                       //8 button pressed | both down
                      topMotor.setSpeed(3000);
                      topMotor.move(100);
                      bottomMotor.setSpeed(3000);
                      bottomMotor.move(-100);
          break;  
                    
        }
      }
     // receive the next value
    
    }
    sig=0;
IrReceiver.resume();

topMotor.run();
bottomMotor.run();
}


void calibrate(){
/*
while(digitalRead(Tswitch) == LOW){
  topMotor.move(10);

  
}
while(digitalRead(Bswitch) == LOW){
  bottomMotor.move(-10);

}
topMotor.setCurrentPosition (0);
bottomMotor.setCurrentPosition (0);
*/
}

void insert(){
/*/
while(digitalRead(Bswitch) == LOW){
  
//inserted is 37.15 mm from bearing
//top moves down ~ 140mm so it is on the plunger, ~65mm away from bottom rung
bottomMotor.move(200);


topMotor.move(-800);

}
*/
}

void depositing(){

        bottomMotor.setMaxSpeed((2.5/0.005));
       topMotor.setMaxSpeed((2.5/0.005)*.8);

            topMotor.move(-stepsPermm*needleDist+(stepsPermm*plungDist));
            bottomMotor.move((stepsPermm*needleDist));
            
        
        
}
