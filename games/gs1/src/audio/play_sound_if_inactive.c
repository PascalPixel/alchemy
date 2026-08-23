#include "audio_engine_symbols.h"
#include "types.h"

struct SoundTableEntry {
    u32 header;
    u16 player;
    u16 padding;
};

struct MusicPlayerView {
    u32 song_header;
    s32 status;
    u32 unknown08;
};

void MusicPlayer_StartSong(struct MusicPlayerView *, u32);
extern struct MusicPlayerView *Data_080fc624[];
extern struct SoundTableEntry Data_080fc684[];

void Audio_PlaySoundIfInactive(u16 sound_id)
{
    struct MusicPlayerView **players = Data_080fc624;
    struct SoundTableEntry *sound_table = Data_080fc684;
    struct SoundTableEntry *sound = &sound_table[sound_id];
    struct MusicPlayerView *player = players[sound->player * 3];

    if (player->song_header != sound->header) {
        MusicPlayer_StartSong(player, sound->header);
    } else {
        s32 status = player->status;
        u16 low_status = *(volatile u16 *)&player->status;

        if (low_status == 0 || status < 0)
            MusicPlayer_StartSong(player, player->song_header);
    }
}
