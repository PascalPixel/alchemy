#include "FIXED_MATH.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "SYSTEM.H"

extern s8 BattleFx_RandomChildValues[];

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Animation_ApplyChildValuesFar(void *, s32);

struct GlobalState {
    u8 unknown_000[0x249];
    u8 saved_byte;
    u8 unknown_24a[6];
    u32 saved_callback;
};

void BattleEffect_SetRandomTableValueOnObject(s32 arg0)
{
    s8 *table = BattleFx_RandomChildValues;
    s32 index = Random16();
    Animation_ApplyChildValuesFar((void *)arg0, table[(u32)(index * 8) >> 16]);
}
