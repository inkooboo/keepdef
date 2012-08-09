//
//  game_controller.hpp
//  keepdef
//
//  Created by Admin on 8/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef keepdef_game_controller_hpp
#define keepdef_game_controller_hpp

# include "master.hpp"
# include "event_manager.hpp"

# include <cocos2d.h>
# include <string>

class game_controller_t : public cocos2d::CCObject, public subsystem_t
{    
    virtual void start();
    virtual void stop();
    
public:
    game_controller_t();
    
    void start_game(const std::string &login, const std::string &pass);
    
    void left_fire(cocos2d::CCObject* pSender);
    void right_fire(cocos2d::CCObject* pSender);
    
    void event_handler(const event_t &event);
    
private:
    bool m_started;
    std::string m_my_name;
    std::string m_enemy_name;
};

#endif
