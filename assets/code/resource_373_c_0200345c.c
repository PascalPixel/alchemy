typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 373: reset or tear down a consecutive run of scene actors.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8 save at 0x0200345c
 * through the single epilogue at 0x020034b8..0x020034c0, followed by its
 * one-word literal pool at 0x020034c4.  Both loops fall into that epilogue,
 * and no frame or high-register state continues past it.
 *
 * All three calls are placed.  None is an interworking `call_via rN` site.
 *
 * The two loop counters are compared with `bcs`/`bcc`, so the run length is
 * unsigned; the actor selector is a plain integer that is incremented once
 * per iteration.
 *
 * UNCERTAINTY: the called service addresses are the ones encoded in the
 * overlay image, which is the convention this tree already uses for overlay
 * calls whose targets share the 0x02000000 namespace.
 */

struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

extern struct Resource373Actor *Func_02009458(s32 selector);
extern void Func_020093d0(struct Resource373Actor *actor, s32 mode);
extern void Func_020094ec(s32 selector, s32 a, s32 b);

void Func_0200345c(s32 firstSelector, u32 count, s32 mode)
{
    s32 selector = firstSelector;
    u32 index;

    if (mode == 0) {
        for (index = 0; index < count; index++) {
            struct Resource373Actor *actor = Func_02009458(selector);

            actor->flag55 = 0;
            Func_020093d0(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17 */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16 */
            actor->field10 = 0x034a0000;   /* pool word */
            selector++;
        }
        return;
    }

    for (index = 0; index < count; index++) {
        Func_020094ec(selector, 0, 0);
        selector++;
    }
}
