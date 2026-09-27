/* NONMATCHING: ship H2 2026-09-27, 624/624 bytes, 24 differing
 * halfwords, 23 aligned edits. Inline color-channel arguments were predicted
 * to change equal-priority shift creation order, based on -dL and scheduler
 * dumps (both shifts priority 4; later instruction wins). Complete output
 * remains byte-identical to baseline; inlining canonicalizes that boundary.
 * Full normalized diff read. Stop these two scope axes without another
 * spelling sweep; retain the ordinary palette expression as successor.
 * ship H1 2026-09-27, 624/624 bytes, 24 differing
 * halfwords, 23 aligned edits. Initializing the halfword priority before
 * clearing the spawn flag emits byte-identical baseline output (cmp of the
 * complete candidate). The scheduler still places the flag store first and
 * keeps priority/stack-address roles reversed. Full normalized diff read;
 * source-level initializer order does not own this scheduling boundary.
 * Previous baseline: 624 bytes, candidate 624, 24 differing halfwords, 23
 * halfword edits (2026-09-27). Scene_ClosePresentationSequence, meant for
 * FIELD/BABI_FUNE/F_00B34.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Complete extent 02000b34..02000da4: pool 0cac..0ce8, return 0d86,
 * final pool 0d88..0da0. The earlier 624 / 24 / 23 baseline keeps all pools.
 * Canonical family audit (2026-09-27): event table publishes 02008b35;
 * imports 0808a0d0 -> ObjectMotion_SetPositionAndReset (080921c4) and
 * 0808a1b8 -> ObjectMotion_ArmCallback (08092adc), both void, not generic
 * position/facing guesses. 080b0060 waits for audio state byte clear.
 * MAP_SCROLL.H layers[1/2].offset_y are +140/+170, agreeing with exact
 * SETUP_SCENE.C's lowered layers and the 0194 wave callback's restore.
 * H1: direct canonical calls, typed map record and named task callbacks:
 * 628 / 252 / 123. All calls/pool values remain correct, but priority 3200
 * and 10000 become shared saved-register constants, adding r9/fp saves.
 * Exact PALETTE_CYCLE.C documents inline-call boundaries preventing this.
 * H2: existing FIELD_EVENT.H event/work/task helpers and a typed inline
 * motion call restore per-call argument materialization: 624 / 24 / 23.
 * cmp of all 624 candidate bytes against the prior baseline is identical;
 * both pools and every previously matching call remain unchanged. Retained
 * as the typed model, with H1 preserved in a3c3a2d0b. No ABI omission or
 * missing call was found; types alone do not close the recorded residual.
 * Closed after this single evidence-backed follow-up, no register sweep.
 * Acceptance remains the complete 624 bytes including both pools and ROM
 * compare/coverage/verify; no credit for either draft. No constant sweep.
 * Three structural trials, starting from 620 / 253 / 153:
 * 1. Reconstructed the actual handoff: BLDALPHA 1010/0810, flag f8 set
 *    twice, flag f4 cleared, two-argument task callbacks, ramp through
 *    59ffff, BG3/BG2/BG1 priorities 3/3/2 via volatile stack halfword,
 *    flag e8 cleared, BLDCNT bf and BLDY ramps. Produced 616 / 242 / 112.
 * 2. Separate extern globals instead of neighbouring absolute casts restored
 *    independent loads and the retained f8 pointer: 628 / 182 / 88.
 * 3. Separate bright/dim loop constants and initialize priority two only at
 *    its phase: 624 / 25 / 23, with all literal pools matching.
 * Reopened bounded structural trials (2026-09-26):
 * 1. Inline priority-store helper: 620 / 172 / 131; added volatile stack
 *    reloads, changed priority constants and moved pools. Rejected.
 * 2. Bright/dim constants outside an explicit countdown loop: 624 / 24 / 23.
 *    Retained: counter initialization now follows constant setup as in ROM.
 * 3. One changing priority halfword: 624 / 91 / 40; extended its lifetime,
 *    displaced the retained zero and removed a later counter reset. Rejected.
 * Remaining: first palette shifts reversed, blink-loop argument setup and
 * decrement scheduled early, priority-three versus stack-address r5/r4
 * swap, priority-two temporary, and final BLDCNT/BLDY setup scheduling.
 * Stop at three structural hypotheses. No credit until every byte matches. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "MAP_SCROLL.H"

extern u32 Data_020097e8;
extern u32 Data_020097ec;
extern u32 Data_020097f4;
extern u32 Data_020097f8;
extern u32 Data_020097fc;
extern u32 Data_02009804;
extern u32 Data_02009808;

/* AUDITED GENERATED PRESENTATION FINALE for Scene_ClosePresentationSequence:
 * 35 calls, palette ramps, blend-register setup, and runtime handoff. */

void Main_0808a460(void);
void Main_080b0060(void);
void Scene_RunExtendedPresentationSequence(void);
void BabiFune_UpdateWaves(void);
void SceneState_CountDownEveryFortyTicks(void);
void BabiFune_CyclePalette(void);

/* Exact PALETTE_CYCLE.C keeps service arguments inside an inline call. */
static __inline__ void Call3(void (*service)(s32, s32, s32),
                           s32 first, s32 second, s32 third)
{
    service(first, second, third);
}

void Scene_ClosePresentationSequence(void)
{
    struct MapScrollWork *runtime;
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 phase;
    volatile u16 cnt;

    runtime = Data_03001e70;

    Event_Begin();
    Main_0808a460();
    Call3(Engine_ObjectMotionSetPositionAndReset, 0, 312, 232);
    Call3(Engine_ObjectMotionArmCallback, 0, 49152, 0);
    Event_Wait(40);
    Audio_PlayCue(140);
    for (i1 = 0; i1 <= 15; i1++) {
        *(volatile u16 *)0x05000000 = (i1 << 11) | (i1 << 5);
        Event_Wait(10);
    }
    {
        s32 color = 0x7e00;

        *(volatile u16 *)0x05000000 = color;
    }
    {
        s32 bright = 0x1010;
        s32 dim = 0x810;

        i2 = 2;
        do {
            Audio_PlayCue(212);
            *(volatile u16 *)0x04000052 = bright;
            Event_Wait(3);
            *(volatile u16 *)0x04000052 = dim;
            i2--;
            Event_Wait(65);
        } while (i2 >= 0);
    }
    Data_020097e8 = 1;
    Data_020097ec = 0;
    Task_AddCallback(BabiFune_UpdateWaves, TASK_PRIORITY_SCENE);
    Data_020097f8 = 1;
    Event_Wait(20);
    Audio_PlayCue(163);
    Work_SetValuesIfNonNegative(65536, 65536, 65536);
    Event_Wait(60);
    Data_020097f8 = 1;
    Work_SetValuesIfNonNegative(131072, 131072, 65536);
    Event_Wait(60);
    Work_SetValuesIfNonNegative(196608, 196608, 65536);
    Data_020097f4 = 0;
    Task_AddCallback(SceneState_CountDownEveryFortyTicks, TASK_PRIORITY_SCENE);
    phase = 0;
    do {
        runtime->layers[1].offset_y += 0x3333;
        runtime->layers[2].offset_y += 0x3333;
        phase += 0x3333;
        Task_Wait(1);
    } while (phase <= 0x59ffff);
    Task_RemoveCallback(SceneState_CountDownEveryFortyTicks);
    {
        /* FAKEMATCH: halfword priority locals retain the short pool reach. */
        struct Half { u16 v; } three, two;

        three.v = 3;
        Data_020097f8 = 0;
        cnt = (*(volatile u16 *)0x0400000e & 0xfffc) | three.v;
        *(volatile u16 *)0x0400000e = cnt;
        cnt = (*(volatile u16 *)0x0400000c & 0xfffc) | three.v;
        *(volatile u16 *)0x0400000c = cnt;
        two.v = 2;
        cnt = (*(volatile u16 *)0x0400000a & 0xfffc) | two.v;
        *(volatile u16 *)0x0400000a = cnt;
    }
    Data_020097e8 = 0;
    Audio_PlayCue(288);
    Task_Wait(1);
    Audio_PlayCue(145);
    {
        s32 blend = 191;

        *(volatile u16 *)0x04000050 = blend;
    }
    for (i3 = 0; i3 <= 16; i3++) {
        *(volatile u16 *)0x04000054 = i3;
        Event_Wait(1);
    }
    Event_Wait(40);
    Work_SetValuesIfNonNegative(-1, -1, 58982);
    Data_02009804 = runtime->layers[1].offset_y;
    Data_02009808 = runtime->layers[2].offset_y;
    Data_020097fc = 1;
    for (i4 = 16; i4 >= 0; i4--) {
        *(volatile u16 *)0x04000054 = i4;
        Event_Wait(8);
    }
    Task_AddCallback(BabiFune_CyclePalette, TASK_PRIORITY_SCENE);
    Audio_PlayCue(80);
    Main_080b0060();
    Event_Wait(20);
    Event_End();
    Scene_RunExtendedPresentationSequence();
}
