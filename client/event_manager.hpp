#ifndef  _EVENT_MANAGER_HPP_
# define  _EVENT_MANAGER_HPP_

# include "master.hpp"
# include "event.hpp"
# include "network.hpp"

# include <list>
# include <functional>

class event_manager_t : public subsystem_t
{
    virtual void start();
    virtual void stop();
public:
    typedef std::function<void(const event_t &)> event_handler_t;
    
    event_manager_t();

    void send_event(const event_t &evt);

    void subscribe_for_events(event_handler_t handler);

    void receive_events();

private:
    void notify(const event_t &evt);

    typedef std::list<event_handler_t> receivers_list_t;
    
    receivers_list_t m_receivers;

    network_t m_net;
};


#endif //_EVENT_MANAGER_HPP_