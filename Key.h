//static variables
//Mellotron M400 has 3 sample for each key
const int sound_channels_number = 3;//
////////////////////////////////////////////////
class Key 
{
    public:
        Key();
        Key(const std::string note_name,int note_octave,std::string sample_src = "",int sound_channel = 0);
        int autoload_sample(); //load automatico in base alla nota
        int autoload_tape();
        int load_sample(std::string sample_src,int sound_channel); //load con percorso forzato
        int load_tape(std::string sample_src,int sound_channel);
        void align_head(int channel);
        bool strike(float volume);
        bool release(bool damper);
        bool damp(bool damper);
        unsigned long long getAllocatedMemory();
        std::string note;
        int octave;
    private:
        //da capire in che formato viene letto lo stream audio .wav
        //in modo che sample[(int)sound_channel] corrisponda al sample giusto
        //http://soundfile.sapp.org/doc/WaveFormat/
        //tutti i sample sono MONO 16bit 44100hz
        std::vector<double> sample[sound_channels_number];
        //std::vector<sf::Int16> sample16bit[sound_channels_number];
        sf::Sound head;
        sf::SoundBuffer tape[sound_channels_number];
        unsigned int sample_rate[sound_channels_number];
        unsigned int bit_depth[sound_channels_number];
        unsigned long int sample_length[sound_channels_number]; 
        bool pressed;
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
    for(int c=0;c<sound_channels_number;c++)
    {
        this->sample_length[c] = 0;
        this->bit_depth[c] = 0;
        this->sample_rate[c] = 0;
    }
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
    this->head.stop();
    this->head.setBuffer(this->tape[channel]);
}



bool Key::strike(float volume)
{
    if(this->pressed)
        return false;
    else{
        this->pressed = true;
        std::cout << "\n" << this->note << this->octave << " Key stroke!\n";
        //start playing tape
        this->head.setVolume(volume);
        this->head.play();
    }
    return this->pressed;
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
        this->head.stop();
        std::cout << "\n" << this->note << this->octave << " Key damped!\n";

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
        this->memory_allocated += sizeof(sf::Int16) * this->tape[c].getSampleCount();
    }

    return this->memory_allocated;
}


int Key::load_sample(std::string sample_src,int sound_channel)
{
    std::string path = "./samples/";
    int channel = sound_channel % sound_channels_number;

    AudioFile <double> audioIn;

    if( audioIn.load(path + sample_src) )
    {
        //audioIn.printSummary();

        this->sample_rate[channel] = audioIn.getSampleRate();
        this->bit_depth[channel] = audioIn.getBitDepth();
        this->sample_length[channel] = audioIn.getNumSamplesPerChannel();

        try{    
            this->sample[channel] = audioIn.samples[0];
            this->memory_allocated = sizeof(std::vector<double>) + (sizeof(double) * this->sample[channel].size());
        }catch(std::exception &exc){
            std::cout << "\nError while allocating memory for sample/s.." << exc.what() << "\n";
            return 0;
        }

        return 1;

    }         
    

    return 0;
}

int Key::load_tape(std::string sample_src,int sound_channel)
{
    std::string path = "./samples/";
    int channel = sound_channel % sound_channels_number;

    if( !this->tape[channel].loadFromFile(path + sample_src) )
    {
        std::cout << "\nBuffer not loaded..";
        return 0;
    }

    return 1;
}

int Key::autoload_sample()
{
    bool cycle_exit = false;
    int sounds_loaded = 0;
    char sound = 'a';
    std::string path = "";
    std::string audio_filename = this->note + std::to_string(this->octave) + ".wav";
    //

    do{
        switch(sound)
        {
            case 'a':
                if(this->load_sample(path + "Cello/" + audio_filename,0))
                    sounds_loaded++;
                sound = 'b';
            break;
            case 'b':
                if(this->load_sample(path + "Woodwinds/" + audio_filename,1))
                    sounds_loaded++;
                sound = 'c';
            break;
            case 'c':
                if(this->load_sample(path + "Choir/" + audio_filename,2))
                    sounds_loaded++;
                cycle_exit = true;
            break;
        }

    }while(!cycle_exit);

    return sounds_loaded;
}

int Key::autoload_tape()
{
    bool cycle_exit = false;
    int sounds_loaded = 0;
    char sound = 'a';
    std::string path = "";
    std::string audio_filename = this->note + std::to_string(this->octave) + ".wav";
    //

    do{
        switch(sound)
        {
            case 'a':
                if(this->load_tape(path + "Cello/" + audio_filename,0))
                    sounds_loaded++;
                sound = 'b';
            break;
            case 'b':
                if(this->load_tape(path + "Woodwinds/" + audio_filename,1))
                    sounds_loaded++;
                sound = 'c';
            break;
            case 'c':
                if(this->load_tape(path + "Choir/" + audio_filename,2))
                    sounds_loaded++;
                cycle_exit = true;
            break;
        }

    }while(!cycle_exit);

    return sounds_loaded;
}
//
