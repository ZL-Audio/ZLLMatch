#include "top_panel.h"

TopPanel::TopPanel(PluginProcessor &p, zlinterface::UIBase &base) {
    // init combobox
    std::array<std::string, 4> boxID{zldsp::mode::ID, zldsp::loudness::ID, zldsp::period::ID, zldsp::side::ID};

    zlpanel::attachBoxes<zlinterface::ComboboxComponent, 4>(*this, boxList, boxAttachments, boxID, p.parameters, base);
}

TopPanel::~TopPanel() = default;

void TopPanel::paint(juce::Graphics &) {}

void TopPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;
    items.add(*modeBox);
    items.add(*loudnessBox);
    items.add(*sideBox);
    items.add(*periodBox);
    grid.items = items;

    grid.performLayout(getLocalBounds());
}

void TopPanel::setMode(int modeID) {
    if (modeID == zldsp::mode::learn) {
        loudnessBox->setEditable(true);
        periodBox->setEditable(true);
        sideBox->setEditable(true);
    } else if (modeID == zldsp::mode::effect) {
        loudnessBox->setEditable(true);
        periodBox->setEditable(false);
        sideBox->setEditable(true);
    } else {
        loudnessBox->setEditable(false);
        periodBox->setEditable(false);
        sideBox->setEditable(false);
    }
    triggerAsyncUpdate();
}

void TopPanel::handleAsyncUpdate() {
    repaint();
}