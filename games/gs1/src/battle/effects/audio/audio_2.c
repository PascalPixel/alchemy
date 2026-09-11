#include "types.h"
#include "scene.h"

/* battle/effects/audio/set_queued_sound_and_play.c */
struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *gWork;
void Audio_PlayCue(s32);

void BattleFx_SetQueuedSoundAndPlay(s32 sound_id)
{
    gWork->queued_sound = sound_id;
    if ((s16)sound_id == -1) {
        sound_id = 0x121;
    }
    Audio_PlayCue(0x12a);
    Audio_PlayCue(sound_id);
}

/* battle/effects/audio/play_queued_sound.c */
struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *gWork;
void Audio_PlayCue(s32);

void BattleFx_PlayQueuedSound(void)
{
    s16 sound_id = gWork->queued_sound;

    if (sound_id != -1)
        Audio_PlayCue(sound_id);
}
