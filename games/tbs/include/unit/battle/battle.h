#ifndef ALCHEMY_UNIT_BATTLE_BATTLE_H
#define ALCHEMY_UNIT_BATTLE_BATTLE_H

#include "types.h"

struct BattleActionRecord;
struct BattleEventState;
struct BattleEffectArgument;
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
void BattleIntro_AnnounceEncounter(s32 enemyCount);
void BattleParty_CollectUnitList(void);
s32 BattlePresentation_InitializeTilePattern(void);
void BattlePresentation_ClearBg0VerticalOffset(void);
void Party_ReservedNoOp5B14(s32 unused);
s32 BattleParty_ResetActiveRuntimeFields(void);
s32 BattleParty_ReconcileDjinnRecovery(void);
struct BattleSummonState *BattleSummon_UpdateAvailability(void);
void BattleSummon_ReservedNoOp(void);
s32 LinkBattle_ReceiveParty(void);
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

// Pushes a message to show, waiting for the player to read it.
#define PUSH_MESSAGE(message) BattleEvPush(BATTLE_EVENT_MESSAGE, (message))

// Move camera yaw by 512 while L/R is held; two identical functions exist.
#define DEFINE_ADJUST_CAMERA(name)                                             \
  void name(void) {                                                            \
    struct BattleCamera *cam = WORK_SLOT(HEAP_SLOT_CAMERA);                    \
    struct BattlePresentationTransition *trans =                               \
        WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);                                \
                                                                               \
    if ((gKeysHeld & KEY_L) != 0) {                                            \
      cam->yaw += 512;                                                         \
    }                                                                          \
    if ((gKeysHeld & KEY_R) != 0) {                                            \
      cam->yaw -= 512;                                                         \
    }                                                                          \
    if (trans->flag == 0) {                                                    \
      BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);                \
    }                                                                          \
  }

s32 BattleParty_PrepareActiveOwners(u16 *owners);
s32 BattleEnemyList(s16 *unitIds);
s32 BattleParty_ListLivingUnits(s32 sideMask, u16 *unitIds);
s32 BattleParty_ListActorIds(s32 sideMask, u16 *unitIds);
void BattleObjPut(void);
void BattleParty_ListAllUnitsAndSubmit(void);
s32 BattleMotion_GetSlotPalette(s32 unitId);
s32 EnemyCheck(s32 classId);
s32 SummonSlot_RegisterActorSprites(s32 unitId);
s32 BattleMotion_ReleaseObjectSlotByValue(s32 unitId);
s32 BattleUnit_GetWeaponKind(s32 itemId);
s32 BattleUnit_LookupWeaponValueByClass(s32 unitId);
void BattleObjMake(struct BattleObj *actor, s32 unitId, s32 x, s32 y);
void BattlePlacement_GetStepPair(s32 partyPlace, s32 *x, s32 *y);
void Summon_LayoutPositions(u16 *actorIds, s32 count, s32 *xPositions,
                            s32 *zPositions);
s32 EnemySlotGet(void);
s32 EnemyUpdate(void);
void BattleUnit_RefreshPlacement(void);
s32 BattlePlacement_ContainsId(s16 *list, s32 place);
void BattleActor_UpdateSpritePriorities(void);
s32 BattleUnit_BuildStatusFlags(s32 unitId, struct BattleObj *battleObject);
s32 BattleStatusIcon_Cycle(struct BattleObj *battleObject);
s32 BattlePres_SetActorModeAndAction(s32 unitId);
void ReleaseBattleObjectRecords(s32 unitId);
void BattleActor_SpawnObjectsForList(s16 *unitIds, s32 refreshPoses);
struct BattleObj *BattleObjGet(s32 unitId);
void ResetMotionRecordGroup(void *recordGroup);
void ResetBattleObjectRecordGroups(struct MotionObject *object);
s32 BattleObj_MarkForRelease(s32 unitId);
void *BattleMotion_DestroyAllSlotObjects(void);
s32 Battle_GetWorkEntryPair(s32 place, struct FixedPointPosition *position);
s32 Camera_ApplyTransformByFlag(void);
s32 BattleMotion_ProjectPosition(s32 unitId, s32 *projected);
void *GetMotionRecord(struct MotionObject *object, s32 recordIndex);
void Camera_InitDefaultTransform(void);
void ObjMotRst(s32 unitId);
void BattleMotion_SetupEscapeObject(s32 unitId);
void BattleMotion_InterpolatePosition(struct BattleObj *startSlot,
                                      struct BattleObj *endSlot, s32 progress);
void BattleMotion_SetObjectPosition(struct BattleObj *battleObject);
void BattleMotion_ResetObjectAtScaledAnchor(s32 unitId);
void BattleMotion_InitializeObject(s32 unitId);
void BattleMotion_ApplyVariantMotion(s32 unitId, s32 variant);
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY);
void BattleMotion_ResetSlotObjectMode2(s32 unitId);
void BattleMotion_ReservedNoOp83B0();
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId);
void BattleMotion_SpawnSlotEffectAndWait(s32 unitId);
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *projected);
s32 BattleMotion_ProjectConditionalPosition(s32 unitId, s32 *projected);
s32 Battle_GetObjectTableValue(s32 unitId);

s32 BattlePresentation_BuildSortedUnitEntries(
    struct BattleActionRecord *entries);
s32 BattlePres_RunAction(struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 unitId);
s32 BattleEscape_PlayRun(struct BattleActionRecord *action);
s32 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action);
s32 BattlePresentation_RunPairedUnitTransition(
    struct BattleActionRecord *action);
s32 BattlePres_RunApproachAction(struct BattleActionRecord *input);
s32 BattlePres_RunUnitAction(struct BattleActionRecord *action);
void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 showMessage,
                                        u8 *context);
void BattleMotion_SetMode5AndActivateSlot(s32 unitId);
s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action);

void Palette_UpdatePulseBrightness(void);
void Camera_ConfigureScene(s32 verticalOffset);
void BattleUnit_ResetGuardLevels(void);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions,
                                           s32 count);
void Palette_CopyBanksWithBrightnessOffset(s32 offset);
s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions);
s32 Battle_GetTaggedSlotValue(s32 taggedPlace);
s32 Battle_FindTaggedSlotByValue(u32 unitId);
void Battle_ReservedNoOp9B2C(void);

s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request,
                                      s32 delay);
s32 BattlePres_BuildTargetList(struct BattlePlan *plan,
                               struct BattleEffectArgument *effect);
s32 BattlePres_RunEscape(struct BattlePlan *plan);
s32 BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode);
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused);
s32 BattlePres_RunAttack(struct BattlePlan *plan, s32 mode);
s32 BattlePres_RunPlainAttack(struct BattlePlan *input, s32 flags);
s32 BattlePres_RunItem(struct BattlePlan *plan, s32 unused,
                       struct BattleActionRecord *selection);
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 palette);
s32 BattlePres_RunPsynergyCast(struct BattlePlan *plan, s32 mode);

void BattleMotion_FlashHit(s32 unitId, s32 unused0, s32 unused1);
void BattleActor_RemoveFromLists(s32 unitId);
void BattleMotion_ShowDowned(s32 unitId);
s32 BattleTarget_SelectRandomPosition(s32 requireLivingUnit);
s32 BattleTarget_SelectForAction(s32 actorId, struct BattleAct *action);
s32 BattleActor_ResetRuntimeFields(s32 unitId);
s32 BattlePresentation_WaitForAdvance(void);
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y);
void BattleEv_MarkDisplay(void);
s32 BattleActor_DestroyTemporaryObject(s32 unitId);
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand);
u32 BattleEv_DispatchQueued(void);
u32 BattleEvPush(u32 opcode, u32 operand);
s32 BattleEffectDeadCheck(s32 effect);
s32 BattleTargetExec(struct BattlePlan *plan, s32 slot);
u32 Ability_CheckStatusOrSpecialId(s32 abilityId);
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *request,
                                  s32 retry);
void Dma_StopAllThenRunHook(void);
void BattleEventRuntime_BeginPhase(s32 cue);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
void BattleEvent_Playback(void);
u32 BattleEventRuntime_Reset(void);
s32 BattleEventRuntime_WaitForReady(void);
u32 BattleParty_IsUnitListed(u32 unitId);
s32 BattleParty_CountStandbyDjinn(u32 unitId, u8 *counts);
s32 BattleCommand_BuildPlan(struct BattleActionRecord *request,
                            struct BattlePlan *plan);

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 BattleUnit_TickAttackModifier(s32 unitId);
s32 BattleUnit_TickConfusion(s32 unitId);
s32 Battle_AdvanceCounterAndCheckChance(s32 unitId);
s32 BattleUnit_TickRefrain(s32 unitId);
s32 BattleUnit_TickReflect(s32 unitId);
s32 BattleUnit_TickAgilityModifier(s32 unitId);
s32 DjinnRecovery_Tick(void);
s32 DjinnRecovery_TickTwenty(void);
void Battle_ReservedNoOpF674(void);
s32 Battle_ProcessRoundEnd(void);
s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan);

s32 BattlePres_RunMosaicFadeOut(void);
void Graphics_BuildSequentialTileTable(s32 *destination);
void BattlePresentation_BuildTilemap(s32 *destination);
void BattlePres_UpdateHBlankScroll(void);
void BattlePresentation_UploadTileVariant(void);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_DrawTransitionRows(void);
void Graphics_ClearBg0Vofs(void);
void BattlePres_RunEntryTransition(s32 enemyCount, s32 encounterId);
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level);
void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 unusedResource,
                                             s32 fade);
void BattlePresentation_InitializeWorkAndResetState(void);
void Runtime_ReleaseHeapBlock10(void);
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level);
void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale);
void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom);
void BattlePres_SetupTransitionAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                              s32 zoom);
void Graphics_FadeAlphaOut(void);
void Graphics_FadeAlphaIn(void);
void Graphics_SetBlendControl(void);
void BattlePresentation_InitializeTransitionEntries(
    struct BattleTransitionEntry *entries);
s32 BattlePres_DivideBy16(s32 value);
s32 Runtime_AdjustDebugValueWithButtons(s32 selectedValue);
void BattlePres_SetActorRecordMode(s32 actorId, s32 mode);
void BattlePres_SetOtherActorRecordModes(s32 selectedUnitId);
void BattlePres_ClearAllActorRecordModes(void);
void Graphics_AdvancePaletteCycle(void);
void BattlePres_SetActorModes(u16 *selectedUnits, s32 mode);

#ifdef UBFIX
void BattleFx_UpdateStarField(void);
#else
s32 BattleFx_UpdateStarField(void);
#endif
#ifdef UBFIX
void BattleFx_UploadStarField(void);
#else
u32 BattleFx_UploadStarField(void);
#endif
s32 BattleFx_InitializeStarField(s32 mode);
s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale,
                                s32 count);
// The native implicit-int declaration disagrees with the later void definition.
// A plain prototype keeps EN size but changes its first caller at .text+0x4c62.
#ifdef UBFIX
void BattleFx_PlayUnitElementEffect(s32 unitId, s32 element, s32 mode,
                                    s32 variant);
#endif
void BattlePres_RestorePalette(void);
void Runtime_RemoveIrqHandlerSlot2(void);

s32 BattleFormation_AverageLevel(s32 recordId);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin);
s32 Owner_ApplyLevelGains(s32 owner, s32 levels);
s32 EnemyCountDecrement(s32 classId, s32 unused);
s32 EnemyCountRelease(s32 actorId);
s32 EnemyCountClear(s32 classId);
s32 BattleGroupRandom(s32 recordId);
s32 BattleFormation_BuildEnemyList(s32 recordId);

u32 BattleClass_GetDefeatSoundKind(s32 index);
s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);
s32 BattleClass_IsSmall(s32 index);
u32 BattleClass_GetAttackEffect(u32 index);
u32 BattleClass_GetAttackHop(u32 index);
s32 Summon_IsEntrySecondaryFlagged(s32 index);
s32 BattleClass_GetEffectHeight(s32 index);

u32 BattleSpoils_RankItem(u32 inventoryEntry);
void Summon_ClearWorkFields(void);
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus);
void Battle_AwardSpoils(void);
void Battle_ReservedNoOp2A08(void);

#endif
