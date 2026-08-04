#include "types.h"

struct Window_08020198 {
    s32 field_00;
    s32 field_04;
    u16 field_08;
    u16 field_0a;
    u16 x;
    u16 y;
    u16 field_10;
    u16 field_12;
    u16 field_14;
    u16 flags;
    u16 field_18;
    u16 field_1a;
    u16 field_1c;
    u16 field_1e;
    u16 field_20;
    u16 field_22;
};

struct SaveSummary_08020198 {
    u8 unknown_00[0x10];
    u8 name[12];
    u8 level_or_present;
    u8 glyph_index;
    u8 unknown_1e[2];
    s32 play_time_ticks;
    s32 amount;
    u8 unknown_28[0x18];
};

void Func_08016478(struct Window_08020198 *window);
void Func_0801e41c(struct Window_08020198 *window, s32 left, s32 top,
                   s32 right, s32 bottom);
void Func_0801e8b0(u8 *text, struct Window_08020198 *window, s32 offset_x,
                   s32 offset_y);
void Func_0801e940(u8 *text, struct Window_08020198 *window, u32 offset_x,
                   u32 offset_y);
void Func_0801e9d4(s32 value, s32 digits, struct Window_08020198 *window,
                   s32 offset_x, s32 offset_y);
void Func_0801e7c0(s32 character, struct Window_08020198 *window,
                   u32 offset_x, u32 offset_y);
void *Func_0801f680(s32 value, u8 *output);
void Func_0801ea08(s32 value, s32 digits, struct Window_08020198 *window,
                   s32 offset_x, s32 offset_y);

void Func_08020198(struct Window_08020198 *window,
                   struct SaveSummary_08020198 *summary)
{
    u8 time_text[16];

    if (window == 0)
        return;

    Func_08016478(window);
    Func_0801e41c(window, 0, 4, 13, 4);
    Func_0801e8b0(summary->name, window, 0, 0);
    Func_0801e940((u8 *)0x080371e0, window, 72, 0);
    Func_0801e9d4(summary->level_or_present, 2, window, 80, 0);
    Func_0801e7c0(summary->glyph_index + 0x741, window, 0, 16);
    Func_0801e7c0(9, window, 0, 32);
    Func_0801e940(Func_0801f680(summary->play_time_ticks, time_text),
                  window, 48, 40);
    Func_0801ea08(summary->amount, 6, window, 0, 48);
    Func_0801e7c0(0xc88, window, 48, 48);
}
