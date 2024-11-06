#include <WiFi.h>

const char ssid[] = "sandbox370";        // change your network SSID (name)
const char pass[] = "+s0a+s03!2gether?";  // change your network password (use for WPA, or use as key for WEP)

WiFiServer server(80);

// Button pins
const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 4;
const int button4Pin = 5;

void setup() {
  Serial.begin(115200);
  // Commented out to prevent waiting for Serial - needed to do this because this While function made it so that the box never connected to Wifi when it was unplugged from the computer
  // while (!Serial) { ;; }
  WiFi.mode(WIFI_STA);
  Serial.print("Attempting to connect to SSID: ");
  WiFi.begin(ssid, pass);

  // Set up buttons
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);

  // attempt to connect to WiFi network with a timeout
  unsigned long startAttemptTime = millis();
  const unsigned long connectionTimeout = 10000; // 10 seconds
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < connectionTimeout) {
    delay(100);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(ssid);
    server.begin();
    printWifiStatus();
  } else {
    Serial.println("Failed to connect to WiFi.");
  }
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // read the HTTP request header line by line
    while (client.connected()) {
      if (client.available()) {
        String header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (header.equals("\r"))  // the end of HTTP request
          break;
        //Serial.println(header);  // print HTTP request to Serial Monitor - NOTE: Removed this because it was flooding the serial port with unnecessary information
      }
    }

    // Create button variables
    int valB1 = getButton1();
    int valB2 = getButton2();
    int valB3 = getButton3();
    int valB4 = getButton4();

    // send the HTTP response
    // send the HTTP response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();                     // the separator between HTTP header and body
    
    // send the HTML response body
    client.println("<!DOCTYPE html>");
    client.println("<html lang=\"en\">");
    client.println("<head>");
    client.println("<title>ESP32 Button Server</title>");
    client.println("<meta http-equiv=\"refresh\" content=\"0.1\">");
    
    // All of the below is CSS that helps create the diamond pattern of the graphical circles. I am not good with CSS so I used chatGPT's help to set this up, admittedly!
    client.println("<style>");
    client.println("body {");
    client.println("    font-size: 24px;");
    client.println("    margin: 20px;");
    client.println("}");
    client.println(".text-container {");
    client.println("    margin-bottom: 40px;");
    client.println("}");
    client.println(".circle-container {");
    client.println("    position: relative;");
    client.println("    width: 200px;");
    client.println("    height: 200px;");
    client.println("    margin: 50px auto;");
    client.println("}");
    client.println(".circle {");
    client.println("    width: 50px;");
    client.println("    height: 50px;");
    client.println("    border-radius: 50%;");
    client.println("    border: 2px solid #ccc;");
    client.println("    position: absolute;");
    client.println("    display: inline-block;");
    client.println("}");
    client.println(".circle:nth-child(1) {");
    client.println("    top: 0;");
    client.println("    left: 50%;");
    client.println("    transform: translateX(-50%);");
    client.println("}");
    client.println(".circle:nth-child(2) {");
    client.println("    top: 50%;");
    client.println("    right: 0;");
    client.println("    transform: translateY(-50%);");
    client.println("}");
    client.println(".circle:nth-child(3) {");
    client.println("    bottom: 0;");
    client.println("    left: 50%;");
    client.println("    transform: translateX(-50%);");
    client.println("}");
    client.println(".circle:nth-child(4) {");
    client.println("    top: 50%;");
    client.println("    left: 0;");
    client.println("    transform: translateY(-50%);");
    client.println("}");
    client.println("</style>");
    client.println("</head>");
    
        // Output the text, with colored words that match with the appropriate buttons
    client.println("<body>");
    client.println("<div class=\"text-container\">");
    client.printf("<p>The <span style=\"color:blue;font-weight:bold\">first button</span> is reading: %d</p>", valB1);
    client.printf("<p>The <span style=\"color:green;font-weight:bold\">second button</span> is reading: %d</p>", valB2);
    client.printf("<p>The <span style=\"color:black;font-weight:bold\">third button</span> is reading: %d</p>", valB3);
    client.printf("<p>The <span style=\"color:#FFBF00;font-weight:bold\">fourth button</span> is reading: %d</p>", valB4);
    client.println("</div>");
    
    // Create the colored circles, turning them "on" if the button is pressed
    client.println("<div class=\"circle-container\">");
    client.printf("<span class=\"circle\" style=\"background-color:%s;\"></span>", valB1 ? "blue" : "transparent");
    client.printf("<span class=\"circle\" style=\"background-color:%s;\"></span>", valB2 ? "green" : "transparent");
    client.printf("<span class=\"circle\" style=\"background-color:%s;\"></span>", valB3 ? "black" : "transparent");
    client.printf("<span class=\"circle\" style=\"background-color:%s;\"></span>", valB4 ? "#FFBF00" : "transparent");
    client.println("</div>");
    
    client.println("</body>");
    client.println("</html>");

    client.flush();
    delay(10); // Give the web browser time to receive the data

    // close the connection:
    client.stop();
  }
}

void printWifiStatus() {
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

int getButton1() {
  int button1Val = digitalRead(button1Pin);
  return button1Val; 
}

int getButton2() {
  int button2Val = digitalRead(button2Pin);
  return button2Val; 
}

int getButton3() {
  int button3Val = digitalRead(button3Pin);
  return button3Val; 
}

int getButton4() {
  int button4Val = digitalRead(button4Pin);
  return button4Val; 
}