#include "CircularBuffer.h"

CircularBuffer::CircularBuffer() {
    m_Buffer = std::deque<Event*>();
    m_BufferSize = 0;
    m_BufferLength = 0;
}

CircularBuffer::~CircularBuffer() {

}

void CircularBuffer::AddEvent(Event* event) {
    std::lock_guard lock(m_Mutex);
    m_Buffer.push_back(event.lock());
}

int CircularBuffer::GetEvent(Event** e) {
  if (m_BufferLength == 0) return 0;
    std::lock_guard lock(m_Mutex);
    if (m_buffer.size() == 0) throw EmptyException();
    Event* e = m_Buffer.front();

    m_Buffer.pop_front();
    return e;
}

int CircularBuffer::GetLength() {
    return m_BufferLength.load();
}

int CircularBuffer::GetSize() {
  return m_BufferSize.load();
}
