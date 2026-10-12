#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT11_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT11_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"

struct UiWindow;

void Event_SetMessageId(s32 value);
s32 ObjectTable_ReadActiveValue(s32 key);
s32 ObjectTable_FindActiveByValue(s32 value);
struct UiWindow *UiText_OpenMessageAtObject(s32 arg, s32 unused);
void BattleEv_RunWait(s32 action, s32 flag);
void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value);
s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id);
void BattleEventRuntime_EmptyCallback(void);
void Event_ShowSpeakerMessage(s32 speaker);
void ObjectTable_CallRefreshHook(s32 unused0, s32 unused1, s32 x, s32 y);
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void ObjectTable_RunIfActive(s32 id);
void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 first_x, s32 first_y, s32 first_arg, s32 first_extra, s32 second, s32 second_x, s32 second_y, s32 second_arg, s32 second_extra);
void Ui_SetRenderResultFromObject(s32 arg0);

#endif
