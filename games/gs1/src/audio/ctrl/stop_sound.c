#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "abi/audio/ctrl/stop_sound.h"

typedef struct {
    u32 header;
    u16 player;
} SoundTableEntry;

void MusicPlayer_Stop(void *player);
extern void *gRom[];
extern SoundTableEntry gRom2[];

void Audio_StopSound(u16 audio_cue_id)
{
    void **players = gRom;
    SoundTableEntry *audio_cue_table = gRom2;
    SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    void *player = players[audio_cue->player * 3];

    if (*(u32 *)player == audio_cue->header)
        MusicPlayer_Stop(player);
}
