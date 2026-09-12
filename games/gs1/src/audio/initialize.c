#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"

/* audio/init/initialize.c */
struct CgbChannel;
struct MusicPlayerState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);

struct MusicPlayerState {
    u32 song_header_word;
    u32 status;
    u8 track_count;
    u8 priority;
    u8 command;
    u8 config;
    u8 clock_and_gap[0x0c];
    u8 *memory_area;
    u8 tempo_and_fade_bytes[0x10];
    struct MusicTrackState *tracks;
    void *voice_group;
    u32 ident;
    PlayerMainCallback next_callback;
    struct MusicPlayerState *next_player;
};

struct PlayerBootstrapRecord {
    struct MusicPlayerState *player;
    struct MusicTrackState *tracks;
    u8 max_tracks;
    u8 unknown09;
    u16 config;
};

void CgbAudio_Initialize(struct CgbChannel *);
void AudioEngine_Initialize(struct AudioEngineState *);
void AudioEngine_SetMode(u32 value);
void MusicPlayer_Initialize(
    struct MusicPlayerState *,
    struct MusicTrackState *,
    u32);
extern struct MusicPlayerView *RomBytes_080f9675[];
extern u32 Value_00000008;
extern u8 gOv;
extern struct PlayerBootstrapRecord RomBytes_080fc624[];

void Audio_Initialize(void)
{
    u16 count;

    Audio_Place((s32)&RomBytes_080f9675 & ~1, 0x03007000, 0x04000100);
    AudioEngine_Initialize((struct AudioEngineState *)0x02003050);
    CgbAudio_Initialize((struct CgbChannel *)0x02004090);
    AudioEngine_SetMode(0x0097F800);

    count = (u32)&Value_00000008;
    if (count != 0) {
        struct PlayerBootstrapRecord *record = RomBytes_080fc624;
        u32 remaining = count;
        do {
            struct MusicPlayerState *player = record->player;
            MusicPlayer_Initialize(
                player,
                record->tracks,
                record->max_tracks);
            player->config = record->config;
            player->memory_area = &gOv;
            record++;
            remaining--;
        } while (remaining != 0);
    }
}

/* audio/mix/run_mixer.c */
s32 AudioEngine_RunMixerTick(void);

void AudioEngine_RunMixer(void)
{
    AudioEngine_RunMixerTick();
}
