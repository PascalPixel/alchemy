#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "abi/audio/play/play_or_resume_sound.h"

typedef struct {
    s32 header;
    u16 player;
} SoundTableEntry;

extern s32 *gRom[];
extern SoundTableEntry gRom2[];

void MusicPlayer_StartSong(void *, s32);
void Audio_ResumePlayer(void *);

void Audio_PlayOrResumeSound(u16 audio_cue_id)
{
    s32 **players = gRom;
    SoundTableEntry *audio_cue_table = gRom2;
    SoundTableEntry *audio_cue = &audio_cue_table[audio_cue_id];
    s32 *player = players[audio_cue->player * 3];
    s32 current_header = player[0];
    s32 target_header = audio_cue->header;

    if (current_header != target_header) {
        MusicPlayer_StartSong(player, target_header);
    } else {
        s32 status = player[1];

        if ((u16)status == 0)
            MusicPlayer_StartSong(player, current_header);
        else if (status < 0)
            Audio_ResumePlayer(player);
    }
}
