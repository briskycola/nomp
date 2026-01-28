#include <mpv/client.h>
#include <string>

class mpvPlayer
{
    private:
        // mpvHandle represents the mpv audio player itself.
        //
        // It handles audio playback, commands, properties,
        // and events.
        mpv_handle *mpvHandle;
    public:
        mpvPlayer();
        ~mpvPlayer();
        void play(const std::string &filename);
};
