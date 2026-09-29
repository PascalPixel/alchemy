/* Saving the battle results between two message windows, and the
   three-digit counter drawn into the map. */
#include "LOBBY.H"

extern u8 MsgLobbySavingBattleResults[];
extern u8 MsgLobbyBattleResultsSaved[];
extern u8 MsgLobbySavingMonsterBattle[];
extern u8 MsgLobbyMonsterBattleResults[];

void Engine_AudioPlayCue(s32 cue);
s32 UiText_OpenMessageWindow(s32 message, s32 x, s32 y, s32 flags);
s32 UiWork_IsComplete(void);
s32 UiWork_Finalize(s32 window, s32 flags);
void Engine_TaskWait(s32 frames);
void SaveState_ProcessSelectedSlot(void);
s32 Engine_MathDivide(s32 value, s32 divisor);
s32 Engine_MathRemainder(s32 value, s32 divisor);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width,
                           s32 height);
s32 Engine_MapRedraw(void);

s32 LinkLobby_SaveBattleResults(void)
{
    s32 window;

    Engine_AudioPlayCue(85);
    window = UiText_OpenMessageWindow((s32)MsgLobbySavingBattleResults, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    SaveState_ProcessSelectedSlot();
    /* FAKEMATCH: closing the first window through a void call sets the
     * window argument before the flags, as the game does. */
    ((void (*)(s32, s32))UiWork_Finalize)(window, 1);
    Engine_TaskWait(1);
    window = UiText_OpenMessageWindow((s32)MsgLobbyBattleResultsSaved, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    return UiWork_Finalize(window, 1);
}

s32 LinkLobby_SaveMonsterBattleResults(void)
{
    s32 window;

    Engine_AudioPlayCue(85);
    window = UiText_OpenMessageWindow((s32)MsgLobbySavingMonsterBattle, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    SaveState_ProcessSelectedSlot();
    /* FAKEMATCH: as above. */
    ((void (*)(s32, s32))UiWork_Finalize)(window, 1);
    Engine_TaskWait(1);
    window = UiText_OpenMessageWindow((s32)MsgLobbyMonsterBattleResults, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    return UiWork_Finalize(window, 1);
}

s32 LinkLobby_DrawThreeDigitValue(s32 value)
{
    s32 digit;

    if (value > 999)
        value = 999;
    for (digit = 0; digit <= 2; digit++) {
        Engine_MapCopyCellsTo(27, Engine_MathRemainder(value, 10), 16 - digit, 8, 1, 1);
        value = Engine_MathDivide(value, 10);
    }
    return Engine_MapRedraw();
}
