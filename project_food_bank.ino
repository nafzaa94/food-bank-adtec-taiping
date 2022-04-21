// lcd
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// rfid
#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

//irsensor 1
int irsensor1 = 3;
int valueirsensor1 = 0;

//irsensor 2
int irsensor2 = 4;
int valueirsensor2 = 0;

//irsensor 3
int irsensor3 = 5;
int valueirsensor3 = 0;

// button 
int Button = 2;
int valueButton = 0;

// driver 1
int signaldriver1 = 8;
int signaldriver2 = 9;
// driver 2
int signaldriver3 = 10;
int signaldriver4 = 11;


int state = 0;
int state1 = 0;
int state2 = 0;
int staterfid = 0;

int statefunction = 0;

int Var = 0;


void setup() {
  Serial.begin(9600);
  lcd.begin(); // kalu dy error tukar begin jd init
  lcd.backlight();
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(Button, INPUT_PULLUP);
  pinMode(irsensor1, INPUT);
  pinMode(irsensor2, INPUT);
  pinMode(irsensor3, INPUT);
  pinMode(signaldriver1, OUTPUT);
  pinMode(signaldriver2, OUTPUT);
  pinMode(signaldriver3, OUTPUT);
  pinMode(signaldriver4, OUTPUT);

  lcd.setCursor(0, 4);
  lcd.print("WELCOME");

  digitalWrite(signaldriver1, LOW);
  digitalWrite(signaldriver2, LOW);

  digitalWrite(signaldriver3, HIGH);
  digitalWrite(signaldriver4, LOW);
}

void loop() {
  //running rfid
  rfid();
  
  valueButton = digitalRead(button);
  valueirsensor1 = digitalRead(irsensor1);
  valueirsensor2 = digitalRead(irsensor2);
  valueirsensor3 = digitalRead(irsensor3);

  if (valueirsensor1 == LOW && state1 == 0){
    state1 = 1
    }

  if (valueirsensor2 == LOW && state == 0){
    state = 1;
    }

  if (state == 1)
    {
    digitalWrite(signaldriver3, LOW);
    digitalWrite(signaldriver4, LOW);
    }

  if (state1 == 1){
      digitalWrite(signaldriver1, LOW);
      digitalWrite(signaldriver2, LOW);

      statefunction = 1;
      }

  if (valueButton == LOW && Var == 0 && staterfid == 1){
    lcd.setCursor(1, 0);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("SILA TGU...");
    Var = 1;
    state = 0;
    state1 = 0;
    state2 = 0;
    statefunction = 0;
    }

  switch (Var) {
  case 1:
    digitalWrite(signaldriver1, HIGH);
    digitalWrite(signaldriver2, LOW);

    if (statefunction == 1){
      Var = 2;
      }
    break;
  case 2:
    digitalWrite(signaldriver3, LOW);
    digitalWrite(signaldriver4, HIGH);
    Var = 3
    break;
  case 3:
    if (valueirsensor3 == LOW && state2 == 0){
      digitalWrite(signaldriver3, HIGH);
      digitalWrite(signaldriver4, LOW);
      state2 = 1;
      state = 0;
      }

    if (state == 1){
      Var = 4;
      }
    break;
  case 4:
    digitalWrite(signaldriver1, LOW);
    digitalWrite(signaldriver2, HIGH);
    Var = 0;
    staterfid = 0;
    lcd.setCursor(1, 0);
    lcd.print("                ");
    break;
}

}

void rfid(){
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
    if (rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
        tag += rfid.uid.uidByte[i];
      }
      Serial.println(tag);
    }

  if (tag == ""){
    lcd.setCursor(1, 0);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("ACCESS>>>>>");
    delay(3000);
    lcd.setCursor(1, 0);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("TEKAN BTN");
    staterfid = 1;
    }
  else {
    lcd.setCursor(1, 0);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("INVALID...");
    delay(3000);
    lcd.setCursor(1, 0);
    lcd.print("                ");
    staterfid = 0;
    }
  }
