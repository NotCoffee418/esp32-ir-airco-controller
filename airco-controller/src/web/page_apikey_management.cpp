#include "page_apikey_management.h"
#include "authorization.h"
#include "storage/api_keys.h"
#include "web/web_helpers.h"

// Private functions
void _apiHandleGenerate(WebServer& server);
void _apiHandleRemove(WebServer& server);
void _apiHandleList(WebServer& server);


// Note that particular API endpoints are managed with the session cookie rather than the API key.
void registerApiKeyManagementHandlers(WebServer& server) {
    server.on("/api-keys", [&server]() {
        if (!authorizeWebHandler(server)) {
            return;
        }
        
        serveFile(server, "/web/api_keys.html", "text/html");
    });

    server.on("/api/api-key/generate", [&server]() {
        if (!authorizeWebHandler(server)) {
            return;
        }

        if (!requireGet(server)) {
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
        apiSuccessResp(server, result.key);
    } else {
        apiErrorResp(server, result.error);
    }
}

void _apiHandleRemove(WebServer& server) {
    String key = server.arg("apiKey");
    if (key.isEmpty()) {
        apiErrorResp(server, "Key is required");
        return;
    }

    bool success = removeApiKey(key);
    if (success) {
        apiSuccessResp(server, "Key removed");
    } else {
        apiErrorResp(server, "Failed to remove key");
    }
}

void _apiHandleList(WebServer& server) {
    std::vector<String> keys = getActiveApiKeys();
    apiSuccessResp(server, keys);
}