#include "TYPES.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
s32 Menu_RunWorkspaceSelectionLoop(void);
s32 Save_WriteSelectedSlot(void);
void UiText_ShowPositionedMessageAndWait(s32, s32);
s32 Menu_RunWorkspaceOptions(void);

#if defined(TBS_EDITION_DE)
#define RESULT_CELL_ADDR 0x03001CD8
#else
#define RESULT_CELL_ADDR 0x03001CC8
#endif

extern char MsgSleepMode;

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
        *(u8 *)RESULT_CELL_ADDR = result;
    } else if (result == 2) {
        if (Menu_RunWorkspaceOptions() == -1) {
            goto retry;
        }
    }
    return 0;
}
