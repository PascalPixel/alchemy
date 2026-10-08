#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "fixed_math.h"
#include "object_lookup.h"
#include "object_dispatch.h"
#include "types.h"
#include "system.h"
#include "party_state.h"
#include "animspr.h"

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(void*,s32))ObjectDispatch_ApplyValueToChildrenFar)



void BattleEffect_SetRandomTableValueOnObject(struct DispatchObject *object)
{
    s8 *table = BattleFx_RandomChildValues;
    s32 index = Random16();
    Animation_ApplyChildValuesFar(object, table[(u32)(index * 8) >> 16]);
}

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
