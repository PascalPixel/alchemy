#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "event_work.h"
#include "field_scene.h"

extern s32 Data_02002618;

struct ObjectRuntime *Object_GetById(s32 id);
void Func_0200130c(s32 kind, s32 x, s32 z, s32 a, s32 b);
void Func_02001a28(s32 dx, s32 dz, s32 effect);
void Func_02000460(s32 mode);

// The scene's set-up: screen effect 0x204, actor 64's action variant 3 and
// actor 8 held still, then Func_0200130c and Func_02001a28 by the entrance.
s32 Func_02000290(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 0x204;
    ObjectMotion_SetActionVariant(64, 3);
    Data_02002618 = 0;
    Object_GetById(8)->motion_flags = 0;
    if ((u16)(gPartyState.entrance - 6) <= 1) {
        Func_0200130c(33, 0x280, 0x380, 48, 1);
        Func_02001a28(0, 0, 0x204);
    } else if (gPartyState.entrance == 3 || gPartyState.entrance == 5 || gPartyState.entrance == 13) {
        Func_0200130c(43, 0x280, 0x380, 49, 1);
        Func_02001a28(-21, 32, 0x204);
    } else {
        Func_0200130c(43, 0x280, 0x380, 0, 0);
        Func_02001a28(-2, -27, 0x204);
    }
    return 0;
}

s32 Func_02000358(void)
{
    Scene_SetArrivalFlags(0);
    Func_02000460(0);
    return 0;
}
