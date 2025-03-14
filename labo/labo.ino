#define BTN_PIN 2
#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);
unsigned long currentTime = 0;
int ThermistorPin = A0;
int Vo; // Voltage à la sortie
float R1 = 10000; // Résistance
float logR2, R2, T, Tc, Tf;
float c1 = 1.129148e-03, c2 = 2.34125e-04, c3 = 8.76741e-08;
int attente=3000;
int button = 2;
int pinX = A1;
int pinY = A2;
int valeurX = 0;
int valeurY = 0;
bool AC=false;
int ledON=HIGH;
int ledOFF=LOW;
int led = 8;
unsigned long previousMillis = 0;
const long interval = 5000;
  int limite1=25;
  int limite2=24;
  int compteur = 0;
int etatBouton = 0;
int dernierEtatBouton = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
lcd.begin();
ecran_depart();
pinMode(led, OUTPUT);
pinMode(button, INPUT_PULLUP);
pinMode(ThermistorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  Vo = analogRead(ThermistorPin);
boutton();
console();
}


void ecran_depart(){
  uint8_t symbole12[8]=
{
    0b01000,
    0b11000,
    0b01000,
    0b01111,
    0b11001,
    0b11101,
    0b00010,
    0b01111,
};

uint8_t dessin[8]=
{
    0b00000,
    0b01110,
    0b11111,
    0b10101,
    0b11111,
    0b01110,
    0b01110,
    0b00000,
};

lcd.print("Toure");
lcd.backlight();
lcd.createChar(0, symbole12);
lcd.createChar(1, dessin);
lcd.setCursor(0,1);
lcd.write(1);
lcd.setCursor(15,1);
lcd.write(0);
delay(attente);
}
void temp(){
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  unsigned long currentMillis = millis();

  if (Tc > limite1) {
    if (!AC) {

      if (currentMillis - previousMillis >= interval) {
        AC = true;
        digitalWrite(led, ledON);
      }
    } else {

      previousMillis = currentMillis;
    }
  }

  else {
    if (AC) {

      if (currentMillis - previousMillis >= interval) {
        AC = false;
        digitalWrite(led, ledOFF);
      }
    } else {

      previousMillis = currentMillis;
    }
  }
}

void ecranTemp(){
  lcd.setCursor(0, 0);
  lcd.print("Temperature : ");
  lcd.print(Tc);
  lcd.setCursor(0, 1);
  lcd.print("AC : ");
  int state= digitalRead(led);
  lcd.print(AC ? "ON " : "OFF");
}



void Sens() {
  valeurY = analogRead(pinX);
}

void vitesse() {
  valeurX = analogRead(pinY);

  if (valeurX < 511) {

    valeurX = map(valeurX, 0, 510, 120, 0);
  } else if (valeurX > 511) {

    valeurX = map(valeurX, 512, 1023, 0, -25);
  } else {

    valeurX = 0;
  }
  Serial.println(valeurX);
}
void displayVitesse() {
  lcd.setCursor(0, 0);

  if (valeurX >= 0) {
    lcd.print("Avance : ");
  } else {
    lcd.print("Recule : ");
  }

  lcd.print(valeurX);

  if (valeurX >= -9 && valeurX <= 99) {
    lcd.print(" ");
  }

  if (valeurX >= -9 && valeurX <= 9) {
    lcd.print(" ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Direction : ");

  if (valeurY > 506) {
    lcd.setCursor(13, 1);
    lcd.print("D");
  }
  if (valeurY < 506) {
    lcd.setCursor(13, 1);
    lcd.print("G");
  }
}
void boutton(){
etatBouton = digitalRead(button);
if (etatBouton != dernierEtatBouton) {
    if (etatBouton == HIGH) {
      compteur++;
      lcd.clear();
    }
    dernierEtatBouton = etatBouton;
  }
  if (compteur % 2 != 0) { 
   ecranTemp();
     temp();
  } else {

    displayVitesse();
    Sens();
    vitesse();
  }
}
void console(){
  int Tempsconsole=100;
  long numeroEtd=1970812;
  unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= Tempsconsole) {
Serial.print("etd:");
Serial.print(numeroEtd);
Serial.print(",valX:");
Serial.print(valeurX);
Serial.print(",valY:");
Serial.print(valeurY);
Serial.print(",valSys:");
Serial.print(etatBouton);
Serial.println();
}
}