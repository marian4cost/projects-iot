#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NewPing.h>

const char *ssid = "17282";
const char *password = "123456789";

const int triggerPin = 5;
const int echoPin = 4;

ESP8266WebServer server(80);
NewPing sonar(triggerPin, echoPin);

void setup() {
  Serial.begin(115200);
  conectarWiFi();
  configurarServidor();
}

void loop() {
  server.handleClient();

  // Medir a distância e exibir no Monitor Serial
  float distancia_cm = sonar.ping_cm();
  Serial.print("Distância: ");
  Serial.print(distancia_cm);
  Serial.println(" cm");

  delay(1000); // Aguarde 1 segundo entre as leituras para evitar uma saída muito rápida no Monitor Serial
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
}

void configurarServidor() {
  server.on("/", HTTP_GET, []() {
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    float distancia_cm = sonar.ping_cm();

    String mensagem = "Distância: " + String(distancia_cm) + " cm";

    String html = "<html><body>";
    html += "<p>" + mensagem + "</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.println("Servidor iniciado");
}
