#ifndef ZLLMATCH_MONITOR_H
#define ZLLMATCH_MONITOR_H

#include "RMSTracker.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

template <typename FloatType>
class Monitor {
public:
    Monitor() = default;

    void reset() {
        peak = -120;
        bPeak = -120;
        rmsTracker.reset();
    }

    void process (const juce::AudioBuffer<float>& buffer) {
        bPeak = buffer.getMagnitude (0, buffer.getNumSamples());
        peak = juce::jmax (bPeak, peak);
        rmsTracker.process (buffer);
    }

    FloatType getLoudness() {
        return juce::Decibels::gainToDecibels (rmsTracker.getPeriodLoudness());
    }

    FloatType getBufferPeak() {
        return juce::Decibels::gainToDecibels (bPeak);
    }

    FloatType getPeak() {
        return juce::Decibels::gainToDecibels (peak);
    }

private:
    FloatType peak = -120, bPeak = -120;
    RMSTracker<FloatType> rmsTracker;
};

#endif //ZLLMATCH_MONITOR_H
