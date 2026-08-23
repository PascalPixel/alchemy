#include "audio_engine_symbols.h"
#include "types.h"

struct CgbChannel;
struct MusicPlayerState;
struct MusicTrackState;
struct AudioEngineState;

typedef void (*PlayerMainCallback)(struct MusicPlayerState *);

struct MusicPlayerState {
    u32 unknown00;
    u32 status;
    u8 track_count;
    u8 priority;
    u8 unknown0a;
    u8 config;
    u8 unknown0c[0x0c];
    u8 *memory_area;
    u8 unknown1c[0x10];
    struct MusicTrackState *tracks;
    void *unknown30;
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

void Func_08006864(s32, s32, s32);
void CgbAudio_Initialize(struct CgbChannel *);
void AudioEngine_Initialize(struct AudioEngineState *);
void AudioEngine_SetMode(u32 value);
void MusicPlayer_Initialize(
    struct MusicPlayerState *,
    struct MusicTrackState *,
    u32);
extern u8 Data_080f9675;
extern u32 Value_00000008;
extern u8 Data_02004350;
extern struct PlayerBootstrapRecord Data_080fc624[];

void Audio_Initialize(void)
{
    u16 count;

    Func_08006864((s32)&Data_080f9675 & ~1, 0x03007000, 0x04000100);
    AudioEngine_Initialize((struct AudioEngineState *)0x02003050);
    CgbAudio_Initialize((struct CgbChannel *)0x02004090);
    AudioEngine_SetMode(0x0097F800);

    count = (u32)&Value_00000008;
    if (count != 0) {
        struct PlayerBootstrapRecord *record = Data_080fc624;
        u32 remaining = count;
        do {
            struct MusicPlayerState *player = record->player;
            MusicPlayer_Initialize(
                player,
                record->tracks,
                record->max_tracks);
            player->config = record->config;
            player->memory_area = &Data_02004350;
            record++;
            remaining--;
        } while (remaining != 0);
    }
}
