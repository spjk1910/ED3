#include <Arduino.h>

unsigned long Serial_time = 0; // time in us
double th1_ref = 10;
double th2_ref = 10;
double th3_ref = 10;

void SerialDataPrintHelper(unsigned long Serial_time, double MOT_cmd, double th_ref, double th);

// ====================================================================================
void Init_Serial()
{
  Serial.begin(115200);
  while (!Serial)
    ;
}
// ====================================================================================
void SerialDataPrint()
{
  if (micros() - Serial_time >= 1000000)
  {
    Serial_time = micros();

    // Motor 1
    Serial.print("Motor 1: ");
    SerialDataPrintHelper(Serial_time, MOT1_cmd, th1_ref, th1);
    
    // Motor 2
    Serial.print("Motor 2: ");
    SerialDataPrintHelper(Serial_time, MOT2_cmd, th2_ref, th2);
    
    // Motor 3
    Serial.print("Motor 3: ");
    SerialDataPrintHelper(Serial_time, MOT3_cmd, th3_ref, th3);
  }
}
// ====================================================================================
void SerialDataWrite()
{
  static String received_chars;
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    received_chars += inChar;
    if (inChar == '\n')
    {
      switch (received_chars[0])
      {
      case 'a':
        received_chars.remove(0, 1);
        th1_ref = received_chars.toFloat();
        break;
      case 's':
        received_chars.remove(0, 1);
        th2_ref = received_chars.toFloat();
        break;
      case 'd':
        received_chars.remove(0, 1);
        th3_ref = received_chars.toFloat();
        break;
      case 'q':
        received_chars.remove(0, 1);
        kp = received_chars.toFloat();
        break;
      case 'w':
        received_chars.remove(0, 1);
        ki = received_chars.toFloat();
        break;
      case 'e':
        received_chars.remove(0, 1);
        kd = received_chars.toFloat();
        break;
      default:
        break;
      }
      received_chars = "";
    }
  }
}

void SerialDataPrintHelper(unsigned long Serial_time, double MOT_cmd, double th_ref, double th){
  // For MATLAB
  Serial.print(Serial_time / 10000);
  Serial.print(",");
  Serial.print(MOT_cmd);
  Serial.print(",");
  Serial.print(th_ref);
  Serial.print(",");
  Serial.print(th);
  Serial.println();

  // For Teleplot
  Serial.println(Serial_time / 10000);
  Serial.print(">MOT_cmd:");
  Serial.println(MOT_cmd);
  Serial.print(">th:");
  Serial.println(th);
  Serial.print(">th_ref:");
  Serial.println(th_ref);
  Serial.print(">kp:");
  Serial.println(kp);
  Serial.print(">ki:");
  Serial.println(ki);
  Serial.print(">kd:");
  Serial.println(kd);

  delay(20);
}