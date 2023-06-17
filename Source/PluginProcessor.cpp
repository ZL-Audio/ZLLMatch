#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                          .withInput ("Target", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, juce::Identifier ("ZLLMatchParameters"), ZLDsp::getParameterLayout()),
      controller (this, parameters),
      controllerAttach (controller, parameters) {
    parameters.state.addChild (
        { "uiState", { { "width", ZLInterface::WindowWidth }, { "height", ZLInterface::WindowHeight } }, {} }, -1, nullptr);
}

PluginProcessor::~PluginProcessor() = default;

//==============================================================================
const juce::String PluginProcessor::getName() const {
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const {
    return false;
}

bool PluginProcessor::producesMidi() const {
    return false;
}

bool PluginProcessor::isMidiEffect() const {
    return false;
}

double PluginProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int PluginProcessor::getNumPrograms() {
    return 1;
}

int PluginProcessor::getCurrentProgram() {
    return 0;
}

void PluginProcessor::setCurrentProgram (int index) {
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index) {
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName) {
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    auto channels = static_cast<juce::uint32> (juce::jmin (getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), channels };
    controller.prepare (spec);
}

void PluginProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages) {
    juce::ignoreUnused (midiMessages);
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    controller.process (buffer);
}

//==============================================================================
bool PluginProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor() {
    return new PluginEditor (*this);
    //    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData) {
    auto state = parameters.copyState();
    auto conState = state.getOrCreateChildWithName ("conState", nullptr);
    conState.setProperty ("diffs", controller.toString(), nullptr);
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr) {
        if (xmlState->hasTagName (parameters.state.getType())) {
            auto state = juce::ValueTree::fromXml (*xmlState);
            auto diffs = state.getChildWithName ("conState");
            if (diffs.isValid()) {
                state.removeChild(diffs, nullptr);
                if (diffs.hasProperty ("diffs")) {
                    controller.fromString (diffs.getProperty ("diffs"));
                }
            }
            parameters.replaceState (state);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

Controller<float>* PluginProcessor::getController() {
    return &controller;
}