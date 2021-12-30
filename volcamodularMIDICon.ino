
#include "MIDIUSB.h"

//アナログ出力できる分解能を計算する。
//3.3v / 1024 = 0.00322265625
//ピッチを送るための線は0番のピン
#define CV_PIN A0

//ゲートは2番のピン
const int GATE_PIN = 2;
const float cvstep = 0.00322265625;
int dacWrite = 0;

void setup() {
  Serial.begin(115200);
  //分解能を10bit=1024段階に設定する。
  analogWriteResolution(10);
  pinMode(GATE_PIN, OUTPUT);
  //初期値として、CVを0Vに設定すること。
  analogWrite(CV_PIN, 0);
}

void loop() {
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  if (rx.header != 0) {
    //note onの合図を見つけたとき
    if (rx.byte1 == 144) {
      //音階がC1以上の時
      if (rx.byte2 > 35) {
        float outputCV = (rx.byte2 - 32) * 0.04167;
        dacWrite = int(outputCV / cvstep);
        Serial.println(dacWrite);
        analogWrite(CV_PIN, dacWrite);
        digitalWrite(GATE_PIN, HIGH);
      }
    }
    //note offの合図を見つけたとき
    if (rx.byte1 == 128) {
      digitalWrite(GATE_PIN, LOW);
    }
  }
  //float voltage = analogRead(A3) * 3.3 / 4096.0;
  //SerialUSB.println(voltage);
}
