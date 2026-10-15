#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02003e64();
void Func_02003e6c();
void Func_02003f0c();
void Func_02003fa4();
struct ObjectRuntime *Object_GetById();

void Func_0200208c(void)
{
    ((void (*)())Func_02003e64)();
    Call1((void (*)())Func_02003fa4, 0);
    Call3((void (*)())Func_02003f0c, gPartyState.current_owner, 6, 1);
    ((void (*)())Func_02003e6c)();
    return;
}

void Func_020020b4(void)
{
    struct ObjectRuntime *object0;

    object0 = Object_GetById(gPartyState.current_owner);
    object0->terrain_id = 2;
    return;
}

void Func_020020d0(void)
{
    struct ObjectRuntime *object0;

    object0 = Object_GetById(gPartyState.current_owner);
    object0->terrain_id = 0;
    return;
}
