#include "usbPackets.hpp"
#include "bufferedUsb.hpp"
#include <iostream>
#include <iomanip>
#include "globalMem.hpp"

UsbChannel *channel;
Memory *mem;

bool UsbWriteWordRequest::actOnPkt()
{
    uint32_t addressData = this->Address.data;
    uint32_t Data = this->Value.data;
    mem->writeMemory(addressData, Data);
    std::cout << "Request #" << std::hex << std::setw(4) << std::setfill('0') << this->PktCnt.data
              << " request: write  data to mem 0x" << std::hex << std::setw(8) << std::setfill('0') << addressData
              << " Writing data with 0x" << std::hex << std::setw(8) << std::setfill('0') << Data << std::dec << std::endl;
    // printf("Got a request for mem %#018x responded with %#010x", this->Address.data, response);
    UsbPacket *pkt = new UsbWriteWordResponse(addressData, this->PktCnt.data);
    channel->SendPacket(pkt);
    delete pkt;
    return true;
}

bool UsbWriteWordResponse::actOnPkt()
{
    return false;
}

bool UsbReadWordRequest::actOnPkt()
{
    uint32_t addressData = this->Address.data;
    uint32_t response = mem->readMemory(addressData);
    std::cout << "Request #" << std::hex << std::setw(4) << std::setfill('0') << this->PktCnt.data
              << " request: read data from mem 0x" << std::hex << std::setw(8) << std::setfill('0') << addressData
              << " responded with 0x" << std::hex << std::setw(8) << std::setfill('0') << response << std::dec << std::endl;
    // printf("Got a request for mem %#018x responded with %#010x", this->Address.data, response);
    UsbPacket *pkt = new UsbReadWordResponse(addressData, response, this->PktCnt.data);
    channel->SendPacket(pkt);
    delete pkt;
    return true;
}
bool UsbReadWordResponse::actOnPkt()
{
    return false;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " [interface name] [bin file path]" << std::endl;
        return 1;
    }

    channel = new UsbChannel(new BufferedUsb(argv[1]));
    mem = new Memory();
    mem->loadBinFile(argv[2]);

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
