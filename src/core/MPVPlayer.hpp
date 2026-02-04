#include <mpv/client.h>
#include <string>

class MPVPlayer
{
    private:
        // mpvHandle represents the mpv audio player itself.
        //
        // It handles audio playback, commands, properties,
        // and events.
        mpv_handle *mpvHandle;
    public:
        MPVPlayer();
        ~MPVPlayer();
        bool play(const std::string &filename);
};
