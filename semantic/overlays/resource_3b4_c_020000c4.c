/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/overlay_call_targets.ts`.  The band
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
 * resource_3b4 owner at 0x020000c4, 384 bytes: code 0x020000c4-0x02000235,
 * two bytes of alignment, and the pool words 0x0200a644, 0xffff0000 and
 * 0x00003333 at 0x02000238.
 *
 * Push the object standing in front of the party one cell along the party's
 * facing.  0x020002a8 in this overlay is the per-frame guard for the same
 * mechanic and uses the same step table.
 *
 * The facing is the top nibble of the party record's halfword at +6
 * (`lsrs #12`), indexing the sixteen-word step table `Data_0200a644` --
 * in-image data at file offset 0x2644 under this overlay's 0x02008000 link
 * base.  Each word packs two 16.16 deltas: `& 0xffff0000` is the X delta in
 * place and `<< 16` lifts the low halfword into the Z delta.  The table word
 * is re-read from memory before each candidate rather than cached, and the
 * three-word candidate is rebuilt on the stack each time; both are as the
 * assembly has them.
 *
 * Four things must hold before the push happens: something must occupy the
 * cell ahead; the cell beyond it must be free or occupied by something whose
 * byte at +89 does not have bit 0 set; the cell above the pushed object must
 * pass the same test (that probe raises the Y word by 0x100000, `movs #128 /
 * lsls #13`); and the terrain probe Func_080091d8 must not block it.  Finally
 * the pushed object's byte at +98 must be zero.
 *
 * That last byte is loaded into sl and, once the zero test has passed, sl is
 * reused as the constant 0 written into the +36 and +44 words of both records;
 * the assembly really does share the register, and the value is zero either
 * way.
 *
 * Both records are given the marker 0x3333 in their words at +48 and +52 (the
 * pool word held in r5 across the two calls before it), the party's motion
 * words at +56 and +64 are set to 0x80000000 (`movs #128 / lsls #24`), and the
 * party's 16.16 position words at +8 and +16 are rebuilt from the signed
 * halfwords at +10 and +18 -- the integer-part views of those same words,
 * which is the pairing this project records for these records.
 *
 * The prologue saves r8, r9 and sl by hand and the epilogue restores them
 * before `pop {r5, r6, r7} / pop {r0} / bx r0`, so the owner is void.
 */

/* Sixteen packed step words, in-image at file offset 0x2644. */
extern u32 Data_0200a644[];

/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
/* Byte-exact in this overlay: returns the occupying slot record, or 0. */
s32 *Func_0200006c();
/* Terrain probe; a positive result blocks the move. */
s32 Func_080091d8();
void Func_08009080();
void Func_080000c0();
void Func_080f9010();
void Func_08009150();
void Func_08009158();
void Func_0808a5e8();

void Func_020000c4(void)
{
    s32 *actor = Func_0808a080(0);
    s32 facing = *(u16 *)((u8 *)actor + 6) >> 12;
    s32 candidate[3];
    s32 *target;
    s32 *blocker;
    u32 step;
    s32 zero;

    step = Data_0200a644[facing];
    candidate[0] = actor[2] + (s32)(step & 0xffff0000);   /* +8  */
    candidate[1] = actor[3];                              /* +12 */
    candidate[2] = actor[4] + (s32)(step << 16);          /* +16 */

    target = Func_0200006c(candidate, actor);
    if (target == 0) return;

    /* The cell beyond the target, along the same facing. */
    step = Data_0200a644[facing];
    candidate[0] = target[2] + (s32)(step & 0xffff0000);
    candidate[1] = target[3];
    candidate[2] = target[4] + (s32)(step << 16);

    blocker = Func_0200006c(candidate, target);
    if (blocker != 0 && (((u8 *)blocker)[89] & 1) != 0) return;

    /* The cell above the target. */
    candidate[0] = target[2];
    candidate[1] = target[3] + 0x100000;
    candidate[2] = target[4];

    blocker = Func_0200006c(candidate, target);
    if (blocker != 0 && (((u8 *)blocker)[89] & 1) != 0) return;

    ((u8 *)target)[34] = 2;

    step = Data_0200a644[facing];
    candidate[0] = target[2] + (s32)(step & 0xffff0000);
    candidate[1] = target[3];
    candidate[2] = target[4] + (s32)(step << 16);

    if (Func_080091d8(target, candidate) > 0) return;

    zero = ((u8 *)target)[98];
    if (zero != 0) return;

    Func_08009080(actor, 8);
    Func_080000c0(15);
    Func_080f9010(185);

    target[12] = 0x3333;   /* +48 */
    target[13] = 0x3333;   /* +52 */
    Func_08009150(target, candidate[0], candidate[1], candidate[2]);

    actor[12] = 0x3333;
    actor[13] = 0x3333;
    Func_08009150(actor, candidate[0], candidate[1], candidate[2]);

    Func_08009158(target);
    Func_0808a5e8();

    target[2] = candidate[0];   /* +8  */
    target[4] = candidate[2];   /* +16 */
    target[9] = zero;           /* +36 */
    target[11] = zero;          /* +44 */

    actor[14] = 0x80000000;     /* +56 */
    actor[16] = 0x80000000;     /* +64 */
    actor[9] = zero;
    actor[11] = zero;
    actor[2] = *(s16 *)((u8 *)actor + 10) << 16;
    actor[4] = *(s16 *)((u8 *)actor + 18) << 16;

    Func_08009080(actor, 1);
}
