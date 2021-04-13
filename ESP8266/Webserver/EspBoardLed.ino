
// ****************************************************************
// Sketch Esp8266 EspBoardLed Modular(Tab)
// created: Jens Fleischer, 2018-05-16
// last mod: Jens Fleischer, 2020-11-21
// For more information visit: https://fipsok.de
// ****************************************************************
// Hardware: Esp8266
// Software: Esp8266 Arduino Core 2.4.2 / 2.5.2 / 2.6.3 / 2.7.4
// Getestet auf: Nodemcu, Wemos D1 Mini Pro
/******************************************************************
  Copyright (c) 2018 Jens Fleischer. All rights reserved.

  This file is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This file is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*******************************************************************/
// Diese Version von EspBoardLed sollte als Tab eingebunden werden.
// #include <ESP8266WebServer.h> muss im Haupttab aufgerufen werden
// Die Funktionalität des ESP8266 Webservers ist erforderlich.
// Die Funktion "espboardLed();" muss im Setup aufgerufen werden.
/**************************************************************************************/

void espboardLed() {
  pinMode(LED_BUILTIN, OUTPUT);     // OnBoardLed Esp Nodemcu Wemos
  server.on ( "/led", espLed );
}

void espLed() {                    //Html Seite
  if (server.hasArg("led")) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));        // LED umschalten
    Serial.println(digitalRead(LED_BUILTIN) ? "LED ist aus" : "LED ist an");
  }
  const bool pinStatus {digitalRead(LED_BUILTIN)};
  String temp = "<!DOCTYPE html><html lang='de'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  temp += "<style>body{background-color:#87cefa;display:flex;flex-flow:column;align-items:center;}button{height:50px;width:140px;font-size:22px;background-color:";
  temp += pinStatus ? "#adff2f" : "red";
  temp += ";}</style><title>Onboard Led</title></head><body><h2>Onboard Led schalten</h2><h3>Esp8266 Nodemcu Wemos</h3><h4>LED ist ";
  temp += pinStatus ? "aus" : "an";
  temp += "<h4><form action='/led' method='POST'><button name='led'>LED ";
  temp += pinStatus ? "Ein" : "Aus";
  temp += "</button></form></body></html>";
  server.send(200, "text/html", temp);
}
