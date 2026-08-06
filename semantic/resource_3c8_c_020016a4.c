#include "types.h"

/*
 * BYTE-EXACT but NOT adoptable: overlay_adopt refuses the span because the
 * audited thumb interval starts at 0x020016a6, one halfword after this
 * owner's 0x020016a4 start, so 0x020016a4..0x02001780 is not inside a single
 * audited interval.  differing_halfwords=0 as written.
 */

/*
 * Resource 3c8 at 0x020016a4: gate a cutscene on whether the player actor
 * stands inside a small rectangle of the map.
 *
 * The owner is complete: `push {r5, lr} ; sub sp, #8` at 0x020016a4 and the
 * matching `add sp, #8 ; pop {r5} ; pop {r0} ; bx r0` at
 * 0x0200176c..0x02001772, followed by a three-word literal pool at
 * 0x02001774-0x0200177f that ends the row.  No live frame or register state
 * escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The rectangle test is the standard unsigned range idiom: the signed
 * halfword at +0x0a has -676 (pooled 0xfffffd5c) added to it and the result
 * is compared unsigned against 7, so the first coordinate must lie in
 * [676, 683].  The signed halfword at +0x12 must then satisfy
 * 788 <= v < 796 (197<<2 and 199<<2).  Any failure takes the "outside" arm.
 *
 * Field offsets: 0x0a and 0x12 are the signed halfword pair the byte-exact
 * sibling 0x020014f4 reads as `unkA`/`unk12`; 0x0c and 0x14 are the pair the
 * byte-exact sibling 0x02001644 writes with the same pooled 0xfffe0000, in
 * the same order (0x14 first); 0x55 is the `mode55` byte written by
 * semantic/overlays/resource_3c8_c_02001d48.c.
 *
 * Halfword coordinate views: the signed halfwords at +0x0a and +0x12 are the
 * high halves - the integer parts - of the 16.16 words at +0x08 and +0x10.
 * The byte-exact sibling 0x020014f4 models the same record both ways, as
 * `Struct_A` (s32 at 8 and 0x10) and `Struct_B` (s16 at 0x0a and 0x12).
 *
 * The eight reserved stack bytes carry arguments five and six of this
 * package's six-argument placement ABI: r0-r3 then [sp,#0] then [sp,#4], the
 * order fixed by the byte-exact sibling 0x020010cc.
 *
 * Twelve `bl` sites reach twelve distinct targets, all accounted for:
 * 0x02006558, 0x020064fc, 0x02006510, 0x0200657e, 0x02006588, 0x020065b0,
 * 0x020065ae, 0x02006550, 0x02006564, 0x020065ca, 0x02006750, 0x02006606.
 * Import naming follows the note in resource_3c8_c_020002f0.c.
 */

struct Actor_020016a4 {
    u8 unknown_00[10];
    s16 unkA;                   /* 0x0a */
    s32 unkC;                   /* 0x0c */
    u8 unknown_10[2];
    s16 unk12;                  /* 0x12 */
    s32 unk14;                  /* 0x14 */
    u8 unknown_18[0x3d];
    u8 mode55;                  /* 0x55 */
};

/*
 * CALL SYMBOLS ARE PER-SITE: every `bl` here is a direct relocation against
 * an absolute .set symbol at its own call site (verified via objdump on the
 * assembled .o) -- lifted verbatim, not the veneer-math resolved names this
 * file used before.
 */
struct Actor_020016a4 *Func_02006558();
void Func_020064fc();
void Func_02006510();
void Func_0200657e();
s32 Func_02006588();
void Func_020065b0();
void Func_020065ae();
void Func_02006550();
void Func_02006564();
void Func_020065ca();
void Func_02006750();
void Func_02006606();

void Func_020016a4(void)
{
    struct Actor_020016a4 *actor;
    s16 across;
    s16 along;

    actor = Func_02006558(0);
    across = actor->unkA;
    along = actor->unk12;

    if ((u32)(across - 676) > 7 || along < 788 || along >= 796) {
        Func_020064fc(53, 50, 42, 49, 1, 1);
        { s32 fifth = 3; s32 last = 5; Func_02006510(55, 117, 41, 117, fifth, last); }
        Func_0200657e(0x201);
        actor->mode55 |= 1;
        actor->unk14 = 0;
        actor->unkC = 0;
        return;
    }

    if (Func_02006588(0x201) != 0) {
        return;
    }

    /* No argument register is written here; r0 still holds the 0 just
     * returned above (Func_02006588 returned 0 on this path), and that
     * dataflow is preserved as written by calling with no argument at all
     * (old-style declaration, so this is legal and emits no `movs r0,#0`). */
    Func_020065b0();

    Func_020065ae(5);
    Func_02006550(52, 50, 42, 49, 1, 1);
    { s32 fifth = 3; s32 last = 5; Func_02006564(52, 117, 41, 117, fifth, last); }
    Func_020065ca(0x201);
    Func_02006750(161);
    actor->mode55 &= 0xfe;
    actor->unk14 = 0xfffe0000;
    actor->unkC = 0xfffe0000;
    Func_02006606();
}
