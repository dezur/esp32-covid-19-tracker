#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <SPI.h>

const char* ssid     = "***";
const char* password = "***";

const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDBTCCAe2gAwIBAgIUdllJkOs+4dCD6H7be1v6SGI9cVIwDQYJKoZIhvcNAQEL\n" \
"BQAwEjEQMA4GA1UEAwwHemVpdC5jbzAeFw0xOTEwMDkxODU0NDBaFw0yMTEwMDgx\n" \
"ODU0NDBaMBIxEDAOBgNVBAMMB3plaXQuY28wggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
"DwAwggEKAoIBAQDKsvPg6Y2SgizgP4Y8Vcxg0ezjXDSypt4PvGXFUkIleA6ctLpe\n" \
"7E83ERruq3KLD/mUOpzkW/wdustBQk8eij5OHU/PFDHUJ49Jj8R9h/f/+om6qfp7\n" \
"FOnYJUnLgFEbYTafcMFoPNvfpCmGWrP4nDrsLi3ECciUjR969p5kD7uCeHRfoDFp\n" \
"7UiV4KrKpPt5nncB7s9DOxYXaioHPaFGKOuH4V2notQ74flECUJVrBWVV0VLWYU0\n" \
"xSbJmLiLzEuZjljSZVaBhBAozLhFWJu0RjkhljKDhWXhnq/cr/GYyZcJZB+Oaxms\n" \
"IJtDv/8R8prYrmDCjJ9mGszKFQt6sAgYdq2RAgMBAAGjUzBRMB0GA1UdDgQWBBQd\n" \ 
"SU37TYLdjeq3vMrxTQPve+vKpzAfBgNVHSMEGDAWgBQdSU37TYLdjeq3vMrxTQPv\n" \
"e+vKpzAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQB8/aLnkdXK\n" \
"zjMZrFPYkbO276wi64bL91LWVoeoLQ65oQ8t9ZifBR6xXkj6KhTgU2wvPSkL5NAq\n" \
"HU4y0gDo6PL+UYBvLIIee843Qnb182CEAJPGrx0Gwhy86o9F7/I8FE/TuKQAP6iS\n" \
"IoEdvyTlZhJ74Sb64A0WbTsxQsDjSIkrFrhH7n5ZuvYOWCUoPrTYuukkOU+VzV8K\n" \
"bGEKcHW+PDc3iBL0ed1vL/qkhiJjq9Vj/GfbCPQ0J3BEBjoUccE7EgriBCsxE6zb\n" \
"Kyu5jEfYt9on+wcx9r6MPEp7NnkcOqkTvueYhaY5CPxkblF16WjDpleTifV4JfKH\n" \
"jHVwzDAbimw0\n" \
"-----END CERTIFICATE-----\n";

WiFiClient client;
HTTPClient http;
DynamicJsonDocument covid(2000);
TFT_eSPI tft = TFT_eSPI();

void displayData(int, int, int);

void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
  if (!client.connect("covid19.mathdro.id", 80)) {
    tft.println("Connection failed");
    return;
  }
}

void loop() {
  http.begin("https://covid19.mathdro.id/api", root_ca);
  int httpCode = http.GET();
  if (httpCode == 200){
    String payload = http.getString();
    http.end();
    deserializeJson(covid, payload);
    int confirmed_value = covid["confirmed"]["value"];
    int recovered_value = covid["recovered"]["value"];
    int deaths_value = covid["deaths"]["value"];
    displayData(confirmed_value, recovered_value, deaths_value);
  }
  delay(60 * 60 * 1000);
}

void displayData(int confirmed, int recovered, int deaths){
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("COVID-19 Global Statistics: ");
  tft.println("Confirmed: " + (String)confirmed);
  tft.println("Recovered: " + (String)recovered);
  tft.println("Deaths: " + (String)deaths);
}