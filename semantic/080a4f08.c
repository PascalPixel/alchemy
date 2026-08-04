#include "layout_guard.h"
#include "types.h"

typedef struct DisplayObject_080a4f08 {
    u8 padding00[5];
    s8 mode;
} DisplayObject_080a4f08;

typedef struct SpriteObject_080a4f08 {
    u8 padding00[0x18];
    u16 attributes;
} SpriteObject_080a4f08;

typedef struct SelectionUi_080a4f08 {
    u8 padding000[0x10c];
    void *window;
    u8 padding110[0x68];
    u16 resource_selector;
    u8 padding17a[0xa0];
    u8 primary_character;
    u8 alternate_character;
    DisplayObject_080a4f08 *prompt_object;
} SelectionUi_080a4f08;

typedef struct DmaTransfer_080a4f08 {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer_080a4f08;

LAYOUT_OFFSET_GUARD(
    SpriteObject080a4f08_Attributes,
    SpriteObject_080a4f08,
    attributes,
    0x18);
LAYOUT_OFFSET_GUARD(
    SelectionUi080a4f08_Window,
    SelectionUi_080a4f08,
    window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    SelectionUi080a4f08_ResourceSelector,
    SelectionUi_080a4f08,
    resource_selector,
    0x178);
LAYOUT_OFFSET_GUARD(
    SelectionUi080a4f08_PrimaryCharacter,
    SelectionUi_080a4f08,
    primary_character,
    0x21a);
LAYOUT_OFFSET_GUARD(
    SelectionUi080a4f08_PromptObject,
    SelectionUi_080a4f08,
    prompt_object,
    0x21c);

extern SelectionUi_080a4f08 *Data_03001f2c;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

s32 Func_080022fc(s32, s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_08003fa4(s32, s32, void *);
s32 Func_08004080(void);
u8 *Func_080048f4(s32, s32);
void Func_08015080(s32, void *, s32, s32);
void Func_08015090(void *, void *, s32, s32);
void Func_080150b0(s32, s32, void *, s32, s32);
SpriteObject_080a4f08 *Func_080150c8(s32, u32, void *, s32, s32);
void Func_08015270(void *);
void Func_08015278(void *);
void *Func_08077008(u8);
s32 Func_080770c0(s32);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
s32 Func_080a3d9c(u8, s32);
void Func_080a4eb8(void);
void Func_080b0040(s32, s32, u8 *);
void Func_080f9010(s32);

/* Present the bounded quantity window and return its wrapped selection. */
s32 Func_080a4f08(
    s32 selection,
    s32 count,
    s32 hide_alternate)
{
    SelectionUi_080a4f08 *ui = Data_03001f2c;
    u8 *buffer = Func_080048f4(0x0e, 0x400);
    void *window = ui->window;
    s32 initial_selection = selection;
    s32 alternate_total = 0;
    s32 primary_total;
    s32 tile;
    s32 redraw = 1;

    Func_080a4eb8();
    Func_08015270(window);

    if (hide_alternate == 0) {
        alternate_total = Func_080a3d9c(
            ui->alternate_character,
            ui->resource_selector & 0x01ff);
    }
    primary_total = Func_080a3d9c(
        ui->primary_character,
        ui->resource_selector & 0x01ff);

    tile = Func_08004080();
    if (tile != 0x60) {
        SpriteObject_080a4f08 *right_marker;
        u16 attributes;

        Func_08003fa4(tile, 0x100, 0);
        Func_080150c8(tile, 0x40004000, window, 0x30, 0x20);
        right_marker = Func_080150c8(
            tile, 0x40004000, window, 0x50, 0x20);
        attributes = right_marker->attributes;
        right_marker->attributes =
            (attributes & 0xfc00) | ((attributes + 4) & 0x03ff);
        Func_080a1ac0(0x80, 0x28);

        for (;;) {
            if (Func_080770c0(0x150) != 0)
                break;

            if (redraw != 0) {
                volatile DmaTransfer_080a4f08 *dma =
                    (volatile DmaTransfer_080a4f08 *)0x040000d4;

                redraw = 0;
                selection = Func_080022fc(selection + count, count);
                Func_08015270(window);
                Func_08015080(0x0ade, window, 0x20, 0);

                dma->source = (const void *)0x080af08c;
                dma->destination = buffer;
                dma->control = 0x84000040;
                Func_080b0040(0x1e, 0x0e, buffer);
                Func_080b0040(count + selection, 0, buffer);
                Func_080b0040(
                    initial_selection + selection + 1, 0x0a, buffer);
                Func_080b0040(initial_selection, 2, buffer);
                Func_08003fa4(tile, 0x100, buffer);

                Func_080150b0(
                    selection + 1, 2, window, 0x20, 0x20);
                Func_08015080(
                    (ui->resource_selector & 0x01ff) + 0x182,
                    window,
                    0x10,
                    8);
                Func_080150b0(
                    primary_total - selection - 1,
                    2,
                    window,
                    0x10,
                    0x18);
                if (hide_alternate == 0) {
                    Func_080150b0(
                        alternate_total + selection + 1,
                        2,
                        window,
                        0x50,
                        0x18);
                }

                Func_08015090(
                    Func_08077008(ui->primary_character),
                    window,
                    0x10,
                    0x10);
                if (hide_alternate == 0) {
                    Func_08015090(
                        Func_08077008(ui->alternate_character),
                        window,
                        0x50,
                        0x10);
                }
            }

            if ((Data_03001c94 & 1) != 0) {
                Func_080f9010(0x70);
                break;
            }
            if ((Data_03001c94 & 2) != 0) {
                selection = -1;
                Func_080f9010(0x71);
                break;
            }

            Func_080a1a40(0x80, 0x28);
            if ((Data_03001b04 & 0x20) != 0) {
                selection--;
                redraw = 1;
                Func_080f9010(0x6f);
            }
            if ((Data_03001b04 & 0x10) != 0) {
                selection++;
                redraw = 1;
                Func_080f9010(0x6f);
            }
            Func_080030f8(1);
        }
    }

    Func_08015270(window);
    Func_08015278(window);
    Func_08002dd8(0x0e);
    ui->prompt_object->mode = 0x0d;
    if (Func_080770c0(0x150) != 0)
        selection = -1;
    return selection;
}
