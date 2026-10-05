#ifndef PROTO_H
#define PROTO_H

#include "TYPES.H"

struct AnimationObject;
struct BattleActionRecord;
struct BattleEffectArgument;
struct BattleEffectBuffers;
struct BattleElementStats;
struct BattleObjectSlot;
struct BattlePlan;
struct BattleUnit;
struct EffectPosition;
struct EffectSlot;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct Obj;
struct ObjectRuntime;
struct Object_0801c0dc;
struct PendingEvent;
struct PlacementTable;
struct RenderOutput;
struct ScaledSprite;
struct ScriptInterpreter;
struct SoundPlayer;
struct SoundTrack;
struct SoundWork;
struct UiPanel;

s32 _call_via_r3(s32, u8 *, u8, s32);
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
void Ability_LoadGlyphFar(s32, s32, s32 *, s32 *, s32);
void *AffineEffect_InitializeWork(void);
s32 Animation_InitializeObjects(struct AnimationObject *);
s32 Animation_LookupValueByKey(s32 key);
void Animation_SetStateFlags(struct FieldActor *object, s32 flags);
s32 AnimationObject_Allocate(s32 id);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *, s32);
s32 Audio_Check(void);
void Audio_EmptyCallback(void);
void Audio_ResumeAllPlayers(void);
void Audio_ResumePlayer(struct SoundPlayer *player);
void Audio_StopAllPlayers(void);
s32 Audio_UmulHigh32(s32, s32);
void AudioCommand_WaitForStateByteClear(void);
void AudioEngine_Initialize(struct SoundWork *work);
void AudioEngine_ResumeDirectSound(void);
void AudioEngine_RunMixerTick(void);
void AudioEngine_SetMode(u32 mode);
void AudioEngine_SetPcmRate(u32 mode);
void AudioEngine_SuspendDirectSound(void);
s32 Battle_ResolveTargetAction(struct BattlePlan *plan, s32 slot);
s16 *BattleAction_FindDescriptor(s16 action);
s32 BattleActor_RemoveFromLists(s32);
void BattleEffect_InitializeSharedScene(void);
void BattleEvent_Playback(void);
void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode);
s32 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
void BattleFx_BuildBuffer(s32 mode, struct BattleEffectBuffers *source, u16 *destination, s32 half);
void BattleFx_CallEffect14(void);
void BattleFx_ClearChildValueOnMismatch(void);
s32 BattleFx_DispatchRequestKind(void);
void BattleFx_DrawScaledObject(struct EffectSlot *object);
s32 BattleFx_FindMatchingEvent(s32, s32, void *);
u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1);
s32 BattleFx_HasMatchingEvent5(s32 effectId);
void BattleFx_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void BattleFx_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
void BattleFx_LoadActionEffectResources(s32 action, s32 flags);
s32 BattleFx_LookupResult(void *);
void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode);
s32 BattleFx_Run(void);
void BattleFx_RunNoEffect(void);
void BattleFx_RunOrbitingParticles(void);
s32 BattleFx_RunProjectileVolley(void *effect, s32 mode);
s32 BattleFx_RunProjectileVolleyB(void *effect, s32 mode);
void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode);
s32 BattleFx_RunSparkGroups(void *, s32);
s32 BattleFx_SelectBattleCue(s32 row, s32 column);
void BattleFx_SetupObjectPair(s32 actor, s32 target);
void BattleFx_SnapScaleToFull(void *object);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 BattleFx_StartBufferInterpolationFar(s32);
void *BattleFx_StartItemBreak(void *object);
s32 BattleFx_StartRandomParticleEmitter(s32, s32);
void BattleFx_UpdateArcEffect(void);
void BattleLayout_HighlightPartyPanelsFar(u16 *);
void BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void BattleMotion_DestroyAllSlotObjects(void);
s32 BattleMotion_GetSlotPalette(s32 id);
void BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);
void BattleMotion_ProjectScaledPositionFar(s32, struct EffectPosition *);
void BattleMotion_ReleaseObjectSlotByValue(s32 object_id);
void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id);
void BattleMotion_SetMode5AndActivateSlot(s32 unit_id);
void BattleMotion_SetObjectPosition(struct BattleObjectSlot *slot);
void BattleMotion_SetRecordChildValues(void *, s32);
void BattleMotion_SetupEscapeObject(s32 object_id);
void BattlePalette_UpdateBlend(void);
s32 BattleParty_ListActorIdsFar(s32 side, s32 group);
s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
s32 BattleParty_PrepareReserveOwners(u16 *owners);
void BattlePlacement_UpdateTimedEntriesTwentyTimesFar(void);
void BattlePres_RunBurstScene(struct EffectStep *, s32);
void BattlePres_SetActorModes(u16 *, s32);
void BattlePres_SetActorModesFar(u16 *,s32);
s32 BattlePres_WaitSync(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
void BattlePresentation_WaitForAdvance(void);
void BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
void BattleUnit_ResetGuardLevels(void);
void Camera_ConfigureScene(s32 scene);
void Camera_InitDefaultTransform(void);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);
void Cgb_UpdateChannels(void);
void CgbChannel_Mute(u8 channel);
s32 Curve_LookupScaledValue(s32 selector, s32 mode);
s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align);
s32 Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);
s32 Djinn_AddToOwnerFar(s32, s32, s32);
void DjinnMenu_DrawPage(struct UiPanel *panel, s32 first);
void EffectRuntime_StopCurrentObject(void);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
s32 Equipment_HasValueFar(s32 group, s32 entry);
void Event_ClearInvalidPackedValuesFar(s32);
void Event_DelayEffectFramesFar(void);
void Event_FinishScreenTransitionFar(void);
s32 Event_SpawnObjectTable(s32 event_id, s32 state);
s32 Event_ValidatePackedId(u32 packed_id);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second);
void Field_BeginPaletteTransition(s32 mode);
void Field_DispatchTypeHandler(s32 kind);
void Field_EndActorSpriteEffect(s32 mode);
void FieldEvent_RunTypeHandler(void);
s32 FixedSqrt(s32 value);
void Func_02000b2c(void);
void Func_02000d30(void);
void Func_02001c7c(s32, s32);
void Func_02001c84(s32, s32);
void Func_02001c8c(void);
void Func_02001c94(void);
void Func_02001c9c(void);
void Func_020023a4(void *object);
void Func_0200258c(void);
void Func_0200260c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02002a34(s32 a, s32 b);
void Func_02002a44(void);
void Func_02002a4c(void);
void Func_02002a54(void);
void Func_02002a5c(struct ObjectRuntime *object, s32 mode);
s32 Func_08020190(s32, s32, s32);
void Func_0803dd24(void);
void Func_080c8368(s32, s32);
void Func_080c8378(s32, s32);
void Func_080c8380(s32);
void Func_080c8390(s32);
void Func_080c8398(void);
void Func_080c83a0(void);
s32 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);
void Func_080caa2c(u32 object_id);
s32 Func_080cc67c(s32);
s32 Func_080cc7c4(void);
s32 Func_080cdac0(s32 effect, s32 from, s32 to);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void Func_080cee80(u32 object_id);
void Func_080e3074(void);
void Func_080e70f8(u32 object_id);
void Func_0811b75c(s16 *list, s32 refresh, s32 spawn);
void Func_0818f620(void *effect, s32 mode);
void Func_081b83c4(void);
void GameFlag_ClearBitFar(s32 flag);
void GameFlag_RefreshLureCapFar(void);
void GameFlag_SetBitFar(s32 flag);
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift);
s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
s32 ItemIcon_Compose(s32, s32);
void ItemMenu_ArrangeCategoryItemIcons(u8 *items);
void ItemMenu_DrawEquippedItemNames(s32 window, u8 *items);
void ItemMenu_PosCategory(void);
s32 LuckyDice_Run(void);
void Map_ApplyWorkOriginAndSpanFar(void);
s32 Map_DisableUpdateCallbackFar(void);
s32 Map_EnableUpdateCallbackFar(void);
s32 Map_GetTerrainHeightFar(u8, s32, s32);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);
void Map_RenderAnimatedTileFrame(u8 *object, u32 position);
s32 Map_RenderAnimatedTileFramesForObjectFar(void *object);
void Menu_AppendResourceEntry(s32 no);
void Menu_DrawFlagBitTable(s32, s32);
void Menu_EndResourceSelection(void);
s32 Menu_HandleFlagGridInput(s32, s32 *, s32 *);
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h);
s32 Menu_RunResourceSelectionLoop(s32);
void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant);
void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_Initialize(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void Object_AttachWorkTargetToObject(s32 id, s32 flag);
s32 Object_IsTargetUnset(void *arg0);
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void ObjectDispatch_InitFromTable4WithArgumentFar(void *object, void *effect);
void ObjectDispatch_WaitForValue16Far(struct ObjectRuntime *);
void ObjectEffect_EndContextEffect(s32 effect);
void ObjectEffect_PrepareContextEffect(s32 effect);
void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset);
void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_NoOp(void);
void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetActionCallback(struct ObjectRuntime *, s32);
void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z);
void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant);
void ObjectMotion_SnapToTerrain(void *object);
void ObjectSystem_Configure(s32 mode);
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
s32 ObjectTable_FindLastActiveId(void);
void Owner_CalculateBaseElements(s32 owner_no, struct BattleElementStats *elements);
s32 Owner_DetermineClass(s32 class_id, u32 *djinn, s32 item);
s32 Owner_GetDefaultElement(void *state);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);
void Owner_RefreshClassActions(s32);
void Owner_UpdateClassActions(s32 owner_no);
void Owner_UpdateRatioPairFar(struct BattleUnit *, s32);
void OwnerAction_AddFar(void);
void PaletteGlow_UpdateSine(void);
void ParticleTrail_Spawn(struct FieldActor *actor, s32 x, s32 y, s32 z);
s32 Party_AddActiveOwnerFar(s32 event_id);
void Party_AdjustCoinsFar(s32);
s32 Party_CountActiveOwnersFar(void);
s32 Party_ListActiveOwnersFar(s16 *);
void Party_RemoveActiveOwnerFar(s32 owner);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Party_SumDjinnCountsFar(s32);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale, u16 angle);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite, s32 *position, s32 *scale, s32 mode);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale, s32 angle, s32 layer);
s32 Render_ProjectPoint(s32 *point, s32 *screen);
void Render_ResetTransformState(void);
void RenderOutput_ClearList(void *work);
void RenderOutput_Release(struct RenderOutput *node);
void *Resource_FarCall005(s32);
s32 Resource_LoadByMode(s32 mode, s32 value);
void Resource_LoadCode(u32 index, void *destination);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
void Resource_ResetPendingTransfer(void);
void Resource_ScheduleOwnerReset(void);
void ResourceMetadata_ClearRecord(void *destination);
void RunBattleEffect05(void);
s32 RunFlashEraseVerifier(u8 *, u8 *);
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
void Runtime_RemapBytesByTableFar(void *buffer, s32 size);
void Runtime_WriteDebugTextTiles(const u8 *src);
void Scene_SetArrivalFlags(s32 unused);
s32 Shop_CanServe(s32 entry_no, s32 kind);
void Sound_Mixer(void);
s32 Summon_IsEntryFlagged(s32 index);
void SummonFx_RunCanvasEffect(void *action, s32 mode);
void Text_FormatHexToWork(u32);
void *Text_FormatPlayTime(s32, u8 *);
void Text_FormatSignedDecimalToWork(s32);
s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit);
struct PlacementTable *Trade_GetOfferStateFar(s32 owner);
s32 Trade_RemoveOfferFar(s32 unit_id, s32 element, s32 index);
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
s32 Ui_ClearVramBlock(void);
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);
void Ui_PrepareTransferFromTableEntry(u32 index);
void Ui_SetRectHighlightFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
void UiIcon_PrepareObject(struct RenderOutput *icon);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y);
void UiText_DrawQuantity(s32, s32);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawString(u8 *text, s32 output, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(s32 image, s32 layer, s32 x, s32 y);
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
void UiText_RenderWideStringInWindow(u16 *text, s32 work, s32 x, s32 y);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void UiTextResource_Initialize(struct Object_0801c0dc *obj, s32 *slot);
void UiTextResource_Release(s32 value);
void UiTextResource_SetPosition(struct Obj *obj, s32 arg1, s32 arg2);
void UiTimedNotice_CloseIfActiveFar(void);
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_DrawDividerLineFar(s32, s32, s32, s32, s32);
void UiWindow_DrawPartyStatusContents(s32 flags);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);
void UiWork_AnimateSpriteSlots(void);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_FinalizeAndReleaseBlock16(void);
void UiWork_FinalizeSharedSlotFar(void);
void UiWork_InitCountersAndScheduleRefresh(s32 kind);
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
s32 UiWork_IsIdleFar(s32);
void UiWork_ProcessDirectWork(void);
void UiWork_ProcessRenderChannels(void);
s32 UiWork_SetParamNibbleFar(s32 color);
void UiWork_SetTwoEntriesTo999(void);
void UiWork_UploadDirtyBlocks(void);
void UpdateRisingParticleBurst(void *object);
void VramBlock_LoadResourceFar(s32 entry_no, s32 mode, s32 resource_id);

struct BattleActionRecord;
struct BattleObjectSlot;
struct DispatchObject;
struct ObjectRuntime;

#ifndef OWN_BattleActor_SpawnObjectsForList
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
#endif
#ifndef OWN_BattleFx_GetResourceGroup
s8 BattleFx_GetResourceGroup(s32 effect_index);
#endif
#ifndef OWN_BattleLayout_HighlightPartyPanels
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
#endif
#ifndef OWN_BattleMotion_ApproachTarget
void BattleMotion_ApproachTarget(s32 actor_id, s32 target_id, s32 travel_divisor, s32 initial_velocity_y);
#endif
#ifndef OWN_BattleMotion_InterpolatePosition
void BattleMotion_InterpolatePosition(struct BattleObjectSlot *start_slot, struct BattleObjectSlot *end_slot, s32 progress);
#endif
#ifndef OWN_BattlePres_BuildOpponentEntries
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
#endif
#ifndef OWN_BattlePres_BuildUnitEntries
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
#endif
#ifndef OWN_CountRemainingErasedFlashBytes
s32 CountRemainingErasedFlashBytes(u8 *ptr);
#endif
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
#ifndef OWN_Object_SetPartAttribute
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
#endif
#ifndef OWN_Object_SetPositionAndResetMotionFar
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
#endif
#ifndef OWN_ObjectDispatch_ApplyValueToChildrenFar
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object, s32 value);
#endif
#ifndef OWN_Owner_RecalculateStatsFar
void Owner_RecalculateStatsFar(s32);
#endif
#ifndef OWN_Resource_DecodeByteLzInRam
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
#endif
#ifndef OWN_StartFlashTimer
void StartFlashTimer(u8 index);
#endif
#ifndef OWN_UiIcon_BuildAbilityIconTiles
void UiIcon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
#endif
#ifndef OWN_UiText_DrawStringInWindowFar
void UiText_DrawStringInWindowFar(u8 *text, s32 arg1, u32 x, u32 y);
#endif

#endif
