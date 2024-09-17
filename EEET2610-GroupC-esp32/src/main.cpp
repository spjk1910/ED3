#include <Arduino.h>
#include "MyMotor.h"
#include "MyEncoder.h"
#include "MyPID.h"
#include "MySerial.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WebSocketsServer.h>
#include "data.h"

// WiFi Credentials
const char *ssid = "ESP32";
const char *password = "0123456780";
const int ws_port = 1337;

// Server and WebSocket instances
AsyncWebServer server(80);
WebSocketsServer webSocket(ws_port);

// Store joystick and angle values
volatile float joystickX = 0.0;
volatile float joystickY = 0.0;
volatile int sliderZ = 0;

char msg_buf[150];

// Semaphore for synchronizing tasks
SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();

// WebSocket callback
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", client_num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(client_num);
            Serial.printf("[%u] Connection from ", client_num);
            Serial.println(ip.toString());
            break;
        }
        case WStype_TEXT: {
            String message = String((char *)payload);
            if (message.startsWith("{")) {
                // Parse the received message to extract x, y, z
                sscanf(message.c_str(), "{\"x\":%f,\"y\":%f,\"z\":%d}", &joystickX, &joystickY, &sliderZ);
                Serial.printf("X: %.2f, Y: %.2f, Z: %d\n", joystickX, joystickY, sliderZ);
            }
            break;
        }
        default:
            break;
    }
}

// Task: Send joystick data if changed
void Send_Task(void *arg) {
    static float prevJoystickX = 0.0, prevJoystickY = 0.0;
    static int prevSliderZ = 0;

    while (1) {
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
            if (joystickX != prevJoystickX || joystickY != prevJoystickY || sliderZ != prevSliderZ) {
                // Ensure the format is consistent
                sprintf(msg_buf, "X: %.2f, Y: %.2f, Z: %d", joystickX, joystickY, sliderZ);

                // Send data via Serial (for MATLAB)
                Serial.println(msg_buf);

                // Send data via WebSocket as before
                webSocket.broadcastTXT(msg_buf);

                // Update previous values
                prevJoystickX = joystickX;
                prevJoystickY = joystickY;
                prevSliderZ = sliderZ;
            }
            xSemaphoreGive(xSemaphore);
        }
        vTaskDelay(200 / portTICK_RATE_MS); // Adjust the delay as needed
    }
}

// Task: Handle SerialDataWrite
void Serial_Task(void *arg) {
    while (1) {
        SerialDataWrite();
        vTaskDelay(10 / portTICK_RATE_MS); // Adjust delay as needed
    }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() + "] HTTP GET request of " + request->url());
    request->send_P(200, "text/html", index_html);
}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() + "] HTTP GET request of " + request->url());
    request->send(404, "text/plain", "Not found");
}

/***********************************************************
 * Main
 */
void setup() {
    // Initialize components
    Init_Motor();
    Init_Encoder();
    Init_PID();
    Init_Serial();

    // Start WiFi access point
    WiFi.softAP(ssid, password);

    // Setup HTTP server
    server.on("/", HTTP_GET, onIndexRequest);
    server.onNotFound(onPageNotFound);
    server.begin();

    // Setup WebSocket server
    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);

    // Create FreeRTOS tasks
    xTaskCreate(Send_Task, "Send_Task", 4096, NULL, 10, NULL);
    xTaskCreate(Serial_Task, "Serial_Task", 4096, NULL, 10, NULL);
}

// Main loop
void loop() {
    Get_Angle();
    Compute_PID();
    Run_Motor();

    webSocket.loop();
}