#include "types.h"
#include "scene.h"
#include "gs1_edition.h"

/* menu/draw/draw_selection_row.c */
struct Work;

void RenderOutput_PrepareForRedraw(void *);
void UiText_DrawNumber(s32 n, s32 digits, s32 work, s32 x, s32 y);
void UiText_DrawString(u8 *str, s32 work, s32 x, s32 y);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);
extern u8 Value_0000099b[];
extern u8 Value_00000a07[];
extern u8 gRom[];
void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second)
{
    s16 selected = first;
    s32 label = Menu_Apply(selected, *second) + (s32)Value_0000099b;
    RenderOutput_PrepareForRedraw(work);
    UiText_DrawNumber(selected, 3, (s32)work, 0, 14);
    UiText_DrawNumber(*second, 3, (s32)work, MENU_LABEL_X, 14);
    UiText_DrawString(gRom, (s32)work, MENU_TEXT_X, 0);
    UiText_DrawResource(selected + (s32)Value_00000a07, (s32)work, 0, 0);
    UiText_DrawString(gRom, (s32)work, MENU_TEXT_X, 14);
    UiText_DrawResource(label, (s32)work, MENU_LABEL_X, 0);
}

/* menu/sel/run_selection.c */
struct MenuDefaults {
    u8 padding_000[0x1C8];
    s16 primary;
    s16 secondary;
};

struct TextObject {
    u8 storage[12];
};

struct Work;

extern struct MenuDefaults gCell;
extern volatile u32 gIw;

struct Work *UiWindow_Create(s32 kind, s32 x, s32 y, s32 width, s32 layer);
void Menu_DrawSelectionRow(
    struct Work *work, s16 primary, const s16 *secondary);

void WaitFrames(s32 frames);

void UiWork_Finalize(struct Work *work, s32 release);

s16 Menu_RunSelection(void)
{
    s32 resource;
    s16 mode;
    s16 secondary;
    struct TextObject object;
    s16 primary;
    struct Work *work;
    s16 result;

    work = 0;
    mode = 0;
    primary = gCell.primary;
    secondary = gCell.secondary;
    work = UiWindow_Create(0, 7, 30, 5, 2);
    Menu_DrawSelectionRow(work, primary, &secondary);
    Menu_Apply(&object, &resource);

    while (gIw != 0)
        WaitFrames(1);

    for (;;) {
        result = Menu_SetMode(work, primary, &secondary, &mode);
        if (result == -1) {
            TextResource_Release(resource);
            UiWork_Finalize(work, 2);
            Menu_Apply2(primary, secondary);
            return result;
        }
        if (result == -2) {
            TextResource_Release(resource);
            UiWork_Finalize(work, 2);
            return result;
        }

        Menu_Place(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
        primary = result;
        WaitFrames(1);
    }
}

/* menu/sel/run_selection_with_cursor_object.c */
s32 UiWindow_Create(s32, s32, s32, s32, s32);

void WaitFrames(s32);

void UiWork_Finalize(struct Work *work, s32 release);

s32 Menu_RunSelectionWithCursorObject(void)
{
    u8 obj[12];
    s32 pos[2];
    s32 res;
    s32 sel;
    s32 win;
    s32 ev;

    sel = 8;
    win = 0;
    pos[0] = win;
    pos[1] = win;
    win = UiWindow_Create(1, 0, 28, 20, 2);
    Menu_Apply(win, sel);
    Menu_Apply2(obj, &res);
    goto loop_test;
loop_body:
    if (ev == 1)
        Menu_Apply(win, *(volatile s32 *)&sel);
    Menu_Place(obj,
                  pos[0] * 8 + 58,
                  pos[1] * 8 + 20);
loop_test:
    WaitFrames(1);
    ev = Menu_PollFlagGrid(win, &sel, pos);
    if (ev != -1)
        goto loop_body;
    TextResource_Release(res);
    UiWork_Finalize(win, 2);
    return 0;
}

/* menu/draw/draw_flag_bit_table.c */
void Menu_DrawFlagBitTable(s32 window, s32 start_flag)
{
    s32 row;
    s32 y;
    s32 flag;
    char label[5];
    char bits[17];

    RenderOutput_PrepareForRedraw();
    Menu_SetMode(0x0803742c, window, 48, 0);

    flag = start_flag << 8;
    for (row = 0; row != 16; row++) {
        s32 i;

        y = row * 8 + 16;

        for (i = 0; i != 5; i++) {
            label[i] = 0;
        }
        Menu_Place(flag, 3, (s32)label);
        Menu_SetMode((s32)label, window, 0, y);
        Menu_SetMode(0x08037428, window, 32, y);

        for (i = 0; i < 16; i++) {
            s32 val = GameFlag_IsSet(flag);
            bits[i] = (val != 0) + 48;
            flag++;
        }
        bits[i] = 0;
        Menu_SetMode((s32)bits, window, 48, y);
    }
}

/* menu/status/reserved_no_op.c */
void Menu_ReservedNoOp294D0(void)
{
}
