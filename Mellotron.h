#include <unordered_map>
#include "utils/RtMidi.h"
#include "Key.h"
//
const std::string key_note_names[12] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
//definita poi nel main, da rivedere
std::unordered_map<std::string,int> piano_models; 
 //fine variabili statiche
//////////////////////////////////////////
class Mellotron
{
    public:
        Mellotron(const unsigned int keys,std::string lowest_note,int lowest_octave);
        ~Mellotron();
        //
        int load_tapes(std::string model = "M400");
        int load_tapes(std::string sound_src,int channel);
        //
        int keyStrikeByNote(std::string note_name,float dynamic);
        int keyStrikeByKeyN(int key_number,float dynamic);
        int keyStrikeByMidiInterface(int midi_input,float dynamic);
        //
        int keyReleaseByNote(std::string note_name);
        int keyReleaseByKeyN(int key_number);
        int keyReleaseByMidiInterface(int midi_input);
        //
        void setTapeChannel(int channel_number);
        int getTapeChannel();
        //
        int damperOn();
        void damperOff();
        bool damperStatus();
        //
        void raiseVolume();
        void lowerVolume();
        void setVolume(unsigned int vol);
        unsigned int getVolume();
        //
        void raisePitch();
        void lowerPitch();
        float getPitch();
        void resetPitch();
        //
        void info();
        //MIDI handlers
        int check_for_MIDI();
    private:
        Key keyboard[35];
        //std::vector<Key> keyboard;
        //array associativo per accesso nome nota,numero tasto
        std::unordered_map<std::string,int> keyboard_map;
        std::unordered_map<int,int> midi_key_map;
        bool damper;
        unsigned int general_volume;
        float pitch;
        int key_n;
        int sound_channel;
        //MIDI data
        RtMidiIn *midiin;
        //MIDI callback
        static void MIDIreadCallback(double deltatime, std::vector< unsigned char > *message,void *userData);
};


//metodi classe Mellotron
Mellotron::Mellotron(const unsigned int keys,std::string lowest_note,int lowest_octave)
{
    this->key_n = keys;
    this->damper = true;
    this->sound_channel = 0;
    this->midiin = 0;
    this->general_volume = 50;
    this->pitch = 1.00;

    int d = 0;
    
    //finds out with wich note has to start to enum keys
    for(int x=0;x<sizeof(key_note_names);x++)
    {
        if(key_note_names[x] == lowest_note)
        {
            d = x;
        }
    }
    //
    

    Key temp;
    int temp_midi_key = 0; 
    //
    for(int c=0;c<keys;c++)
    {
        temp.note = key_note_names[d % 12];
        temp.octave = lowest_octave + (d / 12);
        temp_midi_key = ((lowest_octave + (d / 12)) * 12) + (d % 12);
        //note insert
        this->keyboard[c].note = temp.note;
        this->keyboard[c].octave = temp.octave;
        //mapping
        this->keyboard_map[temp.note + std::to_string(temp.octave)] = c;
        this->midi_key_map[temp_midi_key] = c;
        d++;
    }

}

int Mellotron::keyStrikeByKeyN(int key_number,float dynamic)
{
    //ignore dynamic key press
    return this->keyboard[key_number].strike(this->general_volume,this->pitch);
}

int Mellotron::keyStrikeByNote(std::string note_name,float dynamic)
{
    //ignore dynamic key press
    try{
        return this->keyboard[this->keyboard_map.at(note_name)].strike(this->general_volume,this->pitch);
    }catch(const std::out_of_range& oor){
        //std::cout << "\nKey not playable - (" << oor.what() <<") Out of Range Exception!";
        return 0;
    }
}

int Mellotron::keyStrikeByMidiInterface(int midi_input,float dynamic)
{
    //ignore dynamic key press
    try{
        return this->keyboard[this->midi_key_map.at(midi_input)].strike(this->general_volume,this->pitch);
    }catch(const std::out_of_range& oor){
        //std::cout << "\nKey not playable - (" << oor.what() <<") Out of Range Exception!";
        return 0;
    }
}

int Mellotron::keyReleaseByKeyN(int key_number)
{
    return this->keyboard[key_number].release(this->damper);
}

int Mellotron::keyReleaseByNote(std::string note_name)
{
    try{
        return this->keyboard[this->keyboard_map.at(note_name)].release(this->damper);
    }catch(const std::out_of_range& oor){
        //std::cout << "\nKey not playable - (" << oor.what() <<") Out of Range Exception!";
        return -1;
    }
}

int Mellotron::keyReleaseByMidiInterface(int midi_input)
{
    try{
        return this->keyboard[this->midi_key_map.at(midi_input)].release(this->damper);
    }catch(const std::out_of_range& oor){
        //std::cout << "\nKey not playable - (" << oor.what() <<") Out of Range Exception!";
        return 0;
    }
}

void Mellotron::setTapeChannel(int channel_number)
{
    this->sound_channel = channel_number % sound_channels_number;
    for(int c=0; c < this->key_n; c++)
    {
        this->keyboard[c].align_head(this->sound_channel);
    }
}

int Mellotron::getTapeChannel()
{
    return this->sound_channel;
}

void Mellotron::damperOff()
{
    this->damper = false;
}

int Mellotron::damperOn()
{
    this->damper = true;

    int x = 0;
    for(int c = 0; c < this->key_n; c++)
    {
        x += this->keyboard[c].damp(this->damper);
    }

    return x;
}

bool Mellotron::damperStatus()
{
    return this->damper;
}

int Mellotron::load_tapes(std::string model)
{
    int success = 0;
    for(int c=0;c<this->key_n;c++)
        success+=this->keyboard[c].autoload_tape(piano_models.at(model));

    this->setTapeChannel(0);
    return success;

}

int Mellotron::load_tapes(std::string sound_src,int channel)
{
    int success = 0;
    for(int c=0;c<this->key_n;c++)
        success+=this->keyboard[c].load_tape(sound_src,channel);
    this->setTapeChannel(0);
    return success;
}

void Mellotron::raiseVolume()
{
    if(this->general_volume < 100)
        this->general_volume += 10;
}

void Mellotron::lowerVolume()
{
    if(this->general_volume > 0)
        this->general_volume -= 10;
}

unsigned int Mellotron::getVolume()
{
    return this->general_volume;
}

void Mellotron::setVolume(unsigned int vol)
{
    this->general_volume = vol;
}

void Mellotron::raisePitch()
{
    this->pitch += 0.01;
}

void Mellotron::lowerPitch()
{
    this->pitch -= 0.01;
}

float Mellotron::getPitch()
{
    return this->pitch;
}

void Mellotron::resetPitch()
{
    this->pitch = 1.00;
}


void Mellotron::info()
{
    std::string temp_name;
    unsigned long long mem_size = 0;

    int d = 0;
    
    //finds out with wich note has to start to enum keys
    for(int x=0;x<sizeof(key_note_names);x++)
    {
        if(key_note_names[x] == this->keyboard[0].note)
        {
            d = x;
        }
    }
    //
    d = (d + (12 * this->keyboard[0].octave ) );
    //
    for(int c=0;c<this->key_n;c++)
    {
        temp_name = this->keyboard[c].note + std::to_string(this->keyboard[c].octave);

        mem_size += this->keyboard[c].getAllocatedMemory();
        std::cout << "Key"<< c << ": " << temp_name << "\n";
        std::cout << "Key_Map[" << temp_name << "] : "<< this->keyboard_map[temp_name] << "\n";
        std::cout << "MIDI_Map[" << d << "] : "<< this->midi_key_map.at(d) << "\n";
        std::cout << "\n";
        d++;
    }

    std::cout << "\nKeyboard sample memory allocated: " << mem_size << " Bytes";
    std::cout << "\nTape Selected: " << this->sound_channel + 1 << "\n";
    //std::cout << "\nDamper Status: " << (this->damper ? "normal" : "sustain") << "\n";
}
/////
//MIDI HANDLERS
int Mellotron::check_for_MIDI()
{
    // RtMidiIn constructor
    try {
        this->midiin = new RtMidiIn();
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }

    // Check inputs and print available devices
    unsigned int nPorts = midiin->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    if(nPorts > 0){
        for ( unsigned int i=0; i<nPorts; i++ ) {
            try {
                portName = midiin->getPortName(i);
            }
            catch ( RtMidiError &error ) {
                error.printMessage();
                delete this->midiin;
                this->midiin = 0;
                return -1;
            }
            std::cout << "\nPort #" << i << ": " << portName;
        }
    }else{
        delete this->midiin;
        this->midiin = 0;
        return -1;
    }
    //choose device port
    unsigned int device_port = 0;
    if(nPorts > 1)
    {
        std::cout << "\nChoose port number to use ( 0 to " << nPorts - 1 << " ): ";
        std::cin >> device_port;
    }
    //open device
    try {
        this->midiin->openPort( device_port );
        this->midiin->setCallback( & this->MIDIreadCallback,(void *)this );
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        delete this->midiin;
        this->midiin = 0;
        return -1;
    }
    //
    std::cout << "\n\nReading MIDI from port #" << device_port << ": " << this->midiin->getPortName() << std::endl;
    //
    return device_port;
}

void Mellotron::MIDIreadCallback(double deltatime, std::vector< unsigned char > *message,void *userData)
{
    
    unsigned int nBytes = message->size();
    /*
    for ( unsigned int i=0; i<nBytes; i++ )
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        std::cout << "stamp = " << deltatime << std::endl;
    */

    //(void *) is a generic pointer, to read data, cast is needed (without creating real object) 
    Mellotron *pInstrument = static_cast<Mellotron *>(userData);
    

    if(nBytes > 0){
        switch((int)message->at(0))
        {
            case 144:
                //note hit
                if((int) message->at(2) > 0)
                    pInstrument->keyStrikeByMidiInterface((int)message->at(1),(int)message->at(2));
                else    
                    pInstrument->keyReleaseByMidiInterface((int)message->at(1));
            break;
            case 176:
                //damper
                
            break;
        }
    }

}

//destructor
Mellotron::~Mellotron()
{
    if(this->midiin != 0){
        this->midiin->closePort();
        this->midiin->~RtMidiIn();
    }
    //
    std::cout << "\nMellotron succesfully shut-down!\n\n" << std::endl;
}
