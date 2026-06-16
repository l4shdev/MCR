#pragma once
#include <Geode/Geode.hpp>
#include <vector>
#include <string>
#include <filesystem>

using namespace geode::prelude;

enum class BotState {
    None,
    Recording,
    Playing
};

struct MacroInput {
    int frame;
    int button;
    bool player2;
    bool down;

    // Untuk menyimpan ke file
    std::string serialize() const {
        return fmt::format("{}|{}|{}|{}", frame, button, (int)player2, (int)down);
    }

    static MacroInput deserialize(const std::string& line) {
        auto parts = geode::utils::string::split(line, "|");
        if (parts.size() < 4) return {0, 0, false, false};
        return {
            std::stoi(parts[0]),
            std::stoi(parts[1]),
            (bool)std::stoi(parts[2]),
            (bool)std::stoi(parts[3])
        };
    }
};

class MacroManager {
private:
    std::vector<MacroInput> m_inputs;
    BotState m_state = BotState::None;
    int m_currentInputIndex = 0;

public:
    static MacroManager& get();

    void recordInput(int frame, int button, bool player2, bool down);
    void handlePlayback(int frame);
    
    void startRecording();
    void stopRecording();
    void startPlaying();
    void stopPlaying();

    bool isRecording() const { return m_state == BotState::Recording; }
    bool isPlaying() const { return m_state == BotState::Playing; }
    BotState getState() const { return m_state; }

    void clear();
    bool save(std::filesystem::path path);
    bool load(std::filesystem::path path);
};
