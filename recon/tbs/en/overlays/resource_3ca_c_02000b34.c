/* NONMATCHING draft of Func_02000b34 (resource_3ca, 624 bytes): 11 differing
 * instructions, all in the three background priority writes.
 * 2026-10-02: every register write sits in a one-pass block (REG_SET). The
 * loop notes of such a block are scheduling barriers in sched2, and with
 * them both loops and the blend ramps match: the palette shifts keep source
 * order and each counter step lands after the wait argument. The blink loop
 * is a counted for loop, which the compiler turns into the countdown.
 * Left: the game loads 3 into r4 and builds the address of the stack
 * halfword in r5 only at its first store (a reload address, not a pseudo);
 * here the address is a pseudo from expansion and takes r4. The 0x7e00
 * palette write also shifts one instruction early. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Event_Wait(s32 frames)
{
    Engine_EventWait(frames);
}

static inline void Task_Wait(s32 frames)
{
    Engine_TaskWait(frames);
}

static inline s32 Task_AddCallback(void (*callback)(void), s32 priority)
{
    return Engine_TaskAddCallback(callback, priority);
}

static inline s32 Task_RemoveCallback(void (*callback)(void))
{
    return Engine_TaskRemoveCallback(callback);
}

#include "MAP_SCROLL.H"

extern u32 BabiFune_ShimmerActive;
extern u32 BabiFune_ShimmerPhase;
extern u32 Data_020097f4;
extern u32 BabiFune_DriftActive;
extern u32 BabiFune_SwellActive;
extern u32 BabiFune_SwellLayerSixY;
extern u32 BabiFune_SwellLayerSevenY;

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

#define REG_SET(address, value) do { *(volatile u16 *)(address) = (value); } while (0)

void Scene_ClosePresentationSequence(void)
{
    struct MapScrollWork *runtime;
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 phase;
    volatile u16 cnt;

    runtime = gCam;

    Event_Begin();
    Main_0808a460();
    Call3(Engine_ObjectMotionSetPositionAndReset, 0, 312, 232);
    Call3(Engine_ObjectMotionArmCallback, 0, 49152, 0);
    Event_Wait(40);
    Audio_PlayCue(140);
    for (i1 = 0; i1 <= 15; i1++) {
        REG_SET(0x05000000, (i1 << 11) | (i1 << 5));
        Event_Wait(10);
    }
    {
        s32 color = 0x7e00;

        REG_SET(0x05000000, color);
    }
    {
        s32 bright = 0x1010;
        s32 dim = 0x810;

        for (i2 = 0; i2 < 3; i2++) {
            Audio_PlayCue(212);
            REG_SET(0x04000052, bright);
            Event_Wait(3);
            REG_SET(0x04000052, dim);
            Event_Wait(65);
        }
    }
    BabiFune_ShimmerActive = 1;
    BabiFune_ShimmerPhase = 0;
    Task_AddCallback(BabiFune_UpdateWaves, TASK_PRIORITY_SCENE);
    BabiFune_DriftActive = 1;
    Event_Wait(20);
    Audio_PlayCue(163);
    Work_SetValuesIfNonNegative(65536, 65536, 65536);
    Event_Wait(60);
    BabiFune_DriftActive = 1;
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
        BabiFune_DriftActive = 0;
        cnt = (*(volatile u16 *)0x0400000e & 0xfffc) | three.v;
        REG_SET(0x0400000e, cnt);
        cnt = (*(volatile u16 *)0x0400000c & 0xfffc) | three.v;
        REG_SET(0x0400000c, cnt);
        two.v = 2;
        cnt = (*(volatile u16 *)0x0400000a & 0xfffc) | two.v;
        REG_SET(0x0400000a, cnt);
    }
    BabiFune_ShimmerActive = 0;
    Audio_PlayCue(288);
    Task_Wait(1);
    Audio_PlayCue(145);
    {
        s32 blend = 191;

        REG_SET(0x04000050, blend);
    }
    for (i3 = 0; i3 <= 16; i3++) {
        REG_SET(0x04000054, i3);
        Event_Wait(1);
    }
    Event_Wait(40);
    Work_SetValuesIfNonNegative(-1, -1, 58982);
    BabiFune_SwellLayerSixY = runtime->layers[1].offset_y;
    BabiFune_SwellLayerSevenY = runtime->layers[2].offset_y;
    BabiFune_SwellActive = 1;
    for (i4 = 16; i4 >= 0; i4--) {
        REG_SET(0x04000054, i4);
        Event_Wait(8);
    }
    Task_AddCallback(BabiFune_CyclePalette, TASK_PRIORITY_SCENE);
    Audio_PlayCue(80);
    Main_080b0060();
    Event_Wait(20);
    Event_End();
    Scene_RunExtendedPresentationSequence();
}
