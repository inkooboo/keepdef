#ifndef _GAME_LAYER_HPP_
# define _GAME_LAYER_HPP_

# include "master.hpp"
# include "player.hpp"

# include <cocos2d.h>

class GameLayer : public cocos2d::CCLayer, public Subsystem
{
    friend class Master;

    explicit GameLayer(Master *master);

    virtual void start();
    virtual void stop();

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // a selector callback
    void menuOpenCallback(cocos2d::CCObject* pSender);

    void show();
    void hide();

    void update_scene();

    void show_tip(const std::string &tip);
    void show_good_tip(const std::string &tip);
    void show_bad_tip(const std::string &tip);

    void update_player(const Player &pl);
    void update_enemy(const Player &pl);

private:

    void update_someone(const Player &pl, cocos2d::CCSprite *sprite);
    void update_tip(cocos2d::CCSprite *tip, float dx, float dy);

    cocos2d::CCSize _size;
    cocos2d::CCLabelTTF *_tip;
    cocos2d::CCLabelTTF *_good_tip;
    cocos2d::CCLabelTTF *_bad_tip;

    cocos2d::CCSprite *_player;
    cocos2d::CCSprite *_player_hp;
    cocos2d::CCLabelTTF *_player_name;

    cocos2d::CCSprite *_enemy;
    cocos2d::CCSprite *_enemy_hp;
    cocos2d::CCLabelTTF *_enemy_name;

    cocos2d::CCMenu *_pMenu;
};

#endif //_GAME_LAYER_HPP_