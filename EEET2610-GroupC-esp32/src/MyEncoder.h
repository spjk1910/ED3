#include <Arduino.h>

#define ENC_RES 330 // Encoder resolution*Gearbox ratio: 11*30

#define ENC1_A 17    // MOT 1A
#define ENC1_B 18   // MOT 1B
volatile long int cnt1 = 0; // Volatile as it changed during interrupt
double th1 = 0;             // Position angle in degrees

#define ENC2_A 26    // MOT 2A
#define ENC2_B 25   // MOT 2B
volatile long int cnt2 = 0; // Volatile as it changed during interrupt
double th2 = 0;             // Position angle in degrees

#define ENC3_A 16    // MOT 3A
#define ENC3_B 4   // MOT 3B
volatile long int cnt3 = 0; // Volatile as it changed during interrupt
double th3 = 0;             // Position angle in degrees

//================================================================================
void readEncoder1()
{
    int b1 = digitalRead(ENC1_B);
    cnt1 = (b1 > 0) ? (cnt1 + 1) : (cnt1 - 1);
}

void readEncoder2()
{
    int b2 = digitalRead(ENC2_B);
    cnt2 = (b2 > 0) ? (cnt2 + 1) : (cnt2 - 1);
}

void readEncoder3()
{
    int b3 = digitalRead(ENC3_B);
    cnt3 = (b3 > 0) ? (cnt3 + 1) : (cnt3 - 1);
}

void Init_Encoder()
{
    pinMode(ENC1_A, INPUT);
    pinMode(ENC1_B, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENC1_A), readEncoder1, RISING);

    pinMode(ENC2_A, INPUT);
    pinMode(ENC2_B, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENC2_A), readEncoder2, RISING);

    pinMode(ENC3_A, INPUT);
    pinMode(ENC3_B, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENC3_A), readEncoder3, RISING);
}

void Get_Angle()
{
    th1 = cnt1 * 360 / ENC_RES; // Conversion between encoder count and degree
    th2 = cnt2 * 360 / ENC_RES; // Conversion between encoder count and degree
    th3 = cnt3 * 360 / ENC_RES; // Conversion between encoder count and degree
}