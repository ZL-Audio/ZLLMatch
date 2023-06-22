#ifndef ZLINFLATOR_INTERFACE_DEFINES_H
#define ZLINFLATOR_INTERFACE_DEFINES_H

#include "juce_gui_basics/juce_gui_basics.h"

namespace ZLInterface {
    auto static const WindowHeight = 280;
    auto static const WindowWidth = 385;
    auto static const WindowFixedAspectRatio = 1.375;
    auto static const WindowMinHeight = 200;
    auto static const WindowMinWidth = 275;
    auto static const WindowMaxHeight = WindowMinHeight * 10;
    auto static const WindowMaxWidth = WindowMinWidth * 10;


    auto static const TextColor = juce::Colour(87, 96, 110);
    auto static const TextInactiveColor = juce::Colour(87, 96, 110).withAlpha(0.5f);
    auto static const TextHideColor = juce::Colour(87, 96, 110).withAlpha(0.25f);
    auto static const BackgroundColor = juce::Colour(214, 223, 236);
    auto static const BackgroundInactiveColor = juce::Colour(214, 223, 236).withAlpha(0.8f);
    auto static const BackgroundHideColor = juce::Colour(214, 223, 236).withAlpha(0.5f);
    auto static const DarkShadowColor = juce::Colour(119, 122, 126);
    auto static const BrightShadowColor = juce::Colour(237, 246, 255);

    auto static const FontTiny = 0.5f;
    auto static const FontSmall = 0.75f;
    auto static const FontNormal = 1.0f;
    auto static const FontLarge = 1.25f;
    auto static const FontHuge = 1.5f;
    auto static const FontHuge2 = 3.0f;
    auto static const FontHuge3 = 4.5f;

    auto static const RefreshFreqHz = 120;

    static juce::Rectangle<float> fillRoundedRectangle(juce::Graphics &g, juce::Rectangle<float> boxBounds, float cornerSize,
                                     bool curveTopLeft = true, bool curveTopRight = true,
                                     bool curveBottomLeft = true, bool curveBottomRight = true) {
        juce::Path path;
        auto radius = juce::jmax(juce::roundToInt(1.5f * cornerSize), 1);
        boxBounds = boxBounds.withSizeKeepingCentre(
                boxBounds.getWidth() - static_cast<float>(radius) - cornerSize,
                boxBounds.getHeight() - static_cast<float>(radius) - cornerSize);
        path.addRoundedRectangle(boxBounds.getX(), boxBounds.getY(),
                                 boxBounds.getWidth(), boxBounds.getHeight(),
                                 cornerSize, cornerSize,
                                 curveTopLeft, curveTopRight,
                                 curveBottomLeft, curveBottomRight);
        juce::DropShadow darkShadow(ZLInterface::DarkShadowColor, radius,
                                    {juce::roundToInt(cornerSize * 0.25f), juce::roundToInt(cornerSize * 0.25f)});
        darkShadow.drawForPath(g, path);
        juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, radius,
                                      {-juce::roundToInt(cornerSize * 0.25f), -juce::roundToInt(cornerSize * 0.25f)});
        brightShadow.drawForPath(g, path);
        g.setColour(ZLInterface::BackgroundColor);
        g.fillPath(path);
        return boxBounds;
    }

    static juce::Rectangle<float> drawEllipse(juce::Graphics &g, juce::Rectangle<float> boxBounds, float cornerSize) {
        juce::Path path;
        auto radius = juce::jmax(juce::roundToInt(1.5f * cornerSize), 1);
        boxBounds = boxBounds.withSizeKeepingCentre(
                boxBounds.getWidth() - static_cast<float>(radius) - cornerSize,
                boxBounds.getHeight() - static_cast<float>(radius) - cornerSize);
        path.addEllipse(boxBounds);
        juce::DropShadow darkShadow(ZLInterface::DarkShadowColor, radius,
                                    {juce::roundToInt(cornerSize * 0.25f), juce::roundToInt(cornerSize * 0.25f)});
        darkShadow.drawForPath(g, path);
        juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, radius,
                                      {-juce::roundToInt(cornerSize * 0.25f), -juce::roundToInt(cornerSize * 0.25f)});
        brightShadow.drawForPath(g, path);
        g.setColour(ZLInterface::BackgroundColor);
        g.fillPath(path);
        return boxBounds;
    }

    static std::string formatFloat(float x, int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << x;
        return stream.str();
    }
}

#endif //ZLINFLATOR_INTERFACE_DEFINES_H
