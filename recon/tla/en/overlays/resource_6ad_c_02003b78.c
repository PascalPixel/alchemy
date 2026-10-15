/* Near miss of overlay 6ad (the same function is in overlays 6aa and 69e): differ 2. The listing loads x
 * (ldr r6, [r0, #8]) between building 64 and copying it for the first mask; the draft copies first. Every
 * placement of an empty asm or one-pass block around split x and z statements leaves the same 2. */
#include "../../../../games/tla/src/scene/mars_room_f/room.h"

struct WarpEntry {
    s16 x;
    s16 z;
    s16 flag;
    s16 unk6;
};

struct WarpWork {
    s16 unk00;
    s16 positionFlag;
    s16 unk04;
    s16 sourceX;
    s16 sourceZ;
    s16 count;
    s16 current;
    s16 busy;
    u8 unk10[0x10];
    struct WarpEntry entries[1];
};

void Func_02003af0(void);

void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
    s32 x = (object->x >> 20) | (s16)(work->sourceX & 64);
    s32 z = (object->z >> 20) | (s16)(work->sourceZ & 64);

    Func_02003af0();
    Func_020056c0(161);
    Func_020054e0_(work->sourceX, work->sourceZ + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
