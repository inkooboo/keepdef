//
//  event.hpp
//  keepdef
//
//  Created by Andrey Kubarkov on 7/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef keepdef_event_hpp
#define keepdef_event_hpp

# include <string>

enum event_type_t
{
    EV_INVALID = 0
    , EV_DISCONNECTED
    , EV_START
    , EV_HIT
    , EV_NOTIFY
    , EV_END_GAME
    
    , EV_MAX
};

class event_t
{
public:
    explicit event_t(event_type_t type)
        : m_type(type)
    {
    }

    static std::string serialize(const event_t &evt)
    {
        return std::string();
    }
    
private:
    event_type_t m_type;
};

#endif
