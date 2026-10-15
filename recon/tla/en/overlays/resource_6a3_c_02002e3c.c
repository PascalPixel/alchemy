/*
 * Draft of overlay 6a3 Func_02002e3c, the scene's map preparation. 2
 * instructions differ: the listing builds the last call's zero argument
 * (movs r2, #0) right after the motion flag store and before the two position
 * loads, the draft after them. The six-argument calls go through Call6 and
 * the three-argument one through Call3 (direct calls: 12 differ). Tried for
 * the last call: Call4, a prototyped direct call (both 2), a named zero
 * shared with the flag store (4), a barrier on the zero (10).
 */
#include "types.h"
#include "field_event.h"
#include "call.h"

void Scene_SetArrivalFlags();
void Func_02002fc4();
void Func_02003a38();
void Func_02003ac0();
struct FieldActor *Object_GetById();
s32 GameFlag_Test();
void Func_02004844();
void Func_0200474c();
void Func_020049ac();
void Func_02004774(s32 x, s32 z, s32 a, s32 b);

s32 Func_02002e3c(void)
{
    struct FieldActor *object;

    Scene_SetArrivalFlags(0);
    Func_02002fc4(1);
    Func_02003a38();
    Func_02003ac0(20, 0);
    object = Object_GetById(20);
    if (GameFlag_Test(0xa0c)) {
        Call3(Func_02004844, 20, 0x3f80000, 0xf80000);
        Call6(Func_0200474c, 104, 42, 5, 11, 61, 42);
        Call6(Func_0200474c, 106, 52, 1, 1, 63, 84);
        Func_020049ac(0x4100000, 0xa00000, 2, 236);
    }
    object->motionFlags = 0;
    Func_02004774(object->x, object->z, 0, -6);
    return 0;
}
