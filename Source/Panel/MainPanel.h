#ifndef ZLLMATCH_MAINPANEL_H
#define ZLLMATCH_MAINPANEL_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/dsp_defines.h"
#include "../DSP/Controller.h"
#include "TopPanel.h"
#include "MidPanel.h"
#include "BottomPanel.h"



class MainPanel : public juce::Component, public juce::AudioProcessorValueTreeState::Listener{
public:
    explicit MainPanel (juce::AudioProcessorValueTreeState& parameters,
        Controller<float>* controller);

    ~MainPanel() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    void parameterChanged (const juce::String& parameterID, float newValue) override;

private:
    TopPanel topPanel;
    MidPanel midPanel;
    BottomPanel bottomPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)

    void setMode(int modeID);
};

#endif //ZLLMATCH_MAINPANEL_H
