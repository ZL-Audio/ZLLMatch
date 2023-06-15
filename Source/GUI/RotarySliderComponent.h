#ifndef ZLINFLATOR_ROTARYSLIDERCOMPONENT_H
#define ZLINFLATOR_ROTARYSLIDERCOMPONENT_H

#include "NameLookAndFeel.h"
#include "RotarySliderLookAndFeel.h"
#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class RotarySliderComponent : public juce::Component {
public:

    explicit RotarySliderComponent(const juce::String &labelText) :
            myLookAndFeel() {
        // setup slider
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxIsEditable(false);
        slider.setDoubleClickReturnValue(true, 0.0);
        slider.setLookAndFeel(&myLookAndFeel);
        addAndMakeVisible(slider);

        // setup label
        label.setText(labelText, juce::dontSendNotification);
        label.setLookAndFeel(&nameLookAndFeel);
        addAndMakeVisible(label);
    }

    void resized() override {
        auto bound = getLocalBounds().toFloat();
        auto boundMinWH = juce::jmin(bound.getWidth(), bound.getHeight() - fontSize * ZLInterface::FontHuge);
        bound = bound.withSizeKeepingCentre(boundMinWH, boundMinWH + fontSize * ZLInterface::FontHuge);
        auto textBound = bound.removeFromTop(fontSize * ZLInterface::FontHuge);
        label.setBounds(textBound.toNearestInt());
        auto bounds = bound;
        auto radius = juce::jmin(sliderHeight * bounds.getWidth(), sliderHeight * bounds.getHeight());
        auto buttonBounds = bounds.withSizeKeepingCentre(radius, radius);
        slider.setBounds(buttonBounds.toNearestInt());
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(ZLInterface::BackgroundColor);
    }

    juce::Slider &getSlider() { return slider; }

    juce::Label &getLabel() { return label; }

    void setFontSize(float size) {
        fontSize = size;
        myLookAndFeel.setFontSize(size);
        nameLookAndFeel.setFontSize(size);
    }

private:
    RotarySliderLookAndFeel myLookAndFeel;
    NameLookAndFeel nameLookAndFeel;
    juce::Slider slider;
    juce::Label label;
    float fontSize;

    constexpr static float sliderHeight = 0.85f;
    constexpr static float labelHeight = 1.f - sliderHeight;
};

#endif //ZLINFLATOR_ROTARYSLIDERCOMPONENT_H
