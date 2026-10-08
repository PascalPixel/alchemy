#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "inventory_menu.h"
#include "btltype.h"
#include "ui.h"
#include "btlsys.h"
#include "text/msg_ids.h"

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)
#define UiText_DrawStringInWindowFar ((void (*)(const void*,s32,s32,s32))UiText_DrawStringInWindowFar)



void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWork_SetParamNibbleFar(s32 color);
void WaitFrames(s32 frames);

#if EDITION_INTERNATIONAL
#define STAT_DIGITS 3
#define STAT_X      200
#else
#define STAT_DIGITS 4
#define STAT_X      192
#endif

void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags)
{
    struct InventoryMenuState *menu;
    struct BtlUnit *unit;

    menu = gMenuWork;
    unit = UnitGetFar(owner);
    menu->cursor->active = 1;
    flags &= 0x100;
    if (flags == 0) {
        UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
    }
    UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
    UiText_DrawCharacterAtOffsetFar(unit->class_index + (s32)MsgClassName, window, 0, 32);
#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar(Menu_LvString, window, 104, 0);
#else
    UiText_DrawStringInWindowFar(Menu_LvString, window, 104, 0);
#endif
    UiWork_SetParamNibbleFar(15);
    UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar(Menu_HpString, window, 40, 16);
#else
    UiText_DrawStringInWindowFar(Menu_HpString, window, 40, 16);
#endif
    UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
    UiText_DrawNumberInWindowFar(unit->max_hp, 4, window, 112, 16);
    UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 16);
#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar(Menu_PpString, window, 40, 24);
#else
    UiText_DrawStringInWindowFar(Menu_PpString, window, 40, 24);
#endif
    UiText_DrawNumberInWindowFar(unit->ep, 4, window, 72, 24);
    UiText_DrawNumberInWindowFar(unit->max_ep, 4, window, 112, 24);
    UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel, window, 40, 8);
    UiText_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
    if (flags == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 23, window, 152, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 22, window, 152, 8);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 21, window, 152, 16);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 20, window, 152, 24);
    UiText_DrawNumberInWindowFar(unit->atk, STAT_DIGITS, window, STAT_X, 0);
    UiText_DrawNumberInWindowFar(unit->def, STAT_DIGITS, window, STAT_X, 8);
    UiText_DrawNumberInWindowFar(unit->agi, STAT_DIGITS, window, STAT_X, 16);
    UiText_DrawNumberInWindowFar(unit->luck, STAT_DIGITS, window, STAT_X, 24);
}
