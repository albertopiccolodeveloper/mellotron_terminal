#include <future>
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

class TapeHead 
{
    public:

    TapeHead(sf::Sound &head);

    void setBuffer(sf::SoundBuffer &buffer);

    void play();
    void stop();
    //
    void asynchronous_stop();
    

    void setVolume(float volume);
    float getVolume();
    
    

    private:
    
    std::future<void> async_stop;
    static void fadeOut(void * head);

    sf::Sound head;

};

TapeHead::TapeHead(sf::Sound &head)
{
    this->async_stop = std::async(std::launch::async, this->fadeOut, &head);
}

void TapeHead::setBuffer(sf::SoundBuffer &buffer)
{
    this->head.setBuffer(buffer);
}

void TapeHead::play()
{
    this->head.play();
}

void TapeHead::setVolume(float volume)
{
    this->head.setVolume(volume);
}

float TapeHead::getVolume()
{
    return this->head.getVolume();
}

void TapeHead::stop()
{
    this->head.stop();
}

void TapeHead::asynchronous_stop()
{
    //this->async_stop = std::async(std::launch::async, this->fadeOut,(void *)this);
    this->async_stop.get();
    this->head.stop();
}

void TapeHead::fadeOut(void * head)
{
    //std::cout << "\nAsync stop called";
    //(void *) is a generic pointer, to read data, cast is needed (without creating real object) 
    //TapeHead *self = static_cast<TapeHead *>(head);
    sf::Sound *self = static_cast<sf::Sound*>(head);
    //
    float steps = 1000;
    unsigned int sleepy_time = 2000 * 5;
    //fadeout (to avoid clicking)
    float fade = self->getVolume();
    float attenuation = fade / steps;
    while(fade > 0)
    {
        self->setVolume(fade);
        fade -= attenuation;
        for(unsigned int c = 0; c < sleepy_time; c++)
        {
            //wait

        }
        //sf::sleep(sf::microseconds(sleepy_time_microseconds));
    }

}