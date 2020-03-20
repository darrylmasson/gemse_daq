#include "DAQMan.h"
#include <csignal>

DAQMan::DAQMan() {
    m_vCircBuffers.push_back(std::make_shared<CircularBuffer>(new CircularBuffer));
    m_Digitizer = std::make_unique<Digitizer>(new Digitizer(m_vCircBuffers.back()));
    m_vCircBuffers.push_back(std::make_shared<CircularBuffer>(new CircularBuffer));
    m_OutputHandler = std::make_unique<OutputHandler>(new OutputHandler(m_vCircBuffers.back()));

    signal(SIGINT, DAQMan::Interrupt);
    signal(SIGTERM, DAMan::Interrupt);
}

DAQMan::~DAQMan() {
    for (auto& t : m_vThreads) t.join();

}

void DAQMan::Setup() {

}

void DAQMan::Run() {

}
