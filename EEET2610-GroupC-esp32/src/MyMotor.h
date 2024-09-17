#include <Arduino.h>

#define MOT1_A 19 // MOT 1
#define MOT1_B 21 // MOT 1B

#define MOT2_A 33 // MOT 2A
#define MOT2_B 32 // MOT 2B

#define MOT3_A 14 // MOT 3A
#define MOT3_B 27 // MOT 3B

#define MOT1_Channel 0 // MOT 1 channel
#define MOT2_Channel 1 // MOT 2 channel
#define MOT3_Channel 2 // MOT 3 channel

#define PWM_FREQ 10000 // PWM Frequency: 10kHz
#define PWM_RES 8      // PWM resolution 255

double MOT1_cmd = 10; // MOT1 command [-255; 255]
double MOT2_cmd = 10; // MOT2 command [-255; 255]
double MOT3_cmd = 10; // MOT3 command [-255; 255]

//============================================================
void Init_Motor()
{
    pinMode(MOT1_A, OUTPUT);
    pinMode(MOT1_B, OUTPUT);

    pinMode(MOT2_A, OUTPUT);
    pinMode(MOT2_B, OUTPUT);
    
    pinMode(MOT3_A, OUTPUT);
    pinMode(MOT3_B, OUTPUT);

    ledcSetup(MOT1_Channel, PWM_FREQ, PWM_RES);
    ledcSetup(MOT2_Channel, PWM_FREQ, PWM_RES);
    ledcSetup(MOT3_Channel, PWM_FREQ, PWM_RES);
}
//============================================================
void Send_PWM(int PINA, int PINB, double mot_cmd, int channel)
{
    // Reverse direction, if the mot_cmd is from [-255; 0]
    if (mot_cmd < 0)
    {
        ledcAttachPin(PINB, channel);
        ledcDetachPin(PINA);
        digitalWrite(PINA, LOW);
        ledcWrite
        (channel, abs(mot_cmd));
    }
    // Forward direction, if the mot_cmd is from [0; 255]; 
    else 
    {
        ledcAttachPin(PINA, channel);
        ledcDetachPin(PINB);
        digitalWrite(PINB, LOW);
        ledcWrite(channel, abs(mot_cmd));
    }
}

//============================================================
void Run_Motor()
{
    Send_PWM(MOT1_A, MOT1_B, MOT1_cmd, MOT1_Channel);
    Send_PWM(MOT2_A, MOT2_B, MOT2_cmd, MOT2_Channel);
    Send_PWM(MOT3_A, MOT3_B, MOT3_cmd, MOT3_Channel);
}