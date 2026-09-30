#include "TYPES.H"
extern u8 gTitleExtraOptionEnabled[];

struct MenuModeLabelState;
extern struct MenuModeLabelState *gMenuSelectWork;
extern u8 MsgPasswordLevel[];

s32 UiWindow_Create(s32, s32, s32, s32, s32);

s32 Menu_SelectSaveSlotAction(void)
{
    s32 type;
    s32 ret;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = SaveState_ScanRecordFlags();
    if (type < 0) {
        return -1;
    }
    if (type == 0) {
        return 0;
    }
    if (type == 3) {
        group = 1;
    } else if (type == 0x67) {
        group = 2;
    } else if (type > 0x64) {
        group = 3;
    } else {
        initial = 1;
    }
    AffineEffect_InitializeWork();
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x15);
    }
    if (group <= 1U) {
        Menu_AppendResourceEntry(0x16);
    }
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x17);
    }
    Menu_AppendResourceEntry(0x18);
    if ((*(s16 *)gTitleExtraOptionEnabled) != 0) {
        Menu_AppendResourceEntry(0x1D);
    }
    if (gTitleSendOptionEnabled != 0) {
        Menu_AppendResourceEntry(0x1E);
    }
    Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(initial);
    Menu_EndResourceSelection();
    if (ret >= 0) {
        ret = Menu_SaveSlotActionByPosition[ret + (group * 6)];
    }
    return ret;
}
