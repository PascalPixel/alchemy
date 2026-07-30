#include "layout_guard.h"
#include "types.h"

typedef struct DisplayObject_0801f200 {
    u8 padding00[8];
    u16 field_08;
    u16 field_0a;
    u16 field_0c;
} DisplayObject_0801f200;

typedef struct DisplayContext_0801f200 {
    DisplayObject_0801f200 *display;
    u16 saved_04;
    u16 saved_06;
    u16 saved_08;
    u16 saved_0a;
    u16 saved_mode;
} DisplayContext_0801f200;

typedef struct UiState_0801f200 {
    u8 padding000[0xea5];
    u8 feature_enabled;
    s8 busy;
    s8 draw_style;
} UiState_0801f200;

typedef struct Roster_0801f200 {
    u8 padding00[0x58];
    u16 ids[6];
} Roster_0801f200;

typedef struct Entry_0801f200 {
    u8 padding00[0x34];
    s16 maximum;
    s16 comparison_maximum;
    s16 current;
    s16 comparison_current;
} Entry_0801f200;

LAYOUT_OFFSET_GUARD(
    DisplayContext0801f200_Mode,
    DisplayContext_0801f200,
    saved_mode,
    0x0c);
LAYOUT_OFFSET_GUARD(
    UiState0801f200_Feature,
    UiState_0801f200,
    feature_enabled,
    0xea5);
LAYOUT_OFFSET_GUARD(
    Entry0801f200_Maximum,
    Entry_0801f200,
    maximum,
    0x34);
LAYOUT_OFFSET_GUARD(
    Roster0801f200_Ids,
    Roster_0801f200,
    ids,
    0x58);

s32 Func_080022ec(s32);
void Func_08016178(u16, u16, u16, u16);
void Func_08016498(void *);
void Func_080170f8(u16, u16, u16, u16);
void Func_08018efc(void *, s32, s32, s32);
void Func_08019000(void *, s32, s32, s32, s32);
void Func_0801e71c(s32);
void Func_0801e8b0(void *, void *, s32, s32);
void Func_0801ea3c(s16, void *, s32, s32, s32);
void Func_0801eea0(s32);
void Func_0801ef68(void *, s32);
void Func_0801f088(void *, s32, s32, s32);
Entry_0801f200 *Func_08077008(u16);
u32 Func_08077148(void);
u32 Func_080b50c8(s32);
s32 Func_080b5130(s32, u8 *);

/*
 * Rebuild the variable roster/value display. The feature-enabled layout reads
 * a sentinel-terminated halfword roster; the compact layout widens the active
 * byte roster before rendering the same primary and comparison values.
 */
void Func_0801f200(s32 mode)
{
    DisplayContext_0801f200 *context =
        *(DisplayContext_0801f200 **)0x03001e90;
    UiState_0801f200 *ui = *(UiState_0801f200 **)0x03001e8c;
    Roster_0801f200 *roster = *(Roster_0801f200 **)0x03001e74;
    DisplayObject_0801f200 *display = context->display;
    u16 ids[6];
    u32 count;
    u32 id_count = 0;
    s32 row_bias = 0;
    u16 options;
    s32 base_column;
    u32 i;

    if (ui->feature_enabled != 0) {
        count = Func_080b50c8(0);
        row_bias = -1;
        while (id_count < count && roster->ids[id_count] != 0xff) {
            ids[id_count] = roster->ids[id_count];
            id_count++;
        }
    } else {
        const u8 *source = (const u8 *)0x02000438;

        count = Func_08077148();
        while (id_count < count) {
            ids[id_count] = source[id_count];
            id_count++;
        }
        ids[id_count] = 0xff;
    }
    count = id_count;

    if (mode == -1)
        options = context->saved_mode;
    else
        options = (u16)mode;

    if ((options & 1) == 0)
        options &= (u16)~2;
    if (ui->feature_enabled == 0 || Func_080b5130(0, 0) == 0)
        options &= (u16)~2;

    if (options == 9) {
        Func_08016178(
            context->saved_04,
            context->saved_06,
            context->saved_08,
            context->saved_0a);
        return;
    }

    ui->busy = 1;
    if (context->saved_mode == options) {
        Func_08016498(display);
        Func_0801ef68(display, options);
    } else {
        Func_08016178(
            context->saved_04,
            context->saved_06,
            context->saved_08,
            context->saved_0a);
        Func_0801eea0(options);
        display->field_08 = context->saved_08;
        display->field_0a = context->saved_0a;
        display->field_0c = context->saved_04;
        Func_080170f8(
            context->saved_04,
            context->saved_06,
            context->saved_08,
            context->saved_0a);
        Func_0801ef68(display, options);
    }

    base_column = (options & 2) != 0 ? 5 : 0;
    for (i = 0; i < count; i++) {
        Entry_0801f200 *entry = Func_08077008(ids[i]);
        s32 x = base_column * 8 + (s32)i * 48;
        s32 primary_scale;

        if (entry->current == 0) {
            Func_0801e71c(2);
        } else if (entry->current <= entry->maximum / 4) {
            Func_0801e71c(4);
        } else {
            Func_0801e71c(15);
        }

        ui->draw_style = ui->feature_enabled != 0 ? 5 : 14;
        Func_0801ea3c(
            entry->current,
            display,
            x,
            row_bias * 8 + 8,
            0);
        ui->draw_style = 15;
        Func_0801e8b0(entry, display, x, row_bias * 8);
        Func_0801e71c(15);

        if (entry->maximum != 0) {
            primary_scale = Func_080022ec(entry->current * 40);
            if (primary_scale == 0 && entry->current != 0)
                primary_scale = 1;
            Func_0801f088(
                display,
                base_column + 1 + (s32)i * 6,
                row_bias + 2,
                primary_scale);
        }

        if ((options & 1) != 0) {
            s32 comparison_scale;

            ui->draw_style = ui->feature_enabled != 0 ? 5 : 14;
            Func_0801ea3c(
                entry->comparison_current,
                display,
                x,
                row_bias * 8 + 16,
                1);
            if (entry->comparison_maximum != 0) {
                comparison_scale =
                    Func_080022ec(entry->comparison_current * 40);
                if (comparison_scale == 0 &&
                    entry->comparison_current != 0) {
                    comparison_scale = 1;
                }
                Func_0801f088(
                    display,
                    base_column + 1 + (s32)i * 6,
                    row_bias + 3,
                    comparison_scale);
            }
        }
    }

    ui->draw_style = 15;
    if (ui->feature_enabled != 0 && (options & 2) != 0) {
        u8 digits[4];
        s32 extra_row = row_bias;

        if ((options & 1) != 0)
            extra_row++;
        Func_080b5130(0, digits);
        Func_08019000(display, 0x5001, 0, extra_row, 0);
        Func_08019000(display, 0x5002, 2, extra_row, 0);
        Func_08019000(display, 0x5003, 0, extra_row + 1, 0);
        Func_08019000(display, 0x5004, 2, extra_row + 1, 0);
        Func_08018efc(display, digits[0] + 0x30, 1, extra_row);
        Func_08018efc(display, digits[1] + 0x30, 3, extra_row);
        Func_08018efc(display, digits[2] + 0x30, 1, extra_row + 1);
        Func_08018efc(display, digits[3] + 0x30, 3, extra_row + 1);
    }
    ui->busy = 0;
}
