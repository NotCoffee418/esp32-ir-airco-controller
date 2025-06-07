#include <Arduino.h>
#include <WiFi.h>

// WiFi credentials - will be loaded from config.ini
const char* wifi_ssid = "XXXXXXXXXXXXXX";
const char* wifi_password = "XXXXXXXXXXX";

// Static IP configuration
IPAddress local_IP(192, 168, 50, 201);
IPAddress gateway(192, 168, 50, 1);      // Usually your router IP
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);       // Google DNS
IPAddress secondaryDNS(8, 8, 4, 4);

void tempWifiSetup() {
    // Wait a bit to ensure wifi is ready
    Serial.println("Waiting for WiFi to be ready...");
    delay(1000);

    // Configure static IP
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("Failed to configure static IP");
    }


    // Connect to WiFi
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.print("Connecting to WiFi: " + String(wifi_ssid));

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}