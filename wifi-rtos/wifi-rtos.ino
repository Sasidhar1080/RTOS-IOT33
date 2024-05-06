#include <Arduino.h>
#include <WiFiNINA.h>
#include <FreeRTOS_SAMD21.h>

const char* ssid = "TP-Link_1EF4"; // Change to your network SSID
const char* password = "!th3sw9sT^uZ0"; // Change to your network password

int status = WL_IDLE_STATUS;

// Function prototype for the FreeRTOS task
void vWiFiConnectTask(void *pvParameters);

// Function to print connection details
void printConnectionDetails() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC Address: ");
  for (int i = 5; i >= 0; i--) {
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Create the FreeRTOS task for Wi-Fi connection
  xTaskCreate(vWiFiConnectTask, "WiFiConnect", 2048, NULL, 1, NULL);

  // Start the FreeRTOS scheduler
  vTaskStartScheduler();
}

void loop() {
  // With FreeRTOS, the scheduler manages task execution, loop remains empty
}

// FreeRTOS task for Wi-Fi connection and printing details
void vWiFiConnectTask(void *pvParameters) {
  // Ensure Wi-Fi shield is present
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Wi-Fi shield not present");
    vTaskSuspend(NULL); // Suspend this task if Wi-Fi shield is missing
  }

  // Attempt to connect to Wi-Fi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);

    // Delay for 5 seconds before retrying connection
    vTaskDelay(pdMS_TO_TICKS(5000));
  }

  // Connection successful
  Serial.println("Connected to Wi-Fi");
  printConnectionDetails(); // Print connection details

  // End the task
  vTaskDelete(NULL);
}
