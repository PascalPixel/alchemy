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

void BattleEffect_PauseObject(s32 arg0)
{
    u8 *object;
    u8 *entry;

    object = ObjectTable_Get(arg0);
    if (object != NULL) {
        gGameState.saved_callback = *(u32 *)(object + 0x6C);
        gGameState.saved_byte = 0;
        if (object[0x54] == 1) {
            entry = *(u8 **)(*(u8 **)(object + 0x50) + 0x28);
            if (entry != NULL) {
                gGameState.saved_byte = entry[5];
            }
        }
        *(u32 *)(object + 0x6C) = (u32)BattleEffect_SetRandomTableValueOnObject;
        object[0x5B] = 1;
        ObjectDispatch_ApplyValueToChildrenFar(object, 0);
    }
}
