#include "MovingAverageFloat.h"

MovingAverageFloat <16> Current_A_Flt;

/* Current Read */
float   Current_A_RAW;
float   Current_B_RAW;
float   Current_A;              // Read Current of Motor A
float   Current_B;              // Read Current of Motor B

float   Offset_A            = 0;
float   Offset_B            = 0;

float   Offset_Current_A    = 0;
float   Offset_Current_B    = 0;

/* Currnet Tunning Status */
bool    Current_A_Tune_Sts       = true;
bool    Current_B_Tune_Sts       = true;  

unsigned long   ctTime;  
unsigned long   cloopTime;

unsigned long   ctunningTime;

#define CURRENT_A_PIN   A0

void Update_Current_A(){
    Current_A_RAW   = analogRead(CURRENT_A_PIN);
    Current_A_Flt.add(Current_A_RAW);
    Current_A       = (Current_A_Flt.get() * 0.0269) - 13.62 + Offset_Current_A;

    Serial.print("Current Sense_A : ");
    Serial.print(Current_A, DEC);
    Serial.print(" A       ");

    Serial.print("RAW Data A : ");
    Serial.println(Current_A_RAW);
}

void AutoTune_Current(){

    while(Current_A_Tune_Sts){
        
        Serial.println("...............Start tunning.......................");
        
        ctTime = millis();

        Serial.print("ctTime :");
        Serial.println(ctTime);
        Serial.print("cloopTime :");
        Serial.println(cloopTime);
        Serial.print("ctunningTime :");
        Serial.println(ctunningTime);


        if(ctTime >= (ctunningTime + 17000)){
            
            Offset_A = Current_A * (-1);
            //Offset_Current_B = Current_B * (-1);

            Serial.println("................Finish Tunning....................");
            Serial.print("Offset A : ");
            Serial.print(Offset_A);
            Serial.print("  Current A : ");
            Serial.println(Current_A);
            
            Serial.println(".............................");

            Current_A_Tune_Sts = false;
        }

        /* Task 1000 ms */
        if(ctTime >= (cloopTime + 1000)){
            cloopTime   = ctTime;                             // Update Time
            
            Update_Current_A();

            Serial.println(".............Updated Current................");
        }

    }
}

void setup(){
    
    Serial.begin(9600);

    Serial.println(".........Initialized.................");

    /* Start Timer */
    ctTime              = millis();         // Update Running timer
    cloopTime           = ctTime;           // For 1 sec loop current
    ctunningTime        = ctTime;           // For first 16 sec tunning time 

    AutoTune_Current();

    Offset_Current_A    = Offset_A;
    Offset_Current_B    = Offset_B;

    Serial.println(".............Finish Setup loop.....................");


}

void loop(){
    
    /* Update current timming */
    ctTime = millis();

    /* Task 1000 ms */
    if(ctTime >= (cloopTime + 1000)){
        cloopTime   = ctTime;                             // Update Time
         
        Update_Current_A();

        Serial.println();

    }

}