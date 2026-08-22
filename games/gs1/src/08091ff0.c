#include "types.h"

struct BattleEventState {
    u8 padding[0xcc8];
    s16 queued_sound;
};

extern struct BattleEventState *Data_03001ebc;
void Func_080f9010(s32);

void Func_08091ff0(s32 sound_id)
{
    Data_03001ebc->queued_sound = sound_id;
    if ((s16)sound_id == -1) {
        sound_id = 0x121;
    }
    Func_080f9010(0x12a);
    Func_080f9010(sound_id);
}
