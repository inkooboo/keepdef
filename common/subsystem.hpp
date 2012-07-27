#ifndef __SUBSYSTEM_HPP__
# define __SUBSYSTEM_HPP__

class master_t;

class subsystem_t
{
    friend class master_t;
public:
    virtual void start() {}
    virtual void stop() {}
    
    virtual inline ~subsystem_t() = 0;

    inline master_t & master();
    
private:
    master_t *m_master;
};

// Implementation

inline subsystem_t::~subsystem_t()
{
}

inline master_t & subsystem_t::master()
{
    return *m_master;
}

#endif //__SUBSYSTEM_HPP__
