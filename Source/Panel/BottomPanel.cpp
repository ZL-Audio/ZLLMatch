#include "BottomPanel.h"

BottomPanel::BottomPanel(juce::AudioProcessorValueTreeState& parameters) {
    // init sliders
    std::array sliderList { &strengthSlider, &boundSlider, &gateSlider, &targetSlider };
    std::array sliderID { ZLDsp::strength::ID, ZLDsp::bound::ID, ZLDsp::gate::ID, ZLDsp::target::ID };
    for (size_t i = 0; i < sliderList.size(); ++i) {
        *sliderList[i] = std::make_unique<RotarySliderComponent> (parameters.getParameter (sliderID[i])->name);
        addAndMakeVisible (*(*sliderList[i]));
        sliderAttachments.add (
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (parameters, sliderID[i], (*sliderList[i])->getSlider()));
    }
}

BottomPanel::~BottomPanel() = default;

void BottomPanel::paint (juce::Graphics&) {}

void BottomPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;
    items.add(*strengthSlider);
    items.add(*boundSlider);
    items.add(*gateSlider);
    items.add(*targetSlider);
    grid.items = items;

    grid.performLayout(getLocalBounds());
}

void BottomPanel::setFontSize (float size) {
    std::array sliderList { &strengthSlider, &boundSlider, &gateSlider, &targetSlider };
    for (auto const& s : sliderList) {
        (*s)->setFontSize (size);
    }
}