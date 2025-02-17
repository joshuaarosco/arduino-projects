#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

ESP8266WebServer server(80);
DNSServer dnsServer;

const char* ssid = ""; // Enter your default SSID here
const char* password = ""; // Enter your default password here

String htmlContent;

void handleRoot() {
  String html ="<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>JANIBOT CONTROLLER</title> <style> *{ font-family: \"Helvetica Neue\",Helvetica,Arial,sans-serif; -webkit-box-sizing: border-box; -moz-box-sizing: border-box; box-sizing: border-box; overflow: hidden; /* Hide scrollbars */ } body{ margin: 0px; } .text-center { text-align: center; } .btn{ border: 1px solid #555555; font-size: 20px; } .white{ color: #ffffff!important; } .btn-control{ width: 75px; height: 75px; border-radius: 50%; background-color: #ffffff; color: #555555; } .btn-control-sm{ width: 55px; height: 55px; border-radius: 50%; background-color: #ffffff; color: #555555; } .mr-20{ margin-right: 20px; } .mr-30{ margin-right: 30px; } .mt-25{ margin-top: 40px; } .mt-30{ margin-top: 30px; } .mt-100{ margin-top: 100px; } .mt-50{ margin-top: 50px; } .h-100{ height: 100%; } .btn-auto{ height: 75px; width: 150px; border-radius: 50px; font-size: 25px; background-color: #ffffff; color: #555555; } .p-10{ padding-bottom: 20px; } .pb-25{ padding: 25px; } .ml-25{ margin-left: 25px; } .mr-25{ margin-right: 25px; } .pt-30{ padding-top: 30px; } .power{ color: #ff6e6e!important; } .wifi{ color: #5db4ff!important; } .title{ color: #ffffff; color: #555555; } .col-xs-5{ width: 50%; } .col-xs-6 { width: 50%; position: relative; min-height: 1px; padding-left: 13px; padding-right: 13px; float: left; } .col-xs-4 { width: 33.33333333%; position: relative; min-height: 1px; padding-left: 13px; padding-right: 13px; float: left; } .col-xs-5 { width: 41.66666667%; position: relative; min-height: 1px; padding-left: 13px; padding-right: 13px; float: left; } .col-xs-2 { width: 1 6.66666667%; position: relative; min-height: 1px; padding-left: 13px; padding-right: 13px; float: left; } .col-xs-3 { width: 25%; position: relative; min-height: 1px; padding-left: 13px; padding-right: 13px; float: left; } .col-xs-12 { width: 100%; position: relative; min-height: 1px; padding-left: 13px; padding-right: 13px; float: left; } .row { margin-left: -13px; margin-right: -13px; } .pull-right { float: right !important; } .pt-90 { padding-top: 90px; } .h-50{ height: 50px; } .mb{ margin-bottom: 35px; } .text-left{ text-align: left; } .text-right{ text-align: right; } </style> </head> <body> <!--container--> <div class=\"row p-10\"> <div class=\"col-xs-4 pt-30\"> <div class=\"row pb-25\"> <div class=\"col-xs-6\"> <form method=\"POST\" action=\"K\"> <button name=\"command\" value=\"K\" type=\"submit\" class=\"btn btn-control-sm pull-right\">◀</button> </form> </div> <div class=\"col-xs-6\"> <form method=\"POST\" action=\"E\"> <button name=\"command\" value=\"E\" type=\"submit\" class=\"btn btn-control-sm\">▶</button> </form> </div> </div> <div class=\"row\"> <div class=\"col-xs-6\"> <form method=\"POST\" action=\"L\"> <button name=\"command\" value=\"L\" type=\"submit\" class=\"btn btn-control pull-right\">◀</button> </form> </div> <div class=\"col-xs-6\"> <form method=\"POST\" action=\"R\"> <button name=\"command\" value=\"R\" type=\"submit\" class=\"btn btn-control\">▶</button> </form> </div> </div> </div> <div class=\"col-xs-4 text-center\"> <h1 class=\"title mb\"> <strong>JANIBOT</strong> </h1> <div class=\"row\"> <div class=\"col-xs-5\"> <form method=\"POST\" action=\"O\"> <button name=\"command\" value=\"O\" type=\"submit\" class=\"btn btn-control mr-20\">☉</button> </form> </div> <div class=\"col-xs-7\"> <form method=\"POST\" action=\"A\"> <button name=\"command\" value=\"A\" type=\"submit\" class=\"btn btn-auto mr-20\">AUTO</button> </form> </div> </div> </div> <div class=\"col-xs-4\"> <div class=\"row\"> <div class=\"col-xs-5 pt-90 text-right\"> <form method=\"POST\" action=\"T\"> <button name=\"command\" value=\"T\" type=\"submit\" class=\"btn btn-control\">⟳</button> </form> </div> <div class=\"col-xs-6\"> <div class=\"row pt-30\"> <div class=\"col-xs-12 text-left\"> <form method=\"POST\" action=\"F\"> <button name=\"command\" value=\"F\" type=\"submit\" class=\"btn btn-control mr-25\">▲</button> </form> </div> <div class=\"col-xs-12 text-left\"> <form method=\"POST\" action=\"B\"> <button name=\"command\" value=\"B\" type=\"submit\" class=\"btn btn-control mt-30 mr-25\">▼</button> </form> </div> </div> </div> </div> </div> </div> <div class=\"row\"> <div class=\"col-xs-12 text-center h-50\"></div> </div> </div> </body> </html>";

  server.send(200, "text/html", html);
}

void handleCommandF() {
  // Serial.println("Moving Forward");
  Serial.write("F");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandB() {
  // Serial.println("Moving Backward");
  Serial.write("B");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandK() {
  // Serial.println("Small Left");
  Serial.write("K");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandE() {
  // Serial.println("Small Right");
  Serial.write("E");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandL() {
  // Serial.println("Turning Left");
  Serial.write("L");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandR() {
  // Serial.println("Turning Right");
  Serial.write("R");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandT() {
  // Serial.println("Turning Right 180 Deg");
  Serial.write("T");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandO() {
  // Serial.println("Turning On the Vaccuum Fan");
  Serial.write("O");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleCommandA() {
  // Serial.println("Janibot has been set to Automatic");
  Serial.write("A");
  server.sendHeader("Location","/", true);        
  server.send(303, "text/plain", "");
}

void handleNotFound() {
  // Redirect all requests to the root URL
  server.sendHeader("Location", "/", true);
  server.send(303, "text/plain", "");
}

void setup() {
  Serial.begin(115200);

  // Set the ESP8266 as an access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP("JANIBOT", ""); // Set the AP name and password

  // Start the web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/F", HTTP_POST, handleCommandF);
  server.on("/B", HTTP_POST, handleCommandB);
  server.on("/K", HTTP_POST, handleCommandK);
  server.on("/E", HTTP_POST, handleCommandE);
  server.on("/L", HTTP_POST, handleCommandL);
  server.on("/R", HTTP_POST, handleCommandR);
  server.on("/T", HTTP_POST, handleCommandT);
  server.on("/O", HTTP_POST, handleCommandO);
  server.on("/A", HTTP_POST, handleCommandA);
  server.onNotFound(handleNotFound);
  server.begin();

  // Enable captive portal functionality
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Serial.println();
  // Serial.println("JANIBOT Web Server started");
  // Serial.print("Access Point IP address: ");
  // Serial.println(WiFi.softAPIP());
} 

void loop() {
  // Handle incoming client requests
  server.handleClient();
  dnsServer.processNextRequest();
}