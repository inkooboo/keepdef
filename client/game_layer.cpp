#include "game_layer.hpp"
//#include "game.hpp"
#include "cocos_scene.hpp"
#include "menu_layer.hpp"

USING_NS_CC;

GameLayer::GameLayer()
{
    init();
}

void GameLayer::start()
{
    // add layer as a child to scene
    g_cocos_scene->addChild(this);
}

void GameLayer::stop()
{
    g_cocos_scene->removeChild(this, false);
}


// on "init" you need to initialize your instance
bool GameLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    _size = CCDirector::sharedDirector()->getWinSize();

    CCMenuItemImage *pMenuItem = CCMenuItemImage::create(
                                        "shesterenka.png",
                                        "shesterenka_p.png",
                                        this,
                                        menu_selector(GameLayer::menuOpenCallback) );
    pMenuItem->setPosition( ccp(_size.width - 27, 28) );


    CCMenuItemImage *left_button = CCMenuItemImage::create(
                                        "test_button.png",
                                        "test_button_pressed.png",
                                        0,//&master().subsystem<Game>(),
                                        0 //menu_selector(Game::left_fire) 
                                        );
    left_button->setPosition( ccp(_size.width /2 - 60, 20) );

    CCMenuItemImage *right_button = CCMenuItemImage::create(
                                        "test_button.png",
                                        "test_button_pressed.png",
                                        0,//&master().subsystem<Game>(),
                                        0 //menu_selector(Game::right_fire) 
                                        );
    right_button->setPosition( ccp(_size.width / 2 + 60, 20) );

    // create menu, it's an autorelease object
    _pMenu = CCMenu::create(pMenuItem, left_button, right_button, NULL);
    _pMenu->setPosition( CCPointZero );
    _pMenu->setVisible(false);
    this->addChild(_pMenu, 1);


    _tip = CCLabelTTF::create("", "Arial", 36);
    _tip->setPosition( ccp(_size.width/2, _size.height/2) );
    _tip->setVisible(false);
    this->addChild(_tip, 1);

    _good_tip = CCLabelTTF::create("", "Arial", 36);
    _good_tip->setPosition( ccp(_size.width/2, _size.height/2) );
    _good_tip->setColor(ccc3(100, 100, 255));
    _good_tip->setVisible(false);
    this->addChild(_good_tip, 1);

    _bad_tip = CCLabelTTF::create("", "Arial", 36);
    _bad_tip->setPosition( ccp(_size.width/2, _size.height/2) );
    _bad_tip->setColor(ccc3(255, 100, 100));
    _bad_tip->setVisible(false);
    this->addChild(_bad_tip, 1);

    CCSprite* pSprite = CCSprite::create("background.png");
    pSprite->setPosition( ccp(_size.width/2, _size.height/2) );
    this->addChild(pSprite, 0);    

    _player = CCSprite::create("good_face.png");
    _player->setPosition( ccp(_size.width/2, 20) );
    this->addChild(_player, 0);    
    _player_hp = CCSprite::create("blue_hp.png");
    _player_hp->setPosition( ccp(_size.width/2, 2) );
    this->addChild(_player_hp, 0);    
    _player_name = CCLabelTTF::create("", "Arial", 20);
    _player_name->setPosition( ccp(_size.width / 2, 55) );
    this->addChild(_player_name, 1);

    _enemy = CCSprite::create("good_face.png");
    _enemy->setPosition( ccp(_size.width/2, _size.height - 20) );
    this->addChild(_enemy, 0);    
    _enemy_hp = CCSprite::create("red_hp.png");
    _enemy_hp->setPosition( ccp(_size.width/2, _size.height - 2) );
    this->addChild(_enemy_hp, 0);    
    _enemy_name = CCLabelTTF::create("", "Arial", 20);
    _enemy_name->setPosition( ccp(_size.width / 2, _size.height - 45) );
    this->addChild(_enemy_name, 1);

    return true;
}

void GameLayer::menuOpenCallback(CCObject* pSender)
{
    hide();
    master().subsystem<MenuLayer>().show();
}

void GameLayer::show()
{
    _pMenu->setVisible(true);
}

void GameLayer::hide()
{
    _pMenu->setVisible(false);
}

void GameLayer::update_scene()
{
//    update_tip(_tip, 2, 2);
//    update_tip(_bad_tip, 0, -2);
//    update_tip(_good_tip, 0, 2);
}

//void GameLayer::update_tip(CCSprite *tip, float dx, float dy)
//{
//    if (tip->getIsVisible())
//    {
//        CCPoint pos = tip->getPosition();
//        pos.x += dx;
//        pos.y += dy;
//        tip->setPosition(pos);
//
//        if (pos.x > _size.width || pos.y > _size.height || pos.x < 0 || pos.y < 0)
//        {
//            tip->setIsVisible(false);
//        }
//    }
//}

void GameLayer::show_good_tip(const std::string &tip)
{
    _good_tip->setString(tip.c_str());
    _good_tip->setPosition( ccp(_size.width/2, _size.height/2) );
    _good_tip->setVisible(true);
}

void GameLayer::show_bad_tip(const std::string &tip)
{
    _bad_tip->setString(tip.c_str());
    _bad_tip->setPosition( ccp(_size.width/2, _size.height/2) );
    _bad_tip->setVisible(true);
}

void GameLayer::show_tip(const std::string &tip)
{
    _tip->setString(tip.c_str());
    _tip->setPosition( ccp(_size.width/2, _size.height/2) );
    _tip->setVisible(true);
}

//void GameLayer::update_player(const Player &pl)
//{
//    update_someone(pl, _player);
//    _player_hp->setScaleX(pl.hp > 0 ? pl.hp / 100 : 0);
//    _player_name->setString(pl.name.c_str());
//}
//
//void GameLayer::update_enemy(const Player &pl)
//{
//    update_someone(pl, _enemy);
//    _enemy_hp->setScaleX(pl.hp > 0 ? pl.hp / 100 : 0);
//    _enemy_name->setString(pl.name.c_str());
//}
//
//void GameLayer::update_someone(const Player &pl, CCSprite *sprite)
//{
//    if (pl.hp > 50)
//    {
//        sprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("good_face.png"));
//    }
//    else if (pl.hp > 0)
//    {
//        sprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("angry_face.png"));
//    }
//    else
//    {
//        sprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("rip_face.png"));
//    }
//}

