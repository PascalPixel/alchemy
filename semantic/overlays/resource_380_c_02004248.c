typedef int s32;

/*
 * resource_380 owner at 0x02004248, 24 bytes including the trailing
 * alignment halfword: the overlay's 97x-called workhorse turns out to
 * be a two-call convenience wrapper.  Complete owner: `push {r5, lr}`
 * at 0x02004248 through `pop {r0} / bx r0` at 0x0200425c; next owner's
 * prologue at 0x02004260.
 *
 * This overlay executes at 0x02000000 (its bl targets land in the
 * 0x02008xxx resident module, which fits only under a flat base; the
 * image is 0x5190 bytes).  Callees are unidentified beyond call shape.
 */

extern void Func_02008cb4(s32 arg0, s32 arg1);
extern void Func_02008bea(s32 arg0);

void Func_02004248(s32 arg0, s32 arg1)
{
    Func_02008cb4(arg0, 0);
    Func_02008bea(arg1);
}
