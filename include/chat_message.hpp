//
// chat_message.hpp
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Game_State.hpp"
#include "client_action.hpp"

class chat_message
{
public:
    chat_message();
    ~chat_message();
    const char* data() const;
    char* data();
    std::size_t length() const;
    const char* body() const;
    char* body();
    std::size_t body_length() const;
    void body_length(std::size_t new_length);
    bool decode_header();
    void encode_header();

private:
    std::size_t body_length_;

public:

};

#endif
