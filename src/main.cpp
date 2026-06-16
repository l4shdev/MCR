#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "MacroManager.hpp"
#include "MCRLayer.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    CCSprite* m_mcrButton = nullptr;

    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        if (!PlayLayer::init(level, useReplay, dontSave)) return false;
        
        // Reset state when entering level
        if (MacroManager::get().isPlaying()) {
            MacroManager::get().startPlaying(); // Reset index
        }

        // Add floating button for MCR menu
        auto winSize = CCDirector::get()->getWinSize();
        m_mcrButton = CCSprite::create("mcr_button.png");
        m_mcrButton->setScale(0.1f); // Adjust scale for floating button
        auto mcrMenu = CCMenu::create();
        auto mcrMenuItem = CCMenuItemSpriteExtra::create(
            m_mcrButton,
            this,
            menu_selector(MyPlayLayer::onMCRMenu)
        );
        mcrMenuItem->setPosition({winSize.width / 2 - 200, winSize.height / 2 - 120}); // Bottom-left corner
        mcrMenu->addChild(mcrMenuItem);
        mcrMenu->setPosition({0,0});
        this->addChild(mcrMenu, 100);
        
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

    void onMCRMenu(CCObject*) {
        MCRLayer::create()->show();
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        
        // Remove the MCR button from PauseLayer as it's now in PlayLayer
        auto menu = this->getChildByID("center-menu");
        if (menu) {
            menu->removeChildByID("mcr-button");
            menu->updateLayout();
        }
    }
};
