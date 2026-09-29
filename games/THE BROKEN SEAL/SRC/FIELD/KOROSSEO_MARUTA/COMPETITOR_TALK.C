/* A competitor's question at entrance 2. Each competitor has three lines, the
 * question and its two answers, counted from MsgKorosseoDidntThinkBattles; a
 * yes closes the screen and selects that competitor. */
#include "LOG_ROLLING.H"
extern u8 MsgKorosseoDidntThinkBattles[];

void KorosseoMaruta_RunCompetitorTalk(s32 a0)
{
    u8 *work;
    s32 owner;
    s32 msg;
    s32 lines;
    s32 entrance;

    work = (u8 *)gEventWork;
    owner = gGameState.selected_actor;
    entrance = gGameState.entrance;
    if (entrance == 2) {
        Event_Begin();
        msg = (s32)MsgKorosseoDidntThinkBattles;
        lines = (a0 << 1) + a0;
        Event_SetMessage(lines + msg);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(owner, 0) == 0) {
            s32 yes = msg + 1;
            Event_SetMessage(lines + yes);
            Event_ShowMessage(a0, 0);
            *(s32 *)(work + 0x1c0) = 0x200;
            *(s32 *)(work + 0x1c8) = 15;
            Event_CloseScreen();
            Event_WaitForScreen();
            Korosseo_SelectSoloCompetitor(a0);
            Event_OpenScreen();
            Event_WaitForScreen();
        } else {
            s32 no = msg + 2;
            Event_SetMessage(lines + no);
            Event_ShowMessage(a0, 0);
        }
        Event_End();
    }
}
