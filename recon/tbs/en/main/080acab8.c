/*
 * Draft: DjinnMenu_DrawStatPreview, 18 of 889 instructions off (score 615),
 * rewritten on 2026-10-02 (wave 1b slice 5) from the machine-lifted draft
 * that could not be scored. English edition only; the Japanese function is
 * shorter and draws its labels from message 0x8b0.
 *
 * Every remaining difference is the order of a few instructions inside one
 * basic block, or the scratch register beside them: the label constant is
 * loaded after the level number's call in the listing and before it here,
 * and the class name, changed-class, list-row and final copy calls set up
 * their arguments in another order. Djinn_AddToOwnerFar returning a value is
 * what orders the first case of the switch; no other callee return type helps.
 * The changed-class arm keeps the pixel column in a variable, which computes
 * it in the argument register as the listing does.
 *
 * i = 0 before the second page test is a dead store that only keeps the two
 * page tests apart: without it GCC threads the first test past the second and
 * merges the blocks (277 instructions off). The listing keeps both tests, so
 * its source has something between them that leaves no code. A one-pass loop
 * (do ... while (0)) from the label constant to the end of that block puts the
 * constant after the call as the listing has it, but moves two other
 * instructions; that and an inlined helper with no return type both leave
 * such invisible marks, and neither was settled.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"
#include "RUNTIME_MEM.H"

struct RenderInput;

struct DjinnMenuState {
    u8 unknown_000[0x178];
    u16 entries[110];               /* 0x178 */
    u8 entry_index[2];              /* 0x254 */
    u8 entry_element[2];            /* 0x256 */
};

struct WindowWork {
    u8 unknown_000[0xea3];
    u8 dirty;
};

extern struct DjinnMenuState *gMenuWork;
extern struct WindowWork *gWindowWork;
extern const char Data_080af28c[];

void UiNumber_DrawAt(s32 value, s32 digits, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(struct RenderInput *window, s32 tile, s32 x, s32 y, s32 palette);
void UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant);
void UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 icon);
void UiWork_SetParamNibbleFar(s32 value);
void UiWindow_DrawDividerLineFar(struct RenderInput *window, s32 x0, s32 y0, s32 x1, s32 y1);
void SideObject_CreateFar(s32 owner, s32 a, s32 side, struct RenderInput *window, s32 b, s32 c);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 index);
void Djinn_ActivateFar(s32 owner, s32 element, s32 index);
void Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
void Owner_RecalculateStatsFar(s32 owner);
struct BattleAction *Ability_GetData(s32 action);
s8 OwnerAction_DiffSlots(void *before, void *after, u16 *out, s32 *gained, s32 *lost);

s32 DjinnMenu_DrawStatPreview(struct RenderInput *window, s32 x, s32 y, s32 owner,
    s32 give, s32 take, s32 mode, s32 page, s32 side)
{
    struct BattleUnit *state;
    struct BattleUnit *saved;
    s32 give_element;
    s32 give_index;
    s32 give_set;
    s32 take_element;
    s32 take_index;
    s32 take_set;
    s32 lost;
    s32 gained;
    u16 actions[48];
    struct DjinnMenuState *menu;
    s32 i;

    state = Owner_GetStateFar(owner);
    menu = gMenuWork;
    give_element = menu->entry_element[give];
    give_index = menu->entry_index[give];
    give_set = (u16)(menu->entries[give] & 0x8000);
    take_element = menu->entry_element[take];
    take_index = menu->entry_index[take];
    take_set = (u16)(menu->entries[take] & 0x8000);
    saved = (struct BattleUnit *)Runtime_BumpAllocate(sizeof(struct BattleUnit));
    Iwram_CopyWords(saved, state, sizeof(struct BattleUnit));

    if (page == 0) {
        if (mode == 3) {
            UiNumber_DrawAt(state->max_hp, 3, window, x * 8 + 80, y * 8 + 56);
            UiNumber_DrawAt(state->max_pp, 3, window, x * 8 + 80, y * 8 + 64);
            UiNumber_DrawAt(state->hp, 3, window, x * 8 + 48, y * 8 + 56);
            UiNumber_DrawAt(state->pp, 3, window, x * 8 + 48, y * 8 + 64);
            UiText_DrawStringAtOffsetFar(Data_080af28c + 4, window, x * 8 + 72, y * 8 + 56);
            UiText_DrawStringAtOffsetFar(Data_080af28c + 4, window, x * 8 + 72, y * 8 + 64);
        } else {
            UiNumber_DrawAt(state->hp, 3, window, x * 8 + 48, y * 8 + 56);
            UiNumber_DrawAt(state->pp, 3, window, x * 8 + 48, y * 8 + 64);
        }
        UiNumber_DrawAt(state->attack, 3, window, x * 8 + 48, y * 8 + 72);
        UiNumber_DrawAt(state->defense, 3, window, x * 8 + 48, y * 8 + 80);
        UiNumber_DrawAt(state->agility, 3, window, x * 8 + 48, y * 8 + 88);
        UiNumber_DrawAt(state->luck, 2, window, x * 8 + 56, y * 8 + 96);
    }

    switch (mode) {
    case 0:
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    case 1:
        give_index &= 31;
        Djinn_DeactivateFar(owner, give_element, give_index);
        break;
    case 2:
        if (give_set) {
            give_index &= 31;
            Djinn_DeactivateFar(owner, give_element, give_index);
        }
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        if (take_set)
            Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    case 4:
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        if (take_set)
            Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    }

    Owner_RecalculateStatsFar(owner);
    state = Owner_GetStateFar(owner);

    if (page == 0) {
        s32 label;

        UiText_DrawStringAtOffsetFar(state->name, window, x * 8 + 40, y * 8);
        UiText_DrawStringAtOffsetFar(Data_080af28c, window, x * 8 + 40, y * 8 + 16);
        UiNumber_DrawAt(state->level, 2, window, x * 8 + 88, y * 8 + 16);
        label = 0x8ae;
        UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
        UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
        UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
        UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
        UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
        UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
        UiText_DrawCharacterAtOffsetFar(saved->class_index + 0x741, window, x * 8, y * 8 + 32);
    }

    i = 0;
    if (page == 0) {
        s32 column;
        s32 icon_x;
        s32 px;

        if (saved->class_index != state->class_index) {
            px = x * 8;
            UiText_DrawCharacterAtOffsetFar(state->class_index + 0x741, window, px, y * 8 + 48);
            UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
        }
        column = x;
        if (saved->class_index != state->class_index)
            column += 5;
        for (i = 0; i < 4; i++) {
            UiWindow_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5, 0);
            UiWindow_SetTilemapEntryFar(window,
                ((struct OwnerDjinnState *)state)->active_counts[i] + 0xf030,
                column + i * 2 + 1, y + 5, 0);
        }
        icon_x = x * 8 + 70;
        if (state->hp != saved->hp) {
            UiNumber_DrawAt(state->hp, 4, window, x * 8 + 72, y * 8 + 56);
            if (state->hp > saved->hp)
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 56, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 56, 1);
        }
        if (state->pp != saved->pp) {
            UiNumber_DrawAt(state->pp, 4, window, x * 8 + 72, y * 8 + 64);
            if (state->pp > saved->pp)
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 64, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 64, 1);
        }
        if (state->attack != saved->attack) {
            UiNumber_DrawAt(state->attack, 4, window, x * 8 + 72, y * 8 + 72);
            if (state->attack > saved->attack)
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 72, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 72, 1);
        }
        if (state->defense != saved->defense) {
            UiNumber_DrawAt(state->defense, 4, window, x * 8 + 72, y * 8 + 80);
            if (state->defense > saved->defense)
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 80, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 80, 1);
        }
        if (state->agility != saved->agility) {
            UiNumber_DrawAt(state->agility, 4, window, x * 8 + 72, y * 8 + 88);
            if (state->agility > saved->agility)
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 88, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 88, 1);
        }
        if (state->luck != saved->luck) {
            UiNumber_DrawAt(state->luck, 2, window, x * 8 + 88, y * 8 + 96);
            if (state->luck > saved->luck)
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 96, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, icon_x, y * 8 + 96, 1);
        }
    }

    if (page > 0) {
        s32 rows;
        s32 first;
        s8 count;
        s8 row;
        s8 line;

        rows = 6 - (mode != 3);
        first = rows * (page - 1);
        count = OwnerAction_DiffSlots(((struct OwnerActionState *)saved)->action_slots,
            ((struct OwnerActionState *)state)->action_slots, actions, &gained, &lost);
        line = 0;
        for (row = 0; first < count && row < rows; row++, first++) {
            UiIcon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                actions[first] & 0x3fff);
            if (actions[first] & 0x8000)
                UiWork_SetParamNibbleFar(4);
            else if (actions[first] & 0x4000)
                UiWork_SetParamNibbleFar(2);
            else
                UiWork_SetParamNibbleFar(15);
            UiText_DrawCharacterAtOffsetFar((actions[first] & 0x3fff) + 0x333, window,
                x * 8 + 16, (y + line * 2) * 8 + 8);
            UiText_DrawNumberAtOffsetFar(Ability_GetData(actions[first])->pp_cost, 2, window,
                x * 8 + 88, (y + line * 2) * 8 + 8);
            line++;
        }
        UiWork_SetParamNibbleFar(15);
        UiText_DrawCharacterAtOffsetFar(0xaed, window, x * 8 + 88, y * 8);
        if (mode != 3) {
            s32 lines;

            lines = 0;
            if (gained) {
                UiWork_SetParamNibbleFar(4);
                UiText_DrawCharacterAtOffsetFar(0xba2, window, x * 8, y * 8 + 88);
                lines = 1;
            }
            if (lost) {
                UiWork_SetParamNibbleFar(2);
                UiText_DrawCharacterAtOffsetFar(0xba3, window, x * 8, (y + lines) * 8 + 88);
                lines++;
            }
            if (lines == 0)
                UiText_DrawCharacterAtOffsetFar(0xba8, window, x * 8, y * 8 + 88);
            UiWork_SetParamNibbleFar(15);
            UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
        }
        gWindowWork->dirty = 1;
    }

    if (page == 0)
        SideObject_CreateFar(owner, 0, side, window, 0, 0);
    Iwram_CopyWords(state, saved, sizeof(struct BattleUnit));
    Runtime_BumpFree(saved);
    return 1;
}
