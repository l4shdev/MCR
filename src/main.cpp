#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "MacroManager.hpp"
#include "MCRLayer.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;
        
        // Reset state when entering level
        if (MacroManager::get().isPlaying()) {
            MacroManager::get().startPlaying(); // Reset index
        }
        
        return true;
    }

    void update(float dt) {
        PlayLayer::update(dt);
        
        int frame = static_cast<int>(m_gameState.m_levelTime * 240); // Simple frame calculation
        MacroManager::get().handlePlayback(frame);
    }

    void handleButton(bool down, int button, bool player2) {
        PlayLayer::handleButton(down, button, player2);
        
        if (MacroManager::get().isRecording()) {
            int frame = static_cast<int>(m_gameState.m_levelTime * 240);
            MacroManager::get().recordInput(frame, button, player2, down);
        }
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        
        auto winSize = CCDirector::get()->getWinSize();
        auto menu = this->getChildByID("center-menu");
        
        auto btnSprite = CircleButtonSprite::createWithSpriteFrameName("GJ_playBtn2_001.png", 0.5f);
        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite,
            this,
            menu_selector(MyPauseLayer::onMCRMenu)
        );
        btn->setID("mcr-button");
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onMCRMenu(CCObject*) {
        MCRLayer::create()->show();
    }
};
