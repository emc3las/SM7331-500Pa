//
// Minimal sketch for reading pressure from SM7331-BCE-S-500.00-351
//
#include <Wire.h>
double dp0 = 0; // to store the zero drift
 
void setup() {
  Wire.begin();
  Serial.begin(9600);
  dp0 = readDpSM(200); // these initial readings should be done with zero pressure difference applied
}
 
void loop() {
  Serial.println(readDpSM(100) - dp0);
}

#define SM_DP_MAX 500.0
#define SM_DP_MIN -500.0
#define SM_DFS 65535.0
#define SM_ADD 0x6C
#define SM_DP_ADD 0x30

double readDpSM(int nSamples) {
  int16_t dout;
  uint8_t lo_byte;
  int8_t hi_byte;
  double dp = 0.0;

  Wire.beginTransmission(byte(SM_ADD));
  Wire.write(byte(SM_DP_ADD));
  Wire.endTransmission();
  delay(50);
  for (int i=0; i<nSamples; i++) {
      Wire.requestFrom(byte(SM_ADD), 2);
      lo_byte = Wire.read(); // receive low byte as lower 8 bits (unsigned)
      hi_byte = Wire.read(); // receive high byte (signed)
      dout = (hi_byte << 8) | lo_byte; // shift high byte to be high 8 bits and completes the reading with low byte
      dp += (SM_DP_MAX - SM_DP_MIN)/nSamples*double(dout)/SM_DFS;
  }
  return(dp + 0.5*(SM_DP_MAX + SM_DP_MIN)); // pressure difference in Pa (averaged over nSamples samples)
}
