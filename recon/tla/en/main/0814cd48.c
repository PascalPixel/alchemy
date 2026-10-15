/* Near miss, differ 6 of 103: The Broken Seal's ObjectGroup_UpdateMembers as ⚓️ has
   it: the member count is a byte at 0x1b, the timers sit at 0x78dc of the effect
   work, and a member's frame is reset only when its parameter changes. Reading the
   count through the inline accessor gives the native's registers in the member
   loop (differ 44 reading it directly). Left: the timer store, where the native
   interleaves the timer's load with the 0x78dc offset (add r1, sp, #52; movs r3,
   #240; ldrb r0, [r1]; lsls r3, r3, #7; ldr r1, [sp, #12]; adds r3, #220); here
   the offset is built first. An operand barrier on the timer changes the
   allocation (differ 55). */
#include "types.h"
#include "ram_buffer.h"

struct FxGroupEntry
{
    u8 unknown_00[5];
    u8 param;
    u8 unknown_06[16];
    u8 frame;
};

struct FxGroup
{
    u8 unknown_00[27];
    u8 count;
    u8 unknown_1c[12];
    struct FxGroupEntry *entries[4];
};

struct FxSlot
{
    void *object;
    u8 unknown_04[28];
    struct FxGroupEntry *animation_entry;
    struct FxGroupEntry *effect_entry;
    u8 unknown_28[2];
    u8 fading;
};

struct FxTimerWork
{
    u8 unknown_0000[0x78dc];
    u8 actor_timers[8];
};

struct FxSlot *GetBattleObjectSlotFar(s32 id);
struct FxGroup *GetMotionRecordFar(void *object, s32 index);
s32 BattleMotion_GetSlotPaletteFar(s32 id);
void Animation_ApplyChildArgumentFar(void *group, s32 value);

static __inline__ u8 GroupCount(struct FxGroup *group)
{
    return group->count;
}

void Func_0814cd48(s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value)
{
    struct FxSlot *set;
    struct FxGroup *group;
    struct FxTimerWork *state;
    s32 group_index;

    set = GetBattleObjectSlotFar(set_id);
    state = (struct FxTimerWork *)Ram_HeapSlots->battle_fx_work[0];
    group_index = 0;

    while ((group = GetMotionRecordFar(set->object, group_index)) != NULL)
    {
        if (state_slot != -1)
            state->actor_timers[state_slot] = ((u8 *)&state_value)[0];
        if (set->fading == 0)
        {
            if (object_value != -1)
            {
                s32 object_index;

                object_index = 0;
                if (GroupCount(group) != 0)
                {
                    struct FxGroupEntry **objects;

                    objects = group->entries;
                    do
                    {
                        struct FxGroupEntry *object;

                        object = *objects++;
                        if (object != NULL && object != set->effect_entry
                            && object != set->animation_entry)
                        {
                            u8 old = object->param;

                            if (object_value == 0)
                                object->param = BattleMotion_GetSlotPaletteFar(set_id);
                            else
                                object->param = object_value;
                            if (object->param != old)
                                object->frame = 0xff;
                        }
                        object_index++;
                    } while (object_index != GroupCount(group));
                }
            }
            if (group_value != -1)
                Animation_ApplyChildArgumentFar(group, group_value);
        }
        group_index++;
    }
}
