#include "FIXED_MATH.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "SYSTEM.H"

extern s8 BattleFx_RandomChildValues[];

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Animation_ApplyChildValuesFar(void *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(void *, s32);

struct GlobalState {
    u8 unknown_000[0x249];
    u8 saved_byte;
    u8 unknown_24a[6];
    u32 saved_callback;
};

extern struct GlobalState gGameState;

void BattleFx_ResumeObject(void)
{
    u8 *object = ObjectTable_Get();
    if (object != 0) {
        if (*(void (**)(s32))(object + 0x6C) == BattleEffect_SetRandomTableValueOnObject) {
            u8 *state = (u8 *)&gGameState;
            *(s32 *)(object + 0x6C) = *(s32 *)(state + 0x250);
            *(s32 *)(state + 0x250) = 0;
            Animation_ApplyChildValuesFar(object, *(s8 *)(state + 0x249));
        }
        object[0x5B] = 0;
        ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    }
}
