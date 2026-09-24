#include "TYPES.H"

/* main:080a8604 CharacterMenu_DrawStatusAilments - hand-written draft, 184
   of 384 halfwords differ (784 of 768 bytes). Residual: register roles
   (state and the element-stat cursor share r8 in the ROM, the ailment count
   and loop index share sl), the power column x lives in fp rather than on
   the stack, and the level column x - 8 is recomputed at each use in the
   ROM where GCSE shares it here. The Djinn test is spelled as the ROM's
   neg/orr/lsr store-flag (FAKEMATCH below).

   Draws a member's status page: the ailments they suffer, then, for each
   element, their Djinn counts and elemental level, power and resistance. */

extern u8 Value_00000afe[];

struct MenuState {
    u8 unknown_000[380];
    u8 *cursor;
    u8 unknown_180[164];
    u16 mode;
};

struct StatusUnit {
    u8 unknown_000[72];
    s16 element_stats[4][2];
    u8 unknown_058[192];
    u8 djinn_set[4];
    u8 djinn_total[4];
};

s32 Party_SumDjinnCountsFar(s32 side);
struct StatusUnit *Owner_GetStateFar(s32 unit);
void ItemMenu_DrawOwnerStatus(void *window, s32 unit, s32 mode);
void CharacterMenu_BuildAvailability(u8 *ailments, s32 flags, s32 unit);
void CharacterMenu_UpdateSelectionIcons(u8 *ailments);
void Func_080a9d3c(u8 *ailments);
void UiWindow_ClearInteriorTilesFar(void *window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 text, void *window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, void *window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const void *text, void *window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, void *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(void *window, s32 index, s32 x, s32 y, s32 tile);
void UiWork_SetParamNibbleFar(s32 color);
void WaitFrames(s32 frames);
s32 Func_080771f8(s32 unit, s32 element);

void CharacterMenu_DrawStatusAilments(void *window, s32 unit, s32 mode)
{
    struct MenuState *state;
    s32 has_djinn;
    s32 total;
    struct StatusUnit *status;
    s32 row;
    s32 keep;
    s32 count;
    s32 y;
    s32 text;
    s16 *stats;
    s32 power_x;
    s32 level_x;
    u8 *djinn;
    u8 ailments[8];

    state = *(struct MenuState **)0x03001f2c;
    total = Party_SumDjinnCountsFar(-1);
    /* FAKEMATCH: the ROM tests the Djinn total with a neg/orr/lsr flag. */
    has_djinn = (u32)(-total | total) >> 31;
    status = Owner_GetStateFar(unit);
    row = 7;
    if ((mode & 0xff) != 1)
        row = 10;
    state->cursor[5] = 1;
    ItemMenu_DrawOwnerStatus(window, unit, mode);
    CharacterMenu_BuildAvailability(ailments, 1, unit);
    CharacterMenu_UpdateSelectionIcons(ailments);
    keep = mode & 0x100;
    if (keep == 0)
        UiWindow_ClearInteriorTilesFar(window, 0, 40, 96, 96);
    count = 0;
    if (ailments[0] != 0) {
        UiText_DrawCharacterAtOffsetFar(0xbd5, window, 16, 40);
        count = 1;
    }
    if (ailments[1] != 0) {
        UiText_DrawCharacterAtOffsetFar(0xbd6, window, 16, count * 16 + 40);
        count++;
    }
    if (ailments[2] != 0) {
        UiText_DrawCharacterAtOffsetFar(0xbd7, window, 16, count * 16 + 40);
        count++;
    }
    if (ailments[3] != 0) {
        UiText_DrawCharacterAtOffsetFar(0xbd8, window, 16, count * 16 + 40);
        count++;
    }
    if (ailments[4] != 0) {
        UiText_DrawCharacterAtOffsetFar(0xbd9, window, 16, count * 16 + 40);
        count++;
    }
    if (count == 0)
        UiText_DrawCharacterAtOffsetFar(0xbd4, window, 0, 40);
    CharacterMenu_UpdateSelectionIcons(ailments);
    Func_080a9d3c(ailments);
    if (state->mode == 3)
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
        UiText_DrawCharacterAtOffsetFar(0xafd, window, 64, row * 8 + 8);
    if (mode == 1) {
        if (!has_djinn)
            row--;
        y = row * 8;
        UiText_DrawStringAtOffsetFar((const void *)0x080af22c, window, 64, y + 16);
        text = (s32)Value_00000afe;
        UiText_DrawCharacterAtOffsetFar(text, window, 64, y + 24);
        UiText_DrawCharacterAtOffsetFar(text + 1, window, 64, y + 32);
    }
    stats = status->element_stats[0];
    power_x = 104;
    level_x = 120;
    djinn = status->djinn_set;
    for (count = 0; count <= 3; count++) {
        if (has_djinn)
            UiText_DrawNumberInWindowFar(djinn[0], 1, window, level_x, row * 8 + 8);
        if ((mode & 0xff) == 1) {
            if (has_djinn) {
                UiText_DrawNumberInWindowFar(djinn[4], 1, window, power_x, row * 8 + 8);
                UiText_DrawStringInWindowFar((const void *)0x080af230, window, level_x - 8, row * 8 + 8);
            }
            UiText_DrawNumberInWindowFar(Func_080771f8(unit, count), 2, window, level_x - 8, row * 8 + 16);
            UiText_DrawNumberInWindowFar(stats[0], 3, window, power_x, row * 8 + 24);
            UiText_DrawNumberInWindowFar(stats[1], 3, window, power_x, row * 8 + 32);
        }
        stats += 2;
        power_x += 32;
        level_x += 32;
        djinn++;
    }
}
