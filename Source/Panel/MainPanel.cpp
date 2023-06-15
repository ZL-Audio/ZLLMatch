#include "MainPanel.h"

MainPanel::MainPanel (juce::AudioProcessorValueTreeState& parameters,
    Controller<float>* controller) : topPanel (parameters),
                                     midPanel (parameters, controller),
                                     bottomPanel (parameters) {
    addAndMakeVisible (topPanel);
    addAndMakeVisible (midPanel);
    addAndMakeVisible (bottomPanel);
}

MainPanel::~MainPanel() = default;

void MainPanel::paint (juce::Graphics& g) {
    g.fillAll (ZLInterface::BackgroundColor);
    auto bound = getLocalBounds().toFloat();
    auto padding = bound.getHeight() * 0.057142857f;
    bound = bound.withSizeKeepingCentre (bound.getWidth() - padding, bound.getHeight() - padding);
    float cornerSize = bound.getHeight() * 0.034285714f;
    ZLInterface::fillRoundedRectangle (g, bound, cornerSize);
}

void MainPanel::resized() {
    topPanel.setFontSize(static_cast<float>(getLocalBounds().toFloat().getHeight()) * 0.0514f);
    midPanel.setFontSize(static_cast<float>(getLocalBounds().toFloat().getHeight()) * 0.0514f);
    bottomPanel.setFontSize(static_cast<float>(getLocalBounds().toFloat().getHeight()) * 0.0514f);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.templateRows = {Track(Fr(3)), Track(Fr(3)), Track(Fr(5))};
    grid.templateColumns = {Track(Fr(1))};

    juce::Array<juce::GridItem> items;
    items.add(midPanel);
    items.add(topPanel);
    items.add(bottomPanel);
    grid.items = items;

    auto bound = getLocalBounds().toFloat();
    auto padding = bound.getHeight() * 0.09142857f;
    bound = bound.withSizeKeepingCentre(bound.getWidth() - padding, bound.getHeight() - padding);
    grid.performLayout(bound.toNearestInt());
}