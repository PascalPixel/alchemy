#include "types.h"

/*
 * resource_392 owner at 0x02000a68, 84 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools-rs/overlay-driver resource_392), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Body: write 516 to the workspace's +448 s32; unless story flag
 * 0xfd3 is set, run Func_02000cb4(11) (this overlay's drafted row);
 * always run Func_02001358 on records 8, 9 and 10; unless flag 0x845
 * is set, run Func_02001564(11); return 0.  Both gates are
 * "flag clear -> act", which is the shape a scene uses for work that
 * must not repeat once its flag is raised.
 *
 * Complete owner: `push {lr}` at 0x02000a68 through `movs r0, #0 /
 * pop {r1} / bx r1` at 0x02000aaa-0x02000aae, then the three-word
 * literal pool 0x02000ab0-0x02000abb (0x03001ebc, 0xfd3, 0x845);
 * the next owner is 0x02000abc, which this driver itself calls.
 * Returns a literal 0.
 *
 * Local callees: Func_0200173a is already drafted
 * (resource_392_c_02000cb4.c); Func_02001352 and Func_02000abc are
 * not yet owned and are declared here only.
 *
 * Uncertainty: the flag ids are literal, and the record ids 8/9/10
 * and 11 are passed through unexamined -- what Func_0200134c and
 * Func_02000abc do with them is open until those rows are drafted.
 */

extern u8 *Data_03001ebc;

extern void Func_020008c0(s32 id);
extern void Func_02000abc(s32 id);
extern void Func_02000cb4(s32 id);

extern s32 Func_020018a2(s32 flagId);

s32 Func_02000a68(void)
{
    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Func_02001880(0xfd3) == 0) {
        Func_0200173a(11);
    }
    Func_0200134c(8);
    Func_02001352(9);
    Func_02001358(10);
    if (Func_020018a2(0x845) == 0) {
        Func_02001564(11);
    }
    return 0;
}
