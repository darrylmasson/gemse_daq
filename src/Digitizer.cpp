#include "Digitizer.h"
#include "CAENVMElib.h"

Digitizer::Digitizer(std::weak_ptr<CircularBuffer> cb) {
    LogMessage(Logger::Trace, "Digitizer c'tor");
    m_EventSink = cb.lock();
    if (int err = CAENComm_OpenDevice(CAENComm_USB, LinkNum, 0, 0, &m_iHandle)) {
        LogMessage(Logger::Error, "Could not open digitizer", err);
        // failure
    }
    std::signal(SIGINT, Digitizer::Interrupt);
    std::signal(SIGTERM, Digitizer::Interrupt);
    LogMessage(Logger::Trace, "Digitizer c'tor done");
}

Digitizer::~Digitizer() {
    LogMessage(Logger::Trace, "Digitizer d'tor");
    if (int err = CAENComm_CloseDevice(m_iHandle)) {
        LogMessage(Logger::Error, "Could not close digitizer", err);
        // failure
    }
    LogMessage(Logger::Trace, "Digitizer d'tor done");
}

void Digitizer::Setup(std::vector<std::pair<uint32_t, uint32_t>>& reg_writes) {
    LogMessage(Logger::Debug, "Setting up digi");
    //m_ReadoutBuffer.resize(1024*1024); // buffer size on digitizer
    for (auto& gw : reg_writes) {
        if (WriteRegister(gw.first, gw.second)) {
            // blah
            break;
        }
    }
    LogMessage(Logger::Debug, "Setting up digi done");
}

void Digitizer::StartRun() {
    LogMessage(Logger::Trace, "Starting run");

    if ((int err = WriteRegister(0x8100, 0x4)) != 0) {
        LogMessage(Logger::Error, "Could not start acquisition", err);
    }
    LogMessage(Logger::Trace, "Starting run done");
}

void Digitizer::EndRun() {
    LogMessage(Logger::Debug, "Ending run");
    if ((int err = WriteRegister(0x8100, 0)) != 0) {
        LogMessage(Logger::Error, "Could not stop acquisition", err);
    }
    LogMessage(Logger::Trace, "Ending run done");
}

type Digitizer::ReadEvent(int offset_words=0) {
    LogMessage(Logger::Trace, "Reading event");
    uint32_t event_size, num_words;
    if (ReadRegister(0x814C, &event_size)) {

    }
    m_ReadoutBuffer.resize(event_size);
    if (int err = CAENComm_BLTRead(m_iHandle, 0x0000+offset_words, event_size, m_ReadoutBuffer.data(), &num_words) != CAENComm_Success) {
        LogMessage(Logger::Error, "Problem reading event", err)
    }

    m_EventSink.AddEvent(new Event(m_ReadoutBuffer.data()));
    return num_words;
}

int Digitizer::WriteRegister(uint32_t addr, uint32_t data) {
    std::stringstream ss;
    ss << "Writing 0x" << std::hex << data << " to 0x" << addr;
    LogMessage(Logger::Trace, ss.str());
    if (int err = CAENComm_Write32(m_iHandle, addr, data)) {
        LogMessage(Logger::Error, "Error writing register", err);
        // failure
    }
}

int Digitizer::ReadRegister(uint32_t addr, uint32_t* data) {
    std::stringstream ss;
    ss << std::hex << "Reading from 0x" << addr;
    LogMessage(Logger::Trace, ss.str());
    if (int err = CAENComm_Read32(m_iHandle, addr, data)) {
        LogMessage(Logger::Error, "Error reading register", err);
        // failure
    }
    ss << ", got 0x" << *data;
    LogMessage(Logger::Trace, ss.str());
}

int Digitizer::GetNumEvents() {
    uint32_t num_events;
    if (ReadRegister(0x812C, &num_events)) {

    }
    return num_events;
}

void LogMessage(int level, std::string message, int err=0) {
    Logger::Log(level, "Digitizer", message);
    if (err != 0) {
        char* errmsg;
        CAENComm_DecodeError(err, errmsg);
        Logger::Log(level, "Digitizer", errmsg);
    }
}

void Digitizer::Run() {
    int iEventsStored = 0;
    while (Digitizer::gSignalStatus == 0) {
        CAENComm_IRQWait(m_iHandle, 1000);
        if ((iEventsStored = GetNumEvents()) != 0) {
            for (int i = 0; i < iEventsStored; i++) ReadEvent();
        }
    }
}
