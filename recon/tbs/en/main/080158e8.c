/* 080158e8: ARM draft (Tile_BuildMetatiles); ⚓️'s copy is 080388fc.
   Game ARM route (K1): stock agbcc_arm at the library ARM setting,
   alchemy diff --arm: 92/92/common92/differ 0 (2026-10-07, session 1 of
   S02) with two tagged order devices, each a non-volatile empty asm that
   rewrites a live register in place. Cause: stock agbcc_arm's old sched.c
   costs an anti-dependence at its producer's latency (a store 2), where
   native placement needs it at 0 or 1. An asm consumer makes the link
   free (insn_cost sets LINK_COST_FREE when the user is unrecognized):
   one after the last store takes the jump's anti-dependence on dst
   (differ 6 -> 4: add sl, mov r1 and cmp rise one store); one after the
   seventh store that rewrites dst and v takes f+d's anti-dependence on
   the stores of v, so strh 7 sits right above f+d (4 -> 0), in sched1
   and again in sched2. Either alone: 4 and 6. Japanese (not scored by
   diff: an ARM scaffold) lacks the 0xf01a and 0xf01b cases.
   ⚓️ 080388fc --game tla --arm: 93/92/common92/differ 1, the one being
   its Thumb entry (bx pc; .2byte 0x0200), handwritten assembly (C2).
   Adoption, not yet done: ☀️ CONSTANTS.LD sizes the code by
   Tile_BuildMetatilesEnd; ja/de/es/fr/it rom.s hold it in one section
   with UiText_DrawGlyphArm; GRAPHICS.H declares it const u8 [] for
   STATUS.C's copy; ⚓️ names the Thumb entry Tile_BuildMetatiles and
   its even address Tile_BuildMetatilesCode (08042588.s).
   Earlier, at differ 6: instruction order; S2 allows a tagged device
   once reshaping fails.
   History: differ 0 under agbcc_arm with 2.96's arm_adjust_cost copied
   in; adopted 2026-10-06 and again 2026-10-07 in both games under that
   copy, and returned here when it was dropped as a compiler change fitted
   to this one function (K3). agscc -marm with the game options: differ 19;
   with -mno-apcs-frame 19; with -mno-apcs-frame -ffixed-fp 19.
   Native: 92 instructions and 41 switch-table words, 532 bytes total.
   Current: stock agscc --arm 92/91/common82/differ19 (was differ72);
   fixed agbcc_arm library setting (-O2 -mthumb-interwork
   -fomit-frame-pointer -fcall-used-r4) 92/92/common89/differ6 (was 10).
   The uniform -O control gave 92/96/common39/differ110.
   Remaining agbcc_arm difference: add sl, mov r1 and cmp each sit one
   store lower than native. sched1 decides it (-fno-schedule-insns2 is
   unchanged): its backward list scheduler places cmp right above bls
   because the jump's anti-dependence on the last post-increment store
   costs 2, so the fillers start one stall slot too low. Native has
   strh directly above bls and f+d directly below the store before it,
   as if a store cost 1 there; no reshaping tried moves cmp off T-2.
   Trials: one packed-value local v set three times, each value computed
   before its dst++ (not a birthing insn, so f+d no longer launches next
   to the last store) 10->6; for-loop row counter instead of an explicit
   next = row + 1 gives the same r9 and 6 (do/while with next: --arm 18);
   while (++row < 20) 6; src += 2 after the last store 6, before the
   first 6; u32 v 6; a row pointer p with dst = p at the end 6;
   for (col = 0; ...) without the zero chain 8; earlier:
   moving packed-value calculation or row update alone 10; u16 packed
   value 14; indexed stores worsen register allocation.
   No compiler route follows from these scores. */
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

void Tile_BuildMetatiles(u16 *dst, u16 *src)
{
    u32 row;
    u32 col;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
    s32 found;
    s32 v;

    for (row = 0; row < 20; row++) {
        d = 0;
        a = d;
        c = d;
        f = d;
        b = f;
        e = f;
        found = f;
        col = f;
        dst++;
        for (; col < 30; col++) {
            u16 cmd = *src++;

            if (cmd == 0)
                continue;
            if (found == 0) {
                switch (cmd) {
                case 0xf010:
                    a = col * 8;
                    b = a + 8;
                    c = a + 1;
                    d = a;
                    e = d + 7;
                    f = b;
                    found++;
                    break;
                case 0xf013:
                    a = col * 8;
                    b = a + 8;
                    c = a;
                    d = a + 1;
                    e = b;
                    f = a + 7;
                    found++;
                    break;
                case 0xf007:
                case 0xf016:
                case 0xf01a:
                    a = col * 8;
                    b = a + 8;
                    d = a;
                    c = d;
                    f = b;
                    e = f;
                    found++;
                }
            } else {
                switch (cmd) {
                case 0xf012:
                    b = col * 8 + 8;
                    e = col * 8 + 7;
                    f = b;
                    break;
                case 0xf015:
                    b = col * 8 + 8;
                    e = b;
                    f = col * 8 + 7;
                    break;
                case 0xf007:
                case 0xf017:
                case 0xf01b:
                    f = col * 8 + 8;
                    b = f;
                    e = f;
                    break;
                }
            }
        }
        v = e + (c << 8);
        *dst++ = v;
        v = b + (a << 8);
        dst++;
        *dst++ = v;
        dst++;
        *dst++ = v;
        dst++;
        *dst++ = v;
        dst++;
        src += 2;
        *dst++ = v;
        dst++;
        *dst++ = v;
        dst++;
        *dst++ = v;
        /* FAKEMATCH: rewrites dst and v in place so f+d no longer waits two cycles on the stores of v; strh 7 then sits right above f+d (differ 4 -> 0) */
        asm("" : "=r"(dst), "=r"(v) : "0"(dst), "1"(v));
        v = f + (d << 8);
        dst++;
        *dst++ = v;
        /* FAKEMATCH: rewrites dst in place so the loop jump no longer waits two cycles on the last store; add sl, mov r1 and cmp rise one store (differ 6 -> 4) */
        asm("" : "=r"(dst) : "0"(dst));
    }
}
