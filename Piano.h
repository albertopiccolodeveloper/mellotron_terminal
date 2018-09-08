#include <unordered_map>
//
const std::string key_note_names[12] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
 //fine variabili statiche
//////////////////////////////////////////
class Piano
{
    public:
        Piano(const unsigned int keys,std::string lowest_note,int lowest_octave);
        int load_sounds();
        int load_sounds(std::string sound_src,int channel);
        int keyStrikeByNote(std::string note_name,unsigned int dynamic);
        int keyStrikeByKeyN(int key_number,unsigned int dynamic);
        int keyReleaseByNote(std::string note_name);
        int keyReleaseByKeyN(int key_number);
        int damperOn();
        void damperOff();
        bool damperStatus();
        void stampa_tastiera();
    private:
        Key keyboard[88];
        std::unordered_map<std::string,int> keyboard_map;
        //array associativo per accesso nome nota,numero tasto
        bool damper;
        unsigned int general_volume;
        int key_n;
        int sound_track;
};


//metodi classe Piano
Piano::Piano(const unsigned int keys,std::string lowest_note,int lowest_octave)
{
    this->key_n = keys;
    this->damper = true;
    this->sound_track = 0;

    int d = 0;
    
    for(int x=0;x<sizeof(key_note_names);x++)
    {
        if(key_note_names[x] == lowest_note)
        {
            d = x;
        }
    }
    

    Key temp; 
    //std::pair<std::string,int> temp_map_value;
    for(int c=0;c<keys;c++)
    {
        temp.note = key_note_names[d % 12];
        temp.octave = lowest_octave + (d / 12);
        //inserimento nota
        this->keyboard[c] = temp;
        //
        //temp_map_value.first = temp.note + std::to_string(temp.octave);
        //temp_map_value.second = c;
        //per poter accedere indipendentemente con il numero del tasto o con la nota
        //this->keyboard_map.insert( temp_map_value );
        this->keyboard_map[temp.note + std::to_string(temp.octave)] = c;
        d++;
    }

}

int Piano::keyStrikeByKeyN(int key_number,unsigned int dynamic)
{
    return this->keyboard[key_number].strike(dynamic);
}

int Piano::keyStrikeByNote(std::string note_name,unsigned int dynamic)
{
    try{
        return this->keyboard[this->keyboard_map.at(note_name)].strike(dynamic);
    }catch(const std::out_of_range& oor){
        std::cout << "\nKey not playable - (" << oor.what() <<") Out of Range Exception!";
        return 0;
    }
}

int Piano::keyReleaseByKeyN(int key_number)
{
    return this->keyboard[key_number].release(this->damper);
}

int Piano::keyReleaseByNote(std::string note_name)
{
    try{
        return this->keyboard[this->keyboard_map.at(note_name)].release(this->damper);
    }catch(const std::out_of_range& oor){
        std::cout << "\nKey not playable - (" << oor.what() <<") Out of Range Exception!";
        return -1;
    }
}

void Piano::damperOff()
{
    this->damper = false;
}

int Piano::damperOn()
{
    this->damper = true;

    int x = 0;
    for(int c = 0; c < this->key_n; c++)
    {
        x += this->keyboard[c].damp(this->damper);
    }

    return x;
}

bool Piano::damperStatus()
{
    return this->damper;
}

int Piano::load_sounds()
{
    int success = 0;
    for(int c=0;c<this->key_n;c++)
        success+=this->keyboard[c].autoload_sample();

    return success;
}

int Piano::load_sounds(std::string sound_src,int channel)
{
    int success = 0;
    for(int c=0;c<this->key_n;c++)
        success+=this->keyboard[c].load_sample(sound_src,channel);
    return success;
}



void Piano::stampa_tastiera()
{
    std::string temp_name;
    unsigned long int mem_size = 0;
    for(int c=0;c<this->key_n;c++)
    {
        temp_name = this->keyboard[c].note + std::to_string(this->keyboard[c].octave);
        /*
        if(temp_name.length() > 2){
            std::cout << "#";
        }
        */
        mem_size += this->keyboard[c].getAllocatedMemory();
        std::cout << "Key"<< c << ": " << temp_name << "\n";
        //std::cout << "Key_Map[" << temp_name << "] : "<< this->keyboard_map[temp_name] << "\n";  
    }

    std::cout << "\nKeyboard sample memory allocated: " << mem_size << " Bytes";

    std::cout << "\nDamper Status: " << (this->damper ? "normal" : "sustain") << "\n";
}
/////