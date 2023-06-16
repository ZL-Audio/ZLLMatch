#ifndef ZLLMATCH_MIDPANEL_H
#define ZLLMATCH_MIDPANEL_H

#include "../DSP/Controller.h"
#include "../DSP/dsp_defines.h"
#include "../GUI/ButtonComponent.h"
#include "../GUI/ChangingValueComponent.h"
#include "../GUI/interface_defines.h"
#include "LogoPanel.h"
#include <juce_audio_processors/juce_audio_processors.h>

class MidPanel : public juce::Component {
public:
    explicit MidPanel (juce::AudioProcessorValueTreeState& parameters,
        Controller<float>* controller);

    ~MidPanel() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    void setFontSize (float size);

    void setMode(int modeID);

private:
    LogoPanel logoPanel;
    std::unique_ptr<ButtonComponent> ceilButton;
    std::unique_ptr<ChangingValueComponent> gainValue;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
};

#endif //ZLLMATCH_MIDPANEL_H
