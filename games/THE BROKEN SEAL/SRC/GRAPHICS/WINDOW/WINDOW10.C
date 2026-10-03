#include "TYPES.H"
#include "WINDOW.H"
#include "CALLBACK_SCHEDULER.H"
#include "TBS_EDITION.H"
#include "SYSTEM.H"
#include "UI.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void UiWindow_SetPaletteBitRectFar(s32, s32, s32, s32, s32);


struct ChooserMenu {
    u8 reserved_000[0x10];
    s32 owner_window;
    u8 reserved_014[0x1c];
    s32 option_window;
    u8 reserved_034[0xd8];
    s32 message_window;
};

struct ChooserTextWork {
    u8 reserved_000[RENDER_MENU_BUSY_OFS];
    u8 menu_busy;
    u8 reserved_ea7[RENDER_COUNTER_OFS - RENDER_MENU_BUSY_OFS - 1];
    u16 resource;
    u8 reserved_12b8[RENDER_RESULT_OFS + 4 - RENDER_COUNTER_OFS - 2];
    u8 text_busy;
};


struct ChooserMessage {
    struct ChooserMessage *next;
    u8 reserved_04[16];
    u16 flags;
    u16 reserved_16;
    u16 timer;
    u16 count;
};

extern struct ChooserMenu *gMenuWork;
extern volatile u32 gKeysRepeat;
extern volatile u32 gKeyState;
extern char MsgDjinnInfoPrompt;
extern char MsgDjinnInfoTopic;
extern char MsgDjinnInfoText;
void RenderOutput_ClearListFar(s32 window);
void UiWindow_Clear(s32 window);
void UiWindow_MarkVisibleTileAttributesFar(void);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
s32 UiText_OpenEntryMessageFar(s32 window, s32 message);
s32 Menu_DrawAtWindowOffset(void *window, s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Menu_GetModuloOfSum(s32 value, s32 modulus);
void Audio_PlayCue(s32 cue);
s32 Resource_ResetEntry(u32 resource);
void UiWork_FinalizeFar(s32 window, s32 mode);
void Menu_UpdateEntryObjectTransforms(void);
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);

s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    UiWindow_SetPaletteBitRectFar(((struct UiWindow *)win)->x + arg1 + 1, ((struct UiWindow *)win)->y + arg2 + 1, arg3, arg4, arg5);
}

/* Set palette `palette` on every tile of a clipped rectangle of the 32x32
   tile map that uses palette 15, and mark the touched rows dirty. */
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    u8 *base;
    u16 *p;
    s32 col;
    u32 tile;
    u32 masked;

    base = gWindowWork[0];
    palette <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        do {
            p = &((u16 (*)[32])base)[y][x];
            for (col = width; col != 0; col--) {
                tile = *p;
                if (((tile >> 12) & 15) == 15) {
                    masked = tile & 0xffff0fff;
                    *p = masked | palette;
                }
                p++;
            }
            ((struct UiRenderWork *)base)->dirty |= 2 << ((u32)y >> 2);
            /* FAKEMATCH: a do-while(0) around the row count decrement keeps the
               reference register order */
            do {
                height--;
            } while (0);
            y++;
        } while (height != 0);
    }
}

void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    UiWindow_SetRectPalette(((struct UiWindow *)obj)->x + x + 1,
        ((struct UiWindow *)obj)->y + y + 1, width, height, palette);
}

/* The other editions keep their code here in their scaffolds for now. */

/* The Djinn help screen: seven topics, each with its explanation. Up and
   down or A step through them; B or Select returns -1 and Start -2, which
   also clears the menu's windows. */
s32 DjinnMenu_ShowHelp(void)
{
    s32 win_b;
    s32 win_a;
    struct ChooserMenu *menu;
    struct ChooserTextWork *work;
    s32 result;
    s32 previous;
    s32 selection;
    s32 list;
    struct ChooserMessage **slot;
    s32 message;
    s32 cnt;
    s32 list_message;
    void **slot_cells;

    /* The text work sits forty cells below the menu work. */
    slot_cells = (void **)&gMenuWork;
    menu = slot_cells[0];
    work = slot_cells[-40];
    result = 0;
    previous = 0;
    selection = 0;
    RenderOutput_ClearListFar(menu->option_window);
    WaitFrames(1);
    UiWindow_Clear(menu->message_window);
    message = (s32)&MsgDjinnInfoPrompt;
    UiText_DrawCharacterAtOffsetFar(message, menu->message_window, 0, 0);
    message++;
    UiText_DrawCharacterAtOffsetFar(message, menu->message_window, 0, 16);
    UiWindow_SetRectPalette(1, 1, 11, 3, 6);
    UiWindow_ApplyRectAtObjectOrigin((void *)menu->option_window, 0, 0, 28, 10, 6);
    list = UiWindow_CreateFar(0, 9, 8, 10, 6);
    win_b = UiWindow_CreateFar(8, 12, 22, 7, 2);
    win_a = UiWindow_CreateFar(8, 9, 22, 3, 2);
    UiWindow_MarkVisibleTileAttributesFar();
    cnt = 0;
    list_message = (s32)&MsgDjinnInfoTopic;
    do {
        UiText_DrawCharacterAtOffsetFar(cnt + list_message, list, 0, cnt * 8);
        cnt++;
    } while (cnt <= 6);
    do {
        UiWindow_Clear(win_a);
        UiText_DrawMessageAt(selection + (s32)&MsgDjinnInfoTopic, win_a, 0, 0);
        slot = (struct ChooserMessage **)UiText_OpenEntryMessageFar(win_b, selection + (s32)&MsgDjinnInfoText);
        Menu_DrawAtWindowOffset((void *)list, 0, previous, 6, 1, 15);
        Menu_DrawAtWindowOffset((void *)list, 0, selection, 6, 1, 14);
        previous = selection;
        for (;;) {
            UiMenu_PositionCursor(-12, (((struct UiWindow *)list)->y + selection) * 8 + 8);
            WaitFrames(1);
            if (gKeysRepeat & 0x90) {
                selection++;
                selection = Menu_GetModuloOfSum(selection, 7);
                Audio_PlayCue(111);
                break;
            } else if (gKeysRepeat & 0x60) {
                selection--;
                selection = Menu_GetModuloOfSum(selection, 7);
                Audio_PlayCue(111);
                break;
            } else if (gKeyState & 8) {
                Audio_PlayCue(113);
                result = -2;
                break;
            } else if (gKeyState & 6) {
                Audio_PlayCue(113);
                result = -1;
                break;
            } else if (gKeyState & 1) {
                if (UiWork_IsCompleteFar()) {
                    selection++;
                    selection = Menu_GetModuloOfSum(selection, 7);
                    Audio_PlayCue(112);
                    break;
                } else {
                    Audio_PlayCue(111);
                }
            }
        }
        if (work->resource != 99) {
            Resource_ResetEntry(work->resource);
            work->resource = 99;
        }
        ((struct ChooserTextWork *)gWindowWork[0])->text_busy = 0;
        UiWindow_Clear(win_b);
        {
            struct ChooserMessage *entry = *slot;
            entry->timer = 0;
            entry->count = 0;
            entry->flags = 0;
        }
        *slot = 0;
    } while (result == 0);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    RenderOutput_ClearListFar(win_a);
    RenderOutput_ClearListFar(win_b);
    WaitFrames(1);
    UiWork_FinalizeFar(win_a, 1);
    UiWork_FinalizeFar(list, 1);
    UiWork_FinalizeFar(win_b, 1);
    UiWindow_MarkVisibleTileAttributesFar();
    if (result == -2) {
        UiWindow_Clear(menu->message_window);
        UiWindow_Clear(menu->option_window);
        UiWindow_Clear(menu->owner_window);
        ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    }
    Scheduler_AddOrUpdateCallback((s32)(Menu_UpdateEntryObjectTransforms), 0xc80);
    return result;
}
