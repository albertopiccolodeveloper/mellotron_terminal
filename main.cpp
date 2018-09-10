#include <iostream>
#include <string>
//utils
#include "utils/AudioFile.h" //thank you Adam Stark https://github.com/adamstark/AudioFile
#include <SFML/Audio.hpp>
//Piano Class
#include "Key.h"
#include "Piano.h"

using namespace std;


void print_help()
{
    cout << "\nType +/- to change octave\nType l/L to autoload test_key/Mellotron samples\nType d/D to damp test key/toggle the Mellotron damper\nType p to print the keyboard\nType h to print this help\nType x to exit..";
}


int main()
{
    Piano Mellotron(35,"G",2);
    Key test_key;
    

    cout << "\nMellotron emulator v1.0\nby Alberto Piccolo, 2018\n\n";

    //loop for key_testing
    bool loop_continue = true;
    char key_input;
    int octave = 2;
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

    Mellotron.stampa_tastiera();
    //
    print_help();

    do{
        cout << "\nCurrent octave: " << octave ;
        cout << " - >>";
        cin >> key_input;
        
        switch(key_input)
        {
            case 'x':
                loop_continue = false;
            break;
            case 'p':
                Mellotron.stampa_tastiera();
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
            case 'D':
                if(Mellotron.damperStatus())
                    Mellotron.damperOff();
                else
                    cout << "\n" << Mellotron.damperOn() << " Keys damped..\n";
            break;
            case 'l':
                if(test_key.load_tape("Cello/G2.wav",0))
                {
                    test_key.align_head(0);
                }
            break;
            case 'L':
                cout << "\n" << Mellotron.load_tapes() << " sounds loaded..\n";         
                break;
           case 's':
                test_key.strike(100);
           break;
           case 'd':
                test_key.damp(true);
           break;
            default:
                Mellotron.keyStrikeByNote(tastiera[key_input] + to_string(octave),100);
                system("sleep 4s");//simula durata nota
                Mellotron.keyReleaseByNote(tastiera[key_input] + to_string(octave));
        }

    }while(loop_continue);

    return 0;
}