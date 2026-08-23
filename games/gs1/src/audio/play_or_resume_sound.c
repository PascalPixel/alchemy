#include "audio_engine_symbols.h"
#include "types.h"

typedef struct {
    s32 header;
    u16 player;
} SoundTableEntry;

extern s32 *Data_080fc624[];
extern SoundTableEntry Data_080fc684[];

void MusicPlayer_StartSong(void *, s32);
void Func_080fa264(void *);

void Audio_PlayOrResumeSound(u16 sound_id)
{
    s32 **players = Data_080fc624;
    SoundTableEntry *sound_table = Data_080fc684;
    SoundTableEntry *sound = &sound_table[sound_id];
    s32 *player = players[sound->player * 3];
    s32 current_header = player[0];
    s32 target_header = sound->header;

    if (current_header != target_header) {
        MusicPlayer_StartSong(player, target_header);
    } else {
        s32 status = player[1];

        if ((u16)status == 0)
            MusicPlayer_StartSong(player, current_header);
        else if (status < 0)
            Func_080fa264(player);
    }
}
