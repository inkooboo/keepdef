//
//  game_controller.cpp
//  keepdef
//
//  Created by Admin on 8/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "game_controller.hpp"

#include "config.hpp"
#include "event.hpp"
#include "game_layer.hpp"
#include "md5.h"

#include <sstream>

game_controller_t::game_controller_t()
    : m_started(false)
{
}

void game_controller_t::start()
{
    master_t::subsystem<event_manager_t>().subscribe_for_events(std::bind(&game_controller_t::event_handler, this, std::placeholders::_1));
}

void game_controller_t::stop()
{
}

void game_controller_t::start_game(const std::string &login, const std::string &pass)
{
    if (!m_started)
    {        
        event_t evt_start(EV_CLIENT_START);
        evt_start.set("login", login);
        evt_start.set("password", md5(pass));
        master_t::subsystem<event_manager_t>().send_event(evt_start);
        m_name = login;
    }
}

void game_controller_t::event_handler(const event_t &event)
{
    switch (event.description().id())
    {
            
        case EV_SERVER_START:
        {
            auto enemy_name = event.get<std::string>("enemy");
            master_t::subsystem<game_layer_t>().show_tip("START!");
            master_t::subsystem<game_layer_t>().update_player(m_name, 100.f);
            master_t::subsystem<game_layer_t>().update_enemy(enemy_name, 100.f);
            m_started = true;
            break;
        }
        
        case EV_SERVER_HIT:
        {
            auto power = event.get<double>("power");
            
            // show tip
            std::stringstream ss;
            ss << "HIT " << power << "!"; 
            
            if (event.get<std::string>("target") == m_name)
            {
                master_t::subsystem<game_layer_t>().update_player(m_name, event.get<double>("target_hp"));
                master_t::subsystem<game_layer_t>().show_bad_tip(ss.str());
            }
            else
            {
                master_t::subsystem<game_layer_t>().update_enemy(event.get<std::string>("target"), event.get<double>("target_hp"));
                master_t::subsystem<game_layer_t>().show_good_tip(ss.str());
            }
            
        }
            break;
            
        case EV_DISCONNECTED:
        {
            std::stringstream ss;
            ss << "DISCONNECTED! " << event.get<std::string>("code") << " " << event.get<std::string>("server"); 
            master_t::subsystem<game_layer_t>().show_tip(ss.str());
            m_started = false;
        }
            break;
            
        case EV_NOTIFY:
            master_t::subsystem<game_layer_t>().show_tip(event.get<std::string>("message"));
            break;
            
        case EV_END_GAME:
        {
            std::stringstream ss;
            ss << event.get<std::string>("winner") << " won the match!"; 
            master_t::subsystem<game_layer_t>().show_tip(ss.str());
            m_started = false;
        }
            break;
            
            //case :
            //    break;
            
        case EV_INVALID:
        default:
            break;
    }
    
}

void game_controller_t::left_fire(cocos2d::CCObject* pSender)
{
    if (!m_started)
    {
        return;
    }
    event_t evt(EV_CLIENT_HIT);
    evt.set("from", ""/*_player.name*/);
    evt.set("target", ""/*_enemy.name*/);
    evt.set("side", "left");
    master_t::subsystem<event_manager_t>().send_event(evt);
}

void game_controller_t::right_fire(cocos2d::CCObject* pSender)
{
    if (!m_started)
    {
        return;
    }
    event_t evt(EV_CLIENT_HIT);
    evt.set("from", ""/*_player.name*/);
    evt.set("target", ""/*_enemy.name*/);
    evt.set("side", "right");
    master_t::subsystem<event_manager_t>().send_event(evt);
}
