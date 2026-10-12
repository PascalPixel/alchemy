#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02000188(void);
void Func_020001cc(void);
void Func_020002cc(void);
void Func_020006e4(void);

static __inline__ void AddCallback(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

void Func_02000b84(void)
{
    Object_GetById(gPartyState.current_owner)->priority_flags |= 0x20;
    AddCallback(Func_02000188, 0x480);
    AddCallback(Func_020001cc, 0x480);
    if (gPartyState.entrance == 10)
        Func_020002cc();
    do { } while (0); /* FAKEMATCH: the one-pass block keeps the second entrance test a load of its own (ldrsh, cmp #20); without it the two tests share a halfword load and compare shifted (differ 33). */
    if (gPartyState.entrance == 20)
        Func_020006e4();
}
