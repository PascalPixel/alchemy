#include "colosso_log_rolling_stage.h"

/*
 * The two rate words and state byte remain byte-offset accesses because this
 * owner needs the temporary pointer walk to preserve register allocation.
 * Its six imported calls are per-site veneers whose semantics are unproven.
 */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */
extern u8 *Func_020086f0(s32 object_id);
extern void Func_020084a4(void);
extern void Func_0200848c(u8 *object, s32 mode);
extern void Func_020084c8(u8 *object, s32 x, s32 y, s32 z);
extern void Func_020084d6(u8 *object);
extern void Func_020084a6(u8 *object, s32 enabled);

void ColossoLogRollingStage_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    u8 *object = Func_020086f0(object_id);

    if (object == 0) {
        return;
    }

    {
        s32 move_rate = 0x14000;
        u8 *state_byte = object;
        u8 zero = 0;
        *(s32 *)(object + 0x30) = move_rate;
        *(s32 *)(object + 0x34) = move_rate >> 1;
        state_byte += 0x5b;
        *state_byte = zero;
    }

    Func_020084a4();
    Func_0200848c(object, 5);
    Func_020084c8(object, x << 16, *(s32 *)(object + 12), z << 16);
    Func_020084d6(object);
    Func_020084a6(object, 1);
}
