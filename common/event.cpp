//
//  event.cpp
//  keepdef
//
//  Created by inkooboo on 8/9/12.
//
//

#include "event.hpp"

event_t::event_t(event_type_t type)
    : m_description(event_description_t::description_by_id(type))
{
}

event_t::event_t(const char *data, size_t length)
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

const event_description_t & event_t::description() const
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

