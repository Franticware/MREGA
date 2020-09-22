#ifdef USESDL

#include "opl.h"
#include "opal.h"

#include <SDL.h>
#include <inttypes.h>
#include <samplerate.h>
#include <vector>

extern SDL_AudioDeviceID audioDevice;

Opal opal(0);
SRC_STATE *src_state;
std::vector<float> samples;

void SDLCALL oplEmulationCallback(void *userdata, Uint8 * stream, int len)
{
    (void)(userdata);
    /*
    uint64_t ticks = SDL_GetPerformanceCounter();
    printf("audio %d %" PRIu64 "\n", len, ticks);
    fflush(stdout);

    for (int i = 0; i != len; ++i)
    {
        stream[i] = 0;
    }
    */

    int sampleCount = len >> 1;
    int frameCount = sampleCount >> 1;

    if (OPL_mode)
    {
        samples.resize(sampleCount);
        src_callback_read(src_state, 4000.0/4143.0, frameCount, &(samples[0]));
        src_float_to_short_array(&(samples[0]), (int16_t*)stream, sampleCount);
    }
    else
    {
        memset(stream, 0, len);
    }
}

static long src_input_callback(void *cb_data, float **audio)
{
    (void)(cb_data);
    const int input_frames = 1;

    static float audioFrameF[2];

    int16_t audioFrameS[2];
    opal.Sample(audioFrameS, audioFrameS + 1);

    src_short_to_float_array(audioFrameS, audioFrameF, 2);

    *audio = audioFrameF;

    return input_frames ;
} /* src_input_callback */

void initOplEmulation()
{
    int converter = SRC_SINC_FASTEST;
    int error;
    src_state = src_callback_new(src_input_callback, converter, 2, &error, 0);

    SDL_PauseAudioDevice(audioDevice, 0);
}

void WriteFM(uint16_t regs, uint8_t value)
{
    SDL_LockAudioDevice(audioDevice);
    opal.Port(regs, value);
    SDL_UnlockAudioDevice(audioDevice);

    //uint32_t ticks = SDL_GetTicks();
    //printf("fm %d\n", ticks);

    //uint64_t ticks = SDL_GetPerformanceCounter();
    //printf("fm %" PRIu64 "\n", ticks);

    //fflush(stdout);
}

int opl_init(int addr, int mode)
{
    (void)(addr);
    int ret = 0;
    if (mode)
    {
        ret = 3;
    }
    OPL_mode = ret;
    return ret;
}

void opl_deinit()
{
    src_delete(src_state);
}

#endif
