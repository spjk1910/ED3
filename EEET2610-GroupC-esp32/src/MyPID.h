#include <Arduino.h>
#include <PID_v1.h>

extern double th1, th2, th3, MOT1_cmd, MOT2_cmd, MOT3_cmd;
extern double th1_ref, th2_ref, th3_ref;
double kp = 10.0, ki = 5.0, kd = 0.05; // dinh-son, works for 1 motor
// double kp = 10.0, ki = 10.0, kd = 0.05; // dinh-son, works for 1 motor
// double kp = 5.0, ki = 5.0, kd = 0.05;
// double kp = 150, ki = 5, kd = 10; // dat

// Input, output, reference
PID MOT1_PID(&th1, &MOT1_cmd, &th1_ref, kp, ki, kd, DIRECT);
PID MOT2_PID(&th2, &MOT2_cmd, &th2_ref, kp, ki, kd, DIRECT);
PID MOT3_PID(&th3, &MOT3_cmd, &th3_ref, kp, ki, kd, DIRECT);

// ================================================================
// Function Definition
// ================================================================
void Init_PID()
{
  MOT1_PID.SetMode(AUTOMATIC);
  MOT1_PID.SetOutputLimits(-255, 255);
  MOT1_PID.SetSampleTime(10);

  MOT2_PID.SetMode(AUTOMATIC);
  MOT2_PID.SetOutputLimits(-255, 255);
  MOT2_PID.SetSampleTime(10);

  MOT3_PID.SetMode(AUTOMATIC);
  MOT3_PID.SetOutputLimits(-255, 255);
  MOT3_PID.SetSampleTime(10);
}
// ================================================================
void Compute_PID()
{
  MOT1_PID.SetTunings(kp, ki, kd);
  MOT1_PID.Compute();

  MOT2_PID.SetTunings(kp, ki, kd);
  MOT2_PID.Compute();

  MOT3_PID.SetTunings(kp, ki, kd);
  MOT3_PID.Compute();
}
// ================================================================
