#ifndef _SERVER_GAME_HPP_
# define _SERVER_GAME_HPP_

# include "player.hpp"
# include "event.hpp"
# include "logger.hpp"

# include <string>
# include <sstream>
# include <cstdlib>

struct game_t
{
    size_t m_id;

    player_t m_a;
    player_t m_b;

    enum state_t
    {
          EMPTY
        , ONE_PLAYER
        , IN_ACTION
    };

    game_t()
        : m_state(EMPTY)
    {
    }

    void reset()
    {
        m_state = EMPTY;
    }

    state_t state()
    {
        return m_state;
    }

    void init_first_player(size_t id, const player_t &a)
    {
        m_state = ONE_PLAYER;
        m_id = id;
        m_a = a;
    }

    void init_second_player(const player_t &b)
    {
        m_state = IN_ACTION;
        m_b = b;

        // start game
        event_t evt(EV_SERVER_START);
        evt.set("id", m_id);

        evt.set("enemy", m_b.m_name);
        m_a.session->send_event(evt);

        evt.set("enemy", m_a.m_name);
        m_b.session->send_event(evt);
    }

    void on_event(event_t &evt)
    {
        switch (evt.description().id())
        {
        case EV_CLIENT_HIT:
            {
                size_t power = 5 + rand() % 10;

                event_t send(EV_SERVER_HIT);
//                if (evt.get<std::string>("target") == m_a.m_name)
//                {
//                    m_a.m_hp -= power;
//                    send.set<double>("power") = power;
//                    a.session->send_event(evt);
//                    b.session->send_event(evt);
//                    if (a.hp <= 0)
//                    {
//                        Event win = Event::create(EV_NOTIFY);
//                        std::stringstream ss;
//                        ss << b.name << " won the match!!!";
//                        win["message"] = ss.str();
//                        a.session->send_event(win);
//                        b.session->send_event(win);
//                    }
//                }
//                else if (std::string(evt["target"]) == b.name)
//                {
//                    b.hp -= power;
//                    evt["power"] = power;
//                    a.session->send_event(evt);
//                    b.session->send_event(evt);
//                    if (b.hp <= 0)
//                    {
//                        Event win = Event::create(EV_NOTIFY);
//                        std::stringstream ss;
//                        ss << a.name << " won the match!!!";
//                        win["message"] = ss.str();
//                        a.session->send_event(win);
//                        b.session->send_event(win);
//                    }
//                }

            }
            break;

        //case :
        //    break;

        case EV_INVALID:
        default:
            break;
        }    
    }

private:
    State _state;
};




#endif //_SERVER_GAME_HPP_