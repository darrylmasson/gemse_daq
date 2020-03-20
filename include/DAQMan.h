#ifndef _DAQMAN_H_
#define _DAQMAN_H_ 1

#include <vector>
#include <memory>

class CircularBuffer;
class OutputHandler;
class Digitizer;

class DAQMan {
    // DAQ manager
    public:
        DAQMan();
        ~DAQMan();

    private:
        std::vector<std::shared_ptr<CircularBuffer>> m_vCircBuffers;
        std::shared_ptr<OutputHandler> m_OutputHandler;
        std::vector<std::thread> m_vThreads;
        std::unique_ptr<Digitizer> m_Digitizer;


        static volatile std::sig_atomic_t gSignalStatus;

        static void Interrupt(int signal) {OutputHandler::gSignalStatus = signal;}
};

#endif // _DAQMAN_H_ defined
