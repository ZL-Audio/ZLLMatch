#ifndef ZLINFLATOR_NAMELOOKANDFEEL_H
#define ZLINFLATOR_NAMELOOKANDFEEL_H

#include "interface_defines.h"
#include "juce_gui_basics/juce_gui_basics.h"

class NameLookAndFeel : public juce::LookAndFeel_V4 {
public:
  void drawLabel(juce::Graphics &g, juce::Label &label) override {
    g.fillAll(ZLInterface::BackgroundColor);
    g.setColour(ZLInterface::TextColor);
    auto labelArea{label.getLocalBounds().toFloat()};
    auto center = labelArea.getCentre();
    if (fontSize > 0) {
      g.setFont(fontSize);
    } else {
      g.setFont(labelArea.getHeight() * 0.6f);
    }
    g.drawSingleLineText(
        juce::String(label.getText()),
        juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
        juce::roundToInt(center.y + g.getCurrentFont().getDescent()),
        juce::Justification::centred);
  }

  void setFontSize(float size) { fontSize = size; }

private:
  std::atomic<float> fontSize = 0.0f;
};

#endif // ZLINFLATOR_NAMELOOKANDFEEL_H
