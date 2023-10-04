#ifndef ZLLMATCH_TOPPANEL_H
#define ZLLMATCH_TOPPANEL_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "../PluginProcessor.h"
#include "../DSP/dsp_defines.h"
#include "../GUI/interface_definitions.h"
#include "../GUI/combobox_component.h"
#include "panel_definitions.h"


class TopPanel : public juce::Component, private juce::AsyncUpdater {
public:
    explicit TopPanel(PluginProcessor &p, zlinterface::UIBase &base);

    ~TopPanel() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    void setMode(int modeID);

private:
    std::unique_ptr<zlinterface::ComboboxComponent> modeBox, loudnessBox, periodBox, sideBox;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> boxAttachments;
    std::array<std::unique_ptr<zlinterface::ComboboxComponent>*, 4> boxList{&modeBox, &loudnessBox,
                                                                            &periodBox, &sideBox};

    void handleAsyncUpdate() override;
};

#endif //ZLLMATCH_TOPPANEL_H
