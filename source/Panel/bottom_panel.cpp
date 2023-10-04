#include "bottom_panel.h"

BottomPanel::BottomPanel(PluginProcessor &p, zlinterface::UIBase &base) {
    // init sliders
    std::array<std::string, 4> rotarySliderID{zldsp::strength::ID, zldsp::bound::ID, zldsp::gate::ID,
                                              zldsp::target::ID};
    zlpanel::attachSliders<zlinterface::RotarySliderComponent, 4>(*this, rotarySliderList,
                                                                  rotarySliderAttachments,
                                                                  rotarySliderID,
                                                                  p.parameters, base);
}

BottomPanel::~BottomPanel() = default;

void BottomPanel::paint(juce::Graphics &) {}

void BottomPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;
    items.add(*strengthSlider);
    items.add(*boundSlider);
    items.add(*targetSlider);
    items.add(*gateSlider);
    grid.items = items;

    grid.performLayout(getLocalBounds());
}

void BottomPanel::setMode(int modeID) {
    if (modeID == zldsp::mode::learn) {
        boundSlider->setEditable(true);
        gateSlider->setEditable(true);
        targetSlider->setEditable(true);
    } else if (modeID == zldsp::mode::effect) {
        boundSlider->setEditable(true);
        gateSlider->setEditable(false);
        targetSlider->setEditable(true);
    } else if (modeID == zldsp::mode::envelope) {
        boundSlider->setEditable(false);
        gateSlider->setEditable(false);
        targetSlider->setEditable(false);
    }
    triggerAsyncUpdate();
}

void BottomPanel::handleAsyncUpdate() {
    repaint();
}