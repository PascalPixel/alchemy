#include "TASK.H"

/*
 * The mode task's per-frame routine is installed as a callback.  The branch
 * chain picks one of five mode records by mode, consulting param only when
 * mode is 3.  The stores that follow reset the rest of the task's state.
 */
void KorosseoKabe_InitializeModeTask(u32 mode, u32 param)
{
    s32 handler;

    KorosseoKabe_ModeTaskMode = (u16)mode;
    KorosseoKabe_ModeTaskParam = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        Engine_TaskAddCallback(CommandInterpolationRenderer_Update, budget);
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

    KorosseoKabe_ModeTaskTimer = 0;
    KorosseoKabe_ModeTaskHandler = handler;
    KorosseoKabe_ModeTaskStep = 0;
    KorosseoKabe_ModeTaskCount = 0;
    KorosseoKabe_ModeTaskWord = 0;
}

void KorosseoKabe_RunScriptedTransition(s32 mode)
{
    if (mode == 0) {
        Event_Begin();
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(30);
        Audio_PlayCue(0x59);
        Korosseo_LoadPortrait(0);
        KorosseoKabe_InitializeModeTask(1, 0);
        Event_Wait(120);
        Event_End();
        return;
    }

    Audio_PlayCue(0xf7);
    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    {
        s16 *base = (s16 *)&KorosseoKabe_ModeRecordDefault;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Event_Wait(30);
    Audio_PlayCue(mode + 0x5a);
    Korosseo_LoadPortrait(mode);
    KorosseoKabe_InitializeModeTask(1, 0);
    Event_Wait(120);

    goto check_transition;
wait_transition:
    Task_Wait(1);
check_transition:
    if (AudioCommand_GetStateByte() != 0)
        goto wait_transition;

    Audio_PlayCue(0x121);
    Korosseo_LoadPortrait(5);
    KorosseoKabe_InitializeModeTask(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    KorosseoKabe_InitializeModeTask(2, 1);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Korosseo_LoadPortrait(6);
    KorosseoKabe_InitializeModeTask(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Korosseo_LoadPortrait(7);
    KorosseoKabe_InitializeModeTask(4, 0);
    Audio_PlayCue(0xed);
    Audio_PlayCueFromEventWork();
    Event_End();
    GameFlag_Set(0x123);
}

void FieldScene_RunLateSequence(s32 a0)
{
    void Event_OpenScreen();

    s32 kind;

    Audio_PlayCue(247);
    Event_OpenScreen();
    Event_WaitForScreen();
    KorosseoKabe_ModeRecordThree.span = a0 * 60;
    KorosseoKabe_ModeRecordThreeAlt.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Event_Wait(30);
        Audio_PlayCue(86);
        Korosseo_LoadPortrait(8);
        Value2(KorosseoKabe_InitializeModeTask, 3, 1);
        Event_Wait(-a0 * 60 + 60);
        kind = 0;
    } else {
        Event_Wait(30);
        Audio_PlayCue(a0 + 90);
        Korosseo_LoadPortrait(4);
        Value2(KorosseoKabe_InitializeModeTask, 3, 0);
        Event_Wait(a0 * 60 + 60);
        kind = 8;
    }
    Actor_ShowEmote(kind, 0x105, 0);
    while (Value0(AudioCommand_GetStateByte)!= 0) {
        Task_Wait(1);
    }
    Audio_PlayCue(19);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Event_CloseScreen();
    Event_WaitForScreen();
}
