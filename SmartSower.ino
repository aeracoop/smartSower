#include <Servo.h>

// Dronecoria Smart Sower
// by Lot Amoros
// License GPL v3

//Download and print the hardware:
//https://wikifactory.com/@aeracoop/dronecoria/files/seedDispenser

//PPM Decode from rinozen:
//https://gist.github.com/rinozen/ab73d79dbe7be05d6adceb85872da84a

//PPM Input conected to PIN2
//PPM Channel 6: Sowing rate.
//PPM Channel 7: Sowing distance.

//PWM Output Sowing Rate Servo to D9
//PWM Output Sowing Distance Motor to D6

const int minRC = 1200; //value to trigger betwen Low & Med
const int maxRC = 1800; //value to trigger betwen Med & High

int rate1 = 35; //Rate 1 of seeds
int rate2 = 30; //Rate 2 of seeds

Servo servoRate; // Object Servo from library

unsigned long int ft, lt, x, tfail;
int ch, chx[9][13];

int rate = 0;
int dist = 0;
int zero = 0;

const int idx   = 10;
const int total = 11;
const int val   = 12;
const int dist1 = 100;
const int dist2 = 200;

void setup() {
  Serial.begin(9600);
  servoRate.attach(9);  // attach servo to digital pin 9

  //If servo works counterwise:
  rate1 = 180 - rate1;
  rate2 = 180 - rate2;
  zero = 180;

  inizializeVectors();
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), decodePPM, FALLING);
  servoRate.write(zero); //Close the door
}

void test3x3() {
  setServo(rate1, 3000);
  servoRate.write(zero);
  delay(5000);

  setServo(rate1, 3000);
  servoRate.write(zero);
  delay(5000);

  setServo(rate1, 3000);
  servoRate.write(zero);
  delay(5000);
  delay(500000);
}
}

void inizializeVectors() {
  ft = 0; lt = 0; x = 0; tfail = 0; ch = 0;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 13; j++) {
      chx[i][j] = 0;
    }
  }
}

//This function adds artifical noise in order the facilitate the fall of the seeds.
void setServo(int s, int time)
{
  int t = millis();
  do
  {
    servoRate.write(rate1 - 5);
    delay(200);
    servoRate.write(rate1);
    delay(200);
    servoRate.write(rate1 - 5);
    delay(200);
  } while ((millis() - t) < time);
}

void loop() {
  if ((millis() - tfail) > 500) {
    Serial.println("Disconnect");
  } else {
    if (chx[6][val] < minRC)
      rate = 0;
    if (chx[6][val] > minRC && chx[6][val] < maxRC)
      rate = rate1;
    if (chx[6][val] > maxRC)
      rate = rate2;
    servoRate.write(rate);

    Serial.print("CH7: "); Serial.print(chx[6][val]);
    Serial.print("Rate: "); Serial.print(rate);   Serial.print("\t");

    if (chx[7][val] < minRC)
      dist = 0;
    if (chx[7][val] > minRC && chx[7][val] < maxRC)
      dist = dist1;
    if (chx[7][val] > maxRC)
      dist = dist2;

    Serial.print("CH8: "); Serial.print(chx[7][val]);
    Serial.print("Dist: "); Serial.print(dist);
    Serial.print("\t");
    Serial.println();
  }
}

void decodePPM() {
  lt    = micros();
  tfail = millis();
  x     = lt - ft;
  ft    = lt;
  if (x > 3000) {
    ch          = 0;
    chx[0][val] = x;
  } else {
    ch += 1;
    int indx       = chx[ch][idx];
    chx[ch][total] = chx[ch][total] - chx[ch][indx];
    chx[ch][indx]  = x;
    chx[ch][total] = chx[ch][total] + chx[ch][indx];
    chx[ch][idx]   = chx[ch][idx] + 1;
    if (chx[ch][idx] > 9) {
      chx[ch][idx] = 0;
    }
    chx[ch][val] = chx[ch][total] / 10;
  }
}


/*Serial.print("CH1: "); Serial.print(chx[0][val]); Serial.print("\t");
  Serial.print("CH2: "); Serial.print(chx[1][val]); Serial.print("\t");
  Serial.print("CH3: "); Serial.print(chx[2][val]); Serial.print("\t");
  Serial.print("CH4: "); Serial.print(chx[3][val]); Serial.print("\t");
  Serial.print("CH5: "); Serial.print(chx[4][val]); Serial.print("\t");
  Serial.print("CH6: "); Serial.print(chx[5][val]); Serial.print("\t");
  Serial.print("CH9: ");Serial.println(chx[8][val]);*/
