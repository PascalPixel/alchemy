#include "types.h"

#define SceneDialogue_RunActorTenFlag881Dialogue Func_020002b8

/*
 * resource_399 scripted step at 0x020002b8, 204 bytes.
 *
 * Complete owner: `push {lr}` at 0x020002b8 and the interworking return
 * `pop {r0} / bx r0` at 0x02000372.  The popped register is r0, so it holds
 * the return address and nothing is returned — the owner is `void`.  Bytes
 * 0x02000376-0x02000383 are alignment plus the three-word literal pool.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_399
 * 02b8 --annotate`; an overlay `bl` stores `target - 2`, so the disassembler's
 * own annotations are wrong.  23 sites, 11 distinct imports.  Per-target
 * multiset against the assembly, which is what the C below spells:
 *   Func_0808a018 x1, GameFlag_IsSet x1, Func_0808a170 x2, Func_0808a180 x3,
 *   Func_0808a1f0 x2, Func_0808a010 x4, Func_0808a100 x4, Func_0808a148 x2,
 *   Func_0808a190 x1, Func_0808a1b8 x2, Func_0808a020 x1.
 * (The inventory's calls=20 counts distinct-ish targets and predates the
 * corrected `bl` decoding; it is a lower bound, not a check.)
 *
 * Shape: one scripted section bracketed by Func_0808a018/Func_0808a020, with
 * the story-flag query GameFlag_IsSet(0x881) choosing between two arms.  The
 * two arms are the SAME sequence on channel 10 and differ in exactly two
 * places — the message id (0x163c against 0x152d) and one step call, where the
 * flag-set arm uses Func_0808a190(10, 0) and the flag-clear arm uses
 * Func_0808a180(10, 0).  They are written out separately rather than merged so
 * the call multiset stays exact; the near-identity is itself the check that
 * neither arm was mis-transcribed.
 *
 * Constants built by shifts: `movs r1,#129 ; lsls #1` is 258 (a pose id), and
 * `movs r1,#192 ; lsls #6` is 0x3000 — three sixteenths of a turn in the
 * 16-bit angle unit Func_0808a1b8 takes.
 *
 * Func_0808a190 returns a status value: other independently reconstructed call
 * sites test that value.  It is ignored here, but retaining the truthful s32
 * declaration is load-bearing because the return-register dataflow determines
 * the reference's r1-before-r0 argument order at this call.
 *
 * The typed inline wrappers around the two angle calls are also load-bearing.
 * Their parameters preserve the reference's `movs r1,#192 / movs r0,#10 /
 * lsls r1,#6' materialisation without changing either call's behavior.
 */

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Most remain old-style because arity varies. */
void Func_020024ec();
s32 Func_020024d2();
void Func_020025cc();
void Func_020025dc();
void Func_02002616();
void Func_0200250c();
void Func_020025a4();
void Func_0200251a();
s32 Func_0200261c();
void Func_02002630();
void Func_020025d0();
void Func_02002620();
void Func_0200266a();
void Func_02002560();
void Func_020025f8();
void Func_0200256e();
void Func_02002660();
void Func_02002684();
void Func_02002624();
void Func_020025a8();
                                /* frame wait */
                                /* scripted-section open */
                                /* scripted-section close */

                                /* scene request */
                                /* message line */

                                /* orientation over frames */

                                /* story-flag query */

static __inline__ void Call_02002630(s32 channel, s32 angle, s32 frames)
{
    Func_02002630(channel, angle, frames);
}

static __inline__ void Call_02002684(s32 channel, s32 angle, s32 frames)
{
    Func_02002684(channel, angle, frames);
}

void SceneDialogue_RunActorTenFlag881Dialogue(void)
{
    Func_020024ec();

    if (Func_020024d2(0x881) != 0) {
        Func_020025cc(0x163c);
        Func_020025dc(10, 0);
        Func_02002616(10, 258);
        Func_0200250c(40);
        Func_020025a4(10, 1);
        Func_0200251a(20);
        Func_020025dc(10, 0, 20);
        Func_0200261c(10, 0);
        Call_02002630(10, 0x3000, 10);
        Func_020025d0(10, 9);
    } else {
        Func_02002620(0x152d);
        Func_02002630(10, 0);
        Func_0200266a(10, 258);
        Func_02002560(40);
        Func_020025f8(10, 1);
        Func_0200256e(20);
        Func_02002630(10, 0, 20);
        Func_02002660(10, 0);
        Call_02002684(10, 0x3000, 10);
        Func_02002624(10, 9);
    }

    Func_020025a8();
}
