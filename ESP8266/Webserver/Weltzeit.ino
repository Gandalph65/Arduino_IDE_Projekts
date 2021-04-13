
// ****************************************************************
// Sketch Esp8266 Weltzeit Modular(Tab)
// created: Jens Fleischer, 2020-07-09
// last mod: Jens Fleischer, 2020-07-09
// For more information visit: https://fipsok.de
// ****************************************************************
// Hardware: Esp8266
// Software: Esp8266 Arduino Core 2.7.1 / 2.7.2 / 2.7.3 / 2.7.4
// Getestet auf: Nodemcu, Wemos D1 Mini Pro
/******************************************************************
  Copyright (c) 2020 Jens Fleischer. All rights reserved.

  This file is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This file is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*******************************************************************/
// Diese Version von Weltzeit sollte als Tab eingebunden werden.
// #include <ESP8266WebServer.h> muss im Haupttab aufgerufen werden.
// Funktion "setupTime();" muss im setup() nach dem Verbindungsaufbau aufgerufen werden.
/**************************************************************************************/

#include <time.h>
#ifndef ZEITVARS
#define ZEITVARS
const char* const PROGMEM NTP_SERVER[] = {"fritz.box", "de.pool.ntp.org", "at.pool.ntp.org", "ch.pool.ntp.org", "ptbtime1.ptb.de", "europe.pool.ntp.org"};
const char* const PROGMEM DAY_NAMES[] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
const char* const PROGMEM DAY_SHORT[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
const char* const PROGMEM MONTH_NAMES[] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
const char* const PROGMEM MONTH_SHORT[] = {"Jan", "Feb", "Mrz", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"};
#endif

void setupWorldTime() {
  static char zone[2][45] = {"356", "CET-1CEST,M3.5.0,M10.5.0/3"};
  configTime(zone[1], NTP_SERVER[4,5,6]);                          // deinen NTP Server einstellen (von 0 - 5 aus obiger Liste) alternativ lassen sich durch Komma getrennt bis zu 3 Server angeben
  server.on("/posixTZ", HTTP_POST, [&]() {
    if (server.hasArg("zone")) {
      byte i {0};
      char str[55];
      strcpy (str, server.arg("zone").c_str());
      char* ptr = strtok(str, "[\"");
      while (ptr != NULL) {                                    // Json dekodieren
        if (strcmp(ptr, ",")) strcpy(zone[i++], ptr);
        ptr = strtok(NULL, "\"]");
      }
      setTZ(zone[1]);
    }
    server.send(200, "text/plain", zone[0]);
  });
  server.on("/posixTZ", HTTP_GET, []() {
    struct tm ti;                                              // http://www.cplusplus.com/reference/ctime/tm/
    static char buf[26];                                       // je nach Format von "strftime" eventuell anpassen
    time_t now = time(&now);
    localtime_r(&now, &ti);
    strftime (buf, sizeof(buf), R"(["%T","%d.%m.%Y"])", &ti);  // http://www.cplusplus.com/reference/ctime/strftime/
    server.send(200, "application/json", buf);
  });
}
