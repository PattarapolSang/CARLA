


#include "U8glib.h"


U8GLIB_ST7920_128X64_1X u8g(23, 17, 16);  // SPI Com: SCK = en = 23, MOSI = rw = 17, CS = di = 16

//int vol = 0;

#define outputA 33 
 #define outputB 31
 #define buzzer 37
 
 const int  buttonPin = 35; 
 int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 1;         // current state of the button
int lastButtonState = 1;     // previous state of the button
int longPressDuration = 2000;
 int pseudo_counter = 0;
 bool pressType;
 
 int counter0 = 0; 
 int pseudo_counter0 = 0;
 int aState0;
 int aLastState0; 


  int counter1 = 0; 
 int pseudo_counter1 = 0;
 int aState1;
 int aLastState1;

 int counter2 = 0; 
 int pseudo_counter2 = 0;
 int aState2;
 int aLastState2;

 int counter3 = 0; 
 int pseudo_counter3 = 0;
 int aState3;
 int aLastState3;

 unsigned int aw = 10000;

 unsigned int a0 = 0;
 unsigned int a1 = 0;
 unsigned int a2 = 0;
 unsigned int a3 = 0;
 char c[5] = "0";
 char c0[5]= "0";
 char c1[5]= "0";
 char c2[5]= "0";
 char c3[5]= "0";
 char cw[6]= "10000";
 /////////////////////////////////////////////////////////
 void setupBuzzer(){
   pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
 }
 /////////////////////////////////////////////////////////
void setupEnc(){
  pinMode (outputA,INPUT_PULLUP);
   pinMode (outputB,INPUT_PULLUP);
    //Serial.println(counter);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void counterEnc0(){
   aState0 = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState0 != aLastState0){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState0) { 
       counter0 ++;
     } else {
       counter0 --;
     }
     if (counter0 < 0) counter0 = 0;
     if (counter0 > 200) counter0 = 200;
     pseudo_counter0 = counter0/2;
     Serial.print("Position: ");
     Serial.println(pseudo_counter0);
      a0 = calculation(pseudo_counter0);
      String b0;
      b0 = (String)a0;
      b0.toCharArray(c0, 5);
   } 
   aLastState0 = aState0; // Updates the previous state of the outputA with the current state
}
///////////////////////////////////////////////////////////////////////////////////////
void counterEnc1(){
   aState1 = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState1 != aLastState1){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState1) { 
       counter1 ++;
     } else {
       counter1 --;
     }
     if (counter1 < 0) counter1 = 0;
     if (counter1 > 200) counter1 = 200;
     pseudo_counter1 = counter1/2;
     Serial.print("Position: ");
     Serial.println(pseudo_counter1);
      a1 = calculation(pseudo_counter1);
      String b1;
      b1 = (String)a1;
      b1.toCharArray(c1, 5);
   } 
   aLastState1 = aState1; // Updates the previous state of the outputA with the current state
}
////////////////////////////////////////////////////////////////////////////////////////////////
void counterEnc2(){
   aState2 = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState2 != aLastState2){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState2) { 
       counter2 ++;
     } else {
       counter2 --;
     }
     if (counter2 < 0) counter2 = 0;
     if (counter2 > 200) counter2 = 200;
     pseudo_counter2 = counter2/2;
     Serial.print("Position: ");
     Serial.println(pseudo_counter2);
      a2 = calculation(pseudo_counter2);
      String b2;
      b2 = (String)a2;
      b2.toCharArray(c2, 5);
   } 
   aLastState2 = aState2; // Updates the previous state of the outputA with the current state
}
////////////////////////////////////////////////////////////////////////////////////////////////
void counterEnc3(){
   aState3 = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState3 != aLastState3){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState3) { 
       counter3 ++;
     } else {
       counter3 --;
     }
     if (counter3 < 0) counter3 = 0;
     if (counter3 > 200) counter3 = 200;
     pseudo_counter3 = counter3/2;
     Serial.print("Position: ");
     Serial.println(pseudo_counter3);
      a3 = calculation(pseudo_counter3);
      String b3;
      b3 = (String)a3;
      b3.toCharArray(c3, 5);
   } 
   aLastState3 = aState3; // Updates the previous state of the outputA with the current state
}
////////////////////////////////////////////////////////////////////////////////////////////////
int calculation(int pseudo_counter){
  int vol =0;
  vol = pseudo_counter*10;
  if(vol < 0)vol = 0;
  if(vol > 1000)vol = 1000;
  return vol;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void draw_version() {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_fur14);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 42, 15, "TGD");
  u8g.setFont(u8g_font_fur11);
   u8g.drawStr( 12, 36, "Chemical Mixer");
  u8g.setFont(u8g_font_u8glib_4);
  //u8g.drawStr( 100, 63, "V 1.0.0");
  u8g.drawStr( 84, 63, "V 1.0.0 BETA");
  
}
////////////////////////////////////////////////////////////////////////////////////////////////
void draw() {
  aw = 10000 - (a0+a1+a2+a3);
  String bw;
      bw = (String)aw;
      bw.toCharArray(cw, 6);
      int x;
      if (buttonPushCounter == 0){
        x = 12;
      }
      else if(buttonPushCounter == 1){
        x = 24;
      }
      else if(buttonPushCounter == 2){
        x = 36;
      }
      else if(buttonPushCounter == 3){
        x = 48;
      }
      else{
        
      }
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont_75r);
  u8g.drawStr( 0, x, ":");  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 12, " ChemA = ");
  u8g.drawStr( 70, 12, c0 );
  u8g.drawStr( 112, 12, "mL");
  u8g.drawStr( 0, 24, " ChemB = ");
  u8g.drawStr( 70, 24, c1 );
  u8g.drawStr( 112, 24, "mL");
  u8g.drawStr( 0, 36, " ChemC = ");
  u8g.drawStr( 70, 36, c2 );
  u8g.drawStr( 112, 36, "mL");
  u8g.drawStr( 0, 48, " ChemD = ");
  u8g.drawStr( 70, 48, c3 );
  u8g.drawStr( 112, 48, "mL");
  u8g.drawStr( 0, 60, " Water = ");
  u8g.drawStr( 70, 60, cw );
  u8g.drawStr( 112, 60, "mL");
}
//////////////////////////////////////////////////////////////////////////////////////////////
void draw_process(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 15, 32, "Processing..."); 
}
//////////////////////////////////////////////////////////////////////////////////////////////
void draw_filling(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 25, 32, "Filling..."); 
}
//////////////////////////////////////////////////////////////////////////////////////////////
void draw_done(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 48, 32, "DONE"); 
}
//////////////////////////////////////////////////////////////////////////////////////////////
void draw_problem(void) { 
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_freedoomr10r);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 45, 32, "ERROR"); 
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool bpc(){
   // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    long prev = 0; 
    if (buttonState == LOW) {
      // if the current state is LOW(Active Low) then the button went from off to on:
      prev = millis();
      pressType = HIGH; // mean long press
    while((millis()- prev)<= longPressDuration){
      if(digitalRead(buttonPin) == HIGH){
        pressType = LOW; //mean short press
        break;
      }
    }
    if(pressType == HIGH){
       buttonPushCounter = 9; // long press counter state no.
    }
    else{
      buttonPushCounter++;
    }
      if(buttonPushCounter > 3 && buttonPushCounter < 9) buttonPushCounter = 0; //CHANGE 3 to any no. to unlimit chem types in mixer !!! up to 8
      if(buttonPushCounter > 9) buttonPushCounter = 0; //return to case 0
////      Serial.println("on");
////      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
////      delay(50);
////      lastButtonState = buttonState;
//      //return true;
     delay(50);
     lastButtonState = buttonState;
     if(pressType == LOW){
       return true; // Short press mean true to advance into some process
    }
    else{
      return false;// 
    }   
    } 
    else {
//      delay(50);
//      lastButtonState = buttonState;
     // return false;
      // if the current state is HIGH(Active Low) then the button went from on to off:
     // Serial.println("off");
     delay(50);
     lastButtonState = buttonState;
    }
    // Delay a little bit to avoid bouncing
//    delay(50);
  }
  // save the current state as the last state, for next time through the loop
//    lastButtonState = buttonState;

    //Classify press type
    
    if(pressType == LOW){
       return true; // Short press mean true to advance into some process
    }
    else{
      return false;// 
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//void bpc(){
//   // read the pushbutton input pin:
//  buttonState = digitalRead(buttonPin);
//
//  // compare the buttonState to its previous state
//  if (buttonState != lastButtonState) {
//    // if the state has changed, increment the counter
//    long prev = 0; 
//    if (buttonState == LOW) {
//      // if the current state is LOW(Active Low) then the button went from off to on:
//      prev = millis();
//      pressType = HIGH; // mean long press
//    while((millis()- prev)<= longPressDuration){
//      if(digitalRead(buttonPin) == HIGH){
//        pressType = LOW; //mean short press
//        break;
//      }
//    }
//    if(pressType == HIGH){
//       buttonPushCounter = 9; // long press counter state no.
//    }
//    else{
//      buttonPushCounter++;
//    }
//      if(buttonPushCounter > 3 && buttonPushCounter  < 9) buttonPushCounter = 0; //CHANGE 1 to any no. to unlimit chem types in mixer !!! up to 8
//////      Serial.println("on");
//////      Serial.print("number of button pushes: ");
//      Serial.println(buttonPushCounter);
//////      delay(50);
//////      lastButtonState = buttonState;
////      //return true;
//        
//    } 
//    else {
////      delay(50);
////      lastButtonState = buttonState;
//     // return false;
//      // if the current state is HIGH(Active Low) then the button went from on to off:
//     // Serial.println("off");
//    }
//    // Delay a little bit to avoid bouncing
//    delay(50);
//  }
//  // save the current state as the last state, for next time through the loop
//  lastButtonState = buttonState;
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//void bpc(){
//   // read the pushbutton input pin:
//  buttonState = digitalRead(buttonPin);
//
//  // compare the buttonState to its previous state
//  if (buttonState != lastButtonState) {
//    // if the state has changed, increment the counter
//    if (buttonState == LOW) {
//      // if the current state is HIGH then the button went from off to on:
//      buttonPushCounter++;
//      if(buttonPushCounter > 1 ) buttonPushCounter = 0;
////      Serial.println("on");
////      Serial.print("number of button pushes: ");
//      Serial.println(buttonPushCounter);
//    } else {
//      // if the current state is LOW then the button went from on to off:
//     // Serial.println("off");
//    }
//    // Delay a little bit to avoid bouncing
//    delay(50);
//  }
//  // save the current state as the last state, for next time through the loop
//  lastButtonState = buttonState;
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool init_checking() {
//  long measurement;
//  measurement = pulseIn (vibroPin, HIGH); //wait for the pin to get HIGH and returns measurement
//  if (measurement > 1000) return true;
//  else return false;
  delay(1000);
  return true;
  //return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool process_checking() { //1.if no tank -> false, 2.set zero tank, 3.close lid check, 4.calculate stepper rev 
//  long measurement;
//  measurement = pulseIn (vibroPin, HIGH); //wait for the pin to get HIGH and returns measurement
//  if (measurement > 1000) return true;
//  else return false;
  //return false;
  u8g.firstPage();
   do{ //Start to draw first page
     draw_process();
   } while( u8g.nextPage() );
  delay(1000);
  return true; // process check ok
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
#include "BasicStepperDriver.h"
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define A_MOTOR_STEPS 200
#define A_RPM 350

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define A_MICROSTEPS 1

// All the wires needed for full functionality
#define A_DIR A1
#define A_STEP A0
//Uncomment line to use enable/disable functionality
#define A_SLEEP 38

BasicStepperDriver A_stepper(A_MOTOR_STEPS, A_DIR, A_STEP, A_SLEEP);
void setupChem_A(){
  A_stepper.begin(A_RPM, A_MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
     A_stepper.setEnableActiveState(HIGH);
}
////////////////////////
void Chem_A(int A_Chem){
      A_stepper.enable();
      delay(1000);
      Serial.println("Rotate");
//      A_stepper.rotate(3600);
      A_stepper.move(A_Chem*54L*A_MICROSTEPS); //KHL-SZ-S35 = 40, KHL-SZ-S24 = 54 these are constant forever
       A_stepper.disable();
     // delay(1000);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
//#include "BasicStepperDriver.h"
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define B_MOTOR_STEPS 200
#define B_RPM 350

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define B_MICROSTEPS 1

// All the wires needed for full functionality
#define B_DIR A7
#define B_STEP A6
//Uncomment line to use enable/disable functionality
#define B_SLEEP A2

BasicStepperDriver B_stepper(B_MOTOR_STEPS, B_DIR, B_STEP, B_SLEEP);
void setupChem_B(){
  B_stepper.begin(B_RPM, B_MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
     B_stepper.setEnableActiveState(HIGH);
}
////////////////////////
void Chem_B(int B_Chem){
      B_stepper.enable();
      delay(1000);
      Serial.println("Rotate");
//      B_stepper.rotate(3600);
      B_stepper.move(B_Chem*54L*B_MICROSTEPS); //KHL-SZ-S35 = 40, KHL-SZ-S24 = 54 these are constant forever
       B_stepper.disable();
     // delay(1000);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
//#include "BasicStepperDriver.h"
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define C_MOTOR_STEPS 200
#define C_RPM 350

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define C_MICROSTEPS 1

// All the wires needed for full functionality
#define C_DIR 48
#define C_STEP 46
//Uncomment line to use enable/disable functionality
#define C_SLEEP A8

BasicStepperDriver C_stepper(C_MOTOR_STEPS, C_DIR, C_STEP, C_SLEEP);
void setupChem_C(){
  C_stepper.begin(C_RPM, C_MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
     C_stepper.setEnableActiveState(HIGH);
}
////////////////////////
void Chem_C(int C_Chem){
      C_stepper.enable();
      delay(1000);
      Serial.println("Rotate");
//      C_stepper.rotate(3600);
      C_stepper.move(C_Chem*54L*C_MICROSTEPS); //KHL-SZ-S35 = 40, KHL-SZ-S24 = 54 these are constant forever
       C_stepper.disable();
     // delay(1000);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
//#include "BasicStepperDriver.h"
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define D_MOTOR_STEPS 200
#define D_RPM 350

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define D_MICROSTEPS 1

// All the wires needed for full functionality
#define D_DIR 28
#define D_STEP 26
//Uncomment line to use enable/disable functionality
#define D_SLEEP 24

BasicStepperDriver D_stepper(D_MOTOR_STEPS, D_DIR, D_STEP, D_SLEEP);
void setupChem_D(){
  D_stepper.begin(D_RPM, D_MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
     D_stepper.setEnableActiveState(HIGH);
}
////////////////////////
void Chem_D(int D_Chem){
      D_stepper.enable();
      delay(1000);
      Serial.println("Rotate");
//      D_stepper.rotate(3600);
      D_stepper.move(D_Chem*54L*D_MICROSTEPS); //KHL-SZ-S35 = 40, KHL-SZ-S24 = 54 these are constant forever
       D_stepper.disable();
     // delay(1000);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Servo.h>
Servo servo1;  // create servo object to control a servo
void setupWaterPump(){
   servo1.attach(2);  // attaches the servo on pin 2 to the servo object
//   servo1.writeMicroseconds(2000);
//   delay(500);
   servo1.writeMicroseconds(1000);
//   delay(500);
}
////////////////
void waterFill(unsigned long  water){
   servo1.writeMicroseconds(1800);
   delay(water*12L);//assume 4L per min -> 15000msec per 1000mL || 5L per min -> 12000msec per 1000mL
   servo1.writeMicroseconds(1000);   
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
void buzz_1(){
  digitalWrite(buzzer, HIGH);   
  delay(80);                      
  digitalWrite(buzzer, LOW);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void buzz_2(){
  digitalWrite(buzzer, HIGH);   
  delay(800);                      
  digitalWrite(buzzer, LOW);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void buzz_3(){
  digitalWrite(buzzer, HIGH);  
  delay(200);                       
  digitalWrite(buzzer, LOW);    
  delay(200);                       
  digitalWrite(buzzer, HIGH);   
  delay(200);                       
  digitalWrite(buzzer, LOW);   
  delay(200);                      
  digitalWrite(buzzer, HIGH);   
  delay(200);                       
  digitalWrite(buzzer, LOW);    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void buzz_4(){
   digitalWrite(buzzer, HIGH);  
  delay(150);                      
  digitalWrite(buzzer, LOW);   
  delay(15);                      
  digitalWrite(buzzer, HIGH);   
  delay(150);                       
  digitalWrite(buzzer, LOW);    
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void setup(void) {
   Serial.begin(250000);
   pinMode(buttonPin, INPUT_PULLUP);
  setupEnc();
  setupBuzzer();
  setupChem_A();
  setupChem_B();
  setupChem_C();
  setupChem_D();
  setupWaterPump();
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  pinMode(8, OUTPUT);
//------------------------------------------------
   u8g.firstPage();
   do{ //Start to draw first page
     draw_version();
   } while( u8g.nextPage() );

//------------------------------------------------
if(init_checking() == false){//normally within 3 sec.
       u8g.firstPage();
       do{
          draw_problem();
       } while( u8g.nextPage() );
       while(1){
        buzz_3(); 
        delay(1000);     
        // Error Wait for Restart
       }       
}

//------------------------------------------------
buzz_2(); //Ok start main process
delay(1000); 
}   
//------------------------------------------------
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void loop(void) {   
u8g.firstPage(); 
   do{
     draw(); // draw default 
   } while(u8g.nextPage());
//ready to get into operating state
//pass through Case 0
switch(buttonPushCounter){
   case 0:
           buzz_1();
           do{
                  int last_pseudo_counter0 = pseudo_counter0;
                  counterEnc0();                
                  if(pseudo_counter0 != last_pseudo_counter0){//update value specifically at c0
                    u8g.firstPage();
                    do{
                      draw();
                    } while(u8g.nextPage()); 
                  }
                  bpc();
           }
           while(buttonPushCounter == 0);//bpc change state
           
           break;
  case 1:
          buzz_1();
          do{
                  int last_pseudo_counter1 = pseudo_counter1;
                  counterEnc1();                 
                  if(pseudo_counter1 != last_pseudo_counter1){//update value specifically at c0
                    u8g.firstPage();
                    do{
                      draw();
                    } while(u8g.nextPage()); 
                  draw();}
                  bpc();
           }
           while(buttonPushCounter == 1);//bpc change state
           
           break;
  case 2:
          buzz_1();
          do{
                  int last_pseudo_counter2 = pseudo_counter2;
                  counterEnc2();                 
                  if(pseudo_counter2 != last_pseudo_counter2){//update value specifically at c0
                    u8g.firstPage();
                    do{
                      draw();
                    } while(u8g.nextPage()); 
                  draw();}
                  bpc();
           }
           while(buttonPushCounter == 2);//bpc change state
           
           break;
  case 3:
          buzz_1();
          do{
                  int last_pseudo_counter3 = pseudo_counter3;
                  counterEnc3();                 
                  if(pseudo_counter3 != last_pseudo_counter3){//update value specifically at c0
                    u8g.firstPage();
                    do{
                      draw();
                    } while(u8g.nextPage()); 
                  draw();}
                  bpc();
           }
           while(buttonPushCounter == 3);//bpc change state
           
           break;                  
  case 9:
            buzz_2();
            //while(1);
            //Begin calculation and calibration
          if(process_checking() == false){//normally within 3 sec. //True when normal
            u8g.firstPage();
            do{
              draw_problem();
            } while( u8g.nextPage() );
            while(1){
            buzz_3();
            delay(500);      
            // Error Wait for Restart
            }       
          }
          
              buzz_1();
              u8g.firstPage();
              do{
                draw_filling(); //start filling process
              } while( u8g.nextPage() );
              delay(3000);
              waterFill(2000);
              buzz_1();
              Chem_A(a0);
              buzz_1();
              waterFill(2000);
              buzz_1();
              Chem_B(a1);
              buzz_1();
              waterFill(aw-2000-2000);

//              while(process_post() == true){
//                //any process here... cleaing, etc.
//              }
              u8g.firstPage();
              do{
                draw_done(); //finish filling process
              } while( u8g.nextPage() );
//              while(bpc() == false){
//                nothing but waiting
              buzz_4();
              delay(500);
          while(bpc() == false){ // do until short press occurs
                Serial.println("Done");
          }
          break;
          
default: //Case which not present in any 0-9
       u8g.firstPage();
       do{
          draw_problem();
       } while( u8g.nextPage() );
       buzz_3();
       delay(500);
       break;
     }
}
