#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "MovingAverageFloat.h"
MovingAverageFloat <16> Freq_A_Flt;
MovingAverageFloat <16> Freq_B_Flt;
MovingAverageFloat <16> Current_A_Flt;
MovingAverageFloat <16> Current_B_Flt;

#define FREQ_A_PIN  8
#define FREQ_B_PIN  9

#define CURRENT_A_PIN   A0
#define CURRENT_B_PIN   A1

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

int     Htime_A;            //integer for storing high time
int     Ltime_A;            //integer for storing low time
float   Ttime_A;            // integer for storing total time of a cycle
float   frequency_A;        //storing frequency
float   frequency_A_Flt;

int     Htime_B;            //integer for storing high time
int     Ltime_B;            //integer for storing low time
float   Ttime_B;            // integer for storing total time of a cycle
float   frequency_B;        //storing frequency
float   frequency_B_Flt;

float   Dif_AB;

/* Current Read */
float   Current_A_RAW;
float   Current_B_RAW;
float   Current_A;              // Read Current of Motor A
float   Current_B;              // Read Current of Motor B

void Update_Frequency_A(){
    Htime_A         =   pulseIn(FREQ_A_PIN,HIGH);       //read high time
    Ltime_A         =   pulseIn(FREQ_A_PIN,LOW);        //read low time     
    Ttime_A         =   Htime_A + Ltime_A;
    frequency_A     =   1000000/Ttime_A;                //getting frequency with Ttime is in Micro seconds
    
    Freq_A_Flt.add(frequency_A);
    frequency_A_Flt = Freq_A_Flt.get();
}

void Update_Frequency_B(){
    Htime_B         =   pulseIn(FREQ_B_PIN,HIGH);       //read high time
    Ltime_B         =   pulseIn(FREQ_B_PIN,LOW);        //read low time     
    Ttime_B         =   Htime_B + Ltime_B;
    frequency_B     =   1000000 / (Ttime_B);            //getting frequency with Ttime is in Micro seconds
    
    Freq_B_Flt.add(frequency_B);
    frequency_B_Flt = Freq_B_Flt.get();
}

void Update_Current_A(){
    Current_A_RAW   = analogRead(CURRENT_A_PIN);
    Current_A_Flt.add(Current_A_RAW);
    Current_A       = (Current_A_Flt.get() * 0.0266) - 13.267;

    Serial.print("Current Sense_A : ");
    Serial.print(Current_A, DEC);
    Serial.print(" A       ");

    Serial.print("RAW Data A : ");
    Serial.println(Current_A_RAW);
}

void Update_Current_B(){
    Current_B_RAW   = analogRead(CURRENT_B_PIN);
    Current_B_Flt.add(Current_B_RAW);
    Current_B       = (Current_B_Flt.get() * 0.0269) - 13.359;

    Serial.print("Current Sense_B : ");
    Serial.print(Current_B, DEC);
    Serial.print(" A       ");

    Serial.print("RAW Data B : ");
    Serial.println(Current_B_RAW);
}

void Update_LCD_Monitor(){
    lcd.clear();

    /* Flow Screne */ 
    lcd.setCursor(0,0);
    lcd.print("Freq A   |Freq B");
    lcd.setCursor(0,1);
    lcd.print(frequency_A_Flt);
    lcd.print(" Hz");
    lcd.setCursor(10,1);
    lcd.print(frequency_B_Flt);
    lcd.print(" Hz");

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

void setup()
{
    Serial.begin(9600);
    
    pinMode(FREQ_A_PIN,INPUT);
    pinMode(FREQ_B_PIN,INPUT);
    
    lcd.init();                      // initialize the lcd 
    lcd.init();
    // Print a message to the LCD.
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
}


void loop()
{
    Update_LCD_Monitor();

    Update_Frequency_A();
    Update_Frequency_B();

    Update_Current_A();
    Update_Current_B();

    delay(1000);
}    
