#ifndef ZLLMATCH_TRACKER_H
#define ZLLMATCH_TRACKER_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

template<typename FloatType>
class Tracker {
public:
    Tracker() = default;

    virtual ~Tracker() = default;

    virtual void prepare(const juce::dsp::ProcessSpec &spec) = 0;

    virtual void reset() = 0;

    virtual FloatType getIntegratedLoudness() = 0;

    virtual void process(const juce::AudioBuffer<float> &buffer, FloatType gate) = 0;
};

#endif // ZLLMATCH_TRACKER_H
