#include "types.h"
#include "ram_buffer.h"
#include "battle_unit.h"

struct ItemMenuCursor {
    u8 unknown_00[5];
    u8 state;
};

struct ItemMenuState {
    u8 unknown_000[0x170];
    struct ItemMenuCursor *cursor;
};

extern u8 MsgExpLabel[];
extern u8 MsgClassName[];
extern const u8 Menu_LvString[];
extern const u8 Menu_SlashString[];
extern const u8 Menu_HpString[];
extern const u8 Menu_PpString[];

void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWork_SetParamNibbleFar(s32 color);
struct BattleUnit *UnitGet(s32 owner);
void WaitFrames(s32 frames);

void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags)
{
    struct ItemMenuState *menu;
    struct BattleUnit *unit;

    menu = Ram_HeapSlots->menu_runtime;
    unit = UnitGet(owner);
    menu->cursor->state = 1;
    flags &= 0x100;
    if (flags == 0)
        UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
    UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
    UiText_DrawCharacterAtOffsetFar(unit->class_index + (s32)MsgClassName, window, 0, 32);
    UiText_DrawStringAtOffsetFar(Menu_LvString, window, 104, 0);
    UiWork_SetParamNibbleFar(15);
    UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
    UiText_DrawStringAtOffsetFar(Menu_HpString, window, 40, 16);
    UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
    UiText_DrawNumberInWindowFar(unit->max_hp, 4, window, 112, 16);
    UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 16);
    UiText_DrawStringAtOffsetFar(Menu_PpString, window, 40, 24);
    UiText_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
    UiText_DrawNumberInWindowFar(unit->max_pp, 4, window, 112, 24);
    UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel, window, 40, 8);
    UiText_DrawNumberInWindowFar(unit->experience, 8, window, 80, 8);
    if (flags == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 23, window, 152, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 22, window, 152, 8);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 21, window, 152, 16);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 20, window, 152, 24);
    UiText_DrawNumberInWindowFar(unit->atk, 3, window, 200, 0);
    UiText_DrawNumberInWindowFar(unit->def, 3, window, 200, 8);
    UiText_DrawNumberInWindowFar(unit->agi, 3, window, 200, 16);
    UiText_DrawNumberInWindowFar(unit->luck, 3, window, 200, 24);
}
