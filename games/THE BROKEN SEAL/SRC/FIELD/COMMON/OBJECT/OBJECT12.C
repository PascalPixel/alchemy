#include "BATTLE_SESSION.H"
#include "BATTLE_EFFECT_WORK.H"
#include "TYPES.H"
#include "ANIMSPR.H"

void *GetMotionRecordFar(void *, s32);
s32 BattleMotion_GetSlotField14Far(s32);
void AnimationObjects_SelectAnimationFar(void *, s32);
extern struct BattleEffectWork *gBattleFxWork;
struct BattleObjectSlot *GetBattleObjectSlotFar(s32);

s32 BattleFx_BeginTiledCanvas(s32);
void BattleFx_EndCanvasLayer(void);

void ObjectGroup_UpdateMembers(s32 set_id, s32 object_value, s32 group_value,
                               s32 state_slot, s32 state_value)
{
    struct BattleObjectSlot *set;
    struct AnimationObject *group;
    struct BattleEffectWork *state;
    s32 group_index;

    set = GetBattleObjectSlotFar(set_id);
    state = gBattleFxWork;
    group_index = 0;

    while ((group = GetMotionRecordFar(set->object, group_index)) != NULL) {
        if (state_slot != -1) {
            state->actor_timers[state_slot] = ((u8 *)&state_value)[0];
        }
        if (set->fading == 0) {
            if (object_value != -1) {
                s32 object_index;

                object_index = 0;
                if (group->count != 0) {
                    struct AnimationEntry **objects;

                    objects = group->entries;
                    do {
                        struct AnimationEntry *object;

                        object = *objects++;
                        if (object != NULL && object != (struct AnimationEntry *)set->effect_entry
                            && object != (struct AnimationEntry *)set->animation_entry) {
                            if (object_value == 0)
                                object->param = BattleMotion_GetSlotField14Far(set_id);
                            else
                                object->param = object_value;
                            object->frame = 0xff;
                        }
                        object_index++;
                    } while (object_index != group->count);
                }
            }
            if (group_value != -1)
                AnimationObjects_SelectAnimationFar(group, group_value);
        }
        group_index++;
    }
}

void BattleFx_InitTilemapAndFlushQueue(void)
{
    BattleFx_BeginTiledCanvas(1);
    BattleFx_EndCanvasLayer();
}
