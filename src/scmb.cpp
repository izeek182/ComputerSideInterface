#include "usbPackets.hpp"
#include "bufferedUsb.hpp"

UsbChannel *channel;

bool UsbReadWordRequest::actOnPkt()
{
    channel->SendPacket(new UsbReadWordResponse(this->Address.data, this->Address.data * 2, this->PktCnt.data));
    return true;
}
bool UsbReadWordResponse::actOnPkt()
{
    return false;
}

int main()
{

    channel = new UsbChannel(new BufferedUsb("ttyS3"));

    while (1)
    {
        if (channel->PacketReady())
        {
            channel->processNextPacket();
        }
    }

    return 0;
}
