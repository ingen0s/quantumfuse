#include <WiFi.h>
#include <WebServer.h>
#include <AD985X.h>

// Pins for Freenove ESP32-S3
#define W_CLK 15
#define FQ_UD 16
#define DATA 4
#define RESET 2

AD9851 dds(W_CLK, FQ_UD, DATA, RESET); // Using AD9851

// WiFi credentials - CHANGE THESE
const char* ssid = "";
const char* password = "";

WebServer server(80);

// Current settings
double currentFreq = 0.580; // Default to 580 mHz (0.580 Hz)
unsigned long onDelay = 500;
unsigned long offDelay = 500;
bool isRunning = false;

void setup() {
  pinMode(RESET, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(FQ_UD, OUTPUT);
  pinMode(DATA, OUTPUT);
  
  dds.begin();
  dds.setRefClockHigh(); // Enable 6x multiplier for AD9851 (e.g., 30 MHz crystal → 180 MHz)
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // AD9851 setup
  Serial.println("Initializing AD9851 for web control (50x50x1mm synthetic crystal + piezo pickup).");
  digitalWrite(RESET, HIGH);
  delayMicroseconds(10);
  digitalWrite(RESET, LOW);
  
  // Set initial frequency
  dds.setFrequency(currentFreq);
  
  // Web server routes
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/start", handleStart);
  server.on("/stop", handleStop);
  server.on("/status", handleStatus);
  
  server.begin();
  Serial.println("Web server started. Access at http://[ESP32-IP]/");
}

void loop() {
  server.handleClient();
  
  if (isRunning) {
    // Run the frequency cycle or continuous based on settings
    static unsigned long lastAction = 0;
    static bool isOn = false;
    
    if (millis() - lastAction > (isOn ? onDelay : offDelay)) {
      if (isOn) {
        dds.setFrequency(0);
        Serial.println("OFF");
        isOn = false;
      } else {
        dds.setFrequency(currentFreq);
        Serial.println("ON at " + String(currentFreq, 6) + " Hz");
        isOn = true;
      }
      lastAction = millis();
    }
  }
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>AD9851 Web Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; margin: 20px; }
    .form-group { margin: 10px 0; }
    input, button { padding: 5px; margin: 5px; }
    #status { background: #f0f0f0; padding: 10px; margin: 10px 0; }
  </style>
</head>
<body>
  <h1>AD9851 DDS Web Dashboard</h1>
  <div id="status">Status: Loading...</div>
  
  <h2>Frequency Control</h2>
  <div class="form-group">
    <label>Frequency (Hz, supports mHz like 0.58 or MHz like 1000000):</label>
    <input type="number" id="freq" step="0.001" value="0.58">
  </div>
  
  <h2>Timing Control</h2>
  <div class="form-group">
    <label>ON Delay (ms):</label>
    <input type="number" id="onDelay" value="500">
  </div>
  <div class="form-group">
    <label>OFF Delay (ms):</label>
    <input type="number" id="offDelay" value="500">
  </div>
  
  <h2>AD9851 Phase Control</h2>
  <div class="form-group">
    <label>Phase (0-360°):</label>
    <input type="number" id="phase" min="0" max="360" value="0">
    <button onclick="setPhase()">Set Phase</button>
  </div>
  
  <h2>Control</h2>
  <button onclick="updateSettings()">Update Settings</button>
  <button onclick="startGen()">Start Generator</button>
  <button onclick="stopGen()">Stop Generator</button>
  
  <script>
    function updateStatus() {
      fetch('/status').then(r => r.json()).then(data => {
        document.getElementById('status').innerHTML = 
          'Status: ' + (data.running ? 'Running' : 'Stopped') + 
          '<br>Frequency: ' + data.freq + ' Hz' +
          '<br>ON Delay: ' + data.onDelay + ' ms' +
          '<br>OFF Delay: ' + data.offDelay + ' ms';
      });
    }
    
    function updateSettings() {
      const freq = document.getElementById('freq').value;
      const onDelay = document.getElementById('onDelay').value;
      const offDelay = document.getElementById('offDelay').value;
      fetch('/set?freq=' + freq + '&on=' + onDelay + '&off=' + offDelay)
        .then(r => r.text()).then(msg => alert(msg));
      updateStatus();
    }
    
    function startGen() {
      fetch('/start').then(r => r.text()).then(msg => alert(msg));
      updateStatus();
    }
    
    function stopGen() {
      fetch('/stop').then(r => r.text()).then(msg => alert(msg));
      updateStatus();
    }
    
    function setPhase() {
      const phase = document.getElementById('phase').value;
      fetch('/set?phase=' + phase).then(r => r.text()).then(msg => alert(msg));
    }
    
    // Auto-update status every 2 seconds
    setInterval(updateStatus, 2000);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("freq")) {
    currentFreq = server.arg("freq").toFloat();
    if (isRunning) {
      dds.setFrequency(currentFreq);
    }
    Serial.println("Frequency set to: " + String(currentFreq, 6) + " Hz");
  }
  if (server.hasArg("on")) {
    onDelay = server.arg("on").toInt();
  }
  if (server.hasArg("off")) {
    offDelay = server.arg("off").toInt();
  }
  if (server.hasArg("phase")) {
    int phase = server.arg("phase").toInt();
    dds.setPhase(phase % 360); // Ensure phase is 0-360
    Serial.println("Phase set to: " + String(phase) + " degrees");
  }
  server.send(200, "text/plain", "Settings updated");
}

void handleStart() {
  isRunning = true;
  dds.setFrequency(currentFreq);
  Serial.println("Generator started at " + String(currentFreq, 6) + " Hz");
  server.send(200, "text/plain", "Generator started");
}

void handleStop() {
  isRunning = false;
  dds.setFrequency(0);
  Serial.println("Generator stopped");
  server.send(200, "text/plain", "Generator stopped");
}

void handleStatus() {
  String json = "{";
  json += "\"running\":" + String(isRunning ? "true" : "false") + ",";
  json += "\"freq\":" + String(currentFreq, 6) + ",";
  json += "\"onDelay\":" + String(onDelay) + ",";
  json += "\"offDelay\":" + String(offDelay);
  json += "}";
  server.send(200, "application/json", json);
}
