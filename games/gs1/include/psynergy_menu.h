#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

#include "layout_guard.h"
#include "menu_result.h"

struct OwnerActionState;

/*
 * The menu-runtime cell is polymorphic.  This view belongs to the
 * Psynergy menu; keep it separate from ItemMenuState even where the two
 * modes deliberately share field offsets.
 */
struct PsynergyMenuIcon {
    u8 unknown_00[5];
    u8 state;                       /* 0x05 */
    u16 x;                          /* 0x06 */
    u16 y;                          /* 0x08 */
    u8 unknown_0a[4];
    u8 render_target;               /* 0x0e */
};

struct PsynergyMenuState {
    u8 unknown_000[0x10];
    s32 auxiliary_window;           /* 0x010 */
    u8 unknown_014[0x0c];
    s32 psynergy_window;            /* 0x020 */
    u8 unknown_024[0x20];
    struct PsynergyMenuIcon *entry_grid_cursor; /* 0x044 */
    struct PsynergyMenuIcon *entry_icons[32];   /* 0x048 */
    u8 unknown_0c8[0x44];
    s32 message_window;             /* 0x10c */
    u8 selected_column;             /* 0x110 */
    u8 selected_row;                /* 0x111 */
    u8 column_count;                /* 0x112 */
    u8 row_count;                   /* 0x113 */
    u8 unknown_114[0x64];
    u16 selected_psynergy;          /* 0x178 */
    u8 unknown_17a[0x4e];
    u16 psynergies[32];             /* 0x1c8 */
    u8 unknown_208[0x10];
    u8 psynergy_count;              /* 0x218 */
    u8 unknown_219;
    u8 owner_ids[8];                /* 0x21a */
    u8 unknown_222[0x3e];
    s8 selected_index_by_owner[8];  /* 0x260 */
};

/* The shared cell is likewise mode-owned while the Psynergy menu is active. */
struct PsynergyMenuGlobalState {
    u8 unknown_000[0x220];
    u16 psynergy_shortcuts[2];      /* 0x220 */
};

LAYOUT_OFFSET_GUARD(
    PsynergyMenuIcon_State,
    struct PsynergyMenuIcon,
    state,
    0x05);
LAYOUT_OFFSET_GUARD(
    PsynergyMenuIcon_RenderTarget,
    struct PsynergyMenuIcon,
    render_target,
    0x0e);
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
    selected_psynergy,
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
LAYOUT_OFFSET_GUARD(
    PsynergyMenuGlobalState_PsynergyShortcuts,
    struct PsynergyMenuGlobalState,
    psynergy_shortcuts,
    0x220);

extern struct PsynergyMenuState *Data_03001f2c;
extern struct PsynergyMenuGlobalState Data_02000240;
extern char Value_00000af2;

/* Shared icon-grid helper used by both inventory and Psynergy menu modes. */

#define PsynergyMenu_EmptyMessage              Value_00000af2

s32 PsynergyMenu_ClassifySelectedPsynergy(void);
void PsynergyMenu_RefreshOwnerPsynergy(s32 owner);
s32 PsynergyMenu_ReturnTrue(void);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
void PsynergyMenu_CreateEntryGrid(void);
void PsynergyMenu_CloseWindows(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
u8 PsynergyMenu_CollectActions(struct OwnerActionState *owner, u16 *actions, s32 mode);
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 owner_index);
s32 PsynergyMenu_IsActionRestricted(s32 encoded_action);
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner);
void PsynergyMenu_PreparedIconsNoOp(void);
void Menu_HideEmptyEntryIcons(const u16 *entries);

#endif
