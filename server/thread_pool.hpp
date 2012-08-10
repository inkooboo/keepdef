#ifndef _THREAD_POOL_HPP_
# define _THREAD_POOL_HPP_

# include "master.hpp"

# include "config.hpp"
# include "logger.hpp"

# include <boost/asio.hpp>

# include <thread>
# include <memory>
# include <vector>
# include <iterator>
# include <sstream>

class thread_pool_t : public subsystem_t
{
    virtual void start()
    {
        m_work.reset(new boost::asio::io_service::work(m_svc));

        size_t threads = master_t::subsystem<config_t>()["threads"];
        if (threads < 1)
        {
            threads = 2;
        }

        logger::log(TRACE) << "[Thread pool] Using " << threads << " threads";

        m_threads.resize(threads -1);

        size_t thread_index = 0;
        for (auto &thr : m_threads)
        {
            thr = std::thread(&thread_pool_t::worker_thread_func, this, thread_index);
            ++thread_index;
        }

    }

    virtual void stop()
    {
        m_work.reset();

        for (auto &thr : m_threads)
        {
            if (thr.joinable())
            {
                thr.join();
            }
        }

    }

public:

    void join_thread_pool()
    {
        logger::log(TRACE, "[Thread pool] Thread joined to thread pool");
        m_svc.run();
    }

    boost::asio::io_service & io_service()
    {
        return m_svc;
    }

private:

    void worker_thread_func(size_t n)
    {
        std::ostringstream name;
        name << n;
        logger::thread_name_t this_thread(name.str());
        join_thread_pool();
    }

    boost::asio::io_service m_svc;
    std::unique_ptr<boost::asio::io_service::work> m_work;

    std::vector<boost::thread> m_threads;
};


#endif //_THREAD_POOL_HPP_
