#include "TYPES.H"
#include "SAVE_STATE.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
s32 Menu_RunWorkspaceSelectionLoop(void);
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

/* The debug icon browser: ITEM, ENERGY and STATUS pages of 32 icons. */
struct DebugMenuState {
    u8 unknown_00[4];
    u16 active;
};

extern struct DebugMenuState *gMenuCtrlWork;
extern volatile u32 gKeysRepeat;
extern u8 Data_08037440[], Data_08037448[], Data_08037450[];
extern u8 Data_08037458[], Data_08037460[];

void WaitFrames(s32);
void UiWork_Finalize(struct RenderInput *, s32);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(const u8 *, struct RenderInput *, s32, s32);
void UiText_DrawNumberInWindow(s32, s32, struct RenderInput *, s32, s32);
void UiIcon_BuildItemIconTiles(u32, s32, s32 *, s32 *, s32);
void UiIcon_BuildAbilityIconTiles(u32, s32, s32 *, s32 *, s32);
s32 Ui_BuildPatternToSlot(s32, s32, s32);

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
        if (gKeysRepeat & 0x20) {
            page--;
            redraw = 1;
        }
        if (gKeysRepeat & 0x10) {
            redraw = 1;
            page++;
        }
        if (gKeysRepeat & 0x200) {
            mode--;
            redraw = 1;
        }
        if (gKeysRepeat & 0x100) {
            redraw = 1;
            mode++;
        }
        if (gKeysRepeat & 1)
            goto close;
        if (gKeysRepeat & 2)
            goto close;
        if (redraw != 0) {
            redraw = 0;
            page = (page + 8) % 8;
            mode = (mode + 3) % 3;
            UiWork_Finalize(window, 2);
            window = UiWindow_Create(10, 0, 18, 12, 2);
            if (mode == 0)
                UiText_DrawStringInWindow(Data_08037440, window, 0, 0);
            else if (mode == 1)
                UiText_DrawStringInWindow(Data_08037448, window, 0, 0);
            else
                UiText_DrawStringInWindow(Data_08037450, window, 0, 0);
            UiText_DrawStringInWindow(Data_08037458, window, 0, 8);
            UiText_DrawNumberInWindow(page, 0, window, 40, 8);
            base = page << 5;
            UiText_DrawNumberInWindow(base, 3, window, 64, 8);
            UiText_DrawStringInWindow(Data_08037460, window, 88, 8);
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
