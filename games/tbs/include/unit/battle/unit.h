#ifndef ALCHEMY_UNIT_BATTLE_UNIT_H
#define ALCHEMY_UNIT_BATTLE_UNIT_H

#include "types.h"

struct BtlObj;
struct FixedPointPosition;
struct MotionObject;

s32 BattleParty_PrepareActiveOwners(u16 *owners);
s32 BtlEnmList(s16 *unit_ids);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattleParty_ListActorIds(s32 groups, u16 *dst);
void BtlObjPut(void);
void BattleParty_ListAllUnitsAndSubmit(void);
s32 BattleMotion_GetSlotPalette(s32 id);
s32 EnmChk(s32 summon);
s32 SummonSlot_RegisterActorSprites(s32 unit);
s32 BattleMotion_ReleaseObjectSlotByValue(s32 value);
s32 Resource_FindFreeSlot(s32 key);
s32 BattleUnit_LookupWeaponValueByClass(s32 id);
void BtlObjMake(struct BtlObj *actor, s32 unit, s32 x, s32 y);
void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y);
void Summon_LayoutPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions);
s32 EnmSlotGet(void);
s32 EnmUpd(void);
void BattleUnit_RefreshPlacement(void);
s32 BattlePlacement_ContainsId(s16 *list, s32 no);
void BattleActor_UpdateSpritePriorities(void);
s32 BattleUnit_BuildStatusFlags(s32 id, struct BtlObj *slot);
s32 BattleStatusIcon_Cycle(struct BtlObj *slot);
s32 BattlePres_SetActorModeAndAction(s32 id);
void ReleaseBattleObjectRecords(s32 no);
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
struct BtlObj *BtlObjGet(s32 object_id);
void ResetMotionRecordGroup(void *owner);
void ResetBattleObjectRecordGroups(struct MotionObject *object);
s32 ActivateBattleObjectSlot(s32 object_id);
void *BattleMotion_DestroyAllSlotObjects(void);
s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out);
s32 Camera_ApplyTransformByFlag(void);
s32 BattleMotion_ProjectPosition(s32 id, s32 *projected);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);
void Camera_InitDefaultTransform(void);
void ObjMotRst(s32 slot_id);
void BattleMotion_SetupEscapeObject(s32 object_id);
void BattleMotion_InterpolatePosition(struct BtlObj *start_slot, struct BtlObj *end_slot, s32 progress);
void BattleMotion_SetObjectPosition(struct BtlObj *slot);
void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id);
void BattleMotion_InitializeObject(s32 id);
void BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
void BattleMotion_ApproachTarget(s32 actor_id, s32 target_id, s32 travel_divisor, s32 initial_velocity_y);
void BattleMotion_ResetSlotObjectMode2(s32 id);
void BattleMotion_ReservedNoOp83B0();
void BattleMotion_ApplyPairMidpoint(s32 first_id, s32 second_id);
void BattleMotion_SpawnSlotEffectAndWait(s32 id);
s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected);
s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected);
s32 Battle_GetObjectTableValue(s32 id);

#endif
