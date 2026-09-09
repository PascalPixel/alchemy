#include "types.h"
#include "global_cells.h"

void Func_080049ac(void);
s32 GameFlag_IsSet(s32);
s32 Func_080051e8(void *, void *);
s32 Func_080051d8(void *, void *);
/*
 * The call in the taken branch is a typed indirect call to the relocated
 * routine at 0x030002C0, whose argument count is not established.
 * Func_080072f0 names the bx rN veneer slot that reaches it, not a routine
 * at that address.
 */
void Func_080072f0(void *);
extern u8 Data_080c2a7c[];

s32 Camera_ApplyTransformByFlag(void)
{
    u8 *state = *(u8 **)ADDR_03001E80;
    Func_080049ac();
    if (GameFlag_IsSet(0x16B) != 0) {
        ((void (*)(void *))0x030002C0)(Data_080c2a7c);
        return Func_080051e8(state, state + 0xC);
    } else {
        return Func_080051d8(state, state + 0xC);
    }
}
