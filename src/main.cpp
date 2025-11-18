#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "SEU_WIFI"
#define WIFI_PASSWORD "SUA_SENHA"

#define API_KEY "SUA_API_KEY"
#define DATABASE_URL "https://seu-projeto.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define LED_VERDE  2
#define LED_VERM   4
#define LED_AZUL   5
#define LED_AMAR   18

#define BTN_VERDE  13
#define BTN_VERM   12
#define BTN_AZUL   14
#define BTN_AMAR   27

int leds[4]   = {LED_VERDE, LED_VERM, LED_AZUL, LED_AMAR};
int botoes[4] = {BTN_VERDE, BTN_VERM, BTN_AZUL, BTN_AMAR};

int sequencia[100];     
int nivel = 0;
int highScore = 0;

void piscaLed(int index) {
  digitalWrite(leds[index], HIGH);
  delay(300);
  digitalWrite(leds[index], LOW);
  delay(150);
}

int lerBotao() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == LOW) { 
      piscaLed(i);
      delay(250);
      return i;
    }
  }
  return -1;
}

void tocarSequencia() {
  for (int i = 0; i < nivel; i++) {
    piscaLed(sequencia[i]);
    delay(300);
  }
}

void salvarHighScore(int score) {
  Firebase.RTDB.setInt(&fbdo, "/ranking/highscore", score);
}

void carregarHighScore() {
  if (Firebase.RTDB.getInt(&fbdo, "/ranking/highscore")) {
    highScore = fbdo.intData();
  } else {
    highScore = 0;
  }
}

void gameOver() {
  Serial.println("ERROU!");

  if (nivel - 1 > highScore) {
    highScore = nivel - 1;
    salvarHighScore(highScore);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) digitalWrite(leds[j], HIGH);
    delay(150);
    for (int j = 0; j < 4; j++) digitalWrite(leds[j], LOW);
    delay(150);
  }

  nivel = 0;
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  carregarHighScore();
  Serial.println("Highscore atual: " + String(highScore));
}

void loop() {
  if (nivel == 0) {
    delay(800);
    sequencia[0] = random(0, 4);
    nivel = 1;
  }

  tocarSequencia();

  for (int i = 0; i < nivel; i++) {
    int botao = -1;

    while (botao == -1) {
      botao = lerBotao();
    }

    // errou
    if (botao != sequencia[i]) {
      gameOver();
      return;
    }
  }

  sequencia[nivel] = random(0, 4);
  nivel++;
  delay(500);
}
