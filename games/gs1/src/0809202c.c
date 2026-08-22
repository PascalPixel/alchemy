#include "types.h"

struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *Data_03001ebc;
void Func_080f9010(s32);

void Func_0809202c(void)
{
    s16 sound_id = Data_03001ebc->queued_sound;

    if (sound_id != -1)
        Func_080f9010(sound_id);
}
