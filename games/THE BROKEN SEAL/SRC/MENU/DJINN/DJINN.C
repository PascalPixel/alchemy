#include "EDITION.H"
#include "TYPES.H"
#include "OWNER_STATE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_UNIT.H"
#include "RUNTIME_MEM.H"
#include "UI.H"
#include "DJINN_MENU.H"

extern struct DjinnMenuWork *gMenuWork;

/* A set Djinni's entry carries bit 15, spelled as the signed halfword flag. */
#define DJINN_ENTRY_SET (-0x8000)

s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 owner,
    s32 give, s32 take, s32 mode, s32 page, s32 side);

extern struct UiWork *gWindowWork;
extern const char DjinnMenu_TextLevel[];
extern const char DjinnMenu_TextSlash[];
extern u8 MsgPreviewStatLabel[];
extern u8 MsgClassName[];
extern u8 MsgAbilityName[];
extern u8 MsgPsynergyPp[];
extern u8 MsgPsynergyGained[];
extern u8 MsgPsynergyLost[];
extern u8 MsgPsynergyUnchanged[];

void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 palette);
void UiIcon_DrawVariantWithTileOffset(s32 window, s32 x, s32 y, s32 variant);
void UiIcon_CreateWithLoadedResource(s32 window, s32 x, s32 y, s32 icon);
void UiWork_SetParamNibbleFar(s32 value);
void UiWindow_DrawDividerLineFar(s32 window, s32 x0, s32 y0, s32 x1, s32 y1);
void SideObject_CreateFar(s32 owner, s32 a, s32 side, s32 window, s32 b, s32 c);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 index);
void Djinn_ActivateFar(s32 owner, s32 element, s32 index);
void Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
void Owner_RecalculateStatsFar(s32 owner);
struct BattleAction *Ability_GetData(s32 action);
s8 OwnerAction_DiffSlots(void *before, void *after, u16 *out, s32 *gained, s32 *lost);

/*
 * Lists an owner's Djinn as packed halfword entries: element in bits 5-6,
 * index in bits 0-4 and, for set Djinn, bit 15. With element -1 every
 * element is listed and each entry also carries the owner in bits 8-14.
 * Returns the number of entries written.
 */
s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element)
{
    struct BattleUnit *state = Owner_GetStateFar(owner);
    s32 count = 0;
    s32 row;
    s32 bit;
    s32 entry;

    if (element == -1) {
        for (row = 0; row < 4; row++) {
            for (bit = 0; bit < 20; bit++) {
                if (state->djinn_active[row] & (1 << bit)) {
                    entry = (row << 5) | bit | DJINN_ENTRY_SET;
                    entry |= owner << 8;
                    out[count++] = entry;
                } else if (state->djinn_available[row] & (1 << bit)) {
                    out[count++] = (row << 5) | bit | (owner << 8);
                }
            }
        }
    } else {
        for (bit = 0; bit < 20; bit++) {
            if (state->djinn_active[element] & (1 << bit))
                out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
            else if (state->djinn_available[element] & (1 << bit))
                out[count++] = (element << 5) | bit;
        }
    }
    return count;
}

#if EDITION_INTERNATIONAL
#define ALT_PARAM 2
#else
#define ALT_PARAM 4
#endif

s32 Menu_RunPairedEntryAction(s32 mode, s32 param)
{
    s32 sp14;
    struct DjinnMenuWork *menu;

    menu = gMenuWork;
    if (mode == 0) {
        sp14 = mode;
        DjinnMenu_DrawStatPreview(menu->second_window, 0, 0, menu->pair_owner[1], 1, mode, 2, param, 1);
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pair_owner[0], mode, 1, 2, param, mode);
    } else {
        DjinnMenu_DrawStatPreview(menu->second_window, 0, 0, menu->shown_owner[1], 1, 0, ALT_PARAM, param, 1);
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shown_owner[0], 0, 0, 1, param, 0);
    }
    return 1;
}

#if EDITION_INTERNATIONAL
#define PREVIEW_DIGITS 3
#define PREVIEW_CURRENT_X 48
#define PREVIEW_MAX_X 80
#define PREVIEW_SLASH_X 72
#define PREVIEW_LUCK_X 56
#define PREVIEW_ARROW_X icon_x
#define PREVIEW_PP_X 88
#else
#define PREVIEW_DIGITS 4
#define PREVIEW_CURRENT_X 32
#define PREVIEW_MAX_X 72
#define PREVIEW_SLASH_X 64
#define PREVIEW_LUCK_X 48
#define PREVIEW_ARROW_X (x * 8 + 64)
#define PREVIEW_PP_X 56
#endif
static __inline__ void Unit_Copy(struct BattleUnit *dst, struct BattleUnit *src)
{
    Iwram_CopyWords(dst, src, sizeof(struct BattleUnit));
}

s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 owner,
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
    struct DjinnMenuWork *menu;

    state = Owner_GetStateFar(owner);
    menu = gMenuWork;
    give_element = menu->pair_element[give];
    give_index = menu->pair_index[give];
    give_set = (u16)(menu->pair_entries[give] & 0x8000);
    take_element = menu->pair_element[take];
    take_index = menu->pair_index[take];
    take_set = (u16)(menu->pair_entries[take] & 0x8000);
    saved = (struct BattleUnit *)Runtime_BumpAllocate(sizeof(struct BattleUnit));
    Unit_Copy(saved, state);

    if (page == 0) {
        if (mode == 3) {
            UiNumber_DrawAt(state->max_hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 56);
            UiNumber_DrawAt(state->max_pp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 64);
            UiNumber_DrawAt(state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
            UiNumber_DrawAt(state->pp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 56);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 64);
        } else {
            UiNumber_DrawAt(state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
            UiNumber_DrawAt(state->pp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
        }
        UiNumber_DrawAt(state->attack, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 72);
        UiNumber_DrawAt(state->defense, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 80);
        UiNumber_DrawAt(state->agility, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 88);
        UiNumber_DrawAt(state->luck, 2, window, x * 8 + PREVIEW_LUCK_X, y * 8 + 96);
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
        UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, x * 8 + 40, y * 8 + 16);
        UiNumber_DrawAt(state->level, 2, window, x * 8 + 88, y * 8 + 16);
        label = (s32)MsgPreviewStatLabel;
        UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
        UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
        UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
        UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
        UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
        UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
        UiText_DrawCharacterAtOffsetFar((s32)MsgClassName + saved->class_index, window, x * 8, y * 8 + 32);
    }

    {
        s32 i = 0;

    if (page == 0) {
        s32 column;
#if EDITION_INTERNATIONAL
        s32 icon_x;
#endif
        s32 px;

        if (saved->class_index != state->class_index) {
            px = x * 8;
            UiText_DrawCharacterAtOffsetFar((s32)MsgClassName + state->class_index, window, px, y * 8 + 48);
            UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
        }
        column = x;
        if (saved->class_index != state->class_index)
            column += 5;
        for (i = 0; i < 4; i++) {
            UiWindow_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5, 0);
            UiWindow_SetTilemapEntryFar(window,
                state->djinn_active_counts[i] + 0xf030,
                column + i * 2 + 1, y + 5, 0);
        }
#if EDITION_INTERNATIONAL
        icon_x = x * 8 + 70;
#endif
        if (state->hp != saved->hp) {
            UiNumber_DrawAt(state->hp, 4, window, x * 8 + 72, y * 8 + 56);
            if (state->hp > saved->hp)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 1);
        }
        if (state->pp != saved->pp) {
            UiNumber_DrawAt(state->pp, 4, window, x * 8 + 72, y * 8 + 64);
            if (state->pp > saved->pp)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 1);
        }
        if (state->attack != saved->attack) {
            UiNumber_DrawAt(state->attack, 4, window, x * 8 + 72, y * 8 + 72);
            if (state->attack > saved->attack)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 1);
        }
        if (state->defense != saved->defense) {
            UiNumber_DrawAt(state->defense, 4, window, x * 8 + 72, y * 8 + 80);
            if (state->defense > saved->defense)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 1);
        }
        if (state->agility != saved->agility) {
            UiNumber_DrawAt(state->agility, 4, window, x * 8 + 72, y * 8 + 88);
            if (state->agility > saved->agility)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 1);
        }
        if (state->luck != saved->luck) {
            UiNumber_DrawAt(state->luck, 2, window, x * 8 + 88, y * 8 + 96);
            if (state->luck > saved->luck)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 1);
        }
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
        count = OwnerAction_DiffSlots(saved->action_slots,
            state->action_slots, actions, &gained, &lost);
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
            UiText_DrawCharacterAtOffsetFar((s32)MsgAbilityName + (actions[first] & 0x3fff), window,
                x * 8 + 16, (y + line * 2) * 8 + 8);
            UiText_DrawNumberAtOffsetFar(Ability_GetData(actions[first])->pp_cost, 2, window,
                x * 8 + 88, (y + line * 2) * 8 + 8);
            line++;
        }
        UiWork_SetParamNibbleFar(15);
        UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, x * 8 + PREVIEW_PP_X, y * 8);
        if (mode != 3) {
            s32 lines;

            lines = 0;
            if (gained) {
                UiWork_SetParamNibbleFar(4);
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyGained, window, x * 8, y * 8 + 88);
                lines = 1;
            }
            if (lost) {
                UiWork_SetParamNibbleFar(2);
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyLost, window, x * 8, (y + lines) * 8 + 88);
                lines++;
            }
            if (lines == 0)
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyUnchanged, window, x * 8, y * 8 + 88);
            UiWork_SetParamNibbleFar(15);
            UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
        }
        gWindowWork->dirty = 1;
    }

    if (page == 0)
        SideObject_CreateFar(owner, 0, side, window, 0, 0);
    Unit_Copy(state, saved);
    Runtime_BumpFree(saved);
    return 1;
}
