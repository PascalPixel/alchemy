/* Draft: complete 720-byte owner and literal pools; candidate 720 bytes,
   19 differing halfwords (18 aligned edits). Typed menu, text-work, window
   and message records recover the nested loops. Remaining differences are
   argument setup, register allocation and scheduling; allocator inspected. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"

struct ChooserMenu {
    u8 reserved_000[0x10];
    s32 owner_window;
    u8 reserved_014[0x1c];
    s32 option_window;
    u8 reserved_034[0xd8];
    s32 message_window;
};

struct ChooserTextWork {
    u8 reserved_000[0xea6];
    u8 menu_busy;
    u8 reserved_ea7[0x40f];
    u16 resource;
    u8 reserved_12b8[0x40];
    u8 text_busy;
};

struct ChooserWindow {
    u8 reserved_00[14];
    u16 row;
};

struct ChooserMessage {
    struct ChooserMessage *next;
    u8 reserved_04[16];
    u16 flags;
    u16 reserved_16;
    u16 timer;
    u16 count;
};

extern struct ChooserMenu *Data_03001f2c;
extern struct ChooserTextWork *Data_03001e8c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;
extern char Value_00000c30;
extern char Value_00000c32;
void RenderOutput_ClearListFar(s32 window);
void Func_08015060(s32 window);
void Func_080152a8(void);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
struct ChooserMessage **Func_080153f8(s32 window, s32 message);
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiWindow_ApplyRectAtObjectOrigin(s32 window, s32 x, s32 y, s32 width, s32 height, s32 palette);
void Menu_DrawAtWindowOffset(s32 window, s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Menu_GetModuloOfSum(s32 value, s32 modulus);
void Audio_PlayCue(s32 cue);
s32 Resource_ResetEntry(u32 resource);
void UiWork_FinalizeFar(s32 window, s32 mode);
void Menu_UpdateEntryObjectTransforms(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

/* Show seven messages, permitting cursor movement and two exit actions. */
s32 Func_080ab314(void)
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

    menu = Data_03001f2c;
    /* Adjacent runtime cells hold the text work and current menu work. */
    work = *(struct ChooserTextWork **)((u8 *)&Data_03001f2c - 0xa0);
    result = 0;
    previous = 0;
    selection = 0;
    RenderOutput_ClearListFar(menu->option_window);
    WaitFrames(1);
    Func_08015060(menu->message_window);
    message = (s32)&Value_00000c30;
    UiText_DrawCharacterAtOffsetFar(message, menu->message_window, 0, 0);
    message++;
    UiText_DrawCharacterAtOffsetFar(message, menu->message_window, 0, 16);
    UiWindow_SetRectPalette(1, 1, 11, 3, 6);
    UiWindow_ApplyRectAtObjectOrigin(menu->option_window, 0, 0, 28, 10, 6);
    list = UiWindow_CreateFar(0, 9, 8, 10, 6);
    win_b = UiWindow_CreateFar(8, 12, 22, 7, 2);
    win_a = UiWindow_CreateFar(8, 9, 22, 3, 2);
    Func_080152a8();
    cnt = 0;
    list_message = (s32)&Value_00000c32;
    do {
        UiText_DrawCharacterAtOffsetFar(cnt + list_message, list, 0, cnt * 8);
        cnt++;
    } while (cnt <= 6);
    do {
        Func_08015060(win_a);
        Func_08015078(selection + (s32)&Value_00000c32, win_a, 0, 0);
        slot = Func_080153f8(win_b, selection + 0xc39);
        Menu_DrawAtWindowOffset(list, 0, previous, 6, 1, 15);
        Menu_DrawAtWindowOffset(list, 0, selection, 6, 1, 14);
        previous = selection;
        for (;;) {
            UiMenu_PositionCursor(-12, (((struct ChooserWindow *)list)->row + selection) * 8 + 8);
            WaitFrames(1);
            if (Data_03001b04 & 0x90) {
                selection++;
                selection = Menu_GetModuloOfSum(selection, 7);
                Audio_PlayCue(111);
                break;
            } else if (Data_03001b04 & 0x60) {
                selection--;
                selection = Menu_GetModuloOfSum(selection, 7);
                Audio_PlayCue(111);
                break;
            } else if (Data_03001c94 & 8) {
                Audio_PlayCue(113);
                result = -2;
                break;
            } else if (Data_03001c94 & 6) {
                Audio_PlayCue(113);
                result = -1;
                break;
            } else if (Data_03001c94 & 1) {
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
        Data_03001e8c->text_busy = 0;
        Func_08015060(win_b);
        {
            struct ChooserMessage *entry = *slot;
            /* FAKEMATCH: source swap compensates the scheduler's tied
               halfword stores while preserving their independent values. */
            entry->timer = 0;
            entry->count = 0;
            entry->flags = 0;
        }
        *slot = 0;
    } while (result == 0);
    Data_03001e8c->menu_busy = 1;
    RenderOutput_ClearListFar(win_a);
    RenderOutput_ClearListFar(win_b);
    WaitFrames(1);
    UiWork_FinalizeFar(win_a, 1);
    UiWork_FinalizeFar(list, 1);
    UiWork_FinalizeFar(win_b, 1);
    Func_080152a8();
    if (result == -2) {
        Func_08015060(menu->message_window);
        Func_08015060(menu->option_window);
        Func_08015060(menu->owner_window);
        Data_03001e8c->menu_busy = 0;
    }
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
    return result;
}
