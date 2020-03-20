#ifndef _EVENT_H_
#define _EVENT_H_ 1

#include <vector>
#include <atomic>

class Peak {
    public:
        Peak();
        ~Peak();

        int64_t timestamp;

        double peak_height;
        double integral;

        double risetime;

};

class Event {
    // docstring
    public:
        Event(const char*, int, int64_t);
        ~Event();

        void Process();
        static void Reset() {s_eventid = 0;}

        int WriteToDisk(std::ofstream& fout);

        int64_t timestamp;
        int runid;
        int eventid;

        double baseline_pre;
        double baseline_pre_rms;
        double baseline_post;
        double baseline_post_rms;
        double peak_height;
        double peak_integral;
        double energy;

        bool is_saturated;

        std::vector<uint16_t> m_WF;
        std::vector<double> m_Derivative;
        std::vector<Peak> m_vPeaks;

    private:
        static atomic<int> s_eventid;

        // maybe use dlib for fitting templates?
};

#endif // _EVENT_H_ defined
