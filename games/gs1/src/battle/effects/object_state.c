#include "types.h"

s32 Func_0808e4b4(s32, s32, void *);

s32 BattleFx_HasMatchingEvent5(s32 effectId)
{
    s32 local;
    s32 result = Func_0808e4b4(0x70000005, (u16)effectId, &local);
    return (u32)((-result) | result) >> 31;
}

s32 Func_0808e14c(u16);

u32 BattleFx_HasTrigger(u16 effectId)
{
    s32 result;

    result = Func_0808e14c(effectId);
    return (u32)((0 - result) | result) >> 0x1F;
}

s32 Func_080091a8(s32, s32, s32);

void ObjectMotion_SnapToTerrain(void *object)
{
    s32 height;

    height = Func_080091a8(0, *(s32 *)((u8 *)object + 8),
        *(s32 *)((u8 *)object + 0x10));
    *(s32 *)((u8 *)object + 0x0c) = height;
    *(s32 *)((u8 *)object + 0x14) = height;
}
