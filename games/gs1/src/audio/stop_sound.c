#include "audio_engine_symbols.h"
#include "types.h"

typedef struct {
    u32 header;
    u16 player;
} SoundTableEntry;

void MusicPlayer_Stop(void *player);
extern void *Data_080fc624[];
extern SoundTableEntry Data_080fc684[];

void Audio_StopSound(u16 sound_id)
{
    void **players = Data_080fc624;
    SoundTableEntry *sound_table = Data_080fc684;
    SoundTableEntry *sound = &sound_table[sound_id];
    void *player = players[sound->player * 3];

    if (*(u32 *)player == sound->header)
        MusicPlayer_Stop(player);
}
