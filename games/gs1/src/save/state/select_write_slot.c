#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"
#include "runtime_interfaces.h"

/* save/state/select_write_slot.c */
u32 Random16(void);

u32 SaveState_SelectWriteSlot(s32 mode)
{
    u32 empty[16];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = *(u8 **)ADDR_03001F1C;
    count = 0;
    index = 0;
    do {
        value = *slot++;
        if (value == 0) {
            empty[count++] = index;
        }
        index += 1;
    } while (index <= 0xFU);
    index = 0x10;
    if (count != 0) {
        if (count == 1) {
            index = empty[0];
            if (State_Do(mode) == 0x10) {
                index = 0x10;
            }
        } else {
            index = UnsignedModulo(Random16(), count);
            index = empty[index];
        }
    }
    return index;
}

/* save/state/write_workspace_slot.c */
s32 State_SetMode(s32, s32, s32, s32);
s32 State_Apply(u16, s32);

struct Work_08005868 {
    u8 unknown_00[64];
    s32 data;
};

u32 SaveState_WriteWorkspaceSlot(code)
u16 code;
{
    s32 *param = (s32 *)0x02004C04;
    s32 result;
    struct Work_08005868 *work;
    s32 value;

    work = *(struct Work_08005868 **)ADDR_03001F1C;
    value = code & 0xFFFF;
    if ((State_SetMode(value, (s32)&work->data,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = State_Apply(value, (s32)&work->data);
    return (u32)((0 - result) | result) >> 0x1F;
}
