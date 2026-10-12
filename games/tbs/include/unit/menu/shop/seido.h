#ifndef ALCHEMY_UNIT_MENU_SHOP_SEIDO_H
#define ALCHEMY_UNIT_MENU_SHOP_SEIDO_H

#include "types.h"

struct EffectSlot;

s32 Shop_ServicePrice(s32 entry_no, s32 kind);
s32 Shop_CanServe(s32 entry_no, s32 kind);
s32 Shop_CountUnits(void);
s32 Shop_MsgByMode(s32 value);
void UiMessage_ShowResolvedAndWait(s32 value);
void UiMessage_ShowResolvedAndRestoreState(s32 arg0);
s32 Shop_ConfirmAct(s32 unit_id);
s32 Sanctum_RunPartyService(void);
void BattleUnit_ResetStateByMode(s32 id, s32 mode);
void Shop_HiliteUnit(s32 enabled, s32 selected);
void Shop_DrawSelMsg(s32 target, s32 selection);
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect);
void Shop_ResetEffects(void);
void Shop_RunPartyMemberIconBurst(s32 member);

#endif
