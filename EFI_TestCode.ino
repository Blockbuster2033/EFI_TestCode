// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "FRITZ!Box 7530 ZJ";
const char* password = "25871492552595834549";

bool ledState = 0;
const int ledPin = LED_BUILTIN;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #222222;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
  }
  /*.button:hover {background-color: #0f8b8d}*/
  .button:active {
    background-color: #0f8b8d;
    box-shadow: 2 2px #CDCDCD;
    transform: translateY(2px);
  }
  .state {
    font-size: 1.5rem;
    color:#8c8c8c;
    font-weight: bold;
  }
  .matrix {
    display: grid;
    grid-template-columns: repeat(5, 1fr);
    grid-template-rows: repeat(5, 1fr);
    gap: 1px;
  }
  .matrixfield {
    display: flex;
    justify-content: center;
    align-items: center;
    background-color: #fff;
    border: 1px solid #ccc;
    height: 25px;
    width: 25px;
  }
  body {
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
  }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 2</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
  <div class="matrix">
  <div id="11" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="11" data-name="11" placeholder="0" id="11" /></div>
  <div id="12" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="12" data-name="12" placeholder="0" id="12" /></div>
  <div id="13" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="13" data-name="13" placeholder="0" id="13" /></div>
  <div id="14" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="14" data-name="14" placeholder="0" id="14" /></div>
  <div id="15" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="15" data-name="15" placeholder="0" id="15" /></div>
  <div id="21" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="21" data-name="21" placeholder="0" id="21" /></div>
  <div id="22" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="22" data-name="22" placeholder="0" id="22" /></div>
  <div id="23" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="23" data-name="23" placeholder="0" id="23" /></div>
  <div id="24" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="24" data-name="24" placeholder="0" id="24" /></div>
  <div id="25" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="25" data-name="25" placeholder="0" id="25" /></div>
  <div id="31" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="31" data-name="31" placeholder="0" id="31" /></div>
  <div id="32" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="32" data-name="32" placeholder="0" id="32" /></div>
  <div id="33" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="33" data-name="33" placeholder="0" id="33" /></div>
  <div id="34" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="34" data-name="34" placeholder="0" id="34" /></div>
  <div id="35" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="35" data-name="35" placeholder="0" id="35" /></div>
  <div id="41" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="41" data-name="41" placeholder="0" id="41" /></div>
  <div id="42" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="42" data-name="42" placeholder="0" id="42" /></div>
  <div id="43" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="43" data-name="43" placeholder="0" id="43" /></div>
  <div id="44" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="44" data-name="44" placeholder="0" id="44" /></div>
  <div id="45" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="45" data-name="45" placeholder="0" id="45" /></div>
  <div id="51" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="51" data-name="51" placeholder="0" id="51" /></div>
  <div id="52" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="52" data-name="52" placeholder="0" id="52" /></div>
  <div id="53" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="53" data-name="53" placeholder="0" id="53" /></div>
  <div id="54" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="54" data-name="54" placeholder="0" id="54" /></div>
  <div id="55" class="matrixfield"><input type="text" class="matrixfield" maxlength="256" name="55" data-name="55" placeholder="0" id="55" /></div>
</div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
   function initMatrix() {
    const matrixFields = document.querySelectorAll('.matrixfield input');
    const socket = new WebSocket('ws://localhost:8080');
    socket.addEventListener('open', (event) => {
      console.log('WebSocket connection established');
    });
    matrixFields.forEach(field => {
      field.addEventListener('input', () => {
        if (field.value.trim() !== '') {
          console.log(`Field ${field.id} is filled with value ${field.value}`);
          socket.send(JSON.stringify({ id: field.id, value: field.value }));
        }
      });
    });
  }
  initMatrix();
</script>
</body>
</html>
)rawliteral";

void notifyClients() {
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      Serial.println("LED State has changed.");
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}