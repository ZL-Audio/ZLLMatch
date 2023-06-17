#ifndef ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H
#define ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H

#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySliderLookAndFeel : public juce::LookAndFeel_V4 {
public:
    explicit RotarySliderLookAndFeel() = default;

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override {
        juce::ignoreUnused (slider);
        // draw background
        g.fillAll (ZLInterface::BackgroundColor);
        // calculate values
        auto rotationAngle =
            rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();
        auto diameter = juce::jmin (bounds.getWidth(), bounds.getHeight());
        bounds = bounds.withSizeKeepingCentre (0.9f * diameter, 0.9f * diameter);
        // draw knob
        ZLInterface::drawEllipse (g, bounds, fontSize * 0.15f);
        // draw arrow
        juce::Path path;
        float triangleUnit = diameter * 0.05f;
        path.addTriangle (-1.15f * triangleUnit, triangleUnit, 1.15f * triangleUnit, triangleUnit, 0, -triangleUnit);
        auto transform = juce::AffineTransform::translation (
            -0.0f + bounds.getCentreX(), -0.375f * diameter + bounds.getCentreY())
                             .rotated (
                                 rotationAngle, bounds.getCentreX(), bounds.getCentreY());
        if (editable) {
            g.setColour (ZLInterface::TextColor);
        } else {
            g.setColour (ZLInterface::TextInactiveColor);
        }
        g.fillPath (path, transform);
    }

    juce::Label* createSliderTextBox (juce::Slider&) override {
        auto* l = new juce::Label();
        l->setJustificationType (juce::Justification::centred);
        l->setInterceptsMouseClicks (false, false);
        return l;
    }

    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override {
        auto localBounds = slider.getLocalBounds().toFloat();
        juce::Slider::SliderLayout layout;
        auto textBounds = juce::Rectangle<float> (localBounds.getX() + 0.15f * localBounds.getWidth(),
            localBounds.getY() + 0.15f * localBounds.getHeight(),
            localBounds.getWidth() * 0.7f,
            localBounds.getHeight() * 0.7f);
        layout.textBoxBounds = textBounds.toNearestInt();
        layout.sliderBounds = localBounds.toNearestInt();
        return layout;
    }

    void drawLabel (juce::Graphics& g, juce::Label& label) override {
        if (editable.load()) {
            g.setColour (ZLInterface::TextColor);
        } else {
            g.setColour (ZLInterface::TextInactiveColor);
        }
        auto labelArea { label.getLocalBounds().toFloat() };
        auto center = labelArea.getCentre();
        if (fontSize > 0) {
            g.setFont (fontSize * ZLInterface::FontHuge);
        } else {
            g.setFont (labelArea.getHeight() * 0.6f);
        }
        g.drawSingleLineText (juce::String (label.getText()),
            juce::roundToInt (center.x + g.getCurrentFont().getHorizontalScale()),
            juce::roundToInt (center.y + g.getCurrentFont().getDescent()),
            juce::Justification::horizontallyCentred);
    }

    void setFontSize (float size) {
        fontSize = size;
    }

    void setEditable (bool f) {
        editable.store (f);
    }

private:
    std::atomic<float> fontSize = 0.0f;
    std::atomic<bool> editable = true;
};

#endif //ZLINFLATOR_ROTARYSLIDERLOOKANDFEEL_H