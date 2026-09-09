/*
 * Write one workspace slot of the save state, returning nonzero when
 * either step reports a nonzero result.
 */
#include "types.h"
#include "global_cells.h"

/*
 * Func_080072f0 names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address rather than a parameter of the callee.
 */
s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_08006c68(u16, s32);

struct Work_08005868 {
    u8 unknown_00[64];
    s32 data;
};

/*
 * The old-style definition is deliberate: with a prototype the u16
 * parameter would arrive unpromoted and the mask below would change. The
 * word at 0x02004C04 is the address called while its pointer is passed as
 * an argument; the shift by 16 tests the low halfword of the result.
 */
u32 SaveState_WriteWorkspaceSlot(code)
u16 code;
{
    s32 *param = (s32 *)0x02004C04;
    s32 result;
    struct Work_08005868 *work;
    s32 value;

    work = *(struct Work_08005868 **)ADDR_03001F1C;
    value = code & 0xFFFF;
    if ((Func_080072f0(value, (s32)&work->data,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = Func_08006c68(value, (s32)&work->data);
    return (u32)((0 - result) | result) >> 0x1F;
}
