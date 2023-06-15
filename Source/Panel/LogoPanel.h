#ifndef ZLLMATCH_LOGOPANEL_H
#define ZLLMATCH_LOGOPANEL_H

#include <BinaryData.h>
#include <juce_audio_processors/juce_audio_processors.h>

class LogoPanel : public juce::Component {
public:
    explicit LogoPanel();

    ~LogoPanel() override;

    void paint (juce::Graphics& g) override;

private:
    const std::unique_ptr<juce::Drawable> logoDrawable;
};

#endif //ZLLMATCH_LOGOPANEL_H
