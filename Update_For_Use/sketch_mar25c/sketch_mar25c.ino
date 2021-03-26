#include <SPI.h>
#include <SD.h>

File myFile; // สร้างออฟเจก File สำหรับจัดการข้อมูล
const int   chipSelect          = 4;

int         File_Name_Count     = 0;
String      File_Name           = "Log_";

bool        Search_Sts          = 1;

float       Flow_A              = 1.0;
float       Current_A           = 1.1;
float       Flow_B              = 2.0;
float       Current_B           = 2.2;

void Scan_File_Name(){
    while (Search_Sts){
        
        Serial.print("We are checking file : ");
        Serial.println(File_Name_Count);
        File_Name   = "Log_";
        File_Name.concat(File_Name_Count);
        File_Name.concat(".txt");

        Serial.print("This is file name:  ");
        Serial.println(File_Name);
        myFile      = SD.open(File_Name, FILE_READ);
        
        if(myFile){
            Serial.print("Already have : ");
            Serial.println(File_Name_Count);
            Serial.print("File name  : ");
            Serial.println(File_Name);  

            File_Name_Count++;
            myFile.close(); 
        }
        else{
            Serial.print("Finish at : ");
            Serial.println(File_Name_Count);
            Serial.print("File name  : ");
            Serial.println(File_Name);

            myFile.close(); 
            Search_Sts  =   false;
        }
        //Search_Sts = false;
    
    }
}

void setup()
{
    Serial.begin(9600);

    Serial.print("Initializing SD card...");
    pinMode(SS, OUTPUT);

    if (!SD.begin(chipSelect)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    Scan_File_Name();
    
    Serial.print("This is file name after searching:  ");
    Serial.println(File_Name);  

    myFile = SD.open(File_Name, FILE_WRITE);               // เปิดไฟล์ที่ชื่อ test.txt เพื่อเขียนข้อมูล โหมด FILE_WRITE
    Serial.print("We write on file name   : ");


    Serial.println(File_Name);
    // ถ้าเปิดไฟล์สำเร็จ ให้เขียนข้อมูลเพิ่มลงไป
    if (myFile) {
        Serial.print("Writing to test.txt...");
        myFile.println("Board initialized Ready to write Data to Board");
        myFile.println("Flow_A,Current_A,Flow_B,Current_B");
        myFile.println("");                // สั่งให้เขียนข้อมูล
        myFile.close();                                     // ปิดไฟล์
        Serial.println("done.");
    } else {       
        Serial.println("error opening test.txt");           // ถ้าเปิดไฟลืไม่สำเร็จ ให้แสดง error 
    }

}

void loop(){
    Serial.println("We are in the main loop");
    Serial.println(".............................");
    
    myFile = SD.open(File_Name, FILE_WRITE);               // เปิดไฟล์ที่ชื่อ test.txt เพื่อเขียนข้อมูล โหมด FILE_WRITE
    Serial.print("We write on file name   : ");
    Serial.println(File_Name);

    if (myFile) {

        myFile.print(Flow_A);
        myFile.print(",");
        myFile.print(Current_A);
        myFile.print(",");
        myFile.print(Flow_B);
        myFile.print(",");
        myFile.println(Current_B);

        myFile.close();                                     // ปิดไฟล์
        Serial.println("done.");

    } else {       
        Serial.println("error opening test.txt");           // ถ้าเปิดไฟลืไม่สำเร็จ ให้แสดง error 
    }    

}
