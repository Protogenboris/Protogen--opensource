#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Define GPIO pins for buttons
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_C 7
#define BUTTON_D 4
#define BUTTON_E 20
#define BUTTON_F 8
#define BUTTON_G 9

// MAC address of the receiver (Primary ESP32)
uint8_t receiverMac[] = {0x24, 0xDC, 0xC3, 0x99, 0xE4, 0x8C}; // Replace with actual MAC

// Function to send a command
void sendCommand(const char* command) {
    esp_now_send(receiverMac, (uint8_t*)command, strlen(command));
    Serial.printf("Command '%s' sent.\n", command);
}

// ESP-NOW status callback
void onSent(const uint8_t* macAddr, esp_now_send_status_t status) {
    Serial.printf("Last command sent to %02X:%02X:%02X:%02X:%02X:%02X: %s\n",
                  macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5],
                  (status == ESP_NOW_SEND_SUCCESS) ? "Success" : "Failed");
}

void setup() {
    Serial.begin(115200);

    // Initialize WiFi in STA mode
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(onSent);

    // Register peer (receiver)
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, receiverMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    // Configure buttons as input
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
    pinMode(BUTTON_D, INPUT_PULLUP);
    pinMode(BUTTON_E, INPUT_PULLUP);
    pinMode(BUTTON_F, INPUT_PULLUP);
    pinMode(BUTTON_G, INPUT_PULLUP);

    Serial.println("ProtoPaw initialized and ready to send commands.");
}

void loop() {
    // Check button presses and send corresponding commands
    if (digitalRead(BUTTON_A) == LOW) {
        sendCommand("A");
        delay(300); // Debounce delay
    } else if (digitalRead(BUTTON_B) == LOW) {
        sendCommand("B");
        delay(300);
    } else if (digitalRead(BUTTON_C) == LOW) {
        sendCommand("C");
        delay(300);
    } else if (digitalRead(BUTTON_D) == LOW) {
        sendCommand("D");
        delay(300);
    } else if (digitalRead(BUTTON_E) == LOW) {
        sendCommand("E");
        delay(300);
    } else if (digitalRead(BUTTON_F) == LOW) {
        sendCommand("F");
        delay(300);
    } else if (digitalRead(BUTTON_G) == LOW) {
        sendCommand("G");
        delay(300);
    }
}
