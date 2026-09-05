/* Copy the low two mode bits into the object's owner record. */
#include "types.h"

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

#define SetEffectOwnerMode Func_020009dc
void SetEffectOwnerMode(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}
