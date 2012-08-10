#include "master.hpp"
#include "config.hpp"

#include "logger.hpp"
#include "server.hpp"
#include "thread_pool.hpp"
#include "game_manager.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

int main(int argc, char* argv[])
{
    logger::thread_name_t this_thread("Main");

    srand((unsigned int)time(0));

    try
    {
        master_t master;

        master.add_subsystem<config_t>();
        master.add_subsystem<logger_t>();
        master.add_subsystem<thread_pool_t>();
        master.add_subsystem<server_t>();
        master.add_subsystem<game_manager_t>();

        master.start();

        master.subsystem<thread_pool_t>().join_thread_pool();

        master.stop();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
