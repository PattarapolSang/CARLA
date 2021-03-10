/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/
#define MIN_ANGLE_RATE  0
#define MAX_ANGLE_RATE  180


#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

String  incomebyte;

int     incomeint;

int     Angle_Update;
int     Angle_Request;

void setup() {

    Serial.begin(9600);
    myservo.attach(10);  // attaches the servo on pin 9 to the servo object
}

void loop() {
        //////////Recieve new flow/////////////////////
    if (Serial.available())
    {   
        

        incomebyte  =   Serial.readString();
        incomeint   =   incomebyte.toInt();
        
        //UpdateFlow();
        if (incomeint != Angle_Update)
        {
            Angle_Update =   incomeint;
            UpdateAngle();
        }

    }

    //val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
    //val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    myservo.write(Angle_Request);                  // sets the servo position according to the scaled value
    delay(15);                           // waits for the servo to get there
}

void UpdateAngle(){
    //////////Delay Update//////////////////////
    Serial.println("Recieving new angle....");
    delay(500);
    Serial.println("...3");
    //delay(1000);
    Serial.println("...2");
    //delay(1000);
    Serial.println("...1");
    //delay(1000);
    Angle_Request = UpperLowerLimit(Angle_Update, MIN_ANGLE_RATE, MAX_ANGLE_RATE);
    Serial.println("Flow Updated");
    Serial.print("Angle_Request ");
    Serial.println(Angle_Request);
}

int UpperLowerLimit(int var, int min, int max){  
    ////Lower Limit////
    if(var < min){
        var = min;
    }
    ////Upper Limit////
    if(var > max){
        var = max;
    }     
    return var;
}
