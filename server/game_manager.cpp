#include "game_manager.hpp"

#include "server_config.hpp"

#include "server_player.hpp"
#include "logger.hpp"

GameManager::GameManager(Master *_master)
    : Subsystem(_master)
    , _creating_game(0)
{
    size_t max_games = master().subsystem<Config>()["max_games"];
    logger::log(TRACE) << "[Game manager] Maximum games set to " << max_games;
    _games.resize(max_games);
}

ServerGame * GameManager::join_game(Session *s, Event &evt)
{
    boost::mutex::scoped_lock g(_creating_game_guard);

    if (_creating_game == ~0u)
    {
        _creating_game = find_free_game();
        if (_creating_game == ~0u)
        {
            return 0;
        }
    }

    ServerGame &game = _games[_creating_game];

    switch (game.state())
    {
    case ServerGame::EMPTY:
        {
            ServerPlayer pl;
            pl.init(s, evt["login"]);
            game.init_first_player(_creating_game, pl);
        }
        break;
    case ServerGame::ONE_PLAYER:
        {
            ServerPlayer pl;
            pl.init(s, evt["login"]);
            game.init_second_player(pl);

            _creating_game = find_free_game();
        }
        break;
    default:
        assert(!"Join to nonempty game");
        break;
    }
    return &game;
}

void GameManager::leave_game(size_t id, Session *s)
{
    ServerGame &game = _games[id];

    if (game.state() == ServerGame::IN_ACTION)
    {
        ServerPlayer disconnected;
        ServerPlayer other;
        if (s == game.a.session)
        {
            disconnected = game.a;
            other = game.b;
        }
        else if (s == game.b.session)
        {
            disconnected = game.b;
            other = game.a;
        }
        else
        {
            assert(!"Logic error when leaving game");
        }

        Event evt = Event::create(EV_END_GAME);
        evt["winner"] = other.name;

        other.session->send_event(evt);

        disconnected.session->game = 0;
        other.session->game = 0;
    }
    else
    {
        s->game = 0;
    }

    game.reset();
}

size_t GameManager::find_free_game()
{
    for (size_t i = 0; i < _games.size(); ++i)
    {
        if (_games[i].state() == ServerGame::EMPTY)
        {
            return i;
        }
    }

    return ~0u;
}