#include <SimpleKalmanFilter.h>
const int T0=4;
const int E0=2; 
const int T1=6;
const int E1=8;


const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

SimpleKalmanFilter simpleKalmanFilter0(2, 2, 0.01);
SimpleKalmanFilter simpleKalmanFilter1(2, 2, 0.01);

void setup() { 
  
  Serial.begin(9600); // Serial monitoring 
  Serial.println("distance,smooth,kalman");
  pinMode(T0, OUTPUT); // Initializing Trigger Output and Echo Input 
  pinMode(E0, INPUT_PULLUP);
  pinMode(T1, OUTPUT); // Initializing Trigger Output and Echo Input 
  pinMode(E1, INPUT_PULLUP);
}

void loop(){
 int jarak_0, jarak_1;
 jarak_0=ukur_jarak(T0,E0);
 int kalman_0=simpleKalmanFilter0.updateEstimate(jarak_0);
 jarak_1=ukur_jarak(T1,E1);
 int kalman_1=simpleKalmanFilter1.updateEstimate(jarak_1);
 Serial.print(kalman_0); //bacaan real
 Serial.print("\t");
 Serial.print(kalman_1); //bacaan real
 Serial.print("\t");
 Serial.println();
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
    //int kalman_distance=simpleKalmanFilter.updateEstimate(distance); 
                    
    //Serial.print("Distance ");

    //Serial.print(distance); //bacaan real
    //Serial.print("\t");
        
    //Serial.print(kalman_distance); //bacaan dengan kalman filter  
    //Serial.print("\t");
    //Serial.println();
    return distance;
    delay(50);
}


