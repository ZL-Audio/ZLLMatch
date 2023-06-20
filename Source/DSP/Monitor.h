#ifndef ZLLMATCH_MONITOR_H
#define ZLLMATCH_MONITOR_H

#include "Tracker//Tracker.h"
#include "Tracker/LUFSTracker.h"
#include "Tracker/RMSTracker.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

template<typename FloatType>
class Monitor {
public:
    Monitor() = default;

    void prepare(const juce::dsp::ProcessSpec &spec) {
        for (auto &t: trackers) {
            t->prepare(spec);
        }
    }

    void reset() {
        peak = 0;
        bPeak = 0;
        for (auto &t: trackers) {
            t->reset();
        }
    }

    void process(const juce::AudioBuffer<float> &buffer, FloatType gate) {
        bPeak = buffer.getMagnitude(0, buffer.getNumSamples());
        peak = juce::jmax(bPeak, peak);
        for (auto &t: trackers) {
            t->process(buffer, juce::Decibels::decibelsToGain(gate));
        }
    }

    std::vector<FloatType> getLoudness() {

        std::vector<FloatType> loudness;
        for (auto &t: trackers) {
            loudness.push_back(t->getIntegratedLoudness());
        }
        return loudness;
    }

    FloatType getBufferPeak() { return juce::Decibels::gainToDecibels(bPeak); }

    FloatType getPeak() { return juce::Decibels::gainToDecibels(peak); }

private:
    FloatType peak = 0, bPeak = 0;
    std::array<std::unique_ptr<Tracker<FloatType>>, ZLDsp::loudness::loudnessNUM> trackers{
            std::make_unique<RMSTracker<FloatType>>(false),
            std::make_unique<LUFSTracker<FloatType>>(),
            std::make_unique<RMSTracker<FloatType>>(true),
    };
};

#endif //ZLLMATCH_MONITOR_H
