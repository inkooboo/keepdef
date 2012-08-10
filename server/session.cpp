#include "session.hpp"
#include "game.hpp"
#include "game_manager.hpp"
#include "logger.hpp"
#include "master.hpp"

session_t::session_t(boost::asio::io_service& io_service)
    : socket_(io_service)
    , game(0)
{
}

boost::asio::ip::tcp::socket& session_t::socket()
{
    return m_socket;
}

void session_t::start_read()
{
    m_socket.async_read_some(boost::asio::buffer(data_, max_length),
                             std::bind(&session_t::handle_read, this,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred));
}

void session_t::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        event_t evt(m_data, bytes_transferred);

        logger::log(DEBUG) << "[Session] Event got:\n" << evt.serialize();

        if (!m_game)
        {
            m_game = master_t::subsystem<game_manager_t>().join_game(this, evt);
            if (!game)
            {
                event_t notify(EV_NOTIFY);
                notify.set("message", "Max game limit exceed");
                send_event(notify);
                delete this;

                return;
            }
        }
        else
        {
           m_game->on_event(evt);
        }

        start_read();
    }
    else
    {
		if (m_game)
        {
            master_t::subsystem<game_manager_t>().leave_game(game->id, this);
		}
        delete this;
    }
}

void session_t::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
    }
    else
    {
		if (m_game)
        {
	        master_t::subsystem<game_manager_t>().leave_game(game->id, this);
		}
        delete this;
    }
}

void session_t::send_event(const event_t &evt)
{
    std::string serialized = evt.serialize();

    logger::log(DEBUG) << "[Session] Send event:\n" << serialized;

    boost::asio::async_write(m_socket,
                             boost::asio::buffer(serialized.c_str(), serialized.length()),
                             std::bind(&Session::handle_write, this,
                             boost::asio::placeholders::error));
}
