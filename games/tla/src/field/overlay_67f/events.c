#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02000eac();
struct ObjectRuntime *Object_GetById();

void Func_02001184(void)
{
    struct ObjectRuntime *object0;
    s32 result1;

    object0 = Object_GetById(gPartyState.current_owner);
    result1 = Value3((s32 (*)())Func_02000eac, 2, (object0->x >> 20), (object0->z >> 20));
    if ((result1 >> 8) != 232) goto L_020091b2;
    object0->terrain_id = 2;
L_020091b2:
    return;
}

void Func_020011b8(void)
{
    struct ObjectRuntime *object0;

    object0 = Object_GetById(gPartyState.current_owner);
    object0->terrain_id = 0;
    return;
}
