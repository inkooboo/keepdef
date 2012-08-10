#include "server.hpp"

#include "server_config.hpp"
#include "thread_pool.hpp"
#include "logger.hpp"

#include <functional>

server_t::server_t()
    : io_service_(master_t::subsystem<thread_pool_t>().io_service())
    , m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), master_t::subsystem<config_t>().get<int>("port")))
{
    logger::log(TRACE) << "[Server] Create server using port " << master_t::subsystem<config_t>().get<int>("port");
}

void server_t::start()
{
    logger::log(TRACE, "[Server] Start server");
    start_accept();
}

void server_t::start_accept()
{
    session_t *new_session = new session_t(m_io_service);
    m_acceptor.async_accept(new_session->socket(), boost::bind(&server_t::handle_accept, this, new_session, boost::asio::placeholders::error));
}

void server_t::handle_accept(session_t *new_session, const boost::system::error_code& error)
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
