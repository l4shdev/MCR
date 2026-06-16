#include "MacroManager.hpp"
#include <fstream>

MacroManager& MacroManager::get() {
    static MacroManager instance;
    return instance;
}

void MacroManager::recordInput(int frame, int button, bool player2, bool down) {
    if (m_state != BotState::Recording) return;
    m_inputs.push_back({frame, button, player2, down});
}

void MacroManager::handlePlayback(int frame) {
    if (m_state != BotState::Playing) return;
    
    auto pl = PlayLayer::get();
    if (!pl) return;

    while (m_currentInputIndex < m_inputs.size() && m_inputs[m_currentInputIndex].frame <= frame) {
        auto& input = m_inputs[m_currentInputIndex];
        pl->handleButton(input.down, input.button, input.player2);
        m_currentInputIndex++;
    }
}

void MacroManager::startRecording() {
    clear();
    m_state = BotState::Recording;
}

void MacroManager::stopRecording() {
    m_state = BotState::None;
}

void MacroManager::startPlaying() {
    m_state = BotState::Playing;
    m_currentInputIndex = 0;
}

void MacroManager::stopPlaying() {
    m_state = BotState::None;
}

void MacroManager::clear() {
    m_inputs.clear();
    m_currentInputIndex = 0;
}

bool MacroManager::save(std::filesystem::path path) {
    std::ofstream file(path);
    if (!file.is_open()) return false;

    for (const auto& input : m_inputs) {
        file << input.serialize() << "\n";
    }
    file.close();
    return true;
}

bool MacroManager::load(std::filesystem::path path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        m_inputs.push_back(MacroInput::deserialize(line));
    }
    file.close();
    return true;
}
