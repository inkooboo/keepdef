#ifndef __MASTER_HPP__
# define __MASTER_HPP__

# include "subsystem.hpp"
# include "noncopyable.hpp"

# include <vector>
# include <cassert>
# include <type_traits>

class master_t : private noncopyable_t
{
public:
    template <typename SubsystemType, typename... Args>
    inline void add_subsystem(Args ...args);

    template <typename SubsystemType>
    inline SubsystemType & subsystem();
    
    inline void start();
    inline void stop();

    inline ~master_t();

private:
    template <typename SubsystemType>
    inline SubsystemType ** subsystem_instance();

    template <typename SubsystemType, typename... Args>
    inline void internal_add_subsystem(std::true_type, SubsystemType *, Args ...args);
    
    template <typename SubsystemType, typename... Args>
    inline void internal_add_subsystem(std::false_type, SubsystemType *, Args ...args);
    
    template <typename SubsystemType>
    inline SubsystemType & internal_subsystem(std::true_type, SubsystemType *);
    
    template <typename SubsystemType>
    inline SubsystemType & internal_subsystem(std::false_type, SubsystemType *);
    
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

namespace internal
{
    template <typename UnmanagedSubsystemType>
    struct unmanaged_subsystem_t : public subsystem_t
    {
        template <typename... Args>
        unmanaged_subsystem_t(Args ...args) : holder(args...) {}
        UnmanagedSubsystemType holder;
    };
}


template <typename SubsystemType, typename... Args>
inline void master_t::add_subsystem(Args ...args)
{
    internal_add_subsystem(std::is_base_of<subsystem_t, SubsystemType>(), (SubsystemType *)0, args...);
}

template <typename SubsystemType, typename... Args>
inline void master_t::internal_add_subsystem(std::true_type, SubsystemType *, Args ...args)
{
    SubsystemType **instance = subsystem_instance<SubsystemType>();
    assert(*instance == 0);
    
    *instance = new SubsystemType(args...);
    static_cast<subsystem_t *>(*instance)->m_master = this;
    m_subsystem_holder.push_back(*instance);
}

template <typename SubsystemType, typename... Args>
inline void master_t::internal_add_subsystem(std::false_type, SubsystemType *, Args ...args)
{
    typedef internal::unmanaged_subsystem_t<SubsystemType> UnmanagedSubsystemType;
    UnmanagedSubsystemType **instance = subsystem_instance<UnmanagedSubsystemType>();
    assert(*instance == 0);
    
    *instance = new UnmanagedSubsystemType(args...);
    m_subsystem_holder.push_back(*instance);
}

template <typename SubsystemType>
inline SubsystemType & master_t::subsystem()
{
    return internal_subsystem(std::is_base_of<subsystem_t, SubsystemType>(), (SubsystemType *)0);
}


template <typename SubsystemType>
inline SubsystemType & master_t::internal_subsystem(std::true_type, SubsystemType *)
{
    SubsystemType **instance = subsystem_instance<SubsystemType>();
    assert(*instance != 0);
    
    return **instance;
}

template <typename SubsystemType>
inline SubsystemType & master_t::internal_subsystem(std::false_type, SubsystemType *)
{
    typedef internal::unmanaged_subsystem_t<SubsystemType> UnmanagedSubsystemType;
    
    UnmanagedSubsystemType **instance = subsystem_instance<UnmanagedSubsystemType>();
    assert(*instance != 0);
    
    return (*instance)->holder;
}


#endif //__MASTER_HPP__