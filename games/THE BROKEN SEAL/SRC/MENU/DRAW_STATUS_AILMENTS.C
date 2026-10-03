#include "CHARACTER_MENU.H"
#include "BATTLE_UNIT.H"
#include "EDITION.H"
#include "TYPES.H"

extern u8 MsgPowerLabel[];
extern u8 MsgNormalLabel, MsgDownLabel, MsgPoisonLabel, MsgVenomLabel, MsgCurseLabel, MsgHauntLabel, MsgDjinnLabel;

extern struct CharacterMenuState *gMenuWork;
extern const u8 Menu_LvString[];
extern const u8 Data_080af230[];

s32 Party_SumDjinnCountsFar(s32 side);
struct BattleUnit *Owner_GetStateFar(s32 unit);
void ItemMenu_DrawOwnerStatus(s32 window, s32 unit, s32 mode);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 flags, s32 unit);
s32 CharacterMenu_UpdateSelectionIcons(const u8 *ailments);
void ItemMenu_ApplyFlags(const u8 *ailments);
void UiWindow_ClearInteriorTilesFar(void *window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 text, void *window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, void *window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const void *text, void *window, s32 x, s32 y);
void UiNumber_DrawAt(s32 value, s32 digits, void *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(void *window, s32 index, s32 x, s32 y, s32 tile);
void UiWork_SetParamNibbleFar(s32 color);
void WaitFrames(s32 frames);
s32 Owner_GetResistanceValueFar(s32 unit, s32 element);

/* Draws a unit's page of the status screen: the ailments it suffers and,
 * outside the short mode, the Djinn it holds and its power and resistance in
 * each element. */
void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 unit, s32 mode)
{
    struct CharacterMenuState *state = gMenuWork;
    struct BattleUnit *status;
    s32 row;
    s32 has_djinn;
    s32 keep;
    s32 i;
    u8 ailments[8];

    if (Party_SumDjinnCountsFar(-1) != 0)
        has_djinn = 1;
    else
        has_djinn = 0;
    status = Owner_GetStateFar(unit);
    if ((mode & 0xff) == 1)
        row = 7;
    else
        row = 10;
    state->status_cursor->active = 1;
    ItemMenu_DrawOwnerStatus((s32)window, unit, mode);
    CharacterMenu_BuildAvailability(ailments, 1, unit);
    CharacterMenu_UpdateSelectionIcons(ailments);
    keep = mode & 0x100;
    if (keep == 0)
        UiWindow_ClearInteriorTilesFar(window, 0, 40, 96, 96);
    i = 0;
    if (ailments[CHARACTER_DOWN] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgDownLabel, window, 16, 40);
        i = 1;
    }
    if (ailments[CHARACTER_POISON] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgPoisonLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_VENOM] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgVenomLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_CURSE] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgCurseLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_HAUNT] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgHauntLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (i == 0)
        UiText_DrawCharacterAtOffsetFar((s32)&MsgNormalLabel, window, 0, 40);
    CharacterMenu_UpdateSelectionIcons(ailments);
    ItemMenu_ApplyFlags(ailments);
    if (state->page == 3)
        return;
    if (keep == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 64, 56, 224, 96);
    }
    UiWork_SetParamNibbleFar(15);
    if (mode == 1 || has_djinn == 1) {
        UiWindow_SetTilemapEntryFar(window, 1, 15, row, 4);
        UiWindow_SetTilemapEntryFar(window, 2, 19, row, 4);
        UiWindow_SetTilemapEntryFar(window, 3, 23, row, 4);
        UiWindow_SetTilemapEntryFar(window, 4, 27, row, 4);
    }
    if (has_djinn)
        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnLabel, window, 64, row * 8 + 8);
    if (mode == 1) {
        if (!has_djinn)
            row--;
#if EDITION_INTERNATIONAL
        UiText_DrawStringAtOffsetFar(Menu_LvString, window, 64, row * 8 + 16);
#else
        UiText_DrawStringInWindowFar(Menu_LvString, window, 64, row * 8 + 16);
#endif
        UiText_DrawCharacterAtOffsetFar((s32)MsgPowerLabel, window, 64, row * 8 + 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgPowerLabel + 1, window, 64, row * 8 + 32);
    }
    for (i = 0; i <= 3; i++) {
        if (has_djinn)
            UiNumber_DrawAt(status->djinn_owned_counts[i], 1, window, i * 32 + 120, row * 8 + 8);
        if ((mode & 0xff) == 1) {
            if (has_djinn) {
                UiNumber_DrawAt(status->djinn_active_counts[i], 1, window, i * 32 + 104, row * 8 + 8);
                UiText_DrawStringInWindowFar(Data_080af230, window, i * 32 + 112, row * 8 + 8);
            }
            UiNumber_DrawAt(Owner_GetResistanceValueFar(unit, i), 2, window, i * 32 + 112, row * 8 + 16);
            UiNumber_DrawAt(status->elements[i].power, 3, window, i * 32 + 104, row * 8 + 24);
            UiNumber_DrawAt(status->elements[i].resist, 3, window, i * 32 + 104, row * 8 + 32);
        }
    }
}
