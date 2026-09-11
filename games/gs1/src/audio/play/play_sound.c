#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "abi/audio/play/play_sound.h"

typedef struct {
    s32 header;
    u16 player;
} SoundTableEntry;

extern s32 gRom[];
extern SoundTableEntry gRom2[];

void MusicPlayer_StartSong(s32, s32);

void Audio_PlaySound(u16 audio_cue_id)
{
    s32 *player_records = gRom;
    SoundTableEntry *audio_cue_table = gRom2;
    SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    s32 *player_record = &player_records[audio_cue->player * 3];

    MusicPlayer_StartSong(player_record[0], audio_cue->header);
}
