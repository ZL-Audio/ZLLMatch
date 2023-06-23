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
    auto static const TextInactiveColor = TextColor.withAlpha(0.5f);
    auto static const TextHideColor = TextColor.withAlpha(0.25f);
    auto static const BackgroundColor = juce::Colour(214, 223, 236);
    auto static const BackgroundInactiveColor = BackgroundColor.withAlpha(0.8f);
    auto static const BackgroundHideColor = BackgroundColor.withAlpha(0.5f);
    auto static const DarkShadowColor = juce::Colour(168, 172, 178);
    auto static const BrightShadowColor = juce::Colour(237, 246, 255);

    auto static const FontTiny = 0.5f;
    auto static const FontSmall = 0.75f;
    auto static const FontNormal = 1.0f;
    auto static const FontLarge = 1.25f;
    auto static const FontHuge = 1.5f;
    auto static const FontHuge2 = 3.0f;
    auto static const FontHuge3 = 4.5f;

    auto static const RefreshFreqHz = 120;

    static juce::Rectangle<float> fillRoundedShadowRectangle(juce::Graphics &g,
                                                             juce::Rectangle<float> boxBounds,
                                                             float cornerSize,
                                                             bool curveTopLeft = true,
                                                             bool curveTopRight = true,
                                                             bool curveBottomLeft = true,
                                                             bool curveBottomRight = true,
                                                             bool fit = true,
                                                             bool drawBright = true, bool drawDark = true) {
        juce::Path path;
        auto radius = juce::jmax(juce::roundToInt(cornerSize * 0.75f), 1);
        if (fit) {
            boxBounds = boxBounds.withSizeKeepingCentre(
                    boxBounds.getWidth() - static_cast<float>(radius) - 1.42f * cornerSize,
                    boxBounds.getHeight() - static_cast<float>(radius) - 1.42f * cornerSize);
        }
        path.addRoundedRectangle(boxBounds.getX(), boxBounds.getY(),
                                 boxBounds.getWidth(), boxBounds.getHeight(),
                                 cornerSize, cornerSize,
                                 curveTopLeft, curveTopRight,
                                 curveBottomLeft, curveBottomRight);
        auto offset = static_cast<int>(cornerSize * 0.5f);
        if (drawBright) {
            juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, radius,
                                          {-offset, -offset});
            brightShadow.drawForPath(g, path);
        }
        if (drawDark) {
            juce::DropShadow darkShadow(ZLInterface::DarkShadowColor, radius,
                                        {offset, offset});
            darkShadow.drawForPath(g, path);
        }
        g.setColour(ZLInterface::BackgroundColor);
        g.fillPath(path);
        return boxBounds;
    }

    static juce::Rectangle<float> fillRoundedInnerShadowRectangle(juce::Graphics &g,
                                                                  juce::Rectangle<float> boxBounds,
                                                                  float cornerSize,
                                                                  float blurRadius,
                                                                  bool curveTopLeft = true,
                                                                  bool curveTopRight = true,
                                                                  bool curveBottomLeft = true,
                                                                  bool curveBottomRight = true,
                                                                  bool flip = false) {
        juce::Path mask;
        mask.addRoundedRectangle(boxBounds.getX(), boxBounds.getY(),
                                 boxBounds.getWidth(), boxBounds.getHeight(),
                                 cornerSize, cornerSize,
                                 curveTopLeft, curveTopRight,
                                 curveBottomLeft, curveBottomRight);
        g.saveState();
        g.reduceClipRegion(mask);
        g.fillAll(ZLInterface::BackgroundColor);
        auto offset = static_cast<int>(blurRadius * 1.5f);
        auto radius = juce::jmax(juce::roundToInt(blurRadius * 1.5f), 1);
        if (!flip) {
            juce::DropShadow darkShadow(ZLInterface::DarkShadowColor.withAlpha(0.75f), radius,
                                        {-offset, -offset});
            darkShadow.drawForPath(g, mask);
            juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, radius,
                                          {offset, offset});
            brightShadow.drawForPath(g, mask);
        } else {
            juce::DropShadow brightShadow(ZLInterface::DarkShadowColor, radius,
                                          {offset, offset});
            brightShadow.drawForPath(g, mask);
            juce::DropShadow darkShadow(ZLInterface::BrightShadowColor.withAlpha(0.75f), radius,
                                        {-offset, -offset});
            darkShadow.drawForPath(g, mask);
        }
        boxBounds = boxBounds.withSizeKeepingCentre(
                boxBounds.getWidth() - 0.75f * static_cast<float>(radius),
                boxBounds.getHeight() - 0.75f * static_cast<float>(radius));
        juce::Path path;
        path.addRoundedRectangle(boxBounds.getX(), boxBounds.getY(),
                                 boxBounds.getWidth(), boxBounds.getHeight(),
                                 cornerSize, cornerSize,
                                 curveTopLeft, curveTopRight,
                                 curveBottomLeft, curveBottomRight);

        juce::DropShadow backShadow(ZLInterface::BackgroundColor, radius,
                                    {0, 0});
        backShadow.drawForPath(g, path);
        g.restoreState();
        return boxBounds;
    }

    static juce::Rectangle<float> drawShadowEllipse(juce::Graphics &g,
                                                    juce::Rectangle<float> boxBounds,
                                                    float cornerSize,
                                                    juce::Colour mainColour = ZLInterface::BackgroundColor,
                                                    bool fit = true,
                                                    bool drawBright = true, bool drawDark = true) {
        juce::Path path;
        auto radius = juce::jmax(juce::roundToInt(cornerSize * 0.75f), 1);
        if (fit) {
            boxBounds = boxBounds.withSizeKeepingCentre(
                    boxBounds.getWidth() - static_cast<float>(radius) - 1.5f * cornerSize,
                    boxBounds.getHeight() - static_cast<float>(radius) - 1.5f * cornerSize);
        }
        path.addEllipse(boxBounds);
        auto offset = static_cast<int>(cornerSize * 0.5f);
        if (drawDark) {
            juce::DropShadow darkShadow(ZLInterface::DarkShadowColor, radius,
                                        {offset, offset});
            darkShadow.drawForPath(g, path);
        }
        if (drawBright) {
            juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, radius,
                                          {-offset, -offset});
            brightShadow.drawForPath(g, path);
        }
        g.setColour(mainColour);
        g.fillPath(path);
        return boxBounds;
    }

    static juce::Rectangle<float> drawInnerShadowEllipse(juce::Graphics &g,
                                                         juce::Rectangle<float> boxBounds,
                                                         float cornerSize,
                                                         bool flip = false) {
        juce::Path mask;
        mask.addEllipse(boxBounds);
        g.saveState();
        g.reduceClipRegion(mask);
        g.fillAll(ZLInterface::BackgroundColor);
        auto radius = juce::jmax(juce::roundToInt(cornerSize * 1.5f), 1);
        auto offset = static_cast<int>(cornerSize * 1.f);
        if (!flip) {
            juce::DropShadow darkShadow(ZLInterface::DarkShadowColor.withAlpha(0.75f), radius,
                                        {-offset, -offset});
            darkShadow.drawForPath(g, mask);
            juce::DropShadow brightShadow(ZLInterface::BrightShadowColor, radius,
                                          {offset, offset});
            brightShadow.drawForPath(g, mask);
        } else {
            juce::DropShadow brightShadow(ZLInterface::DarkShadowColor, radius,
                                          {offset, offset});
            brightShadow.drawForPath(g, mask);
            juce::DropShadow darkShadow(ZLInterface::BrightShadowColor.withAlpha(0.75f), radius,
                                        {-offset, -offset});
            darkShadow.drawForPath(g, mask);
        }
        boxBounds = boxBounds.withSizeKeepingCentre(
                boxBounds.getWidth() - 0.75f * static_cast<float>(radius),
                boxBounds.getHeight() - 0.75f * static_cast<float>(radius));
        juce::Path path;
        path.addEllipse(boxBounds);

        juce::DropShadow backShadow(ZLInterface::BackgroundColor, radius,
                                    {0, 0});
        backShadow.drawForPath(g, path);
        g.restoreState();
        return boxBounds;
    }

    static std::string formatFloat(float x, int precision) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << x;
        return stream.str();
    }
}

#endif //ZLINFLATOR_INTERFACE_DEFINES_H
