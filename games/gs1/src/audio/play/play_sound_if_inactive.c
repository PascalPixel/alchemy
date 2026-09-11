#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "abi/audio/play/play_sound_if_inactive.h"

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
extern struct MusicPlayerView *gRom[];
extern struct SoundTableEntry gRom2[];

void Audio_PlaySoundIfInactive(u16 audio_cue_id)
{
    struct MusicPlayerView **players = gRom;
    struct SoundTableEntry *audio_cue_table = gRom2;
    struct SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    struct MusicPlayerView *player = players[audio_cue->player * 3];

    if (player->song_header != audio_cue->header) {
        MusicPlayer_StartSong(player, audio_cue->header);
    } else {
        s32 status = player->status;
        u16 low_status = *(volatile u16 *)&player->status;

        if (low_status == 0 || status < 0)
            MusicPlayer_StartSong(player, player->song_header);
    }
}
