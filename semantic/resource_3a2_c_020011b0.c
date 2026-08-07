#include "types.h"

/*
 * resource_3a2 owner at 0x020011b0, 744 bytes.  This is the overlay's ROOT:
 * image offset 0 is an exported-entry veneer table (`ldr r4,[pc,#0] / bx r4 /
 * .word <entry>`) and its FIRST entry's word is 0x020091b1, which under the
 * proven 0x02008000 link base is file offset 0x11b0 plus the Thumb bit.
 *
 * LINK BASE PROOF (0x02008000; in-image address = pool_word - 0x8000).  The six
 * words of the offset-0 export table are 0x020091b1, 0x0200807d, 0x020080b1,
 * 0x020080b9, 0x02009181 and 0x020080ad -- every one ODD, i.e. a Thumb entry,
 * and every one resolving to a known function start in this image: 0x11b0
 * (this owner), 0x007c, 0x00b0, 0x00b8, 0x1180 and 0x00ac.  Four of those six
 * are already-tracked byte-exact siblings (assets/code/resource_3a2_c_0200007c.c,
 * _020000b8.c, _02001180.c and the leaf at 0x00ac's neighbour _02000054.c), so
 * the base is proven against material that already reproduces the ROM.  The
 * overlay's remaining `0x0200[89ab]xxx` pool words agree without exception:
 * every odd one (0x02008031, 0x02008055, 0x020080e9, 0x02008141, 0x02008161,
 * 0x02008181, 0x0200884d, 0x02008871, 0x020088a9, 0x020088e1, 0x02008925,
 * 0x02008ac1, 0x02008c31, 0x02008fcd, 0x02008ff1, 0x02009029, 0x02009091,
 * 0x020090b9, 0x02009145) lands exactly on a function start, and every even one
 * (0x02009620, 0x0200962c, 0x020096b8, 0x02009730, 0x02009788) lands past the
 * import band's end at 0x1620 and is therefore in-image data.
 *
 * Boundary.  Prologue `push {r5, r6, lr}` / `sub sp, #8` at 0x020011b0; the
 * single interworking return is `add sp,#8 / pop {r5,r6} / pop {r1} / bx r1` at
 * 0x0200145a.  Per the epilogue rule, `pop {rN} ; bx rN` with N != 0 leaves r0
 * live, so r0 IS the result -- and the instruction before the unwind is
 * `movs r0,#0`, so this owner returns the constant 0 as s32.  A control-flow
 * walk from the prologue reaches no halfword at or after 0x02001460, so
 * 0x02001460-0x02001497 is literal pool, filling the row's span exactly
 * (0x020011b0 + 744 = 0x02001498, the first veneer of the import band):
 *   0x02001460 = 0x03001ebc  IWRAM workspace-POINTER cell
 *   0x02001464 = 0x02000240  cross-overlay RAM global block (NOT in-image: it
 *                            is below the 0x02008000 link band)
 *   0x02001468 = 0x00000058  scene id
 *   0x0200146c = 0x000008b2  progress-flag id
 *   0x02001470 = 0x0000012f  cue id
 *   0x02001474 = 0x00000109  progress-flag id
 *   0x02001478 = 0x0000004a  scene id (the same one the byte-exact sibling
 *                            assets/code/resource_3a2_c_02001180.c tests)
 *   0x0200147c = 0x00000201  progress-flag id
 *   0x02001480 = 0x0000089a  progress-flag id
 *   0x02001484 = 0x00000895  progress-flag id
 *   0x02001488 = 0x000008b3  progress-flag id
 *   0x0200148c = 0x02009730  in-image data address (even -> data), = 0x1730
 *   0x02001490 = 0xffff0000  record field value
 *   0x02001494 = 0x000008b1  progress-flag id
 *
 * Call sites resolved with `bun tools/lib/overlay_call_targets.ts resource_3a2 11b0`
 * (the printed annotations of overlay_show are NOT used: an overlay `bl` stores
 * target_offset - 2).  56 sites, 17 distinct targets, of which 5 are this
 * overlay's own prologues and 12 are import veneers:
 *   0x14b0 -> Func_08009180   0x1508 -> Func_0808a080
 *   0x14b8 -> Func_080091c0   0x1518 -> Func_0808a098
 *   0x14c0 -> Func_080091e0   0x1550 -> Func_0808a0f0
 *   0x14d0 -> Func_080770c0   0x1558 -> Func_0808a100
 *   0x14d8 -> Func_080770c8   0x15a8 -> Func_0808a1b8
 *   0x14e0 -> Func_080770d0   0x1600 -> Func_0808a5e0
 *   own prologues: 0x020001ec, 0x02000870, 0x02000b2c, 0x020010b8, 0x02001144
 *
 * PER-TARGET MULTISET, placed against the tool's per-target site counts
 * (56 = 56; the inventory's `calls=52` is a known FLOOR, not an equality test):
 *   Func_080770c0 x16 = 16   Func_0808a0f0 x10 = 10
 *   Func_080770c0 sites 0x1204 0x1242 0x127a 0x1290 0x12c0 0x12ca 0x12d4 0x12e8
 *                       0x12f2 0x132c 0x1380 0x138a 0x13fc 0x1436 0x1440 0x144a
 *   Func_0808a100 x5 = 5     Func_0808a080 x4 = 4     Func_080091c0 x3 = 3
 *   Func_0808a1b8 x3 = 3     Func_080770c8 x2 = 2     Func_080770d0 x2 = 2
 *   Func_08009180 x2 = 2     Func_0808a098 x2 = 2     Func_080091e0 x1 = 1
 *   Func_0808a5e0 x1 = 1     Func_020001ec x1 = 1     Func_02000870 x1 = 1
 *   Func_02000b2c x1 = 1     Func_020010b8 x1 = 1     Func_02001144 x1 = 1
 * Every early exit in this owner is a plain `b.n` to the shared `movs r0,#0`
 * unwind and carries no call, so `return 0;` spells them without inflating or
 * deflating any per-target count.
 *
 * Behaviour: the overlay's per-entry scene dispatcher.  It first publishes 256
 * into the workspace word at +448 (the same slot Func_02000180 writes 32 into),
 * then switches on the current room id, the signed halfword Data_02000240[224]
 * -- the fixed cross-overlay shape, and the very field the byte-exact sibling
 * Func_02001180 branches on against this owner's own pooled 0x4a.
 *   room 0x58: set up four actor poses and a placement, run the shared setup
 *     Func_02000870, optionally reposition actor 13, then pick a cutscene off
 *     the sub-state Data_02000240[225] -- 2 posts cue 0x12f, 3 runs the long
 *     scene Func_020001ec unless flag 0x109 is already set.
 *   room 0x4a: unlock actor 14's record (bit 1 of its byte at +35), then run
 *     the flag-gated scene fragments Func_020010b8 (flag 0x200) and
 *     Func_02001144 (flag 0x201), post cue 0x12f while the sub-state is 4 or 5,
 *     restore actor placements and headings for whichever of flags 0x89a/0x895/
 *     0x8b2 are set, and finally run Func_02000b2c when the sub-state is 5 and
 *     none of 0x8b1/0x109/0x8b2 has fired.
 *   any other room: nothing.
 *
 * The sub-state window test at 0x020012a4 is read from the assembly rather than
 * simplified: the halfword is loaded UNSIGNED, 4 is subtracted, the result is
 * shifted left 16 and compared unsigned against 0x00010000, so it selects
 * exactly the two values 4 and 5 and is spelled that way.
 *
 * Uncertainties: Func_08009180 and Func_080091c0 take six arguments, four in
 * registers and two through the 8-byte outgoing area this owner reserves; only
 * the six values actually stored are asserted, not their meaning.  The record
 * word written at +24 is the pooled 0xffff0000 and its field is not named.
 * Func_0808a080's return is used as a record pointer here exactly as the
 * already-admitted semantic/overlays/resource_3a2_c_02000180.c uses it.
 * Old-style declarations keep every import's interface open, as overlay sources
 * require -- two `Func_` names can be one import and arities vary per site.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;
extern u8 Data_02009730[];

void Func_020027d0();
void Func_02002730();
void Func_02002738();
void Func_02002740();
void Func_020026b4();
void Func_02001a70(void);
s32 Func_020026d6();
void Func_02002768();
void Func_020027ca();
void Func_02002716();
s32 Func_02002714();
void Func_0200143a(void);
u8 *Func_02002766();
void Func_02002724();
u8 *Func_02002772();
s32 Func_0200274c();
void Func_020027e0();
void Func_02002344(void);
s32 Func_02002762();
void Func_020027f6();
void Func_020023e6(void);
void Func_0200279c();
s32 Func_02002792();
s32 Func_0200279c_b();
s32 Func_020027a6();
void Func_02002834();
s32 Func_020027ba();
s32 Func_020027c4();
void Func_02002862();
void Func_020027f0();
void Func_02002878();
s32 Func_020027fe();
void Func_02002812();
void Func_0200281a();
s32 Func_02002852();
s32 Func_0200285c();
void Func_020028ea();
void Func_020028f4();
void Func_02002902();
void Func_02002910();
void Func_0200291e();
void Func_02002982();
void Func_0200298e();
void Func_02002906();
u8 *Func_020028fc();
s32 Func_020028ce();
void Func_02002960();
void Func_02002930();
u8 *Func_02002926();
s32 Func_02002908();
s32 Func_02002912();
s32 Func_0200291c();
void Func_02001f80(void);

                     

                     

                     

                     

                         

                         

s32 Func_020011b0(void)
{
    u8 *workspace;
    s16 room;

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 256;

    room = Data_02000240[224];

    if (room == 0x58) {
        Func_020027d0(169);
        Func_02002730(11, 5);
        Func_02002738(12, 5);
        Func_02002740(14, 2);
        Func_020026b4(21, 9, 1, 1, 21, 73);

        Func_02001a70();

        if (Func_020026d6(0x8b2)) {
            Func_02002768(13, 136 << 16, 128 << 17);
            Func_020027ca(13, 0, 0);
        }

        if (Data_02000240[225] == 2) {
            Func_02002716(0x12f);
            return 0;
        }
        if (Data_02000240[225] != 3) return 0;
        if (Func_02002714(0x109)) return 0;

        Func_0200143a();
        return 0;
    }

    if (room != 0x4a) return 0;

    Func_02002724(Func_02002766(14), 0);
    Func_02002772(14)[35] |= 2;

    if (Func_0200274c(0x200)) {
        Func_020027e0(14, 5);
        Func_02002344();
    }

    if (Func_02002762(0x201)) {
        Func_020027f6(15, 4);
        Func_020023e6();
    }

    if ((u16)(Data_02000240[225] - 4) <= 1) Func_0200279c(0x12f);

    if (!Func_02002792(0x89a) && !Func_0200279c_b(0x895) &&
        !Func_020027a6(0x8b2)) {
        Func_02002834(10, 0, 0);
    }

    if (!Func_020027ba(0x8b2) && Func_020027c4(0x895) &&
        Data_02000240[225] == 2) {
        Func_02002862(11, 0, 0);
        Func_020027f0(0x8b2);
        Func_020027f6(0x8b3);
        Func_02002878(10, 0, 0);
    }

    if (Func_020027fe(0x8b2)) {
        Func_020027f6(54, 21, 53, 21, 1, 2);
        Func_02002812(18, 20, 1, 3, 17, 21);
        Func_0200281a(44, 18, 43, 17, 1, 1);
        Func_02002834(8, 17, 1, 1, 7, 17);
    }

    if (Func_02002852(0x895) && !Func_0200285c(0x8b2)) {
        Func_020028ea(12, 0, 0);
        Func_020028f4(13, 0, 0);
        Func_02002902(8, 192 << 16, 132 << 17);
        Func_02002910(9, 164 << 16, 140 << 17);
        Func_0200291e(10, 184 << 16, 152 << 17);
        Func_02002982(8, 160 << 7, 0);
        Func_0200298e(10, 176 << 8, 0);
        Func_02002906(9, Data_02009730);
        *(s32 *)(Func_020028fc(9) + 24) = (s32)0xffff0000;
    }

    if (!Func_020028ce(0x8b2)) {
        Func_02002960(9, 164 << 16, 140 << 17);
        Func_02002930(9, Data_02009730);
        *(s32 *)(Func_02002926(9) + 24) = (s32)0xffff0000;
    }

    if (Data_02000240[225] == 5 && !Func_02002908(0x8b1) &&
        !Func_02002912(0x109) && !Func_0200291c(0x8b2)) {
        Func_02001f80();
    }

    return 0;
}
