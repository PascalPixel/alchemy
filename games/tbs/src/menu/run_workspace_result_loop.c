#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "save_state.h"
#include "scene.h"
#include "resource.h"
#include "render_input.h"
#include "io_reg.h"
#include "uiflow.h"
#include "text/msg_ids.h"

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)

#define gMenuCtrlWork (*(struct DebugMenuState * *)&gMenuCtrlWork)
void UiText_ShowPositionedMessageAndWait(s32, s32);

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
        UiText_ShowPositionedMessageAndWait((s32)MsgSleepMode, 1);
        gSleepRequested = result;
    } else if (result == 2) {
        if (Menu_RunWorkspaceOptions() == -1) {
            goto retry;
        }
    }
    return 0;
}

struct DebugMenuState {
    u8 unknown_00[4];
    u16 active;
};


void WaitFrames(s32);
void UiText_DrawStringInWindow(const u8 *, struct RenderInput *, s32, s32);
void UiText_DrawNumberInWindow(s32, s32, struct RenderInput *, s32, s32);

s32 DebugMenu_BrowseIcons(void)
{
    s32 redraw;
    s32 tile;
    s32 page;
    s32 mode;
    struct RenderInput *window;
    s32 slot;
    s32 base;
    s32 i;
    s32 row;
    s32 x;
    s32 y;

    window = NULL;
    page = 0;
    mode = 0;
    redraw = 1;
    gMenuCtrlWork->active = redraw;
    WaitFrames(1);

    do {
        if (gKeysRepeat & KEY_LEFT) {
            page--;
            redraw = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
            redraw = 1;
            page++;
        }
        if (gKeysRepeat & KEY_L) {
            mode--;
            redraw = 1;
        }
        if (gKeysRepeat & KEY_R) {
            redraw = 1;
            mode++;
        }
        if (gKeysRepeat & KEY_A)
            goto close;
        if (gKeysRepeat & KEY_B)
            goto close;
        if (redraw != 0) {
            redraw = 0;
            page = (page + 8) % 8;
            mode = (mode + 3) % 3;
            UiWork_Finalize(window, 2);
            window = UiWindow_Create(10, 0, 18, 12, 2);
            if (mode == 0)
                UiText_DrawStringInWindow(Workspace_ItemTitle, window, 0, 0);
            else if (mode == 1)
                UiText_DrawStringInWindow(Workspace_EnergyTitle, window, 0, 0);
            else
                UiText_DrawStringInWindow(Workspace_StatusTitle, window, 0, 0);
            UiText_DrawStringInWindow(Workspace_PageLabel, window, 0, 8);
            UiText_DrawNumberInWindow(page, 0, window, 40, 8);
            base = page << 5;
            UiText_DrawNumberInWindow(base, 3, window, 64, 8);
            UiText_DrawStringInWindow(Workspace_RangeDash, window, 88, 8);
            UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
            for (i = 0; i <= 31; i++) {
                slot = -1;
                row = i / 8;
                x = (i - row * 8) * 16;
                y = row * 16 + 16;
                if (mode == 0) {
                    UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
                    RenderOutput_Create(slot, 0x40000000, window, x, y);
                } else if (mode == 1) {
                    UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
                    RenderOutput_Create(slot, 0x40000000, window, x, y);
                } else {
                    slot = Resource_FindFreeEntry();
                    Ui_BuildPatternToSlot(i, 0, slot);
                    RenderOutput_Create(slot, 0x40000000, window, x, y);
                }
            }
        }
        WaitFrames(1);
    } while (1);
close:
    UiWork_Finalize(window, 2);
    gMenuCtrlWork->active = 0;
    return 0;
}
