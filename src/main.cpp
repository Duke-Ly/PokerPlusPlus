#include <iostream>
#include <gtkmm.h>

Gtk::Window* window = nullptr;

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create(argc, argv, "Poker++");

  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("gui_mockup.glade");
  }
  catch (const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch (const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch (const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  refBuilder->get_widget("window", window);

  if (window)
    app->run(*window);

  delete window;

  
  return 0;
}
