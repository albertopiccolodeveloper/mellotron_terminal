#include <iostream>
#include <string>
//utils
#include <SFML/Audio.hpp>
//Mellotron Class
#include "Key.h"
#include "Mellotron.h"

using namespace std;


void print_help()
{
    cout << "\nType +/- to change octave\nType s to switch between sounds\nType d to toggle the damper\nType a to release all the keys in current octave\nType p to print keyboard info\nType h to print this help\nType x to exit.." << endl;
}

void print_hello()
{
    cout << "\nMellotron emulator v1.0\nby Alberto Piccolo, 2018";
    cout << endl;
}


int main()
{
    print_hello();
    //piano init
    Mellotron M400(35,"G",2);
    //load tapes
    cout << "\nLoading tapes.. " << flush;
    cout << M400.load_tapes() << " loaded." << endl; 


    //loop for key_testing
    bool loop_continue = true;
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

    //M400.info();
    //
    print_help();

    do{
        cout << "\nCurrent octave: " << octave ;
        cout << "\nCurrent tape: " << M400.getTapeChannel() + 1;
        cout << endl << ">>";
        cin >> key_input;
        
        switch(key_input)
        {
            case 'x':
                loop_continue = false;
            break;
            case 'p':
                M400.info();
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
                M400.setTapeChannel(M400.getTapeChannel() + 1);
            break;
            case 'd':
                if(M400.damperStatus())
                    M400.damperOff();
                else
                    cout << "\n" << M400.damperOn() << " Keys damped..\n";
            break;
            case 'a':
                M400.keyReleaseByNote(tastiera['q'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['2'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['w'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['3'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['e'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['r'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['5'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['t'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['6'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['y'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['7'] + to_string(octave));
                M400.keyReleaseByNote(tastiera['u'] + to_string(octave));
            break;
            case 'l':
                cout << "\nnope.";       
                break;
            default:
                M400.keyStrikeByNote(tastiera[key_input] + to_string(octave),100);
                //system("sleep 4s");//simula durata nota
                //M400.keyReleaseByNote(tastiera[key_input] + to_string(octave));
        }

    }while(loop_continue);

    return 0;
}