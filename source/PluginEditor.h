#pragma once

#include "Panel/main_panel.h"
#include "PluginProcessor.h"
#include "State/property.h"
#include <BinaryData.h>

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, private juce::Value::Listener,
                     private juce::AudioProcessorValueTreeState::Listener,
                     private juce::AsyncUpdater {
public:
    explicit PluginEditor(PluginProcessor &);

    ~PluginEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;

    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor &processorRef;
    zlstate::Property property;
    MainPanel mainPanel;
    MainPanelAttach mainPanelAttach;
    juce::Value lastUIWidth, lastUIHeight;

    constexpr const static std::array IDs{zlstate::uiStyle::ID,
                                          zlstate::windowW::ID, zlstate::windowH::ID};

    void valueChanged(juce::Value &) override;

    void parameterChanged(const juce::String &parameterID, float newValue) override;

    void handleAsyncUpdate() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
