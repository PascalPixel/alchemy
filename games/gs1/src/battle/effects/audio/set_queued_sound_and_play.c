#include "types.h"
#include "scene.h"
#include "abi/battle/effects/audio/set_queued_sound_and_play.h"

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
