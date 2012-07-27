#ifndef __SUBSYSTEM_HPP__
# define __SUBSYSTEM_HPP__

#include "noncopyable.hpp"

class master_t;

class subsystem_t : private noncopyable_t
{
public:
    inline master_t & master();

    virtual void start() {}
    virtual void stop() {}

    virtual inline ~subsystem_t() = 0;

protected:
    inline explicit subsystem_t(master_t *master);

    master_t *m_master;
};


// implementation


inline master_t & subsystem_t::master()
{
    return *m_master;
}

inline subsystem_t::~subsystem_t()
{
}

inline subsystem_t::subsystem_t(master_t *master)
    : m_master(master)
{
}

#endif //__SUBSYSTEM_HPP__
