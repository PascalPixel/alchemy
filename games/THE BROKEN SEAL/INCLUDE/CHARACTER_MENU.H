#ifndef ALCHEMY_CHARACTER_MENU_H
#define ALCHEMY_CHARACTER_MENU_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "ANIMSPR.H"
#include "WINDOW.H"

struct BattleUnit;
struct OwnerActionSlot;

/* The character and command pages allocated by Menu_OpenCharacterSelector
   and ActionMenu_Open. Inventory and Psynergy keep their own mode records. */
struct CharacterMenuState {
    u8 unknown_000[8];
    s32 selected_owner;
    u8 unknown_00c[4];
    struct UiWindow *owner_window;
    struct RenderOutput *owner_cursors[2];
    s8 owner_index[2];
    s8 owner_count[2];
    struct UiWindow *action_window;
    struct UiWindow *status_window;
    struct UiWindow *unknown_window_028;
    struct UiWindow *help_window;
    u8 unknown_030[0x18];
    struct RenderOutput *entry_icons[32];
    struct RenderOutput *category_icons[8];
    u8 unknown_0e8[0x24];
    struct UiWindow *selector_window;
    u8 selected_column;
    u8 selected_row;
    u8 column_count;
    u8 row_count;
    struct AnimationObject *owner_objects[8];
    s16 owner_x[8];
    s16 owner_y[8];
    s32 owner_scale[8];
    u16 selected_slots[2];
    u16 selected_actions[2];
    struct RenderOutput *status_cursor;
    u8 unknown_180[0x48];
    u16 entries[32];
    u16 owner_ids[8];
    u8 entry_count;
    u8 party_count;
    u8 selected_ids[2];
    struct RenderOutput *selected_icon;
    u16 page;
    u8 unknown_222[2];
    struct AnimationObject *row_objects[4];
    s16 row_x[4];
    s16 row_y[4];
};

LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerWindow, struct CharacterMenuState, owner_window, 0x10);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerCursors, struct CharacterMenuState, owner_cursors, 0x14);
LAYOUT_OFFSET_GUARD(CharacterMenuState_StatusWindow, struct CharacterMenuState, status_window, 0x24);
LAYOUT_OFFSET_GUARD(CharacterMenuState_EntryIcons, struct CharacterMenuState, entry_icons, 0x48);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectorWindow, struct CharacterMenuState, selector_window, 0x10c);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectedColumn, struct CharacterMenuState, selected_column, 0x110);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerObjects, struct CharacterMenuState, owner_objects, 0x114);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerY, struct CharacterMenuState, owner_y, 0x144);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectedSlots, struct CharacterMenuState, selected_slots, 0x174);
LAYOUT_OFFSET_GUARD(CharacterMenuState_StatusCursor, struct CharacterMenuState, status_cursor, 0x17c);
LAYOUT_OFFSET_GUARD(CharacterMenuState_OwnerIds, struct CharacterMenuState, owner_ids, 0x208);
LAYOUT_OFFSET_GUARD(CharacterMenuState_SelectedIds, struct CharacterMenuState, selected_ids, 0x21a);
LAYOUT_OFFSET_GUARD(CharacterMenuState_Page, struct CharacterMenuState, page, 0x220);
LAYOUT_OFFSET_GUARD(CharacterMenuState_RowObjects, struct CharacterMenuState, row_objects, 0x224);
LAYOUT_OFFSET_GUARD(CharacterMenuState_RowX, struct CharacterMenuState, row_x, 0x234);
LAYOUT_SIZE_GUARD(CharacterMenuState_PrefixSize, struct CharacterMenuState, 0x244);

enum CharacterAilment {
    CHARACTER_DOWN,
    CHARACTER_POISON,
    CHARACTER_VENOM,
    CHARACTER_CURSE,
    CHARACTER_HAUNT,
    CHARACTER_AILMENT_COUNT
};

s32 Menu_OpenCharacterSelector(void);
s32 CharacterSelector_Run(void);
s32 CharacterSelector_RunRearrange(void);
s32 CharacterSelector_MoveEntry(s32 selected, s32 direction);
s32 CharacterMenu_SelectOwner(s32 slot);
s32 CharacterMenu_SelectCommand(void);
s32 Menu_RunActionFlow(void);
/* Existing callers supply extra words to this zero-argument definition. */
void Menu_InitSelectorCursorAndEntries();
void Menu_CreateWindowAndEntryObjects(s32 owner, s32 page);
void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 owner, s32 mode);
s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 owner);
void CharacterMenu_DrawSelectionCursor(s32 pane, s32 selected, u8 *entries, s32 invert);
void CharacterMenu_DrawSelectionLabels(s32 window, s32 selected, const u8 *entries);
void StatusMenu_ShowOwnerProgressMessage(struct UiWindow *window, s32 variant, s32 preserve);
void Menu_DrawOwnerStatusPanel(s32 unused, s32 owner, s32 slot, s32 mode);
void Render_SetTilemapFlagRect(const struct UiWindow *window, s32 x, s32 y, s32 width, s32 height, u32 attributes);
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *old_slots, const struct OwnerActionSlot *new_slots, u16 *out, s32 *gained, s32 *lost);

#endif
