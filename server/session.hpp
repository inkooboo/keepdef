#ifndef _SESSION_HPP_
# define _SESSION_HPP_

# include "../Classes/event.hpp"
# include "../Classes/master.hpp"

# include <boost/bind.hpp>
# include <boost/asio.hpp>

struct ServerGame;

struct Session
{
    ServerGame *game;

    Session(boost::asio::io_service& io_service, Master *master);

    boost::asio::ip::tcp::socket& socket();

    void start_read();

    void send_event(const Event &evt);

private:
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

    void handle_write(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket socket_;
    enum { max_length = 8192 };
    char data_[max_length];

    Master *_master;
};

#endif //_SESSION_HPP_