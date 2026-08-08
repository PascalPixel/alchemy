#include "types.h"

/*
 * Resource 3b5, cutscene script step at 0x020003d0 (224 bytes; 196 bytes of
 * code plus two interior literal pools at 0x02000416-0x02000423 and
 * 0x02000496-0x020004af), the exact twin of Func_020002f0.
 *
 * Twin evidence, not inference: a byte diff of the two 224-byte rows
 * (`overlay_show.ts` output with the address column stripped) shows the code
 * differs in exactly ONE place, `movs r0, #16` -> `movs r0, #17`, and the
 * literal pools differ only in the six text ids, each exactly one higher.
 * Two further diff hunks (`subs r5, r2, #6` -> `subs r6, ...` and
 * `subs r2, r5, #6` -> `subs r3, ...`) sit inside the pool ranges and are the
 * disassembly of changed pool words, not instructions.  `overlay_twins.ts`
 * does not report this pair; sorting the overlay's rows by (span, calls) and
 * eyeballing the equal 224/12 sizes found it in seconds.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8 save at
 * 0x020003d0-0x020003d4, and the matching interworking return at
 * 0x0200048c-0x02000494.  r0 holds the popped return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds 0x020083d1
 * in a record's handler word — Func_020003d0 + the Thumb bit under this
 * overlay's proven 0x02008000 link base.  The table pairs it with 0x020082f1
 * (Func_020002f0) at six separate places: the two are the partner halves of
 * one exchange, actor 16 speaking and actor 17 replying.
 *
 * IMPORTANT: as in the twin, the six selection arms all fall into ONE
 * `bl Func_0808a170` at 0x02000456.  The line id is hoisted into a local so
 * the import is called exactly once and the per-target multiset stays honest.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; twelve sites,
 * matching the inventory's `calls=12`.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
u8 *Func_0200115a();
void Func_0200114a();
s32 Func_02001150();
s32 Func_0200116c();
s32 Func_02001180();
s32 Func_0200118e();
void Func_0200124c();
void Func_0200121c();
void Func_0200124e();
void Func_02001280();
void Func_0200117a();
void Func_020011ee();

                    

                     

                     

void Func_020003d0(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    u8 *actor;
    u16 *flags;
    s16 heading;
    s32 line;

    actor = Func_0200115a(17);
    heading = *(s16 *)(actor + 6);
    flags = (u16 *)(actor + 100);

    Func_0200114a();

    *flags |= 2;

    if (*(s16 *)(workspace + 382) == 0) {
        if (Func_02001150(0x950) != 0) {
            line = 0x2366;
        } else if (Func_0200116c(0x962) != 0) {
            line = 0x21e3;
        } else {
            line = 0x1f96;
        }
    } else {
        if (Func_02001180(0x950) != 0) {
            line = 0x2372;
        } else if (Func_0200118e(0x962) != 0) {
            line = 0x21f6;
        } else {
            line = 0x1fab;
        }
    }

    Func_0200124c(line);

    Func_0200121c(17, 0);
    Func_0200124e(17, 0, 2);
    Func_02001280(17, 0, 10);

    *(u16 *)(actor + 6) = (u16)heading;

    Func_0200117a(1);

    *flags &= 1;

    Func_020011ee();
}
