/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#include "ESP8266WiFi.h"
#include "ethers.h"


const uint8_t pk[32] = {0xAA,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void printHex(uint8_t* s, int length){
      Serial.printf("0x");
      for (int i = 0; i < length; i ++) {
        Serial.printf("%02x", s[i]);
    }
}

void testSig(){
const char* data = "test";
uint8_t digest[32];
uint8_t signature[65];
uint8_t address[80];

memset((char*)digest, 0, 32);
memset((char*)address, 0, 80);
memset((char*)signature, 0, 65);


ethers_keccak256((uint8_t*)data, 4, digest);

for(int i =0; i<32; i++){
  printf("%d: %d\n",i, digest[i]);
}
Serial.printf("\n");
Serial.printf("Digest: ");
delay(100);
printHex(digest, 32);
delay(100);
ethers_sign(pk, digest,signature);
Serial.printf("\n -.-- \n");
delay(100);
Serial.printf("Signature: ");
delay (100);
printHex(signature, 65);
delay(100);


ethers_privateKeyToAddress(pk,address);
Serial.printf("\nAddress: ");
delay(100);
printHex(address, 20);
delay(100);

}

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

Serial.println("doing some stuff..");

testSig();

Serial.println("\nSetup done");
}

void loop()
{

  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);

  // wait for a second
  delay(500);

  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);

   // wait for a second
  delay(200);

  return;


  printf("hello\n");

    Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}
