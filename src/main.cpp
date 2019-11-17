#include <Arduino.h>
#include <FastLED.h>
#include <Artnet.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

uint8_t eth_MAC[] = {0x02, 0xF0, 0x0D, 0xBE, 0xEF, 0x01};
// IPAddress eth_IP(192, 168, 42, 121);
// IPAddress eth_MASK(255, 255, 255, 0);
// IPAddress eth_DNS(192, 168, 42, 1);
// IPAddress eth_GW(192, 168, 42, 1);

#define RESET_P 26

const int MAX_BUFFER_UDP = 1500;
char packetBuffer[MAX_BUFFER_UDP]; //buffer to store incoming data

// art net parameters
const int art_net_header_size = 17;
const int max_packet_size = 576;
char ArtNetHead[8] = "Art-Net";
char OpHbyteReceive = 0;
char OpLbyteReceive = 0;

void reset_board()
{
  Serial.print("Resetting Wiz W5500 Ethernet Board...  ");
  pinMode(RESET_P, OUTPUT);
  digitalWrite(RESET_P, HIGH);
  delay(250);
  digitalWrite(RESET_P, LOW);
  delay(50);
  digitalWrite(RESET_P, HIGH);
  delay(350);
  Serial.println("Done.");
}

void print_hardware_status(uint8_t refval)
{
  switch (refval)
  {
  case 0:
    Serial.println("No hardware detected.");
    break;
  case 1:
    Serial.println("WizNet W5100 detected.");
    break;
  case 2:
    Serial.println("WizNet W5200 detected.");
    break;
  case 3:
    Serial.println("WizNet W5500 detected.");
    break;
  default:
    Serial.println("UNKNOWN - Update espnow_gw.ino to match Ethernet.h");
  }
}

void print_link_status(uint8_t refval)
{
  switch (refval)
  {
  case 0:
    Serial.println("Uknown status.");
    break;
  case 1:
    Serial.println("Link flagged as UP.");
    break;
  case 2:
    Serial.println("Link flagged as DOWN. Check cable connection.");
    break;
  default:
    Serial.println("UNKNOWN - Update espnow_gw.ino to match Ethernet.h");
  }
}

EthernetUDP Udp;
Artnet artnet;

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\tESP32 Artnet Controller\r\n");

  // Use Ethernet.init(pin) to configure the CS pin.
  Ethernet.init(5); // GPIO5 on the ESP32.
  reset_board();

  Serial.println("Starting ETHERNET connection...");
  // Ethernet.begin(eth_MAC, eth_IP, eth_DNS, eth_GW, eth_MASK);
  Ethernet.begin(eth_MAC);

  delay(200);

  Serial.print("Ethernet IP is: ");
  Serial.println(Ethernet.localIP());
  Serial.print("The UDP port is: ");
  Serial.println(ART_NET_PORT);
  Serial.print("Checking connection.");

  bool rdy_flag = false;
  for (uint8_t i = 0; i <= 20; i++)
  {
    if ((Ethernet.hardwareStatus() == EthernetNoHardware) || (Ethernet.linkStatus() == LinkOFF))
    {
      Serial.print(".");
      rdy_flag = false;
      delay(80);
    }
    else
    {
      rdy_flag = true;
      break;
    }
  }
  if (rdy_flag == false)
  {
    Serial.println("\n\r\tHardware fault, or cable problem... cannot continue.");
    Serial.print("Hardware Status: ");
    print_hardware_status(Ethernet.hardwareStatus());
    Serial.print("   Cable Status: ");
    print_link_status(Ethernet.linkStatus());
    while (true)
    {
      delay(10); // Halt.
    }
  }
  else
  {
    Serial.println("OK");
  }

  // artnet.begin();
}

void loop()
{
  // uint16_t r = artnet.read();
  // if (r == ART_POLL)
  // {
  //   Serial.println("POLL");
  // }
  // if (r == ART_DMX)
  // {
  //   // print out our data
  //   Serial.print("universe number = ");
  //   Serial.print(artnet.getUniverse());
  //   Serial.print("\tdata length = ");
  //   Serial.print(artnet.getLength());
  //   Serial.print("\tsequence n0. = ");
  //   Serial.println(artnet.getSequence());
  //   Serial.print("DMX data: ");
  //   for (int i = 0; i < artnet.getLength(); i++)
  //   {
  //     Serial.print(artnet.getDmxFrame()[i]);
  //     Serial.print("  ");
  //   }
  //   Serial.println();
  //   Serial.println();
  // }
}