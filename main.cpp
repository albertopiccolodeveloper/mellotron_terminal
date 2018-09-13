#include <iostream>
#include <string>
#include <fstream>
//Mellotron Class
#include "Mellotron.h"

using namespace std;


void print_help()
{
    cout << "\nType 1 to 9 (0 = 100%) to set volume\nType +/- to change pitch\nType </> to switch between tape channel\nType m to scan again for MIDI devices\nType l to load different preset sounds\nType L to load custom sounds\nType p to print keyboard info\nType h to print this help\nType x to exit.." << endl;
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
    //check for MIDI
    cout << "\nChecking for MIDI input.. " << flush;
    Mellotron.check_for_MIDI();
    cout << endl;

    // loop variable
    bool exit_cycle = false;
    //

    //loop for key_testing
    string sound_src = "";
    string preset;
    int channel_new_sound = 0;
    char key_input;
    int octave = 3;
    int sound_channel = 0;
    float volume = 100;
    //

    //Mellotron.info();
    //
    print_help();

    do{
        cout << "\nTape Channel: " << Mellotron.getTapeChannel() + 1;
        cout << "\nVolume: " << Mellotron.getVolume() << " %";
        cout << "\nPitch: " << Mellotron.getPitch();
        cout << endl << ">> ";
        cin >> key_input;
        
        switch(key_input)
        {
            case 'p':
                Mellotron.info();
            break;
            case 'h':
                print_help();
            break;
            case 'm':
                cout << "\nChecking for MIDI input.. " << flush;
                Mellotron.check_for_MIDI();
                cout << endl;
            break;
            case '1':
                Mellotron.setVolume(10);
            break;
            case '2':
                Mellotron.setVolume(20);
            break;
            case '3':
                Mellotron.setVolume(30);
            break;
            case '4':
                Mellotron.setVolume(40);
            break;
            case '5':
                Mellotron.setVolume(50);
            break;
            case '6':
                Mellotron.setVolume(60);
            break;
            case '7':
                Mellotron.setVolume(70);
            break;
            case '8':
                Mellotron.setVolume(80);
            break;
            case '9':
                Mellotron.setVolume(90);
            break;
            case '0':
                Mellotron.setVolume(100);
            break;
            case '+':
                Mellotron.raisePitch();
            break;
            case '-':
                Mellotron.lowerPitch();
            break;
            case '>':
                sound_channel = Mellotron.getTapeChannel();
                if(sound_channel < (sound_channels_number - 1))
                    Mellotron.setTapeChannel(sound_channel + 1);
            break;
            case '<':
                sound_channel = Mellotron.getTapeChannel();
                if(sound_channel > 0)
                    Mellotron.setTapeChannel(sound_channel - 1);
            break;
            case 'L':
                cout << "\nChoose sound and channel:\n";
                cout << "\nSound Path (relative to samples folder) >>";
                cin >> sound_src;
                cout << "\nTape Channel (1,2,3) >>";
                cin >> channel_new_sound;
                cout << "\nLoading tapes.. " << flush;
                cout << Mellotron.load_tapes(sound_src,channel_new_sound - 1) << " sounds loaded.";
                break;
            case 'l':
                cout << "\nChoose Mellotron Preset: ( M400 , MKII , M300 , ensemble ) >>";
                cin >> preset;
                cout << "\nLoading tapes.. " << flush;
                cout << Mellotron.load_tapes(preset) << " sounds loaded.";
            break;
            case 'x':
                exit_cycle = true;
            break;
            default:
                //nope
                cout << "\nNo command found..";
                print_help();
        }

    }while(!exit_cycle);

    return 0;
}