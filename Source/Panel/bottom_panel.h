#ifndef ZLLMATCH_BOTTOMPANEL_H
#define ZLLMATCH_BOTTOMPANEL_H

#include "../DSP/dsp_defines.h"
#include "../GUI/rotary_slider_component.h"
#include "../GUI/interface_definitions.h"
#include "panel_definitions.h"
#include "../PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class BottomPanel : public juce::Component, private juce::AsyncUpdater  {
public:
    explicit BottomPanel(PluginProcessor &p, zlinterface::UIBase &base);

    ~BottomPanel() override;

    void paint(juce::Graphics &) override;

    void resized() override;

    void setMode(int modeID);

private:
    std::unique_ptr<zlinterface::RotarySliderComponent> strengthSlider, boundSlider, gateSlider, targetSlider;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> rotarySliderAttachments;
    std::array<std::unique_ptr<zlinterface::RotarySliderComponent> *, 4> rotarySliderList{&strengthSlider, &boundSlider,
                                                                                          &gateSlider, &targetSlider};

    void handleAsyncUpdate() override;
};

#endif //ZLLMATCH_BOTTOMPANEL_H
