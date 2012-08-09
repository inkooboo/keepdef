//
//  network.cpp
//  keepdef
//
//  Created by inkooboo on 8/9/12.
//
//

#include "network.hpp"


network_t::network_t()
    : m_is_connected(false)
    , m_curl(0)
{
}

bool network_t::is_connected() const
{
    return m_is_connected;
}

int network_t::connect(const char *URL)
{
    disconnect();
    
    m_curl = curl_easy_init();
    if (!m_curl)
    {
        return false;
    }
    if (curl_easy_setopt(m_curl, CURLOPT_URL, URL))
    {
        return false;
    }
    if (curl_easy_setopt(m_curl, CURLOPT_CONNECT_ONLY, 1L))
    {
        return false;
    }
    CURLcode res = curl_easy_perform(m_curl);
    if (!res)
    {
        m_is_connected = true;
    }
    
    return res;
}

void network_t::disconnect()
{
    if (!m_is_connected)
    {
        return;
    }
    
    m_is_connected = false;
    if (m_curl)
    {
        curl_easy_cleanup(m_curl);
    }
    m_curl = 0;
}

size_t network_t::send(const char *data, size_t len)
{
    size_t sent;
    if (curl_easy_send(m_curl, data, len, &sent))
    {
        disconnect();
        return 0;
    }
    
    return sent;
}

void network_t::recv(std::vector<char> &data)
{
    static const int GROW_SIZE = 8192;
    
    data.resize(GROW_SIZE);
    
    char *buf_begin_ptr = &data[0];
    size_t len = 0;
    
    size_t received = 1;
    while (received)
    {
        CURLcode res = curl_easy_recv(m_curl, buf_begin_ptr, data.size() - len, &received);
        
        if (CURLE_AGAIN == res)
        {
            break;
        }
        else if (res)
        {
            disconnect();
            break;
        }
        else if (received)
        {
            len += received;
            data.resize(len + GROW_SIZE);
            buf_begin_ptr = &data[len];
        }
    }
    
    data.resize(len);
}
