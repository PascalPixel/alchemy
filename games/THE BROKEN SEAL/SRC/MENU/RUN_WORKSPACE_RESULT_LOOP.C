#include "TYPES.H"
#include "SCENE.H"
s32 Menu_RunWorkspaceSelectionLoop(void);
s32 Save_WriteSelectedSlot(void);
void UiText_ShowPositionedMessageAndWait(s32, s32);
s32 Menu_RunWorkspaceOptions(void);

extern char MsgSleepMode;
extern u8 gSleepRequested;

s32 Menu_RunWorkspaceResultLoop(void)
{
    s32 result;

retry:
    result = Menu_RunWorkspaceSelectionLoop();
    if (result == -1) {
        return -1;
    }
    if (result == 0) {
        if (Save_WriteSelectedSlot() == -1) {
            goto retry;
        }
    } else if (result == 1) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgSleepMode, 1);
        gSleepRequested = result;
    } else if (result == 2) {
        if (Menu_RunWorkspaceOptions() == -1) {
            goto retry;
        }
    }
    return 0;
}
