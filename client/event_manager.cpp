#include "event_manager.hpp"
#include "config.hpp"

#include <cstring>

event_manager_t::event_manager_t()
{
}

void event_manager_t::start()
{
}

void event_manager_t::stop()
{
    m_net.disconnect();
}

void event_manager_t::send_event(const event_t &evt)
{
	if (!m_net.is_connected())
	{
		std::string server(master().subsystem<config_t>().get<std::string>("server"));
		int result = m_net.connect(server.c_str());
		if (result)
		{
			event_t evt(EV_DISCONNECTED);
//			evt["code"] = result;
//			evt["server"] = server;
			notify(evt);
			return;
		}
	}

    std::string serialized = event_t::serialize(evt);
    if (!m_net.send(serialized.c_str(), serialized.length()) && !m_net.is_connected())
    {
        event_t evt(EV_DISCONNECTED);
        notify(evt);
    }
}

void event_manager_t::subscribe_for_events(event_handler_t handler)
{
    m_receivers.push_back(handler);
}


void event_manager_t::receive_events()
{
    if (!m_net.is_connected())
    {
        return;
    }

//    MovableNetworkData rcv;
//	_net.recv(rcv);
//
//    if (!rcv.empty())
//    {
//        const char *data = rcv.data();
//        size_t len = rcv.len();
//        while (len > 0)
//        {
//            const char *search = (const char *)memchr(data, '\r', len);
//            ++search;
//            size_t chunk_len = search - data;
//            
//            Event received = Event::parse(data, chunk_len);
//                
//            notify(received);
//
//            data = search;
//            len -= chunk_len;
//        }
//    }
//    else
//    {
//        if (!m_net.is_connected())
//        {
//            Event evt = Event::create(EV_DISCONNECTED);
//            notify(evt);
//        }
//    }


}

void event_manager_t::notify(const event_t &evt)
{
    for (auto &handler : m_receivers)
    {
        handler(evt);
    }
}
