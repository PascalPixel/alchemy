#include "types.h"

/* resource_3bc state-machine slot 6, complete 248-byte owner and pool.
 *
 * Every helper here is reached through a per-call-site veneer, including
 * repeat calls to the "same" helper (Func_0808a010/Func_0808a180/
 * Func_0808a170 each recur with a DIFFERENT veneer address per occurrence)
 * and even the sibling-overlay call at 0x20024e4 (Func_0200288c is itself
 * reached via a veneer, not directly). Symbol names below are the literal
 * per-site call targets read from the raw (non-annotated) disassembly, per
 * the "call symbols are per-site" rule -- do not collapse repeats to one
 * shared declaration.
 */

extern s16 Data_02000240[];

extern void Func_02004d72(void);       /* site 0x20024e4 -> Func_0200288c veneer */
extern void Func_02006eec(void);       /* site 0x20024ea -> Func_0808a018 veneer */
extern s32 Func_02004f88(s32 actor, s32 slot); /* site 0x20024f2 -> Func_02002a94 veneer */
extern void Func_02006fb0(s32 message);  /* site 0x20024fe -> Func_0808a170 veneer (state==0 arm) */
extern void Func_02007004(s32 x, s32 z); /* site 0x200250a -> Func_0808a208 veneer */
extern void Func_0200701e(s32 x, s32 y, s32 z, s32 mode); /* site 0x200251c -> Func_0808a210 veneer */
extern void Func_0200702a(void);         /* site 0x2002520 -> Func_0808a218 veneer */
extern void Func_02006f20(s32 frames);   /* site 0x2002526 -> Func_0808a010 veneer #1 */
extern void Func_02006ff0(s32 actor, s32 mode); /* site 0x200252e -> Func_0808a180 veneer #1 */
extern void Func_02005f92(s32 x, s32 z, s32 mode); /* site 0x2002538 -> Func_02003a58 veneer */
extern void Func_02006f38(s32 frames);   /* site 0x200253e -> Func_0808a010 veneer #2 */
extern void Func_02007008(s32 actor, s32 mode); /* site 0x2002546 -> Func_0808a180 veneer #2 */
extern void Func_0200600e(s32 x, s32 z, s32 mode); /* site 0x2002550 -> Func_02003abc veneer #1 */
extern void Func_02006f50(s32 frames);   /* site 0x2002556 -> Func_0808a010 veneer #3 */
extern void Func_02007020(s32 actor, s32 mode); /* site 0x200255e -> Func_0808a180 veneer #3 */
extern void Func_02006026(s32 x, s32 z, s32 mode); /* site 0x2002568 -> Func_02003abc veneer #2 */
extern void Func_02006f68(s32 frames);   /* site 0x200256e -> Func_0808a010 veneer #4 */
extern void Func_02007038(s32 actor, s32 mode); /* site 0x2002576 -> Func_0808a180 veneer #4 */
extern void Func_02006094(void);         /* site 0x200257a -> Func_02003b18 veneer */
extern void Func_02006f7a(s32 frames);   /* site 0x2002580 -> Func_0808a010 veneer #5 */
extern void Func_0200707a(s32 actor, s32 mode); /* site 0x2002588 -> Func_0808a200 veneer */
extern void Func_020050e2(s32 actor, s32 slot); /* site 0x2002590 -> Func_02002b50 veneer */
extern void Func_0200704e(s32 message);  /* site 0x200259c -> Func_0808a170 veneer (state==1 arm) */
extern void Func_02007066(s32 actor, s32 mode); /* site 0x20025a4 -> Func_0808a180 veneer #5 */
extern s32 Func_0200515c(s32 state, s32 actor, s32 slot); /* site 0x20025ae -> Func_02002bac veneer */
extern void Func_02006fbc(void);         /* site 0x20025b2 -> Func_0808a020 veneer */

void Func_020024d0(s32 actor)
{
    s32 state;

    if (Data_02000240[225] == 2) {
        Func_02004d72();
        return;
    }

    Func_02006eec();
    state = Func_02004f88(actor, 6);

    if (state == 0) {
        Func_02006fb0(0x20c7);
        Func_02007004(0x30000, 0x6000);
        Func_0200701e(0x5080000, -1, 0x980000, 1);
        Func_0200702a();
        Func_02006f20(30);
        Func_02006ff0(actor, 0);
        Func_02005f92(0xb4, 0x58, 0);
        Func_02006f38(60);
        Func_02007008(actor, 0);
        Func_0200600e(0x20, 0x54, 10);
        Func_02006f50(30);
        Func_02007020(actor, 0);
        Func_02006026(0x60, 0x54, 30);
        Func_02006f68(60);
        Func_02007038(actor, 0);
        Func_02006094();
        Func_02006f7a(2);
        Func_0200707a(0, 0);
        Func_020050e2(actor, 6);
    } else if (state == 1) {
        Func_0200704e(0x20c6);
        Func_02007066(actor, 0);
    }

    Func_0200515c(state, actor, 6);
    Func_02006fbc();
}
