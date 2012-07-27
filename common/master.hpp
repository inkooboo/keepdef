#ifndef __MASTER_HPP__
# define __MASTER_HPP__

# include "subsystem.hpp"
# include "noncopyable.hpp"

# include <vector>
# include <cassert>

class master_t : private noncopyable_t
{
public:
    template <typename SubsystemType>
    inline void add_subsystem();

    template <typename SubsystemType>
    inline SubsystemType & subsystem();

    inline void start();
    inline void stop();

    inline ~master_t();

private:

    template <typename SubsystemType>
    inline SubsystemType ** subsystem_instance();

    typedef std::vector<subsystem_t *> holder_type;
    holder_type m_subsystem_holder;
};

// Implementation

inline void master_t::start()
{
    for (auto &subsystem : m_subsystem_holder)
    {
        subsystem->start();
    }
}

inline void master_t::stop()
{
    for (auto &subsystem : m_subsystem_holder)
    {
        subsystem->stop();
    }
    
    for (auto &subsystem : m_subsystem_holder)
    {
        delete subsystem;
    }
}

inline master_t::~master_t()
{
    for (auto &subsystem : m_subsystem_holder)
    {
        delete subsystem;
    }
}

template <typename SubsystemType>
inline SubsystemType ** master_t::subsystem_instance()
{
    static SubsystemType *instance = 0;
    return &instance;
}

template <typename SubsystemType>
inline void master_t::add_subsystem()
{
    SubsystemType **instance = subsystem_instance<SubsystemType>();
    assert(*instance == 0);

    *instance = new SubsystemType();
    static_cast<subsystem_t *>(*instance)->master() = this;
    m_subsystem_holder.push_back(*instance);
}

template <typename SubsystemType>
inline SubsystemType & master_t::subsystem()
{
    SubsystemType **instance = subsystem_instance<SubsystemType>();
    assert(*instance != 0);
   
    return **instance;
}

#endif //__MASTER_HPP__