#ifndef _LOGGER_HPP_
# define _LOGGER_HPP_

# include "master.hpp"

# include "noncopyable.hpp"

# include <string>
# include <sstream>
# include <memory>


class logger_t : public subsystem_t
{
public:
    logger_t();
    ~logger_t();
};

enum log_level_t
{
    LOG_DEBUG = 1, LOG_TRACE = 2, LOG_CRITICAL = 3
};

namespace logger
{

    class thread_name_t;

    namespace detail
    {

        /**
         * @brief Internal logging function which perform real logging action.
         */
        void log_str(const log_level_t level, const std::string &to_log);

        /**
         * @brief Useful temporary object that contains current logging message as stream.
         * @remarks
         *      Perform real logging on destruction when non-empty.
         */
        struct rvalue_string
        {
            std::ostringstream *str_;

            log_level_t level_;

            template<typename T>
            rvalue_string operator <<(const T &val)
            {
                *str_ << val;
                return *this;
            }

            rvalue_string() 
            	: str_(0) 
            {
            }
            
            rvalue_string(rvalue_string &other)
            {
            	this->str_ = other.str_;
            	other.str_ = 0;
            	this->level_ = other.level_;
            }
            
            ~rvalue_string()
            {
                try
                {
                    if (str_)
                    {
                        log_str(level_, str_->str());
                        delete str_;
                    }
                }
                catch (...)
                {
                }
            }
        };

    } //namespace detail

    /**
     * @brief Function construct logging stream.
     * @remarks
     *      Used for stream-style logging.
     */
    detail::rvalue_string log(const log_level_t level);

    /**
     * @brief Function perform direct logging of string to log.
     */
    void log(const log_level_t level, const std::string &text);

    /**
     * @brief Sets logging name for thread.
     * @remarks
     *      Uses thread local storage.
     */
    class thread_name_t : private noncopyable_t
    {
    public:
        thread_name_t(const std::string &name);
        thread_name_t();
        std::string name_;
    };

} //namespace logger

#endif //_LOGGER_HPP_
