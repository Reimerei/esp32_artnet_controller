#include <Arduino.h>
#include <FastLED.h>
#include <Artnet.h>
#include <Ethernet3.h>
#include <EthernetUdp3.h>

uint8_t eth_MAC[] = {0x02, 0xF0, 0x0D, 0xBE, 0xEF, 0x01};
IPAddress eth_IP(192, 168, 2, 2);
IPAddress eth_MASK(255, 255, 255, 0);

#define W5500_RESET_PIN 26

#define NUM_LEDS 170
#define NUM_CHANNELS 12

// Taken pins: 23, 19, 18, 26

#define DATA_PIN_0 25
#define DATA_PIN_1 22
#define DATA_PIN_2 26
#define DATA_PIN_3 33
#define DATA_PIN_4 5
#define DATA_PIN_5 32
#define DATA_PIN_6 12
#define DATA_PIN_7 4
#define DATA_PIN_8 15
#define DATA_PIN_9 13
#define DATA_PIN_10 14
#define DATA_PIN_11 27

CRGB leds[NUM_CHANNELS][NUM_LEDS];

int previousDataLength = 0;
unsigned last_frame = 0;
void on_dmx_frame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
  unsigned now = millis();
  // we have 510 channels per universe, which corresponds to the RGB values for 170 leds
  // for simplicity we assume that our led strips have exactly this length, so we can map the universe number to num in the led array

  memcpy(leds[universe], data, 170 * sizeof(CRGB));
  // Serial.print(":memcpy");

  // Serial.print(now - last_frame);
  // trigger the render when the last universe is received. This is not ideal, using ArtSync would be better
  // if (universe == NUM_CHANNELS - 1)
  if (now - last_frame > 30)
  {
    last_frame = now;
    // Serial.println(":");

    FastLED.show();
  }
  // Serial.print(now);
  // Serial.print(":");
  Serial.print(universe);
  // Serial.println();
  // Serial.print(":show");
  Serial.println(":");
}

void on_sync()
{
  // FastLED.show();
  Serial.println("sync");
}

Artnet artnet;
EthernetUDP Udp;

void setup()
{
  Serial.begin(921600);
  while (!Serial)
  {
  }
  Serial.println("\n\tESP32 Artnet Controller\r\n");

  Ethernet.setCsPin(5);
  Ethernet.setRstPin(W5500_RESET_PIN);
  Ethernet.softreset();
  Ethernet.hardreset();
  Ethernet.init(1);

  Serial.print("Starting ETHERNET connection.");
  // Ethernet.begin(eth_MAC, eth_IP, eth_MASK, eth_DNS, eth_GW);
  Ethernet.begin(eth_MAC, eth_IP);
  // Ethernet.begin(eth_MAC);

  while (!Ethernet.link())
  {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Ethernet IP is: ");
  Serial.println(Ethernet.localIP());
  Serial.print("The link speed is: ");
  Serial.println(Ethernet.speedReport());

  FastLED.addLeds<NEOPIXEL, DATA_PIN_0>(leds[0], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds[1], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds[2], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds[3], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_4>(leds[4], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_5>(leds[5], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_6>(leds[6], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_7>(leds[7], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_8>(leds[8], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_9>(leds[9], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_10>(leds[10], NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_11>(leds[11], NUM_LEDS);

  Serial.print("Initializing ArtNet on port: ");
  Serial.println(ARTNET_PORT);
  // delay(1000);
  artnet.begin();
  artnet.setArtDmxCallback(on_dmx_frame);
  artnet.setArtSyncCallback(on_sync);

  Udp.begin(ARTNET_PORT);

  Serial.println("Startup complete");
}

void loop()
{
  uint16_t res = artnet.read();
  if (res != ARTNET_DMX && res != 0)
  {
    Serial.println(res);
  }
}
