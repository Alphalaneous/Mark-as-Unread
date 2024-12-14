#include <Geode/Geode.hpp>
#include <Geode/modify/GJMessageCell.hpp>
#include <Geode/modify/GJMessagePopup.hpp>
#include <Geode/modify/GJRequestCell.hpp>
#include <Geode/modify/FriendRequestPopup.hpp>

using namespace geode::prelude;

class $modify(GJRequestCell) {

    void loadFromScore(GJUserScore* p0) {
		if (Mod::get()->getSavedValue<bool>(fmt::format("unread-friend-{}", p0->m_accountID), false)) {
			p0->m_newFriendRequest = true;
		}
		GJRequestCell::loadFromScore(p0);
	}
};

class $modify(MyFriendRequestPopup, FriendRequestPopup) {

	struct Fields {
		GJFriendRequest* m_friendRequest;
	};

    bool init(GJFriendRequest* p0) {
		if (!FriendRequestPopup::init(p0)) return false;

		m_fields->m_friendRequest = p0;
		if (Mod::get()->hasSavedValue(fmt::format("unread-friend-{}", p0->m_accountID))) {
			Mod::get()->setSavedValue(fmt::format("unread-friend-{}", p0->m_accountID), false);
		}

		CCMenu* unreadButtonMenu = CCMenu::create();
		unreadButtonMenu->setContentSize({60, 24});
		unreadButtonMenu->setPositionY(unreadButtonMenu->getPositionY() + 65);
		unreadButtonMenu->setPositionX(unreadButtonMenu->getPositionX() + 132);
		unreadButtonMenu->setID("unread-menu"_spr);

		RowLayout* layout = RowLayout::create();

		unreadButtonMenu->setLayout(layout);

		ButtonSprite* spr = ButtonSprite::create("Unread", 60, 10, 2, false, "bigFont.fnt", "GJ_button_06.png", 25);

		CCMenuItemSpriteExtra* unreadButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyFriendRequestPopup::markAsUnread));
		unreadButton->setID("unread-button"_spr);

		unreadButtonMenu->addChild(unreadButton);

		unreadButtonMenu->updateLayout();

		m_mainLayer->addChild(unreadButtonMenu);

		handleTouchPriority(this);

		return true;
	}

	void markAsUnread(CCObject* sender) {
		Mod::get()->setSavedValue(fmt::format("unread-friend-{}", m_fields->m_friendRequest->m_accountID), true);
	}
};

class $modify(GJMessageCell) {

    void loadFromMessage(GJUserMessage* p0) {
		if (Mod::get()->getSavedValue<bool>(fmt::format("unread-message-{}", p0->m_messageID), false)) {
			p0->m_read = false;
		}
		GJMessageCell::loadFromMessage(p0);
	}
};

class $modify(MyGJMessagePopup, GJMessagePopup) {

	struct Fields {
		GJUserMessage* m_message;
	};

    void loadFromGJMessage(GJUserMessage* p0) {
		GJMessagePopup::loadFromGJMessage(p0);
		m_fields->m_message = p0;
		if (Mod::get()->hasSavedValue(fmt::format("unread-message-{}", p0->m_messageID))) {
			Mod::get()->setSavedValue(fmt::format("unread-message-{}", p0->m_messageID), false);
		}

		CCMenu* unreadButtonMenu = CCMenu::create();
		unreadButtonMenu->setContentSize({60, 24});
		unreadButtonMenu->setPositionY(unreadButtonMenu->getPositionY() + 90);
		unreadButtonMenu->setPositionX(unreadButtonMenu->getPositionX() + 170);
		unreadButtonMenu->setID("unread-menu"_spr);

		RowLayout* layout = RowLayout::create();

		unreadButtonMenu->setLayout(layout);

		ButtonSprite* spr = ButtonSprite::create("Unread", 60, 10, 2, false, "bigFont.fnt", "GJ_button_06.png", 25);

		CCMenuItemSpriteExtra* unreadButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyGJMessagePopup::markAsUnread));
		unreadButton->setID("unread-button"_spr);
		unreadButtonMenu->addChild(unreadButton);

		unreadButtonMenu->updateLayout();

		m_mainLayer->addChild(unreadButtonMenu);

		handleTouchPriority(this);

	}

	void markAsUnread(CCObject* sender) {
		Mod::get()->setSavedValue(fmt::format("unread-message-{}", m_fields->m_message->m_messageID), true);
	}
};