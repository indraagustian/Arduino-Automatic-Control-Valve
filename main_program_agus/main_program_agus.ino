#include <SimpleKalmanFilter.h>
const int T0=4;
const int E0=2; 
const int T1=6;
const int E1=8;

/////////Distance measurement var/////////////////////
const int numReadings = 10;
unsigned int readings[numReadings];
unsigned int readIndex = 0;              // the index of the current reading
unsigned int total = 0;                  // the running total
unsigned int average = 0;                // the average
int jarak_0, jarak_1;

SimpleKalmanFilter simpleKalmanFilter0(2, 2, 0.01);
SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.01);
int dmin=25;
int dmax=150;// distance Threshold;
int dstate_0, dstate_1;
int con_0, con_1, cof_0, cof_1;


//relay
const int rel0=10;
const int rel1=11;

//timer
unsigned long time_0;
unsigned long time_1;
unsigned long interval_on=40;//40
unsigned long interval_off=40;//20



void setup() { 
  Serial.begin(9600); // Serial monitoring 
  //distance sensor
  Serial.println("Keran otomatis");
  pinMode(T0, OUTPUT); // Initializing Trigger Output and Echo Input 
  pinMode(E0, INPUT_PULLUP);
  pinMode(T1, OUTPUT); // Initializing Trigger Output and Echo Input 
  pinMode(E1, INPUT_PULLUP);
  //relay
  pinMode(rel0,OUTPUT);
  pinMode(rel1,OUTPUT);
  digitalWrite(rel0,HIGH);
  digitalWrite(rel1,HIGH);
}

void loop(){
 jarak_0=ukur_jarak(T0,E0);
 int kalman_0=simpleKalmanFilter0.updateEstimate(jarak_0);
 jarak_1=ukur_jarak(T1,E1);
 int kalman_1=simpleKalmanFilter1.updateEstimate(jarak_1);
// Serial.print(kalman_0); //bacaan real
// Serial.print("\t");
// Serial.print(kalman_1); //bacaan real
// Serial.print("\t");
// Serial.println();

////////////activate relay 0
 if(kalman_0>dmin && kalman_0<dmax){
    cof_0=0;
    if(dstate_0==0)con_0++;
    if(con_0>interval_on){
      dstate_0=1;
      con_0=0;
      Serial.print("keran1:on ");
      //Serial.print(dstate_0);
      Serial.print(" jarak: ");
      Serial.println(kalman_0);
      digitalWrite(rel0,LOW);
    }
  }
 else if(kalman_0<=dmin || kalman_0>=dmax){
    con_0=0;
    if(dstate_0==1)cof_0++;
    if(cof_0>interval_off){
      dstate_0=0;
      cof_0=0;
      Serial.print("keran1:off ");
      //Serial.print(dstate_0);
      Serial.print(" jarak: ");
      Serial.println(kalman_0);
      digitalWrite(rel0,HIGH);
    }
 }

////////////activate relay 1
 if(kalman_1>dmin && kalman_1<dmax){
    cof_1=0;
    if(dstate_1==0)con_1++;
    if(con_1>interval_on){
      dstate_1=1;
      con_1=0;
      Serial.print("keran2:on ");
      //Serial.print(dstate_1);
      Serial.print(" jarak: ");
      Serial.println(kalman_1);
      digitalWrite(rel1,LOW);
    }
  }
 else if(kalman_1<=dmin || kalman_1>=dmax){
    con_1=0;
    if(dstate_1==1)cof_1++;
    if(cof_1>interval_off){
      dstate_1=0;
      cof_1=0;
      Serial.print("keran2:off  ");
      //Serial.print(dstate_1);
      Serial.print(" jarak: ");
      Serial.println(kalman_1);
      digitalWrite(rel1,HIGH);
    }
 }
}

int ukur_jarak(int TRIG,int ECHO){  //fungsi mengukur jarak
    int distance;
    digitalWrite(TRIG, LOW); // Set the trigger pin to low for 2uS 
    delayMicroseconds(10); 
    
    digitalWrite(TRIG, HIGH); // Send a 10uS high to trigger ranging 
    delayMicroseconds(20); 
    
    digitalWrite(TRIG, LOW); // Send pin low again 
    distance = pulseIn(ECHO, HIGH,26000); // Read in times pulse 
    
    distance= distance/58; //Convert the pulse duration to distance
                           //You can add other math functions to calibrate it well
    return distance;
    delay(50);
}




