#ifndef  _NETWORK_HPP_
# define  _NETWORK_HPP_

# include "movable_network_data.hpp"

# include <curl.h>

class Network
{
public:
    Network()
        : _is_connected(false)
        , _curl(0)
    {
    }

    inline bool is_connected()
    {
        return _is_connected;
    }

    int connect(const char *URL)
    {
        disconnect();

        _curl = curl_easy_init();
        if (!_curl)
        {
            return false;
        }
        if (curl_easy_setopt(_curl, CURLOPT_URL, URL))
        {
            return false;
        }
        if (curl_easy_setopt(_curl, CURLOPT_CONNECT_ONLY, 1L))
        {
            return false;
        }
		CURLcode res = curl_easy_perform(_curl);
        if (!res)
        {
	        _is_connected = true;
        }

        return res;
    }

    void disconnect()
    {
        if (!_is_connected)
        {
            return;
        }

        _is_connected = false;
        if (_curl)
        {
            curl_easy_cleanup(_curl);
        }
        _curl = 0;
    }

    size_t send(const char *data, size_t len)
    {
        size_t sent;
        CURLcode res = CURLE_OK;
        if (curl_easy_send(_curl, data, len, &sent))
        {
            disconnect();
            return 0;
        }

        return sent;
    }

    void recv(MovableNetworkData &data)
    {
        char buf[8192];

        size_t received = 1;
        while(received)
        {
            CURLcode res = curl_easy_recv(_curl, buf, sizeof(buf), &received);
            if (CURLE_AGAIN == res)
            {
                break;
            }
            else if (res)
            {
                disconnect();
                break;
            }
            else
            {
                data.add(buf, received);
            }
        }
    }

private:
    volatile bool _is_connected;

    CURL *_curl;
};

#endif //_NETWORK_HPP_

