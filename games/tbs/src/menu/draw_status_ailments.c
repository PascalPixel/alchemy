#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "edition.h"
#include "types.h"
#include "btlsys.h"
#include "text/msg_ids.h"
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

#define UiWindow_ClearInteriorTilesFar ((void (*)(void*,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)
#define UiText_DrawStringInWindowFar ((void (*)(const void*,void*,s32,s32))UiText_DrawStringInWindowFar)
#define UiText_DrawNumberInWindowFar ((void (*)(s32,s32,void*,s32,s32))UiText_DrawNumberInWindowFar)



s32 Party_SumDjinnCountsFar(s32 side);
void UiText_DrawCharacterAtOffsetFar(s32 text, void *window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, void *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(void *window, s32 index, s32 x, s32 y, s32 tile);
void UiWork_SetParamNibbleFar(s32 color);
void WaitFrames(s32 frames);

void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 unit, s32 mode)
{
    struct CharacterMenuState *state = gMenuWork;
    struct BtlUnit *status;
    s32 row;
    s32 has_djinn;
    s32 keep;
    s32 i;
    u8 ailments[8];

    if (Party_SumDjinnCountsFar(-1) != 0)
        has_djinn = 1;
    else
        has_djinn = 0;
    status = UnitGetFar(unit);
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
        UiText_DrawCharacterAtOffsetFar((s32)MsgDownLabel, window, 16, 40);
        i = 1;
    }
    if (ailments[CHARACTER_POISON] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgPoisonLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_VENOM] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgVenomLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_CURSE] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgCurseLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_HAUNT] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgHauntLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (i == 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgNormalLabel, window, 0, 40);
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
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnLabel, window, 64, row * 8 + 8);
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
            UiText_DrawNumberInWindowFar(status->djinn_owned_counts[i], 1, window, i * 32 + 120, row * 8 + 8);
        if ((mode & 0xff) == 1) {
            if (has_djinn) {
                UiText_DrawNumberInWindowFar(status->djinn_active_counts[i], 1, window, i * 32 + 104, row * 8 + 8);
                UiText_DrawStringInWindowFar(Menu_SlashString, window, i * 32 + 112, row * 8 + 8);
            }
            UiText_DrawNumberInWindowFar(Owner_GetResistanceValueFar(unit, i), 2, window, i * 32 + 112, row * 8 + 16);
            UiText_DrawNumberInWindowFar(status->elm[i].pow, 3, window, i * 32 + 104, row * 8 + 24);
            UiText_DrawNumberInWindowFar(status->elm[i].res, 3, window, i * 32 + 104, row * 8 + 32);
        }
    }
}
