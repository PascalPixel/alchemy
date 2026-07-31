typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x020045d0 (160 bytes incl. pool,
 * 4 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` plus `r8` spill at 0x020045d0, epilogue `pop {r3} / mov
 * r8,r3 / pop {r5,r6} / pop {r1} / bx r1` at 0x0200465e-0x02004666. The
 * two-word pool at 0x02004668-0x0200466c (0x0200e968, a cached-object
 * global; 0x01c70000) is included per the usual pool rule, immediately
 * followed by the next owner's push {r5,r6,lr} at 0x02004670, already
 * this overlay's row `0x02004670 | 7 calls`, so the span is
 * 0x020045d0-0x02004670, 160 bytes.
 *
 * A get-or-create singleton: if a lookup call finds an existing cached
 * instance, return it straight from the `Data_0200e968` global; else
 * allocate a new one, clear/mask several of its (and its linked
 * object's) fields, run two setup calls whose exact argument shapes are
 * transcribed faithfully (the second call's own return value is
 * discarded, clobbered before use), cache the new object in the global,
 * and return it.
 *
 * No meaningful incoming argument -- the reference overwrites r0 before
 * ever reading it.
 *
 * Raw callee naming.
 */

extern u8 *Data_0200e968;

u8 *Func_0200aa7c();
u8 *Func_0200aa58();
s32 Func_0200aa66();
void Func_0200aace();
void Func_0200aa8e();
void Func_0200aa8c(void);
void Func_0200aafc();

u8 *Func_020045d0(void)
{
    u8 *cached = Func_0200aa7c(0x200);

    if (cached != 0) {
        return Data_0200e968;
    }

    {
        u8 *obj = Func_0200aa58(22, 0x01c70000, 0x40000, 0x2200000);
        u8 *linked;
        s32 arg2;

        obj[0x55] = 0;
        obj[0x5c] = 1;

        linked = *(u8 **)(obj + 0x50);
        linked[0x27] = 0;
        linked[5] &= (u8)~0x20;
        linked[9] &= 0xf;

        arg2 = Func_0200aa66(17, 0x608) + 0x400;
        Func_0200aace(0xe8);
        Func_0200aa8e(linked[0x1c], 128, arg2);
        Func_0200aa8c();
        Func_0200aafc(0x200);

        Data_0200e968 = obj;
        return obj;
    }
}
