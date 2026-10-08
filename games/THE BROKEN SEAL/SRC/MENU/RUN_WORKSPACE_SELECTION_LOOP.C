#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "WORKSPACE_OPTIONS.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "RENDER_INPUT.H"
#include "SOUND_IDS.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "BTLSYS.H"
#include "UIFLOW.H"
void AudioCommand_PlayFar(s32 cue);

#define gKeyState ((u8 *)&gKeyState)
#define gKeysRepeat ((u8 *)&gKeysRepeat)
#define gWindowWork (*(struct UiTileSlotWork * *)&gWindowWork)

#define Shop_SetCursorFar ((void (*)(void*,s32,s32,s32))Shop_SetCursorFar)
#define Ui_ApplyTableOffsetToPair ((void (*)(void*))Ui_ApplyTableOffsetToPair)
#define UiIcon_PrepareObjectFar ((void (*)(void*))UiIcon_PrepareObjectFar)


extern u8 gDebugMode;



s32 Menu_RunWorkspaceSelectionLoop(void)
{
    struct RenderInput *window;
    s32 redraw;
    struct RenderOutput *pair;
    s32 first;
    s32 i;
    s32 j;
    s32 rows;
    s32 settings_only;
    s32 dim;
    s32 y;
    s32 result;
    s32 page;
    s32 index;
    s32 x;
    s32 top;
    s8 *tbl;
    struct RenderOutput *icon;
    struct WorkspaceWork *work;

    redraw = 1;
    rows = 3;
    settings_only = GameFlag_TestFar(0x17E);
    first = 0;
    Menu_InitializeSelectedWorkspace();
    work = gSelectionWork;
    window = Menu_CreateWorkspaceWindows();
#if EDITION_INTERNATIONAL
    y = -0x18;
    if (gDebugMode != 0) {
        y = -0x10;
    }
    pair = RenderResource_CreatePair(6, window, 0x28, y);
#else
    pair = RenderResource_CreatePair(6, window, 0x28, -0x18);
#endif
    WaitFrames(1);
    page = work->page;
    if (settings_only != 0) {
        rows = 1;
        first = 2;
    }
    if (gDebugMode != 0) {
        rows += 3;
    }
redraw_menu:
    if (redraw != 0) {
        redraw = 0;
        page = (page + rows) % rows;
        work->page = page;
        i = 0;
        if (i < rows) {
            j = first;
            tbl = Menu_WorkspaceIconFrames;
            do {
                icon = work->icon[i];
                icon->sentinel = 0xFB;
                UiIcon_PrepareObjectFar(icon);
                index = (u8)icon->index;
                dim = 0;
                if (i != work->page) {
                    dim = 1;
                }
                RenderResource_LoadFrame(*(s8 *)(j + (s32)tbl), index, dim);
                i++;
                j++;
            } while (i < rows);
        }
        top = window->y;
        x = window->x * 8;
        y = (((page * 3) + top) * 8) + 0x10;
        Shop_SetCursorFar(
            &work->cursor,
            x,
            y,
            3);
    }
    Ui_ApplyTableOffsetToPair(pair);
    WaitFrames(1);
    if (*(volatile s32 *)gKeyState & KEY_A) {
        result = page;
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    } else if (*(volatile s32 *)gKeyState & (KEY_B | KEY_START)) {
        result = -1;
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    } else {
        if (*(volatile s32 *)gKeysRepeat & KEY_UP) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            page -= 1;
            redraw = 1;
        } else if (*(volatile s32 *)gKeysRepeat & KEY_DOWN) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            page += 1;
            redraw = 1;
        }
        goto redraw_menu;
    }
    UiWork_Finalize(window, 2);
    Runtime_ScheduleCallbackAndReleaseBlock20A();
    WaitFrames(1);
    if (result >= 0) {
        result += first;
    }
    return result;
}

struct UiTileSlotWork {
    u8 unknown_000[RENDER_TILE_ATTR_OFS];
    u8 used[0x100];
    u16 next;
};


void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern, u8 *remap)
{
    struct UiTileSlotWork *work = gWindowWork;
    u8 pixels[128];
    u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    u32 slot = *(u8 *)entry;
    u8 *src;
    u8 *dst;
    u32 n;
    u32 i;

    dst = pixels;
    src = (u8 *)(0x06000000 + 32 * slot);
    for (i = 0; i < 32; i++) {
        /* FAKEMATCH: the second byte's address as its own value keeps the two stores on separate address registers. */
        u8 *high = dst + 1;

        n = *src++;
        dst[0] = n & 15;
        *high = n >> 4;
        dst += 2;
    }

    dst = pixels;
    src = sheet + pattern * 32;
    for (i = 0; i < 32; i++) {
        u32 value = *src++;
        u32 color = remap[value & 15];

        if (color != 0)
            *dst = color;
        dst++;
        color = remap[value >> 4];
        if (color != 0)
            *dst = color;
        dst++;
    }

    {
        u8 *p;

        dst = pixels;
        /* FAKEMATCH: counter before pointer, in one statement, orders the two initial moves as the listing has them. */
        i = 0, p = dst;
        while (i < 32) {
            u32 value = p[0] | p[1] << 4;

            p += 2;
            *dst++ = value;
            i++;
        }
    }

    if ((s8)slot >= 0) {
        for (n = 0; n < 128; n++) {
            u32 value = work->next;
            u32 following = (value + 1) % 128;

            slot = (u8)value;
            work->next = following;
            if (work->used[slot] == 0)
                break;
        }
        work->used[slot] = 1;
        slot |= 0x80;
        *entry = slot | 0xf000;
        *mirror = slot | 0xf000;
    }
    Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, REG_DMA3);
}
