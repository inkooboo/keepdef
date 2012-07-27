#ifndef _GAME_MANAGER_HPP_
# define _GAME_MANAGER_HPP_

# include "../Classes/master.hpp"
# include "server_game.hpp"
# include "session.hpp"

# include <boost/thread/mutex.hpp>
# include <vector>

class GameManager : public Subsystem
{
    friend class Master;

    explicit GameManager(Master *_master);

    typedef std::vector<ServerGame> GamesStorageType;

public:
    ServerGame * join_game(Session *s, Event &evt);
    void leave_game(size_t id, Session *s);

private:
    size_t find_free_game();

    boost::mutex _creating_game_guard;
    size_t _creating_game;

    GamesStorageType _games;
};




#endif //_GAME_MANAGER_HPP_