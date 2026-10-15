/* Draft, differ 16 of 42: the native loop is not touched by loop.c (gPartyState reloaded each pass, no strength reduction, no count-down), which this goto loop reproduces; only the pp and count registers are swapped (r6/r7). As a for loop: differ 45 (reduced and reversed). Tried: statement and declaration order; permute 1 minute. */
#include "types.h"
#include "object_runtime.h"
#include "owner_state.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 UnitPpAdd();
void Func_02003c3c(void);
void Func_02004a1c(void);

void Func_0200050c(void)
{
    s32 count;
    s32 pp;
    s32 i;

    gPartyState.render_mode = 7;
    Func_02003c3c();
    Func_02004a1c();
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    count = Party_CountActiveOwners();
    pp = 0;
    i = 0;
    if (i < count) {
    loop:
        pp += ((struct BtlUnit *)UnitGet(gPartyState.active_owners[i]))->pp;
        i++;
        if (i < count)
            goto loop;
    }
    if (pp == 0)
        UnitPpAdd(gPartyState.current_owner, 1);
}
