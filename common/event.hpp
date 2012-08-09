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
    inline explicit event_t(event_type_t type);
    inline event_t(const char *data, size_t length);
    
    inline std::string serialize() const;

    inline const event_description_t & description() const;
    
    template <typename T>
    inline void set(const char *name, const T &value);
    
    template <typename T>
    inline T get(const char *name) const;
    
private:
    const event_description_t *m_description;
    std::map<std::string, std::string> m_params;
};

// Implementation

inline event_t::event_t(event_type_t type)
    : m_description(event_description_t::description_by_id(type))
{
}

inline event_t::event_t(const char *data, size_t length)
{
    std::stringstream ss(std::string(data, length));
    
    int id;
    ss >> id;
    
    m_description = event_description_t::description_by_id(static_cast<event_type_t>(id));
    
    while (!ss.eof())
    {
        std::string line;
        std::getline(ss, line, '\n');
        
        size_t space = line.find(' ');
        if (std::string::npos == space)
        {
            continue;
        }
        
        std::string name = line.substr(0, space);
        std::string value = line.substr(space + 1, line.length() - space - 1);
        
        assert(m_description->has_parameter(name.c_str()));

        m_params[name].swap(value);
    }
}

inline const event_description_t & event_t::description() const
{
    return *m_description;
}

std::string event_t::serialize() const
{
    std::stringstream ss;
    
    ss << m_description->id() << std::endl;
    
    for (auto &it : m_params)
    {
        ss << it.first << " " << it.second << std::endl;
    }
    
    return ss.str();
}

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
