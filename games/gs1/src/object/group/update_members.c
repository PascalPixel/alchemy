#include "b5_context.h"
#include "types.h"
#include "scene.h"
#include "abi/object/group/update_members.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define SET_OWNER(set) FIELD(set, void *, 0x00)
#define SET_EXCLUDED_20(set) FIELD(set, void *, 0x20)
#define SET_EXCLUDED_24(set) FIELD(set, void *, 0x24)
#define SET_SUPPRESS_UPDATES(set) FIELD(set, s16, 0x2a)
#define GROUP_OBJECT_COUNT(group) FIELD(group, u8, 0x27)
#define OBJECT_VALUE(object) FIELD(object, u8, 0x05)
#define OBJECT_REFRESH(object) FIELD(object, u8, 0x16)

void *Obj_Run(void *, s32);

void Object_InitializeMode(void *, s32);
extern u32 gIw;

void ObjectGroup_UpdateMembers(
    s32 set_id,
    s32 object_value,
    s32 group_value,
    s32 state_slot,
    s32 state_value)
{
    struct B5Context *set;
    void *group;
    u8 *state;
    s32 group_index;

    set = Obj_Run2(set_id);
    state = (u8 *)gIw;
    group_index = 0;

    while ((group = Obj_Run(SET_OWNER(set), group_index)) != NULL) {
        if (state_slot != -1) {
            s32 state_offset = state_slot + 0x7818;

            state[state_offset] = ((u8 *)&state_value)[0];
        }

        if (SET_SUPPRESS_UPDATES(set) == 0) {
            if (object_value != -1) {
                s32 object_index;

                object_index = 0;
                if (GROUP_OBJECT_COUNT(group)!= 0) {
                    void **objects;

                    objects = (void **)((u8 *)group + 0x28);
                    do {
                        void *object;

                        object = *objects++;
                        if (object != NULL
                            && object != SET_EXCLUDED_24(set)
                            && object != SET_EXCLUDED_20(set)) {
                            if (object_value == 0)
                                OBJECT_VALUE(object) = Obj_Check(set_id);
                            else
                                OBJECT_VALUE(object) = object_value;
                            OBJECT_REFRESH(object) = 0xff;
                        }
                        object_index++;
                    } while (object_index != GROUP_OBJECT_COUNT(group));
                }
            }

            if (group_value != -1)
                Object_InitializeMode(group, group_value);
        }
        group_index++;
    }
}
