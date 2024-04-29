#include <LiquidCrystal.h>
#include <DHT.h>
 
// Definindo os pinos dos sensores e atuadores
const int ldrPin = A0;        // Pino do LDR
const int greenLedPin = 10;   // Pino do LED verde
const int yellowLedPin = 9;   // Pino do LED amarelo
const int redLedPin = 8;      // Pino do LED vermelho
const int buzzerPin = 7;      // Pino do buzzer
const int dhtPin = 6;         // Pino do DHT22
 
// Configurações do LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
// Inicialização do sensor DHT22
DHT dht(dhtPin, DHT22);
 
// Variáveis para cálculo da média
const int numReadings = 5;
int ldrReadings[numReadings];
int index = 0;
int total = 0;
 
void setup() {
  // Inicializando o monitor serial
  Serial.begin(9600);
 
  // Inicializando o display LCD
  lcd.begin(16, 2);
  lcd.print("      (o_0)");
  lcd.setCursor(0, 1);
  lcd.print("PROJECT  AGNELLO");
  delay(2000);
  lcd.clear();
 
  // Inicializando o sensor DHT22
  dht.begin();
 
  // Configurando os pinos dos atuadores
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
 
  // Definindo o estado inicial dos LEDs
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(yellowLedPin, HIGH);
  digitalWrite(redLedPin, HIGH);
}
 
void loop() {
  // Leitura do sensor de luminosidade (LDR)
  int ldrValue = analogRead(ldrPin);
  // Verificar se a leitura do LDR é válida (não é zero)
  if (ldrValue == 0) {
    Serial.println("Erro: LDR leitura inválida");
    return;
  }
 
  // Mapeamento com checagem de limites
  int brightness = map(ldrValue,1015,8,0,100);
  brightness = constrain(brightness, 0, 100); // Garante que o valor esteja entre 0 e 100
  Serial.println(ldrValue);
 
  // Adiciona a leitura atual ao conjunto de leituras
  total -= ldrReadings[index];
  ldrReadings[index] = brightness;
  total += brightness;
  index = (index + 1) % numReadings;
 
  // Calcula a média das leituras
  int averageBrightness = total / numReadings;
 
  // Exibindo a média da luminosidade no monitor serial
  Serial.print("Média de luminosidade: ");
  Serial.println(brightness);
 
  // Verificando o nível de luminosidade e tomando ações correspondentes
  if (brightness >= 70) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    lcd.clear();
    lcd.print("Ambiente muito");
    lcd.setCursor(0, 1);
    lcd.print("claro");
    delay(5000);
    tone(buzzerPin, 2000, 5000);
  } else if (brightness >= 40) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    lcd.clear();
    lcd.print("Ambiente a meia");
    lcd.setCursor(0, 1);
    lcd.print("luz");
    delay(5000);
  } else {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, LOW);
    lcd.clear();
    lcd.print("Ambiente escuro");
    delay(5000);
  }
 
  // Leitura do sensor de temperatura e umidade (DHT22)
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
 
  // Exibindo as leituras do DHT22 no monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C, Umidade: ");
  Serial.print(humidity);
  Serial.println("%");
 
  // Verificando as condições de temperatura e umidade e tomando ações correspondentes
  if (temperature >= 15.0) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    lcd.clear();
    lcd.print("Temp. ALTA");
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(temperature);
    delay(5000);
    tone(buzzerPin, 2000, 5000);
  } else if (temperature <= 10.0) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    lcd.clear();
    lcd.print("Temp. BAIXA");
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(temperature);
    delay(5000);
    tone(buzzerPin, 2000, 5000);
  } else {
    lcd.clear();
    lcd.print("Temp. OK");
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(temperature);
    delay(5000);
  }
 
  if (humidity >= 70.0) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    lcd.clear();
    lcd.print("Umid. ALTA");
    lcd.setCursor(0, 1);
    lcd.print("Umidade:");
    lcd.print(humidity);
    lcd.print("%");
    delay(5000);
    tone(buzzerPin, 2000, 5000);
  } else if (humidity <= 50.0) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
    lcd.clear();
    lcd.print("Umid. BAIXA");
    lcd.setCursor(0, 1);
    lcd.print("Umidade:");
    lcd.print(humidity);
    lcd.print("%");
    delay(5000);
  } else {
    lcd.clear();
    lcd.print("Umid. OK");
    lcd.setCursor(0, 1);
    lcd.print("Umidade:");
    lcd.print(humidity);
    lcd.print("%");
    delay(5000);
  }
}
