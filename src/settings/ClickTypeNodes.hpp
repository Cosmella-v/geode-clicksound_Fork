#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "ClickTypes.hpp"
using namespace geode::prelude;

CCNode* createCheckboxButtonTexture(std::string text, bool isSelect){
    auto label = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
    if (isSelect){label->setString(fmt::format("{} + on", text).c_str());}
    label->setScale(.33F);
    return static_cast<CCNode*>(label);
}


// STOLE FROM VIPER'S BETTER MENU BUT CUSTOMIZED BY COOPER 
class ClickTypeNode : public SettingNode {
protected:
    int m_currentPos;
    CCMenuItemToggler* usefulBtn;
    CCMenuItemToggler* memeBtn;
    CCMenuItemToggler* customBtn;

    int getActiveCornerTag(int corner) {
        switch (corner) {
            case 1: // useful
                return 2004;
            case 2: // meme
                return 2005;
            case 3: // custom
                return 2006;
            default:
                return 2007;
        }
    }
    int tagToCorner(int tag) {
        switch (tag) {
            case 2004: // useful
                return 1;
            case 2005: // meme
                return 2;
            case 2006: // custom
                return 3;
            default:
                return 4;
        }
    }
    template <typename T>
    bool init(T* value, float width, std::string prefixText = "Unknown [Mod Developer didn't put anything here]") {
        if (!SettingNode::init(value))
            return false;

       
        m_currentPos = value->getType();
        this->setContentSize({ width, 35.f });
        auto layer = CCLayer::create();
        layer->setScale(1);
        layer->setContentSize({ (width/2), 35.f });
        layer->setPosition(width/2, 0);

        auto menu = CCMenu::create();
        //menu->setScale(1);
        menu->setContentSize({ width, 15.f} );
        menu->setPosition(width/2, 17.5f);
        menu->setLayout(RowLayout::create()
      			->setGap(20.f)
      			->setAxisAlignment(AxisAlignment::Center)
      			->setAutoScale(false)
      			->setCrossAxisOverflow(true)
    	);

        // stole from geode code cause there's no docs on how CCScale9Sprite works
        auto bgSelector = cocos2d::extension::CCScale9Sprite::create(
            "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
        );
        bgSelector->setColor({ 71, 39, 10 });
        bgSelector->setOpacity(255);
        bgSelector->setContentSize({ width, 15.f });
        bgSelector->setPosition(width/2, 17.5f);


        usefulBtn = CCMenuItemToggler::create(
            createCheckboxButtonTexture("Useful", true),
            createCheckboxButtonTexture("Useful", false),
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );
        memeBtn = CCMenuItemToggler::create(
            createCheckboxButtonTexture("Meme", true),
            createCheckboxButtonTexture("Meme", false),
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );
        customBtn = CCMenuItemToggler::create(
            createCheckboxButtonTexture("Custom", true),
            createCheckboxButtonTexture("Custom", false),
            this,
            menu_selector(ClickTypeNode::onCornerClick)
        );

        usefulBtn->setTag(getActiveCornerTag(1));
        memeBtn->setTag(getActiveCornerTag(2));
        customBtn->setTag(getActiveCornerTag(3));
        int currentCorner = m_currentPos;
        usefulBtn->toggle(!(usefulBtn->getTag() == getActiveCornerTag(currentCorner)));
        memeBtn->toggle(!(memeBtn->getTag() == getActiveCornerTag(currentCorner)));
        customBtn->toggle(!(customBtn->getTag() == getActiveCornerTag(currentCorner)));
        
        layer->addChild(bgSelector);
        layer->addChild(menu);
        menu->addChild(usefulBtn);
        menu->addChild(memeBtn);
        menu->addChild(customBtn);

        auto label = CCLabelBMFont::create(fmt::format("{} Type",prefixText).c_str(), "bigFont.fnt");
        label->setScale(0.750);
        label->setPositionX(94);
        label->setPositionY(17.5f);
        label->setScale(.6F);

        this->addChild(label);
        this->addChild(layer);
        menu->updateLayout();
        return true;
    }
    void onCornerClick(CCObject* sender) {
        usefulBtn->toggle(true);
        memeBtn->toggle(true);
        customBtn->toggle(true);
        m_currentPos = tagToCorner(sender->getTag());
        this->dispatchChanged();
    };

public:
    void commit() override {
        static_cast<ClickTypeValue*>(m_value)->setType(m_currentPos);
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
        return m_currentPos != static_cast<ClickTypeValue*>(m_value)->getType();
    }
    bool hasNonDefaultValue() override {
        return m_currentPos != 1;
    }

    // Geode calls this to reset the setting's value back to default
    void resetToDefault() override {
        usefulBtn->toggle(false);
        memeBtn->toggle(true);
        customBtn->toggle(true);
        m_currentPos = 1;
    }
    template <typename T>
    static ClickTypeNode* create(T* value, float width, std::string prefixText = "?") {
        auto ret = new ClickTypeNode;
        if (ret && ret->init(value, width, prefixText)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
