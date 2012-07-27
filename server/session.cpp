#include "session.hpp"
#include "server_game.hpp"
#include "game_manager.hpp"
#include "logger.hpp"

Session::Session(boost::asio::io_service& io_service, Master *master)
    : socket_(io_service)
    , _master(master)
    , game(0)
{
}

boost::asio::ip::tcp::socket& Session::socket()
{
    return socket_;
}

void Session::start_read()
{
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            boost::bind(&Session::handle_read, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        Event evt = Event::parse(data_, bytes_transferred);

        logger::log(DEBUG) << "[Session] Event got:\n" << evt.serialize();

        if (!game)
        {
            game = _master->subsystem<GameManager>().join_game(this, evt);
            if (!game)
            {
                Event notify = Event::create(EV_NOTIFY);
                notify["message"] = "Max game limit exceed";
                send_event(notify);
                delete this;

                return;
            }
        }
        else
        {
            game->on_event(evt);
        }

        start_read();
    }
    else
    {
		if (game)
        {
	        _master->subsystem<GameManager>().leave_game(game->id, this);
		}
        delete this;
    }
}

void Session::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
    }
    else
    {
		if (game)
        {
	        _master->subsystem<GameManager>().leave_game(game->id, this);
		}
        delete this;
    }
}

void Session::send_event(const Event &evt)
{
    std::string serialized = evt.serialize();

    logger::log(DEBUG) << "[Session] Send event:\n" << serialized;

    boost::asio::async_write(socket_,
                             boost::asio::buffer(serialized.c_str(), serialized.length()),
                             boost::bind(&Session::handle_write, this,
                             boost::asio::placeholders::error));
}
