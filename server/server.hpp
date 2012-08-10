#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include "master.hpp"
# include "event.hpp"
# include "session.hpp"

# include <boost/asio.hpp>

class server_t: public subsystem_t
{
    virtual void start();

public:
    server_t();

private:
    void start_accept();

    void handle_accept(session_t *new_session, const boost::system::error_code& error);

    boost::asio::io_service &m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif //_SERVER_HPP_