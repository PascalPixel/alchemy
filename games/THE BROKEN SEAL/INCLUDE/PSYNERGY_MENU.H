#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

#include "LAYOUT_GUARD.H"
#include "MENU_RESULT.H"
#include "RENDER_INPUT.H"

struct AnimationObject;

struct BattleUnit;

/*
 * The menu-runtime cell is polymorphic.  This view belongs to the
 * Psynergy menu; keep it separate from InventoryMenuState even where the two
 * modes deliberately share field offsets.
 */
struct PsynergyMenuState {
    u8 unknown_000[8];
    s32 selected_owner;             /* 0x008 */
    u8 unknown_00c[4];
    s32 auxiliary_window;           /* 0x010 */
    struct RenderOutput *pane_icon[2]; /* 0x014 */
    s8 tab_index[2];                /* 0x01c */
    u8 tab_counts[2];               /* 0x01e */
    s32 psynergy_window;            /* 0x020 */
    s32 status_window;              /* 0x024 */
    s32 shortcut_window;            /* 0x028 */
    void *info_window;              /* 0x02c */
    u8 unknown_030[4];
    s32 list_window;                /* 0x034 */
    u8 unknown_038[0x0c];
    struct RenderOutput *entry_grid_cursor; /* 0x044 */
    struct RenderOutput *entry_icons[32]; /* 0x048 */
    u8 unknown_0c8[0x44];
    s32 message_window;             /* 0x10c */
    u8 selected_column;             /* 0x110 */
    u8 selected_row;                /* 0x111 */
    u8 column_count;                /* 0x112 */
    u8 row_count;                   /* 0x113 */
    struct AnimationObject *tab_objects[8];             /* 0x114 */
    u16 tab_x[8];                   /* 0x134 */
    u16 row_positions[8];           /* 0x144 */
    s32 owner_scale[8];             /* 0x154, projected portrait size */
    u16 pane_row[2];                /* 0x174 */
    u16 pane_action[2];             /* 0x178 */
    u8 unknown_17c[0x4c];
    u16 psynergies[32];             /* 0x1c8 */
    u16 owner_table[8];             /* 0x208 */
    u8 psynergy_count;              /* 0x218 */
    u8 owner_count;                 /* 0x219 */
    u8 owner_ids[2];                /* 0x21a */
    struct RenderOutput *cursor_icon; /* 0x21c */
    u16 flags;                     /* 0x220 */
    u8 unknown_222[0x1a];
    s16 slot_y[4];                 /* 0x23c */
    u8 unknown_244[0x1c];
    s8 selected_index_by_owner[8]; /* 0x260 */
    u8 mode;                      /* 0x268 */
};

LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_AuxiliaryWindow,
    struct PsynergyMenuState,
    auxiliary_window,
    0x010);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_PsynergyWindow,
    struct PsynergyMenuState,
    psynergy_window,
    0x020);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_EntryGridCursor,
    struct PsynergyMenuState,
    entry_grid_cursor,
    0x044);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_EntryIcons,
    struct PsynergyMenuState,
    entry_icons,
    0x048);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_MessageWindow,
    struct PsynergyMenuState,
    message_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_SelectedColumn,
    struct PsynergyMenuState,
    selected_column,
    0x110);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_SelectedRow,
    struct PsynergyMenuState,
    selected_row,
    0x111);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_ColumnCount,
    struct PsynergyMenuState,
    column_count,
    0x112);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_RowCount,
    struct PsynergyMenuState,
    row_count,
    0x113);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_SelectedPsynergy,
    struct PsynergyMenuState,
    pane_action[0],
    0x178);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_Psynergies,
    struct PsynergyMenuState,
    psynergies,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_PsynergyCount,
    struct PsynergyMenuState,
    psynergy_count,
    0x218);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_OwnerIds,
    struct PsynergyMenuState,
    owner_ids,
    0x21a);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuState_SelectedIndexByOwner,
    struct PsynergyMenuState,
    selected_index_by_owner,
    0x260);

LAYOUT_OFFSET_GUARD(PsynergyMenuState_TabObjects, struct PsynergyMenuState, tab_objects, 0x114);
LAYOUT_OFFSET_GUARD(PsynergyMenuState_OwnerScale, struct PsynergyMenuState, owner_scale, 0x154);

extern struct PsynergyMenuState *gMenuWork;
extern char MsgPsynergyMenuEmpty;

/* Shared icon-grid helper used by both inventory and Psynergy menu modes. */

s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 PsynergyMenu_SetupActionIcons(u16 *owner_ids, u16 *unused);
void PsynergyMenu_RefreshOwnerPsynergy(s32 owner);
s32 PsynergyMenu_ReturnTrue(void);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
void PsynergyMenu_CreateEntryGrid(void);
void PsynergyMenu_CloseWindows(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
s32 PsynergyMenu_CollectActions(struct BattleUnit *owner, u16 *actions, s32 mode);
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 owner_index);
s32 PsynergyMenu_IsActionRestricted(s32 encoded_action);
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner);
void PsynergyMenu_PreparedIconsNoOp(void);
void Menu_HideEmptyEntryIcons(const u16 *entries);

#endif
