#ifndef IR_Connect_h
#define IR_Connect_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define TYPICAL_REQUEST_TIME_INTERVAL_MS 500
#define CONNECT_TIMEOUT_MS 10000

typedef void (*CallbackFunction)();

struct Func {
    String name;
    CallbackFunction func;
    Func* next;
  
    Func(const String& k, CallbackFunction f) : name(k), func(f), next(nullptr) {}
};

class IR_Connect {
    public:
        IR_Connect(String irConnectNumber, String deviceName);
        IR_Connect(String irConnectNumber, String deviceName, uint32_t requestTimeIntervalMs);

        bool connect();
        bool isConnected();
        void disconnect();
        void getCommand();
        void on(const String& key, CallbackFunction func);
        void onNotFoundCommand(CallbackFunction func);
    
    private:
        String _ssid;
        String _deviceName;
        uint32_t _requestTimeIntervalMs;
        uint32_t _lastRequestTime = 0;

        Func* _findFunc(const String& name);
        Func* _head;
        CallbackFunction _onNotFoundCommand;
        void _runCommand(const String& name);

};
#endif