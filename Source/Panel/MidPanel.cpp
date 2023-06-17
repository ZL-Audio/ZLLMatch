#include "MidPanel.h"

MidPanel::MidPanel (juce::AudioProcessorValueTreeState& parameters, Controller<float>* controller) : logoPanel() {
    // init buttons
    std::array buttonList { &ceilButton };
    std::array buttonID { ZLDsp::ceil::ID };
    for (size_t i = 0; i < buttonList.size(); ++i) {
        *buttonList[i] = std::make_unique<ButtonComponent> (parameters.getParameter (buttonID[i])->name);
        addAndMakeVisible (*(*buttonList[i]));
        buttonAttachments.add (
            std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (parameters, buttonID[i], (*buttonList[i])->getButton()));
    }

    // init changing value label
    std::array changingValueList { &gainValue };
    for (auto& i : changingValueList) {
        *i = std::make_unique<ChangingValueComponent> (controller);
        addAndMakeVisible (*(*i));
    }

    addAndMakeVisible(logoPanel);
}

MidPanel::~MidPanel() = default;

void MidPanel::paint (juce::Graphics&) {}

void MidPanel::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(2)), Track(Fr(1))};

    juce::Array<juce::GridItem> items;
    items.add(logoPanel);
    items.add(*gainValue);
    items.add(*ceilButton);

    grid.items = items;

    grid.performLayout(getLocalBounds());
}

void MidPanel::setFontSize (float size) {
    ceilButton->setFontSize(size);
    gainValue->setFontSize(size);
}

void MidPanel::setMode(int modeID) {
    if (modeID == ZLDsp::mode::envelope) {
        ceilButton->setEditable(false);
    } else {
        ceilButton->setEditable(true);
    }
}