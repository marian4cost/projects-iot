#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "";
const char *password = "";

const int triggerPin = 5;
const int echoPin = 4;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT); 
  pinMode(echoPin, INPUT);    
  
  conectarWiFi();
  configurarServidor();
}


void loop() {
  server.handleClient();

  float distancia_cm = sonarPing();
  Serial.print("volume: ");
  Serial.print(distancia_cm);
  Serial.println(" cm");

  delay(1000); 
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("conectando");
  }
  Serial.println("conectado");
}

float sonarPing() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2.0;
}

void configurarServidor() {
  server.on("/", HTTP_GET, []() {
    Serial.print("endereço ip: ");
    Serial.println(WiFi.localIP());

    float distancia_cm = sonarPing();

    String mensagem = "volume: " + String(distancia_cm) + " cm";

    String html = "<html><body>";
    html += "<p>" + mensagem + "</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.println("localhost startada");
}
