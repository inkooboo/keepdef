#include "cocos_scene.hpp"

#include "master.hpp"
#include "event_manager.hpp"
#include "config.hpp"
#include "network.hpp"
//#include "game.hpp"
#include "game_layer.hpp"
#include "menu_layer.hpp"

#include "CCFileUtils.h"

#include <cstdlib>
#include <ctime>

USING_NS_CC;

cocos_scene_t *g_cocos_scene;

CCScene* cocos_scene_t::scene()
{
    // 'scene' is an autorelease object
    cocos_scene_t *scene = cocos_scene_t::create();
    
    g_cocos_scene = scene;

    srand((unsigned int)time(NULL));

    //// initialize subsystems

    unsigned char *data = 0;
    unsigned long size = 0;
    data = CCFileUtils::sharedFileUtils()->getFileData("config.txt", "r", &size);
    
    std::string cfg_str;
    if (data && size)
    {
        cfg_str.assign((char *)data, size);
    }
    
    
    scene->m_master.add_subsystem<config_t>(cfg_str);
    scene->m_master.add_subsystem<event_manager_t>();
//    scene->m_master.add_subsystem<Game>();
    scene->m_master.add_subsystem<GameLayer>();
    scene->m_master.add_subsystem<MenuLayer>();
    
    scene->m_master.start();

    ////// schedule game loop
    scene->schedule(schedule_selector(cocos_scene_t::tick), 1.f/60.f);

    // return the scene
    return scene;
}

void cocos_scene_t::tick(cocos2d::CCTime dt)
{
    m_master.subsystem<event_manager_t>().receive_events();
    m_master.subsystem<GameLayer>().update_scene();
}

void cocos_scene_t::end_scene()
{
    m_master.stop();

    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
