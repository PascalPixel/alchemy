#include "layout_guard.h"
#include "types.h"

enum { OPTION_COUNT_080AB314 = 7 };

typedef struct MenuWindow_080ab314 {
    u8 padding00[0x0e];
    u16 row_offset;
} MenuWindow_080ab314;

typedef struct PreviewPane_080ab314 {
    u8 padding00[0x14];
    s16 field14;
    u8 padding16[2];
    s16 field18;
    s16 field1a;
} PreviewPane_080ab314;

typedef struct BattleMenu_080ab314 {
    u8 padding000[0x10];
    void *auxiliary_window;
    u8 padding014[0x1c];
    void *display;
    u8 padding034[0xd8];
    void *message_window;
} BattleMenu_080ab314;

typedef struct MenuRuntime_080ab314 {
    u8 padding000[0xea6];
    s8 drawing;
    u8 paddingea7[0x40f];
    u16 preview_resource;
    u8 padding12b8[0x40];
    s8 preview_active;
} MenuRuntime_080ab314;

LAYOUT_OFFSET_GUARD(
    MenuWindow080ab314_RowOffset,
    MenuWindow_080ab314,
    row_offset,
    0x0e);
LAYOUT_OFFSET_GUARD(
    PreviewPane080ab314_Field14,
    PreviewPane_080ab314,
    field14,
    0x14);
LAYOUT_OFFSET_GUARD(
    BattleMenu080ab314_Display,
    BattleMenu_080ab314,
    display,
    0x30);
LAYOUT_OFFSET_GUARD(
    BattleMenu080ab314_MessageWindow,
    BattleMenu_080ab314,
    message_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    MenuRuntime080ab314_Drawing,
    MenuRuntime_080ab314,
    drawing,
    0xea6);
LAYOUT_OFFSET_GUARD(
    MenuRuntime080ab314_PreviewResource,
    MenuRuntime_080ab314,
    preview_resource,
    0x12b6);
LAYOUT_OFFSET_GUARD(
    MenuRuntime080ab314_PreviewActive,
    MenuRuntime_080ab314,
    preview_active,
    0x12f8);

extern BattleMenu_080ab314 *Data_03001f2c;
extern MenuRuntime_080ab314 *Data_03001e8c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

void Func_080030f8(s32);
void Func_08003f3c(u16);
void Func_080041d8(const void *, s32);
MenuWindow_080ab314 *Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(void *, s32);
s32 Func_08015048(void);
void Func_08015060(void *);
void Func_08015078(s32, void *, s32, s32);
void Func_08015080(s32, void *, s32, s32);
void Func_08015278(void *);
void Func_080152a8(void);
PreviewPane_080ab314 **Func_080153f8(void *, s32);
void Func_080a1a40(s32, s32);
s32 Func_080aa538(s32, s32);
void Func_080ab1f4(void *, s32, s32, s32, s32, s32);
void Func_080ab21c(s32, s32, s32, s32, s32);
void Func_080ab2ec(void *, s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static void ReleasePreview_080ab314(
    MenuRuntime_080ab314 *runtime,
    void *preview_window,
    PreviewPane_080ab314 **preview_slot)
{
    PreviewPane_080ab314 *pane;

    if (runtime->preview_resource != 0x63) {
        Func_08003f3c(runtime->preview_resource);
        runtime->preview_resource = 0x63;
    }
    runtime->preview_active = 0;
    Func_08015060(preview_window);

    pane = *preview_slot;
    pane->field1a = 0;
    pane->field18 = 0;
    pane->field14 = 0;
    *preview_slot = 0;
}

/* Run the seven-entry menu; return -1 for cancel or -2 for alternate exit. */
s32 Func_080ab314(void)
{
    BattleMenu_080ab314 *ui = Data_03001f2c;
    MenuRuntime_080ab314 *runtime = Data_03001e8c;
    MenuWindow_080ab314 *rows;
    void *preview_window;
    void *cursor_window;
    s32 result = 0;
    s32 selection = 0;
    s32 previous = 0;
    s32 index;

    Func_08015278(ui->display);
    Func_080030f8(1);
    Func_08015060(ui->message_window);
    Func_08015080(0x0c30, ui->message_window, 0, 0);
    Func_08015080(0x0c31, ui->message_window, 0, 0x10);
    Func_080ab21c(1, 1, 0x0b, 3, 6);
    Func_080ab2ec(ui->display, 0, 0, 0x1c, 0x0a, 6);

    rows = Func_08015010(0, 9, 8, 0x0a, 6);
    preview_window = Func_08015010(8, 0x0c, 0x16, 7, 2);
    cursor_window = Func_08015010(8, 9, 0x16, 3, 2);
    Func_080152a8();

    for (index = 0; index < OPTION_COUNT_080AB314; index++)
        Func_08015080(0x0c32 + index, rows, 0, index * 8);

    do {
        PreviewPane_080ab314 **preview_slot;

        Func_08015060(cursor_window);
        Func_08015078(0x0c32 + selection, cursor_window, 0, 0);
        preview_slot = Func_080153f8(
            preview_window, 0x0c39 + selection);
        Func_080ab1f4(rows, 0, previous, 6, 1, 0x0f);
        Func_080ab1f4(rows, 0, selection, 6, 1, 0x0e);
        previous = selection;

        for (;;) {
            Func_080a1a40(
                -0x0c,
                (rows->row_offset + selection) * 8 + 8);
            Func_080030f8(1);

            if ((Data_03001b04 & 0x90) != 0) {
                selection = Func_080aa538(
                    selection + 1, OPTION_COUNT_080AB314);
                Func_080f9010(0x6f);
                break;
            }
            if ((Data_03001b04 & 0x60) != 0) {
                selection = Func_080aa538(
                    selection - 1, OPTION_COUNT_080AB314);
                Func_080f9010(0x6f);
                break;
            }
            if ((Data_03001c94 & 8) != 0) {
                Func_080f9010(0x71);
                result = -2;
                break;
            }
            if ((Data_03001c94 & 6) != 0) {
                Func_080f9010(0x71);
                result = -1;
                break;
            }
            if ((Data_03001c94 & 1) != 0) {
                if (Func_08015048() != 0) {
                    selection = Func_080aa538(
                        selection + 1, OPTION_COUNT_080AB314);
                    Func_080f9010(0x70);
                    break;
                }
                Func_080f9010(0x6f);
            }
        }

        ReleasePreview_080ab314(
            runtime, preview_window, preview_slot);
    } while (result == 0);

    runtime->drawing = 1;
    Func_08015278(cursor_window);
    Func_08015278(preview_window);
    Func_080030f8(1);
    Func_08015018(cursor_window, 1);
    Func_08015018(rows, 1);
    Func_08015018(preview_window, 1);
    Func_080152a8();

    if (result == -2) {
        Func_08015060(ui->message_window);
        Func_08015060(ui->display);
        Func_08015060(ui->auxiliary_window);
        runtime->drawing = 0;
    }

    Func_080041d8((const void *)0x080a19a1, 0x0c80);
    return result;
}
