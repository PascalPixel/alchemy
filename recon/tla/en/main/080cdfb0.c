#include "battle_system.h"
#include "field_runtime.h"
#include "scene.h"
#include "effect_runtime.h"
#include "global_cells.h"
#include "types.h"
#include "object_lookup.h"
#include "battle_effect_runtime.h"
#include "item.h"
#include "game_state.h"
#include "battle_unit.h"
#include "motobj.h"
#include "animspr.h"

s32 Event_FindFacingTrigger(s32 source)
{
    /* FAKEMATCH: retain the existing word-result collision call boundary;
       the true u8 declaration adds two narrowing shifts, four bytes here. */
    struct FieldStepWork *runtime = (struct FieldStepWork *)Data_03001ebc;
    const struct SceneEvent *trigger = runtime->events;
    s32 facing = ((struct MotionObject *)ObjectTable_Get(
        gGameState.selectedActor))->angle;
    s32 selected = BattleEffect_SelectNearbyObject(gGameState.selectedActor);
    s32 collision;

    source &= 0x1ff;
    collision = GetFocusedObjectCollision();

    while (trigger->control != -1) {
        s32 highValue = (s16)(u16)trigger->trigger & 0xf000;
        s16 hasFacing = (u16)trigger->trigger & 0x0800;
        s32 lowValue = (u16)trigger->trigger & 0xff;

        if ((trigger->control & 0x0f) == 4 &&
            GameFlag_IsConditionActive(trigger->condition) != 0 &&
            (hasFacing == 0 ||
             (u16)(highValue - facing + 0x17ff) <= 0x2ffe)) {
            u32 flags = trigger->control;
            s32 owner = ((const u8 *)&trigger->control)[1];

            if (source == 0 || owner == source) {
                if ((flags & 0x10) != 0) {
                    if (lowValue == selected)
                        return (s32)trigger;
                } else if (lowValue == collision) {
                    return (s32)trigger;
                }
            }
        }
        trigger++;
    }
    return 0;
}
