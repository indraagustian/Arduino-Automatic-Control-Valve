/* This code works with JSN SR04 T ultrasound module
 * It measures the distance and shows it on the Serial monitor
 * Refer to www.SurtrTech. com or SurtrTech YT channel for more informations 
 */
#include <SimpleKalmanFilter.h>
#define TRIG 6 //Module pins // ping 1 4
#define ECHO 8 // ping 1 2
int distance;
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

void setup() { 
  
  Serial.begin(9600); // Serial monitoring 
  Serial.println("distance,smooth,kalman");
  pinMode(TRIG, OUTPUT); // Initializing Trigger Output and Echo Input 
  pinMode(ECHO, INPUT_PULLUP);

    // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    }
  } 
  
void loop() { 
   meas_distance();
    
}

void meas_distance(){  //fungsi mengukur jarak
    digitalWrite(TRIG, LOW); // Set the trigger pin to low for 2uS 
    delayMicroseconds(10); 
    
    digitalWrite(TRIG, HIGH); // Send a 10uS high to trigger ranging 
    delayMicroseconds(20); 
    
    digitalWrite(TRIG, LOW); // Send pin low again 
    distance = pulseIn(ECHO, HIGH,26000); // Read in times pulse 
    
    distance= distance/58; //Convert the pulse duration to distance
                           //You can add other math functions to calibrate it well
    int new_distance=smoothing_read(distance);      
    int kalman_distance=simpleKalmanFilter.updateEstimate(distance); 
                    
    //Serial.print("Distance ");

    Serial.print(distance); //bacaan real
    Serial.print("\t");
    
    Serial.println(new_distance); //bacaan di-smoothing
    Serial.print("\t");
     
    Serial.print(kalman_distance); //bacaan dengan kalman filter  
    Serial.print("\t");
    //Serial.println();
    delay(50);
}

int smoothing_read(int x) {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = x;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  return average;
  //Serial.println(average);
  delay(1);        // delay in between reads for stability
}

