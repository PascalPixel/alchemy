/* 2026-09-27 continuation: staging both options and callback pointers before
 * the flag clear keeps 284 bytes but gives 29 edits, with the pointer still
 * published after the clear. A one-record options array and a typed motion
 * bitfield each compile to the canonical 11-edit result. The opening OR and
 * post-wait publication remain independent unresolved source lifetimes. */
/* 2026-09-27 babi-land final audit: whole 284-byte extent and five pool
 * words rescored: 11 differing halfwords / 11 normalized edits. Exact
 * KUUPUAPPU_MURA/DRIFT.C publishes its callback to a constructor-owned leaf
 * actor while retaining a separate source actor and sprite. Here the
 * callback belongs to the stack EffectOptions consumed by Effect_Spawn;
 * the leader and its flag address remain live. Those records and the exact
 * MAP_INIT.C callback agree with FIELD_EFFECT.H. DRIFT supplies no shared
 * halfword zero producer here; do not transfer volatile zeros or reopen
 * the recorded callback/publication axes. DONE +0, alignment +0.
 * Canonical 284/11 restored after publication P1 (74e203d9a) and P2
 * (1646e59e6): neither admits early options plus saved-to-store copy and
 * reference zero lifetime together. Stop this boundary axis; SetFlagBits
 * remains proven for the opening OR only, not a remedy for this hunk.
 * Publication P2 result: 284/284 bytes, 90 halfwords / 29 aligned edits.
 * Options reaches r8 before the clear, but via r2, which also remains the
 * callback-store base: the required saved-to-r0 copy disappears. Body is
 * two bytes short with a new trailing pad; all five pool words agree.
 * Zero46 is still QI / 2 uses / 4 insns; options34 remains 4 uses / 59 insns /
 * 7 calls. No zero-lifetime admission. Full diff read; -da equals ordinary.
 * Preserve both negative models in Git, then restore 284/11 baseline.
 * Publication P2: P1's boundary preserves the address copy, but the copy
 * follows the clear. Publish o before the isolated clear, so its saved
 * lifetime crosses that region; require r8 publication before the clear.
 * Publication P1 result: 284/284 bytes, 13 halfwords / 13 aligned edits.
 * Separate add sp16 -> r8 -> store-address copy now survives before the
 * callback store, but after the flag clear; early-copy admission not met.
 * Options pseudo34 shrinks 59 to 57 insns (still 4 uses / 7 calls); zero46
 * stays QI / 2 uses / 4 insns. Two early call setups regress. Complete loop,
 * frame56 and all five pool words remain exact. -da equals normal assembly.
 * Publication P1: Suhalla's one-pass byte publication preserved a separate
 * temporary-to-saved pointer copy. Baseline options pseudo34 is already a
 * USER local but reload forms its address after the QI clear, copying late.
 * Test clear boundary before assigning o; admission is early options r8
 * publication with frame56, complete loop body and five pools unchanged.
 * NONMATCHING: 284 of 284 bytes, 11 halfword edits (2026-09-24). Hand-written
 * from the resolved disassembly as a single-overlay unit binding Engine_* at
 * their import veneers plus advance_effect_motion = 0x02009068 (thumb, the
 * effect update in MAP_INIT.C). Matched: the spray velocities (vz multiplies
 * by -0x1999 so it is synthesised by shifts; m - 5 is its own temporary so
 * the 0x3332 product is not distributed). Remaining: the motion-flag OR loads
 * the byte into r3 and the 2 into r2 (the reference swaps them), and the
 * block after the six-frame wait: the reference takes &options into r8 first
 * and stores the zero flag byte from r5 after loading the update address;
 * this draft stores the zero from r3 first.
 *
 * 2026-09-27: registered as babi-chika-leader-spray. FIELD_EFFECT.H supplies
 * the same options and actor layouts as BABI_IRIGUCHI/BRANCHING_EVENT.C;
 * the callback is the exact BABI_CHIKA/MAP_INIT.C motion updater.
 * Canonical -da dumps show the flag-clear zero is a block-local QImode
 * pseudo (46), while options (34) spans seven calls. Reusing one signed
 * local for that zero and later vx keeps a saved register but rotates the
 * leader/counter/velocity registers: 41 halfwords; also using it for the
 * earlier OR gives 39. Moving options.update before the clear leaves the
 * same size but 23 aligned edits and a two-byte-shorter instruction body.
 * Makyuri's inline SetFlagBits fixes the opening OR exactly, but changes
 * reload registers later: 18 halfwords, 17 edits. All four models are
 * rejected; this keeps the 11-halfword baseline. A next attempt needs an
 * independently supported lifetime for the zero/options pair, not another
 * declaration permutation or store-order sweep.
 * The dump's callback load was also tested with an explicit update pointer
 * assigned before the flag clear, then stored to options afterwards. This
 * does move the callback load first, but does not retain options early or
 * put zero in r5: 284 bytes, 82 halfwords, 29 aligned edits, with a new
 * trailing instruction-alignment halfword. Reject that lifetime model too;
 * the 11-halfword baseline below remains unchanged.
 *
 * 2026-09-27 Mercury transfer audit: freshly scored the full owner and pool
 * at 284 bytes / 11 differing halfwords / 11 aligned edits. No new model
 * admitted. LAMP.C's plain Half zero shares an earlier HI angle-store zero;
 * this owner has only the QI flag-clear producer (CSE pseudo 46, four-insn
 * lifetime), with no earlier HI store. Its constructor and spin-zero store
 * belong to the separately called COMMON/EFFECT/SPAWN.C, not this RTL body.
 * Flags 0x1000001 select only options.update, so initializing a halfword
 * option would add an unsupported store. The exact MAP_INIT.C callback and
 * BABI_IRIGUCHI/BRANCHING_EVENT.C consumer confirm the existing layout.
 * A phased flags/options pointer is also unsupported: both remain live
 * through the loop, and flags is reused after the final sprite-flags call.
 * Keep the baseline; do not repeat signed zero/vx reuse, callback staging,
 * or store-order models without a new producer/consumer witness. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"

s32 Engine_MathModulo(s32 value, s32 divisor);
void Main_0808a118(s32 mode);
void advance_effect_motion(union FieldObject *object);

void BabiChika_RunLeaderSpray(void)
{
    struct FieldActor *leader = Object_GetById(0);
    u8 *flags;
    struct EffectOptions options;
    struct EffectOptions *o;
    u32 i;

    Engine_EventBegin();
    Object_SetMode(leader, 6);
    Main_0808a118(0);
    Object_SetMode(leader, 1);
    Engine_ActorSetSpriteFlags(leader, 0);
    flags = &leader->motion_flags;
    *flags |= 2;
    Engine_AudioPlayCue(152);
    leader->velocity_y = 0x40000;
    Engine_ObjectSetPosition(leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + 0xc0000);
    Engine_TaskWait(6);
    o = &options;
    *flags = 0;
    o->update = advance_effect_motion;
    Engine_AudioPlayCue(127);
    for (i = 0; i < 8; i++) {
        leader->y.fixed -= 0x20000;
        leader->target_y = leader->y.fixed;
        Engine_TaskWait(1);
        if (i & 1) {
            s32 m = Engine_MathModulo(Engine_RandomNext(), 10) - 5;
            s32 vx = m * 0x3332;
            s32 vz;

            vz = Engine_MathModulo(Engine_RandomNext(), 10) * -0x1999 - 0x7ffd;

            Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, vx, 0, vz, 0x1000001, o);
        }
    }
    Engine_ActorSetSpriteFlags(leader, 1);
    *flags = 3;
    Engine_EventEnd();
}
