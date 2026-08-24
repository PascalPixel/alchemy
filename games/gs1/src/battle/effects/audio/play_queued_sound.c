#include "types.h"

struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *Data_03001ebc;
void Audio_PlayCue(s32);

void BattleEffect_PlayQueuedSound(void)
{
    s16 sound_id = Data_03001ebc->queued_sound;

    if (sound_id != -1)
        Audio_PlayCue(sound_id);
}
