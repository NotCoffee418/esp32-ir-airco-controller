// Logic for identifying the device

#include <Arduino.h>
#include <WiFi.h>

uint32_t simpleHash(const uint8_t* mac, const char* salt);
String hashToAlphaNum(uint32_t hash, int length);


// Character set for PIN (no confusing chars: 0,1,I,O)
const char CHARSET[] = "23456789ABCDEFGHJKLMNPQRSTUVWXYZ";
const int CHARSET_SIZE = 32;

String getSetupHotspotName() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    String name = "ACCTRL-";
    
    uint32_t hotspotHash = simpleHash(mac, "HOTSPOT");
    return name + hashToAlphaNum(hotspotHash, 4);
}

String getAccessPin() {
    uint8_t mac[6];
    WiFi.macAddress(mac);

    uint32_t pinHash = simpleHash(mac, "PINCODE");
    return hashToAlphaNum(pinHash, 8);
}


// Simple hash function for MAC address + salt
uint32_t simpleHash(const uint8_t* mac, const char* salt) {
  uint32_t hash = 0x811c9dc5; // FNV offset basis
  
  // Hash MAC bytes
  for (int i = 0; i < 6; i++) {
    hash ^= mac[i];
    hash *= 0x01000193; // FNV prime
  }
  
  // Hash salt
  while (*salt) {
    hash ^= *salt++;
    hash *= 0x01000193;
  }
  
  return hash;
}


// Generate alphanumeric string from hash
String hashToAlphaNum(uint32_t hash, int length) {
    String result = "";
    for (int i = 0; i < length; i++) {
        result += CHARSET[hash % CHARSET_SIZE];
        hash /= CHARSET_SIZE;
        
        // Rehash when we're running low on entropy
        if (hash < CHARSET_SIZE && i < length - 1) {
            hash = hash * 0x01000193 + 0x811c9dc5; // Mix it up
        }
    }
    return result;
}

String generateRandomString(int length) {
   String result = "";
   
   // Use current time + random as seed for hash
   uint32_t hash = millis() + random(0xFFFFFFFF);
   hash ^= 0x811c9dc5; // FNV offset basis for extra mixing
   
   for (int i = 0; i < length; i++) {
       result += CHARSET[hash % CHARSET_SIZE];
       hash *= 0x01000193; // FNV prime
       hash ^= (hash >> 16); // Additional mixing
   }
   
   return result;
}