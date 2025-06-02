#include <IR_Connect.h>

IR_Connect::IR_Connect(String irConnectNumber, String deviceName, uint32_t requestTimeIntervalMs) {
    _ssid = "IR_Connect_" + irConnectNumber;
    _deviceName = deviceName;
    _requestTimeIntervalMs = requestTimeIntervalMs;
    _head = nullptr;
    _onNotFoundCommand = nullptr;
}

IR_Connect::IR_Connect(String irConnectNumber, String deviceName) : IR_Connect(irConnectNumber, deviceName, TYPICAL_REQUEST_TIME_INTERVAL_MS) {}

bool IR_Connect::connect() {
    WiFi.begin(_ssid, "");
    uint32_t startConnectionTimeMs = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startConnectionTimeMs < CONNECT_TIMEOUT_MS) {
        yield();
    }

    if (WiFi.status() == WL_CONNECTED)
        return true;

    WiFi.disconnect();
    return false;
}

bool IR_Connect::isConnected() {
    return (WiFi.status() == WL_CONNECTED) ? true : false;
}

void IR_Connect::disconnect() {
    WiFi.disconnect();
}

void IR_Connect::getCommand() {
    uint32_t timeNow = millis();
    if (timeNow - _lastRequestTime > _requestTimeIntervalMs) {
        _lastRequestTime = timeNow;
        WiFiClient client;
        HTTPClient http;
        http.begin(client, "http://172.217.28.1/get-command");
        http.addHeader("Content-Type", "application/json");
        int httpCode = http.POST("{\"name\": \"" + _deviceName + "\"}");
        if (httpCode > 0) {
            String payload = http.getString();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload.c_str());
            _runCommand(String(doc["command"]));
        }
        http.end();
    }
}


Func* IR_Connect::_findFunc(const String& name) {
    Func* current = _head;
    while (current != nullptr) {
      if (current->name == name)
        return current;
      current = current->next;
    }
    return nullptr;
}

void IR_Connect::on(const String& name, CallbackFunction func) {
    Func* node = _findFunc(name);
    if (node != nullptr)
      node->func = func;
    else {
      Func* newNode = new Func(name, func);
      newNode->next = _head;
      _head = newNode;
    }
  }

void IR_Connect::onNotFoundCommand(CallbackFunction func) {
    _onNotFoundCommand = func;
}

void IR_Connect::_runCommand(const String& name) {
    Func* node = _findFunc(name);
    if (node != nullptr && node->func != nullptr) {
      node->func();
    } else {
      if (_onNotFoundCommand != nullptr)
        _onNotFoundCommand();
    }
}