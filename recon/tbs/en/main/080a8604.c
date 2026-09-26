#include "TYPES.H"

/* main:080a8604, whole 768-byte owner through 080a8904.
 * H1: correct mode from draft offset 0x224 to ROM offset 0x220, restore
 * value-returning availability/icon interfaces, and transfer named menu
 * and label globals plus typed cursor/element fields from exact neighbors.
 * Caller 080a8114 supplies window, owner and mode 1. The 080a8914 status
 * panel owns the same text interfaces; 080a8b10 and 080a9dc4 return s32.
 * Prediction: whole extent and 40-byte frame, no omitted or extra calls.
 * Gate: zero differing bytes including pools, then compare/test/coverage/
 * verify. Budget: first corrected model plus two structural variants.
 * H1: 784/768 bytes, 160 aligned edits. Mode and actual return interfaces
 * are repaired. Frame remains 44 rather than 40, work/count use sl/r8
 * instead of r8/sl, power_x spills and level_x-8 survives the call chain.
 * Plain !=0 creates an extra branch; retain the prior tagged branchless
 * nonzero encoding in the next model. No new matching credit.
 * H2: one phase pointer spans menu setup and the element-stat walk, as
 * the reference's r8 carrier does. Keep the typed views at each access.
 * Prediction: menu and element cursor share allocation, freeing the power
 * column from its stack slot. Preserve the branchless has-Djinn encoding.
 * H2 result: 784/768 bytes, 181 aligned edits, topology equal. Frame is
 * now 40 and power_x is in fp, but the combined pointer spills at sp+20;
 * mode/owner slots agree while Djinn/row slots differ. The persistent
 * level_x-8 remains. Shared pointer allocation is not a matching witness.
 */

extern u8 Value_00000afe[];

struct StatusCursor {
    u8 unknown_00[5];
    u8 state;
};

struct ElementStat {
    s16 power;
    s16 resistance;
};

struct MenuState {
    u8 unknown_000[380];
    struct StatusCursor *cursor;
    u8 unknown_180[160];
    u16 mode;
};

struct StatusUnit {
    u8 unknown_000[72];
    struct ElementStat element_stats[4];
    u8 unknown_058[192];
    u8 djinn_set[4];
    u8 djinn_total[4];
};

extern struct MenuState *gMenuWork;
extern const u8 Menu_LvString[];
extern const u8 Data_080af230[];

s32 Party_SumDjinnCountsFar(s32 side);
struct StatusUnit *Owner_GetStateFar(s32 unit);
void ItemMenu_DrawOwnerStatus(void *window, s32 unit, s32 mode);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 flags, s32 unit);
s32 CharacterMenu_UpdateSelectionIcons(const u8 *ailments);
void ItemMenu_ApplyFlags(const u8 *ailments);
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
    void *work;
    s32 has_djinn;
    s32 total;
    struct StatusUnit *status;
    s32 row;
    s32 keep;
    s32 count;
    s32 y;
    s32 text;
    s32 power_x;
    s32 level_x;
    u8 *djinn;
    u8 ailments[8];

    work = gMenuWork;
    total = Party_SumDjinnCountsFar(-1);
    /* FAKEMATCH: spell the reference's branchless nonzero encoding. */
    has_djinn = (u32)(-total | total) >> 31;
    status = Owner_GetStateFar(unit);
    row = 7;
    if ((mode & 0xff) != 1)
        row = 10;
    ((struct MenuState *)work)->cursor->state = 1;
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
    ItemMenu_ApplyFlags(ailments);
    if (((struct MenuState *)work)->mode == 3)
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
        UiText_DrawStringAtOffsetFar(Menu_LvString, window, 64, y + 16);
        text = (s32)Value_00000afe;
        UiText_DrawCharacterAtOffsetFar(text, window, 64, y + 24);
        UiText_DrawCharacterAtOffsetFar(text + 1, window, 64, y + 32);
    }
    /* FAKEMATCH: reuse the menu-phase pointer for the element-stat phase. */
    work = status->element_stats;
    power_x = 104;
    level_x = 120;
    djinn = status->djinn_set;
    for (count = 0; count <= 3; count++) {
        if (has_djinn)
            UiText_DrawNumberInWindowFar(djinn[0], 1, window, level_x, row * 8 + 8);
        if ((mode & 0xff) == 1) {
            if (has_djinn) {
                UiText_DrawNumberInWindowFar(djinn[4], 1, window, power_x, row * 8 + 8);
                UiText_DrawStringInWindowFar(Data_080af230, window, level_x - 8, row * 8 + 8);
            }
            UiText_DrawNumberInWindowFar(Func_080771f8(unit, count), 2, window, level_x - 8, row * 8 + 16);
            UiText_DrawNumberInWindowFar(((struct ElementStat *)work)->power, 3, window, power_x, row * 8 + 24);
            UiText_DrawNumberInWindowFar(((struct ElementStat *)work)->resistance, 3, window, power_x, row * 8 + 32);
        }
        work = (struct ElementStat *)work + 1;
        power_x += 32;
        level_x += 32;
        djinn++;
    }
}
