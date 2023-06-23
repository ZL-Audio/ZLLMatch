#ifndef ZLLMATCH_DSP_DEFINES_H
#define ZLLMATCH_DSP_DEFINES_H

#include <juce_audio_processors/juce_audio_processors.h>

namespace ZLDsp {
// floats
    template<class T>
    class FloatParameters {
    public:
        static std::unique_ptr<juce::AudioParameterFloat> get() {
            return std::make_unique<juce::AudioParameterFloat>(T::ID, T::name, T::range,
                                                               T::defaultV);
        }
    };

    class strength : public FloatParameters<strength> {
    public:
        auto static constexpr ID = "strength";
        auto static constexpr name = "Strength (%)";
        inline auto static const range =
                juce::NormalisableRange<float>(0.0f, 100.0f, 1.f);
        auto static constexpr defaultV = 100.0f;

        static float formatV(float v) { return v / 100.f; }
    };

    class gate : public FloatParameters<gate> {
    public:
        auto static constexpr ID = "gate";
        auto static constexpr name = "Gate (dB)";
        inline auto static const range =
                juce::NormalisableRange<float>(-110.f, -10.f, 0.1f);
        auto static constexpr defaultV = -60.f;
    };

    class target : public FloatParameters<target> {
    public:
        auto static constexpr ID = "target";
        auto static constexpr name = "Target";
        inline auto static const range =
                juce::NormalisableRange<float>(-30.f, -6.f, 0.1f);
        auto static constexpr defaultV = -18.f;
    };

    class gain : public FloatParameters<gain> {
    public:
        auto static constexpr ID = "gain";
        auto static constexpr name = "Gain (dB)";
        inline auto static const range =
                juce::NormalisableRange<float>(-30., 30.f, 0.01f);
        auto static constexpr defaultV = 0.f;
    };

    class bound : public FloatParameters<bound> {
    public:
        auto static constexpr ID = "bound";
        auto static constexpr name = "Bound (dB)";
        inline auto static const range =
                juce::NormalisableRange<float>(0., 30.f, 0.1f);
        auto static constexpr defaultV = 30.f;
    };

// bools
    template<class T>
    class BoolParameters {
    public:
        static std::unique_ptr<juce::AudioParameterBool> get() {
            return std::make_unique<juce::AudioParameterBool>(T::ID, T::name,
                                                              T::defaultV);
        }
    };

    class ceil : public BoolParameters<ceil> {
    public:
        auto static constexpr ID = "ceil";
        auto static constexpr name = "Ceil";
        auto static constexpr defaultV = false;
    };

// choices
    template<class T>
    class ChoiceParameters {
    public:
        static std::unique_ptr<juce::AudioParameterChoice> get() {
            return std::make_unique<juce::AudioParameterChoice>(
                    T::ID, T::name, T::choices, T::defaultI);
        }
    };

    class mode : public ChoiceParameters<mode> {
    public:
        auto static constexpr ID = "mode";
        auto static constexpr name = "Mode";
        inline auto static const choices =
                juce::StringArray{"Learn", "Effect", "Envelope"};
        int static constexpr defaultI = 1;
        enum {
            learn, effect, envelope
        };
    };

    class loudness : public ChoiceParameters<loudness> {
    public:
        auto static constexpr ID = "measurement";
        auto static constexpr name = "Measurement";
        inline auto static const choices = juce::StringArray{"RMS", "LUFS", "RMS-G"};
        int static constexpr defaultI = 0;
        enum {
            rms, lufs, rms_g, loudnessNUM
        };

        template<typename FloatType>
        static std::vector<FloatType> getEmptyLoudness() {
            return std::vector<FloatType>(loudnessNUM, -180);
        }
    };

    class period : public ChoiceParameters<period> {
    public:
        auto static constexpr ID = "period";
        auto static constexpr name = "Period";
        inline auto static const choices = juce::StringArray{"Whole", "Segment"};
        int static constexpr defaultI = 0;
        enum {
            whole, segment
        };
    };

    class side : public ChoiceParameters<side> {
    public:
        auto static constexpr ID = "side";
        auto static constexpr name = "Side";
        inline auto static const choices = juce::StringArray{"Value", "Aux"};
        int static constexpr defaultI = 0;
        enum {
            value, aux
        };
    };

    inline juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout() {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        layout.add(strength::get(), gate::get(), target::get(), gain::get(),
                   bound::get(), ceil::get(), mode::get(), period::get(),
                   loudness::get(), side::get());
        return layout;
    }

    template<typename T, size_t elementSize>
    inline juce::String VectorToBase64String(const T &vec) {
        juce::MemoryBlock mb(vec.data(), vec.size() * elementSize);
        return mb.toBase64Encoding();
    }

    template<typename T>
    inline juce::String VectorToBase64String(const std::vector<T> &vec) {
        return VectorToBase64String<std::vector<T>, sizeof(T)>(vec);
    }

    template<typename T>
    inline std::vector<T> Base64StringToVector(const juce::String &str) {
        juce::MemoryBlock mb;
        if (mb.fromBase64Encoding(str)) {
            juce::Array<T> arr(static_cast<T *>(mb.getData()),
                               static_cast<int>(mb.getSize() / sizeof(T)));
            std::vector<T> vec;
            for (auto &a: arr) {
                vec.push_back(a);
            }
            return vec;
        }

        jassertfalse;
        return {};
    }
} // namespace ZLDsp

#endif // ZLLMATCH_DSP_DEFINES_H
