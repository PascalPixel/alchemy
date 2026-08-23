#include "audio_engine_symbols.h"
#include "types.h"

typedef struct {
    u32 header;
    u16 player;
} SoundTableEntry;

void Func_080fa264(void *player);
extern void *Data_080fc624[];
extern SoundTableEntry Data_080fc684[];

void Audio_ResumeSound(u16 sound_id)
{
    u32 table_offset = sound_id;
    void * volatile *players;
    SoundTableEntry *sound_table;
    SoundTableEntry *sound;
    void *player;
    u32 current_header;
    u32 target_header;
    u32 player_id;
    u32 player_slot;

    table_offset <<= 16;
    players = Data_080fc624;
    sound_table = Data_080fc684;
    table_offset >>= 13;
    sound = (SoundTableEntry *)((unsigned char *)sound_table + table_offset);
    player_id = sound->player;
    player_slot = player_id << 1;
    player_slot += player_id;
    player = players[player_slot];
    current_header = *(volatile u32 *)player;
    target_header = sound->header;

    if (current_header == target_header)
        Func_080fa264(player);
}
