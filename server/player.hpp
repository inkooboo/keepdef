#ifndef  _SERVER_PLAYER_HPP_
# define  _SERVER_PLAYER_HPP_

# include "session.hpp"

# include <string>

struct player_t
{
    player_t()
    {
    }

    void init(session_t *session, const std::string &name)
    {
        m_session = session_;
        m_hp = 100.f;
        m_name = name;
    }

    session_t *m_session;
    float m_hp;
    std::string m_name;
};

#endif // _SERVER_PLAYER_HPP_
