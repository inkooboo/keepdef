#include "event_manager.hpp"
#include "config.hpp"

#include <cstring>

EventManager::EventManager(Master *master)
    : Subsystem(master)
{
}

void EventManager::start()
{
}

void EventManager::stop()
{
    _net.disconnect();
}

void EventManager::send_event(const Event &evt)
{
	if (!_net.is_connected())
	{
		std::string server(master().subsystem<Config>()["server"]);
		int result = _net.connect(server.c_str());
		if (result)
		{
			Event evt = Event::create(EV_DISCONNECTED);
			evt["code"] = result;
			evt["server"] = server;
			notify(evt);
			return;
		}
	}

    std::string serialized(evt.serialize());
    if (!_net.send(serialized.c_str(), serialized.length()) && !_net.is_connected())
    {
        Event evt = Event::create(EV_DISCONNECTED);
        notify(evt);
    }
}

void EventManager::subscribe_for_events(EventReceiver *receiver)
{
    _receivers.push_back(receiver);
}


void EventManager::unsubscribe_from_events(EventReceiver *receiver)
{
    _receivers.remove(receiver);
}

void EventManager::receive_events()
{
    if (!_net.is_connected())
    {
        return;
    }

    MovableNetworkData rcv;
	_net.recv(rcv);

    if (!rcv.empty())
    {
        const char *data = rcv.data();
        size_t len = rcv.len();
        while (len > 0)
        {
            const char *search = (const char *)memchr(data, '\r', len);
            ++search;
            size_t chunk_len = search - data;
            
            Event received = Event::parse(data, chunk_len);
                
            notify(received);

            data = search;
            len -= chunk_len;
        }
    }
    else
    {
        if (!_net.is_connected())
        {
            Event evt = Event::create(EV_DISCONNECTED);
            notify(evt);
        }
    }


}

void EventManager::notify(Event &evt)
{
    for (ReceiversList::iterator it = _receivers.begin(); it != _receivers.end(); ++it)
    {
        (*it)->on_event(evt);
    }
}