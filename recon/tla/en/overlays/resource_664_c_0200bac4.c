/*
 * Draft of Func_02003ac4 (overlay 664, 0200bac4). differ 4: x's shift is
 * emitted before y's literal load, where ⚓️ loads the literal first and
 * shifts x last. The direct call and call.h's Call4 give the same order.
 */
#include "types.h"
#include "party_state.h"

void OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005e84(s32 id, s32 a, s32 b);
void Func_02005efc(s32 value);
void Func_02005f2c(s32 value);

void Func_02003ac4(void)
{
    Func_02005e14();
    Func_02005f2c(0);
    OverlayObject_PrepareObject(0xd80000, 0xffe00000, 0x2200000, 286);
    Func_02005e84(gPartyState.current_owner, 6, 0);
    Func_02005efc(3);
    Func_02005e1c();
}
