#ifndef  _EVENT_MANAGER_HPP_
# define  _EVENT_MANAGER_HPP_

# include "master.hpp"
# include "event.hpp"
# include "network.hpp"

# include <list>

struct EventReceiver
{
    virtual void on_event(Event &evt) = 0;

    virtual ~EventReceiver() {}
};

class EventManager : public Subsystem
{
    friend class Master;

    explicit EventManager(Master *master);

    virtual void start();
    virtual void stop();

    typedef std::list<EventReceiver *> ReceiversList;

public:

    void send_event(const Event &evt);

    void subscribe_for_events(EventReceiver *receiver);
    void unsubscribe_from_events(EventReceiver *receiver);
    void receive_events();

private:
    void notify(Event &evt);

    ReceiversList _receivers;

    Network _net;
};


#endif //_EVENT_MANAGER_HPP_