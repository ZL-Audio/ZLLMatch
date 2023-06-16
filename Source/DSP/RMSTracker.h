#ifndef ZLLMATCH_RMSTRACKER_H
#define ZLLMATCH_RMSTRACKER_H

#include "dsp_defines.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

template <typename FloatType>
class RMSTracker {
public:
    RMSTracker() = default;

    ~RMSTracker() {
        reset();
    }

    void prepare(const juce::dsp::ProcessSpec &spec) {
        juce::ignoreUnused(spec);
    }

    void reset() {
        ms.clear();
    }

    FloatType getIntegratedLoudness() {
        auto meanSquare = std::reduce (ms.begin(), ms.end()) / static_cast<FloatType> (ms.size());
        return juce::Decibels::gainToDecibels (std::sqrt (meanSquare));
    }

    void process (const juce::AudioBuffer<float>& buffer, FloatType gate) {
        FloatType _ms = 0;
        for (auto channel = 0; channel < buffer.getNumChannels(); channel++) {
            auto data = buffer.getReadPointer (channel);
            for (auto i = 0; i < buffer.getNumSamples(); i++) {
                _ms += data[i] * data[i];
            }
        }
        _ms = _ms / static_cast<FloatType> (buffer.getNumSamples());
        if (_ms > gate) {
            ms.push_back (_ms);
        }
    }

private:
    std::vector<FloatType> ms;
};

#endif //ZLLMATCH_RMSTRACKER_H
