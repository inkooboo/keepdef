#ifndef _GAME_MANAGER_HPP_
# define _GAME_MANAGER_HPP_

# include "master.hpp"
# include "game.hpp"
# include "session.hpp"

# include <thread>
# include <vector>

class game_manager_t : public subsystem_t
{
    typedef std::vector<game_t> games_storage_t;

public:
    game_manager_t();
    
    game_t * join_game(session_t *s, event_t &evt);
    
    void leave_game(size_t id, session_t *s);

private:
    size_t find_free_game();

    std::mutex m_creating_game_guard;
    size_t m_creating_game;

    games_storage_t m_games;
};




#endif //_GAME_MANAGER_HPP_