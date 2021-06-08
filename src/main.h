#ifndef _MAIN_H
#define _MAIN_H

// Global Libs
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <FS.h>
#include <LittleFS.h>
#include <Hash.h>
#include <Ticker.h>
// Local Libs
#include "DNSServer.h"
// Local includes
#include "version.h"
// Sketch definitions
#define SPIFFS LittleFS

#define PIEZO_PIN 4

// Maximum number of simultaneous clients connected (WebSocket)
#define MAX_WS_CLIENT 3

#define CLIENT_NONE 0
#define CLIENT_ACTIVE 1

#define HELP_TEXT "[[b;green;]ESP8266 Mobile Rick Roll]\n"                 \
                  "------------------------\n"                             \
                  "[[b;cyan;]?] or [[b;cyan;]help]    show this help\n\n"  \
                  "[[b;cyan;]debug {0/1}]  show/set debug output\n"        \
                  "[[b;cyan;]silent {0/1}] show/set silent mode\n"         \
                  "[[b;cyan;]ssid 's']     show/set SSID to 's'\n"         \
                  "[[b;cyan;]chan {0-11}]  show/set channel (0=auto)\n"    \
                  "[[b;cyan;]int {n}]      show/set auto scan interval\n"  \
                  "               where 'n' is mins (0=off)\n"             \
                  "[[b;cyan;]msg 's']      show/set message to 's'\n"      \
                  "[[b;cyan;]user 's']     show/set username to 's'\n"     \
                  "[[b;cyan;]pass 's']     show/set password to 's'\n\n"   \
                  "[[b;cyan;]beep {n/rr}]  sound piezo for 'n' ms\n"       \
                  "[[b;cyan;]count]        show Rick Roll count\n"         \
                  "[[b;cyan;]info]         show system information\n"      \
                  "[[b;cyan;]json {e/s/i}] show EEPROM, App Settings,\n"   \
                  "               or System Information\n\n"               \
                  "[[b;cyan;]ls]           list SPIFFS files\n"            \
                  "[[b;cyan;]cat 's']      read SPIFFS file 's'\n"         \
                  "[[b;cyan;]rm 's']       remove SPIFFS file 's'\n\n"     \
                  "[[b;cyan;]scan]         scan WiFi networks in area\n\n" \
                  "[[b;cyan;]reboot]       reboot system\n"                \
                  "[[b;cyan;]reset]        reset default settings\n"       \
                  "[[b;cyan;]save]         save settings to EEPROM"

// Web Socket client state
typedef struct
{
    uint32_t id;
    uint8_t state;
} _ws_client;

enum class statemachine
{
    none,
    beep,
    beep_c,
    beep_rr,
    scan_wifi,
    ap_change,
    read_file
};

static void _u0_putc(char c);
String encryptionTypes(int which);
void dbg_printf(const char *format, ...);
void printfAll(const char *format, ...);
String formatBytes(size_t bytes);
void beep(int delayms);
void beepC(int delayms);
void beep_rr();
int frequency(char note);
void setup(void);
int setupAP(int chan_selected);
void setupEEPROM();
void setupSPIFFS();
void setupDNSServer();
void setupHTTPServer();
void setupOTAServer();
int scanWiFi();
void readFile(String file);
String getSystemInformation();
String getApplicationSettings();
void onTimer();
void eepromLoad();
void eepromSave();
void eepromInitialize();
String getEEPROM();
bool disconnectStationByIP(IPAddress station_ip);
bool disconnectStationByMAC(uint8_t *station_mac);
void loop(void);
void wifi_handle_event_cb(System_Event_t *evt);
void onRequest(AsyncWebServerRequest *request);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void execCommand(AsyncWebSocketClient *client, char *msg);
void client_status(AsyncWebSocketClient *client);

#endif // _MAIN_H