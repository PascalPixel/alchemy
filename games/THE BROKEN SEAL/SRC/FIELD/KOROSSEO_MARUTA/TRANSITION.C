/* The mode task and the scripted transitions between rounds. */
#include "LOG_ROLLING.H"
#include "CALL.H"

void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter)
{
    s32 handler;

    Korosseo_ModeTaskMode = (u16)mode;
    Korosseo_ModeTaskParam = (u16)(parameter << 4);

    {
        s32 budget = 0xc80;
        s32 task = (s32)Korosseo_UpdateModeTask;
        Engine_TaskAddCallback(task, budget);
    }

    handler = (s32)&gColossoModeScriptDefault;
    if (mode == 2) {
        handler = (s32)&gColossoModeScript2;
    }
    if (mode == 4) {
        handler = (s32)&gColossoModeScript4;
    }
    if (mode == 3) {
        if (parameter != 0) {
            handler = (s32)&gColossoModeScript3B;
        } else {
            handler = (s32)&gColossoModeScript3A;
        }
    }

    Korosseo_ModeTaskTimer = 0;
    Korosseo_ModeTaskScript = handler;
    Korosseo_ModeMoveTarget = 0;
    Korosseo_ModeMoveDuration = 0;
    Korosseo_ModeTaskPosition = 0;
}

/*
 * resource_3bc scripted transition owner at 0x02003468, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Func_0808a010,
 * Audio_PlayCue, Func_02002e54, ColossoLogRollingStage_InitializeModeTask, Func_0808a018/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */
void ColossoLogRollingStage_RunScriptedTransition(s32 mode)
{
    extern void Korosseo_LoadPortrait(s32 mode);
    extern s32 AudioCommand_GetStateByte(void);
    extern void Audio_PlayCueFromEventWork(void);

    if (mode == 0) {
        Event_Begin();
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(30);
        Audio_PlayCue(0x59);
        Korosseo_LoadPortrait(0);
        ColossoLogRollingStage_InitializeModeTask(1, 0);
        Event_Wait(120);
        Event_End();
        return;
    }

    Audio_PlayCue(0xf7);
    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    {
        s16 *base = (s16 *)&gColossoModeScriptDefault;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Event_Wait(30);
    Audio_PlayCue(mode + 0x5a);
    Korosseo_LoadPortrait(mode);
    ColossoLogRollingStage_InitializeModeTask(1, 0);
    Event_Wait(120);

    goto check_transition;
wait_transition:
    Task_Wait(1);
check_transition:
    if (AudioCommand_GetStateByte() != 0)
        goto wait_transition;

    Audio_PlayCue(0x121);
    Korosseo_LoadPortrait(5);
    ColossoLogRollingStage_InitializeModeTask(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    ColossoLogRollingStage_InitializeModeTask(2, 1);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Korosseo_LoadPortrait(6);
    ColossoLogRollingStage_InitializeModeTask(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Korosseo_LoadPortrait(7);
    ColossoLogRollingStage_InitializeModeTask(4, 0);
    Audio_PlayCue(0xed);
    Audio_PlayCueFromEventWork();
    Event_End();
    GameFlag_Set(0x123);
}

void FieldScene_RunScene3bcSequenceA(s32 a0)
{
    extern void Korosseo_LoadPortrait();
    extern s32 AudioCommand_GetStateByte();

    s32 kind;

    Audio_PlayCue(247);
    Event_OpenScreen();
    Event_WaitForScreen();
    gColossoModeScript3A.span = a0 * 60;
    gColossoModeScript3B.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Event_Wait(30);
        Audio_PlayCue(86);
        Korosseo_LoadPortrait(8);
        Value2(ColossoLogRollingStage_InitializeModeTask, 3, 1);
        Event_Wait(-a0 * 60 + 60);
        kind = 0;
    } else {
        Event_Wait(30);
        Audio_PlayCue(a0 + 90);
        Korosseo_LoadPortrait(4);
        Value2(ColossoLogRollingStage_InitializeModeTask, 3, 0);
        Event_Wait(a0 * 60 + 60);
        kind = 8;
    }
    Actor_ShowEmote(kind, 0x105, 0);
    while (AudioCommand_GetStateByte()!= 0) {
        Task_Wait(1);
    }
    Audio_PlayCue(19);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Event_CloseScreen();
    Event_WaitForScreen();
}
