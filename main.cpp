#include <iostream>
#include <string>
#include <fstream>
//utils
#include <SFML/Audio.hpp>
//Mellotron Class
#include "Key.h"
#include "Mellotron.h"

using namespace std;


void print_help()
{
    cout << "\nType +/- to change octave\nType s to switch between sounds\nType d to toggle the damper\nType a to release all the keys in current octave\nType l to load different preset sounds\nType L to load custom sounds\nType p to print keyboard info\nType h to print this help\nType x to exit.." << endl;
}

void print_hello()
{
    string line;
    ifstream file("misc/splash_screen.txt"); 
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
    print_hello();
    //piano init
    init_piano_models();
    //
    Mellotron Mellotron(35,"G",2);
    //load std tapes
    cout << "\nLoading tapes.. " << flush;
    cout << Mellotron.load_tapes("M400") << " loaded." << endl;


    //loop for key_testing
    bool loop_continue = true;
    string sound_src = "";
    string preset;
    int channel_new_sound = 0;
    char key_input;
    int octave = 3;
    int sound_channel = 0;
    //mappa per gli input di test da carattere
    unordered_map<char,string> tastiera;
    tastiera['q'] = "C";
    tastiera['2'] = "C#";
    tastiera['w'] = "D";
    tastiera['3'] = "D#";
    tastiera['e'] = "E";
    tastiera['r'] = "F";
    tastiera['5'] = "F#";
    tastiera['t'] = "G";
    tastiera['6'] = "G#";
    tastiera['y'] = "A";
    tastiera['7'] = "A#";
    tastiera['u'] = "B";
    //

    //Mellotron.info();
    //
    print_help();

    do{
        cout << "\nCurrent octave: " << octave ;
        cout << "\nCurrent tape: " << Mellotron.getTapeChannel();
        cout << endl << ">>";
        cin >> key_input;
        
        switch(key_input)
        {
            case 'x':
                loop_continue = false;
            break;
            case 'p':
                Mellotron.info();
            break;
            case 'h':
                print_help();
            break;
            case '+':
                octave++;
            break;
            case '-':
                octave--;
            break;
            case 's':
                Mellotron.setTapeChannel(Mellotron.getTapeChannel() + 1);
            break;
            case 'd':
                if(Mellotron.damperStatus())
                    Mellotron.damperOff();
                else
                    cout << "\n" << Mellotron.damperOn() << " Keys damped..\n";
            break;
            case 'a':
                Mellotron.keyReleaseByNote(tastiera['q'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['2'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['w'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['3'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['e'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['r'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['5'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['t'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['6'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['y'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['7'] + to_string(octave));
                Mellotron.keyReleaseByNote(tastiera['u'] + to_string(octave));
            break;
            case 'L':
                cout << "\nChoose sound and channel:\n";
                cout << "\nSound Path (relative to samples folder) >>";
                cin >> sound_src;
                cout << "\nTape Channel (0,1,2) >>";
                cin >> channel_new_sound;
                cout << "\nLoading tapes.. " << flush;
                cout << Mellotron.load_tapes(sound_src,channel_new_sound) << " sounds loaded.";
                break;
            case 'l':
                cout << "\nChoose Mellotron Preset: ( M400 , MKII , M300 , ensemble ) >>";
                cin >> preset;
                cout << "\nLoading tapes.. " << flush;
                cout << Mellotron.load_tapes(preset) << " sounds loaded.";
            break;
            default:
                Mellotron.keyStrikeByNote(tastiera[key_input] + to_string(octave),100);
        }

    }while(loop_continue);

    return 0;
}