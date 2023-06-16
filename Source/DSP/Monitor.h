#ifndef ZLLMATCH_MONITOR_H
#define ZLLMATCH_MONITOR_H

#include "LUFSTracker.h"
#include "RMSTracker.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

template <typename FloatType>
class Monitor {
public:
    Monitor() = default;

    void prepare (const juce::dsp::ProcessSpec& spec) {
        rmsTracker.prepare (spec);
        lufsTracker.prepare (spec);
    }

    void reset() {
        peak = 0;
        bPeak = 0;
        rmsTracker.reset();
        lufsTracker.reset();
    }

    void process (const juce::AudioBuffer<float>& buffer, FloatType gate) {
        bPeak = buffer.getMagnitude (0, buffer.getNumSamples());
        peak = juce::jmax (bPeak, peak);
        rmsTracker.process (buffer, juce::Decibels::decibelsToGain (gate));
        lufsTracker.process (buffer, juce::Decibels::decibelsToGain (gate));
    }

    FloatType getLoudness() {
        if (loudnessID.load() == ZLDsp::loudness::rms) {
            return rmsTracker.getIntegratedLoudness();
        } else if (loudnessID.load() == ZLDsp::loudness::lufs) {
            return lufsTracker.getIntegratedLoudness();
        } else {
            return static_cast<FloatType> (0);
        }
    }

    FloatType getBufferPeak() {
        return juce::Decibels::gainToDecibels (bPeak);
    }

    FloatType getPeak() {
        return juce::Decibels::gainToDecibels (peak);
    }

    void setLoudnessID (int ID) {
        loudnessID = ID;
    }

private:
    FloatType peak = 0, bPeak = 0;
    std::atomic<int> loudnessID = ZLDsp::loudness::defaultI;
    RMSTracker<FloatType> rmsTracker;
    LUFSTracker<FloatType> lufsTracker;
};

#endif //ZLLMATCH_MONITOR_H
