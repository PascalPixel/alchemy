/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/lib/overlay_call_targets.ts`.  The band
 * 0x02002468-0x0200261f is an 8-byte-per-entry import veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <main-image address>`); calls landing there
 * are named by that trailing word, as the semantic resource_39f sources do.
 * Calls landing on an overlay prologue keep the overlay's `Func_0200xxxx` name.
 *
 * Link base: resource_3b4 is linked at 0x02008000.  The image is 0x2f84 bytes
 * and the table at 0x02002d40-0x02002f84 holds 59 words of the form
 * `<function offset> + 0x8000 + 1` (Thumb bit), e.g. 0x02008ec9 = Func_02000ec8.
 * So a pool word in 0x02008000-0x0200af83 is in-image data at value - 0x8000.
 * `Data_02000240` lies below the base and is referenced by byte-exact sources
 * in many other overlays, so it is a shared EWRAM global, not overlay data.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * is returned and the owner is void.
 *
 * Declarations are old-style because imports here are reached with differing
 * argument counts from different call sites.
 */
#include "types.h"

/*
 * resource_3b4 owner at 0x02000ccc, 388 bytes: code 0x02000ccc-0x02000e35,
 * two bytes of alignment, and the pool words 0x000009c9, 0x000009ca,
 * 0x00000101, 0x00000372, 0x0000036e and 0x02008aa9 at 0x02000e38.
 *
 * The third and last cutscene of the 0x02000ad0 / 0x02000b68 chain: it needs
 * flag 0x9c9 (set by 0x02000b68) and sets its own flag 0x9ca, which is in turn
 * the flag 0x02000e50 waits on.
 *
 * r5 holds the second Func_080770c0 result, already proved zero by the guard,
 * and is reused as the zero written into the halfword at +30 of the record
 * hanging off slot 15's pointer at +80.
 *
 * The body raises slot 15, then walks it through a nine-stop zig-zag whose
 * columns alternate between 880, 882 and 878 while the row climbs 680, 696,
 * 704, 712, 720, 728, 736, 744, 752, 760.  It closes by installing this
 * overlay's own 0x02000aa8 as slot 15's callback: the pool word 0x02008aa9 is
 * 0xaa8 + 0x8000 + 1, the in-image address with the Thumb bit, and 0x02000aa8
 * really is a `push {r5, lr}` prologue.  That is another witness for the
 * 0x02008000 link base.
 *
 * Constants are `movs`/`lsls` pairs except the three column values, which come
 * from the pool: 0x8000 and 30 for the first Func_0808a1b8, 258 for
 * Func_0808a1f0, 0x10000/0x8000 and 0x20000/0x10000 for Func_0808a090,
 * 0x80000 and 0x40000 for the two +40 stores, (0x3580000, 0x3380000) for
 * Func_0808a0f0, and 0xc000/20 plus 256 for the closing pair.
 */

/* Scene flag test: zero means not yet set. */
s32 Func_020031da();
s32 Func_020031e6();
void Func_020031fc();
void Func_02003218();
s32 *Func_02003236();
void Func_020031b0();
void Func_02003306();
s32 *Func_0200324c();
void Func_020032b6();
void Func_020032d0();
void Func_020032b8();
void Func_02003256();
void Func_0200328c();
void Func_0200334a();
s32 *Func_02003290();
void Func_020032c4();
void Func_0200328a();
void Func_0200331a();
void Func_020032c8();
void Func_020032ee();
void Func_020032fa();
void Func_02003308();
void Func_02003314();
void Func_02003322();
void Func_0200332e();
void Func_0200333c();
void Func_02003348();
void Func_02003356();
void Func_0200337c();
void Func_0200332a();
void Func_020033ae();
void Func_020033c8();
s32 *Func_02003366();
void Func_0200335e();
                    
/* Scene flag set. */

/* Returns the record for a numbered slot. */

                     

                     

                     

/* The callback this owner installs; defined at 0x02000aa8 in this overlay. */
void Func_02000aa8(void);

void Func_02000ccc(void)
{
    s32 clear;
    s32 *slot;

    if (Func_020031da(0x9c9) == 0) return;

    clear = Func_020031e6(0x9ca);
    if (clear != 0) return;
    Func_020031fc(0x9ca);

    Func_02003218();

    *(u16 *)((u8 *)(s32 *)slot[20] + 30) = (u16)clear;   /* [+80] -> +30 */
    slot = Func_02003236(15);
    Func_020031b0(slot, 16);

    Func_02003306(152);

    Func_0200324c(15)[10] = 0x80000;   /* +40 */
    Func_020032b6(15, 0x8000, 30);
    Func_020032d0(15, 258);
    Func_020032b8(15, 2);
    Func_02003256(20);
    Func_0200328c(15, 0x10000, 0x8000);
    Func_0200334a(152);

    Func_02003290(15)[10] = 0x40000;   /* +40 */
    Func_020032c4(15, 880, 680);
    Func_0200328a(10);
    Func_0200331a(15, 0x101);
    Func_020032c8(15, 0x20000, 0x10000);

    Func_020032ee(15, 880, 696);
    Func_020032fa(15, 882, 704);
    Func_02003308(15, 880, 712);
    Func_02003314(15, 878, 720);
    Func_02003322(15, 880, 728);
    Func_0200332e(15, 882, 736);
    Func_0200333c(15, 880, 744);
    Func_02003348(15, 878, 752);
    Func_02003356(15, 880, 760);

    Func_0200337c(15, 0x3580000, 0x3380000);
    Func_0200332a(10);
    Func_020033ae(15, 0xc000, 20);
    Func_020033c8(15, 256);

    Func_02003366(15)[27] = (s32)Func_02000aa8;   /* +108 */
    Func_0200335e();
}
