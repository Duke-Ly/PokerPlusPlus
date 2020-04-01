#include <gtkmm.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <ctime>
#include "json.hpp"
#include "asio.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;
using json = nlohmann::json;

Gtk::Window* window = nullptr;
Gtk::Button* button_check = nullptr;
Gtk::Button* button_call = nullptr;
Gtk::Button* button_fold = nullptr;
Gtk::Button* button_bet = nullptr;
Gtk::Button* button_raise = nullptr;
Gtk::Button* button_all_in = nullptr;
Gtk::Button* button_replace = nullptr;
Gtk::CheckButton* check1 = nullptr;
Gtk::CheckButton* check2 = nullptr;
Gtk::CheckButton* check3 = nullptr;
Gtk::CheckButton* check4 = nullptr;
Gtk::CheckButton* check5 = nullptr;
Gtk::Entry* player_bet_entry = nullptr;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints)
        : io_context_(io_context),
          socket_(io_context)
    {
        // CSE3310 This is where the program call the function where connection is established with the server
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
        // CSE3310 This is where the program connection is established with the server
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

static void player_bet_entry_activate()
{
    if (window)
    {
        if (player_bet_entry)
        {
            std::cout << "$" + player_bet_entry->get_text() + " bet was set.\n\n";
        }
    }
}

static void check1_toggled()
{
    if (window)
    {
        if (check1)
            std::cout << "Check Box 1 checked.\n\n";
    }
}

static void check2_toggled()
{
    if (window)
    {
        if (check2)
            std::cout << "Check Box 2 checked.\n\n";
    }
}

static void check3_toggled()
{
    if (window)
    {
        if (check3)
            std::cout << "Check Box 3 checked.\n\n";
    }
}

static void check4_toggled()
{
    if (window)
    {
        if (check4)
            std::cout << "Check Box 4 checked.\n\n";
    }
}

static void check5_toggled()
{
    if (window)
    {
        if (check5)
            std::cout << "Check Box 5 checked.\n\n";
    }
}

static void on_check_button_clicked()
{
    if(window)
    {
        if (button_check)
            std::cout << "Check Button was clicked.\n\n";
    }
}

static void on_call_button_clicked()
{
    if (window)
    {
        if (button_call)
            std::cout << "Call Button was clicked.\n\n";
    }
}

static void on_fold_button_clicked()
{
    if (window)
    {
        if (button_fold)
            std::cout << "Fold Button was clicked.\n\n";
    }
}

static void on_bet_button_clicked()
{
    if (window)
    {
        if (button_bet)
            std::cout << "Bet Button was clicked.\n\n";
    }
}

static void on_raise_button_clicked()
{
    if (window)
    {
        if (button_raise)
            std::cout << "Raise Button was clicked.\n\n";
    }
}

static void on_all_in_button_clicked()
{
    if (window)
    {
        if (button_all_in)
            std::cout << "All-in Button was clicked.\n\n";
    }
}

static void on_replace_button_clicked()
{
    if (window)
    {
        if (button_replace)
            std::cout << "Replace Button was clicked.\n\n";
    }
}

int main(int argc, char* argv[])
{
    int gc = 1;
    auto app = Gtk::Application::create(gc, argv, "Poker++");
    auto refBuilder = Gtk::Builder::create();
    refBuilder->add_from_file("../assets/gui_mockup.glade");
    refBuilder->get_widget("window", window);

    if (window)
    {
        refBuilder->get_widget("button_check", button_check);
        refBuilder->get_widget("button_call", button_call);
        refBuilder->get_widget("button_fold", button_fold);
        refBuilder->get_widget("button_bet", button_bet);
        refBuilder->get_widget("button_raise", button_raise);
        refBuilder->get_widget("button_all_in", button_all_in);
        refBuilder->get_widget("button_replace", button_replace);
        refBuilder->get_widget("check1", check1);
        refBuilder->get_widget("check2", check2);
        refBuilder->get_widget("check3", check3);
        refBuilder->get_widget("check4", check4);
        refBuilder->get_widget("check5", check5);
        refBuilder->get_widget("player_bet_entry", player_bet_entry);

        if (button_check)
        {
            button_check->signal_clicked().connect(sigc::ptr_fun(on_check_button_clicked));
        }
        if (button_call)
        {
            button_call->signal_clicked().connect(sigc::ptr_fun(on_call_button_clicked));
        }
        if (button_fold)
        {
            button_fold->signal_clicked().connect(sigc::ptr_fun(on_fold_button_clicked));
        }
        if (button_bet)
        {
            button_bet->signal_clicked().connect(sigc::ptr_fun(on_bet_button_clicked));
        }
        if (button_raise)
        {
            button_raise->signal_clicked().connect(sigc::ptr_fun(on_raise_button_clicked));
        }
        if (button_all_in)
        {
            button_all_in->signal_clicked().connect(sigc::ptr_fun(on_all_in_button_clicked));
        }
        if (button_replace)
        {
            button_replace->signal_clicked().connect(sigc::ptr_fun(on_replace_button_clicked));
        }
        if (check1)
        {
            check1->signal_clicked().connect(sigc::ptr_fun(check1_toggled));
        }
        if (check2)
        {
            check2->signal_clicked().connect(sigc::ptr_fun(check2_toggled));
        }
        if (check3)
        {
            check3->signal_clicked().connect(sigc::ptr_fun(check3_toggled));
        }
        if (check4)
        {
            check4->signal_clicked().connect(sigc::ptr_fun(check4_toggled));
        }
        if (check5)
        {
            check5->signal_clicked().connect(sigc::ptr_fun(check5_toggled));
        }
        if (player_bet_entry)
        {
            player_bet_entry->signal_activate().connect(sigc::ptr_fun(player_bet_entry_activate));
        }
    }

    if (argc != 3)
    {
        std::cerr << "Usage: chat_client <host> <port>\n";
        return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context]()
    {
        io_context.run();
    });
    app->run(*window);
    c.close();
    t.join();

    delete window;
    return 0;
}
