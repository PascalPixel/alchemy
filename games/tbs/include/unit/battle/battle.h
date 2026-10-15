#ifndef ALCHEMY_UNIT_BATTLE_BATTLE_H
#define ALCHEMY_UNIT_BATTLE_BATTLE_H

#include "types.h"

struct BattleActionRecord;
struct BattleEventState;
struct BattlePresentationWork;
struct BattleSummonState;
struct BattleTransitionEntry;
struct BattleAct;
struct BattleObj;
struct BattlePlan;
struct FixedPointPosition;
struct MotionObject;

s32 DebugParty_LoadPreset(s32 preset);
void DebugBattle_ViewMessages(void);
void DebugBattle_RunMenu(void);
void BattlePresentation_UpdateCamera(void);
void BattleIntro_AnnounceEncounter(s32 enemy_count);
void BattleParty_CollectUnitList(void);
s32 BattlePresentation_InitializeTilePattern(void);
void BattlePresentation_ClearBg0VerticalOffset(void);
void Party_ReservedNoOp5B14(s32 unused);
s32 BattleParty_ResetActiveRuntimeFields(void);
s32 BattleParty_ReconcileDjinnRecovery(void);
struct BattleSummonState *BattleSummon_UpdateAvailability(void);
void BattleSummon_ReservedNoOp(void);
s32 UpdateNameEntries(void);
s32 LinkBattle_SendParty(void);
u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source);
s32 BattlePres_WaitSync(void);
s32 BattlePres_SyncTurn(void);
s32 BattleParty_AssignMemberSlots(void);
s32 Battle_ClearSerialTransferPayload(void);
s32 Battle_RunEncounter(s32 enemyGroup);
void BattlePres_AdjustCameraByShoulderKeys(void);
void BattlePres_AdjustCameraByShoulderKeysAlt(void);
s32 BattleUnit_TickDefenseModifier(s32 unitId);
s32 BattleUnit_TickResistanceModifier(s32 unitId);
s32 BattleUnit_TickDelusion(s32 unitId);
s32 BattleUnit_TickCharm(s32 unitId);
s32 BattleUnit_TickStun(s32 unitId);
s32 BattleUnit_TickSleep(s32 unitId);

#define MESSAGE_PUSH(m) BattleEvPush(BATTLE_EVENT_MESSAGE, (m))

// L・R を押しているあいだカメラの yaw を 512 ずつ動かす。同じ関数がふたつある
#define DEFINE_ADJUST_CAMERA(name)                                                           \
	void name(void)                                                                          \
	{                                                                                        \
		struct BattleCamera					*cam = WORK_SLOT(HEAP_SLOT_CAMERA);              \
		struct BattlePresentationTransition *trans = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND); \
                                                                                             \
		if ((gKeysHeld & KEY_L) != 0)                                                        \
		{                                                                                    \
			cam->yaw += 512;                                                                 \
		}                                                                                    \
		if ((gKeysHeld & KEY_R) != 0)                                                        \
		{                                                                                    \
			cam->yaw -= 512;                                                                 \
		}                                                                                    \
		if (trans->flag == 0)                                                                \
		{                                                                                    \
			BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);                        \
		}                                                                                    \
	}

s32 BattleParty_PrepareActiveOwners(u16 *owners);
s32 BattleEnemyList(s16 *unit_ids);
s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids);
s32 BattleParty_ListActorIds(s32 groups, u16 *dst);
void BattleObjPut(void);
void BattleParty_ListAllUnitsAndSubmit(void);
s32 BattleMotion_GetSlotPalette(s32 id);
s32 EnemyCheck(s32 summon);
s32 SummonSlot_RegisterActorSprites(s32 unit);
s32 BattleMotion_ReleaseObjectSlotByValue(s32 value);
s32 Resource_FindFreeSlot(s32 key);
s32 BattleUnit_LookupWeaponValueByClass(s32 id);
void BattleObjMake(struct BattleObj *actor, s32 unit, s32 x, s32 y);
void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y);
void Summon_LayoutPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions);
s32 EnemySlotGet(void);
s32 EnemyUpdate(void);
void BattleUnit_RefreshPlacement(void);
s32 BattlePlacement_ContainsId(s16 *list, s32 no);
void BattleActor_UpdateSpritePriorities(void);
s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObj *slot);
s32 BattleStatusIcon_Cycle(struct BattleObj *slot);
s32 BattlePres_SetActorModeAndAction(s32 id);
void ReleaseBattleObjectRecords(s32 no);
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
struct BattleObj *BattleObjGet(s32 objectId);
void ResetMotionRecordGroup(void *owner);
void ResetBattleObjectRecordGroups(struct MotionObject *object);
s32 ActivateBattleObjectSlot(s32 objectId);
void *BattleMotion_DestroyAllSlotObjects(void);
s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out);
s32 Camera_ApplyTransformByFlag(void);
s32 BattleMotion_ProjectPosition(s32 id, s32 *projected);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);
void Camera_InitDefaultTransform(void);
void ObjMotRst(s32 slot_id);
void BattleMotion_SetupEscapeObject(s32 objectId);
void BattleMotion_InterpolatePosition(struct BattleObj *start_slot, struct BattleObj *end_slot, s32 progress);
void BattleMotion_SetObjectPosition(struct BattleObj *slot);
void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId);
void BattleMotion_InitializeObject(s32 id);
void BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travel_divisor, s32 initial_velocity_y);
void BattleMotion_ResetSlotObjectMode2(s32 id);
void BattleMotion_ReservedNoOp83B0();
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 second_id);
void BattleMotion_SpawnSlotEffectAndWait(s32 id);
s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected);
s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected);
s32 Battle_GetObjectTableValue(s32 id);

s32 BattlePresentation_BuildSortedUnitEntries(struct BattleActionRecord *entries);
s32 BattlePres_RunAction(struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 objectId);
s32 BattleEscape_PlayRun(struct BattleActionRecord *action);
s32 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action);
s32 BattlePres_RunApproachAction(struct BattleActionRecord *input);
s32 BattlePres_RunUnitAction(struct BattleActionRecord *action);
void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 show_message, u8 *context);
void BattleMotion_SetMode5AndActivateSlot(s32 unitId);
s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action);

void Palette_UpdatePulseBrightness(void);
void Camera_ConfigureScene(s32 pos);
void BattleUnit_ResetGuardLevels(void);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
void Palette_CopyBanksWithBrightnessOffset(s32 offset);
s32 BattlePresentation_BuildActions(struct BattleActionRecord *acts);
s32 Battle_GetTaggedSlotValue(s32 slot);
s32 Battle_FindTaggedSlotByValue(u32 id);
void Battle_ReservedNoOp9B2C(void);

s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay);
s32 BattlePres_BuildTargetList(struct BattlePlan *plan, struct BattlePresentationWork *work);
s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattlePlan *plan);
s32 BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode);
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused);
s32 RunBattlePresentation(struct BattlePlan *plan, s32 mode);
s32 BattlePres_RunSimple(struct BattlePlan *input, s32 flags);
s32 BattlePres_RunItem(struct BattlePlan *plan, s32 unused, struct BattleActionRecord *selection);
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value);
s32 BattlePres_RunPsynergyCast(struct BattlePlan *plan, s32 mode);

void BattleMotion_RunValueSequence(s32 id, s32 unused0, s32 unused1);
void BattleActor_RemoveFromLists(s32 id);
void BattleMotion_InitializeActorRecords(s32 id);
s32 BattleTarget_SelectRandomPosition(s32 require_living_unit);
s32 BattleTarget_SelectForAction(s32 actorId, struct BattleAct *action);
s32 BattleActor_ResetRuntimeFields(s32 id);
s32 BattlePresentation_WaitForAdvance(void);
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y);
void BattleEv_MarkDisplay(void);
s32 BattleActor_DestroyTemporaryObject(s32 arg0);
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand);
u32 BattleEv_DispatchQueued(void);
u32 BattleEvPush(u32 opcode, u32 operand);
s32 BattleEffectDeadCheck(s32 effect);
s32 BattleTargetExec(struct BattlePlan *plan, s32 slot);
u32 Ability_CheckStatusOrSpecialId(s32 abilityId);
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *req, s32 retry);
void Dma_StopAllThenRunHook(void);
void BattleEventRuntime_BeginPhase(s32 cue);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
void BattleEvent_Playback(void);
u32 BattleEventRuntime_Reset(void);
s32 BattleEventRuntime_WaitForReady(void);
u32 BattleParty_IsUnitListed(u32 id);
s32 BattlePlacement_CountValidEntries(u32 unitId, u8 *counts);
s32 BattleCommand_BuildPlan(struct BattleActionRecord *req, struct BattlePlan *plan);

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 BattleUnit_TickAttackModifier(s32 unitId);
s32 BattleUnit_TickConfusion(s32 unitId);
s32 Battle_AdvanceCounterAndCheckChance(s32 id);
s32 BattleUnit_TickRefrain(s32 unitId);
s32 BattleUnit_TickReflect(s32 id);
s32 BattleUnit_TickAgilityModifier(s32 id);
s32 BattlePlacement_UpdateTimedEntries(void);
s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void);
void Battle_ReservedNoOpF674(void);
s32 Battle_ProcessRoundEnd(void);
s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan);

s32 BattlePres_RunMosaicFadeOut(void);
void Graphics_BuildSequentialTileTable(s32 *dst);
void BattlePresentation_BuildTilemap(s32 *destination);
void BattlePres_UpdateHBlankScroll(void);
void BattlePresentation_UploadTileVariant(void);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_DrawTransitionRows(void);
void Graphics_ClearBg0Vofs(void);
void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind);
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level);
void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 value, s32 fade);
void BattlePresentation_InitializeWorkAndResetState(void);
void Runtime_ReleaseHeapBlock10(void);
void BattleBackground_Load(s32 mode, s32 resource, s32 level);
void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);
void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom);
void BattlePres_SetupTransitionAtPairMidpoint(s32 a, s32 b, s32 zoom);
void Graphics_FadeAlphaOut(void);
void Graphics_FadeAlphaIn(void);
void Graphics_SetBlendControl(void);
void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries);
s32 BattlePres_DivideBy16(s32 value);
s32 Runtime_AdjustDebugValueWithButtons(s32 ret);
void BattlePres_SetActorRecordMode(s32 actorId, s32 mode);
void BattlePres_SetOtherActorRecordModes(s32 except);
void BattlePres_ClearAllActorRecordModes(void);
void Graphics_AdvancePaletteCycle(void);
void BattlePres_SetActorModes(u16 *actors, s32 mode);

s32 BattleFx_UpdateStarField(void);
u32 Graphics_UploadVramBlock(void);
s32 BattleFx_InitializeStarField(s32 mode);
s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
/* BattleFx_PlayUnitElementEffect is called before its definition with no prototype in scope. */
void BattlePres_RunWithZeroArguments(void);
void Runtime_RemoveIrqHandlerSlot2(void);

s32 Party_ComputeEligibleMemberAverage(s32 recordId);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin);
s32 Owner_ApplyLevelGains(s32 owner, s32 levels);
s32 EnemyCountDecrement(s32 no, s32 n);
s32 EnemyCountRelease(s32 actorId);
s32 EnemyCountClear(s32 classId);
s32 BattleGroupRandom(s32 recordId);
s32 BattleFormation_BuildEnemyList(s32 recordId);

u32 Summon_GetLayoutKind(s32 index);
s32 Summon_GetEntryValue(s32 index);
s32 Summon_GetEntryFlag1Field(s32 index);
s32 Summon_IsEntryFlagged(s32 index);
u32 Battle_GetSpriteFlagNibble(u32 index);
u32 Battle_GetSpriteFlagHighBits(u32 index);
s32 Summon_IsEntrySecondaryFlagged(s32 index);
s32 Summon_GetEntryByte4(s32 index);

u32 Item_EncodeBankedId(u32 value);
void Summon_ClearWorkFields(void);
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 earned);
void Battle_AwardSpoils(void);
void Battle_ReservedNoOp2A08(void);

#endif
