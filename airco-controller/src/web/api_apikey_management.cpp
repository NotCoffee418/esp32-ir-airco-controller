#include "api_apikey_management.h"
#include "authorization.h"
#include "api_structs.h"
#include "storage/api_keys.h"
#include "web/web_helpers.h"

// Private functions
void _apiHandleGenerate(WebServer& server);
void _apiHandleRemove(WebServer& server);
void _apiHandleList(WebServer& server);


// Note that particular API endpoints are managed with the session cookie rather than the API key.
void registerApiKeyManagementApiHandlers(WebServer& server) {
    server.on("/api/api-key/generate", [&server]() {
        if (!authorizeWebHandler(server)) {
            return;
        }

        if (!requirePost(server)) {
            return;
        }

        _apiHandleGenerate(server);
    });

    server.on("/api/api-key/remove", [&server]() {
        if (!authorizeWebHandler(server)) {
            return;
        }

        if (!requirePost(server)) {
            return;
        }

        _apiHandleRemove(server);
    });

    server.on("/api/api-key/list", [&server]() {
        if (!authorizeWebHandler(server)) {
            return;
        }
        _apiHandleList(server);
    });
}

void _apiHandleGenerate(WebServer& server) {
    GenerateApiKeyResult result = generateApiKey();
    if (result.success) {
        server.send(200, "text/plain", apiSuccessResp(result.key));
    } else {
        server.send(500, "text/plain", apiErrorResp(result.error));
    }
}

void _apiHandleRemove(WebServer& server) {
    String key = server.arg("apiKey");
    if (key.isEmpty()) {
        server.send(400, "text/plain", apiErrorResp("Key is required"));
        return;
    }

    bool success = removeApiKey(key);
    if (success) {
        server.send(200, "text/plain", apiSuccessResp("Key removed"));
    } else {
        server.send(500, "text/plain", apiErrorResp("Failed to remove key"));
    }
}

void _apiHandleList(WebServer& server) {
    std::vector<String> keys = getActiveApiKeys();
    server.send(200, "text/plain", apiSuccessResp(keys));
}