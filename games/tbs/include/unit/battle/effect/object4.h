#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT4_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT4_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"

struct ActionDescriptor;
struct EffectDescriptor;
struct MotionObject;

struct ActionDescriptor *BattleAction_FindDescriptor(s32 id);
s32 GameFlag_IsConditionActive(s32 condition);
u32 BattleFx_CheckDescriptorKind3Result(s32 flags, s32 result);
struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value);
void Runtime_CallWorkBlockEntry(void);
s32 BattleFx_FindDescriptorWithOverride(s32 id);
s32 BattleFx_RunDescriptorAction(s32 id);
s32 BattleFx_RunKind6DescriptorAction(s32 value);
s32 BattleAction_RunDescriptor(s32 value);
s32 Battle_DispatchInputEvent(s32 event);
s32 Field_RunTileAction(s32 tile);
s32 BattleFx_MapKeyThroughTable(s32 mode);
s32 BattleEffect_SelectNearbyObject(s32 id);
s32 BattleEffect_SelectNearbyTargetObject(s32 id, s32 mode);
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode);
void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette);
void Battle_ResetEffectCounter(void);
s32 Event_FindFacingTrigger(s32 source);
s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot);
s32 BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result);
s32 BattleFx_ExecutePackedAbilityEffect(s32 packed);
s32 BattleCommand_ExecuteSelectedAction(u32 code);
s32 BattleFx_HasMatchingEvent5(s32 group);
u32 BattleFx_HasTrigger(u16 item);

#endif
