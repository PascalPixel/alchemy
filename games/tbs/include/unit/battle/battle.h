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
#ifdef UBFIX
void BattleParty_ResetActiveRuntimeFields(void);
#else
s32 BattleParty_ResetActiveRuntimeFields(void);
#endif
#ifdef UBFIX
void BattleParty_ReconcileDjinnRecovery(void);
#else
s32 BattleParty_ReconcileDjinnRecovery(void);
#endif
struct BattleSummonState *BattleSummon_UpdateAvailability(void);
void BattleSummon_ReservedNoOp(void);
s32 LinkBattle_ReceiveParty(void);
#ifdef UBFIX
void LinkBattle_SendParty(void);
#else
s32 LinkBattle_SendParty(void);
#endif
u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source);
s32 BattlePresentation_WaitSync(void);
s32 BattlePresentation_SyncTurn(void);
#ifdef UBFIX
void BattleParty_AssignMemberSlots(void);
#else
s32 BattleParty_AssignMemberSlots(void);
#endif
s32 Battle_ClearSerialTransferPayload(void);
s32 Battle_RunEncounter(s32 enemyGroup);
void BattlePresentation_AdjustCameraByShoulderKeys(void);
void BattlePresentation_AdjustCameraByShoulderKeysAlt(void);
s32 BattleUnit_TickDefenseModifier(s32 unitId);
s32 BattleUnit_TickResistanceModifier(s32 unitId);
s32 BattleUnit_TickDelusion(s32 unitId);
s32 BattleUnit_TickCharm(s32 unitId);
s32 BattleUnit_TickStun(s32 unitId);
s32 BattleUnit_TickSleep(s32 unitId);

/// Pushes a message to show, waiting for the player to read it.
#define PUSH_MESSAGE(message) BattleEvent_Push(BATTLE_EVENT_MESSAGE, (message))

/// Moves camera yaw by 512 while L/R is held; two identical functions exist.
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
s32 BattleEnemy_List(s16 *unitIds);
s32 BattleParty_ListLivingUnits(s32 sideMask, u16 *unitIds);
s32 BattleParty_ListActorIds(s32 sideMask, u16 *unitIds);
void BattleObject_Put(void);
void BattleParty_ListAllUnitsAndSubmit(void);
s32 BattleMotion_GetSlotPalette(s32 unitId);
s32 Enemy_CheckRoom(s32 classId);
s32 SummonSlot_RegisterActorSprites(s32 unitId);
#ifdef UBFIX
void BattleMotion_ReleaseObjectSlotByValue(s32 unitId);
#else
s32 BattleMotion_ReleaseObjectSlotByValue(s32 unitId);
#endif
s32 BattleUnit_GetWeaponKind(s32 itemId);
s32 BattleUnit_LookupWeaponValueByClass(s32 unitId);
void BattleObject_Make(struct BattleObj *actor, s32 unitId, s32 x, s32 y);
void BattlePlacement_GetStepPair(s32 partyPlace, s32 *x, s32 *y);
void BattlePlacement_LayoutEnemies(u16 *actorIds, s32 count, s32 *xPositions,
                                   s32 *zPositions);
s32 Enemy_GetFreeSlot(void);
#ifdef UBFIX
void Enemy_Update(void);
#else
s32 Enemy_Update(void);
#endif
void BattleUnit_RefreshPlacement(void);
s32 BattlePlacement_ContainsId(s16 *list, s32 place);
void BattleObject_UpdateSpritePriorities(void);
#ifdef UBFIX
void BattleUnit_BuildStatusFlags(s32 unitId, struct BattleObj *battleObject);
#else
s32 BattleUnit_BuildStatusFlags(s32 unitId, struct BattleObj *battleObject);
#endif
#ifdef UBFIX
void BattleStatusIcon_Cycle(struct BattleObj *battleObject);
#else
s32 BattleStatusIcon_Cycle(struct BattleObj *battleObject);
#endif
#ifdef UBFIX
void BattlePresentation_SetActorModeAndAction(s32 unitId);
#else
s32 BattlePresentation_SetActorModeAndAction(s32 unitId);
#endif
void ReleaseBattleObjectRecords(s32 unitId);
void BattleObject_SpawnObjectsForList(s16 *unitIds, s32 refreshPoses);
struct BattleObj *BattleObject_Get(s32 unitId);
void ResetMotionRecordGroup(void *recordGroup);
void ResetBattleObjectRecordGroups(struct MotionObject *object);
s32 BattleObject_MarkForRelease(s32 unitId);
#ifdef UBFIX
void BattleMotion_DestroyAllSlotObjects(void);
#else
void *BattleMotion_DestroyAllSlotObjects(void);
#endif
s32 Battle_GetWorkEntryPair(s32 place, struct FixedPointPosition *position);
#ifdef UBFIX
void Camera_ApplyTransformByFlag(void);
#else
s32 Camera_ApplyTransformByFlag(void);
#endif
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
s32 BattleUnit_GetEffectHeight(s32 unitId);

s32 BattlePresentation_BuildSortedUnitEntries(
    struct BattleActionRecord *entries);
s32 BattlePresentation_RunAction(struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 unitId);
s32 BattleEscape_PlayRun(struct BattleActionRecord *action);
s32 BattlePresentation_ShowDefendMessage(
    const struct BattleActionRecord *action);
s32 BattlePresentation_RunPairedUnitTransition(
    struct BattleActionRecord *action);
s32 BattlePresentation_RunApproachAction(struct BattleActionRecord *input);
s32 BattlePresentation_RunUnitAction(struct BattleActionRecord *action);
void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 showMessage,
                                        u8 *context);
void BattleMotion_SetMode5AndActivateSlot(s32 unitId);
s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action);

void Palette_UpdatePulseBrightness(void);
void Camera_ConfigureScene(s32 verticalOffset);
void BattleUnit_ResetGuardLevels(void);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePresentation_BuildUnitEntries(struct BattleActionRecord *entries);
s32 BattlePresentation_BuildOpponentEntries(struct BattleActionRecord *entries);
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
#ifdef UBFIX
void BattlePresentation_BuildTargetList(struct BattlePlan *plan,
                                        struct BattleEffectArgument *effect);
#else
s32 BattlePresentation_BuildTargetList(struct BattlePlan *plan,
                                       struct BattleEffectArgument *effect);
#endif
s32 BattlePresentation_RunEscape(struct BattlePlan *plan);
s32 BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode);
#ifdef UBFIX
void BattlePresentation_RunActorEntries(struct BattlePlan *plan, s32 unused);
#else
s32 BattlePresentation_RunActorEntries(struct BattlePlan *plan, s32 unused);
#endif
s32 BattlePresentation_RunAttack(struct BattlePlan *plan, s32 mode);
s32 BattlePresentation_RunPlainAttack(struct BattlePlan *input, s32 flags);
s32 BattlePresentation_RunItem(struct BattlePlan *plan, s32 unused,
                               struct BattleActionRecord *selection);
#ifdef UBFIX
void BattleMotion_SetRecordChildValues(struct MotionObject *object,
                                       s32 palette);
#else
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 palette);
#endif
s32 BattlePresentation_RunPsynergyCast(struct BattlePlan *plan, s32 mode);

void BattleMotion_FlashHit(s32 unitId, s32 unused0, s32 unused1);
void BattleObject_RemoveFromLists(s32 unitId);
void BattleMotion_ShowDowned(s32 unitId);
s32 BattleTarget_SelectRandomPosition(s32 requireLivingUnit);
s32 BattleTarget_SelectForAction(s32 actorId, struct BattleAct *action);
#ifdef UBFIX
void BattleObject_ResetRuntimeFields(s32 unitId);
#else
s32 BattleObject_ResetRuntimeFields(s32 unitId);
#endif
#ifdef UBFIX
void BattlePresentation_WaitForAdvance(void);
#else
s32 BattlePresentation_WaitForAdvance(void);
#endif
#ifdef UBFIX
void BattlePresentation_WaitForPromptAt(s32 x, s32 y);
#else
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y);
#endif
void BattleEvent_MarkDisplay(void);
#ifdef UBFIX
void BattleObject_DestroyTemporaryObject(s32 unitId);
#else
s32 BattleObject_DestroyTemporaryObject(s32 unitId);
#endif
void BattleEvent_GrantDjinnBonus(struct BattleEventState *runtime, u32 operand);
#ifdef UBFIX
void BattleEvent_DispatchQueued(void);
#else
u32 BattleEvent_DispatchQueued(void);
#endif
u32 BattleEvent_Push(u32 opcode, u32 operand);
s32 BattleEffect_CheckDead(s32 effect);
#ifdef UBFIX
void BattleTarget_Execute(struct BattlePlan *plan, s32 slot);
#else
s32 BattleTarget_Execute(struct BattlePlan *plan, s32 slot);
#endif
u32 Ability_IsPsynergy(s32 abilityId);
#ifdef UBFIX
void BattleCommand_SelectAutomatic(struct BattleActionRecord *request,
                                   s32 retry);
#else
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *request,
                                  s32 retry);
#endif
void Dma_StopAllThenRunHook(void);
void BattleEvent_BeginPhase(s32 cue);
s32 BattleEvent_SchedulePhase(s32 frames);
void BattleEvent_Playback(void);
void BattleEvent_Reset(void);
#ifdef UBFIX
void BattleEvent_WaitForReady(void);
#else
s32 BattleEvent_WaitForReady(void);
#endif
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
#ifdef UBFIX
void Battle_ProcessRoundEnd(void);
#else
s32 Battle_ProcessRoundEnd(void);
#endif
#ifdef UBFIX
void BattleUnit_ProcessTurnEnd(struct BattlePlan *plan);
#else
s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan);
#endif

s32 BattlePresentation_RunMosaicFadeOut(void);
void Graphics_BuildSequentialTileTable(void *tilemap);
void BattlePresentation_BuildTilemap(void *tilemap);
void BattlePresentation_UpdateHBlankScroll(void);
void BattlePresentation_UploadTileVariant(void);
void BattlePresentation_AdvanceTransitionTimer(void);
void BattlePresentation_DrawTransitionRows(void);
void Graphics_ClearBg0Vofs(void);
void BattlePresentation_RunEntryTransition(s32 enemyCount, s32 encounterId);
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level);
void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 unusedResource,
                                             s32 fade);
void BattlePresentation_InitializeWorkAndResetState(void);
void Runtime_ReleaseHeapBlock10(void);
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level);
void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale);
#ifdef UBFIX
void BattlePresentation_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y,
                                                    s32 zoom);
#else
void BattlePresentation_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y);
#endif
void BattlePresentation_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom);
void BattlePresentation_SetupTransitionAtPairMidpoint(s32 firstUnitId,
                                                      s32 secondUnitId,
                                                      s32 zoom);
void Graphics_FadeAlphaOut(void);
void Graphics_FadeAlphaIn(void);
void Graphics_SetBlendControl(void);
void BattlePresentation_InitializeTransitionEntries(
    struct BattleTransitionEntry *entries);
s32 BattlePresentation_DivideBy16(s32 value);
s32 Runtime_AdjustDebugValueWithButtons(s32 selectedValue);
void BattlePresentation_SetActorRecordMode(s32 actorId, s32 mode);
void BattlePresentation_SetOtherActorRecordModes(s32 selectedUnitId);
void BattlePresentation_ClearAllActorRecordModes(void);
void Graphics_AdvancePaletteCycle(void);
void BattlePresentation_SetActorModes(u16 *selectedUnits, s32 mode);

#ifdef UBFIX
void ElementGlow_UpdateStarField(void);
#else
s32 ElementGlow_UpdateStarField(void);
#endif
#ifdef UBFIX
void ElementGlow_UploadStarField(void);
#else
u32 ElementGlow_UploadStarField(void);
#endif
s32 ElementGlow_InitializeStarField(s32 mode);
s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale,
                                s32 count);
// The native implicit-int declaration disagrees with the later void definition.
// A plain prototype keeps EN size but changes its first caller at .text+0x4c62.
#ifdef UBFIX
void ElementGlow_Play(s32 unitId, s32 element, s32 mode, s32 variant);
#endif
void BattlePresentation_RestorePalette(void);
void Runtime_RemoveIrqHandlerSlot2(void);

s32 BattleFormation_AverageLevel(s32 recordId);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin);
#ifdef UBFIX
void BattleEnemy_AdjustLevel(s32 owner, s32 levels);
#else
s32 BattleEnemy_AdjustLevel(s32 owner, s32 levels);
#endif
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 unused);
#ifdef UBFIX
void BattleEnemy_ReleaseNameLetter(s32 actorId);
#else
s32 BattleEnemy_ReleaseNameLetter(s32 actorId);
#endif
#ifdef UBFIX
void BattleEnemy_LetterFirstOfClass(s32 classId);
#else
s32 BattleEnemy_LetterFirstOfClass(s32 classId);
#endif
s32 BattleFormation_GetRandomMember(s32 recordId);
s32 BattleFormation_BuildEnemyList(s32 recordId);

u32 BattleClass_GetDefeatSoundKind(s32 index);
s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);
s32 BattleClass_IsSmall(s32 index);
u32 BattleClass_GetAttackEffect(u32 index);
u32 BattleClass_GetAttackHop(u32 index);
s32 BattleClass_HasHeavyImpact(s32 index);
s32 BattleClass_GetEffectHeight(s32 index);

u32 BattleSpoils_RankItem(u32 inventoryEntry);
void BattleSpoils_Clear(void);
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus);
void Battle_AwardSpoils(void);
void Battle_ReservedNoOp2A08(void);

#endif
