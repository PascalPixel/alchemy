/* 080158e8: ARM draft (Tile_BuildMetatiles) for the game ARM route (K1,
   agbcc/camelot_arm). Route decision (2026-10-07), this draft under agscc
   -marm with the game options: differ 19; with -mno-apcs-frame 19; with
   -mno-apcs-frame -ffixed-fp 19.
   differ 0 through the game ARM route: alchemy diff --camelot-arm,
   pinned gcc_arm with agscc's arm_adjust_cost at the library ARM
   setting, 92/92/common92/differ0. Not credited until adopted; the
   listing stays in recon/tbs/raw/080158e8.S until then. Once adopted 2026-10-06 (1f085d789f) under
   the anti/output half of the rules alone; reverted with that route.
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
        v = f + (d << 8);
        dst++;
        *dst++ = v;
    }
}
