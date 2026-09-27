/* NONMATCHING P4 (2026-09-27): phased repeat/options union is byte-identical
 * to the retained 612-byte model: 127 halfwords / 44 aligned edits, all four
 * pool words and offsets unchanged. Own ROM reuses sl for the first phase's
 * repeat count then copies persistent r9 to sl for the second write view.
 * Reusing a union local across those phases does not preserve that copy:
 * allocation still gives count r9/options sl and hoists scale 0x4ccc to r9.
 * Full normalized diff read; the pointer-copy admission fails. This is not
 * the proven 3A0 phased-value result: the redundant pointer view collapses.
 * No follow-up without distinct pointer/dependency evidence. Trial retained
 * here for its commit; restore the simpler prior canonical body afterward.
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
    /* FAKEMATCH: one phased local owns the repeat count, then the write view. */
    union {
        u32 repeat;
        struct EffectOptions *options;
    } phase;
    u32 row;
    s32 offset;
    u32 i;
    struct EffectOptions *opts;

    gEventWork->start_transition = 0x202;
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    Engine_ActorSetChildValue(0, 15);
    Main_0808a5e0(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    phase.repeat = 0;
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
            s32 z = 0x300000 + offset;
            s32 zero = 0;

            do {
                Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3600000, 0, z, 0, zero, zero, 0x880000, opts);
                z += 0x40000;
                i++;
            } while (i <= 3 && row <= 7);
        }
        Engine_TaskWait(3);
        if (row == 3 && phase.repeat <= 2) {
            phase.repeat++;
            goto again;
        }
        Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 9);
    Engine_MapCopyCellsTo(111, 5, 117, 5, 5, 2);
    Engine_MapCopyCellsTo(111, 10, 117, 10, 5, 2);
    Engine_MapCopyCellsTo(111, 7, 111, 5, 5, 2);
    Engine_MapCopyCellsTo(111, 7, 111, 10, 5, 2);
    row = 0;
    phase.options = opts;
    offset = 0;
    do {
        Rubble_SetScaleAndSpin(phase.options);
        i = 0;
        if (row <= 7) {
            s32 z = 0x300000 + offset;
            s32 zero = 0;

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
