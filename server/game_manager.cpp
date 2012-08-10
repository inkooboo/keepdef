#include "game_manager.hpp"

#include "config.hpp"

#include "player.hpp"
#include "logger.hpp"

game_manager_t::game_manager_t()
    : m_creating_game(0)
{
    size_t max_games = master_t::subsystem<config_t>().get<size_t>("max_games");
    logger::log(LOG_TRACE) << "[Game manager] Maximum games set to " << max_games;
    m_games.resize(max_games);
}

game_t * game_manager_t::join_game(Session *s, Event &evt)
{
    std::mutex::scoped_lock g(m_creating_game_guard);

    if (m_creating_game == ~0u)
    {
        m_creating_game = find_free_game();
        if (m_creating_game == ~0u)
        {
            return 0;
        }
    }

    game_t &game = m_games[m_creating_game];

    switch (game.state())
    {
    case game_t::EMPTY:
        {
            player_t pl;
            pl.init(s, evt.get<std::string>("login"));
            game.init_first_player(m_creating_game, pl);
        }
        break;
    case game_t::ONE_PLAYER:
        {
            player_t pl;
            pl.init(s, evt.get<std::string>("login"));
            game.init_second_player(pl);

            m_creating_game = find_free_game();
        }
        break;
    default:
        assert(!"Join to nonempty game");
        break;
    }
    return &game;
}

void game_manager_t::leave_game(size_t id, Session *s)
{
    game_t &game = m_games[id];

    if (game.state() == game_t::IN_ACTION)
    {
        player_t disconnected;
        player_t other;
        if (s == game.a.session)
        {
            disconnected = game.m_a;
            other = game.m_b;
        }
        else if (s == game.m_b.m_session)
        {
            disconnected = game.m_b;
            other = game.m_a;
        }
        else
        {
            assert(!"Logic error when leaving game");
        }

        event_t evt(EV_END_GAME);
        evt.set("winner", other.m_name);

        other.session->send_event(evt);

        disconnected.m_session->m_game = 0;
        other.m_session->m_game = 0;
    }
    else
    {
        s->m_game = 0;
    }

    game.reset();
}

size_t game_manager_t::find_free_game()
{
    for (size_t i = 0; i < m_games.size(); ++i)
    {
        if (m_games[i].state() == game_t::EMPTY)
        {
            return i;
        }
    }

    return ~0u;
}