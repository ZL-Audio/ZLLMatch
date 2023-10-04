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
        gain = zldsp::gain::defaultV;
    }

    void process(juce::AudioBuffer<float> &buffer) noexcept {
        matcher.setPosition(m_processor->getPlayHead()->getPosition()->getTimeInSamples().orFallback(0));
        if (toReset.load()) {
            reset();
            toReset.store(false);
        }
        auto _modeID = modeID.load();
        auto _gate = gate.load();
        if (_modeID == zldsp::mode::learn && m_processor->getPlayHead()->getPosition()->getIsPlaying()) {
            mainMonitor.process(m_processor->getBusBuffer(buffer, true, 0),
                                _gate);
            if (sideID == zldsp::side::aux && m_processor->getBusCount(true) >= 2) {
                targetMonitor.process(m_processor->getBusBuffer(buffer, true, 1),
                                      _gate);
            }
            if (periodID == zldsp::period::whole) {
                return;
            }
            if (learning) {
                if (mainMonitor.getBufferPeak() < _gate) {
                    learnDiff();
                    mainMonitor.reset();
                    targetMonitor.reset();
                    learning = false;
                }
            } else {
                if (mainMonitor.getLoudness()[static_cast<size_t>(loudnessID.load())] > _gate) {
                    learning = true;
                }
            }
        } else if (_modeID == zldsp::mode::effect) {
            gain.store(matcher.getDiff());
            auto currentGain = juce::Decibels::decibelsToGain(gain.load() * strength.load());
            auto mainBuffer = m_processor->getBusBuffer(buffer, true, 0);
            mainBuffer.applyGain(static_cast<float>(currentGain));
        } else if (_modeID == zldsp::mode::envelope) {
            auto currentGain = juce::Decibels::decibelsToGain(gain.load() * strength.load());
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
        if (modeID == zldsp::mode::learn && ID == zldsp::mode::effect &&
            periodID == 0) {
            matcher.setPosition(std::numeric_limits<int64_t>::max());
            learnDiff();
            mainMonitor.reset();
            targetMonitor.reset();
        }
        if (ID == zldsp::mode::learn) {
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
    std::atomic<FloatType> gain = zldsp::gain::defaultV;

    std::atomic<FloatType> strength =
            zldsp::strength::formatV(zldsp::strength::defaultV);
    std::atomic<FloatType> gate = zldsp::gate::defaultV;
    std::atomic<FloatType> target = zldsp::target::defaultV;
    std::atomic<FloatType> bound = zldsp::bound::defaultV;

    std::atomic<bool> ceil = zldsp::ceil::defaultV;

    std::atomic<int> modeID = zldsp::mode::defaultI;
    std::atomic<int> loudnessID = zldsp::loudness::defaultI;
    std::atomic<int> periodID = zldsp::period::defaultI;
    std::atomic<int> sideID = zldsp::side::defaultI;

    void learnDiff() {
        if (sideID == zldsp::side::aux && m_processor->getBusCount(true) >= 2) {
            matcher.learnDiff(mainMonitor.getLoudness(),
                              targetMonitor.getLoudness(),
                              mainMonitor.getPeak());
        } else {
            matcher.learnDiff(mainMonitor.getLoudness(),
                              zldsp::loudness::getEmptyLoudness<FloatType>(),
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
        if (modeID == zldsp::mode::effect) {
            apvts->getParameter(zldsp::gain::ID)->beginChangeGesture();
            apvts->getParameter(zldsp::gain::ID)
                    ->setValueNotifyingHost(
                            zldsp::gain::range.convertTo0to1(controller->getGain()));
            apvts->getParameter(zldsp::gain::ID)->endChangeGesture();
        }
    }

    void addListeners() {
        std::array IDs{zldsp::loudness::ID, zldsp::period::ID, zldsp::side::ID,
                       zldsp::mode::ID, zldsp::ceil::ID, zldsp::strength::ID,
                       zldsp::gate::ID, zldsp::target::ID, zldsp::bound::ID,
                       zldsp::gain::ID};
        for (auto &ID: IDs) {
            apvts->addParameterListener(ID, this);
        }
    }

    void parameterChanged(const juce::String &parameterID,
                          float newValue) override {
        if (parameterID == zldsp::loudness::ID) {
            controller->setLoudnessID(static_cast<int>(newValue));
        } else if (parameterID == zldsp::period::ID) {
            controller->setPeriodID(static_cast<int>(newValue));
        } else if (parameterID == zldsp::side::ID) {
            controller->setSideID(static_cast<int>(newValue));
        } else if (parameterID == zldsp::mode::ID) {
            modeID = static_cast<int>(newValue);
            if (modeID == zldsp::mode::effect) {
                startTimerHz(60);
            } else {
                stopTimer();
            }
            controller->setModeID(static_cast<int>(newValue));
        } else if (parameterID == zldsp::ceil::ID) {
            controller->setCeil(static_cast<bool>(newValue));
        } else if (parameterID == zldsp::strength::ID) {
            controller->setStrength(
                    zldsp::strength::formatV(static_cast<FloatType>(newValue)));
        } else if (parameterID == zldsp::gate::ID) {
            controller->setGate(static_cast<FloatType>(newValue));
        } else if (parameterID == zldsp::target::ID) {
            controller->setTarget(static_cast<FloatType>(newValue));
        } else if (parameterID == zldsp::bound::ID) {
            controller->setBound(static_cast<FloatType>(newValue));
        } else if (parameterID == zldsp::gain::ID) {
            if (modeID == zldsp::mode::envelope) {
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
