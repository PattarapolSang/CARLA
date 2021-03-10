//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "MovingAverageFloat.h"
MovingAverageFloat <16> Freq_A_Flt;
MovingAverageFloat <16> Freq_B_Flt;

#define FREQ_A_PIN  8
#define FREQ_B_PIN  9

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

void setup()
{
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
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Freq A   |Freq B");
    lcd.setCursor(0,1);
    lcd.print(frequency_A_Flt);
    lcd.print(" Hz");
    lcd.setCursor(10,1);
    lcd.print(frequency_B_Flt);
    lcd.print(" Hz");
    lcd.setCursor(0,3);
    lcd.print("Diff Freq = ");
    lcd.print(Dif_AB);
    lcd.print(" Hz");


    Htime_A         =   pulseIn(FREQ_A_PIN,HIGH);        //read high time
    Ltime_A         =   pulseIn(FREQ_A_PIN,LOW);         //read low time     
    Ttime_A         =   Htime_A + Ltime_A;
    frequency_A     =   1000000/Ttime_A;    //getting frequency with Ttime is in Micro seconds
    
    Freq_A_Flt.add(frequency_A);
    frequency_A_Flt = Freq_A_Flt.get();

    Htime_B         =   pulseIn(FREQ_B_PIN,HIGH);        //read high time
    Ltime_B         =   pulseIn(FREQ_B_PIN,LOW);         //read low time     
    Ttime_B         =   Htime_B + Ltime_B;
    frequency_B     =   1000000 / (Ttime_B);    //getting frequency with Ttime is in Micro seconds
    
    Freq_B_Flt.add(frequency_B);
    frequency_B_Flt = Freq_B_Flt.get();

    Dif_AB          =   frequency_A_Flt - frequency_A_Flt;

    delay(1000);
}    