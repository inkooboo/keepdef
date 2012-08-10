#include "logger.hpp"

#include "config.hpp"

#include <boost/thread/tss.hpp>

#include <sstream>
#include <ctime>
#include <cstdio>

namespace logger
{

    boost::thread_specific_ptr<thread_name_t> tss_value;
    FILE *g_log = stdout;

    namespace detail
    {

        void log_str(const log_level_t level, const std::string &to_log)
        {

            const char *thr_name = "noname";
            if (tss_value.get() != NULL)
            {
                thr_name = tss_value->name_.c_str();
            }

            const char *log_level = "dbg";
            switch (level)
            {
                case TRACE:
                    log_level = "trc";
                break;
                case CRITICAL:
                    log_level = "err";
                break;
                default:
                break;
            }

            time_t rawtime;
            struct tm * timeinfo;

            time(&rawtime);
            timeinfo = localtime(&rawtime);

            fprintf(g_log, "%.2d:%.2d:%.2d %s {%s} %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, log_level, thr_name, to_log.c_str());
        }

    } //namespace detail

    detail::rvalue_string log(const log_level_t level)
    {
        detail::rvalue_string ret;
        ret.str_ = new std::ostringstream;
        ret.level_ = level;
        return ret;
    }

    void log(const log_level_t level, const std::string &text)
    {
        detail::log_str(level, text);
    }

    thread_name_t::thread_name_t(const std::string &name) 
        : name_(name)
    {
        if (tss_value.get() != NULL)
        {
            assert("naming named of not empty thread");
        }

        tss_value.reset(this);
    }

    thread_name_t::thread_name_t()
    {
        tss_value.release();
    }

} // namespace logger

logger_t::logger_t()
{
    std::string file_name = master_t::subsystem<config_t>().get<std::string>("log_file");
    
    if (logger::g_log != stdout)
    {
        fclose(logger::g_log);
    }
    if (file_name.empty())
    {
        logger::g_log = stdout;
    }
    else
    {
        FILE *f = fopen(file_name.c_str(), "w");
        logger::g_log = f;
    }
    logger::log(TRACE, "[Logger] Logger system initiated.");
}

logger_t::logger_t()
{
    logger::log(TRACE, "[Logger] Logger system deinitiated.");
    if (logger::g_log != stdout)
    {
        fclose(logger::g_log);
    }
}

