
// ****************************************************************
// Sketch Esp8266 OnBoardLed Modular(Tab)
// created: Jens Fleischer, 2018-09-10
// last mod: Jens Fleischer, 2018-11-04
// For more information visit: https://fipsok.de
// ****************************************************************
// Hardware: Esp8266
// Software: Esp8266 Arduino Core 2.4.2 / 2.5.0 / 2.5.2 / 2.6.2
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
// Diese Version von OnBoardLed sollte als Tab eingebunden werden.
// #include <ESP8266WebServer.h> muss im Haupttab aufgerufen werden
// Die Funktionalität des ESP8266 Webservers ist erforderlich.
// Die Funktion "onboardLed();" muss im Setup aufgerufen werden.
/**************************************************************************************/

void onboardLed() {
  //pinMode(LED_BUILTIN, OUTPUT);     // Einkommentieren falls du diesen Tab ohne den Connect Tab benutzen möchtest
  server.on("/bled", []() {
    if (server.hasArg("zap")) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));    // LED umschalten
      Serial.println(digitalRead(LED_BUILTIN) ? "LED ist aus" : "LED ist an");
    }
    server.send(200, "text/plain", digitalRead(LED_BUILTIN) ? "Ein" : "Aus");
  });
}
