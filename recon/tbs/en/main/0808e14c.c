/* Draft, not exact (2026-09-29): permuter score 255 (from 650), rewritten
   after psynergy similar's nearest match, BattleFx_FindMatchingEvent
   (BATTLE/EFFECT/ABILITY_EVENT.C): one condition chain in a while loop.
   Remaining: register choice for the 0x1f4 offset and the argument of
   GameFlag_IsConditionActive, and the reference loads the flags word and
   owner byte before testing the source mask. */
#include "TYPES.H"
#include "EFFECT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "BATTLE_EFFECT_RUNTIME.H"

void *ObjectTable_Get(u32);
s32 BattleEffect_SelectNearbyObject(u32 object_id);
s32 GameFlag_IsConditionActive(s32 condition);
s32 GetFocusedObjectCollision(void);

struct FacingTrigger {
    s32 flags;
    u16 metadata;
    s16 condition;
    u8 unknown_08[4];
};

struct FacingTriggerRuntime {
    u8 unknown_00[16];
    struct FacingTrigger *triggers;
};

struct FacingObject {
    u8 unknown_00[6];
    u16 facing;
};

s32 Event_FindFacingTrigger(s32 source)
{
    struct FacingTriggerRuntime *runtime =
        (struct FacingTriggerRuntime *)Data_03001ebc;
    struct FacingTrigger *trigger = runtime->triggers;
    s32 facing = ((struct FacingObject *)ObjectTable_Get(
        Data_02000240.object_id))->facing;
    s32 selected = BattleEffect_SelectNearbyObject(Data_02000240.object_id);
    s32 collision;

    source &= 0x1ff;
    collision = GetFocusedObjectCollision();

    while (trigger->flags != -1) {
        s32 high_value = (s16)trigger->metadata & 0xf000;
        s16 has_facing = trigger->metadata & 0x0800;
        s32 low_value = trigger->metadata & 0xff;

        if ((trigger->flags & 0x0f) == 4 &&
            GameFlag_IsConditionActive(trigger->condition) != 0 &&
            (has_facing == 0 ||
             (u16)(high_value - facing + 0x17ff) <= 0x2ffe) &&
            (source == 0 || ((u8 *)&trigger->flags)[1] == source)) {
            if ((trigger->flags & 0x10) != 0) {
                if (low_value == selected)
                    return (s32)trigger;
            } else if (low_value == collision) {
                return (s32)trigger;
            }
        }
        trigger++;
    }
    return 0;
}
