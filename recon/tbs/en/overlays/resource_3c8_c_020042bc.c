/* Astra 2026-09-27 scalar phase transfer: after the first wave, assign
 * repeat = (u32)opts and use that scalar's pointer view for the second
 * scale/spin writer. Unlike Takara PROBE.C's memory-produced coordinate,
 * this pointer copy disappears: complete candidate is byte-identical to
 * P6 (cmp), 612/612, 116 halfwords / 30 aligned edits. Count/options still
 * occupy r9/sl and the second write-view copy is absent. Full normalized
 * diff rejects admission; restore P6 and stop the scalar phase axis.
 * No new function or alignment credit.
 * 2026-09-27 sol-venus-room bounded revalidation: full normalized diff,
 * complete extent, allocator-order dump and all four pool words read again.
 * Retained P6 is 612/612 bytes, 116 differing halfwords / 30 aligned edits.
 * Count/options lifetime disagreement and second-phase pointer-view loss
 * are unchanged; existing phased union already failed to preserve the copy.
 * No independent pointer/dependency fact justifies reopening that axis.
 * Keep the exact-sibling dimensions and row setup admissions unchanged.
 * No new function bytes or alignment credit; still C not yet written.
 *
 * NONMATCHING P6 (2026-09-27): 612/612 bytes, 116 differing halfwords,
 * 30 aligned edits. Transfer exact EAST_PARTICLE_WAVE's row setup order:
 * initialize base z, initialize zero, then add offset, inside the already
 * admitted row <= 7 guard. GCSE retains separate user z/zero producers
 * (SI75/SI76 and SI146/SI147), followed by the z += offset update. Both
 * emitted blocks now have reference base/zero/add order. Full normalized
 * diff read: only those two instruction swaps changed from P5; dimensions,
 * topology, and all four pool words/offsets remain admitted. Retain P6.
 * Remaining: r9/sl count/options roles, absent second write-view copy with
 * hoisted scale literal, and the last rectangle's argument order. Code is
 * still two bytes short before native alignment; complete extent is 612.
 * P4/P5/P6 finish this bounded family pass. No further model without new
 * pointer/dependency evidence; no function or separate alignment credit.
 *
 * NONMATCHING P5 (2026-09-27): 612/612 bytes, 117 differing halfwords,
 * 34 aligned edits; topology and all four pool words/offsets unchanged.
 * Transfer exact EAST_PARTICLE_WAVE's Map_CopyCellsTo boundary to the four
 * intervening 5x2 copies. Like the now-exact ShiftBridge, dimensions belong
 * to inline call parameters rather than raw-call temporaries. Before:
 * width SI95 five refs/48 insns, height SI96 five refs/46 insns, both three
 * calls, yielding width r6/height r5. After: SI99/SI100 both five refs over
 * 48 insns as user formals; width wins the tie and yields reference r5/r6.
 * Both initializers and all eight stack stores now match; no other bytes
 * changed. Freeze that admission. Remaining: r9/sl count/options ownership,
 * missing second write-view copy and hoisted scale literal, row base/zero
 * setup order, and final rectangle argument scheduling. No new DONE.
 *
 * NONMATCHING P4 (2026-09-27): phased repeat/options union is byte-identical
 * to the retained 612-byte model: 127 halfwords / 44 aligned edits, all four
 * pool words and offsets unchanged. Own ROM reuses sl for the first phase's
 * repeat count then copies persistent r9 to sl for the second write view.
 * Reusing a union local across those phases does not preserve that copy:
 * allocation still gives count r9/options sl and hoists scale 0x4ccc to r9.
 * Full normalized diff read; the pointer-copy admission fails. This is not
 * the proven 3A0 phased-value result: the redundant pointer view collapses.
 * No follow-up without distinct pointer/dependency evidence. Trial retained
 * at 9b9a213f4; the simpler prior canonical body is restored below.
 * No function or alignment credit. Exact Venus neighbours are unchanged.
 *
 * NONMATCHING: 612/612 bytes, 127 differing halfwords, 44 aligned edits
 * (2026-09-26). Complete owner 020042bc..02004520 includes the four-word
 * pool at 02004510. All 31 calls audited; entry dispatcher 02003068 calls
 * this for scene 21. Shared EffectOptions and call bindings transferred
 * from Scene_RunEastParticleWaveSequence (020047c0), rechecked exact at
 * 424 bytes. No omitted calls or wrong interfaces found.
 * Baseline: 592/612 bytes, 229 differing halfwords, 118 aligned edits.
 * H1: scope each spawn z/zero after the initial row <= 7 guard, retaining
 * the repeated row test: 612/612, 127 halfwords, 45 aligned edits; equal
 * control-flow topology, with row now in reference r8 and zero in r7.
 * H2: pass &options to both spawns instead of opts, separating the store
 * view from the call view: byte-identical to H1; reverted to opts.
 * H3 retained: inline second-phase scale/spin update scope: 612/612,
 * 127 halfwords, 44 aligned edits. Changes the hoisted constant from
 * 0x17ffc to 0x4ccc but does not restore the reference's two pointer views.
 * Remaining: repeat/options r9-sl ownership, second-phase constant
 * hoisting instead of the pointer copy, rectangle width/height r5-r6,
 * and zero/z initialization order. Decoder gives no unique source repair.
 * Three structural trials exhausted; no generic RA/declaration sweep.
 * This remains not-yet-c and earns no DONE bytes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

void Main_0808a5e0(s32 cue);

static __inline__ void Rubble_SetScaleAndSpin(struct EffectOptions *opts)
{
    opts->start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
    opts->start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
    opts->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
}

void Scene_RunPairedParticleWaveSequence(void)
{
    struct EffectOptions options;
    u32 repeat;
    u32 row;
    s32 offset;
    u32 i;
    struct EffectOptions *opts;

    gEventWork->start_transition = 0x202;
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetChildValue(0, 15);
    Main_0808a5e0(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    repeat = 0;
    row = 0;
    opts = &options;
    offset = 0;
    do {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        i = 0;
        if (row <= 7) {
            s32 z = 0x300000;
            s32 zero = 0;

            z += offset;
            do {
                Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3600000, 0, z, 0, zero, zero, 0x880000, opts);
                z += 0x40000;
                i++;
            } while (i <= 3 && row <= 7);
        }
        Engine_TaskWait(3);
        if (row == 3 && repeat <= 2) {
            repeat++;
            goto again;
        }
        Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 9);
    Map_CopyCellsTo(111, 5, 117, 5, 5, 2);
    Map_CopyCellsTo(111, 10, 117, 10, 5, 2);
    Map_CopyCellsTo(111, 7, 111, 5, 5, 2);
    Map_CopyCellsTo(111, 7, 111, 10, 5, 2);
    row = 0;
    offset = 0;
    do {
        Rubble_SetScaleAndSpin(opts);
        i = 0;
        if (row <= 7) {
            s32 z = 0x300000;
            s32 zero = 0;

            z += offset;
            do {
                Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3000000, 0, z, 0, zero, zero, 0x880000, opts);
                z += 0x40000;
                i++;
            } while (i <= 3 && row <= 7);
        }
        Engine_TaskWait(3);
        Map_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 9);
    Engine_AudioPlayCue(289);
    Engine_EventWait(60);
    Engine_EventRequestExit(21);
    Engine_EventEnd();
}
