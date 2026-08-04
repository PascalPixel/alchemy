#include "types.h"

struct BlendState_080c10e8 {
    u8 unknown_000[0x64e];
    u16 blend_step;
    u16 blend_mode;
};

extern struct BlendState_080c10e8 *Data_03001e74;

void Func_080c1084(void);
void Func_080c1054(void);
void Func_080c0f98(s32 object_id, s32 mode);
void Func_08004278(void (*callback)(void));
void Func_080030f8(s32 frames);
s32 Func_0800387c(s32 register_address, s32 value);
s32 Func_080041d8(void (*callback)(void), s32 priority);
s32 Func_080b6c08(s32 groups, s16 *object_ids);

/*
 * Enable or disable the pulsing blend effect.  An optional terminated object
 * list receives the opposite palette mode from the currently active objects.
 */
s32 Func_080c10e8(const u16 *opposite_objects, s32 mode)
{
    struct BlendState_080c10e8 *state = Data_03001e74;
    s32 result = (s32)opposite_objects;

    if (mode == 0) {
        Func_08004278(Func_080c1084);
        *(volatile u16 *)0x04000054 = 0;
        Func_080c1054();
        Func_080030f8(1);
        result = Func_0800387c(0x04000050, 0);
    }

    if (state != 0 && mode != 0) {
        s16 active_objects[14];
        s32 active_count;
        s32 index;

        state->blend_mode = (u16)mode;
        state->blend_step = 0;
        *(volatile u16 *)0x04000054 = 0;
        *(volatile u16 *)0x04000052 = 0x10;

        active_count = Func_080b6c08(3, active_objects);
        for (index = 0; index < active_count; index++) {
            Func_080c0f98(active_objects[index], mode & 1);
        }

        if (opposite_objects != 0) {
            index = 0;
            while (index < 14 && opposite_objects[index] != 0xff) {
                Func_080c0f98(
                    opposite_objects[index],
                    (mode & 1) ^ 1);
                index++;
            }
        }

        Func_080030f8(1);
        Func_0800387c(0x04000050, 0);
        result = Func_080041d8(Func_080c1084, 0x480);
    }

    return result;
}
