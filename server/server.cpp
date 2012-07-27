#include "server.hpp"

#include "server_config.hpp"
#include "thread_pool.hpp"
#include "logger.hpp"

Server::Server(Master *_master)
    : Subsystem(_master)
    , io_service_(master().subsystem<ThreadPool>().get_io_service())
    , acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), int(master().subsystem<Config>()["port"])))
{
    logger::log(TRACE) << "[Server] Create server using port " << int(master().subsystem<Config>()["port"]);
}

void Server::start()
{
    logger::log(TRACE, "[Server] Start server");
    start_accept();
}

void Server::stop()
{
}

void Server::start_accept()
{
    Session* new_session = new Session(io_service_, &master());
    acceptor_.async_accept(new_session->socket(), boost::bind(&Server::handle_accept, this, new_session, boost::asio::placeholders::error));
}

void Server::handle_accept(Session* new_session, const boost::system::error_code& error)
{
    if (!error)
    {
        new_session->start_read();
    }
    else
    {
        delete new_session;
    }

    start_accept();
}
