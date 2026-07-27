#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define SET_OWNER(set) FIELD(set, void *, 0x00)
#define SET_EXCLUDED_20(set) FIELD(set, void *, 0x20)
#define SET_EXCLUDED_24(set) FIELD(set, void *, 0x24)
#define SET_SUPPRESS_UPDATES(set) FIELD(set, s16, 0x2a)
#define GROUP_OBJECT_COUNT(group) FIELD(group, u8, 0x27)
#define OBJECT_VALUE(object) FIELD(object, u8, 0x05)
#define OBJECT_REFRESH(object) FIELD(object, u8, 0x16)

void *Func_080b5098(s32);
void *Func_080b50d8(void *, s32);
s32 Func_080b5100(s32);
void Func_08009020(void *, s32);
extern u32 Data_03001eec;

void Func_080d6888(
    s32 set_id,
    s32 object_value,
    s32 group_value,
    s32 state_slot,
    s32 state_value)
{
    void *set;
    void *group;
    u8 *state;
    s32 group_index;

    set = Func_080b5098(set_id);
    state = (u8 *)Data_03001eec;
    group_index = 0;

    while ((group = Func_080b50d8(SET_OWNER(set), group_index)) != NULL) {
        if (state_slot != -1) {
            s32 state_offset = state_slot + 0x7818;

            state[state_offset] = ((u8 *)&state_value)[0];
        }

        if (SET_SUPPRESS_UPDATES(set) == 0) {
            if (object_value != -1) {
                s32 object_index;

                object_index = 0;
                if (GROUP_OBJECT_COUNT(group) != 0) {
                    void **objects;

                    objects = (void **)((u8 *)group + 0x28);
                    do {
                        void *object;

                        object = *objects++;
                        if (object != NULL
                            && object != SET_EXCLUDED_24(set)
                            && object != SET_EXCLUDED_20(set)) {
                            if (object_value == 0)
                                OBJECT_VALUE(object) = Func_080b5100(set_id);
                            else
                                OBJECT_VALUE(object) = object_value;
                            OBJECT_REFRESH(object) = 0xff;
                        }
                        object_index++;
                    } while (object_index != GROUP_OBJECT_COUNT(group));
                }
            }

            if (group_value != -1)
                Func_08009020(group, group_value);
        }
        group_index++;
    }
}
