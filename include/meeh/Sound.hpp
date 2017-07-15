#pragma once

#include "NonCpNonMv.hpp"
#include <string>
#include "Raii.hpp"
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

namespace meeh
{

// default volume is max = 128

// todo: passing Sample to ChannelPool to play it
class Sample
{
public:
    Sample(const std::string& filename);

    void setVolume(int volume);
    int getVolume() const;
};

// only one Music can be played at the same time
class Music: public NonCpNonMv
{
public:
    Music(const std::string& filename);

    void setVolume(int volume);
    int getVolume() const;

    void play(bool loop);

    static void pause();
    static void resume();
    static void stop();
    static bool isPlaying();
    static bool isPaused();
    static bool isStopped();

private:
    Raii clean;
    Mix_Music* music;
};

} // meeh
