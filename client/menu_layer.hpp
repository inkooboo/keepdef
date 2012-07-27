#ifndef _MENU_LAYER_HPP_
# define _MENU_LAYER_HPP_

# include "master.hpp"
# include "text_input_helper.hpp"

# include <cocos2d.h>


class MenuLayer : public cocos2d::CCLayer, public subsystem_t
{

    virtual void start();
    virtual void stop();

public:
    MenuLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // a selector callback
    void on_exit_cb(cocos2d::CCObject* pSender);
    void on_start_cb(cocos2d::CCObject* pSender);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    void show();
    void hide();

private:
    cocos2d::CCPoint  _touch_begin;

    TextInput _input_name;
    TextInput _input_password;
};

#endif //_MENU_LAYER_HPP_