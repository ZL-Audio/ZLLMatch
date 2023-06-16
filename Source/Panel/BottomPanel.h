#ifndef ZLLMATCH_BOTTOMPANEL_H
#define ZLLMATCH_BOTTOMPANEL_H

#include "../DSP/dsp_defines.h"
#include "../GUI/RotarySliderComponent.h"
#include "../GUI/interface_defines.h"
#include <juce_audio_processors/juce_audio_processors.h>

class BottomPanel : public juce::Component {
public:
    explicit BottomPanel (juce::AudioProcessorValueTreeState& parameters);

    ~BottomPanel() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    void setFontSize (float size);

    void setMode(int modeID);

private:
    std::unique_ptr<RotarySliderComponent> strengthSlider, boundSlider, gateSlider, targetSlider;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments;
};

#endif //ZLLMATCH_BOTTOMPANEL_H
