#include <LittleFS.h>

#include "network/device_identity.h"
#include "storage/api_keys.h"

std::vector<String> getActiveApiKeys() {
    std::vector<String> keys;

    if (!LittleFS.exists("/api_keys.txt")) {
        return keys; // Return empty vector if file doesn't exist
    }

    File file = LittleFS.open("/api_keys.txt", "r");
    if (!file) {
        return keys; // Return empty vector if can't open file
    }

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim(); // Remove whitespace/newlines
        if (line.length() > 0) {
            keys.push_back(line);
        }
    }

    file.close();
    return keys;
}

bool removeApiKey(String key) {
    std::vector<String> keys = getActiveApiKeys();
    
    auto it = std::find(keys.begin(), keys.end(), key);
    if (it == keys.end()) {
        return false; // Key not found
    }
    
    keys.erase(it);
    
    // Write updated list back to file
    File file = LittleFS.open("/api_keys.txt", "w");
    if (!file) {
        return false;
    }
    
    for (const String& apiKey : keys) {
        file.println(apiKey);
    }
    
    file.close();
    return true;
}



GenerateApiKeyResult generateApiKey() {
    std::vector<String> existingKeys = getActiveApiKeys();

    if (existingKeys.size() >= 10) {
        return {"", "Maximum 10 API keys allowed", false};
    }

    String newKey = generateRandomString(32);

    File file = LittleFS.open("/api_keys.txt", "w");
    if (!file) {
        return {"", "Failed to open file", false};
    }

    for (const String& key : existingKeys) {
        file.println(key);
    }

    file.println(newKey);
    file.close();

    return {newKey, "", true};
}