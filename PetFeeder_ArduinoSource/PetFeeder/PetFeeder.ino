#include <DS1302.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
<<<<<<< HEAD
#include <Stepper.h>
=======

>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
#include <HX711.h>

#define calibration_factor -7050.0
#define DOUT 3
#define CL 2
HX711 scale(DOUT, CL);
SoftwareSerial BTSerial(10, 11);
/*블루투스 10,11 
<<<<<<< HEAD
  mp3 rx1/tx1 18 19   
  무게 2, 3
  모터 4, 5, 6, 7
=======
  mp3 rx1/tx1
  무게 2, 3
  모터 4, 5, 6
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
  RTC: SCL 23 I/O 25 RST 27
*/
bool commandActive = false;
bool secommandActive = false;
bool remainCheckActive = false;
<<<<<<< HEAD
float tempWeight = 0;
const int stepsPerRevolution = 64;
Stepper myStepper(stepsPerRevolution, 7, 5, 6, 4);
=======

int motorA = 6;
int motorB = 5;
int ENA = 4;
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
const int CLK = 23;
const int DAT = 25;
const int RST = 27;
DS1302 myrtc(RST, DAT, CLK);
Time t;

String temp = "";

int inputCount = 0;
int ftCount = 0;
int feedAmount = 100; //default 값
int feedCNT = 0;

int MorningFeedH = 8; // 사료 급여시간 설정
int MorningFeedM = 0;
int LunchFeedH = 12;
int LunchFeedM = 00;
int DinnerFeedH = 18;
int DinnerFeedM = 0;

unsigned long WhenFeedsec = 0;
unsigned long currentTime = 0;
float remainFeed = 0;

byte data = "";
float feedData[10] = {};
String byteToString ="";
DFRobotDFPlayerMini myDFPlayer;

void setup(){
  Serial1.begin(9600);
  BTSerial.begin(9600);
  Serial.begin(9600);
  Serial2.begin(9600);
  myDFPlayer.begin(Serial1);
  delay(1);
  myDFPlayer.volume(30);
<<<<<<< HEAD
  
  scale.begin(DOUT, CL);
  scale.tare();
  scale.set_scale();
  myrtc.halt(false);                 
  myrtc.writeProtect(false);
  myStepper.setSpeed(500);
  
=======
  scale.set_scale(calibration_factor);
  scale.tare();

  myrtc.halt(false);                 
  myrtc.writeProtect(false);

  pinMode(motorB, OUTPUT);
  pinMode(motorA, OUTPUT);
  analogWrite(ENA, 255);
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
  }

  
void loop(){
<<<<<<< HEAD
  scale.set_scale(calibration_factor);
=======
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
  t = myrtc.getTime();
  currentTime = millis();
  
  /*블루투스로 사료 잔여량 확인*/
  while(BTSerial.available()){
    data = BTSerial.read();
    if(data == 'A'){
      byteToString = "";
    }
    if(data == 'T'){
      byteToString = "";
    }
    byteToString += char(data);
    Serial.println(byteToString);

    // seekBar로 급여량 설정하는 부분
    if(byteToString == "Amount"){
      Serial.println(byteToString);
      commandActive = true;
      byteToString = "";
      continue;
    } 
    // EditText로 급여 시간 설정하는 부분
    if(byteToString == "Time"){
      Serial.println(byteToString);
      secommandActive = true;
      byteToString = "";
      continue;
    }
    
    if(byteToString == "playtwo"){
      byteToString = "";
      feeding();
    }
    else if(byteToString == "feedDataRequest"){
      byteToString = "";
      /*정렬 후 전송*/
      if(feedCNT > 8){
        for(int i = 0; i < 9; i++){
          BTSerial.print(feedData[i]);
          delay(100);
        }
      }

      else if(feedCNT <= 8){
        for(int i = 0 ; i < (feedCNT % 9 + 1); i++){
          BTSerial.print(feedData[i]);
          delay(100);
        }
      }
    }
  }
  delay(100);/*오동작 방지 딜레이*/
  
  if(commandActive){
    feedAmount = byteToString.toInt();
    commandActive = false;
    byteToString = "";
  }
  if(secommandActive){
    for(int i=0; i<12; i++){
      if(i<2){
        temp += byteToString[i];
        if(i==1){
          MorningFeedH = temp.toInt();
          temp = "";
        }
      }
      if(i>=2 && i<4){
        temp += byteToString[i];
        if(i==3){
          MorningFeedM = temp.toInt();
          
          temp = "";
        }
      }
      if(i>=4 && i<6){
        temp += byteToString[i];
        if(i==5){
          LunchFeedH = temp.toInt();
          temp = "";
        }
      }
      if(i>=6 && i<8){
        temp += byteToString[i];
        if(i==7){
          LunchFeedM = temp.toInt();
          temp = "";
        }
      }
      if(i>=8 && i<10){
        temp += byteToString[i];
        if(i==9){
          DinnerFeedH = temp.toInt();
          temp = "";
        }
      }
      if(i>=10 && i<12){
        temp += byteToString[i];
        if(i==11){
          DinnerFeedM = temp.toInt();
          temp = "";
        }
      }
    }
    byteToString = "";
    secommandActive = false;
  }
  
  if((t.hour == MorningFeedH && t.min == MorningFeedM) ||(t.hour == LunchFeedH && t.min == LunchFeedM) ||(t.hour == DinnerFeedH && t.min == DinnerFeedM)){
    if(t.sec == 0){
      feeding();
    }
  }
  
  if(remainCheckActive){
    if(currentTime - WhenFeedsec >= 15000){
      remainFeed = getWeight();
      if(feedCNT > 8){
<<<<<<< HEAD
          feedData[9] = feedAmount - remainFeed;
=======
          feedData[9] = remainFeed;
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
          feedData[0] = feedData[1];
          for(int i = 1; i < 9; i++){
              feedData[i] = feedData[i+1];
              delay(100);
            }
        }
        else if(feedCNT <= 8){
<<<<<<< HEAD
          feedData[(feedCNT % 10)] = feedAmount - remainFeed;
=======
          feedData[(feedCNT % 10)] = remainFeed;
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
        }
        feedCNT += 1;
        remainCheckActive = false;
    }
  }
<<<<<<< HEAD
  feeding();
=======
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
}



void mp3(){
<<<<<<< HEAD
  //int song = random(1, 3);
  myDFPlayer.play(1);
}
float getWeight(){
  return (scale.get_units() * 0.453592 * 0.35 * 100) * -1;
=======
  int song = random(1, 3);
  myDFPlayer.play(song);
}
float getWeight(){
  return (scale.get_units() * 45.3592);
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
}

void feeding(){
  mp3();
<<<<<<< HEAD
  while(tempWeight < feedAmount){
    for(int i=0; i<32; i++) {  // 64 * 32 = 2048 한바퀴
      myStepper.step(stepsPerRevolution);
      tempWeight = getWeight();
      if(tempWeight > feedAmount) break;
      Serial.println(tempWeight);
    }
  }
  myDFPlayer.stop();
=======
  while(getWeight() < feedAmount - remainFeed){
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
  }
  digitalWrite(motorA, LOW);
>>>>>>> 71b1ed32c10a5333fe0396fff6ffc294160dcb06
  remainCheckActive = true;
  WhenFeedsec = millis();
}
