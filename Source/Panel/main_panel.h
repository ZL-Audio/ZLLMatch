#ifndef ZLLMATCH_MAINPANEL_H
#define ZLLMATCH_MAINPANEL_H

#include "../DSP/Controller.h"
#include "../DSP/dsp_defines.h"
#include "../PluginProcessor.h"
#include "bottom_panel.h"
#include "mid_panel.h"
#include "top_panel.h"
#include <juce_audio_processors/juce_audio_processors.h>

class MainPanel : public juce::Component {
public:
    explicit MainPanel(PluginProcessor &p, Controller<float> *controller);

    ~MainPanel() override;

    void paint(juce::Graphics &) override;

    void resized() override;

    void setMode(int modeID);

private:
    zlinterface::UIBase uiBase;

    TopPanel topPanel;
    MidPanel midPanel;
    BottomPanel bottomPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

class MainPanelAttach : private juce::AudioProcessorValueTreeState::Listener {
public:
    explicit MainPanelAttach(MainPanel &mainPanel, juce::AudioProcessorValueTreeState &parameters) {
        panel = &mainPanel;
        apvts = &parameters;
        apvts->addParameterListener(zldsp::mode::ID, this);
    }

    ~MainPanelAttach() override {
        apvts->removeParameterListener(zldsp::mode::ID, this);
    }

    void parameterChanged(const juce::String &parameterID, float newValue) override {
        juce::ignoreUnused(parameterID);
        auto ID = static_cast<int>(newValue);
        if (ID != modeID.load()) {
            modeID.store(ID);
            panel->setMode(ID);
        }
    }

private:
    MainPanel *panel;
    juce::AudioProcessorValueTreeState *apvts;
    std::atomic<int> modeID = zldsp::mode::defaultI;
};

#endif //ZLLMATCH_MAINPANEL_H
