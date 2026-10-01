#include "FIXED_MATH.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "PARTY_STATE.H"

extern s8 BattleFx_RandomChildValues[];

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Animation_ApplyChildValuesFar(void *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(void *, s32);

void BattleEffect_SetRandomTableValueOnObject(s32 arg0)
{
    s8 *table = BattleFx_RandomChildValues;
    s32 index = Random16();
    Animation_ApplyChildValuesFar((void *)arg0, table[(u32)(index * 8) >> 16]);
}

/* Pauses an object: keeps its callback and its animation byte in the party
   state, then hands it the random child value routine. */
void BattleEffect_PauseObject(s32 arg0)
{
    u8 *object;
    u8 *entry;

    object = ObjectTable_Get(arg0);
    if (object != NULL) {
        gPartyState.saved_callback = *(u32 *)(object + 0x6C);
        gPartyState.saved_byte = 0;
        if (object[0x54] == 1) {
            entry = *(u8 **)(*(u8 **)(object + 0x50) + 0x28);
            if (entry != NULL) {
                gPartyState.saved_byte = entry[5];
            }
        }
        *(u32 *)(object + 0x6C) = (u32)BattleEffect_SetRandomTableValueOnObject;
        object[0x5B] = 1;
        ObjectDispatch_ApplyValueToChildrenFar(object, 0);
    }
}
