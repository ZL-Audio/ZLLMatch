#ifndef ZLLMATCH_CHANGING_VALUE_COMPONENT_H
#define ZLLMATCH_CHANGING_VALUE_COMPONENT_H

#include "../DSP/Controller.h"
#include "name_look_and_feel.h"
#include "interface_definitions.h"
#include "juce_gui_basics/juce_gui_basics.h"

namespace zlinterface {
    class ChangingValueComponent : public juce::Component, private juce::Timer {
    public:
        explicit ChangingValueComponent(Controller<float> *controller, UIBase &base) :
                nameLookAndFeel(base) {
            source = controller;
            startTimerHz(zlinterface::RefreshFreqHz);
            nameLookAndFeel.setFontScale(zlinterface::FontHuge3);
            label.setLookAndFeel(&nameLookAndFeel);
            addAndMakeVisible(label);
        }

        ~ChangingValueComponent() override {
            stopTimer();
            label.setLookAndFeel(nullptr);
        }

        void resized() override {
            label.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
        }

        void timerCallback() override {
            label.setText(zlinterface::formatFloat(source->getGain(), 2), juce::dontSendNotification);
        }

    private:
        Controller<float> *source;
        juce::Label label;
        NameLookAndFeel nameLookAndFeel;
    };
}

#endif //ZLLMATCH_CHANGING_VALUE_COMPONENT_H
