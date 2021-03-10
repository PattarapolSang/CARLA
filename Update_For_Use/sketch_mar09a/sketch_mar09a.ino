/*
  การใช้งาน Code Water flow Sensor กับ Arduino
  บทความ : https://www.ab.in.th/b/8
*/
volatile int flow_frequency;
volatile int flow_frequency_A;
unsigned int l_hour, f_val;
unsigned int l_hour_A, f_val_A;
unsigned long ctTime, flow_val, cloopTime;

void flow () //  Water flow Interrupt function
{
  flow_frequency++;  f_val++;
}

void flow_A () //  Water flow Interrupt function
{
  flow_frequency_A++;  f_val_A++;
}

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(2), flow, RISING);  
  attachInterrupt(digitalPinToInterrupt(3), flow_A, RISING);
  sei();   ctTime = millis();  cloopTime = ctTime; // Water flow
  
}

void loop () {
  ctTime = millis();
  if (ctTime >= (cloopTime + 1000)) {
    cloopTime = ctTime;
    l_hour = (flow_frequency * 60 / 7.5);
    flow_frequency = 0;
    Serial.print("Water Sensor_A : ");
    Serial.print(l_hour, DEC);
    Serial.print(" L/hour       ");

    l_hour_A = (flow_frequency_A * 60 / 7.5);
    flow_frequency_A = 0;
    Serial.print("Water Sensor_B : ");
    Serial.print(l_hour_A, DEC);
    Serial.println(" L/hour");
  }

  if (f_val >= 450) {
    flow_val++;
    f_val = 0;
  }
  //Serial.print("\t ");
  //Serial.print(flow_val);
  //Serial.println("L");
}
