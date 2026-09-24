#include "B5_CONTEXT.H"

void *GetMotionRecordFar(void *, s32);
s32 BattleMotion_GetSlotField14Far(s32);
void Object_InitializeMode(void *, s32);
extern u32 Data_03001eec;

void ObjectGroup_UpdateMembers(s32 set_id, s32 object_value, s32 group_value,
                               s32 state_slot, s32 state_value)
{
    struct B5Context *set;
    void *group;
    u8 *state;
    s32 group_index;

    set = GetBattleObjectSlotFar(set_id);
    state = (u8 *)Data_03001eec;
    group_index = 0;

    while ((group = GetMotionRecordFar(set->object, group_index)) != NULL) {
        if (state_slot != -1) {
            s32 state_offset = state_slot + 0x7818;

            state[state_offset] = ((u8 *)&state_value)[0];
        }
        if (set->suppress_updates == 0) {
            if (object_value != -1) {
                s32 object_index;

                object_index = 0;
                if (*(u8 *)((u8 *)group + 0x27) != 0) {
                    void **objects;

                    objects = (void **)((u8 *)group + 0x28);
                    do {
                        void *object;

                        object = *objects++;
                        if (object != NULL && object != set->excluded_24
                            && object != set->excluded_20) {
                            if (object_value == 0)
                                *(u8 *)((u8 *)object + 5) = BattleMotion_GetSlotField14Far(set_id);
                            else
                                *(u8 *)((u8 *)object + 5) = object_value;
                            *(u8 *)((u8 *)object + 0x16) = 0xff;
                        }
                        object_index++;
                    } while (object_index != *(u8 *)((u8 *)group + 0x27));
                }
            }
            if (group_value != -1)
                Object_InitializeMode(group, group_value);
        }
        group_index++;
    }
}
