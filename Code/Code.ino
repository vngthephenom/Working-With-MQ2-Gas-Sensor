#include<math.h>
//Find Ro by Running the following code in clean air, and R1 using a multimeter
float Ro = 98800, Vref = 5.0, R1 = 7700;
// Change the following values for different gasses from the corresponding curves
float ltx[] = {200, 500, 800, 1000, 1600, 2000, 3000, 5000, 10000};
float lty[] = {1.67, 1.11, 0.88, 0.78, 0.64, 0.56, 0.46, 0.36, 0.26};
int i, j, t;
int raw[100];      
void setup() {
  // Initializing serial port
  Serial.begin(9600);
  
}
void loop() {
  float gasConcentration, Vs, Rs, slope, Y, Yo, Xo;
  // read 100 Values
  for(j=0; j<100; j++){
    raw[j]=analogRead(A0);      
  }
  //Find the median value
  for(i=0; i<100; i++){      
    for(j=0; j<100-i-1; j++){      
        if(raw[j]<raw[j+1]){
          t=raw[j];      
          raw[j]=raw[j+1];      
          raw[j+1]=t;      
        }
    }
  }
  Vs = Vref - raw[50] * Vref / 1024.0;
  Serial.print("Voltage: ");
  Serial.println(Vs);
  //Find Resistance of the sensor
  Rs = (R1 * Vs) / (Vref - Vs);
  Serial.print("resistance: ");
  Serial.println(Rs);
  //Find the Rs/Ro Ratio
  Y = Rs / Ro;
  Serial.print("RS/RO: ");
  Serial.println(Y);
  Serial.print("Concentration: ");
  //Find the slope and Initial Values usiong the ratio
  slope = find_slope(Y);
  Xo = find_Xo(Y);
  Yo = find_Yo(Y);
  //Find the concentration and Limit the Output to MQ2's range
  gasConcentration = Xo * pow(Y / Yo, slope);
  if (gasConcentration > 10000.00) {
    gasConcentration = 10000.00;
  }
  else if (gasConcentration < 200.00) {
    gasConcentration = 200.00;
  }
  Serial.println(gasConcentration);

  delay(100);
}
float find_slope(float y) {
  for (i = 0; i < 8; i++) {
    if (y < lty[i] && y > lty[i + 1]) {
      break;
    }
  }
  if (y > lty[0]) {
    i=0;
  }
  if (i >= 8) {
    i = 7;
  }
  return log(lty[i + 1] / lty[i]) / log(ltx[i + 1] / ltx[i]);
}
float find_Xo(float y) {
  for (i = 0; i < 8; i++) {
    if (y < lty[i] && y > lty[i + 1]) {
      break;
    }
  }
  if (y > lty[0]) {
    i=0;
  }
  if (i >= 8) {
    i = 7;
  }
  return ltx[i];
}
float find_Yo(float y) {
  for (i = 0; i < 8; i++) {
    if (y < lty[i] && y > lty[i + 1]) {
      break;
    }
  } 
  if (y > lty[0]) {
    i=0;
  }
  if (i >= 8) {
    i = 7;
  }
  return lty[i];
}

