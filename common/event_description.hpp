//
//  events_description.hpp
//  keepdef
//
//  Created by Admin on 8/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef keepdef_events_description_hpp
#define keepdef_events_description_hpp

# include <set>
# include <map>
# include <string>
# include <cassert>

enum event_type_t
{
      EV_INVALID
    , EV_DISCONNECTED
    , EV_CLIENT_START
    , EV_SERVER_START
    , EV_CLIENT_HIT
    , EV_SERVER_HIT
    , EV_NOTIFY
    , EV_END_GAME
};


class event_description_t
{
public:
    event_type_t id() const
    {
        return m_id;
    }
    
    bool has_parameter(const char *name) const
    {
        return m_params.find(name) != m_params.end();
    }
    
    static const event_description_t * description_by_id(event_type_t id)
    {
        static std::map<event_type_t, event_description_t> descriptions {
              {EV_INVALID, event_description_t(EV_INVALID, {})}
            , {EV_DISCONNECTED, event_description_t(EV_DISCONNECTED, {"code", "server"})}
            , {EV_CLIENT_START, event_description_t(EV_CLIENT_START, {"login", "password"})}
            , {EV_SERVER_START, event_description_t(EV_SERVER_START, {"id", "enemy"})}
            , {EV_CLIENT_HIT, event_description_t(EV_CLIENT_HIT, {"source", "target", "type"})}
            , {EV_SERVER_HIT, event_description_t(EV_SERVER_HIT, {"source", "target", "type", "power", "target_hp"})}
            , {EV_NOTIFY, event_description_t(EV_NOTIFY, {"message"})}
            , {EV_END_GAME, event_description_t(EV_END_GAME, {"winner"})}
        };

        assert(descriptions.find(id) != descriptions.end());
        
        return &descriptions[id];
    }
    
    event_description_t()
        : m_id(EV_INVALID)
    {
    }
    
private:
    typedef std::set<std::string> description_params_t;
    
    event_description_t(event_type_t id, description_params_t params)
        : m_id(id)
        , m_params(std::move(params))
    {
    }
    
    event_type_t m_id;
    description_params_t m_params;
};


#endif
