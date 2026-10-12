/* First draft, differ 44 of 47: the selection loop is laid out otherwise (the listing tests
 * selection 0 first and falls through to 1 and 2). Its three messages (0x22, 0x23, 0x1160) are pool words
 * in the listing and need names in all six PO catalogs before this can link. */
#include "types.h"
#include "frame.h"
#include "scene_call.h"

/* Placeholders: messages 0x22, 0x23 and 0x1160 have no name in the PO catalogs yet. */
extern u8 MsgId_0022[];
extern u8 MsgId_0023[];
extern u8 MsgId_1160[];

s32 Func_08040798(s32 selection);
s32 Func_08043690(void);
s32 Func_080400e8(void);

s32 Func_0804e0d0(void)
{
    s32 selection = 0;

    for (;;) {
        selection = Func_08040798(selection);
        if (selection == -1)
            return -1;
        if (selection == 0) {
            if (GameFlag_Test(0x17c)) {
                UiText_ShowPositionedMessageAndWait((s32)MsgId_0022, 1);
                continue;
            }
            if (GameFlag_Test(0x17d)) {
                UiText_ShowPositionedMessageAndWait((s32)MsgId_0023, 1);
                continue;
            }
            if (Func_08043690() != -1)
                break;
        } else if (selection == 1) {
            UiText_ShowPositionedMessageAndWait((s32)MsgId_1160, 1);
            gSleepRequested = selection;
            break;
        } else if (selection == 2) {
            if (Func_080400e8() != -1)
                break;
        } else {
            break;
        }
    }
    return 0;
}
