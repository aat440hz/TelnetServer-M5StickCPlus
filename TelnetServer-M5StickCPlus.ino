#include <WiFi.h>
#include <M5StickCPlus.h>

const char* apSSID = "Telnet_AP";
const char* apPassword = "66666666";
const int apChannel = 1;
const int apMaxConnections = 4; // Maximum number of connected clients

const int port = 23; // Telnet default port

WiFiServer telnetServer(port);

// Store connected clients in an array
WiFiClient clients[apMaxConnections];

void setup() {
  // Initialize Serial for debugging
  M5.begin(115200);

  // Create a SoftAP
  WiFi.softAP(apSSID, apPassword, apChannel, apMaxConnections);
  M5.Lcd.setRotation(1); // Rotate the display 90 degrees
  M5.Lcd.print("SoftAP SSID: ");
  M5.Lcd.println(apSSID);

  // Get the IP address of the SoftAP
  IPAddress apIP = WiFi.softAPIP();
  M5.Lcd.print("AP IP address: ");
  M5.Lcd.println(apIP);

  // Start Telnet server
  telnetServer.begin();
  M5.Lcd.print("Telnet server started on port ");
  M5.Lcd.println(port);
}

void loop() {
  // Check for incoming Telnet client connections
  for (int i = 0; i < apMaxConnections; i++) {
    if (!clients[i] || !clients[i].connected()) {
      clients[i] = telnetServer.available();
      if (clients[i]) {
        M5.Lcd.println("New client connected");
        clients[i].println("Welcome to the Telnet server");
        clients[i].println("Type your message and press Enter to send.");
        clients[i].println();
      }
    }
  }

  // Check for incoming messages from clients
  for (int i = 0; i < apMaxConnections; i++) {
    if (clients[i] && clients[i].connected() && clients[i].available()) {
      char c = clients[i].read();
      Serial.print(c);

      // Echo the character back to the client
      clients[i].write(c);

      // Broadcast the received character to all connected clients
      for (int j = 0; j < apMaxConnections; j++) {
        if (clients[j] && clients[j].connected() && j != i) {
          clients[j].write(c);
        }
      }

      // If the character is a newline, it indicates the end of a message
      if (c == '\n') {
        // You can process the received message here if needed
      }
    }
  }
}
