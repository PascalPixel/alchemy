#include "types.h"
#include "scene.h"
#include "abi/battle/effects/audio/play_queued_sound.h"

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
