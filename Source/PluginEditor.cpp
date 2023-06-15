#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), mainPanel (p.parameters, p.getController()) {
    // set font
    auto sourceCodePro = juce::Typeface::createSystemTypefaceFor (BinaryData::OpenSansSemiBold_ttf,
        BinaryData::OpenSansSemiBold_ttfSize);
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface (sourceCodePro);

    // add main panel
    addAndMakeVisible (mainPanel);

    // set size & size listener
    setResizeLimits (ZLInterface::WindowMinWidth, ZLInterface::WindowMinHeight, ZLInterface::WindowMaxWidth, ZLInterface::WindowMaxHeight);
    getConstrainer()->setFixedAspectRatio (ZLInterface::WindowFixedAspectRatio);
    setResizable (true, p.wrapperType != PluginProcessor::wrapperType_AudioUnitv3);
    lastUIWidth.referTo (p.parameters.state.getChildWithName ("uiState").getPropertyAsValue ("width", nullptr));
    lastUIHeight.referTo (p.parameters.state.getChildWithName ("uiState").getPropertyAsValue ("height", nullptr));
    setSize (lastUIWidth.getValue(), lastUIHeight.getValue());
    lastUIWidth.addListener (this);
    lastUIHeight.addListener (this);
}

PluginEditor::~PluginEditor() {
}

//==============================================================================
void PluginEditor::paint (juce::Graphics& g) {
    juce::ignoreUnused(g);
}

void PluginEditor::resized() {
    mainPanel.setBounds (getLocalBounds());
    lastUIWidth = getWidth();
    lastUIHeight = getHeight();
}

void PluginEditor::valueChanged (juce::Value&) {
    setSize (lastUIWidth.getValue(), lastUIHeight.getValue());
}