/* 2026-09-29 alchemy permute: score 3186 to 2646 on the permuter's scorer
   (0 is exact); remaining 38 register-only, 21 stack-only, 6 operand, 18
   reordered, 11 inserted, 1 deleted. Kept rewrites: 9x reorder independent
   statements, 6x swap commutative operands, 6x reorder local declarations,
   6x test truth or compare with zero, 5x introduce a temporary, 3x remove
   a temporary, 3x change loop form, 3x pointer arithmetic or indexing, 2x
   split or join a compound assignment, 2x move an assignment into or out
   of a condition, 2x toggle register. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
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
 * H3: restore distinct typed phase pointers and express element stats and
 * Djinn counts as indexed owner arrays. Prediction: loop strength reduction
 * creates the paired four-byte/one-byte walks without a user phase pointer
 * or a second scalar counter. H3: 784/768 bytes, 123 aligned edits and
 * equal topology. Both walks are derived, and initial work now uses r8,
 * but the count/element/Djinn carriers and the retained level_x-8 still
 * force a power-column spill and 44-byte frame. Preserve this corrected
 * typed model; three hypotheses exhausted, no adoption. Do not resume the
 * register/lifetime axes without a new fact beyond the recorded RTL.
 */

extern u8 MsgPowerLabel[];
extern u8 MsgNormalLabel, MsgDownLabel, MsgPoisonLabel, MsgVenomLabel, MsgCurseLabel, MsgHauntLabel, MsgDjinnLabel;

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
s32 Owner_GetResistanceValueFar(s32 unit, s32 element);

void CharacterMenu_DrawStatusAilments(void *window, s32 unit, s32 mode)
{
    struct MenuState *state;
    struct StatusUnit *status;
    s32 has_djinn;
    s32 total;
    register s32 count;
    s32 row;
    register s32 text;
    s32 keep;
    s32 y;
    s32 power_x;
    s32 level_x;
    u8 ailments[8];
    struct MenuState *tmp2;

    tmp2 = gMenuWork;
    state = tmp2;
    total = Party_SumDjinnCountsFar(-1);
    /* FAKEMATCH: spell the reference's branchless nonzero encoding. */
    has_djinn = (u32)(-total | total) >> 31;
    status = Owner_GetStateFar(unit);
    row = 7;
    if ((0xff & mode) != 1)
        row = 10;
    state->cursor->state = 1;
    ItemMenu_DrawOwnerStatus(window, unit, mode);
    CharacterMenu_BuildAvailability(ailments, 1, unit);
    CharacterMenu_UpdateSelectionIcons(ailments);
    keep = mode & 0x100;
    if (!(keep != 0))
        UiWindow_ClearInteriorTilesFar(window, 0, 40, 96, 96);
    count = 0;
    if (ailments[0]) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgDownLabel, window, 16, 40);
        count = 1;
    }
    if (ailments[1]) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgPoisonLabel, window, 16, count * 16 + 40);
        count += 1;
    }
    if (ailments[2] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgVenomLabel, window, 16, count * 16 + 40);
        count++;
    }
    if (ailments[3] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgCurseLabel, window, 16, count * 16 + 40);
        count++;
    }
    if (ailments[4] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)&MsgHauntLabel, window, 16, count * 16 + 40);
        count += 1;
    }
    if (count == 0)
        UiText_DrawCharacterAtOffsetFar((s32)&MsgNormalLabel, window, 0, 40);
    CharacterMenu_UpdateSelectionIcons(ailments);
    ItemMenu_ApplyFlags(ailments);
    if (state->mode == 3)
        return;
    if (keep == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 64, 56, 224, 96);
    }
    UiWork_SetParamNibbleFar(15);
    level_x = 120;
    count = 0;
    if (mode == 1 || has_djinn == 1) {
        UiWindow_SetTilemapEntryFar(window, 1, 15, row, 4);
        UiWindow_SetTilemapEntryFar(window, 2, 19, row, 4);
        UiWindow_SetTilemapEntryFar(window, 3, 23, row, 4);
        UiWindow_SetTilemapEntryFar(window, 4, 27, row, 4);
    }
    if (has_djinn)
        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnLabel, window, 64, row * 8 + 8);
    if (1 == mode) {
        if (!(0 != has_djinn))
            row--;
        y = row * 8;
        UiText_DrawStringAtOffsetFar(Menu_LvString, window, 64, y + 16);
        text = (s32)MsgPowerLabel;
        UiText_DrawCharacterAtOffsetFar(text, window, 64, y + 24);
        UiText_DrawCharacterAtOffsetFar(text + 1, window, 64, y + 32);
    }
    power_x = 104;
    if (count <= 3) {
        do {
            if (has_djinn != 0)
                UiText_DrawNumberInWindowFar(*(status->djinn_set + count), 1, window, level_x, row * 8 + 8);
            if ((mode & 0xff) == 1) {
                s32 tmp;
                tmp = level_x - 8;
                if (has_djinn) {
                    UiText_DrawNumberInWindowFar(status->djinn_total[count], 1, window, power_x, 8 + row * 8);
                    UiText_DrawStringInWindowFar(Data_080af230, window, level_x - 8, 8 + row * 8);
                }
                UiText_DrawNumberInWindowFar(Owner_GetResistanceValueFar(unit, count), 2, window, tmp, row * 8 + 16);
                UiText_DrawNumberInWindowFar(status->element_stats[count].power, 3, window, power_x, row * 8 + 24);
                UiText_DrawNumberInWindowFar(status[0].element_stats[count].resistance, 3, window, power_x, row * 8 + 32);
            }
            count++;
            power_x += 32;
            level_x += 32;
        } while (count <= 3);
    }
}
