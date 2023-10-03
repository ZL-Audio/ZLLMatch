#include "mid_panel.h"

MidPanel::MidPanel(PluginProcessor &p,
                   Controller<float> *controller, zlinterface::UIBase &base)
        : logoPanel(p, base) {
    // init buttons
    std::array<std::string, 1> buttonID{zldsp::ceil::ID};
    zlpanel::attachButtons<zlinterface::ButtonComponent, 1>(*this, buttonList, buttonAttachments, buttonID,
                                                            p.parameters, base);


    // init changing value label
    gainValue = std::make_unique<zlinterface::ChangingValueComponent>(controller, base);

    addAndMakeVisible(*gainValue);

    addAndMakeVisible(logoPanel);
}

MidPanel::~MidPanel() = default;

void MidPanel::paint(juce::Graphics &) {}

void MidPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(2)), Track(Fr(4)), Track(Fr(2))};

    juce::Array<juce::GridItem> items;
    items.add(logoPanel);
    items.add(*gainValue);
    items.add(*ceilButton);

    grid.items = items;

    grid.performLayout(getLocalBounds());
}

void MidPanel::setMode(int modeID) {
    if (modeID == zldsp::mode::envelope) {
        ceilButton->setEditable(false);
    } else {
        ceilButton->setEditable(true);
    }
    triggerAsyncUpdate();
}

void MidPanel::handleAsyncUpdate() {
    repaint();
}