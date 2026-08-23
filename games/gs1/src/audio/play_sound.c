#include "audio_engine_symbols.h"
#include "types.h"

typedef struct {
    s32 header;
    u16 player;
} SoundTableEntry;

extern s32 Data_080fc624[];
extern SoundTableEntry Data_080fc684[];

void MusicPlayer_StartSong(s32, s32);

void Audio_PlaySound(u16 sound_id)
{
    s32 *player_records = Data_080fc624;
    SoundTableEntry *sound_table = Data_080fc684;
    SoundTableEntry *sound = &sound_table[sound_id];
    s32 *player_record = &player_records[sound->player * 3];

    MusicPlayer_StartSong(player_record[0], sound->header);
}
