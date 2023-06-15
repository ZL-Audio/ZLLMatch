#include "LogoPanel.h"

LogoPanel::LogoPanel() : logoDrawable (
    juce::Drawable::createFromImageData (BinaryData::logo_svg, BinaryData::logo_svgSize)) {
}

LogoPanel::~LogoPanel() = default;

void LogoPanel::paint (juce::Graphics& g) {
    auto bound = getLocalBounds().toFloat();
    auto padding = juce::jmin (bound.getWidth() * 0.32f, bound.getHeight() * 0.32f);
    bound.removeFromTop(padding * 0.15f);
    bound.removeFromBottom(padding * 0.85f);
    bound.removeFromLeft(padding * 0.15f);
    bound.removeFromRight(padding * 0.85f);
    auto scale = juce::jmin (bound.getWidth() / (float) logoDrawable->getWidth(), bound.getHeight() / (float) logoDrawable->getHeight());
    auto transform = juce::AffineTransform::scale (scale).translated (bound.getX(), bound.getY());
    logoDrawable->setTransform (transform);
    logoDrawable->drawAt (g, bound.getX(), bound.getY(), 1.0f);
}