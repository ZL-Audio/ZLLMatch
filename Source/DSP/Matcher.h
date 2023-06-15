#ifndef ZLLMATCH_MATCHER_H
#define ZLLMATCH_MATCHER_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_dsp/juce_dsp.h"
#include <limits>

#include "Monitor.h"
#include "dsp_defines.h"

template <typename FloatType>
struct GainPos {
    FloatType gain;
    FloatType peak;
    int64_t position;
};

template <typename FloatType>
class Matcher {
public:
    Matcher() = default;

    void reset() {
        diffs.clear();
        diffs.push_back ({ 0, 0, m_position });
        prev_id = 0;
    }

    void setPosition (int64_t pos) {
        m_position = pos;
    }

    void setCeil (bool f) {
        ceil = f;
    }

    void setBound (FloatType v) {
        bound = v;
    }

    void learnDiff (FloatType mainL, FloatType targetL, FloatType mainPeak) {
        auto gain = targetL - mainL;
        diffs.push_back ({ gain, mainPeak, m_position });
    }

    FloatType getDiff() {
        if (m_position < diffs.front().position || m_position >= diffs.back().position) {
            return static_cast<FloatType> (0);
        }
        for (size_t shift_id = 0; shift_id < diffs.size(); ++shift_id) {
            auto index_id = (prev_id + shift_id) % diffs.size();
            auto left_id = (prev_id + shift_id - 1) % diffs.size();
            if (m_position < diffs[index_id].position && m_position >= diffs[left_id].position) {
                prev_id = index_id;
                auto gain = diffs[index_id].gain;
                gain = juce::jlimit (-bound.load(), bound.load(), gain);
                if (ceil) {
                    gain = juce::jmin (gain, -diffs[index_id].peak);
                }
                return gain;
            }
        }
        return static_cast<FloatType> (0);
    }

    void dataToFiles() {
        juce::File resourceFile = juce::File ("/Volumes/Ramdisk/test.txt");
        if (auto output = std::unique_ptr<juce::FileOutputStream> (resourceFile.createOutputStream())) {
            output->setPosition (0);
            output->truncate();
            for (size_t i = 0; i < diffs.size(); ++i) {
                output->writeText (juce::String (diffs[i].position) + " " + juce::String (diffs[i].gain) + "\n", false, false, nullptr);
            }
        }
    }

private:
    std::atomic<int64_t> m_position = 0;
    size_t prev_id = 0;
    std::vector<GainPos<FloatType>> diffs;

    std::atomic<bool> ceil = ZLDsp::ceil::defaultV;
    std::atomic<FloatType> bound = ZLDsp::bound::defaultV;
};

#endif //ZLLMATCH_MATCHER_H
