#include "menu_layer.hpp"
#include "cocos_scene.hpp"
#include "game_layer.hpp"
#include "game_controller.hpp"

USING_NS_CC;

menu_layer_t::menu_layer_t()
{
    init();
}

void menu_layer_t::start()
{
    // add layer as a child to scene
    master_t::subsystem<cocos_scene_t>().addChild(this, 1);
}

void menu_layer_t::stop()
{
    master_t::subsystem<cocos_scene_t>().removeChild(this, false);
}


// on "init" you need to initialize your instance
bool menu_layer_t::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    this->setTouchEnabled(true);

    cocos2d::CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCSprite* pSprite = CCSprite::create("menu_bg.png");
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    this->addChild(pSprite, 0);    
    cocos2d::CCSize bg_size = pSprite->getContentSize();

    CCMenuItemImage *start = CCMenuItemImage::create(
                                        "FireIcon.png",
                                        "FireIcon_p.png",
                                        this,
                                        menu_selector(menu_layer_t::on_start_cb) );
    start->setPosition( ccp(size.width / 3, size.height / 3 * 2) );
    cocos2d::CCLabelTTF *start_l = CCLabelTTF::create("Start", "Arial", 28);
    start_l->setPosition( ccp(size.width / 3, size.height / 3 * 2 - 52) );
    this->addChild(start_l, 1);

    CCMenuItemImage *exit = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(menu_layer_t::on_exit_cb) );
    exit->setPosition( ccp(size.width / 3 * 2, size.height / 3 * 2) );
    cocos2d::CCLabelTTF *exit_l = CCLabelTTF::create("Exit", "Arial", 28);
    exit_l->setPosition( ccp(size.width / 3 * 2, size.height / 3 * 2 - 52) );
    this->addChild(exit_l, 1);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(exit, start, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    _input_password.create_input(this, ccp(size.width/2, size.height/2 - bg_size.height / 2 + 24), CCSize(200, 24), kCCTextAlignmentLeft, "<password>", "Arial", 20);

    _input_name.create_input(this, ccp(size.width/2, size.height/2 - bg_size.height / 2 + 64), CCSize(200, 24), kCCTextAlignmentLeft, "<user name>", "Arial", 20);

    cocos2d::CCLabelTTF *caption = CCLabelTTF::create("Menu", "Arial", 36);
    caption->setPosition( ccp(size.width / 2, size.height / 2 + bg_size.height / 2 - 22) );
    this->addChild(caption, 1);

    return true;
}

void menu_layer_t::on_exit_cb(CCObject* pSender)
{
    master_t::subsystem<cocos_scene_t>().end_scene();
}

void menu_layer_t::on_start_cb(CCObject* pSender)
{
    hide();
    master_t::subsystem<game_layer_t>().show();
    master_t::subsystem<game_controller_t>().start_game(_input_name.get_string(), _input_password.get_string());
}

void menu_layer_t::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

bool menu_layer_t::ccTouchBegan(CCTouch *touch, CCEvent *pEvent)
{
    _touch_begin = touch->locationInView();	
    _touch_begin = CCDirector::sharedDirector()->convertToGL(_touch_begin);

    return true;
}

void menu_layer_t::ccTouchEnded(CCTouch *touch, CCEvent *pEvent)
{
    CCPoint touch_end = touch->locationInView();
    touch_end = CCDirector::sharedDirector()->convertToGL(touch_end);

    float delta = 5.0f;
    if (::abs(touch_end.x - _touch_begin.x) > delta || ::abs(touch_end.y - _touch_begin.y) > delta)
    {
        _touch_begin.x = _touch_begin.y = -1;
        return;
    }

    _input_name.on_click(touch_end);
    _input_password.on_click(touch_end);
}

void menu_layer_t::show()
{
    this->setTouchEnabled(true);
    this->setVisible(true);
}

void menu_layer_t::hide()
{
    this->setTouchEnabled(false);
    this->setVisible(false);
}

