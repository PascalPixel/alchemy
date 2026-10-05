#include "FIXED_MATH.H"
#include "OBJECT_LOOKUP.H"
#include "OBJECT_DISPATCH.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "PARTY_STATE.H"
#include "ANIMSPR.H"

extern s8 BattleFx_RandomChildValues[];

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void ObjectDispatch_ApplyValueToChildrenFar(void *, s32);

void BattleEffect_SetRandomTableValueOnObject(struct DispatchObject *object)
{
    s8 *table = BattleFx_RandomChildValues;
    s32 index = Random16();
    Animation_ApplyChildValuesFar(object, table[(u32)(index * 8) >> 16]);
}

/* Pauses an object: keeps its callback and its animation byte in the party
   state, then hands it the random child value routine. */
void BattleEffect_PauseObject(s32 id)
{
    struct DispatchObject *object;
    struct AnimationEntry *entry;

    object = ObjectTable_Get(id);
    if (object != NULL) {
        gPartyState.saved_callback = (u32)object->update;
        gPartyState.saved_byte = 0;
        if (object->kind == 1) {
            entry = object->target.animation->entries[0];
            if (entry != NULL) {
                gPartyState.saved_byte = entry->param;
            }
        }
        object->update = BattleEffect_SetRandomTableValueOnObject;
        object->value_5b = 1;
        ObjectDispatch_ApplyValueToChildrenFar(object, 0);
    }
}
