#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include "../Classes/master.hpp"
# include "../Classes/event.hpp"
# include "session.hpp"

# include <boost/bind.hpp>
# include <boost/asio.hpp>

class Server: public Subsystem
{
    friend class Master;

    explicit Server(Master *master);

    virtual void start();
    virtual void stop();

private:
    void start_accept();

    void handle_accept(Session* new_session, const boost::system::error_code& error);

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif //_SERVER_HPP_