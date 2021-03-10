#define         PWM_B_PIN           9
#define         ENABLE_MT_B_HPIN    8
#define         ENABLE_MT_B_LPIN    7

#define         PWM_A_PIN           11
#define         ENABLE_MT_A_HPIN    12
#define         ENABLE_MT_A_LPIN    13

#define         YELLOW_PUMP_PIN     10

#define         FlowSensor_PIN      2

#define         MAX_FLOW_RATE       255
#define         MIN_FLOW_RATE       0
#define         Flow_Frequency      7.5
#define         Flow_Rate_PWM       103     //For Flow rate 1 L/Min

#include        <Servo.h>

////////////Motor Drive Request////////////////////
int             incomeint;
int             FlowUpdate;
int             FlowRequest =   0;
int             Start_Sts   =   0;
int             MT_A_Sts    =   0;
int             MT_B_Sts    =   0;

unsigned long   WATER_A     =   2000;
unsigned long   WATER_B     =   500;
unsigned long   Timer_A;
unsigned long   Timer_B;

String          incomebyte;

Servo           PumpMtrDrive;

////////////Flow Sensor////////////////////////////
int             FlowRate    =   0;
volatile int    Flow_Counter;

unsigned long   t_currenTime;
unsigned long   t_loopTime;

unsigned long   t_Motor_CurrentTime;
unsigned long   t_Motor_StartTime;


////////////Code Start Here////////////////////////
void setup() {

    /////////Water Pump////////////////////////////
    PumpMtrDrive.attach(YELLOW_PUMP_PIN);
    PumpMtrDrive.writeMicroseconds(1000);

    /////////Flow sensor///////////////////////////
    pinMode(FlowSensor_PIN, INPUT);
    digitalWrite(FlowSensor_PIN, HIGH); //Internal Pull-UP//

    Serial.begin(9600);

    /////////Motor DC////////////////////////////
    pinMode(PWM_A_PIN, OUTPUT);
    pinMode(ENABLE_MT_A_HPIN, OUTPUT);
    pinMode(ENABLE_MT_A_LPIN, OUTPUT);
    //digitalWrite(ENABLE_MT_A_HPIN, HIGH);
    //digitalWrite(ENABLE_MT_A_HPIN, LOW);

    pinMode(PWM_B_PIN, OUTPUT);
    pinMode(ENABLE_MT_B_HPIN, OUTPUT);
    pinMode(ENABLE_MT_B_LPIN, OUTPUT);
    //digitalWrite(ENABLE_MT_B_HPIN, HIGH);
    //digitalWrite(ENABLE_MT_B_HPIN, LOW);
  
    
    /////////Set up Interupt///////////////////////
    attachInterrupt(0, Flow_PulseCounter, RISING);
    sei();
    t_currenTime    =   millis();
    t_loopTime      =   t_currenTime;
}

void loop() {

    t_Motor_CurrentTime     =   millis();
    Timer_A                 =   WATER_A*60;
    Timer_B                 =   WATER_B*60;

    //////////Recieve new flow/////////////////////
    if (Serial.available())
    {   
        

        incomebyte  =   Serial.readString();
        incomeint   =   incomebyte.toInt();
        
        //UpdateFlow();
        if (incomeint != FlowUpdate)
        {
            FlowUpdate =   incomeint;
            UpdateFlow();
        }

        
        if((incomeint != 0) && (Start_Sts == 0))
        {
            Start_Sts           =   1;
            MT_A_Sts            =   1;
            MT_B_Sts            =   1;
            t_Motor_StartTime   =   millis();
        }

        else if((incomeint == 0))
        {
            Start_Sts           =   0;
            MT_A_Sts            =   0;
            MT_B_Sts            =   0;
            t_Motor_StartTime   =   millis();
        }
    }

    //Serial.println("Flow Request = " + String(FlowRequest));
    Serial.println("Flow Request = 103");
    Serial.println("Start_Sts = " + String(Start_Sts));
    Serial.println("MT_A_Sts = " + String(MT_A_Sts));
    Serial.println("MT_B_Sts = " + String(MT_B_Sts));
    Serial.println("Timer_A = " + String(Timer_A));
    Serial.println("Timer_B = " + String(Timer_B));
    Serial.println("t_Motor_CurrentTime = " + String(t_Motor_CurrentTime));
    Serial.println("t_Motor_StartTime = " + String(t_Motor_StartTime));
    Serial.println("Diff = " + String(t_Motor_CurrentTime - t_Motor_StartTime));

    Water_Request(FlowRequest);
    
    if(Start_Sts == 1){
    //////////Motor Request/////////////////////
    digitalWrite(ENABLE_MT_A_HPIN, HIGH);
    digitalWrite(ENABLE_MT_A_LPIN, LOW);
    digitalWrite(ENABLE_MT_B_HPIN, HIGH);
    digitalWrite(ENABLE_MT_B_LPIN, LOW);   

    /*/////For Brushless//////
    MotorRequest(FlowRequest);
    */

        if(t_Motor_CurrentTime < (t_Motor_StartTime + Timer_A)){
            Motor_A_Request(Flow_Rate_PWM);     
        }
        else if(t_Motor_CurrentTime > (t_Motor_StartTime + Timer_A))
        {
            analogWrite(PWM_A_PIN ,0);
            Serial.println("Pump A finished ........................................");
            MT_A_Sts    =   0;
        }

        if(t_Motor_CurrentTime < (t_Motor_StartTime + Timer_B)){
            Motor_B_Request(Flow_Rate_PWM);     
        }
        else if(t_Motor_CurrentTime > (t_Motor_StartTime + Timer_B))
        {
            analogWrite(PWM_B_PIN ,0);
            Serial.println("Pump B finished ........................................");
            MT_B_Sts    =   0;
        }    

    /*//////////Flow Sensor Calculate/////////////
    Flow_Calculator();
    //Serial.println("Flow Rate = " + String(FlowRate));*/
        if ((MT_A_Sts == 0) && (MT_B_Sts == 0))
        {
            Start_Sts   =   0;
        }
        
    }
    
    else
    {
    digitalWrite(ENABLE_MT_A_HPIN, LOW);
    digitalWrite(ENABLE_MT_A_LPIN, LOW);
    digitalWrite(ENABLE_MT_B_HPIN, LOW);
    digitalWrite(ENABLE_MT_B_LPIN, LOW);
    analogWrite(PWM_A_PIN ,0);
    analogWrite(PWM_B_PIN ,0);    
    }
    
}

void UpdateFlow(){
    //////////Delay Update//////////////////////
    Serial.println("Recieving new flow....");
    delay(500);
    Serial.println("...3");
    //delay(1000);
    Serial.println("...2");
    //delay(1000);
    Serial.println("...1");
    //delay(1000);
    FlowRequest = UpperLowerLimit(FlowUpdate, MIN_FLOW_RATE, MAX_FLOW_RATE);
    Serial.println("Flow Updated");
}

void Motor_A_Request(int flow){
    int PWM_Request =   map(flow, MIN_FLOW_RATE, MAX_FLOW_RATE, 0, 255);
    //PumpMtrDrive.writeMicroseconds(PWM_Request);
    //Serial.println("PWM Request = " + String(PWM_Request));
    analogWrite(PWM_A_PIN ,PWM_Request);
}

void Motor_B_Request(int flow){
    int PWM_Request =   map(flow, MIN_FLOW_RATE, MAX_FLOW_RATE, 0, 255);
    //PumpMtrDrive.writeMicroseconds(PWM_Request);
    //Serial.println("PWM Request = " + String(PWM_Request));
    analogWrite(PWM_B_PIN ,PWM_Request);
}

void Water_Request(int flow){
    int PWM_Request =   map(flow, MIN_FLOW_RATE, MAX_FLOW_RATE, 1000, 1800);
    PumpMtrDrive.writeMicroseconds(PWM_Request);
    Serial.println("PWM Request = " + String(PWM_Request));
    //analogWrite(PWM_B_PIN ,PWM_Request);
}

void Flow_PulseCounter(){
    Flow_Counter++;
}

void Flow_Calculator(){
    
    //Serial.println("Start Calculate Flow Sensor")
    t_currenTime    =   millis();

    if(t_currenTime >= (t_loopTime + 1000)){
        t_loopTime      =   t_currenTime;
        FlowRate        =   Flow_Counter/Flow_Frequency;
        Flow_Counter    =   0;
    }
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
