#ifndef ALCHEMY_UNIT_BATTLE_BATTLE_H
#define ALCHEMY_UNIT_BATTLE_BATTLE_H

#include "types.h"

struct BattleAction;
struct BattlePlaybackWork;
struct BattleAnimArgs;
struct BattleSummonState;
struct BattleTransitionEntry;
struct AbilityDefinition;
struct BattleObject;
struct BattleTurn;
struct FixedPointPosition;
struct MotionObject;

s32 DebugParty_LoadPreset(s32 preset);
void DebugBattle_ViewMessages(void);
void DebugBattle_RunMenu(void);
void BattleCamera_Update(void);
void BattleIntro_AnnounceEncounter(s32 enemyCount);
void BattleUnits_Collect(void);
s32 BattlePresentation_InitializeTilePattern(void);
void BattlePresentation_ClearBg0VerticalOffset(void);
void Party_ReservedNoOp5B14(s32 unused);
#ifdef UBFIX
void BattleParty_ResetActiveRuntimeFields(void);
#else
s32 BattleParty_ResetActiveRuntimeFields(void);
#endif
#ifdef UBFIX
void DjinnRecovery_Reconcile(void);
#else
s32 DjinnRecovery_Reconcile(void);
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
s32 LinkBattle_WaitSync(void);
s32 LinkBattle_SyncTurn(void);
#ifdef UBFIX
void BattleParty_AssignOpponentSlots(void);
#else
s32 BattleParty_AssignOpponentSlots(void);
#endif
s32 Battle_ClearSerialTransferPayload(void);
s32 Battle_Run(s32 enemyGroup);
void BattleCamera_AdjustByShoulderKeys(void);
void BattleCamera_AdjustByShoulderKeysAlt(void);
s32 BattleUnit_TickDefenseStage(s32 unitId);
s32 BattleUnit_TickResistanceStage(s32 unitId);
s32 BattleUnit_TickDelusion(s32 unitId);
s32 BattleUnit_TickCharm(s32 unitId);
s32 BattleUnit_TickStun(s32 unitId);
s32 BattleUnit_TickSleep(s32 unitId);

/// Pushes a message to show, waiting for the player to read it.
#define PUSH_MESSAGE(message) BattlePlayback_Push(PLAYBACK_MESSAGE, (message))

/// Moves camera yaw by 512 while L/R is held; two identical functions exist.
#define DEFINE_ADJUST_CAMERA(name)                                             \
  void name(void) {                                                            \
    struct BattleCamera *cam = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);              \
    struct BattlePresentationTransition *trans =                               \
        HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);                          \
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

s32 BattleParty_PrepareMembers(u16 *owners);
s32 BattleEnemy_List(s16 *unitIds);
s32 BattleUnits_ListLiving(s32 sideMask, u16 *unitIds);
s32 BattleUnits_ListIds(s32 sideMask, u16 *unitIds);
void BattleObject_Put(void);
void BattleUnits_ListAllAndSubmit(void);
s32 BattleMotion_GetSlotPalette(s32 unitId);
s32 Enemy_HasSpriteRoom(s32 classId);
s32 EnemySprite_LoadIntoSlots(s32 unitId);
#ifdef UBFIX
void BattleObject_ReleaseSlotByValue(s32 unitId);
#else
s32 BattleObject_ReleaseSlotByValue(s32 unitId);
#endif
s32 BattleUnit_GetWeaponKind(s32 itemId);
s32 BattleUnit_LookupWeaponValueByClass(s32 unitId);
void BattleObject_Make(struct BattleObject *actor, s32 unitId, s32 x, s32 y);
void BattlePlacement_GetStepPair(s32 partyPlace, s32 *x, s32 *y);
void BattlePlacement_LayoutEnemies(u16 *actorIds, s32 count, s32 *xPositions,
                                   s32 *zPositions);
s32 Enemy_GetFreeUnitId(void);
#ifdef UBFIX
void BattlePlacement_RelayoutEnemies(void);
#else
s32 BattlePlacement_RelayoutEnemies(void);
#endif
void BattleUnit_RefreshPlacement(void);
s32 BattlePlacement_ContainsId(s16 *list, s32 place);
void BattleObject_UpdateSpritePriorities(void);
#ifdef UBFIX
void BattleUnit_BuildStatusFlags(s32 unitId, struct BattleObject *battleObject);
#else
s32 BattleUnit_BuildStatusFlags(s32 unitId, struct BattleObject *battleObject);
#endif
#ifdef UBFIX
void BattleStatusIcon_Cycle(struct BattleObject *battleObject);
#else
s32 BattleStatusIcon_Cycle(struct BattleObject *battleObject);
#endif
#ifdef UBFIX
void BattlePresentation_SetActorModeAndAction(s32 unitId);
#else
s32 BattlePresentation_SetActorModeAndAction(s32 unitId);
#endif
void ReleaseBattleObjectRecords(s32 unitId);
void BattleObject_SpawnObjectsForList(s16 *unitIds, s32 refreshPoses);
struct BattleObject *BattleObject_Get(s32 unitId);
void ResetMotionRecordGroup(void *recordGroup);
void ResetBattleObjectRecordGroups(struct MotionObject *object);
s32 BattleObject_MarkForRelease(s32 unitId);
#ifdef UBFIX
void BattleObject_DestroyAll(void);
#else
void *BattleObject_DestroyAll(void);
#endif
s32 Battle_GetWorkEntryPair(s32 place, struct FixedPointPosition *position);
#ifdef UBFIX
void Camera_ApplyTransformByFlag(void);
#else
s32 Camera_ApplyTransformByFlag(void);
#endif
s32 BattleMotion_ProjectPosition(s32 unitId, s32 *projected);
void *GetMotionRecord(struct MotionObject *object, s32 recordIndex);
void Camera_InitializeDefaultTransform(void);
void BattleMotion_ReturnToSpot(s32 unitId);
void BattleMotion_SetupEscapeObject(s32 unitId);
void BattleMotion_InterpolatePosition(struct BattleObject *startSlot,
                                      struct BattleObject *endSlot,
                                      s32 progress);
void BattleMotion_SetObjectPosition(struct BattleObject *battleObject);
void BattleMotion_ResetObjectAtScaledAnchor(s32 unitId);
void BattleMotion_InitializeObject(s32 unitId);
void BattleMotion_ApplyKnockback(s32 unitId, s32 variant);
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelDivisor,
                                 s32 initialVelocityY);
void BattleMotion_ResetSlotObjectMode2(s32 unitId);
void BattleMotion_ReservedNoOp83B0();
void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId);
void BattleMotion_SpawnSlotEffectAndWait(s32 unitId);
s32 BattleMotion_ProjectScaledPosition(s32 unitId, s32 *projected);
s32 BattleMotion_ProjectConditionalPosition(s32 unitId, s32 *projected);
s32 BattleUnit_GetEffectHeight(s32 unitId);

s32 Battle_BuildSortedUnitActions(struct BattleAction *entries);
s32 BattlePresentation_RunAction(struct BattleAction *action);
s32 BattleObject_IsValidId(u32 unitId);
s32 BattleFlee_PlayRun(struct BattleAction *action);
s32 BattlePresentation_ShowDefendMessage(const struct BattleAction *action);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleAction *action);
s32 BattlePresentation_RunApproachAction(struct BattleAction *input);
s32 BattlePresentation_RunUnitAction(struct BattleAction *action);
void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 showMessage,
                                        u8 *context);
void BattleMotion_SetMode5AndActivateSlot(s32 unitId);
s32 BattleAbility_ReplaceDefeated(const struct BattleAction *action);

void Palette_UpdatePulseBrightness(void);
void Camera_ConfigureView(s32 verticalOffset);
void BattleUnit_ResetGuardLevels(void);
s32 BattleFlee_CheckSuccess(void);
s32 Battle_BuildUnitActions(struct BattleAction *entries);
s32 Battle_BuildOpponentActions(struct BattleAction *entries);
void BattleTurnOrder_Sort(struct BattleAction *entries, s32 count);
s32 Battle_AppendLinkedActions(struct BattleAction *actions, s32 count);
void Palette_CopyBanksWithBrightnessOffset(s32 offset);
s32 Battle_GatherActions(struct BattleAction *actions);
s32 Battle_GetTaggedSlotValue(s32 taggedPlace);
s32 Battle_FindTaggedSlotByValue(u32 unitId);
void Battle_ReservedNoOp9B2C(void);

s32 BattleStaging_DispatchAction(struct BattleAction *request, s32 delay);
#ifdef UBFIX
void BattlePresentation_BuildTargetList(struct BattleTurn *plan,
                                        struct BattleAnimArgs *effect);
#else
s32 BattlePresentation_BuildTargetList(struct BattleTurn *plan,
                                       struct BattleAnimArgs *effect);
#endif
s32 BattleStaging_RunFlee(struct BattleTurn *plan);
s32 BattleStaging_HideField(struct BattleTurn *plan, s32 mode);
#ifdef UBFIX
void BattleStaging_RunActorEntries(struct BattleTurn *plan, s32 unused);
#else
s32 BattleStaging_RunActorEntries(struct BattleTurn *plan, s32 unused);
#endif
s32 BattleStaging_RunAttack(struct BattleTurn *plan, s32 mode);
s32 BattlePresentation_RunPlainAttack(struct BattleTurn *input, s32 flags);
s32 BattleStaging_RunItem(struct BattleTurn *plan, s32 unused,
                          struct BattleAction *selection);
#ifdef UBFIX
void BattleMotion_SetRecordChildValues(struct MotionObject *object,
                                       s32 palette);
#else
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 palette);
#endif
s32 BattleStaging_RunPsynergyCast(struct BattleTurn *plan, s32 mode);

void BattleMotion_FlashHit(s32 unitId, s32 unused0, s32 unused1);
void BattleObject_RemoveFromLists(s32 unitId);
void BattleMotion_ShowDowned(s32 unitId);
s32 BattleAI_ChooseRandomTarget(s32 requireLivingUnit);
s32 BattleAI_ChooseTarget(s32 actorId, struct AbilityDefinition *action);
#ifdef UBFIX
void BattleObject_ResetRuntimeFields(s32 unitId);
#else
s32 BattleObject_ResetRuntimeFields(s32 unitId);
#endif
#ifdef UBFIX
void BattleMessage_WaitForAdvance(void);
#else
s32 BattleMessage_WaitForAdvance(void);
#endif
#ifdef UBFIX
void BattleMessage_WaitForAdvanceAt(s32 x, s32 y);
#else
s32 BattleMessage_WaitForAdvanceAt(s32 x, s32 y);
#endif
void BattlePlayback_MarkMessage(void);
#ifdef UBFIX
void BattleObject_DestroyTemporaryObject(s32 unitId);
#else
s32 BattleObject_DestroyTemporaryObject(s32 unitId);
#endif
void BattlePlayback_GrantDjinnBonus(struct BattlePlaybackWork *runtime,
                                    u32 operand);
#ifdef UBFIX
void BattlePlayback_RunAll(void);
#else
u32 BattlePlayback_RunAll(void);
#endif
u32 BattlePlayback_Push(u32 opcode, u32 operand);
s32 Ability_EffectAppliesToDowned(s32 effect);
#ifdef UBFIX
void BattleAbility_ApplyToTarget(struct BattleTurn *plan, s32 slot);
#else
s32 BattleAbility_ApplyToTarget(struct BattleTurn *plan, s32 slot);
#endif
u32 Ability_IsPsynergy(s32 abilityId);
#ifdef UBFIX
void BattleAI_ChooseAction(struct BattleAction *request, s32 retry);
#else
s32 BattleAI_ChooseAction(struct BattleAction *request, s32 retry);
#endif
void Dma_StopAllThenRunHook(void);
void BattlePlayback_Start(s32 cue);
s32 BattlePlayback_SchedulePhase(s32 frames);
void BattlePlayback_Run(void);
void BattlePlayback_Reset(void);
#ifdef UBFIX
void BattlePlayback_WaitForReady(void);
#else
s32 BattlePlayback_WaitForReady(void);
#endif
u32 BattleUnits_IsListed(u32 unitId);
s32 BattleDjinn_CountStandby(u32 unitId, u8 *counts);
s32 BattleTurn_Build(struct BattleAction *request, struct BattleTurn *plan);

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 BattleUnit_TickAttackStage(s32 unitId);
s32 BattleUnit_TickConfusion(s32 unitId);
s32 Battle_AdvanceCounterAndCheckChance(s32 unitId);
s32 BattleUnit_TickRegenerate(s32 unitId);
s32 BattleUnit_TickReflect(s32 unitId);
s32 BattleUnit_TickAgilityStage(s32 unitId);
s32 DjinnRecovery_Tick(void);
s32 DjinnRecovery_TickTwenty(void);
void Battle_ReservedNoOpF674(void);
#ifdef UBFIX
void Battle_ProcessRoundEnd(void);
#else
s32 Battle_ProcessRoundEnd(void);
#endif
#ifdef UBFIX
void BattleUnit_ProcessTurnEnd(struct BattleTurn *plan);
#else
s32 BattleUnit_ProcessTurnEnd(struct BattleTurn *plan);
#endif

s32 BattleTransition_RunMosaicFadeOut(void);
void Graphics_BuildSequentialTileTable(void *tilemap);
void BattleTransition_BuildTilemap(void *tilemap);
void BattlePresentation_UpdateHBlankScroll(void);
void BattleTransition_UploadTileVariant(void);
void BattleTransition_AdvanceTimer(void);
void BattleTransition_DrawRows(void);
void Graphics_ClearBg0Vofs(void);
void BattleTransition_Run(s32 enemyCount, s32 encounterId);
void BattleBackground_SetPaletteLevel(s32 unused, s32 level);
void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 unusedResource,
                                             s32 fade);
void BattlePresentation_InitializeWorkAndResetState(void);
void FreeHeapSlot10(void);
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level);
void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale);
#ifdef UBFIX
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom);
#else
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y);
#endif
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom);
void BattleCamera_SetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                        s32 zoom);
void BattleBackground_FadeAlphaOut(void);
void BattleBackground_FadeAlphaIn(void);
void BattleBackground_SetBlendControl(void);
void BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries);
s32 BattlePresentation_DivideBy16(s32 value);
s32 Runtime_AdjustDebugValueWithButtons(s32 selectedValue);
void BattleHighlight_SetSpriteMode(s32 actorId, s32 mode);
void BattleHighlight_SetOtherSpriteModes(s32 selectedUnitId);
void BattleHighlight_ClearAllSpriteModes(void);
void BattleBackground_StepHighlightBlend(void);
void BattleHighlight_SetModes(u16 *selectedUnits, s32 mode);

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
s32 ElementGlow_ReleaseVramAndHeapBlocks(s32 unused);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale,
                                s32 count);
// The native implicit-int declaration disagrees with the later void definition.
// A plain prototype keeps EN size but changes its first caller at .text+0x4c62.
#ifdef UBFIX
void ElementGlow_Play(s32 unitId, s32 element, s32 mode, s32 variant);
#endif
void BattleBackground_RestorePalette(void);
void Irq_RemoveVCountHandler(void);

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
void BattleEnemy_LetterFirstOfKind(s32 classId);
#else
s32 BattleEnemy_LetterFirstOfKind(s32 classId);
#endif
s32 BattleFormation_GetRandomMember(s32 recordId);
s32 BattleFormation_BuildEnemyList(s32 recordId);

u32 BattleSprite_GetDefeatSound(s32 index);
s32 BattleSprite_GetSprite(s32 index);
s32 BattleSprite_GetPalette(s32 index);
s32 BattleSprite_IsSmall(s32 index);
u32 BattleSprite_GetAttackAnim(u32 index);
u32 BattleSprite_GetAttackHop(u32 index);
s32 BattleSprite_HasHeavyImpact(s32 index);
s32 BattleSprite_GetAnimHeight(s32 index);

u32 BattleSpoils_RankItem(u32 inventoryEntry);
void BattleSpoils_Clear(void);
s32 BattleEnemy_RecordDefeat(s32 unitId, s32 djinnBonus);
void Battle_AwardSpoils(void);
void Battle_ReservedNoOp2A08(void);

#endif
