#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NewPing.h>

const char *ssid = "SuaRedeWiFi";
const char *password = "SuaSenhaWiFi";

const int triggerPin = ;
const int echoPin = ;

const int maxDistance = 200;

NewPing sonar(triggerPin, echoPin, maxDistance);

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  conectarWiFi();
  configurarServidor();
}

void loop() {
  server.handleClient();
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
}

float medirVolumeAgua() {
  delay(50);
  return sonar.ping_cm();
}

void configurarServidor() {
  // Manipuladores de rota
  server.on("/", HTTP_GET, []() {
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    float distance_cm = medirVolumeAgua();

    String mensagem;
    if (distance_cm > 20) {
      mensagem = "Nível alto";
    } else {
      mensagem = "Tudo sob controle";
    }

    String html = "<html><body>";
    html += "<p>" + mensagem + ": " + String(distance_cm) + " cm</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.println("Servidor iniciado");
}
