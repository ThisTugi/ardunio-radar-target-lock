#include <Servo.h>

#define echoPin 7
#define trigPin 6
#define greenLed 9
#define redLed 10
#define servoPin 3

Servo radarServo;

bool isTargetFound = false;
String mode = "Searching";
int angle = 90;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  radarServo.attach(servoPin);
  radarServo.write(angle);

  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, LOW);

  delay(1000);
}

void loop() {
  
  // İleri Tarama (15° -> 165°)
  for (int a = 15; a <= 165; a += 3) {
    scan(a);
  }

  // Geri Tarama (165° -> 15°)
  for (int a = 165; a >= 15; a -= 3) {
    scan(a);
  }
}

void scan(int targetAngle){
  //Motoru yeni açıya götür ve bekle
  radarServo.write(targetAngle);
  delay(30);

  //Mesafeyi ölç
  int distance = getDistance();

  if(distance >= 5 && distance <= 25){
    delay(20);
    int confirm = getDistance();
    if(confirm < 5 || confirm > 25) return;

    isTargetFound = true;
    mode = "Hunting";
    digitalWrite(redLed,HIGH);
    digitalWrite(greenLed,LOW);

    //Gürültülü verileri engellemek için
    int counter = 0;

    //Hedef sabit kaldığı sürece kitli bekle
    while (isTargetFound) {
      delay(40);
      distance = getDistance();

      if (distance < 5 || distance > 25) {
        counter++;
        if (counter >= 2) {
          isTargetFound = false;
        }
      } else {
        counter = 0;
      }
    }

    mode = "Searching";
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000); 
  if (duration == 0) return 400; 
  return duration * 0.034 / 2;

}