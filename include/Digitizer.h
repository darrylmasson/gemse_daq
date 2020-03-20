#ifndef _DIGITIZER_H_
#define _DIGITIZER_H_ 1

#include <memory>
#include <csignal>

class Buffer;

class Digitizer : public SourcePlugin {
    public:
        Digitizer(std::weak_ptr<Buffer>);
        ~Digitizer();

        void StartRun();
        void EndRun();
        void Setup();
        int WriteRegister(uint32_t, uint32_t);
        int ReadRegister(uint32_t, uint32_t*);

    private:
        void GetNextEvent();
        int m_iHandle;
        std::vector<uint32_t> m_ReadoutBuffer;
};

#endif // _DIGITIZER_H_ defined
