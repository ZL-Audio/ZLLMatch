#ifndef ZLLMATCH_LUFSTRACKER_H
#define ZLLMATCH_LUFSTRACKER_H

#include "LUFSMeter/Ebu128LoudnessMeter.h"
#include "dsp_defines.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

template <typename FloatType>
class LUFSTracker {
public:
    LUFSTracker() = default;

    ~LUFSTracker() {
        reset();
    }

    void prepare (const juce::dsp::ProcessSpec& spec) {
        meter.prepareToPlay (spec.sampleRate, static_cast<int> (spec.numChannels), static_cast<int> (spec.sampleRate), 100);
    }

    void reset() {
        meter.reset();
    }

    FloatType getIntegratedLoudness() {
        return static_cast<FloatType>(meter.getIntegratedLoudness());
    }

    void process (const juce::AudioBuffer<float>& buffer, FloatType gate) {
        juce::ignoreUnused(gate);
        meter.processBlock(buffer);
    }

private:
    Ebu128LoudnessMeter meter;
};

#endif //ZLLMATCH_LUFSTRACKER_H