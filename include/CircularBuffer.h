#ifndef _CIRC_BUFFER_H_
#define _CIRC_BUFFER_H_ 1

#include <deque>
#include <exception>
#include <mutex>
#include <atomic>
#include <condition_variable>

class Event;

class CircularBuffer {
    // threadsafe deque to hold events between stages
    public:
        CircularBuffer();
        ~CircularBuffer();

        void AddEvent(Event*);
        Event* GetEvent(Event*);
        int GetNumEvents(void);

        class EmptyException : public std::exception {
            const char* what() const {return "Buffer empty";}
        };
        std::condition_variable m_CV;

    private:
        std::deque<Event*> m_Buffer;
        std::atomic<int> m_BufferLength;
        std::atomic<long> m_BufferSize;
        std::mutex m_Mutex;
};

#endif // _CIRC_BUFFER_H_ defined
