#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "MovingAverageFloat.h"
MovingAverageFloat <16> Freq_A_Flt;
MovingAverageFloat <16> Freq_B_Flt;

MovingAverageFloat <16> Current_A_Flt;
MovingAverageFloat <16> Current_B_Flt;

#define FREQ_A_PIN      8
#define FREQ_B_PIN      9

#define FLOW_A_PIN      2
#define FLOW_B_PIN      3

#define CURRENT_A_PIN   A0
#define CURRENT_B_PIN   A1

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

/* Frequency Read for Brushless Motor */
int     Htime_A;            // integer for storing high time
int     Ltime_A;            // integer for storing low time
float   Ttime_A;            // integer for storing total time of a cycle
float   frequency_A;        // storing frequency
float   frequency_A_Flt;

int     Htime_B;            // integer for storing high time
int     Ltime_B;            // integer for storing low time
float   Ttime_B;            // integer for storing total time of a cycle
float   frequency_B;        // storing frequency
float   frequency_B_Flt;

unsigned int    Dif_AB;

/* Pulse Read for Brush Motor */
volatile int    Pulse_Flow_A;   // Read Pulse for calculating Flow rate  
volatile int    Pulse_Flow_B;   // Read Pulse for calculating Flow rate
float   Flow_Rate_A;
float   Flow_Rate_B;

/* Current Read */
float   Current_A_RAW;
float   Current_B_RAW;
float   Current_A;              // Read Current of Motor A
float   Current_B;              // Read Current of Motor B

float   Offset_Current_A    = 0;// Offset current A
float   Offset_Current_B    = 0;// Offset current B

float   Offset_A            = 0;// Temporary Offset for current A while tunning
float   Offset_B            = 0;// Temporary Offset for current B while tunning

/* Timer variable */
unsigned long   ctTime;         // Counting time
unsigned long   cloopTime;      // Loop time
unsigned long   ctunningTime;   // Tuning time

void Update_LCD_Monitor(){
    lcd.clear();

    /* Flow Screne */ 
    lcd.setCursor(0,0);
    lcd.print("Flow A   |Flow B");
    lcd.setCursor(0,1);
    lcd.print(Flow_Rate_A);
    lcd.print(" L/m");
    lcd.setCursor(10,1);
    lcd.print(Flow_Rate_B);
    lcd.print(" L/m");

    /* Current Screne */ 
    lcd.setCursor(0,2);
    lcd.print("Current A|Current B");
    lcd.setCursor(0,3);
    lcd.print(Current_A);
    lcd.print(" A");
    lcd.setCursor(10,3);
    lcd.print(Current_B);
    lcd.print(" A");
}

void Frequency_A_update(){
    Htime_A         =   pulseIn(FREQ_A_PIN,HIGH);       //read high time
    Ltime_A         =   pulseIn(FREQ_A_PIN,LOW);        //read low time     
    Ttime_A         =   Htime_A + Ltime_A;
    frequency_A     =   1000000/Ttime_A;                //getting frequency with Ttime is in Micro seconds
    
    Freq_A_Flt.add(frequency_A);
    frequency_A_Flt = Freq_A_Flt.get();
}

void Frequency_B_update(){
    Htime_B         =   pulseIn(FREQ_B_PIN,HIGH);       //read high time
    Ltime_B         =   pulseIn(FREQ_B_PIN,LOW);        //read low time     
    Ttime_B         =   Htime_B + Ltime_B;
    frequency_B     =   1000000 / (Ttime_B);            //getting frequency with Ttime is in Micro seconds
    
    Freq_B_Flt.add(frequency_B);
    frequency_B_Flt = Freq_B_Flt.get();
}

void RPM_A(){ 
    /* This is the function that the interupt calls */ 
    /* This function measures the rising and falling edge of the hall effect sensors signal */
    Pulse_Flow_A++;                    
} 

void RPM_B(){ 
    /* This is the function that the interupt calls */ 
    /* This function measures the rising and falling edge of the hall effect sensors signal */
    Pulse_Flow_B++;                    
} 

void Update_Flow_A(){
    Flow_Rate_A     = ((float(Pulse_Flow_A) * 33.672) - 5.104);

    Serial.print("Water Sensor_A : ");
    Serial.print(Flow_Rate_A, DEC);
    Serial.print(" L/Min       ");

    Serial.print("Pulse Counter : ");
    Serial.println(Pulse_Flow_A);

    Pulse_Flow_A    = 0;  
}

void Update_Flow_B(){
    Flow_Rate_B     = ((float(Pulse_Flow_B) * 35.235) - 2.7);

    Serial.print("Water Sensor_B : ");
    Serial.print(Flow_Rate_B, DEC);
    Serial.print(" L/Min       ");

    Serial.print("Pulse Counter : ");
    Serial.println(Pulse_Flow_B); 

    Pulse_Flow_B    = 0;
}

void Update_Current_A(){
    Current_A_RAW   = analogRead(CURRENT_A_PIN);
    Current_A_Flt.add(Current_A_RAW);
    Current_A       = (Current_A_Flt.get() * 0.027) - 13.62 + Offset_Current_A;

    Serial.print("Current Sense_A : ");
    Serial.print(Current_A, DEC);
    Serial.print(" A       ");

    Serial.print("RAW Data A : ");
    Serial.println(Current_A_RAW);
}

void Update_Current_B(){
    Current_B_RAW   = analogRead(CURRENT_B_PIN);
    Current_B_Flt.add(Current_B_RAW);
    Current_B       = (Current_B_Flt.get() * 0.027) - 14.34 + Offset_Current_B;

    Serial.print("Current Sense_B : ");
    Serial.print(Current_B, DEC);
    Serial.print(" A       ");

    Serial.print("RAW Data B : ");
    Serial.println(Current_B_RAW);
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
            Offset_B = Current_B * (-1);

            Serial.println("................Finish Tunning....................");
            Serial.print("Offset A : ");
            Serial.print(Offset_A);
            Serial.print("  Current A : ");
            Serial.println(Current_A);
            Serial.print("Offset A : ");
            Serial.print(Offset_B);
            Serial.print("  Current A : ");
            Serial.println(Current_B);

            Serial.println("....................................................");

            Current_A_Tune_Sts = false;                         // Finish Status
        }

        /* Task 1000 ms */
        if(ctTime >= (cloopTime + 1000)){
            cloopTime   = ctTime;                               // Update Time
            
            Update_Current_A();

            Serial.println(".............Updated Current................");
        }

    }
}


void setup()
{
    Serial.begin(9600);

    /* initialize the Freq PIN */ 
    pinMode(FREQ_A_PIN,INPUT);
    pinMode(FREQ_B_PIN,INPUT);

    /* initialize the Freq PIN */ 
    pinMode(FLOW_A_PIN,INPUT_PULLUP);
    pinMode(FLOW_B_PIN,INPUT_PULLUP); 

    /* Attrach interupt */
    attachInterrupt(digitalPinToInterrupt(FLOW_A_PIN), RPM_A, RISING);
    attachInterrupt(digitalPinToInterrupt(FLOW_B_PIN), RPM_B, RISING);
    
    /* initialize the lcd */ 
    lcd.init();                      
    lcd.init();

    /* Print a message to the LCD. */
    lcd.backlight();
    lcd.setCursor(3,0);
    lcd.print("Hello, world!");
    lcd.setCursor(2,1);
    lcd.print("Frequency Counter");
    lcd.setCursor(4,2);
    lcd.print("HG ROBOTICS");
    lcd.setCursor(3,3);
    lcd.print("Power By ORBs");

    delay(1000);

    /* Start Interupt */
    sei();

    /* Start Timer */
    ctTime = millis();                      // Update Running timer
    cloopTime = ctTime;                     // For 1 sec loop current

    ctunningTime        = ctTime;           // For first 16 sec tunning time     

    /* Start Tunning Current */
    AutoTune_Current();

    Offset_Current_A    = Offset_A;
    Offset_Current_B    = Offset_B;

    Serial.print("!!!!!!! Done Initialized !!!!!");
}

void loop() {
    /* Update current timming */
    ctTime = millis();

    /* Task 1000 ms */
    if(ctTime >= (cloopTime + 1000)){
        cloopTime   = ctTime;                             // Update Time
        
        Update_LCD_Monitor();
        Update_Flow_A();
        Update_Flow_B();  
        Update_Current_A();
        Update_Current_B();

        Serial.println();

    }

    //Dif_AB          =   Flow_Rate_A - Flow_Rate_B;

} 
