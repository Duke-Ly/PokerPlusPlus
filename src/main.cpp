#include <gtkmm.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <ctime>
#include <assert.h>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"
#include "player_gui.hpp"

using asio::ip::tcp;
using json = nlohmann::json;
using namespace Gtk;
using namespace std;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints)
        : io_context_(io_context),
          socket_(io_context)
    {
        do_connect(endpoints);
    }

    void write(const chat_message& msg)
    {
        asio::post(io_context_,
                   [this, msg]()
        {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
            if (!write_in_progress)
            {
                do_write();
            }
        });
    }

    void close()
    {
        asio::post(io_context_, [this]()
        {
            socket_.close();
        });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints)
    {
        asio::async_connect(socket_, endpoints,
            [this](std::error_code ec, tcp::endpoint)
        {
            if (!ec)
            {
                do_read_header();
            }
        });
    }

    void do_read_header()
    {
        // CSE3310 This is where message body+header is received from the server
        asio::async_read(socket_,
                         asio::buffer(read_msg_.data(), chat_message::header_length),
                         [this](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_msg_.decode_header())
            {
                do_read_body();
            }
            else
            {
                socket_.close();
            }
        });
    }

    void do_read_body()
    {
        // CSE3310 This is where message body is received from the server
        asio::async_read(socket_,
                         asio::buffer(read_msg_.body(), read_msg_.body_length()),
                         [this](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
               char outline[read_msg_.body_length() + 2];
                                       // '\n' + '\0' is 2 more chars
               outline[0] = '\n';
               outline[read_msg_.body_length() + 1] = '\0';
               std::memcpy ( &outline[1], read_msg_.body(), read_msg_.body_length() );

               std::string p = "player "+ std::to_string (read_msg_.gs.player_cards[0][0]) + " " +
                                         std::to_string (read_msg_.gs.player_cards[0][1]) + " " +
                                         std::to_string (read_msg_.gs.player_cards[0][2]) + '\n' +
                                         std::to_string (read_msg_.gs.player_cards[1][0]) + " " +
                                         std::to_string (read_msg_.gs.player_cards[1][1]) + " " +
                                         std::to_string (read_msg_.gs.player_cards[1][2]);

               std::string d = "dealer " + std::to_string (read_msg_.gs.dealer_cards[0]) + " " +
                                          std::to_string (read_msg_.gs.dealer_cards[1]) + " " +
                                          std::to_string (read_msg_.gs.dealer_cards[2]);

               if (read_msg_.gs.player_cards_valid)
                  std::cout<<"player_cards_valid"<<endl;
               else
                  std::cout<<"waiting"<<endl;

               if (read_msg_.gs.dealer_cards_valid)
                  std::cout<<"player_cards_valid"<<endl;
               else
                  std::cout<<"waiting"<<endl;

                std::cout.write(read_msg_.body(), read_msg_.body_length());
                std::cout << "\n";
                do_read_header();
            }
            else
            {
                socket_.close();
            }
        });
    }

    void do_write()
    {
        // CSE3310 This is where message is sent to the chat server
        asio::async_write(socket_,
                          asio::buffer(write_msgs_.front().data(),
                                       write_msgs_.front().length()),
                          [this](std::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                socket_.close();
            }
        });
    }

private:
    asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

// global symbols
chat_client *c;

int main(int argc, char* argv[])
{
    Main kit(argc,argv);
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui_mockup.glade");
    Player_GUI *player_gui = nullptr;
    builder->get_widget_derived("window", player_gui);

    if (argc != 3)
    {
        std::cerr << "Usage: chat_client <host> <port>\n";
        return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    c = new chat_client(io_context, endpoints);

    std::thread t([&io_context]()
    {
        io_context.run();
    });
    kit.run(*player_gui);
    c->close();
    t.join();

    delete player_gui;
    return 0;
}
