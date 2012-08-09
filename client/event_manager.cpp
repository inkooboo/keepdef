#include "event_manager.hpp"
#include "config.hpp"

#include <vector>
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
        std::string server(master_t::subsystem<config_t>().get<std::string>("server"));
        int result = m_net.connect(server.c_str());
        if (result)
        {
            event_t evt(EV_DISCONNECTED);
            evt.set("code", result);
            evt.set("server", server);
            notify(evt);
            return;
        }
    }

    std::string serialized = evt.serialize();
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

    std::vector<char> net_data;
	m_net.recv(net_data);

    if (!net_data.empty())
    {
        const char *data = &net_data[0];
        size_t len = net_data.size();
        while (len > 0)
        {
            const char *search = (const char *)memchr(data, '\r', len);
            ++search;
            size_t chunk_len = search - data;
            
            event_t received(data, chunk_len);
                
            notify(received);

            data = search;
            len -= chunk_len;
        }
    }
    else
    {
        if (!m_net.is_connected())
        {
            event_t evt(EV_DISCONNECTED);
            notify(evt);
        }
    }


}

void event_manager_t::notify(const event_t &evt)
{
    for (auto &handler : m_receivers)
    {
        handler(evt);
    }
}
