#ifndef _THREAD_POOL_HPP_
# define _THREAD_POOL_HPP_

# include "../Classes/master.hpp"

# include "logger.hpp"

# include <boost/thread.hpp>
# include <boost/asio.hpp>
# include <vector>
# include <iterator>
# include <sstream>

class ThreadPool : public Subsystem
{
    friend class Master;

    explicit ThreadPool(Master *master)
        : Subsystem(master)
    {
    }

    virtual void start()
    {
        _work = new boost::asio::io_service::work(_svc);

        size_t threads = master().subsystem<Config>()["threads"];
        if (threads < 1)
        {
            threads = 2;
        }

        logger::log(TRACE) << "[Thread pool] Using " << threads << " threads";

        _threads.resize(threads -1);

        for (std::vector<boost::thread>::iterator i = _threads.begin(); i != _threads.end(); ++i)
        {
            *i = boost::thread(&ThreadPool::worker_thread_func, this, static_cast<size_t>(std::distance(_threads.begin(), i)));
        }

    }

    virtual void stop()
    {
        delete _work;

        for (std::vector<boost::thread>::iterator i = _threads.begin(); i != _threads.end(); ++i)
        {
            if (i->joinable())
            {
                i->join();
            }
        }

    }

public:

    void join_thread_pool()
    {
        logger::log(TRACE, "[Thread pool] Thread joined to thread pool");
        _svc.run();
    }

    boost::asio::io_service & get_io_service()
    {
        return _svc;
    }

private:

    void worker_thread_func(size_t n)
    {
        std::ostringstream name;
        name << n;
        logger::ThreadName this_thread(name.str());
        join_thread_pool();
    }

    boost::asio::io_service _svc;
    boost::asio::io_service::work *_work;

    std::vector<boost::thread> _threads;
};


#endif //_THREAD_POOL_HPP_
