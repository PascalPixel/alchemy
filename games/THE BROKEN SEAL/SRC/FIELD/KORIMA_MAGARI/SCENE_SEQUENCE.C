#include "TYPES.H"
extern u8 MsgFieldFlippedSwitch[];

extern s16 *gKorimaMagariLayout;
extern s32 KorimaMagari_ShakeChance;
void State_CopyPresetA0d0WithOffsetB0(void);
void State_UpdateScrollRegistersWithPreset(void);
void KorimaMagari_DrawPanel();
void Scene_RepaintBoardRecords();
void Engine_TaskWait();
void Engine_EventBegin();
void Engine_MessageShowCentered();
void Engine_MapCopyCells();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorSetPosition();
void Engine_AudioPlayCue();
void Engine_ActorSetAnimation();
void Engine_TaskAddCallback();
void Runtime_SetIrqHandler();
void Engine_TaskRemoveCallback();
void Engine_MapRedraw();
void Engine_EventEnd();

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Scene_RunKorimaMagariSequence(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 base5_200a0dc;
    s32 v3;

    Engine_EventBegin();
    Call2(Engine_CameraSetSpeed, 0x10000, 0x2000);
    Call4(Engine_CameraMoveTo, 0x1080000, -1, 0x1c00000, 1);
    Engine_CameraWaitForMove();
    Call2(Engine_MessageShowCentered, (s32)MsgFieldFlippedSwitch, 1);
    Engine_AudioPlayCue(232);
    if (*gKorimaMagariLayout != 0) {
    } else {
        Call3(Engine_ActorSetPosition, 9, 0x1000000, 0x1ce0000);
        Call6(Engine_MapCopyCells, 77, 34, 1, 2, 83, 25);
        Engine_TaskWait(3);
        Call6(Engine_MapCopyCells, 78, 34, 1, 2, 83, 25);
        Engine_TaskWait(3);
        Call6(Engine_MapCopyCells, 79, 34, 1, 2, 83, 25);
        v5 = 79;
        Engine_TaskWait(30);
        Call6(Engine_MapCopyCells, 67, 34, 2, 5, v5, 25);
        Engine_TaskWait(6);
        Call6(Engine_MapCopyCells, 69, 34, 2, 5, v5, 25);
        Engine_ActorSetAnimation(9, 1);
        Engine_AudioPlayCue(240);
        Engine_TaskWait(6);
        Call6(Engine_MapCopyCells, 71, 34, 2, 5, v5, 25);
        Engine_TaskWait(6);
        Call6(Engine_MapCopyCells, 73, 34, 2, 5, v5, 25);
        Call6(Engine_MapCopyCells, 75, 38, 2, 1, v5, 29);
        Engine_TaskWait(4);
        Call6(Engine_MapCopyCells, 77, 38, 2, 1, v5, 29);
        Engine_TaskWait(6);
        Call6(Engine_MapCopyCells, 79, 38, 2, 1, v5, 29);
        Engine_TaskWait(8);
        Call6(Engine_MapCopyCells, 65, 53, 2, 1, v5, 29);
        Call6(Engine_MapCopyCells, 65, 40, 2, 4, 15, 28);
        goto L_02000626;
    }
    Call3(Engine_ActorSetPosition, 9, 0x1000000, 0x1e00000);
    Call6(Engine_MapCopyCells, 78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Call6(Engine_MapCopyCells, 77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Call6(Engine_MapCopyCells, 76, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Call6(Engine_MapCopyCells, 65, 45, 2, 4, 15, 28);
    Call6(Engine_MapCopyCells, 71, 50, 2, 5, 79, 25);
    Engine_ActorSetAnimation(9, 2);
    Engine_AudioPlayCue(230);
    Engine_TaskWait(6);
    Call6(Engine_MapCopyCells, 69, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Call6(Engine_MapCopyCells, 67, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Call6(Engine_MapCopyCells, 65, 50, 2, 5, 79, 25);
    Engine_TaskWait(30);
    L_02000626:;
    if (*gKorimaMagariLayout == 0) {
        KorimaMagari_DrawPanel(9, 19, 16, 5, *gKorimaMagariLayout, 9, 30);
        KorimaMagari_DrawPanel(9, 51, 16, 5, 1, 9, 30);
        KorimaMagari_DrawPanel(41, 51, 16, 5, 2, 9, 30);
    } else {
        KorimaMagari_DrawPanel(9, 19, 16, 5, 0, 9, 30);
        KorimaMagari_DrawPanel(9, 83, 16, 5, 1, 9, 30);
        KorimaMagari_DrawPanel(41, 83, 16, 5, 2, 9, 30);
    }

    KorimaMagari_ShakeChance = 0;
    Call2(Engine_TaskAddCallback, (s32)State_CopyPresetA0d0WithOffsetB0, 0xc80);
    Engine_TaskWait(1);
    /* FAKEMATCH: the two do/while (0) wraps keep the flag and the counter
     * address in r6 and r5. */
    do {
        Runtime_SetIrqHandler(1, 0, State_UpdateScrollRegistersWithPreset);
    } while (0);
    do {
        Engine_AudioPlayCue(231);
    } while (0);
    KorimaMagari_ShakeChance = 0;
    do {
        Engine_TaskWait(1);
        v3 = (KorimaMagari_ShakeChance + 1);
        KorimaMagari_ShakeChance += 1;
    } while (v3 <= 100);
    Call1(Engine_AudioPlayCue, 0x121);
    if (*gKorimaMagariLayout == 0) {
        KorimaMagari_DrawPanel(9, 19, 16, 5, *gKorimaMagariLayout, 9, 19);
        KorimaMagari_DrawPanel(9, 51, 16, 5, 1, 9, 19);
        KorimaMagari_DrawPanel(41, 51, 16, 5, 2, 9, 19);
    } else {
        KorimaMagari_DrawPanel(9, 19, 16, 5, 0, 9, 19);
        KorimaMagari_DrawPanel(9, 83, 16, 5, 1, 9, 19);
        KorimaMagari_DrawPanel(41, 83, 16, 5, 2, 9, 19);
    }
    Engine_TaskWait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Engine_TaskWait(1);
    Call1(Engine_TaskRemoveCallback, (s32)State_CopyPresetA0d0WithOffsetB0);
    *(u16 *)gKorimaMagariLayout ^= 1;
    Scene_RepaintBoardRecords();
    Engine_MapRedraw();
    Engine_EventEnd();
}
