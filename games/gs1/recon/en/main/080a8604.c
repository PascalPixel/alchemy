#include "types.h"
#include "inventory_menu.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern u8 *Runtime_GetObject(s32 owner);
extern s32 Func_08077290(s32);
extern s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id);
extern s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled);
extern void InventoryMenu_ApplyCategoryItemFlags(u8 *entries);
extern void Func_080a8914(s32 window, s32 owner, s32 mode);
extern void Func_08015068(s32 window, s32 a, s32 b, s32 c, s32 d);
extern void Func_08015090(s32 image, s32 window, s32 x, s32 y);
extern void Func_08015098(u8 *glyph, s32 window, s32 x, s32 y);
extern void Func_08015280(s32 window, s32 tile, s32 x, s32 y, s32 style);
extern s32 Func_080771f8(s32 owner, s32 index);

extern u8 Value_00000bd4;
extern u8 Value_00000bd5;
extern u8 Value_00000bd6;
extern u8 Value_00000bd7;
extern u8 Value_00000bd8;
extern u8 Value_00000bd9;
extern u8 Value_00000afd;
extern u8 Value_00000afe;
extern u8 Data_080af22c;
extern u8 Data_080af230;

void Func_080a8604(s32 window, s32 owner, s32 mode)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    u8 *object;
    s32 compare_mode;
    s32 row_y;
    u8 entries[8];
    s32 skip_border;
    s32 label_count;
    s32 party_i;
    s32 category;
    s32 base72;
    s32 base104;
    s32 base120;
    u8 *base160;
    s32 row8;
    s32 row16;
    s32 avail;

    avail = Func_08077290(-1);
    compare_mode = (u32)(-avail | avail) >> 31;
    object = Runtime_GetObject(owner);

    row_y = 7;
    if ((mode & 0xFF) != 1)
        row_y = 10;

    menu->cursor->state = 1;

    Func_080a8914(window, owner, mode);

    CharacterMenu_BuildAvailability(entries, 1, owner);
    CharacterMenu_UpdateSelectionIcons(entries);

    skip_border = mode & 0x100;
    if (skip_border == 0)
        Func_08015068(window, 0, 40, 96, 96);

    label_count = 0;
    if (entries[0] != 0) {
        UiText_DrawAt((s32)&Value_00000bd5, window, 16, 40);
        label_count = 1;
    }
    if (entries[1] != 0) {
        UiText_DrawAt(
            (s32)&Value_00000bd6, window, 16, label_count * 16 + 40);
        label_count++;
    }
    if (entries[2] != 0) {
        UiText_DrawAt(
            (s32)&Value_00000bd7, window, 16, label_count * 16 + 40);
        label_count++;
    }
    if (entries[3] != 0) {
        UiText_DrawAt(
            (s32)&Value_00000bd8, window, 16, label_count * 16 + 40);
        label_count++;
    }
    if (entries[4] != 0) {
        UiText_DrawAt(
            (s32)&Value_00000bd9, window, 16, label_count * 16 + 40);
        label_count++;
    }
    if (label_count == 0)
        UiText_DrawAt((s32)&Value_00000bd4, window, 0, 40);

    CharacterMenu_UpdateSelectionIcons(entries);
    InventoryMenu_ApplyCategoryItemFlags(entries);

    category = M2C_FIELD(menu, u16 *, 0x220);
    if (category == 3)
        return;

    if (skip_border == 0) {
        WaitFrames(1);
        Func_08015068(window, 64, 56, 224, 96);
    }

    UiPalette_SetColor(15);

    if (mode == 1 || compare_mode == 1) {
        Func_08015280(window, 1, 15, row_y, 4);
        Func_08015280(window, 2, 19, row_y, 4);
        Func_08015280(window, 3, 23, row_y, 4);
        Func_08015280(window, 4, 27, row_y, 4);
    }

    if (compare_mode != 0)
        UiText_DrawAt((s32)&Value_00000afd, window, 64, row_y * 8 + 8);

    if (mode == 1) {
        if (compare_mode == 0)
            row_y--;

        Func_08015090((s32)&Data_080af22c, window, 64, row_y * 8 + 16);
        UiText_DrawAt((s32)&Value_00000afe, window, 64, row_y * 8 + 24);
        UiText_DrawAt((s32)(&Value_00000afe + 1), window, 64, row_y * 8 + 32);
    }

    row8 = row_y * 8;
    row16 = row8 + 8;
    base104 = 104;
    base120 = 120;
    base72 = (s32)(object + 72);
    base160 = object + 280;

    for (party_i = 0; party_i <= 3; party_i++) {
        if (compare_mode != 0)
            UiNumber_DrawAt(base160[0], 1, window, base120, row16);

        if ((mode & 0xFF) == 1) {
            if (compare_mode != 0) {
                UiNumber_DrawAt(base160[4], 1, window, base104, row16);
                Func_08015098(&Data_080af230, window, base120 - 8, row16);
            } else {
                row8 = row_y * 8;
            }

            UiNumber_DrawAt(
                Func_080771f8(owner, party_i), 2, window, base120 - 8,
                row8 + 16);

            UiNumber_DrawAt(
                *(s16 *)(base72 + 0), 3, window, base104, row8 + 24);
            UiNumber_DrawAt(
                *(s16 *)(base72 + 2), 3, window, base104, row8 + 32);
        }

        base72 += 4;
        base104 += 32;
        base120 += 32;
        base160 += 1;
    }
}
