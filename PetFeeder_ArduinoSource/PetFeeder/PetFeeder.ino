#include <DS1302.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Stepper.h>
#include <HX711.h>

#define calibration_factor -7050.0
#define DOUT 3
#define CL 2

HX711 scale(DOUT, CL);
DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial BTSerial(10, 11);
/*블루투스 10,11
  mp3 rx1/tx1 18 19
  무게 2, 3
  모터 4, 5, 6, 7
  RTC: SCL 23 I/O 25 RST 27
*/
bool amountActive = false;
bool timeSetActive = false;
bool remainCheckActive = false;
float tempWeight = 0;
const int stepsPerRevolution = 64;
Stepper myStepper(stepsPerRevolution, 7, 5, 6, 4);
const int CLK = 23;
const int DAT = 25;
const int RST = 27;

DS1302 myrtc(RST, DAT, CLK);
Time t;

String temp = "";
int IR_DETECT = 29;
int feedAmount = 10; //default Amount
int feedCNT = 0;

int MorningFeedH = 8; // default Feeding time
int MorningFeedM = 0;
int LunchFeedH = 12;
int LunchFeedM = 00;
int DinnerFeedH = 18;
int DinnerFeedM = 0;

unsigned long WhenFeedsec = 0;
float remainFeed = 0;
byte data = "";
float feedData[10] = {};
String byteToString = "";
int ledPin = 44;

void setup() {
  Serial1.begin(9600);
  BTSerial.begin(9600);
  Serial.begin(9600);
  Serial2.begin(9600);
  myDFPlayer.begin(Serial1);
  delay(1);
  myDFPlayer.volume(20);

  scale.begin(DOUT, CL);
  scale.set_scale(calibration_factor);
  scale.tare();
  myrtc.halt(false);
  myrtc.writeProtect(false);
  myStepper.setSpeed(500);
  pinMode(IR_DETECT, INPUT);
  pinMode(ledPin, OUTPUT);
}


void loop() {
  scale.set_scale(calibration_factor);
  t = myrtc.getTime();
  int IR = digitalRead(IR_DETECT);
  
  while (BTSerial.available()) {
    data = BTSerial.read();
    //Amount Slicing
    if (data == 'A') {
      byteToString = "";
    }
    //Time Slicing
    if (data == 'T') {
      byteToString = "";
    }
    
    byteToString += char(data);
    Serial.println(byteToString);

    // Ready for set feedAmount with seekBar
    if (byteToString == "Amount") {
      Serial.println(byteToString);
      amountActive = true;
      byteToString = "";
      continue;
    }
    // Ready for set feeding time with EditText
    if (byteToString == "Time") {
      Serial.println(byteToString);
      timeSetActive = true;
      byteToString = "";
      continue;
    }

    //manual feeding
    if (byteToString == "playtwo") {
      byteToString = "";
      feeding();
    }

    // feedData sort, then send to android
    else if (byteToString == "feedDataRequest") {
      byteToString = "";
      if (feedCNT > 8) {
        for (int i = 0; i < 9; i++) {
          BTSerial.print(feedData[i]);
          delay(100);
        }
      }

      else if (feedCNT <= 8) {

        for (int i = 0 ; i < feedCNT; i++) {
          if (feedCNT < 1) {
            BTSerial.print("저장된 데이터가 없습니다");
          }
          else {
            BTSerial.print(feedData[i]);
            delay(100);
          }
        }
      }
    }
  }
  delay(100);/*오동작 방지 딜레이*/

  if (amountActive) {
    feedAmount = byteToString.toInt();
    amountActive = false;
    byteToString = "";
  }

  if (timeSetActive) {
    for (int i = 0; i < 12; i++) {
      if (i < 2) {
        temp += byteToString[i];
        if (i == 1) {
          MorningFeedH = temp.toInt();
          temp = "";
        }
      }
      if (i >= 2 && i < 4) {
        temp += byteToString[i];
        if (i == 3) {
          MorningFeedM = temp.toInt();

          temp = "";
        }
      }
      if (i >= 4 && i < 6) {
        temp += byteToString[i];
        if (i == 5) {
          LunchFeedH = temp.toInt();
          temp = "";
        }
      }
      if (i >= 6 && i < 8) {
        temp += byteToString[i];
        if (i == 7) {
          LunchFeedM = temp.toInt();
          temp = "";
        }
      }
      if (i >= 8 && i < 10) {
        temp += byteToString[i];
        if (i == 9) {
          DinnerFeedH = temp.toInt();
          temp = "";
        }
      }
      if (i >= 10 && i < 12) {
        temp += byteToString[i];
        if (i == 11) {
          DinnerFeedM = temp.toInt();
          temp = "";
        }
      }
    }
    byteToString = "";
    timeSetActive = false;
  }
  
  /*Serial.println(t.hour);
  Serial.println(t.min);
  Serial.println(t.sec);*/
  if ((t.hour == MorningFeedH && t.min == MorningFeedM) || (t.hour == LunchFeedH && t.min == LunchFeedM) || (t.hour == DinnerFeedH && t.min == DinnerFeedM)) {
    if (t.sec == 0) {
      feeding();
    }
  }

  if (remainCheckActive) {
    if (millis() - WhenFeedsec >= 10000) {
      remainFeed = getWeight();
      Serial.println(remainFeed);

      if (feedCNT > 8) {
        feedData[9] = remainFeed;
        feedData[0] = feedData[1];
        for (int i = 1; i < 9; i++) {
          feedData[i] = feedData[i + 1];
          delay(100);
        }
      }
      else if (feedCNT <= 8) {
        feedData[feedCNT] = remainFeed;
      }
      feedCNT += 1;
      remainCheckActive = false;
    }
  }
  if (IR == 0){
    digitalWrite(ledPin, 1);
  }
  if (IR == 1){
    digitalWrite(ledPin, 0);
  }
  delay(100);

}


void mp3() {
  //int song = random(1, 3);
  myDFPlayer.play(1);
}
float getWeight() {
  // zero point of my loadcell censor
  // 5kg loadcell, in grams
  return (scale.get_units() * 0.453592 * 0.35 * 100) * -1;
}

void feeding() {
  mp3();
  tempWeight = 0;
  while (tempWeight < feedAmount) {
    for (int i = 0; i < 32; i++) { // 64 * 32 = 2048 한바퀴
      myStepper.step(stepsPerRevolution);
      tempWeight = getWeight();
      Serial.println(tempWeight);
      if (tempWeight > feedAmount) break;
    }
  }
  delay(500);
  myDFPlayer.stop();
  remainCheckActive = true;
  WhenFeedsec = millis();
}
