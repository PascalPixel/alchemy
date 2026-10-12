#ifndef ALCHEMY_UNIT_MENU_ITEM_PSYNERGY_H
#define ALCHEMY_UNIT_MENU_ITEM_PSYNERGY_H

#include "types.h"

struct BtlUnit;
struct MenuResult;

s32 Menu_OpenConfirmPrompt(void);
s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action);
s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 PsynergyMenu_SelectPartySlot(s32 party_slot);
s32 PsynergyMenu_SetupActionIcons(u16 *owner_ids, u16 *unused);
void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id);
s32 PsynergyMenu_ReturnTrue(void);
void PsynergyMenu_NoOp(void);
s32 PsynergyMenu_SelectTarget(s32 mode);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner);
void PsynergyMenu_CreateEntryGrid(void);
void PsynergyMenu_CloseWindows(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
s32 PsynergyMenu_CollectActions(struct BtlUnit *owner, u16 *actions, s32 mode);
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index);
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state);
s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state);
s32 PsynergyMenu_RunList(s32 pane);
s32 PsynergyMenu_IsActionRestricted(s32 no);

#endif
