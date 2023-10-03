#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor &p)
        : AudioProcessorEditor(&p), processorRef(p),
          property(p.states),
          mainPanel(p, p.getController()),
          mainPanelAttach(mainPanel, p.parameters) {
    for (auto &ID: IDs) {
        processorRef.states.addParameterListener(ID, this);
    }
    // set font
    auto sourceCodePro = juce::Typeface::createSystemTypefaceFor(BinaryData::OpenSansSemiBold_ttf,
                                                                 BinaryData::OpenSansSemiBold_ttfSize);
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(sourceCodePro);

    // add main panel
    addAndMakeVisible(mainPanel);

    // set size & size listener
    setResizeLimits(zlstate::windowW::minV, zlstate::windowH::minV, zlstate::windowW::maxV, zlstate::windowH::maxV);
    getConstrainer()->setFixedAspectRatio(
            static_cast<float>(zlstate::windowW::defaultV) / static_cast<float>(zlstate::windowH::defaultV));
    setResizable(true, p.wrapperType != PluginProcessor::wrapperType_AudioUnitv3);
    lastUIWidth.referTo(p.states.getParameterAsValue(zlstate::windowW::ID));
    lastUIHeight.referTo(p.states.getParameterAsValue(zlstate::windowH::ID));
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
    lastUIWidth.addListener(this);
    lastUIHeight.addListener(this);
}

PluginEditor::~PluginEditor()  {
    for (auto &ID: IDs) {
        processorRef.states.removeParameterListener(ID, this);
    }
}

//==============================================================================
void PluginEditor::paint(juce::Graphics &g) {
    juce::ignoreUnused(g);
}

void PluginEditor::resized() {
    mainPanel.setBounds(getLocalBounds());
    lastUIWidth = getWidth();
    lastUIHeight = getHeight();
}

void PluginEditor::valueChanged(juce::Value &) {
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
}

void PluginEditor::parameterChanged(const juce::String &parameterID, float newValue) {
    juce::ignoreUnused(parameterID, newValue);
    triggerAsyncUpdate();
}

void PluginEditor::handleAsyncUpdate() {
    property.saveAPVTS(processorRef.states);
}