/*
Erdfeuchtigkeitsmesser, Luftfeuchtigkeitsmesser & Thermometer
By Fabian Exel
latest Version
 */
 
 #include <LiquidCrystal.h>
 #define RS 12  //Register Select
 #define E 11   //Enable
 #define D4 5   //Datenleitung 4
 #define D5 4   //Datenleitung 5
 #define D6 3   //Datenleitung 6
 #define D7 2   //Datenleitung 7
 #define COLS 16 //Anzahl der Spalten
 #define ROWS 2  //Anzahl der Zeilen
 LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

  #define piezoPin 10                                                             // Piezo-Element an Pin 13
  #define toneDuration 500                                                        // Ton-Dauer
  #define tonePause 550                                                           // Pausenlänge zwischen den Tönen
  #define pause 500
  int tones[] = {523, 659, 587, 698, 659, 784, 698, 880, tonePause, tonePause, 555, 666, 777, 888, tonePause, tonePause, 543, 654, 765, 876,
                 523, 659, 587, 698, 659, 784, 698, 880, tonePause, tonePause, 555, 666, 777, 888, tonePause, tonePause, 543, 654, 765, 876,
                };
  int elements = sizeof(tones) / sizeof(tones[0]);
  int mostureSensor = 1;
  float temp = 0;
  int accuracy = 1;
  int wait = 5000;
  const int light = 9;
  const int inputPin = 13;
  int v0 = 0;
  int v1 = 0;
  int v2 = 0;
  int v3 = 0;
  int v4 = 0;
  int v5 = 0;
  int vv0 = 0;
  int vv1 = 0;
  int vv2 = 0;
  int vv3 = 0;
  int vv4 = 0;
  int vv5 = 0;
  int text = 0;

double dewPoint(double celsius, double humidity)
{
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

        // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

        // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}

#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 8

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(light, OUTPUT);
  digitalWrite(light, LOW);
  pinMode(inputPin, INPUT);
  Serial.println("reading temperature begin.");
    pinMode(10, OUTPUT);
    noTone(piezoPin);            // Piezo stumm schalten
  lcd.begin(COLS, ROWS);    //Anzahl der Spalten und Zeilen
}

void loop() {
int value= digitalRead(inputPin);
if (value == HIGH){
  digitalWrite(light, HIGH);
  lcd.clear();
  for(int x = 0; x < 2; x++){
  // put your main code here, to run repeatedly:
  //Part of 2. temperature
  static unsigned long sensortStamp = 0;
if(millis() - sensortStamp > 100){
sensortStamp = millis();
int reading = analogRead(0);
temp = reading *0.0048828125*100;
}

  int chk = DHT11.read(DHT11PIN);
  
if(temp>(float)DHT11.temperature+accuracy){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print((float)DHT11.temperature+0.5, 1);
    #if ARDUINO < 100
    lcd.print(0xD0 + 15, BYTE);
    #else 
    lcd.write(0xD0 +15);
    #endif
    lcd.print("C");
}

else if(temp<(float)DHT11.temperature-accuracy){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print((float)DHT11.temperature-0.5, 1);
    #if ARDUINO < 100
    lcd.print(0xD0 + 15, BYTE);
    #else 
    lcd.write(0xD0 +15);
    #endif
    lcd.print("C");
}
else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print((float)DHT11.temperature, 1);
    #if ARDUINO < 100
    lcd.print(0xD0 + 15, BYTE);
    #else 
    lcd.write(0xD0 +15);
    #endif
    lcd.print("C");
}
    lcd.setCursor(0, 1);
    lcd.print("Luft H2O: ");
    lcd.print((float)DHT11.humidity, 0);
    lcd.print("%");

   // read the input on analog pin 1:
  int sensorValue = analogRead(mostureSensor) * 5;
  // print out the value you read:
  Serial.print("Pflanzenwert: ");
  Serial.println(sensorValue);

if(x > 0){
  vv5 = vv4;
  vv4 = vv3;
  vv3 = vv2;
  vv2 = vv1;
  vv1 = vv0;
  vv0 = (float)DHT11.temperature;
 
  delay(wait);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("11 22 33 44 55 C");
  lcd.setCursor(0, 1);
  lcd.print(vv1);
  lcd.setCursor(3, 1);
  lcd.print(vv2);
  lcd.setCursor(6, 1);
  lcd.print(vv3);
  lcd.setCursor(9, 1);
  lcd.print(vv4);
  lcd.setCursor(12, 1);
  lcd.print(vv5);
  lcd.setCursor(15 ,1);
  lcd.print("C");
}

if(x > 0){
  v5 = v4;
  v4 = v3;
  v3 = v2;
  v2 = v1;
  v1 = v0;
  v0 = (float)DHT11.humidity;

  delay(wait);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("11 22 33 44 55 %");
  lcd.setCursor(0, 1);
  lcd.print(v1);
  lcd.setCursor(3, 1);
  lcd.print(v2);
  lcd.setCursor(6, 1);
  lcd.print(v3);
  lcd.setCursor(9, 1);
  lcd.print(v4);
  lcd.setCursor(12, 1);
  lcd.print(v5);
  lcd.setCursor(15 ,1);
  lcd.print("%");
}
    
  delay(wait);
  lcd.clear();
  lcd.setCursor(0,0);
if (vv0 > vv1){
  lcd.print("Es wird waermer");
}
else if (vv0 < vv1){
  lcd.print("Es wird kuehler");
}
else if (vv0 == vv1){
  lcd.print("Temp ist gleich");
}

  lcd.setCursor(0,1);
if (v0 > v1){
  lcd.print("Es wird feuchter");
}
else if (v0 < v1){
  lcd.print("Es wird trocken");
}
else if (v0 == v1){
  lcd.print("Bleibt so feucht");
}

  delay(wait);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tau*: ");
  lcd.print(dewPoint(DHT11.temperature, DHT11.humidity));
    #if ARDUINO < 100
    lcd.print(0xD0 + 15, BYTE);
    #else 
    lcd.write(0xD0 +15);
    #endif
    lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Tau*fast: ");
  lcd.print(dewPointFast(DHT11.temperature, DHT11.humidity));
    #if ARDUINO < 100
    lcd.print(0xD0 + 15, BYTE);
    #else 
    lcd.write(0xD0 +15);
    #endif
    lcd.print("C");
  delay(wait);

    lcd.clear();
    lcd.setCursor(0,0);

 if (sensorValue >= 820){
  lcd.print("Pflanze gieBen!");
  while(sensorValue >= 820){
  noTone(piezoPin);
   for (int i = 0; i < elements; i++) {
    tone(piezoPin, tones[i], toneDuration);                                     // Ton spielen
    delay(tonePause);                                                           // Pause zwischen den Tönen
  }
}
}    
else if (sensorValue >= 0 && sensorValue < 410)
 {
  lcd.print("Pflanze hat H2O");
  }
  
  lcd.setCursor(0,1);

if((float)DHT11.temperature > 19 && temp > 19 && (float)DHT11.humidity < 60){
  lcd.print("Guter Tag :)");  
}
else if((float)DHT11.temperature < 18 || temp < 18 || (float)DHT11.humidity > 70  || ((float)DHT11.temperature > 25 && temp > 25)){
  lcd.print("Schlechter Tag!");   
}
else if(((float)DHT11.temperature > 18 && temp > 18) || (float)DHT11.humidity < 70 || ((float)DHT11.temperature < 22 && temp < 22)){
  lcd.print("Mittlerer Tag :/");   
}

delay(wait);

if(x > 0){
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Zahl der Zeit:");
lcd.setCursor(0,1);
lcd.print(sensorValue * random(100000));  
delay(wait);

lcd.clear();
lcd.setCursor(0,0);
text = random(40);
switch(text){
  case 0:
  lcd.print("BIG BROTHER");
  lcd.setCursor(0,1);
  lcd.print("is watching you");
  break;
  case 1:
  lcd.print(":) You are my");
  lcd.setCursor(0,1);
  lcd.print("best Friend :)");
  break;
  case 2:
  lcd.print("Live free");
  lcd.setCursor(0,1);
  lcd.print("or die!");
  break;
  case 3:
  lcd.print("My name is Bond");
  lcd.setCursor(0,1);
  lcd.print("James Bond");
  break; 
  case 4:
  lcd.print("So many books,");
  lcd.setCursor(0,1);
  lcd.print("so little time.");
  break;
  case 39:
  lcd.print("To be is to do");
  lcd.setCursor(0,1);
  lcd.print("To do is to be");
  break;
  case 6:
  lcd.print("Welcome in the");
  lcd.setCursor(0,1);
  lcd.print("Cyberspace");
  break;
  case 7:
  lcd.print("Do you like");
  lcd.setCursor(0,1);
  lcd.print("(love) me?");
  break;
  case 8:
  lcd.print("sudo apt update");
  lcd.setCursor(0,1);
  lcd.print("... like me :)");
  break;
  case 9:
  lcd.print("Love is all");
  lcd.setCursor(0,1);
  lcd.print("you need.");
  break;
  case 10:
  lcd.print("I am a 8 bit");
  lcd.setCursor(0,1);
  lcd.print("Supercomputer");
  break;
  case 38:
  lcd.print("I want to know");
  lcd.setCursor(0,1);
  lcd.print("gods thoughts");
  break;
  case 12:
  lcd.print("Using Siri?");
  lcd.setCursor(0,1);
  lcd.print("You hate me!");
  break;
  case 13:
  lcd.print("More facts and");
  lcd.setCursor(0,1);
  lcd.print("numbers, please");
  break; 
  case 14:
  lcd.print("Do you know");
  lcd.setCursor(0,1);
  lcd.print("Nerd Korea?");
  break;
  case 15:
  lcd.print("Don't you step");
  lcd.setCursor(0,1);
  lcd.print("on my blue shoes");
  break;
  case 16:
  lcd.print("Here could be");
  lcd.setCursor(0,1);
  lcd.print("your text.");
  break;
  case 37:
  lcd.print("Sorry");
  lcd.setCursor(0,1);
  lcd.print("Just playing");
  break;
  case 18:
  lcd.print("Computers are");
  lcd.setCursor(0,1);
  lcd.print("the best friends");
  break;
  case 19:
  lcd.print("Last but not");
  lcd.setCursor(0,1);
  lcd.print("least");
  break;
  case 20:
  lcd.print("Work, eat, buy,");
  lcd.setCursor(0,1);
  lcd.print("consume then die");
  break;
  case 21:
  lcd.print("Here, some");
  lcd.setCursor(0,1);
  lcd.print("Space Invaders");
  break;
  case 22:
  lcd.print("bam bam bam bam");
  lcd.setCursor(0,1);
  lcd.print("(playing drums)");
  break;
  case 23:
  lcd.print("Imposible is");
  lcd.setCursor(0,1);
  lcd.print("just an opinion");
  break;
  case 24:
  lcd.print("if not now...");
  lcd.setCursor(0,1);
  lcd.print("then when?");
  break;
  case 25:
  lcd.print("My life is my");
  lcd.setCursor(0,1);
  lcd.print("message");
  break;
  case 26:
  lcd.print("Write more");
  lcd.setCursor(0,1);
  lcd.print("SMS and Mails!");
  break;
  case 27:
  lcd.print("Life is short");
  lcd.setCursor(0,1);
  lcd.print("so don't die!");
  break;
  case 28:
  lcd.print("Don't worry");
  lcd.setCursor(0,1);
  lcd.print("Be happy");
  break;
  case 29:
  lcd.print("In a very short");
  lcd.setCursor(0,1);
  lcd.print("period of time");
  break;
  case 30:
  lcd.print("This works");
  lcd.setCursor(0,1);
  lcd.print("for me and you");
  break;
  case 31:
  lcd.print("Hack to the");
  lcd.setCursor(0,1);
  lcd.print("future -->");
  break;
  case 32:
  lcd.print("Trouble is my");
  lcd.setCursor(0,1);
  lcd.print("middle name");
  break;
  case 33:
  lcd.print("Can you meet me");
  lcd.setCursor(0,1);
  lcd.print("in the middle?");
  break;
  case 34:
  lcd.print("Normal is boring");
  lcd.setCursor(0,1);
  lcd.print("Different is hot");
  break;
  case 36:
  lcd.print("You could be");
  lcd.setCursor(0,1);
  lcd.print("anything!");
  break;
  case 35:
  lcd.print("travaling time:");
  lcd.setCursor(0,1);
  lcd.print("in 1s 1s later");
  break;
  case 17:
  lcd.print("Out of my mind");
  lcd.setCursor(0,1);
  lcd.print("Back in 5 min.");
  break;
  case 11:
  lcd.print("this programmed");
  lcd.setCursor(0,1);
  lcd.print("with crazy aaaaa");
  break;
  case 5:
  lcd.print("It depends on");
  lcd.setCursor(0,1);
  lcd.print("who I'm with");
  break;
}

delay(wait);
}}
lcd.clear();
}
else{
  digitalWrite(light, LOW);
}
}

