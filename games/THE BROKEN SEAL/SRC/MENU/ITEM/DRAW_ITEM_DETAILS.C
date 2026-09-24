#include "TYPES.H"

/* The detail panel of the item menu: whether the item does anything, its
   attack and defense bonuses, one row per effect (stat changes, element
   power and resistance, critical rates, special effects), then use,
   consumption, curse and repair notes. */
#include "ITEM.H"

extern u8 Value_00000b6d[], Value_00000af7[], Value_00000af8[], Value_00000b3b[];
extern u8 Value_00000b76[], Value_00000b65[], Value_00000b6e[], Value_00000b63[];
extern u8 Value_00000b73[], Value_00000b71[], Value_00000b6f[];
extern u8 Value_00000b69[], Value_00000b6a[], Value_00000b6c[];
extern u8 Data_080af21c[], Data_080af220[];

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 palette);
void UiWork_PushValueSlotFar(u32 value, u32 slot);
void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);

void ItemMenu_DrawItemDetails(s32 window, s32 item)
{
    struct ItemDefinition *def;
    s32 i;
    s32 found;
    s32 amount;
    s32 consumable;
    s8 row;

    row = 0;
    consumable = 0;
    def = Item_Get(item & 0x1ff);
    if (def->type != 0) {
        found = 0;
        /* FAKEMATCH: the two bonus fields are tested as one masked word. */
        if ((*(u32 *)&def->primary_bonus & 0xffffff) == 0) {
            for (i = 0; i < 4; i++) {
                if (def->effects[i].kind != 0 || def->use_type == 3) {
                    found = 1;
                    break;
                }
            }
        } else {
            found = 1;
        }
        if (found == 1) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000b6d, window, 16, 0);
            row = 1;
        }
        if (def->primary_bonus != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000af7, window, 0, row * 8);
            amount = def->primary_bonus;
            ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
            row++;
        }
        if (def->secondary_bonus != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000af8, window, 0, row * 8);
            amount = def->secondary_bonus;
            ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
            row++;
        }
    }
    for (i = 0; i < 4; i++) {
        if (def->effects[i].kind == 0)
            continue;
        amount = def->effects[i].amount;
        switch (def->effects[i].kind) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 26:
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)Value_00000b3b, window, 0, row * 8);
            ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
            break;
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
            UiWindow_SetTilemapEntryFar(window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)Value_00000b3b, window, 8, row * 8);
            ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)Value_00000b3b, window, 0, row * 8);
            UiText_DrawStringInWindowFar(Data_080af21c, window, 64, row * 8);
            if (amount > 9) {
                UiText_DrawNumberInWindowFar(1, 1, window, 72, row * 8);
                UiText_DrawStringInWindowFar(Data_080af220, window, 80, row * 8);
                UiText_DrawNumberInWindowFar(amount - 10, 1, window, 88, row * 8);
            } else {
                UiText_DrawNumberInWindowFar(0, 1, window, 72, row * 8);
                UiText_DrawStringInWindowFar(Data_080af220, window, 80, row * 8);
                UiText_DrawNumberInWindowFar(amount, 1, window, 88, row * 8);
            }
            break;
        case 23:
        case 25:
        case 27:
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)Value_00000b3b, window, 0, row * 8);
            break;
        }
        row++;
    }
    if (def->flags & 1) {
        UiText_DrawCharacterAtOffsetFar((s32)Value_00000b76, window, 0, row * 8);
        row++;
    }
    if (def->use_type == 3) {
        UiText_DrawCharacterAtOffsetFar((s32)Value_00000b65, window, 0, row * 8);
        consumable = 1;
        row++;
    }
    if (def->use_type != 4 && def->use_type != 0) {
        if (!consumable) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000b6e, window, 16, row * 8);
            row++;
        }
        switch (def->use_type) {
        case 0:
            break;
        case 1:
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000b63, window, 0, row * 8);
            row++;
            break;
        case 2: {
            s32 m;

            if (item & 0x400) {
                m = (s32)Value_00000b73;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
                m++;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
            } else {
                m = (s32)Value_00000b71;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
                m++;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
            }
            break;
        }
        }
    }
    if (def->flags & 16) {
        if (row != 0)
            row++;
        i = (s32)Value_00000b6f;
        UiText_DrawCharacterAtOffsetFar(i, window, 16, row * 8);
        row++;
        amount = (item & 0xf800) / 2048;
        UiWork_PushValueSlotFar(amount + 1, 5);
        UiText_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
        row++;
    }
    found = 0;
    if (row == 0) {
        if (def->flags & 4) {
            UiText_DrawCharacterAtOffsetFar((s32)Value_00000b69, window, 0, 0);
            found = 1;
        }
        if (!found) {
            if (def->flags & 8) {
                UiText_DrawCharacterAtOffsetFar((s32)Value_00000b6a, window, 0, 0);
                found = 1;
            }
            if (!found)
                UiText_DrawCharacterAtOffsetFar((s32)Value_00000b6c, window, 0, 0);
        }
    }
}
