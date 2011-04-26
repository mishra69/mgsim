#ifndef IOBUSINTERFACE_H
#define IOBUSINTERFACE_H

#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

class IOResponseMultiplexer;
class IOInterruptMultiplexer;

class IOBusInterface : public IIOBusClient, public Object
{
public:
    struct IORequest
    {
        IODeviceID  device;
        MemAddr     address;    // for reads&writes
        bool        write;
        IOData      data;       // for writes
    };

private:
    IOResponseMultiplexer&  m_rrmux;
    IOInterruptMultiplexer& m_intmux;
    IIOBus&                 m_iobus;
    IODeviceID              m_hostid;

    Buffer<IORequest>       m_outgoing_reqs;

public:
    IOBusInterface(const std::string& name, Object& parent, Clock& clock, IOResponseMultiplexer& rrmux, IOInterruptMultiplexer& intmux, IIOBus& iobus, IODeviceID devid, const Config& config);

    bool SendInterruptAck(IODeviceID to);

    bool SendRequest(const IORequest& request);
    
    Process p_OutgoingRequests;

    Result DoOutgoingRequests();


    /* From IIOBusClientCallBack */
    bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
    bool OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data);
    bool OnReadResponseReceived(IODeviceID from, const IOData& data);
    bool OnInterruptRequestReceived(IOInterruptID which);

    void GetDeviceIdentity(IODeviceIdentification& id) const;
    


    /* for debugging */
    std::string GetIODeviceName() const { return GetFQN(); };

};


#endif