#include "cocos_scene.hpp"

#include "master.hpp"
#include "event_manager.hpp"
#include "config.hpp"
#include "network.hpp"
#include "game.hpp"
#include "game_layer.hpp"
#include "menu_layer.hpp"

#include <cstdlib>
#include <ctime>

USING_NS_CC;

CocosScene *g_cocos_scene = 0;

CCScene* CocosScene::scene()
{
    // 'scene' is an autorelease object
    CocosScene *scene = CocosScene::node();
    
    g_cocos_scene = scene;

    srand((unsigned int)time(NULL));

    //// initialize subsystems

    scene->_master.add_subsystem<Config>();
    scene->_master.add_subsystem<EventManager>();
    scene->_master.add_subsystem<Game>();
    scene->_master.add_subsystem<GameLayer>();
    scene->_master.add_subsystem<MenuLayer>();
    
    scene->_master.start();

    ////// schedule game loop
    scene->schedule(schedule_selector(CocosScene::tick), 1.f/60.f);

    // return the scene
    return scene;
}

void CocosScene::tick(cocos2d::ccTime dt)
{
    _master.subsystem<EventManager>().receive_events();
    _master.subsystem<GameLayer>().update_scene();
}

void CocosScene::end_scene()
{
    _master.stop();

    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
