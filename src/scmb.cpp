#include "usbPackets.hpp"
#include "bufferedUsb.hpp"
#include <iostream>
#include <iomanip>

UsbChannel *channel;

bool UsbReadWordRequest::actOnPkt()
{
    uint32_t addressData = this->Address.data;
    uint32_t response = addressData * 2;
    std::cout << "Got a request for mem 0x" << std::hex << std::setw(16) << std::setfill('0') << addressData
              << " responded with 0x" << std::hex << std::setw(16) << std::setfill('0') << response << std::dec << std::endl;
    // printf("Got a request for mem %#018x responded with %#010x", this->Address.data, response);
    channel->SendPacket(new UsbReadWordResponse(addressData, response, this->PktCnt.data));
    return true;
}
bool UsbReadWordResponse::actOnPkt()
{
    return false;
}

int main()
{

    channel = new UsbChannel(new BufferedUsb("/dev/ttyACM0"));

    while (1)
    {
        channel->channel->asyncHandler();
        if (channel->PacketReady())
        {
            channel->processNextPacket();
        }
    }

    return 0;
}
