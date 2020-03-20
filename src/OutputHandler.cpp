#include "OutputHandler.h"
#include <csignal>

OutputHandler::OutputHandler(std::weak_ptr<CircularBuffer> cb) {
    m_EventSource = cb.lock();
    m_Buffer = std::vector<Event*>();
    std::signal(SIGINT, OutputHandler::Interrupt);
    std::signal(SIGTERM, OutputHandler::Interrupt);
    m_iEventsPerChunk = 100;
}

OutputHandler::~OutputHandler() {

}

void OutputHandler::FlushBuffer(bool finalize=false) {
    // empty buffer
    if (finalize) {
        while (m_EventSource.GetNumEvents() > 0) {
            try {
                m_Buffer.push_back(m_EventSource.GetEvent());
            } catch (CircularBuffer::EmptyException) {
                break; // ran out of events too soon?
            }
        }
    }
    // sort by timestamp
    std::sort(m_Buffer.begin(), m_Buffer.end(),
            [&](Event* a, Event* b) {
                return a->timestamp > b->timestamp; // descending
    });
    int iEventsWritten = 0;
    for (auto e = m_Buffer.rbegin(); e < m_Buffer.rend(); e++) {
        // write files to disk
        e->Write();
        iEventsWritten++;
        delete e;
        m_Buffer.pop_back();
        if ((iEventsWritten >= m_iEventsPerChunk) && !finalize) break;
    }
}

void OutputHandler::Run() {
    while (OutputHandler::gSignalStatus == 0) {
        try {
            m_Buffer.push_back(m_EventSource.GetEvent());
        } catch (CircularBuffer::EmptyException) {
            std::this_thread::wait_for(1s);
        }
        if (m_Buffer.size() >= m_iEventsPerChunk*2) {
            FlushBuffer();
        }
    }
    FlushBuffer(true);
}
