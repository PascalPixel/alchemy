#include "types.h"

/*
 * resource_3a4: a published callback that sets the mode of actor record 8.
 */

extern u32 Data_03001e40;

extern void Func_02006c2a();
extern void Func_02006c34();

/*
 * Bit 1 of the runtime status word at 0x03001e40 selects mode 7 or mode 6
 * for record 8. That bit's meaning is unverified: other callbacks here mask
 * different bits of the same word. Both branches reach the same veneer, and
 * the declarations carry no parameter list because the arguments are set up
 * in registers at the call site. The owner spans 44 bytes -- the body, one
 * alignment halfword and one literal pool word.
 */
void SceneActor_SetActor8ModeByCounterBit(void)
{
    if (((Data_03001e40 >> 1) & 1) != 0) {
        Func_02006c2a(8, 7);
    } else {
        Func_02006c34(8, 6);
    }
}
