#include "audio_engine_symbols.h"
#include "types.h"

typedef struct {
    u32 header;
    u16 player;
} SoundTableEntry;

void MusicPlayer_Stop(void *player);
extern void *Data_080fc624[];
extern SoundTableEntry Data_080fc684[];

void Audio_StopSound(u16 audio_cue_id)
{
    void **players = Data_080fc624;
    SoundTableEntry *audio_cue_table = Data_080fc684;
    SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    void *player = players[audio_cue->player * 3];

    if (*(u32 *)player == audio_cue->header)
        MusicPlayer_Stop(player);
}
