#include <BluetoothSerial.h>
#include <Servo.h>

//Pinos motores

// Motor Esquerda
#define IN1
#define IN2
#define ENA

// Motor Direita
#define IN3
#define IN4
#define ENB

// Pino do Servo
#define SERVO

// Pino do sensor
#define TRIG
#define ECHO
#define c 0.0343

BluetoothSerial SerialBT
Servo servo;

int velocidade = 255;
String message = "";
int pos = 45;
float duration, distance;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_EQUIPE1");
  Serial.println("Dispositivo iniciado");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  servo.attach(SERVO);
  servo.write(0);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

}

void loop() {

  message = SerialBT.readString();

  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  if (message == "up"){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, velocidade);
    analogWrite(ENB, velocidade);
  }

  if (message = "down"){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, velocidade);
    analogWrite(ENB, velocidade);
  }

  if (message = "left"){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, velocidade);
    analogWrite(ENB, velocidade);
  }

  if (message = "right"){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, velocidade);
    analogWrite(ENB, velocidade);
  }

  if (message = "parar"){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }

  if (message = "dash"){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, velocidade);
    analogWrite(ENB, velocidade);

    delay(2000);
  }

  if (message = "recuo"){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    analogWrite(ENA, velocidade);
    analogWrite(ENB, velocidade);

    delay(2000);
  }


  pulso = pulseIn(ECHO, HIGH);
  distancia = (pulso*c)/2;
  SerialBT.println(distancia);

  if (distancia <= 30){
    servo.write(pos);
    delay(1000);
    servo.write(0);
  }

  delay(100);

}