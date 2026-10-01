#include "TASK.H"
#include "CALL.H"

/*
 * The mode task's per-frame routine is installed as a callback.  The branch
 * chain picks one of five mode records by mode, consulting param only when
 * mode is 3.  The stores that follow reset the rest of the task's state.
 */
void KorosseoKabe_InitializeModeTask(u32 mode, u32 param)
{
    s32 handler;

    Korosseo_ModeTaskMode = (u16)mode;
    Korosseo_ModeTaskParam = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        Engine_TaskAddCallback(Korosseo_UpdateModeTask, budget);
    }

    handler = (s32)&KorosseoKabe_ModeRecordDefault;
    if (mode == 2) {
        handler = (s32)&KorosseoKabe_ModeRecordTwo;
    }
    if (mode == 4) {
        handler = (s32)&KorosseoKabe_ModeRecordFour;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&KorosseoKabe_ModeRecordThreeAlt;
        } else {
            handler = (s32)&KorosseoKabe_ModeRecordThree;
        }
    }

    Korosseo_ModeTaskTimer = 0;
    Korosseo_ModeTaskScript = handler;
    Korosseo_ModeMoveTarget = 0;
    Korosseo_ModeMoveDuration = 0;
    Korosseo_ModeTaskPosition = 0;
}

void KorosseoKabe_RunScriptedTransition(s32 mode)
{
    if (mode == 0) {
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(30);
        Audio_PlayCue(0x59);
        Korosseo_LoadPortrait(0);
        KorosseoKabe_InitializeModeTask(1, 0);
        Engine_EventWait(120);
        Engine_EventEnd();
        return;
    }

    Audio_PlayCue(0xf7);
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    {
        s16 *base = (s16 *)&KorosseoKabe_ModeRecordDefault;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Engine_EventWait(30);
    Audio_PlayCue(mode + 0x5a);
    Korosseo_LoadPortrait(mode);
    KorosseoKabe_InitializeModeTask(1, 0);
    Engine_EventWait(120);

    goto check_transition;
wait_transition:
    Engine_TaskWait(1);
check_transition:
    if (AudioCommand_GetStateByte() != 0)
        goto wait_transition;

    Audio_PlayCue(0x121);
    Korosseo_LoadPortrait(5);
    KorosseoKabe_InitializeModeTask(2, 0);
    Audio_PlayCue(0xec);
    Engine_EventWait(60);
    KorosseoKabe_InitializeModeTask(2, 1);
    Audio_PlayCue(0xec);
    Engine_EventWait(60);
    Korosseo_LoadPortrait(6);
    KorosseoKabe_InitializeModeTask(2, 0);
    Audio_PlayCue(0xec);
    Engine_EventWait(60);
    Korosseo_LoadPortrait(7);
    KorosseoKabe_InitializeModeTask(4, 0);
    Audio_PlayCue(0xed);
    Audio_PlayCueFromEventWork();
    Engine_EventEnd();
    GameFlag_Set(0x123);
}

void FieldScene_RunLateSequence(s32 a0)
{
    void Engine_EventOpenScreen();

    s32 kind;

    Audio_PlayCue(247);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    KorosseoKabe_ModeRecordThree.span = a0 * 60;
    KorosseoKabe_ModeRecordThreeAlt.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Engine_EventWait(30);
        Audio_PlayCue(86);
        Korosseo_LoadPortrait(8);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Value2(KorosseoKabe_InitializeModeTask, 3, 1);
        Engine_EventWait(-a0 * 60 + 60);
        kind = 0;
    } else {
        Engine_EventWait(30);
        Audio_PlayCue(a0 + 90);
        Korosseo_LoadPortrait(4);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Value2(KorosseoKabe_InitializeModeTask, 3, 0);
        Engine_EventWait(a0 * 60 + 60);
        kind = 8;
    }
    Actor_ShowEmote(kind, 0x105, 0);
    while (AudioCommand_GetStateByte()!= 0) {
        Engine_TaskWait(1);
    }
    Audio_PlayCue(19);
    Engine_EventWait(30);
    Audio_PlayCue(0x121);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
}
