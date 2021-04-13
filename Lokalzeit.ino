
// ****************************************************************
// Sketch Esp8266 NTP Lokalzeit Modular(Tab)
// created: Jens Fleischer, 2020-09-20
// last mod: Jens Fleischer, 2020-12-25
// For more information visit: https://fipsok.de
// ****************************************************************
// Hardware: Esp8266
// Software: Esp8266 Arduino Core 2.6.0 - 2.7.4
// Getestet auf: Nodemcu, Wemos D1 Mini Pro, Sonoff Switch, Sonoff Dual
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
// Diese Version von Lokalzeit sollte als Tab eingebunden werden.
// #include <ESP8266WebServer.h> muss im Haupttab aufgerufen werden.
// Funktion "setupTime();" muss im setup() nach dem Verbindungsaufbau aufgerufen werden.
// Automatische Umstellung zwischen Sommer- und Normalzeit anhand der Zeitzone.
// Inclusive Abfrage ob die Zeit vom NTP Server geholt werden konnte.
/**************************************************************************************/

#include <time.h>
#include <coredecls.h>

struct tm tm;

constexpr uint32_t SYNC_INTERVAL = 12;                           // NTP Sync Interval in Stunden einstellen
bool timeSync;

const char* const PROGMEM NTP_SERVER[] = {"fritz.box", "de.pool.ntp.org", "at.pool.ntp.org", "ch.pool.ntp.org", "ptbtime1.ptb.de", "europe.pool.ntp.org"};
const char* const PROGMEM DAY_NAMES[] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
const char* const PROGMEM DAY_SHORT[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
const char* const PROGMEM MONTH_NAMES[] = {"Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
const char* const PROGMEM MONTH_SHORT[] = {"Jan", "Feb", "Mrz", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"};

void time_is_set() {                                             // Diese Funktion wird als Rückruf festgelegt, wenn Zeitdaten abgerufen wurden.
  timeSync = true;                                               // "timeSync" bleibt ab jetzt auf true um den Programmablauf nicht zu verzögern.
  Serial.println("********* NTP Server Timestap Synchronisation  *********");
}

// Eine schwache Funktion ist bereits definiert und gibt 0 zurück (RFC 4330 Best Practices Verletzung)
uint32_t sntp_startup_delay_MS_rfc_not_less_than_60000 () {      //  Optionale Funktion, SNTP-Startverzögerung ändern
  return random(INT32_MAX) % 2000;
}

uint32_t sntp_update_delay_MS_rfc_not_less_than_15000() {        // Optionale Funktion, für den Individuellen SNTP Update Intervall. Standart ist jede Stunde.
  return SYNC_INTERVAL * 36e5;                                   // SNTP-Aktualisierungsverzögerung ändern.
}

void setupTime() {      // deinen NTP Server einstellen (von 0 - 5 aus obiger Liste) alternativ lassen sich durch Komma getrennt bis zu 3 Server angeben
  configTime("CET-1CEST,M3.5.0,M10.5.0/3", NTP_SERVER[1]);       // Zeitzone einstellen https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
  settimeofday_cb(time_is_set);                                  // Rückruf installieren - wird aufgerufen, wenn Settimeofday aufgerufen wird.
  server.on("/time", []() {
    server.send(200, "application/json", localTime());
  });
}

String localTime() {
  static char buf[26];                                           // je nach Format von "strftime" eventuell anpassen
  static time_t previous;
  time_t now = time(NULL);
  if (now != previous) {
    previous = now;
    if (timeSync) {
      localtime_r(&now, &tm);
      /* Verwendungbeispiele
        Serial.println(DAY_NAMES[tm.tm_wday]);                     // druckt den aktuellen Tag
        Serial.println(MONTH_NAMES[tm.tm_mon]);                    // druckt den aktuellen Monat
        Serial.println(DAY_SHORT[tm.tm_wday]);                     // druckt den aktuellen Tag (Abk.)
        Serial.println(MONTH_SHORT[tm.tm_mon]);                    // druckt den aktuellen Monat (Abk.)
      */
      strftime(buf, sizeof(buf), R"(["%T","%d.%m.%Y"])", &tm);   // http://www.cplusplus.com/reference/ctime/strftime/
      Serial.println(buf);
    }
    else {
      snprintf(buf, sizeof(buf), R"(["Warte","auf Sync"])");     // Optional für die Anzeige auf der Webseite
      Serial.println("Warten auf NTP Synchronisation!");
    }
  }
  return buf;
}
