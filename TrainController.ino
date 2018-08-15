#include <Bounce2.h>

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )
//#define COMMON_ANODE

int off[3] = {0, 0, 0};
int red[3] = {255, 0 , 0};
int green[3] = {0, 255, 0};
int blue[3] = {0, 0, 255};
int yellow[3] = {255, 255, 0};
int purple[3] = {255, 0, 255};
int aqua[3] = {0, 255, 255};
int* colors[6] = {red, green, blue, yellow, purple, aqua};
int colLen = arr_len(colors);

//Override Status
const byte pilotRed0 = 22;
const byte pilotGreen0 = 23;
const byte pilotBlue0 = 24;
const byte pilotRed1 = 25;
const byte pilotGreen1 = 26;
const byte pilotBlue1 = 27;
const byte pilotRed2 = 28;
const byte pilotGreen2 = 29;
const byte pilotBlue2 = 30;
const byte pilotRed3 = 31;
const byte pilotGreen3 = 32;
const byte pilotBlue3 = 33;

const byte safetyRed0 = 34;
const byte safetyGreen0 = 35;
const byte safetyBlue0 = 36;
const byte safetyRed1 = 37;
const byte safetyGreen1 = 38;
const byte safetyBlue1 = 39;
const byte safetyRed2 = 40;
const byte safetyGreen2 = 41;
const byte safetyBlue2 = 42;
const byte safetyRed3 = 43;
const byte safetyGreen3 = 44;
const byte safetyBlue3 = 45;

const byte greenLight = A7;
const byte brake = A6;
const byte brakeRelay = A5;

int pilotLED0[3] = {pilotRed0, pilotGreen0, pilotBlue0};
int pilotLED1[3] = {pilotRed1, pilotGreen1, pilotBlue1};
int pilotLED2[3] = {pilotRed2, pilotGreen2, pilotBlue2};
int pilotLED3[3]= {pilotRed3, pilotGreen3, pilotBlue3};

int safetyLED0[3] = {safetyRed0, safetyGreen0, safetyBlue0};
int safetyLED1[3] = {safetyRed1, safetyGreen1, safetyBlue1};
int safetyLED2[3] = {safetyRed2, safetyGreen2, safetyBlue2};
int safetyLED3[3] = {safetyRed3, safetyGreen3, safetyBlue3};

bool pilot[4] = {0, 0, 0, 0};
bool safety[4] = {0, 0, 0, 0};
bool pilotSol = false;
bool safeSol = false;
bool overrideStatus[2] = {pilotSol, safeSol};

bool flag0 = false;
bool flag1 = false;
bool flag2 = false;
bool flag3 = false;

//CSWM Controls
const byte HSpin = A1;
const byte SSpin = A0;
const byte voltmeterCSWM = 2;

//XCV-K
const byte DVI = 51;
const byte HBD = 52;
const byte MKV = 53;

//KME-CKW
const byte tangle = 5;
const byte elevate = 6;

//Light/Flux
const byte voltmeterLeft = 3;
const byte voltmeterRight = 4;

//Utilities
const byte greenButtons = 46;
const byte redButtons = 47;
const byte whiteButtons = 48;
const byte mismth = 49;
const byte linr = 50;
bool linrState = false;
bool mismthState = false;

//Railing
const byte engageButton = 7;
const byte abortButton = 8;

//Flux Mod
const byte fluxSwitch1 = 9;
const byte fluxSwitch2 = 10;
const byte fluxSwitch5 = 11;
const byte fluxSwitch6 = 12;


//Field Flow
const byte flowButton = 13;
bool flowState = false;

Bounce greenButts = Bounce();
Bounce whiteButts = Bounce();
Bounce redButts = Bounce();
Bounce linrButton = Bounce();
Bounce mismthButton = Bounce();
Bounce engageButt = Bounce();
Bounce abortButt = Bounce();
Bounce flowButt = Bounce();
Bounce brakeButt = Bounce();

unsigned long debounceDelay = 50;

unsigned long lastVolt;
unsigned long voltmeterDelay = 300;
void setup() {
  Serial.begin(9600);
  Serial.print("Train Controller");
  pinMode(pilotRed0, OUTPUT);
  pinMode(pilotGreen0, OUTPUT);
  pinMode(pilotBlue0, OUTPUT);
  pinMode(safetyRed0, OUTPUT);
  pinMode(safetyGreen0, OUTPUT);
  pinMode(safetyBlue0, OUTPUT);
  pinMode(HSpin, INPUT);
  pinMode(SSpin, INPUT);
  pinMode(voltmeterCSWM, OUTPUT);
  pinMode(DVI, INPUT);
  pinMode(HBD, INPUT);
  pinMode(MKV, INPUT);
  pinMode(tangle, INPUT);
  pinMode(elevate, INPUT);
  pinMode(voltmeterLeft, OUTPUT);
  pinMode(voltmeterRight, OUTPUT);
  pinMode(greenButtons, INPUT_PULLUP);
  pinMode(whiteButtons, INPUT_PULLUP);
  pinMode(redButtons, INPUT_PULLUP);
  pinMode(linr, INPUT_PULLUP);
  pinMode(mismth, INPUT_PULLUP);
  pinMode(engageButton, INPUT_PULLUP);
  pinMode(abortButton, INPUT_PULLUP);
  pinMode(fluxSwitch1, INPUT);
  pinMode(fluxSwitch2, INPUT);
  pinMode(fluxSwitch5, INPUT);
  pinMode(fluxSwitch6, INPUT);
  pinMode(flowButton, INPUT_PULLUP);
  pinMode(brake, INPUT_PULLUP);
  pinMode(greenLight, OUTPUT);
  pinMode(brakeRelay, OUTPUT);
  
  whiteButts.attach(whiteButtons);
  greenButts.attach(greenButtons);
  redButts.attach(redButtons);
  linrButton.attach(linr);
  mismthButton.attach(mismth);
  engageButt.attach(engageButton);
  abortButt.attach(abortButton);
  flowButt.attach(flowButton);
  brakeButt.attach(brake);
  
  whiteButts.interval(50);
  greenButts.interval(50);
  redButts.interval(50);
  linrButton.interval(50);
  mismthButton.interval(50);
  engageButt.interval(50);
  abortButt.interval(50);
  flowButt.interval(50);
  brakeButt.interval(50);

  lastVolt = millis();
}

void loop() {
  whiteButts.update();
  greenButts.update();
  redButts.update();
  linrButton.update();
  mismthButton.update();
  engageButt.update();
  abortButt.update();
  flowButt.update();
  brakeButt.update();
  
  CSWMcontrols();
  xcvk();
  kmeckw();
  lightFlux();
  utilities();
  railing();
  fluxMod();
  fieldFlow();
  pilotSol = true;
  safeSol = true;
  for(int i = 0; i < 4; i++){
    if(!pilot[i]){
      pilotSol = false;
    }
    if(!safety[i]){
      safeSol = false;
    }
  }
  if(pilotSol && safeSol){
    digitalWrite(greenLight, HIGH);
    if(!digitalRead(brake)){
      Serial.println("Brake!");
      digitalWrite(brakeRelay, LOW);
      delay(50);
    }
    else{
      digitalWrite(brakeRelay, HIGH);
    }
  }
  else{
    digitalWrite(brakeRelay, HIGH);
    digitalWrite(greenLight, LOW);
  }
}

void randomVolt(byte pin){
  int value = random(0, 256);
  if(millis() - lastVolt > voltmeterDelay){
    analogWrite(pin, value);
    lastVolt = millis();
  }
}

void setColor(int* led, int* color){
  #ifdef COMMON_ANODE
  red = 255 - color[0];
  green = 255 - color[1];
  blue = 255 - color[2];
  #endif
  analogWrite(led[0], color[0]);
  analogWrite(led[1], color[1]);
  analogWrite(led[2], color[2]);
}

void playSound(){
  
}

void stopSound(){
  
}

void CSWMcontrols(){
  int SSvalue = analogRead(SSpin);
  int HSvalue = analogRead(HSpin);

  //Serial.print("SS Value: ");
  //Serial.println(SSvalue);
  if(SSvalue >= 480 && SSvalue <= 520){
    if(pilot[2]){
      setColor(pilotLED3, green);
      pilot[3] = true;
    }
    else{
      setColor(pilotLED3, red);
      pilot[3] = false;
    }
  }
  else{
    setColor(pilotLED3, off);
    pilot[3] = false;
  }

  if((HSvalue >= 0 && HSvalue <= 51) || (HSvalue <= 255 && HSvalue >= 200)){
    playSound();
  }
  else{
    stopSound();
  }

  randomVolt(voltmeterCSWM);
}

void xcvk(){
  /*Serial.print("DVI: ");
  Serial.println(digitalRead(DVI));
  Serial.print("HBD: ");
  Serial.println(digitalRead(HBD));
  Serial.print("MKV: ");
  Serial.println(digitalRead(MKV));*/
  if(digitalRead(DVI) && digitalRead(HBD) && digitalRead(MKV)){
    setColor(pilotLED0, green);
    pilot[0] = true;
  }
  else{
    setColor(pilotLED0, off);
    pilot[0] = false;
  }
}

void kmeckw(){
  if(digitalRead(tangle) && digitalRead(elevate) && pilot[3]){
    setColor(safetyLED0, green);
    safety[0] = true;
  }
  else if(digitalRead(tangle) && digitalRead(elevate) && !pilot[3]){
    setColor(safetyLED0, red);
  }
  else{
    setColor(safetyLED0, off);
    safety[0] = false;
  }
}

void lightFlux(){
  randomVolt(voltmeterLeft);
  randomVolt(voltmeterRight);
}

void utilities(){
  if(!digitalRead(greenButtons)){
    playSound();
  }
  else{
    stopSound();
  }
  if(!digitalRead(redButtons)){
    playSound();
  }
  else{
    stopSound();
  }
  if(!digitalRead(whiteButtons)){
    playSound();
  }

  if(linrButton.fell()){
    if(pilot[0] && !linrState){
      Serial.println("Linr: Green");
      setColor(pilotLED1, green);
      pilot[1] = true;
      linrState = true;
    }
    else if(!pilot[0] && !linrState){
      Serial.println("Linr: Yellow");
      setColor(pilotLED1, red);
      pilot[1] = false;
      linrState = true;
    }
    else if(linrState){
      Serial.println("Linr: Off");
      setColor(pilotLED1, off);
      pilot[1] = false;
      linrState = false;
    }
  }
  
  if(mismthButton.fell()){
    if(safety[1] && !mismthState){
      Serial.println("Mismth: Green");
      setColor(safetyLED2, green);
      safety[2] = true;
      mismthState = true;
    }
    else if(!safety[1] && !mismthState){
      Serial.println("Mismth: Yellow");
      setColor(safetyLED2, red);
      safety[2] = false;
      mismthState = true;
    }
    else if(mismthState){
      Serial.println("Mismth: Off");
      setColor(safetyLED2, off);
      safety[2] = false;
      mismthState = false;
    }
  }
}

void railing(){
  if(engageButt.fell()){
    if(pilot[1]){
      Serial.println("Pilot2: Green");
      setColor(pilotLED2, green);
      pilot[2] = true;
    }
    else{
      Serial.println("Pilot: Yellow");
      setColor(pilotLED2, red);
      pilot[2] = false;
    }
  }
  if(abortButt.fell()){
    Serial.println("Pilot2: Off");
    setColor(pilotLED2, off);
    pilot[2] = false;
  }
}

void fluxMod(){
  if(digitalRead(fluxSwitch1)){
    if(safety[2]){
      setColor(safetyLED3, green);
      safety[3] = true;
    }
    else{
      setColor(safetyLED3, red);
      safety[3] = false;
    }
  }
  else{
    setColor(safetyLED3, off);
    safety[3] = false;
  }
  
  if(digitalRead(fluxSwitch2) || digitalRead(fluxSwitch5) || digitalRead(fluxSwitch6)){
    playSound();
  }
  else{
    stopSound();
  }
}

void fieldFlow(){
  if(flowButt.fell()){
    if(safety[0] && !flowState){
      Serial.println("Field Flow: Green");
      setColor(safetyLED1, red);
      safety[1] = true;
      flowState = true;
    }
    else if(!safety[0] && !flowState){
      Serial.println("Field Flow: Yellow");
      setColor(safetyLED1, blue);
      safety[1] = false;
      flowState = true;
    }
    else if(flowState){
      Serial.println("Field Flow: Off");
      setColor(safetyLED1, off);
      safety[1] = false;
      flowState = false;
    }
  }
}

