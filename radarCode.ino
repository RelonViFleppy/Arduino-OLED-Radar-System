#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// Ekran Ayarları
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin Tanımlamaları
Servo myServo;
const int trigPin = 10;
const int echoPin = 11;
const int buzzerPin = 8;
const int servoPin = 9;

// Mesafe Eşiği (Kaç cm'den sonra alarm versin?)
const int alertDistance = 25; 

void setup() {
  Serial.begin(9600); // Bilgisayara veri aktarımı (Data Acquisition)
  
  myServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // OLED Başlatma
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Ekran baglantisi hatali!"));
    for(;;); 
  }
  
  // İlk Açılış Ekranı
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.println("RADAR BASLATILIYOR");
  display.display();
  delay(2000);
}

void loop() {
  // Kablo takılmasını önlemek için 45-135 derece arası tarama
  for(int i = 45; i <= 135; i++){ 
    processRadar(i); 
  }
  for(int i = 135; i >= 45; i--){ 
    processRadar(i); 
  }
}

void processRadar(int angle) {
  myServo.write(angle);
  delay(35); // Servonun oturması için bekleme süresi
  
  int distance = calculateDistance();
  
  // Seri Port Çıktısı (Hocanın istediği format: Açı, Mesafe)
  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance);

  // Ekranı Güncelle
  updateDisplay(angle, distance);
}

int calculateDistance() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout (hatalı ölçümü engeller)
  int dist = duration * 0.034 / 2;
  
  // Hatalı veya çok uzak ölçümleri filtrele
  if (dist > 200 || dist <= 0) dist = 200; 
  return dist;
}

void updateDisplay(int angle, int dist) {
  display.clearDisplay();
  
  // 1. Radar Çizgisini Çiz
  float radian = angle * 3.14 / 180;
  int xLine = 64 + cos(radian) * 60;
  int yLine = 63 - sin(radian) * 60;
  display.drawLine(64, 63, xLine, yLine, WHITE);
  
  // 2. Cisim Algılanırsa İşaretle ve Buzzer Çaldır
  if(dist < alertDistance) {
    // Mesafeyi ekrana sığacak şekilde ölçekle (dist*2 yaklaşık 50px yapar)
    int xObj = 64 + cos(radian) * (dist * 2); 
    int yObj = 63 - sin(radian) * (dist * 2);
    display.fillCircle(xObj, yObj, 3, WHITE); // Cisim noktası
    
    display.setCursor(80, 0);
    display.print("!TESPIT!");
    
    // Buzzer uyarısı
    digitalWrite(buzzerPin, HIGH);
    delay(10); 
    digitalWrite(buzzerPin, LOW);
  }
  
  // 3. Bilgi Metinleri
  display.setCursor(0,0);
  display.print("Aci: "); display.print(angle);
  display.setCursor(0,10);
  display.print("Uzak: "); display.print(dist); display.print("cm");
  
  display.display();
}