//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "chat_message.hpp"
#include "Game_State.hpp"
#include "client_action.hpp"

chat_message::chat_message()
    : body_length_(0) {};

chat_message::~chat_message();

const char* chat_message::data() const
{
    return data_;
}

char* chat_message::data()
{
    return data_;
}

std::size_t chat_message::length() const
{
    return header_length + body_length_;
}

const char* chat_message::body() const
{
    return data_ + header_length;
}

char* chat_message::body()
{
    return data_ + header_length;
}

std::size_t chat_message::body_length() const
{
    return body_length_;
}

void chat_message::body_length(std::size_t new_length)
{
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

bool chat_message::decode_header()
{
    char header[header_length + 1] = "";
    std::strncat(header, data_, 4);
    body_length_ = std::atoi(header);
    char *p = data_ + 4; // skip the integer
    std::memcpy(&ca,p,sizeof(client_action));
    std::memcpy(&gs,p+sizeof(client_action),sizeof(game_state));

    if (body_length_ > max_body_length)
    {
        body_length_ = 0;
        return false;
    }
    return true;
}

void chat_message::encode_header()
{
    char header[4 + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, 4);

    char *p = data_+4; // skip over the int we just extracted
    std::memcpy(p,&ca,sizeof(client_action));
    std::memcpy(p+sizeof(client_action),&gs,sizeof(game_state));
}
