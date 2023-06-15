#pragma once

#include "Panel/MainPanel.h"
#include "PluginProcessor.h"
#include <BinaryData.h>

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, private juce::Value::Listener {
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    MainPanel mainPanel;
    juce::Value lastUIWidth, lastUIHeight;

    void valueChanged (juce::Value&) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
