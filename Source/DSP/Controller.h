#ifndef ZLLMATCH_CONTROLLER_H
#define ZLLMATCH_CONTROLLER_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"

#include "Matcher.h"
#include "Monitor.h"
#include "dsp_defines.h"

template<typename FloatType>
class Controller {
public:
    explicit Controller(juce::AudioProcessor *processor,
                        juce::AudioProcessorValueTreeState &parameters) {
        m_processor = processor;
        apvts = &parameters;
        reset();
    }

    void prepare(const juce::dsp::ProcessSpec &spec) {
        mainMonitor.prepare(spec);
        targetMonitor.prepare(spec);
    }

    void reset() {
        matcher.reset();
        mainMonitor.reset();
        targetMonitor.reset();
        gain = ZLDsp::gain::defaultV;
    }

    void process(juce::AudioBuffer<float> &buffer) noexcept {
        matcher.setPosition(m_processor->getPlayHead()->getPosition()->getTimeInSamples().orFallback(0));
        if (toReset.load()) {
            reset();
            toReset.store(false);
        }
        if (modeID == ZLDsp::mode::learn && m_processor->getPlayHead()->getPosition()->getIsPlaying()) {
            mainMonitor.process(m_processor->getBusBuffer(buffer, true, 0),
                                gate.load());
            if (sideID == ZLDsp::side::aux && m_processor->getBusCount(true) >= 2) {
                targetMonitor.process(m_processor->getBusBuffer(buffer, true, 1),
                                      gate.load());
            }
            if (periodID == ZLDsp::period::whole) {
                return;
            }
            if (learning) {
                if (mainMonitor.getBufferPeak() < gate) {
                    learnDiff();
                    mainMonitor.reset();
                    targetMonitor.reset();
                    learning = false;
                }
            } else {
                if (mainMonitor.getLoudness()[static_cast<size_t>(loudnessID.load())] > gate) {
                    learning = true;
                }
            }
        } else if (modeID == ZLDsp::mode::effect) {
            gain = matcher.getDiff();
            auto currentGain = juce::Decibels::decibelsToGain(gain * strength);
            auto mainBuffer = m_processor->getBusBuffer(buffer, true, 0);
            mainBuffer.applyGain(static_cast<float>(currentGain));
        } else if (modeID == ZLDsp::mode::envelope) {
            auto currentGain = juce::Decibels::decibelsToGain(gain * strength);
            auto mainBuffer = m_processor->getBusBuffer(buffer, true, 0);
            mainBuffer.applyGain(static_cast<float>(currentGain));
        }
    }

    void setStrength(FloatType v) { strength = v; }

    void setGate(FloatType v) { gate = v; }

    void setTarget(FloatType v) {
        target = v;
        matcher.setTarget(v);
    }

    void setModeID(int ID) {
        if (modeID == ZLDsp::mode::learn && ID == ZLDsp::mode::effect &&
            periodID == 0) {
            matcher.setPosition(std::numeric_limits<int64_t>::max());
            learnDiff();
            mainMonitor.reset();
            targetMonitor.reset();
        }
        if (ID == ZLDsp::mode::learn) {
            toReset = true;
        }
        modeID = ID;
    }

    void setCeil(bool f) {
        ceil = f;
        matcher.setCeil(ceil);
    }

    void setBound(FloatType v) {
        bound = v;
        matcher.setBound(v);
    }

    void setLoudnessID(int ID) {
        loudnessID = ID;
        matcher.setLoudnessID(ID);
    }

    void setPeriodID(int ID) { periodID = ID; }

    void setSideID(int ID) {
        sideID = ID;
        matcher.setSideID(ID);
    }

    void setGain(FloatType v) { gain = v; }

    FloatType getGain() { return gain.load(); }

    juce::String toString() { return matcher.toString(); }

    void fromString(const juce::String &str) { matcher.fromString(str); }

private:
    juce::AudioProcessor *m_processor;
    juce::AudioProcessorValueTreeState *apvts;

    Monitor<FloatType> mainMonitor, targetMonitor;
    Matcher<FloatType> matcher;
    std::atomic<bool> learning = false, toReset = false;
    std::atomic<FloatType> gain = ZLDsp::gain::defaultV;

    std::atomic<FloatType> strength =
            ZLDsp::strength::formatV(ZLDsp::strength::defaultV);
    std::atomic<FloatType> gate = ZLDsp::gate::defaultV;
    std::atomic<FloatType> target = ZLDsp::target::defaultV;
    std::atomic<FloatType> bound = ZLDsp::bound::defaultV;

    std::atomic<bool> ceil = ZLDsp::ceil::defaultV;

    std::atomic<int> modeID = ZLDsp::mode::defaultI;
    std::atomic<int> loudnessID = ZLDsp::loudness::defaultI;
    std::atomic<int> periodID = ZLDsp::period::defaultI;
    std::atomic<int> sideID = ZLDsp::side::defaultI;

    void learnDiff() {
        if (sideID == ZLDsp::side::aux && m_processor->getBusCount(true) >= 2) {
            matcher.learnDiff(mainMonitor.getLoudness(),
                              targetMonitor.getLoudness(),
                              mainMonitor.getPeak());
        } else {
            matcher.learnDiff(mainMonitor.getLoudness(),
                              ZLDsp::loudness::getEmptyLoudness<FloatType>(),
                              mainMonitor.getPeak());
        }
    }
};

template<typename FloatType>
class ControllerAttach : public juce::AudioProcessorValueTreeState::Listener,
                         private juce::Timer {
public:
    explicit ControllerAttach(Controller<FloatType> &gainController,
                              juce::AudioProcessorValueTreeState &parameters) {
        controller = &gainController;
        apvts = &parameters;
        addListeners();
    }

    ~ControllerAttach() override { stopTimer(); }

    void timerCallback() override {
        if (modeID == ZLDsp::mode::effect) {
            apvts->getParameter(ZLDsp::gain::ID)->beginChangeGesture();
            apvts->getParameter(ZLDsp::gain::ID)
                    ->setValueNotifyingHost(
                            ZLDsp::gain::range.convertTo0to1(controller->getGain()));
            apvts->getParameter(ZLDsp::gain::ID)->endChangeGesture();
        }
    }

    void addListeners() {
        std::array IDs{ZLDsp::loudness::ID, ZLDsp::period::ID, ZLDsp::side::ID,
                       ZLDsp::mode::ID, ZLDsp::ceil::ID, ZLDsp::strength::ID,
                       ZLDsp::gate::ID, ZLDsp::target::ID, ZLDsp::bound::ID,
                       ZLDsp::gain::ID};
        for (auto &ID: IDs) {
            apvts->addParameterListener(ID, this);
        }
    }

    void parameterChanged(const juce::String &parameterID,
                          float newValue) override {
        if (parameterID == ZLDsp::loudness::ID) {
            controller->setLoudnessID(static_cast<int>(newValue));
        } else if (parameterID == ZLDsp::period::ID) {
            controller->setPeriodID(static_cast<int>(newValue));
        } else if (parameterID == ZLDsp::side::ID) {
            controller->setSideID(static_cast<int>(newValue));
        } else if (parameterID == ZLDsp::mode::ID) {
            modeID = static_cast<int>(newValue);
            if (modeID == ZLDsp::mode::effect) {
                startTimerHz(60);
            } else {
                stopTimer();
            }
            controller->setModeID(static_cast<int>(newValue));
        } else if (parameterID == ZLDsp::ceil::ID) {
            controller->setCeil(static_cast<bool>(newValue));
        } else if (parameterID == ZLDsp::strength::ID) {
            controller->setStrength(
                    ZLDsp::strength::formatV(static_cast<FloatType>(newValue)));
        } else if (parameterID == ZLDsp::gate::ID) {
            controller->setGate(static_cast<FloatType>(newValue));
        } else if (parameterID == ZLDsp::target::ID) {
            controller->setTarget(static_cast<FloatType>(newValue));
        } else if (parameterID == ZLDsp::bound::ID) {
            controller->setBound(static_cast<FloatType>(newValue));
        } else if (parameterID == ZLDsp::gain::ID) {
            if (modeID == ZLDsp::mode::envelope) {
                controller->setGain(static_cast<FloatType>(newValue));
            }
        }
    }

private:
    Controller<FloatType> *controller;
    juce::AudioProcessorValueTreeState *apvts;
    std::atomic<int> modeID;
};

#endif // ZLLMATCH_CONTROLLER_H
