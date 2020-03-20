#ifndef _OUTPUT_HANDLER_H_
#define _OUTPUT_HANDLER_H_ 1

#include <memory>
#include <vector>
#include <csignal>

#include "CircularBuffer.h"

class OutputHandler {
    public:
        OutputHandler(std::weak_ptr<CircularBuffer>);
        ~OutputHandler();

        void Run();

    private:
        void FlushBuffer(bool);

        std::shared_ptr<CircularBuffer> m_EventSource;
        std::vector<Event*> m_Buffer;
        int m_iEventsPerChunk;

        static volatile std::sig_atomic_t gSignalStatus;

        static void Interrupt(int signal) {OutputHandler::gSignalStatus = signal;}
};

#endif // _OUTPUT_HANDLER_H_ defined
