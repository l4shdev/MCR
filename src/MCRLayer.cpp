#include "MCRLayer.hpp"
#include "MacroManager.hpp"

bool MCRLayer::init() {
    if (!FLAlertLayer::init(150)) return false;

    auto winSize = CCDirector::get()->getWinSize();
    
    auto bg = CCScale9Sprite::create("GJ_square01.png");
    bg->setContentSize({300, 200});
    bg->setPosition(winSize / 2);
    m_mainLayer->addChild(bg);

    auto menu = CCMenu::create();
    m_mainLayer->addChild(menu);

    auto title = CCLabelBMFont::create("MCR Bot", "goldFont.fnt");
    title->setPosition({winSize.width / 2, winSize.height / 2 + 80});
    m_mainLayer->addChild(title);

    auto credit = CCLabelBMFont::create("by l4shdev", "chatFont.fnt");
    credit->setScale(0.6f);
    credit->setPosition({winSize.width / 2, winSize.height / 2 + 60});
    m_mainLayer->addChild(credit);

    // Buttons
    auto recordBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Record"),
        this, menu_selector(MCRLayer::onRecord)
    );
    recordBtn->setPosition({-70, 20});
    menu->addChild(recordBtn);

    auto playBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Play"),
        this, menu_selector(MCRLayer::onPlay)
    );
    playBtn->setPosition({70, 20});
    menu->addChild(playBtn);

    auto loadBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Load"),
        this, menu_selector(MCRLayer::onLoad)
    );
    loadBtn->setPosition({-70, -40});
    menu->addChild(loadBtn);

    auto saveBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Save"),
        this, menu_selector(MCRLayer::onSave)
    );
    saveBtn->setPosition({70, -40});
    menu->addChild(saveBtn);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Close"),
        this, menu_selector(MCRLayer::onClose)
    );
    closeBtn->setPosition({0, -80});
    menu->addChild(closeBtn);

    return true;
}

void MCRLayer::onRecord(CCObject*) {
    auto& mgr = MacroManager::get();
    if (mgr.isRecording()) {
        mgr.stopRecording();
        FLAlertLayer::create("MCR", "Recording Stopped", "OK")->show();
    } else {
        mgr.startRecording();
        FLAlertLayer::create("MCR", "Recording Started", "OK")->show();
    }
}

void MCRLayer::onPlay(CCObject*) {
    auto& mgr = MacroManager::get();
    if (mgr.isPlaying()) {
        mgr.stopPlaying();
        FLAlertLayer::create("MCR", "Playback Stopped", "OK")->show();
    } else {
        mgr.startPlaying();
        FLAlertLayer::create("MCR", "Playback Started", "OK")->show();
    }
}

void MCRLayer::onLoad(CCObject*) {
    utils::file::pickFile(
        utils::file::PickMode::OpenFile,
        { .filters = { { .description = "MCR Macro", .extensions = { "*.mcr" } } } },
        [](std::filesystem::path path) {
            if (MacroManager::get().load(path)) {
                FLAlertLayer::create("MCR", "Macro Loaded!", "OK")->show();
            } else {
                FLAlertLayer::create("MCR", "Failed to load macro", "OK")->show();
            }
        }
    );
}

void MCRLayer::onSave(CCObject*) {
    utils::file::pickFile(
        utils::file::PickMode::SaveFile,
        { .filters = { { .description = "MCR Macro", .extensions = { "*.mcr" } } } },
        [](std::filesystem::path path) {
            if (MacroManager::get().save(path)) {
                FLAlertLayer::create("MCR", "Macro Saved!", "OK")->show();
            } else {
                FLAlertLayer::create("MCR", "Failed to save macro", "OK")->show();
            }
        }
    );
}

MCRLayer* MCRLayer::create() {
    auto ret = new MCRLayer();
    if (ret && ret.init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void MCRLayer::show() {
    FLAlertLayer::show();
}
