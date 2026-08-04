#include "types.h"

extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001b04;

void Func_080030f8(u32 frames);
void Func_08004760(void);
void Func_0800479c(void);
void Func_080052f4(s32 source, s32 destination);

enum PaletteComponent_0808d0c8 {
    PALETTE_RED_0808d0c8 = 1,
    PALETTE_GREEN_0808d0c8,
    PALETTE_BLUE_0808d0c8,
};

static volatile u16 *const Palette_0808d0c8 = (volatile u16 *)0x05000000;
static volatile u16 *const PaletteGrid_0808d0c8 = (volatile u16 *)0x0600205A;

static void DrawPaletteGrid_0808d0c8(s32 bank) {
    s32 color_index;
    s32 palette_offset = bank * 16;

    PaletteGrid_0808d0c8[0] = (u16)(0xF0E0 + bank);
    PaletteGrid_0808d0c8[32] = 0xF052;
    PaletteGrid_0808d0c8[64] = 0xF047;
    PaletteGrid_0808d0c8[96] = 0xF042;

    for (color_index = 1; color_index <= 15; color_index++) {
        u16 color = Palette_0808d0c8[palette_offset + color_index];

        PaletteGrid_0808d0c8[color_index] =
            (u16)((bank << 12) + 0xD0 + color_index);
        PaletteGrid_0808d0c8[32 + color_index] =
            (u16)(0xF0E0 + (color & 0x1F));
        PaletteGrid_0808d0c8[64 + color_index] =
            (u16)(0xF0E0 + ((color >> 5) & 0x1F));
        PaletteGrid_0808d0c8[96 + color_index] =
            (u16)(0xF0E0 + ((color >> 10) & 0x1F));
    }
}

static void AdjustPaletteColor_0808d0c8(
    s32 bank,
    s32 color_index,
    enum PaletteComponent_0808d0c8 component,
    s32 amount
) {
    volatile u16 *color_address = &Palette_0808d0c8[bank * 16 + color_index];
    u16 color = *color_address;
    u32 red = color & 0x1F;
    u32 green = (color >> 5) & 0x1F;
    u32 blue = (color >> 10) & 0x1F;

    if (component == PALETTE_RED_0808d0c8) {
        if ((amount > 0 && red < 31) || (amount < 0 && red != 0)) {
            red += amount;
        }
    } else if (component == PALETTE_GREEN_0808d0c8) {
        if ((amount > 0 && green < 31) || (amount < 0 && green != 0)) {
            green += amount;
        }
    } else if ((amount > 0 && blue < 31) || (amount < 0 && blue != 0)) {
        blue += amount;
    }

    *color_address = (u16)((blue << 10) | (green << 5) | red);
}

static void PreviewPaletteColor_0808d0c8(s32 bank, s32 color_index) {
    volatile u16 *color_address = &Palette_0808d0c8[bank * 16 + color_index];
    u16 original_color = *color_address;

    while (Data_03001ae8 & 8) {
        u32 phase;

        for (phase = 0; phase <= 39; phase++) {
            Func_080030f8(1);
            if (!(Data_03001ae8 & 8)) {
                *color_address = original_color;
                return;
            }

            if (phase == 0) {
                *color_address = 0x7FFF;
            } else if (phase == 10 || phase == 30) {
                *color_address = original_color;
            } else if (phase == 20) {
                *color_address = 0;
            }
        }
    }

    *color_address = original_color;
}

/*
 * Interactive palette editor. Draw the selected palette as an RGB grid,
 * navigate its bank/index/component, edit one five-bit channel at a time,
 * and flash-preview the selected colour until the user exits.
 */
void Func_0808d0c8(void) {
    s32 bank = 0;
    s32 color_index = 1;
    enum PaletteComponent_0808d0c8 component = PALETTE_RED_0808d0c8;

    Func_080052f4(0x0809E4CE, 0x06001A00);
    DrawPaletteGrid_0808d0c8(bank);
    Func_080030f8(1);

    for (;;) {
        u32 keys = Data_03001b04;

        if (keys & 0x40) {
            component--;
            if (component < PALETTE_RED_0808d0c8) {
                component = PALETTE_BLUE_0808d0c8;
            }
        }
        if (keys & 0x80) {
            component++;
            if (component > PALETTE_BLUE_0808d0c8) {
                component = PALETTE_RED_0808d0c8;
            }
        }
        if (keys & 0x20) {
            color_index--;
            if (color_index == 0) {
                color_index = 15;
            }
        }
        if (keys & 0x10) {
            color_index++;
            if (color_index > 15) {
                color_index = 1;
            }
        }

        if (keys & 0x200) {
            bank--;
            if (bank < 0) {
                bank = 13;
            }
            DrawPaletteGrid_0808d0c8(bank);
            Func_080030f8(1);
            continue;
        }
        if (keys & 0x100) {
            bank++;
            if (bank > 13) {
                bank = 0;
            }
            DrawPaletteGrid_0808d0c8(bank);
            Func_080030f8(1);
            continue;
        }
        if (keys & 1) {
            AdjustPaletteColor_0808d0c8(bank, color_index, component, 1);
            DrawPaletteGrid_0808d0c8(bank);
            Func_080030f8(1);
            continue;
        }
        if (keys & 2) {
            AdjustPaletteColor_0808d0c8(bank, color_index, component, -1);
            DrawPaletteGrid_0808d0c8(bank);
            Func_080030f8(1);
            continue;
        }
        if (keys & 8) {
            PreviewPaletteColor_0808d0c8(bank, color_index);
        }

        if (Data_03001b04 & 4) {
            break;
        }
        Func_080030f8(1);
    }

    Func_0800479c();
    Func_08004760();
}
