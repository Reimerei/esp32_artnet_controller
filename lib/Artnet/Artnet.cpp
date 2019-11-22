#include <Artnet.h>

Artnet::Artnet() {}

void Artnet::begin()
{
  Udp.begin(ARTNET_PORT);
}

uint16_t Artnet::read()
{
  packetSize = Udp.parsePacket();

  if (packetSize <= ARTNET_MAX_BUFFER && packetSize > 0)
  {
    Udp.read(artnetPacket, ARTNET_MAX_BUFFER);

    // This could be skipped to improve performance
    for (byte i = 0; i < 8; i++)
    {
      if (artnetPacket[i] != ARTNET_ID[i])
        return 0;
    }

    opcode = artnetPacket[8] | artnetPacket[9] << 8;

    if (opcode == ARTNET_DMX)
    {
      sequence = artnetPacket[12];
      incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;
      dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;

      if (artDmxCallback)
        (*artDmxCallback)(incomingUniverse, dmxDataLength, sequence, artnetPacket + ARTNET_DMX_START);
      return ARTNET_DMX;
    }
    if (opcode == ARTNET_SYNC)
    {
      if (artSyncCallback)
        (*artSyncCallback)();
      return ARTNET_SYNC;
    }
  }

  return 0;
}

void Artnet::printPacketHeader()
{
  Serial.print("packet size = ");
  Serial.print(packetSize);
  Serial.print("\topcode = ");
  Serial.print(opcode, HEX);
  Serial.print("\tuniverse number = ");
  Serial.print(incomingUniverse);
  Serial.print("\tdata length = ");
  Serial.print(dmxDataLength);
  Serial.print("\tsequence n0. = ");
  Serial.println(sequence);
}

void Artnet::printPacketContent()
{
  for (uint16_t i = ARTNET_DMX_START; i < dmxDataLength; i++)
  {
    Serial.print(artnetPacket[i], DEC);
    Serial.print("  ");
  }
  Serial.println('\n');
}
