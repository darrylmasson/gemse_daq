#include "Processor.h"
#include "CircularBuffer.h"

static volatile std::sig_atomic_t gSignalStatus;

Processor::Processor(std::weak_ptr<CircularBuffer> source, std::weak_ptr<CircularBuffer> sink) {
    m_EventSource = source.lock();
    m_EventSink = sink.lock();
    std::signal(SIGINT, OutputHandler::Interrupt);
    std::signal(SIGTERM, OutputHandler::Interrupt);

}

Processor::~Processor() {

}

void Processor::Run() {
    while (gSignalStatus == 0) {
        if (m_EventSource.GetNumEvents() == 0){
            std::this_thread::wait_for(1s);
        } else {
            
        }
    }
}

void Processor::ProcessEvent() {
    Event* event = std::dynamic_cast<Event*>(data);
    event->Process();
    m_EventSource->AddEvent(event);
    return;
}
