#include "../Classes/master.hpp"
#include "server_config.hpp"

#include "logger.hpp"
#include "server.hpp"
#include "thread_pool.hpp"
#include "game_manager.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

int main(int argc, char* argv[])
{
    logger::ThreadName this_thread("Main");

    srand((unsigned int)time(0));

    try
    {
        Master master;

        master.add_subsystem<Config>();
        master.add_subsystem<Logger>();
        master.add_subsystem<ThreadPool>();
        master.add_subsystem<Server>();
        master.add_subsystem<GameManager>();

        master.start();

        master.subsystem<ThreadPool>().join_thread_pool();

        master.stop();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
