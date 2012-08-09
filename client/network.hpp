#ifndef  _NETWORK_HPP_
# define  _NETWORK_HPP_

# include <curl.h>

# include <vector>

class network_t
{
public:
    network_t();

    bool is_connected() const;

    int connect(const char *URL);

    void disconnect();

    size_t send(const char *data, size_t len);

    void recv(std::vector<char> &data);

private:
    bool m_is_connected;

    CURL *m_curl;
};

#endif //_NETWORK_HPP_

