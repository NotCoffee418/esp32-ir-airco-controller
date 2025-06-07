#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ArduinoJson.h>
#include <ir_Fujitsu.h>
#include <LittleFS.h>

// WiFi credentials - will be loaded from config.ini
String wifi_ssid = "YOUR_WIFI_SSID";
String wifi_password = "YOUR_WIFI_PASSWORD";
String config_static_ip = "192.168.50.201";
String config_gateway = "192.168.50.1";

// Static IP configuration
IPAddress local_IP(192, 168, 50, 201);
IPAddress gateway(192, 168, 50, 1);      // Usually your router IP
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);       // Google DNS
IPAddress secondaryDNS(8, 8, 4, 4);

// IR receiver setup
const uint16_t IR_RECV_PIN = 4;  // GPIO4
const uint16_t CAPTURE_BUFFER_SIZE = 1024;
const uint8_t TIMEOUT = 50;  // Longer timeout to catch multiple frames
const uint32_t FRAME_COLLECTION_TIME = 3000;  // Collect frames for 3 seconds
unsigned long lastDecodeTime = 0;

IRrecv irrecv(IR_RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
decode_results results;
WebServer server(80);

// Function prototypes (tells compiler these functions exist)
void handleRoot();
void handleLastSignal();
void handleStatus();
void handleNotFound();
void loadConfig();

// Store last received IR signal with decoded data
struct IRSignal {
    String protocol;
    std::vector<uint64_t> values;
    std::vector<uint16_t> bits;
    String decodedData;
    String modelInfo;
    String rawData;
    unsigned long timestamp;
    bool hasData;
    bool hasDecodedData;
    int frameCount;
} lastSignal = {"", {}, {}, "", "", "", 0, false, false, 0};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting IR HTTP Server...");
    
    // Try to initialize filesystem, but don't worry if it fails
    if (LittleFS.begin(true)) {
        Serial.println("LittleFS mounted successfully");
        loadConfig();
    } else {
        Serial.println("LittleFS not available, using hardcoded defaults");
    }
    
    // If still using defaults, you can hardcode your WiFi here temporarily
    if (wifi_ssid == "YOUR_WIFI_SSID") {
        wifi_ssid = "YourActualSSID";           // Put your real SSID here
        wifi_password = "YourActualPassword";   // Put your real password here
        Serial.println("Using hardcoded WiFi credentials");
    }
    
    // Initialize IR receiver
    irrecv.enableIRIn();
    Serial.println("IR Receiver enabled on GPIO " + String(IR_RECV_PIN));
    
    // Configure static IP
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("Failed to configure static IP");
    }
    
    // Connect to WiFi
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    Serial.print("Connecting to WiFi: " + wifi_ssid);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Setup HTTP endpoints
    server.on("/", handleRoot);
    server.on("/last-signal", handleLastSignal);
    server.on("/status", handleStatus);
    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println("HTTP server started");
    Serial.println("Endpoints:");
    Serial.println("  GET / - Web interface");
    Serial.println("  GET /last-signal - JSON with last IR signal");
    Serial.println("  GET /status - System status");
}

void loop() {
    // Handle HTTP requests
    server.handleClient();
    
    // Check for IR signals
    if (irrecv.decode(&results)) {
        unsigned long now = millis();
        
        // If this is a new protocol or too much time passed, start fresh  
        if (lastSignal.protocol != typeToString(results.decode_type) || 
            (now - lastDecodeTime) > FRAME_COLLECTION_TIME) {
            
            lastSignal.protocol = typeToString(results.decode_type);
            lastSignal.values.clear();
            lastSignal.bits.clear();
            lastSignal.frameCount = 0;
            lastSignal.rawData = "";
            lastSignal.decodedData = "";
            lastSignal.modelInfo = "";
            lastSignal.hasDecodedData = false;
            Serial.println("=== Starting new IR sequence ===");
        }
        
        // Add this frame to the sequence
        lastSignal.values.push_back(results.value);  // Keep for compatibility
        lastSignal.bits.push_back(results.bits);
        lastSignal.frameCount++;
        lastSignal.hasData = true;
        lastSignal.timestamp = now;
        lastDecodeTime = now;
        
        // Capture FULL hex data from state array (not just results.value)
        String fullHex = "0x";
        for (int i = results.bits / 8 - 1; i >= 0; i--) {  // Big endian
            if (results.state[i] < 16) fullHex += "0";
            fullHex += String(results.state[i], HEX);
        }
        
        // Also create little endian version to compare
        String fullHexLE = "0x";
        for (int i = 0; i < results.bits / 8; i++) {  // Little endian
            if (results.state[i] < 16) fullHexLE += "0";
            fullHexLE += String(results.state[i], HEX);
        }
        
        // Update raw data with full hex
        if (lastSignal.frameCount > 1) lastSignal.rawData += " | ";
        lastSignal.rawData += "Frame" + String(lastSignal.frameCount) + ": " + fullHex + " (" + String(results.bits) + " bits)";
        
        // Print to serial for debugging
        Serial.println("=== IR Frame " + String(lastSignal.frameCount) + " ===");
        Serial.println("Protocol: " + lastSignal.protocol);
        Serial.println("Full Hex BE (" + String(results.bits) + " bits): " + fullHex);
        Serial.println("Full Hex LE (" + String(results.bits) + " bits): " + fullHexLE);
        Serial.println("Partial (results.value): 0x" + String(results.value, HEX));
        Serial.println("Gap since last: " + String(now - lastDecodeTime) + "ms");
        
        // Show byte-by-byte breakdown
        Serial.print("State bytes: ");
        for (uint16_t i = 0; i < results.bits / 8; i++) {
            if (results.state[i] < 16) Serial.print("0");
            Serial.print(results.state[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        
        // Try to decode EVERY frame as Fujitsu AC to see what the library sees
        if (lastSignal.protocol == "FUJITSU_AC") {
            Serial.println("=== Attempting Fujitsu AC decode on this frame ===");
            
            IRFujitsuAC ac(0);
            if (ac.validChecksum(results.state, results.bits / 8)) {
                ac.setRaw(results.state, results.bits / 8);
                
                Serial.println("DECODE SUCCESS for Frame " + String(lastSignal.frameCount) + ":");
                Serial.println("Power: " + String(ac.getPower() ? "ON" : "OFF"));
                Serial.println("Mode: " + String(ac.getMode()));
                Serial.println("Temperature: " + String(ac.getTemp()) + "°C");
                Serial.println("Fan Speed: " + String(ac.getFanSpeed()));
                Serial.println("Swing: " + String(ac.getSwing()));
                
                // Store the LATEST successful decode
                lastSignal.decodedData = "Power: " + String(ac.getPower() ? "ON" : "OFF") + 
                                       " | Mode: " + String(ac.getMode()) + 
                                       " | Temp: " + String(ac.getTemp()) + "°C" +
                                       " | Fan: " + String(ac.getFanSpeed()) +
                                       " | Swing: " + String(ac.getSwing()) +
                                       " | Model: " + String(ac.getModel()) +
                                       " | Frame: " + String(lastSignal.frameCount);
                
                // Extract model info for web display from the full toString() output
                String fullStateString = ac.toString();
                int modelStart = fullStateString.indexOf("Model: ");
                if (modelStart != -1) {
                    int modelEnd = fullStateString.indexOf(",", modelStart);
                    if (modelEnd != -1) {
                        lastSignal.modelInfo = fullStateString.substring(modelStart + 7, modelEnd); // Skip "Model: "
                    } else {
                        lastSignal.modelInfo = "Model " + String(ac.getModel());
                    }
                } else {
                    lastSignal.modelInfo = "Model " + String(ac.getModel());
                }
                
                lastSignal.hasDecodedData = true;
            } else {
                Serial.println("DECODE FAILED for Frame " + String(lastSignal.frameCount) + " (bad checksum)");
            }
        }
        
        Serial.println();
        
        // Resume receiving
        irrecv.resume();
    }
    
    delay(50);  // Small delay to prevent overwhelming the loop
}

void handleRoot() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 IR Receiver</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial; margin: 40px; background: #f0f0f0; }
        .container { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        .signal { background: #e8f4fd; padding: 15px; border-radius: 5px; margin: 10px 0; }
        .decoded { background: #e8f8e8; padding: 12px; border-radius: 5px; margin: 8px 0; border-left: 4px solid #4CAF50; }
        .technical { background: #f8f8f8; padding: 10px; border-radius: 5px; margin: 8px 0; font-family: monospace; font-size: 0.9em; }
        .timestamp { color: #666; font-size: 0.9em; }
        .stats { display: flex; gap: 15px; margin: 10px 0; }
        .stat { background: #fff; padding: 8px 12px; border-radius: 4px; border: 1px solid #ddd; }
        .note { margin-top: 10px; font-size: 0.8em; color: #888; font-style: italic; }
        button { background: #007cba; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; }
        button:hover { background: #005a87; }
    </style>
</head>
<body>
    <div class="container">
        <h1 id="pageTitle">ESP32 IR Receiver</h1>
        <p>Point your remote at the receiver and press any button!</p>
        
        <button onclick="refreshSignal()">Refresh Last Signal</button>
        
        <div id="lastSignal">
            <h3>Last Signal:</h3>
            <div id="signalData">No signal received yet...</div>
        </div>
    </div>

    <script>
        function refreshSignal() {
            fetch('/last-signal')
                .then(response => response.json())
                .then(data => {
                    const signalDiv = document.getElementById('signalData');
                    if (data.hasData) {
                        // Update page title with model info
                        const titleElement = document.getElementById('pageTitle');
                        if (data.modelInfo) {
                            titleElement.textContent = `ESP32 IR Receiver - Fujitsu ${data.modelInfo}`;
                        } else {
                            titleElement.textContent = `ESP32 IR Receiver - ${data.protocol}`;
                        }
                        
                        let html = `
                            <div class="signal">
                                <strong>Protocol:</strong> ${data.protocol}
                                <div class="timestamp">Received: ${new Date(data.timestamp).toLocaleString()}</div>
                            </div>
                        `;
                        
                        if (data.hasDecodedData) {
                            html += `
                                <div class="decoded">
                                    <strong>Decoded AC Settings:</strong><br>
                                    ${data.decodedData.replace(/\|/g, '<br>')}
                                </div>
                            `;
                        }
                        
                        html += `
                            <div class="stats">
                                <div class="stat"><strong>Frames:</strong> ${data.frameCount}</div>
                                <div class="stat"><strong>Frequency:</strong> ${data.frequency || '38kHz'}</div>
                                <div class="stat"><strong>Signal:</strong> ${data.signalStrength || 'Good'}</div>
                                <div class="stat"><strong>Total Bits:</strong> ${data.frames.reduce((sum, frame) => sum + frame.bits, 0)}</div>
                            </div>
                        `;
                        
                        html += `<div class="technical"><strong>Technical Details:</strong><br>`;
                        html += `${data.rawData || 'No raw data available'}<br>`;
                        html += `<br><strong>Frame Breakdown:</strong><br>`;
                        data.frames.forEach((frame, index) => {
                            html += `Frame ${index + 1}: ${frame.valueHex} (${frame.bits} bits - partial view)<br>`;
                        });
                        html += `</div>`;
                        
                        html += `
                            <div class="note">
                                Tip: Try changing temperature, mode, or fan speed on your remote to see live updates!
                            </div>
                        `;
                        
                        signalDiv.innerHTML = html;
                    } else {
                        signalDiv.innerHTML = 'No signal received yet...';
                    }
                })
                .catch(err => console.error('Error:', err));
        }
        
        // Auto-refresh every 2 seconds
        setInterval(refreshSignal, 2000);
        
        // Initial load
        refreshSignal();
    </script>
</body>
</html>
    )rawliteral";
    
    server.send(200, "text/html", html);
}

void handleLastSignal() {
    JsonDocument doc;
    
    if (lastSignal.hasData) {
        doc["hasData"] = true;
        doc["protocol"] = lastSignal.protocol;
        doc["timestamp"] = lastSignal.timestamp;
        doc["frameCount"] = lastSignal.frameCount;
        doc["decodedData"] = lastSignal.decodedData;
        doc["hasDecodedData"] = lastSignal.hasDecodedData;
        doc["modelInfo"] = lastSignal.modelInfo;
        doc["rawData"] = lastSignal.rawData;
        doc["signalStrength"] = "Strong";
        doc["frequency"] = "38kHz";
        
        JsonArray frames = doc["frames"].to<JsonArray>();
        for (int i = 0; i < lastSignal.frameCount; i++) {
            JsonObject frame = frames.add<JsonObject>();
            frame["value"] = lastSignal.values[i];
            frame["valueHex"] = "0x" + String(lastSignal.values[i], HEX);
            frame["bits"] = lastSignal.bits[i];
        }
    } else {
        doc["hasData"] = false;
        doc["message"] = "No IR signal received yet";
    }
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    server.send(200, "application/json", jsonString);
}

void handleStatus() {
    JsonDocument doc;
    doc["uptime"] = millis();
    doc["freeHeap"] = ESP.getFreeHeap();
    doc["wifiStatus"] = WiFi.status();
    doc["ipAddress"] = WiFi.localIP().toString();
    doc["irReceiverPin"] = IR_RECV_PIN;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    server.send(200, "application/json", jsonString);
}

void handleNotFound() {
    server.send(404, "text/plain", "404 Not Found");
}

void loadConfig() {
    if (!LittleFS.exists("/config.ini")) {
        Serial.println("Config file (config.ini) not found, using defaults");
        return;
    }
    
    File configFile = LittleFS.open("/config.ini", "r");
    if (!configFile) {
        Serial.println("Failed to open config.ini");
        return;
    }
    
    String currentSection = "";
    
    while (configFile.available()) {
        String line = configFile.readStringUntil('\n');
        line.trim();
        
        // Skip empty lines and comments
        if (line.length() == 0 || line.startsWith("#") || line.startsWith(";")) {
            continue;
        }
        
        // Check for section headers [section]
        if (line.startsWith("[") && line.endsWith("]")) {
            currentSection = line.substring(1, line.length() - 1);
            Serial.println("Reading section: " + currentSection);
            continue;
        }
        
        // Parse key = value pairs
        int equalPos = line.indexOf('=');
        if (equalPos > 0) {
            String key = line.substring(0, equalPos);
            String value = line.substring(equalPos + 1);
            key.trim();
            value.trim();
            
            // Remove quotes if present
            if (value.startsWith("\"") && value.endsWith("\"")) {
                value = value.substring(1, value.length() - 1);
            }
            
            // Process based on section and key
            if (currentSection == "wifi") {
                if (key == "ssid") {
                    wifi_ssid = value;
                    Serial.println("Loaded SSID: " + wifi_ssid);
                } else if (key == "password") {
                    wifi_password = value;
                    Serial.println("Loaded WiFi password");
                }
            } else if (currentSection == "network") {
                if (key == "static_ip") {
                    config_static_ip = value;
                    Serial.println("Loaded static IP: " + config_static_ip);
                } else if (key == "gateway") {
                    config_gateway = value;
                    Serial.println("Loaded gateway: " + config_gateway);
                }
            }
        }
    }
    
    configFile.close();
    Serial.println("Config loaded successfully from config.ini");
}