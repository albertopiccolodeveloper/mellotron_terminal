#include <irrKlang/irrKlang.h>
//static variables
//Mellotron M400 has 3 sample for each key
const int sound_channels_number = 3;//
////////////////////////////////////////////////
class Key 
{
    public:
        Key();
        Key(const std::string note_name,int note_octave,std::string sample_src = "",int sound_channel = 0);
        ~Key();
        //
        int autoload_tape(int model = 0);//load automatico in base alla nota
        int load_tape(std::string sample_src,int sound_channel);//load con percorso forzato
        void align_head(int channel);
        //
        irrklang::ISound * strike(float volume);
        bool release(bool damper);
        bool damp(bool damper);
        //
        unsigned long long getAllocatedMemory();
        //public data
        std::string note;
        int octave;
    private:
        //sound variables
        //sf::Sound head;
        irrklang::ISound * tape[sound_channels_number];
        //sf::SoundBuffer tape[sound_channels_number];
        //logic variables
        bool pressed;
        int tape_channel;
        //
        unsigned long long memory_allocated;
};

//metodi classe Key
Key::Key()
{
    //valori nota più bassa
    this->note = "G";
    this->octave = 2;
    this->pressed = false;
    this->memory_allocated = 0;
    //init samples
}

Key::Key(const std::string note_name,int note_octave,std::string sample_src,int sound_channel)
{
    this->note = note_name;
    this->octave = note_octave;
    this->pressed = false;
    this->memory_allocated = 0;
    //loads sample
    if(sample_src != ""){
        if( this->load_tape(sample_src,sound_channel) )
            this->align_head(sound_channel);
    }
        
}

void Key::align_head(int channel)
{
    //this->head.stop();
    //this->head.setBuffer(this->tape[channel]);
    this->tape_channel = channel;
}



irrklang::ISound * Key::strike(float volume)
{
    if(this->pressed)
        return 0;
    else{
        this->pressed = true;
        //std::cout << "\n" << this->note << this->octave << " Key stroke!\n";
        //start playing tape
        //this->head.setVolume(volume);
        //this->head.play();
        this->tape[this->tape_channel]->setVolume(volume);
        
    }
    return this->tape[this->tape_channel];
}

bool Key::release(bool damper)
{
    this->pressed = false;
    //
    return this->damp(damper);
}

bool Key::damp(bool damper)
{
    if(!this->pressed && damper){
        //stop playing tape and rewind
        //this->head.setVolume(0);
        //this->head.pause();
        //this->head.setPlayingOffset(sf::Time::Zero);
        //this->head.stop();
        this->tape[this->tape_channel]->setIsPaused();
        this->tape[this->tape_channel]->setPlayPosition(0);
        
        //std::cout << "\n" << this->note << this->octave << " Key damped!\n";

        return true;
    }

    return false;
}

unsigned long long Key::getAllocatedMemory()
{
    this->memory_allocated = 0;

    for(int c = 0; c < sound_channels_number; c++)
    {
        this->memory_allocated += sizeof(this->tape[c]);
        //this->memory_allocated += sizeof(sf::Int16) * this->tape[c].getSampleCount();
    }

    return this->memory_allocated;
}


int Key::load_tape(irrklang::ISound * pSound,int channel)
{
    

    /*
    if( !this->tape[channel].loadFromFile(path + sample_src) )
    {
        std::cout << "\nBuffer not loaded..";
        return 0;
    }
    */

   if( !pSound )
   {
        std::cout << "\nBuffer not loaded..";
        return 0;
   }

   this->tape[channel] = pSound;

    return 1;
}

int Key::autoload_tape(int model)
{
    bool cycle_exit = false;
    int sounds_loaded = 0;
    char sound = 'a';
    std::string path = "";
    std::string audio_filename = "/" + this->note + std::to_string(this->octave) + ".wav";
    //
    switch(model)
    {
        case 0:
            //Mellotron M400 std preset
            do{
                switch(sound)
                {
                    case 'a':
                        if(this->load_tape(path + "MK2_Flute" + audio_filename,0))
                            sounds_loaded++;
                        sound = 'b';
                    break;
                    case 'b':
                        if(this->load_tape(path + "String_Section" + audio_filename,1))
                            sounds_loaded++;
                        sound = 'c';
                    break;
                    case 'c':
                        if(this->load_tape(path + "Cello" + audio_filename,2))
                            sounds_loaded++;
                        cycle_exit = true;
                    break;
                }

            }while(!cycle_exit);
            //
        break;
        case 1:
            //Mellotron MK II
            do{
                switch(sound)
                {
                    case 'a':
                        if(this->load_tape(path + "MK2_Flute" + audio_filename,0))
                            sounds_loaded++;
                        sound = 'b';
                    break;
                    case 'b':
                        if(this->load_tape(path + "MK2_Violins" + audio_filename,1))
                            sounds_loaded++;
                        sound = 'c';
                    break;
                    case 'c':
                        if(this->load_tape(path + "MK2_Brass" + audio_filename,2))
                            sounds_loaded++;
                        cycle_exit = true;
                    break;
                }

            }while(!cycle_exit);
            //
        break;
        case 2:
            //Mellotron M300
            do{
                switch(sound)
                {
                    case 'a':
                        if(this->load_tape(path + "M300A" + audio_filename,0))
                            sounds_loaded++;
                        sound = 'b';
                    break;
                    case 'b':
                        if(this->load_tape(path + "M300B" + audio_filename,1))
                            sounds_loaded++;
                        sound = 'c';
                    break;
                    case 'c':
                        if(this->load_tape(path + "M300_Brass" + audio_filename,2))
                            sounds_loaded++;
                        cycle_exit = true;
                    break;
                }

            }while(!cycle_exit);
            //
        break;
        case 3:
            //Mellotron ensemble sounds
            do{
                switch(sound)
                {
                    case 'a':
                        if(this->load_tape(path + "Woodwinds" + audio_filename,0))
                            sounds_loaded++;
                        sound = 'b';
                    break;
                    case 'b':
                        if(this->load_tape(path + "String_Section" + audio_filename,1))
                            sounds_loaded++;
                        sound = 'c';
                    break;
                    case 'c':
                        if(this->load_tape(path + "Choir" + audio_filename,2))
                            sounds_loaded++;
                        cycle_exit = true;
                    break;
                }

            }while(!cycle_exit);
            //
        break;
        default:
            std::cout << "\nPreset not found!";
        break;
    }

    //presets end


    return sounds_loaded;
}
//destructor
Key::~Key()
{
    /*
    this->head.stop();
    this->head.~Sound();
    */
}
