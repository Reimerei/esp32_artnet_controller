#include <Arduino.h>
#include <Ethernet3.h>
#include <EthernetUdp3.h>

#define ARTNET_PORT 6454
// Opcodes
#define ARTNET_POLL 0x2000
#define ARTNET_POLL_REPLY 0x2100
#define ARTNET_DMX 0x5000
#define ARTNET_SYNC 0x5200
// Buffers
#define ARTNET_MAX_BUFFER 530
// Packet
#define ARTNET_ID "Art-Net\0"
#define ARTNET_DMX_START 18

struct artnet_reply_s
{
  uint8_t id[8];
  uint16_t opCode;
  uint8_t ip[4];
  uint16_t port;
  uint8_t verH;
  uint8_t ver;
  uint8_t subH;
  uint8_t sub;
  uint8_t oemH;
  uint8_t oem;
  uint8_t ubea;
  uint8_t status;
  uint8_t etsaman[2];
  uint8_t shortname[18];
  uint8_t longname[64];
  uint8_t nodereport[64];
  uint8_t numbportsH;
  uint8_t numbports;
  uint8_t porttypes[4]; //max of 4 ports per node
  uint8_t goodinput[4];
  uint8_t goodoutput[4];
  uint8_t swin[4];
  uint8_t swout[4];
  uint8_t swvideo;
  uint8_t swmacro;
  uint8_t swremote;
  uint8_t sp1;
  uint8_t sp2;
  uint8_t sp3;
  uint8_t style;
  uint8_t mac[6];
  uint8_t bindip[4];
  uint8_t bindindex;
  uint8_t status2;
  uint8_t filler[26];
} __attribute__((packed));

class Artnet
{
public:
  Artnet();

  void begin(byte mac[], byte ip[]);
  void begin();
  uint16_t read();
  void printPacketHeader();
  void printPacketContent();

  // Return a pointer to the start of the DMX data
  inline uint8_t *getDmxFrame(void)
  {
    return artnetPacket + ARTNET_DMX_START;
  }

  inline uint16_t getOpcode(void)
  {
    return opcode;
  }

  inline uint8_t getSequence(void)
  {
    return sequence;
  }

  inline uint16_t getUniverse(void)
  {
    return incomingUniverse;
  }

  inline uint16_t getLength(void)
  {
    return dmxDataLength;
  }

  inline void setArtDmxCallback(void (*fptr)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data))
  {
    artDmxCallback = fptr;
  }

  inline void setArtSyncCallback(void (*fptr)())
  {
    artSyncCallback = fptr;
  }

private:
  uint8_t node_ip_address[4];
  uint8_t id[8];
  EthernetUDP Udp;
  struct artnet_reply_s ArtPollReply;

  uint8_t artnetPacket[ARTNET_MAX_BUFFER];
  uint16_t packetSize;
  IPAddress broadcast;
  uint16_t opcode;
  uint8_t sequence;
  uint16_t incomingUniverse;
  uint16_t dmxDataLength;
  void (*artDmxCallback)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data);
  void (*artSyncCallback)();
};