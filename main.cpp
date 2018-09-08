#include <iostream>
#include <string>
//utils

//Piano Class
#include "Key.h"
#include "Piano.h"

using namespace std;


void print_help()
{
    cout << "\nType +/- to change octave\nType s to autoload piano samples\nType d to toggle the damper\nType l to test sample loading\nType p to print the keyboard\nType h to print this help\nType n for nop\nType x to exit..";
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
            case 'd':
                if(Mellotron.damperStatus())
                    Mellotron.damperOff();
                else
                    cout << "\n" << Mellotron.damperOn() << " Keys damped..\n";
            break;
            case 'n':
                //nope
            break;
            case 'l':
                test_key.load_sample("Cello/G2.wav",0);
            break;
            case 's':
                cout << "\n" << Mellotron.load_sounds() << " sounds loaded..\n";         
                break;
            default:
                Mellotron.keyStrikeByNote(tastiera[key_input] + to_string(octave),100);
                system("sleep 4s");//simula durata nota
                Mellotron.keyReleaseByNote(tastiera[key_input] + to_string(octave));
        }

    }while(loop_continue);

    return 0;
}