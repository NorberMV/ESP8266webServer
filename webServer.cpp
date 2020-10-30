

#include <ESP8266WiFi.h>

const char* ssid = "SSID";
const char* password = "PASS";

WiFiServer server(80);


void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}


// prepare a web page to be send to a client (web browser)
String createHtmlPage(){
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 1\r\n" +  // refresh the page automatically every 1 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>"+"<body>"
            "<header>"
             "<nav>"
        "<ul style = 'list-style:none; position: relative; display: inline-flex;'>"
          "<li><a style='margin: 2px; padding: 5px 10px;  text-decoration: none;  color: rgb(101, 192, 101); cursor: pointer;' href='www.google.com'>Home</a></li>"
          "<li><a style='margin: 2px; padding: 5px 10px;  text-decoration: none;  color: rgb(101, 192, 101); cursor: pointer;' href='#Dinamarca'>Projects</a></li>"
          "<li><a style='margin: 2px; padding: 5px 10px;  text-decoration: none;  color: rgb(101, 192, 101); cursor: pointer;' href='proyectos.html'>IoT</a></li>"
        "</ul>"
           "</header>" +
            "<center>"
            "<h1 style ='color:green'>Analog input:  " + String(analogRead(A0)) +"C"
            "</h1>"
            "</center></html>" +
            "\r\n";
  return htmlPage;
}


void loop()
{
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected()) {
      // read line by line what the client (web browser) is requesting
      if (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n') {
          client.println(createHtmlPage());
          break;
        }
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }
}
