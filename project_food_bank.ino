// lcd
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// rfid
#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = 6;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 53;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

//irsensor 1
int irsensor1 = 3;
int valueirsensor1 = 0;

//irsensor 2
int irsensor2 = 4;
int valueirsensor2 = 0;

int total = 3;
int resettotal = 3;

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
int staterfid = 0;

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
  pinMode(signaldriver1, OUTPUT);
  pinMode(signaldriver2, OUTPUT);
  pinMode(signaldriver3, OUTPUT);
  pinMode(signaldriver4, OUTPUT);

  lcd.setCursor(4, 0);
  lcd.print("WELCOME");

  digitalWrite(signaldriver1, LOW);
  digitalWrite(signaldriver2, LOW);

  digitalWrite(signaldriver3, HIGH);
  digitalWrite(signaldriver4, LOW);
}

void loop() {
  //running rfid
  Rfid();
  
  valueButton = digitalRead(Button);
  valueirsensor1 = digitalRead(irsensor1);
  valueirsensor2 = digitalRead(irsensor2);

  if (valueirsensor2 == LOW && state == 1){
    digitalWrite(signaldriver1, LOW);
    digitalWrite(signaldriver2, LOW);
    state = 0;
    }

  if (total == 0){
    digitalWrite(signaldriver1, HIGH);
    digitalWrite(signaldriver2, LOW);
    total = resettotal;
    }

  if (valueButton == LOW && Var == 0 && staterfid == 1){
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("SILA TGU...");
    Var = 1;
    }

  switch (Var) {
  case 1:
    digitalWrite(signaldriver3, LOW);
    digitalWrite(signaldriver4, HIGH);
    
    Var = 2;
    break;
  case 2:
    if (valueirsensor1 == LOW){
      digitalWrite(signaldriver3, HIGH);
      digitalWrite(signaldriver4, LOW);
      state = 1;
      delay(5000); // 5sec
      Var = 3;
      }
    break;
  case 3:
    digitalWrite(signaldriver1, LOW);
    digitalWrite(signaldriver2, HIGH);
    Var = 4;
    break;
  case 4:
    total = total - 1;
    Var = 0;
    staterfid = 0;
    lcd.setCursor(1, 0);
    lcd.print("                ");
    break;
}

}

void Rfid(){
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
    if (rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
        tag += rfid.uid.uidByte[i];
      }
      Serial.println(tag);
    }

  if (tag == "code card"){
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("ACCESS>>>>>");
    delay(3000);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("TEKAN BTN");
    staterfid = 1;
    }
  else {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1, 1);
    lcd.print("INVALID...");
    delay(3000);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    staterfid = 0;
    }
  }
