#ifndef ZLLMATCH_MAINPANEL_H
#define ZLLMATCH_MAINPANEL_H

#include "../DSP/Controller.h"
#include "../DSP/dsp_defines.h"
#include "BottomPanel.h"
#include "MidPanel.h"
#include "TopPanel.h"
#include <juce_audio_processors/juce_audio_processors.h>

class MainPanel : public juce::Component {
public:
    explicit MainPanel (juce::AudioProcessorValueTreeState& parameters,
        Controller<float>* controller);

    ~MainPanel() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    void setMode (int modeID);

private:
    juce::AudioProcessorValueTreeState* apvts;
    TopPanel topPanel;
    MidPanel midPanel;
    BottomPanel bottomPanel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

//class MainPanelAttach : private juce::AudioProcessorValueTreeState::Listener {
class MainPanelAttach : private juce::Timer {
public:
    explicit MainPanelAttach (MainPanel& mainPanel, juce::AudioProcessorValueTreeState& parameters) {
        panel = &mainPanel;
        apvts = &parameters;
        startTimerHz(ZLInterface::RefreshFreqHz);
//        parameters.addParameterListener (ZLDsp::mode::ID, this);
    }

    ~MainPanelAttach() {
        stopTimer();
    }

    void timerCallback() override {
        auto ID = static_cast<int>(apvts->getRawParameterValue(ZLDsp::mode::ID)->load());
        if (ID != modeID.load()) {
            modeID.store(ID);
            panel->setMode(ID);
        }
    }

private:
    MainPanel* panel;
    juce::AudioProcessorValueTreeState* apvts;
    std::atomic<int> modeID = ZLDsp::mode::defaultI;

//    void parameterChanged (const juce::String& parameterID, float newValue) override {
//        if (parameterID == ZLDsp::mode::ID) {
//            auto modeID = static_cast<int> (newValue);
//            panel->setMode (modeID);
//        }
//    }
};

#endif //ZLLMATCH_MAINPANEL_H
