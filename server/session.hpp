#ifndef _SESSION_HPP_
# define _SESSION_HPP_

# include "event.hpp"

# include <functional>

# include <boost/asio.hpp>

struct game_t;

struct session_t
{
    game_t *m_game;

    session_t(boost::asio::io_service& io_service, Master *master);

    boost::asio::ip::tcp::socket &socket();

    void start_read();

    void send_event(const event_t &evt);

private:
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

    void handle_write(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket m_socket;
    enum { max_length = 8192 };
    char data_[max_length];
};

#endif //_SESSION_HPP_