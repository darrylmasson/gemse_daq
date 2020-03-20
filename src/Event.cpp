#include "Event.h"
#include <math>

int Event::s_eventid = 0;

Event::Event(const char* buffer, int run_id, int64_t ts) {
    timestamp = ts
    runid = run_id;
    uint16_t offset = 0;
    uint32_t event_size = *(buffer+offset) & 0xfffffff;
    eventid = Event::s_eventid++;
    uint16_t* wf_start = reinterpret_cast<uint16_t*>(buffer + 4*sizeof(WORD));
    m_WF.reserve((event_size-4)/2);
    for (int i = 0; i < (event_size-4)/2; i++) m_WF.push_back(wf_start[i]);
    m_Derivative.assign(wf.size(), 0);
}

Event::~Event() {

}

void Event::Process() {
    int bl_samples = 64;
    int wf_len = wf.size()-1; // last accessible value
    int16_t max_v = 0;
    for (int i = 0; i <= wf_len; i++) {
        if (i < bl_samples) {
            baseline_pre += wf[i];
            baseline_pre_rms += wf[i]*wf[i];
            baseline_post += wf[wf_len-i];
            baseline_post_rms += wf[wf_len-i]*wf[wf_len-i];
        }
        if (wf[i] > max_v) max_v = wf[i];
        peak_integral += wf[i];
        if ((0 < i) && (i < wf_len)) {
            m_Derivative[i] = (m_WF[i+1]-m_WF[i-1])/2;
        }
    }
    baseline_pre /= wf_len;
    baseline_pre_rms = sqrt(baseline_pre_rms/wf_len - baseline_pre*baseline_pre);
    baseline_post /= wf_len;
    baseline_post_rms = sqrt(baseline_post_rms/wf_len - baseline_post*baseline_post);

    is_saturated = max_v == (1 << 14);
    peak_height = max_v - baseline_pre;
    peak_integral -= baseline_pre*wf_len;
}

int Event::EncodeForWriting() {
    std::string buffer;

    buffer.append(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
    buffer.append(reinterpret_cast<char*>(&runid), sizeof(runid));
    buffer.append(reinterpret_cast<char*>(&eventid), sizeof(eventid));
    buffer.append(reinterpret_cast<char*>());

}
