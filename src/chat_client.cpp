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
Gtk::Entry* entry_player_chat = nullptr;
Gtk::Button* button_send = nullptr;
Gtk::Button* button_clear = nullptr;
Gtk::Image* image_card1 = nullptr;
Gtk::Image* image_card2 = nullptr;
Gtk::Image* image_card3 = nullptr;
Gtk::Image* image_card4 = nullptr;
Gtk::Image* image_card5 = nullptr;
Gtk::MenuItem* help_menu = nullptr;
Gtk::MenuItem* about_menu = nullptr;
Gtk::Label* label_dealer_message = nullptr;
Gtk::Label* total_balance = nullptr;
Gtk::AboutDialog* about_dialog = nullptr;
Gtk::MessageDialog* help_dialog = nullptr;
Gtk::Button* help_close = nullptr;

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
static void on_help_menu_activate()
{
  help_dialog->show();
}

static void on_help_close_clicked()
{
  help_dialog->hide();
}

static void on_about_dialog_activate_link()
{
  about_dialog->show();
}

static void entry_player_chat_activate()
{
  if (window)
  {
    if (entry_player_chat)
    {
      std::cout << entry_player_chat->get_text() + "\n\n";
      entry_player_chat->set_text("");
    }
  }
}

static void on_send_button_clicked()
{
    if(window)
    {
        if (button_send)
        {
            std::cout << "Send Button was clicked.\n\n";
            std::cout << entry_player_chat->get_text() + "\n\n";

            entry_player_chat->set_text("");
        }

    }
}

static void on_clear_button_clicked()
{
    if(window)
    {
        if (button_clear)
        {
          std::cout << "Clear Button was clicked.\n\n";
          entry_player_chat->set_text("");
        }
    }
}

static void player_bet_entry_activate()
{
    if (window)
    {
        if (player_bet_entry)
        {
              int player_balance = 100;

              int entry = std::stoi(player_bet_entry->get_text(),nullptr,0);
              player_balance = player_balance - entry;

              std::cout << "$" + player_bet_entry->get_text() + " bet was set.\n\n";
              std::cout << "current balance is " << player_balance << "\n\n";

              player_bet_entry->set_text("");
          }

    }
}

static void check1_toggled()
{
    if (window)
    {
        if (check1->get_active())
            std::cout << "Check Box 1 checked.\n\n";
        else
            std::cout << "Check Box 1 UNCHECKED.\n\n";
    }
}

static void check2_toggled()
{
    if (window)
    {
      if (check2->get_active())
          std::cout << "Check Box 2 checked.\n\n";
      else
          std::cout << "Check Box 2 UNCHECKED.\n\n";
    }
}

static void check3_toggled()
{
    if (window)
    {
      if (check3->get_active())
          std::cout << "Check Box 3 checked.\n\n";
      else
          std::cout << "Check Box 3 UNCHECKED.\n\n";
    }
}

static void check4_toggled()
{
    if (window)
    {
      if (check4->get_active())
          std::cout << "Check Box 4 checked.\n\n";
      else
          std::cout << "Check Box 4 UNCHECKED.\n\n";
    }
}

static void check5_toggled()
{
    if (window)
    {
      if (check5->get_active())
          std::cout << "Check Box 5 checked.\n\n";
      else
          std::cout << "Check Box 5 UNCHECKED.\n\n";
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
        {
            int player_balance = 100;

            std::cout << "Bet Button was clicked.\n\n";

            if (player_bet_entry->get_text() == "")
              std::cout << "Error. Player bet not set.\n\n";
            else if (std::stoi(player_bet_entry->get_text(),nullptr,0) > player_balance)
              std::cout << "Error. Bet amount is greater than chip balance.\n\n";
            else
            {
              std::cout << "$" + player_bet_entry->get_text() + " bet was set.\n\n";

              int entry = std::stoi(player_bet_entry->get_text(),nullptr,0);
              int current = player_balance - entry;
              std::cout << "current balance is " + current;
            }

            player_bet_entry->set_text("");
        }
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
  std::string assets[52] = {"../assets/10C.png", "../assets/10D.png", "../assets/10H.png", "../assets/10S.png",
                            "../assets/9C.png", "../assets/9D.png", "../assets/9H.png", "../assets/9S.png",
                            "../assets/8C.png", "../assets/8D.png", "../assets/8H.png", "../assets/8S.png",
                            "../assets/7C.png", "../assets/7D.png", "../assets/7H.png", "../assets/7S.png",
                            "../assets/6C.png", "../assets/6D.png", "../assets/6H.png", "../assets/6S.png",
                            "../assets/5C.png", "../assets/5D.png", "../assets/5H.png", "../assets/5S.png",
                            "../assets/4C.png", "../assets/4D.png", "../assets/4H.png", "../assets/4S.png",
                            "../assets/3C.png", "../assets/3D.png", "../assets/3H.png", "../assets/3S.png",
                            "../assets/2C.png", "../assets/2D.png", "../assets/2H.png", "../assets/2S.png",
                            "../assets/AC.png", "../assets/AD.png", "../assets/AH.png", "../assets/AS.png",
                            "../assets/KC.png", "../assets/KD.png", "../assets/KH.png", "../assets/KS.png",
                            "../assets/QC.png", "../assets/QD.png", "../assets/QH.png", "../assets/QS.png",
                            "../assets/JC.png", "../assets/JD.png", "../assets/JH.png", "../assets/JS.png"};

    if (window)
    {
        if (button_replace)
        {
            std::cout << "Replace Button was clicked.\n\n";

            if (check1->get_active())
            {
                int randomIndex = rand() % 53;
                image_card1->set(assets[randomIndex]);

                check1->set_active(false);
            }
            if (check2->get_active())
            {
              int randomIndex = rand() % 53;
              image_card2->set(assets[randomIndex]);

              check2->set_active(false);
            }
            if (check3->get_active())
            {
              int randomIndex = rand() % 53;
              image_card3->set(assets[randomIndex]);

              check3->set_active(false);
            }
            if (check4->get_active())
            {
              int randomIndex = rand() % 53;
              image_card4->set(assets[randomIndex]);

              check4->set_active(false);
            }
            if (check5->get_active())
            {
              int randomIndex = rand() % 53;
              image_card5->set(assets[randomIndex]);

              check5->set_active(false);
            }
        }
    }
}

void on_help_menu_clicked()
{
  if (window)
  {
    if (help_menu)
    {

    }
  }
}

int main(int argc, char* argv[])
{
    int gc = 1;
    auto app = Gtk::Application::create(gc, argv, "gui_mockup.glade");
    auto refBuilder = Gtk::Builder::create();
    refBuilder->add_from_file("gui_mockup.glade");
    refBuilder->get_widget("window", window);

    if (window)
    {
        // WINDOW TITLE and BACKGROUND COLOR
        window->set_title("Poker++");
        window->override_background_color(Gdk::RGBA{"green"});

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
        refBuilder->get_widget("entry_player_chat", entry_player_chat);
        refBuilder->get_widget("button_send", button_send);
        refBuilder->get_widget("button_clear", button_clear);
        refBuilder->get_widget("image_card1", image_card1);
        refBuilder->get_widget("image_card2", image_card2);
        refBuilder->get_widget("image_card3", image_card3);
        refBuilder->get_widget("image_card4", image_card4);
        refBuilder->get_widget("image_card5", image_card5);
        refBuilder->get_widget("help_menu", help_menu);
        refBuilder->get_widget("about_menu", about_menu);
        refBuilder->get_widget("label_dealer_message", label_dealer_message);
        refBuilder->get_widget("total_balance", total_balance);
        refBuilder->get_widget("about_dialog", about_dialog);
        refBuilder->get_widget("help_dialog", help_dialog);
        refBuilder->get_widget("help_close", help_close);

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
        if (entry_player_chat)
        {
          entry_player_chat->signal_activate().connect(sigc::ptr_fun(entry_player_chat_activate));
        }
        if (button_send)
        {
          button_send->signal_clicked().connect(sigc::ptr_fun(on_send_button_clicked));
        }
        if (button_clear)
        {
          button_clear->signal_clicked().connect(sigc::ptr_fun(on_clear_button_clicked));
        }
        if (about_menu)
        {
          about_menu->signal_activate().connect(sigc::ptr_fun(on_about_dialog_activate_link));
        }
        if (help_menu)
        {
          help_menu->signal_activate().connect(sigc::ptr_fun(on_help_menu_activate));
        }
        if (help_close)
        {
          help_close->signal_clicked().connect(sigc::ptr_fun(on_help_close_clicked));
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
