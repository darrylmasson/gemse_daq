#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_ 1

#include <thread>
#include <vector>
#include <memory>
#include <csignal>

class CircularBuffer;

class Processor {
    public:
        Processor(std::weak_ptr<CircularBuffer>, std::weak_ptr<CircularBuffer>);
        ~Processor();
        void Run();
        void SetMaxThreads(int num) {m_iNumThreads = num;}

    private:
        void* ProcessEvent(void*);

        std::shared_ptr<CircularBuffer> m_EventSource;
        std::shared_ptr<CircularBuffer> m_EventSink;
        std::vector<std::unique_ptr<std::thread>> m_vThreads;

        int m_iNumThreads;

        static volatile std::sig_atomic_t gSignalStatus;

        static void Interrupt(int signal) {Processor::gSignalStatus = signal;}
};

#endif // _PROCESSOR_H_ defined
