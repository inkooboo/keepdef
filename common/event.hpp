//
//  event.hpp
//  keepdef
//
//  Created by Andrey Kubarkov on 7/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef keepdef_event_hpp
#define keepdef_event_hpp

# include "event_description.hpp"

# include <string>
# include <map>
# include <sstream>
# include <cassert>

class event_t
{
public:
    explicit event_t(event_type_t type);
    event_t(const char *data, size_t length);
    
    std::string serialize() const;

    const event_description_t & description() const;
    
    template <typename T>
    inline void set(const char *name, const T &value);
    
    template <typename T>
    inline T get(const char *name) const;
    
private:
    const event_description_t *m_description;
    std::map<std::string, std::string> m_params;
};

// Implementation

template <typename T>
inline void event_t::set(const char *name, const T &value)
{
    assert(m_description->has_parameter(name));

    std::stringstream ss;
    ss << value;
    m_params[name] = ss.str();
}

template <typename T>
inline T event_t::get(const char *name) const
{
    assert(m_description->has_parameter(name));

    T ret;
    
    std::stringstream ss;    
    auto it = m_params.find(name);
    if (it != m_params.end())
    {
        ss << it->second;
    }
    ss >> ret;
    
    return ret;
}

template <>
inline std::string event_t::get<std::string>(const char *name) const
{
    assert(m_description->has_parameter(name));

    std::string ret;
    
    auto it = m_params.find(name);
    if (it != m_params.end())
    {
        ret = it->second;
    }
    
    return ret;
}


#endif
