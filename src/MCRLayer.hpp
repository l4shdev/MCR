#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class MCRLayer : public FLAlertLayer {
protected:
    bool init() override;
    void onRecord(CCObject*);
    void onPlay(CCObject*);
    void onLoad(CCObject*);
    void onSave(CCObject*);

public:
    static MCRLayer* create();
    void show();
};
