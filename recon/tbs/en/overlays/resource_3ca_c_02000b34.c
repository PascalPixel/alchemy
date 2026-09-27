/* NONMATCHING: 624 bytes, candidate 628, 252 differing halfwords, 123
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
 * Prediction for one follow-up: existing event helpers restore per-call
 * argument materialization while retaining the proven map/callback types.
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

    Engine_EventBegin();
    Main_0808a460();
    Engine_ObjectMotionSetPositionAndReset(0, 312, 232);
    Engine_ObjectMotionArmCallback(0, 49152, 0);
    Engine_EventWait(40);
    Engine_AudioPlayCue(140);
    for (i1 = 0; i1 <= 15; i1++) {
        *(volatile u16 *)0x05000000 = (i1 << 11) | (i1 << 5);
        Engine_EventWait(10);
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
            Engine_AudioPlayCue(212);
            *(volatile u16 *)0x04000052 = bright;
            Engine_EventWait(3);
            *(volatile u16 *)0x04000052 = dim;
            i2--;
            Engine_EventWait(65);
        } while (i2 >= 0);
    }
    Data_020097e8 = 1;
    Data_020097ec = 0;
    Engine_TaskAddCallback(BabiFune_UpdateWaves, TASK_PRIORITY_SCENE);
    Data_020097f8 = 1;
    Engine_EventWait(20);
    Engine_AudioPlayCue(163);
    Engine_WorkSetValuesIfNonNegative(65536, 65536, 65536);
    Engine_EventWait(60);
    Data_020097f8 = 1;
    Engine_WorkSetValuesIfNonNegative(131072, 131072, 65536);
    Engine_EventWait(60);
    Engine_WorkSetValuesIfNonNegative(196608, 196608, 65536);
    Data_020097f4 = 0;
    Engine_TaskAddCallback(SceneState_CountDownEveryFortyTicks, TASK_PRIORITY_SCENE);
    phase = 0;
    do {
        runtime->layers[1].offset_y += 0x3333;
        runtime->layers[2].offset_y += 0x3333;
        phase += 0x3333;
        Engine_TaskWait(1);
    } while (phase <= 0x59ffff);
    Engine_TaskRemoveCallback(SceneState_CountDownEveryFortyTicks);
    Data_020097f8 = 0;
    {
        /* FAKEMATCH: halfword priority locals retain the short pool reach. */
        struct Half { u16 v; } three, two;

        three.v = 3;
        cnt = (*(volatile u16 *)0x0400000e & 0xfffc) | three.v;
        *(volatile u16 *)0x0400000e = cnt;
        cnt = (*(volatile u16 *)0x0400000c & 0xfffc) | three.v;
        *(volatile u16 *)0x0400000c = cnt;
        two.v = 2;
        cnt = (*(volatile u16 *)0x0400000a & 0xfffc) | two.v;
        *(volatile u16 *)0x0400000a = cnt;
    }
    Data_020097e8 = 0;
    Engine_AudioPlayCue(288);
    Engine_TaskWait(1);
    Engine_AudioPlayCue(145);
    {
        s32 blend = 191;

        *(volatile u16 *)0x04000050 = blend;
    }
    for (i3 = 0; i3 <= 16; i3++) {
        *(volatile u16 *)0x04000054 = i3;
        Engine_EventWait(1);
    }
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(-1, -1, 58982);
    Data_02009804 = runtime->layers[1].offset_y;
    Data_02009808 = runtime->layers[2].offset_y;
    Data_020097fc = 1;
    for (i4 = 16; i4 >= 0; i4--) {
        *(volatile u16 *)0x04000054 = i4;
        Engine_EventWait(8);
    }
    Engine_TaskAddCallback(BabiFune_CyclePalette, TASK_PRIORITY_SCENE);
    Engine_AudioPlayCue(80);
    Main_080b0060();
    Engine_EventWait(20);
    Engine_EventEnd();
    Scene_RunExtendedPresentationSequence();
}
