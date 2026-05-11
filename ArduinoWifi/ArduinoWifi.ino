#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

#define RESET_PIN 4

WebServer server(80);
DNSServer dnsServer;
Preferences preferences;

const byte DNS_PORT = 53;

bool configMode = false;

// ================= HTML PRO =================
String htmlPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="es-CO">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>ESP32 • Configuración WiFi</title>

<style>

:root{
  --bg1:#0f172a;
  --bg2:#1e293b;
  --glass:rgba(255,255,255,0.12);
  --border:rgba(255,255,255,0.18);
  --text:#ffffff;
  --accent:#38bdf8;
  --accent2:#6366f1;
  --danger:#ef4444;
  --success:#22c55e;
}

*{
  margin:0;
  padding:0;
  box-sizing:border-box;
  font-family:'Segoe UI',sans-serif;
}

body{
  min-height:100vh;
  display:flex;
  justify-content:center;
  align-items:center;
  overflow:hidden;
  background:linear-gradient(135deg,var(--bg1),var(--bg2));
  color:var(--text);
  position:relative;
}

/* Fondo animado */

body::before,
body::after{
  content:'';
  position:absolute;
  width:350px;
  height:350px;
  border-radius:50%;
  filter:blur(80px);
  opacity:0.5;
}

body::before{
  background:#3b82f6;
  top:-100px;
  left:-100px;
}

body::after{
  background:#8b5cf6;
  bottom:-100px;
  right:-100px;
}

/* Card */

.card{
  position:relative;
  width:90%;
  max-width:420px;
  padding:28px;
  border-radius:28px;
  background:var(--glass);
  backdrop-filter:blur(18px);
  border:1px solid var(--border);
  box-shadow:0 8px 40px rgba(0,0,0,0.35);
  animation:fadeUp 0.7s ease;
}

@keyframes fadeUp{
  from{
    opacity:0;
    transform:translateY(20px);
  }
  to{
    opacity:1;
    transform:translateY(0);
  }
}

.logo{
  font-size:55px;
  margin-bottom:10px;
}

h1{
  font-size:24px;
  font-weight:700;
  margin-bottom:5px;
}

.subtitle{
  opacity:0.8;
  font-size:14px;
  margin-bottom:25px;
}

/* Inputs */

.field{
  margin-bottom:15px;
}

label{
  display:block;
  text-align:left;
  margin-bottom:8px;
  font-size:14px;
  opacity:0.9;
}

select,
input{
  width:100%;
  padding:14px;
  border:none;
  border-radius:14px;
  background:rgba(255,255,255,0.15);
  color:white;
  font-size:15px;
  outline:none;
  transition:0.3s;
}

select option{
  color:black;
}

input::placeholder{
  color:rgba(255,255,255,0.7);
}

select:focus,
input:focus{
  background:rgba(255,255,255,0.22);
  transform:scale(1.02);
}

/* Botones */

button{
  width:100%;
  padding:14px;
  border:none;
  border-radius:14px;
  font-size:15px;
  font-weight:600;
  cursor:pointer;
  transition:0.3s;
  margin-top:10px;
}

.primary{
  background:linear-gradient(135deg,var(--accent),var(--accent2));
  color:white;
}

.primary:hover{
  transform:translateY(-2px);
  box-shadow:0 8px 20px rgba(59,130,246,0.4);
}

.secondary{
  background:rgba(255,255,255,0.12);
  color:white;
}

.secondary:hover{
  background:rgba(255,255,255,0.18);
}

.danger{
  background:linear-gradient(135deg,#ef4444,#dc2626);
  color:white;
}

.danger:hover{
  transform:translateY(-2px);
}

/* Estado */

.status{
  margin-top:18px;
  min-height:22px;
  font-size:14px;
  text-align:center;
  transition:0.3s;
}

/* Loader */

.loader{
  width:18px;
  height:18px;
  border:3px solid rgba(255,255,255,0.3);
  border-top:3px solid white;
  border-radius:50%;
  display:inline-block;
  animation:spin 1s linear infinite;
  margin-right:8px;
  vertical-align:middle;
}

@keyframes spin{
  100%{
    transform:rotate(360deg);
  }
}

.footer{
  margin-top:22px;
  text-align:center;
  font-size:12px;
  opacity:0.7;
}

.signal{
  float:right;
  opacity:0.7;
}

</style>
</head>

<body>

<div class="card">

  <div class="logo">📡</div>

  <h1>ESP32 WiFi Setup</h1>

  <div class="subtitle">
    Configura tu dispositivo de forma rápida y segura 🇨🇴
  </div>

  <div class="field">
    <label>Red WiFi</label>
    <select id="ssid"></select>
  </div>

  <div class="field">
    <label>Contraseña</label>
    <input type="password" id="password" placeholder="Ingresa la contraseña">
  </div>

  <button class="primary" onclick="conectar()">
    Conectar dispositivo
  </button>

  <button class="secondary" onclick="scan()">
    🔄 Actualizar redes
  </button>

  <button class="danger" onclick="resetESP()">
    ♻ Restablecer configuración
  </button>

  <div class="status" id="estado"></div>

  <div class="footer">
    Sistema IoT ESP32 • Ingeniería Informatica
  </div>

</div>

<script>

function nivelSenal(rssi){

  if(rssi >= -55) return "📶 Excelente";
  if(rssi >= -70) return "📶 Buena";
  if(rssi >= -85) return "📶 Media";

  return "📶 Baja";
}

function scan(){

  document.getElementById("estado").innerHTML =
    '<span class="loader"></span>Buscando redes...';

  fetch('/scan')
  .then(res => res.json())
  .then(data => {

    let select = document.getElementById("ssid");
    select.innerHTML = "";

    data.forEach(red => {

      let option = document.createElement("option");

      option.value = red.ssid;

      option.text =
        red.ssid + " • " + nivelSenal(red.rssi);

      select.appendChild(option);
    });

    document.getElementById("estado").innerHTML =
      "✅ Redes actualizadas";

  })
  .catch(() => {

    document.getElementById("estado").innerHTML =
      "❌ Error escaneando redes";

  });
}

function conectar(){

  let ssid = document.getElementById("ssid").value;
  let password = document.getElementById("password").value;

  document.getElementById("estado").innerHTML =
    '<span class="loader"></span>Conectando dispositivo...';

  fetch('/connect', {

    method:'POST',

    headers:{
      'Content-Type':'application/json'
    },

    body:JSON.stringify({
      ssid:ssid,
      password:password
    })

  })

  .then(res => res.text())

  .then(msg => {

    document.getElementById("estado").innerHTML = msg;

  })

  .catch(() => {

    document.getElementById("estado").innerHTML =
      "❌ Error de conexión";

  });
}

function resetESP(){

  if(confirm("¿Deseas borrar la configuración WiFi guardada?")){

    fetch('/reset')
    .then(() => {

      document.getElementById("estado").innerHTML =
        "♻ Reiniciando dispositivo...";

    });

  }
}

scan();

</script>

</body>
</html>
)rawliteral";
}

// ================= WIFI =================
bool tryConnect(String ssid, String pass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    delay(500);
    attempts++;
  }

  return WiFi.status() == WL_CONNECTED;
}

// ================= AP =================
void startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_Setup");

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  configMode = true;

  Serial.println("Modo AP");
  Serial.println(WiFi.softAPIP());
}

// ================= ENDPOINTS =================

// UI
void handleRoot() {

  Serial.println("Cliente conectado al portal");

  server.send(200, "text/html", htmlPage());
}

// Escaneo WiFi
void handleScan() {
  int n = WiFi.scanNetworks();
  String json = "[";

  for (int i = 0; i < n; i++) {
    json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + "}";
    if (i < n-1) json += ",";
  }

  json += "]";
  server.send(200, "application/json", json);
}

// Conectar
void handleConnect() {
  String body = server.arg("plain");

  int s = body.indexOf("ssid");
  int p = body.indexOf("password");

  String ssid = body.substring(body.indexOf(":", s)+2, body.indexOf(",", s)-1);
  String pass = body.substring(body.indexOf(":", p)+2, body.indexOf("}", p)-1);

  if (tryConnect(ssid, pass)) {

    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("pass", pass);
    preferences.end();

    server.send(200, "text/plain", "✅ Conectado! Reiniciando...");
    delay(1500);
    ESP.restart();

  } else {
    server.send(200, "text/plain", "❌ Error de conexión");
  }
}

// Reset
void handleReset() {
  preferences.begin("wifi", false);
  preferences.clear();
  preferences.end();

  server.send(200, "text/plain", "Reset OK");
  delay(1000);
  ESP.restart();
}

// ================= INIT =================
bool connectSaved() {

  preferences.begin("wifi", true);

  String ssid = preferences.getString("ssid", "");
  String pass = preferences.getString("pass", "");

  preferences.end();

  if (ssid == "") {

    Serial.println("No hay credenciales guardadas");
    return false;
  }

  Serial.println("Conectando a WiFi...");
  Serial.println(ssid);

  bool connected = tryConnect(ssid, pass);

  if (connected) {

    Serial.println("");
    Serial.println("✅ WiFi conectado");
    Serial.print("📡 IP: ");
    Serial.println(WiFi.localIP());

  } else {

    Serial.println("❌ No se pudo conectar");
  }

  return connected;
}

void checkReset() {

  static unsigned long pressStart = 0;

  if (digitalRead(RESET_PIN) == LOW) {

    if (pressStart == 0) {
      pressStart = millis();
    }

    if (millis() - pressStart > 3000) {

      Serial.println("Borrando configuración WiFi...");

      preferences.begin("wifi", false);
      preferences.clear();
      preferences.end();

      delay(1000);

      ESP.restart();
    }

  } else {

    pressStart = 0;
  }

}

void setup() {

  Serial.begin(115200);

  pinMode(RESET_PIN, INPUT_PULLUP);

  bool connected = connectSaved();

  if (!connected) {

    Serial.println("Iniciando modo AP...");
    startAP();

  } else {

    Serial.println("Modo STA activo");
    Serial.print("IP del ESP32: ");
    Serial.println(WiFi.localIP());
  }

  // ================= ENDPOINTS =================

  server.on("/", handleRoot);

  server.on("/scan", handleScan);

  server.on("/connect", HTTP_POST, handleConnect);

  server.on("/reset", handleReset);

  // Captura cualquier ruta desconocida
  server.onNotFound([]() {
    server.send(200, "text/html", htmlPage());
  });

  // Inicia servidor
  server.begin();

  Serial.println("Servidor HTTP iniciado");
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  if (configMode) {
    dnsServer.processNextRequest();
  }

  checkReset();

}