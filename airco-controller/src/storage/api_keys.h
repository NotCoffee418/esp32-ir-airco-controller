#include <Arduino.h>
#include <vector>

struct GenerateApiKeyResult {
    String key;
    String error;
    bool success;
};


std::vector<String> getActiveApiKeys();
GenerateApiKeyResult generateApiKey();
bool removeApiKey(String key);