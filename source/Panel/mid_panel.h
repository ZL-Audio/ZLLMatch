#ifndef ZLLMATCH_MIDPANEL_H
#define ZLLMATCH_MIDPANEL_H

#include "../DSP/Controller.h"
#include "../DSP/dsp_defines.h"
#include "../GUI/button_component.h"
#include "../GUI/changing_value_component.h"
#include "../GUI/interface_definitions.h"
#include "logo_panel.h"
#include "panel_definitions.h"
#include <juce_audio_processors/juce_audio_processors.h>

class MidPanel : public juce::Component, private juce::AsyncUpdater  {
public:
    explicit MidPanel(PluginProcessor &p,
                      Controller<float> *controller, zlinterface::UIBase &base);

    ~MidPanel() override;

    void paint(juce::Graphics &) override;

    void resized() override;

    void setMode(int modeID);

private:
    zlpanel::LogoPanel logoPanel;
    std::unique_ptr<zlinterface::ButtonComponent> ceilButton;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
    std::array<std::unique_ptr<zlinterface::ButtonComponent> *, 1> buttonList{&ceilButton};

    std::unique_ptr<zlinterface::ChangingValueComponent> gainValue;

    void handleAsyncUpdate() override;
};

#endif //ZLLMATCH_MIDPANEL_H
