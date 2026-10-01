#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"
/* The detail panel of the item menu: whether the item does anything, its
   attack and defense bonuses, one row per effect (stat changes, element
   power and resistance, critical rates, special effects), then use,
   consumption, curse and repair notes. */
#include "ITEM.H"

#define MENU_SUBOBJECT(menu, offset) (*(u8 **)((u8 *)(menu) + (offset)))
struct InventoryMenuState;
extern struct InventoryMenuState *gMenuWork;
extern volatile s32 gKeysRepeat;
extern volatile u32 gKeyState;
void Menu_UpdateEntryObjectTransforms(void);
void Palette_CopyObjectBankToBackground14(void);
s32 GameFlag_TestFar(s32);
void ItemMenu_DrawItemDetails(s32, s32);
s32 RenderOutput_RedrawSavedRectFar(s32);
void UiWork_FinalizeFar(s32, s32);
void Palette_LightenBankHighlight(s32);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);

extern u8 MsgEquipEffectHeading[], MsgStatLabel[], MsgDefenseLabel[], MsgEquipEffectName[];
extern u8 MsgItemCursed[], MsgBestowsPsynergy[], MsgUsesHeading[], MsgSingleUse[];
extern u8 MsgBrokenNotice[], MsgMightBreak[], MsgNumberHeading[], MsgEffectRateSuffix[];
extern u8 MsgRareItem[], MsgImportantItem[], MsgDetailsUnknown[];
extern u8 Data_080af21c[], Data_080af220[];
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 palette);
void UiWork_PushValueSlotFar(u32 value, u32 slot);
void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);

s32 Menu_SelectQuantity(s32 value)
{
#if defined(TBS_EDITION_JA)
    u8 *menu = (u8 *)gMenuWork;
    s32 changed = 1;
#else
    s32 changed = 1;
    u8 *menu = (u8 *)gMenuWork;
    u8 *confirmState = MENU_SUBOBJECT(menu, 540);
#endif
    s32 window;
    s32 quantity = 0;

#if defined(TBS_EDITION_JA)
    /* The Japanese window covers only the left of the screen. */
    window = UiWindow_CreateFar(0, 0, 13, 10, 2);
#else
    confirmState[5] = 13;
    window = UiWindow_CreateFar(0, 0, 30, 10, 2);
#endif
    Scheduler_RemoveCallback((u32)(Menu_UpdateEntryObjectTransforms));

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 13;
    }
    Palette_CopyObjectBankToBackground14();
    WaitFrames(1);

    goto check_exit;

adjust:
    {
        volatile s32 *keys = &gKeysRepeat;

        if (*keys & 0x40) {
            quantity -= 1;
            changed = 1;
        }
        if (*keys & 0x80) {
            quantity += 1;
            changed = 1;
        }
    }
    WaitFrames(1);

check_exit:
    if (GameFlag_TestFar(336) != 0)
        goto done;

    if (changed != 0) {
        changed = 0;
        quantity = (quantity + 5) % 5;
        ItemMenu_DrawItemDetails(window, value);
    }

    {
        volatile u32 *keys = &gKeyState;

        if (*keys & 1)
            goto done;
        if (*keys & 2) {
            quantity = -1;
            goto done;
        }
    }
    goto adjust;

done:
    RenderOutput_RedrawSavedRectFar(window);
    WaitFrames(1);
    UiWork_FinalizeFar(window, 1);
    RenderOutput_RedrawSavedRectFar(*(s32 *)(menu + 16));
    Palette_LightenBankHighlight(14);
    {
        s32 delay = 0xc80;

        Scheduler_AddOrUpdateCallback((s32)((const void *)Menu_UpdateEntryObjectTransforms), delay);
    }

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
#if defined(TBS_EDITION_JA)
        /* The Japanese menu marks itself for a redraw instead of framing
           the right-hand windows again. */
        /* FAKEMATCH: one register carries the 1 both stores write. */
        s32 one = 1;

        iconState[5] = one;
        *(u16 *)(menu + 0x220) = one;
#else
        iconState[5] = 1;
#endif
    }
#if !defined(TBS_EDITION_JA)
    UiWindow_DrawFrameFar(13, 0, 17, 10);
#endif

    return quantity;
}

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
            UiText_DrawCharacterAtOffsetFar((s32)MsgEquipEffectHeading, window, 16, 0);
            row = 1;
        }
        if (def->primary_bonus != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgStatLabel, window, 0, row * 8);
            amount = def->primary_bonus;
            ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
            row++;
        }
        if (def->secondary_bonus != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgDefenseLabel, window, 0, row * 8);
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
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
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
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 8, row * 8);
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
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
#if defined(TBS_EDITION_JA)
            /* The Japanese rate reads 1.5 and then its word for times. */
            if (amount > 9) {
                UiText_DrawNumberInWindowFar(1, 1, window, 48, row * 8);
                UiText_DrawStringInWindowFar(Data_080af220, window, 56, row * 8);
                UiText_DrawNumberInWindowFar(amount - 10, 1, window, 64, row * 8);
            } else {
                UiText_DrawNumberInWindowFar(0, 1, window, 48, row * 8);
                UiText_DrawStringInWindowFar(Data_080af220, window, 56, row * 8);
                UiText_DrawNumberInWindowFar(amount, 1, window, 64, row * 8);
            }
            UiText_DrawCharacterAtOffsetFar((s32)MsgEffectRateSuffix, window, 72, row * 8);
#else
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
#endif
            break;
        case 23:
        case 25:
        case 27:
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
            break;
        }
        row++;
    }
    if (def->flags & 1) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgItemCursed, window, 0, row * 8);
        row++;
    }
    if (def->use_type == 3) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgBestowsPsynergy, window, 0, row * 8);
        consumable = 1;
        row++;
    }
    if (def->use_type != 4 && def->use_type != 0) {
        if (!consumable) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsesHeading, window, 16, row * 8);
            row++;
        }
        switch (def->use_type) {
        case 0:
            break;
        case 1:
            UiText_DrawCharacterAtOffsetFar((s32)MsgSingleUse, window, 0, row * 8);
            row++;
            break;
        case 2: {
            s32 m;

            if (item & 0x400) {
                m = (s32)MsgBrokenNotice;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
                m++;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
            } else {
                m = (s32)MsgMightBreak;
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
        i = (s32)MsgNumberHeading;
        UiText_DrawCharacterAtOffsetFar(i, window, 16, row * 8);
        row++;
        amount = (item & 0xf800) / 2048;
#if defined(TBS_EDITION_JA)
        /* The Japanese note draws the count before its words. */
        UiText_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
        UiText_DrawCharacterAtOffsetFar(i + 1, window, 16, row * 8);
#else
        UiWork_PushValueSlotFar(amount + 1, 5);
        UiText_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
#endif
        row++;
    }
    found = 0;
    if (row == 0) {
        if (def->flags & 4) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgRareItem, window, 0, 0);
            found = 1;
        }
        if (!found) {
            if (def->flags & 8) {
                UiText_DrawCharacterAtOffsetFar((s32)MsgImportantItem, window, 0, 0);
                found = 1;
            }
            if (!found)
                UiText_DrawCharacterAtOffsetFar((s32)MsgDetailsUnknown, window, 0, 0);
        }
    }
}
