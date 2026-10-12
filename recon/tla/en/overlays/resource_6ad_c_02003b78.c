/* Near miss of overlay 6ad (the same function is in overlays 6aa and 69e): differ 2. The listing loads x
 * (ldr r6, [r0, #8]) between building 64 and copying it for the first mask; the draft copies first. Every
 * placement of an empty asm or one-pass block around split x and z statements leaves the same 2. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

struct WarpEntry {
    s16 x;
    s16 z;
    s16 flag;
    s16 unknown_6;
};

struct WarpWork {
    s16 unknown_00;
    s16 position_flag;
    s16 unknown_04;
    s16 source_x;
    s16 source_z;
    s16 count;
    s16 current;
    s16 busy;
    u8 unknown_10[0x10];
    struct WarpEntry entries[1];
};

void Func_02003af0(void);

void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 x = (object->x >> 20) | (s16)(work->source_x & 64);
    s32 z = (object->z >> 20) | (s16)(work->source_z & 64);

    Func_02003af0();
    Func_020056c0(161);
    Func_020054e0_(work->source_x, work->source_z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
