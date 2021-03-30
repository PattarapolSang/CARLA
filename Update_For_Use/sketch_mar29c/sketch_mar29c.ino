#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <SD.h>
File myFile;                // Create new file

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

#define PWM_PIN         10

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

/* Currnet Tunning Status */
bool    Current_Tune_Sts    = true;

/* Timer variable */
unsigned long   ctTime;         // Counting time
unsigned long   cloopTime;      // Loop time
unsigned long   ctunningTime;   // Tuning time
unsigned long   cloopTime_Log;  // Loop time for logging

/* SD card configuration */
String      File_Name           = "Log_";
int         File_Name_Count     = 0;
bool        Search_Sts          = 1;
const int   chipSelect          = 4;

void Update_LCD_Monitor(){
    //lcd.clear();

    /* Flow Screne */ 
    //lcd.setCursor(0,0);
    //lcd.print("Flow A   |Flow B");
    lcd.setCursor(0,1);
    lcd.print(Flow_Rate_A);
    lcd.print(" L/m");
    lcd.setCursor(10,1);
    lcd.print(Flow_Rate_B);
    lcd.print(" L/m");

    /* Current Screne */ 
    //lcd.setCursor(0,2);
    //lcd.print("Current A|Current B");
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

void Update_Flow_A(){
    Flow_Rate_A     = ((float(Pulse_Flow_A) * 33.672) - 5.104);

    /*Serial.print("Water Sensor_A : ");
    Serial.print(Flow_Rate_A, DEC);
    Serial.print(" L/Min       ");

    Serial.print("Pulse Counter : ");
    Serial.println(Pulse_Flow_A);*/

    Pulse_Flow_A    = 0;  
}

void Update_Flow_B(){
    Flow_Rate_B     = ((float(Pulse_Flow_B) * 35.235) - 2.7);

    /*Serial.print("Water Sensor_B : ");
    Serial.print(Flow_Rate_B, DEC);
    Serial.print(" L/Min       ");

    Serial.print("Pulse Counter : ");
    Serial.println(Pulse_Flow_B);*/ 

    Pulse_Flow_B    = 0;
}

void Update_Current_A(){
    Current_A_RAW   = analogRead(CURRENT_A_PIN);
    Current_A_Flt.add(Current_A_RAW);
    Current_A       = (Current_A_Flt.get() * 0.027) - 13.62 + Offset_Current_A;

    /*Serial.print("Current Sense_A : ");
    Serial.print(Current_A, DEC);
    Serial.print(" A       ");

    Serial.print("RAW Data A : ");
    Serial.println(Current_A_RAW);*/
}

void Update_Current_B(){
    Current_B_RAW   = analogRead(CURRENT_B_PIN);
    Current_B_Flt.add(Current_B_RAW);
    Current_B       = (Current_B_Flt.get() * 0.027) - 14.34 + Offset_Current_B;

    /*Serial.print("Current Sense_B : ");
    Serial.print(Current_B, DEC);
    Serial.print(" A       ");

    Serial.print("RAW Data B : ");
    Serial.println(Current_B_RAW);*/
}

void Logging_Initialize(){
    
    /*Serial.print("Initializing SD card...");*/
    pinMode(SS, OUTPUT);

    if (!SD.begin(chipSelect)) {
        /*Serial.println("initialization failed!");*/
        return;
    }
    /*Serial.println("initialization done.");*/

    while (Search_Sts){
        
        /*Serial.print("We are checking file : ");
        Serial.println(File_Name_Count);*/
        File_Name   = "Log_";
        File_Name.concat(File_Name_Count);
        File_Name.concat(".txt");

        /*Serial.print("This is file name:  ");
        Serial.println(File_Name);*/
        myFile      = SD.open(File_Name, FILE_READ);
        
        if(myFile){
            /*Serial.print("Already have : ");
            Serial.println(File_Name_Count);
            Serial.print("File name  : ");
            Serial.println(File_Name); */ 

            File_Name_Count++;
            myFile.close(); 
        }
        else{
            /*Serial.print("Finish at : ");
            Serial.println(File_Name_Count);
            Serial.print("File name  : ");
            Serial.println(File_Name);*/

            myFile.close(); 
            Search_Sts  =   false;
        }
        //Search_Sts = false;
    }    
    /*Serial.print("This is file name after searching:  ");
    Serial.println(File_Name);*/  

    myFile = SD.open(File_Name, FILE_WRITE);               // เปิดไฟล์ที่ชื่อ test.txt เพื่อเขียนข้อมูล โหมด FILE_WRITE
    /*Serial.print("We write on file name   : ");*/


    Serial.println(File_Name);
    // ถ้าเปิดไฟล์สำเร็จ ให้เขียนข้อมูลเพิ่มลงไป
    if (myFile) {
        /*Serial.print("Writing to test.txt...");*/
        myFile.println("Board initialized Ready to write Data to Board");
        myFile.println("Flow_A,Current_A,Flow_B,Current_B");
        myFile.close();                                     // ปิดไฟล์
        /*Serial.println("done.");*/
    } else {       
        //Serial.println("error opening test.txt");           // ถ้าเปิดไฟลืไม่สำเร็จ ให้แสดง error*/ 
    }

}

void Logging_Write(){

    myFile = SD.open(File_Name, FILE_WRITE);               // เปิดไฟล์ที่ชื่อ test.txt เพื่อเขียนข้อมูล โหมด FILE_WRITE
    /*Serial.print("We write on file name   : ");
    Serial.println(File_Name);*/

    if (myFile) {

        myFile.print(Flow_Rate_A);
        myFile.print(",");
        myFile.print(Current_A);
        myFile.print(",");
        myFile.print(Flow_Rate_B);
        myFile.print(",");
        myFile.println(Current_B);

        myFile.close();                                     // ปิดไฟล์
        /*Serial.println("done.");*/
        lcd.setCursor(0,0);
        lcd.print("Logging ");
        lcd.print(File_Name);  
    } else {       
        //Serial.println("error opening test.txt");           // ถ้าเปิดไฟลืไม่สำเร็จ ให้แสดง error */
        lcd.setCursor(0,0);
        lcd.print("Cannot Logging..");
    } 

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

void AutoTune_Current(){

    while(Current_Tune_Sts){
        
        /*Serial.println("...............Start tunning.......................");*/
        
        ctTime = millis();
        
        /*Serial.print("ctTime :");
        Serial.println(ctTime);
        Serial.print("cloopTime :");
        Serial.println(cloopTime);
        Serial.print("ctunningTime :");
        Serial.println(ctunningTime);*/

        if(ctTime >= (ctunningTime + 20000)){
            
            /* Getting error value */
            Offset_A = Current_A * (-1);
            Offset_B = Current_B * (-1);

            /*Serial.println("................Finish Tunning....................");
            Serial.print("Offset A : ");
            Serial.print(Offset_A);
            Serial.print("  Current A : ");
            Serial.println(Current_A);
            Serial.print("Offset A : ");
            Serial.print(Offset_B);
            Serial.print("  Current A : ");
            Serial.println(Current_B);

            Serial.println("....................................................");*/

            Current_Tune_Sts  = false;                        // Finish Status
        }

        /* Task 1000 ms */
        if(ctTime >= (cloopTime + 1000)){
            cloopTime   = ctTime;                               // Update Time
            
            Update_Current_A();
            Update_Current_B();

            /*Serial.println(".............Updated Current................");*/
        }

    }
    
    /* Assign new offset to current formula */   
    Offset_Current_A    = Offset_A;
    Offset_Current_B    = Offset_B;

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

    /* PWM for supply tunning */
    pinMode(PWM_PIN, OUTPUT);
    analogWrite(PWM_PIN, 0);

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
    ctTime              = millis();         // Update Running timer
    cloopTime           = ctTime;           // For 1 sec loop current
    ctunningTime        = ctTime;           // For first 16 sec tunning time     

    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("..Tunning Current..");

    /* Start Tunning Current */
    AutoTune_Current();

    /* SD Card Start to Logging file */
    Logging_Initialize();

    /* Reset timer after Auto tunning */
    ctTime              = millis();         // Update Running timer
    cloopTime           = ctTime;           // For 1 sec loop current    

    /*Serial.print("!!!!!!! Done Initialized !!!!!");*/
}

void loop() {
    /* Update current timming */
    ctTime = millis();

    /* PWM supply at 16 volt */
    analogWrite(PWM_PIN, 165);

    /* Task 1000 ms */
    if(ctTime >= (cloopTime + 1000)){
        cloopTime   = ctTime;                             // Update Time
        
        Update_Current_A();
        Update_Current_B();
        Update_Flow_A();
        Update_Flow_B();
        Update_LCD_Monitor(); 

        /*Serial.println();*/

    }

    if(ctTime >= (cloopTime_Log + 20000)){
        cloopTime_Log   = ctTime;                             // Update Time
        
        Logging_Write();
        //Update_LCD_Monitor();

        /*Serial.println("Write logging!!!");*/

    }
    //Dif_AB          =   Flow_Rate_A - Flow_Rate_B;

} 
