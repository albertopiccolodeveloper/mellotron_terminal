#include <iostream>
#include <string>
#include <fstream>
#include "SFML/Window.hpp"
//Mellotron Class
#include "Mellotron.h"

using namespace std;


void print_help()
{
    cout << "\nPress +/- to change volume\nPress left/right arrow to switch between sounds\nPress m to scan again for MIDI devices\nPress l to load different preset sounds\nPress Ins to load custom sounds\nPress p to print keyboard info\nPress h to print this help\nPress esc to exit.." << endl;
}

void print_hello()
{
    string line;
    ifstream file("misc/splash_screen.txt"); 
    cout << "\n";
    if( file.is_open() )
    {
        while(!file.eof())
        {
            getline(file,line);
            cout << line << "\n";
        }
    }  
    cout << "\nby Little Albert, 2018";
    cout << endl;
    file.close();
}

void print_cursor()
{
    cout << endl << ">> ";
}

void init_piano_models()
{
    piano_models["M400"] = 0;
    piano_models["std"] = 0;
    piano_models["MKII"] = 1;
    piano_models["M300"] = 2;
    piano_models["ensemble"] = 3;
}






int main()
{
    sf::Window window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}