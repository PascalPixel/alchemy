#include "TYPES.H"
#include "ITEM_MENU.H"
#include "BATTLE_TYPES.H"

/* Message identities link as offsets from the "Attack" label, as in
   Shop_DrawItemPage; the class-name base is a link-time symbol loaded from
   the literal pool. */
extern u8 Data_00000af7[];
extern u8 Value_00000741;

/* Fixed labels drawn with the panel. */
extern const u8 Data_080af22c[];
extern const u8 Data_080af230[];
extern const u8 Data_080af234[];
extern const u8 Data_080af238[];

void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWork_SetParamNibbleFar(s32 color);
struct BattleUnit *Owner_GetStateFar(s32 owner);
void WaitFrames(s32 frames);


/* The owner's status panel: name, class, level, HP and PP against their
   maxima and experience on the left, the four battle stats on the right.
   Bit 8 of flags skips clearing the panel first. */
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags)
{
    struct ItemMenuState *menu;
    struct BattleUnit *unit;

    menu = *(struct ItemMenuState **)0x03001f2c;
    unit = Owner_GetStateFar(owner);
    menu->cursor->state = 1;
    flags &= 0x100;
    if (flags == 0) {
        UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
    }
    UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
    UiText_DrawCharacterAtOffsetFar(unit->class_index + (s32)&Value_00000741, window, 0, 32);
    UiText_DrawStringAtOffsetFar(Data_080af22c, window, 104, 0);
    UiWork_SetParamNibbleFar(15);
    UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
    UiText_DrawStringAtOffsetFar(Data_080af234, window, 40, 16);
    UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
    UiText_DrawNumberInWindowFar(unit->max_hp, 4, window, 112, 16);
    UiText_DrawStringInWindowFar(Data_080af230, window, 104, 16);
    UiText_DrawStringAtOffsetFar(Data_080af238, window, 40, 24);
    UiText_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
    UiText_DrawNumberInWindowFar(unit->max_pp, 4, window, 112, 24);
    UiText_DrawStringInWindowFar(Data_080af230, window, 104, 24);
    UiText_DrawCharacterAtOffsetFar((s32)Data_00000af7 + 23, window, 40, 8);
    UiText_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
    if (flags == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
    }
    UiText_DrawCharacterAtOffsetFar((s32)Data_00000af7, window, 152, 0);
    UiText_DrawCharacterAtOffsetFar((s32)Data_00000af7 + 1, window, 152, 8);
    UiText_DrawCharacterAtOffsetFar((s32)Data_00000af7 + 2, window, 152, 16);
    UiText_DrawCharacterAtOffsetFar((s32)Data_00000af7 + 3, window, 152, 24);
    UiText_DrawNumberInWindowFar(unit->attack, 3, window, 200, 0);
    UiText_DrawNumberInWindowFar(unit->defense, 3, window, 200, 8);
    UiText_DrawNumberInWindowFar(unit->agility, 3, window, 200, 16);
    UiText_DrawNumberInWindowFar(unit->luck, 3, window, 200, 24);
}
