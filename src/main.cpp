#include <Arduino.h>
#include <FastLED.h>
#include <Artnet.h>
#include <Ethernet3.h>
#include <EthernetUdp3.h>

// W5500 cables: GND (white), 3.3V (black), 23-MOSI (yellow), 21-RST (green), 19-MISO (blue), 18-SCLK (violet), 5-SCS (grey)
uint8_t eth_MAC[] = {0x02, 0xF0, 0x0D, 0xBE, 0xEF, 0x01};
IPAddress eth_IP(192, 168, 2, 2);
IPAddress eth_MASK(255, 255, 255, 0);

Artnet artnet;
EthernetUDP Udp;

#define W5500_RESET_PIN 21

#define PIXEL_PER_UNIVERSE 170
#define PIXEL_PER_STRIP 340
#define NUM_STRIPS 10

#define DATA_PIN_0 4
#define DATA_PIN_1 22
#define DATA_PIN_2 14
#define DATA_PIN_3 33
#define DATA_PIN_4 27
#define DATA_PIN_5 32
#define DATA_PIN_6 12
#define DATA_PIN_7 25
#define DATA_PIN_8 15
#define DATA_PIN_9 13
// #define DATA_PIN_10 26

CRGB leds[NUM_STRIPS][2 * PIXEL_PER_UNIVERSE];

void on_dmx_frame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
  // this assumes 2 universes per strip (340 total). This part needs to be adapted to the hardware strip layout and artnet
  if (universe < NUM_STRIPS * 2)
  {
    if (universe % 2 == 0)
    {
      memcpy(leds[universe / 2], data, PIXEL_PER_UNIVERSE * sizeof(CRGB));
    }
    else
    {
      memcpy(leds[universe / 2] + PIXEL_PER_UNIVERSE, data, PIXEL_PER_UNIVERSE * sizeof(CRGB));
    }
  }
}

void on_sync()
{
  FastLED.show();
}

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
  Ethernet.begin(eth_MAC, eth_IP);

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

  FastLED.addLeds<NEOPIXEL, DATA_PIN_0>(leds[0], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds[1], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds[2], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds[3], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_4>(leds[4], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_5>(leds[5], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_6>(leds[6], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_7>(leds[7], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_8>(leds[8], PIXEL_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_9>(leds[9], PIXEL_PER_STRIP);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN_10>(leds[10], PIXEL_PER_STRIP);

  Serial.print("Initializing ArtNet on port: ");
  Serial.println(ARTNET_PORT);
  artnet.begin();
  artnet.setArtDmxCallback(on_dmx_frame);
  artnet.setArtSyncCallback(on_sync);

  Udp.begin(ARTNET_PORT);

  Serial.println("Startup complete");
}

void loop()
{
  artnet.read();
}
