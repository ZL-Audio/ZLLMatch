#ifndef ZLLMATCH_TOPPANEL_H
#define ZLLMATCH_TOPPANEL_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/dsp_defines.h"
#include "../GUI/interface_defines.h"
#include "../GUI/ComboboxComponent.h"


class TopPanel : public juce::Component {
public:
    explicit TopPanel(juce::AudioProcessorValueTreeState& parameters);

    ~TopPanel() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    void setFontSize (float size);

    void setMode(int modeID);

private:
    std::unique_ptr<ComboboxComponent> modeBox, loudnessBox, periodBox, sideBox;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboboxAttachments;
};

#endif //ZLLMATCH_TOPPANEL_H
