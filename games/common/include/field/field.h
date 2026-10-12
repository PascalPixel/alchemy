#ifndef FIELD_H
#define FIELD_H

/* FIELD */

#include "types.h"
#include "edition.h"
#include "object_runtime.h"
#if !GAME_TLA
#include "fieldrun.h"
#include "field_scene.h"
#include "menu_list.h"
#endif
#if !GAME_TLA
#include "farcall.h"
#endif

struct DispatchObject;
struct EffectPosition;

#if GAME_TLA
extern const s32 Camera_FixedViewMatrix[];
#endif
extern s32		 Iwram_Data1;
extern s32		 Iwram_Data2;
#if GAME_TLA
extern u8		 Map_TileDissolveOrder[];
#endif
extern const u32 ObjectDispatch_DefaultScript[];
extern u8		 Render_DecodeFrameCodeSize[];
extern u8		 Runtime_ByteRemapTable[];
#if GAME_TLA
s32				 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32				 Event_ValidatePackedId(u32 packed_id);
void			 Map_RenderAnimatedTileFrame(u8 *object, u32 position);
void			 Object_AttachWorkTargetToObject(s32 id, s32 flag);
void			 Object_UpdateAllMotion(void);
void			 Object_UpdateAllThumb(void);
#endif
#ifndef OWN_ObjectDispatch_SetSingleChildFlags
#if GAME_TLA
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *object, u32 value);
#endif
#endif
#if GAME_TLA
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
s32	 ObjectTable_FindLastActiveId(void);
#endif
#if GAME_TLA
void BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);
s32	 Equipment_HasValueFar(s32 group, s32 entry);
s32	 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32	 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
#endif
#if GAME_TLA
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
#endif

#if !GAME_TLA
struct AbilityListEntry;
struct BattleEffectArgument;
struct BtlUnit;
struct BurstParticle;
struct DispatchObject;
struct EffectObject_02000c5c;
struct EffectPosition;
struct EffectRecord;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct FieldPosition;
struct MakyuriLights;
struct MapActor;
struct MapCell;
struct MenuSprite;
struct ModeRecord;
struct MotionObject;
struct ObjectRuntime;
struct PillarEffect;
struct S_02000474;
struct SceneEntrance;
struct SceneEntry;
struct SceneEvent;
struct SceneObject_02000b98;
struct ScenePlacement;
struct SceneRegion;
struct SceneRegionEntry;
struct ScriptTable;
struct SparkleFrame;
struct Sub;
struct Swirl;
struct TileRun2;
struct V;
union FieldObject;

struct Actor_02000640
{
	u8	reserved00[6];
	u16 facing;
	u8	reserved08[92];
	u16 state_flags;
};
struct Actor_02000754
{
	u8 reserved00[91];
	u8 accepted;
};
struct Actor_020007d4
{
	u8 reserved00[91];
	u8 accepted;
};
struct EffectWork
{
	u8					 pad[80];
	struct EffectRecord *record;
};
struct Flags35
{
	u8 pad[35];
	u8 flags;
};
struct Flags9
{
	u8 pad[9];
	u8 low : 2;
	u8 mode : 2;
};
struct HeightTrackedObject
{
	u8	pad00[12];
	s32 height;
};
struct Obj_02002608
{
	u8			pad00[0x18];
	s32			f18;
	u8			pad1c[7];
	u8			f23;
	u8			pad24[12];
	s32			f30;
	s32			f34;
	u8			pad38[24];
	struct Sub *f50;
	u8			pad54[1];
	u8			f55;
};
struct OverlayActorPosition
{
	u8	pad00[8];
	s32 depth_fixed;
};
struct OverlayActorState
{
	u8 pad00[35];
	u8 flags;
};
struct OverlayEffectMotion
{
	u8	pad00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	pad14[28];
	s32 horizontal_rate;
	s32 vertical_rate;
	s32 shadow_x;
	s32 shadow_y;
	s32 shadow_z;
	u8	pad44[32];
	s16 mode;
};
struct PathRecorder
{
	s16 mode;
	s16 mirror;
	s16 actor;
	u16 pos;
	s16 still;
};
struct Presentation
{
	u8 reserved_00[9];
	u8 flags;
};
struct Presentation_02000c1c
{
	u8 unk_00[9];
	u8 flags;
};
struct S_020009dc
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
	u8	pad14[15];
	u8	f23;
};
struct SceneActor_020004b4
{
	u8					 reserved_00[35];
	u8					 state_23;
	u8					 reserved_24[44];
	struct Presentation *presentation;
};
struct SceneActor_02000c1c
{
	u8							  unk_00[35];
	u8							  state_23;
	u8							  unk_24[44];
	struct Presentation_02000c1c *presentation;
};
struct SceneActor_0200113c
{
	u8					 reserved_00[35];
	u8					 state_23;
	u8					 reserved_24[44];
	struct Presentation *presentation;
};
struct SceneWork_020006b4
{
	u8	reserved000[472];
	u16 branch_counter;
};
struct SceneWork_02000e90
{
	u8	reserved000[370];
	u16 actor18_marker;
};
struct ScrollLayer
{
	u8	unknown_00[8];
	s32 x;
};
struct Slot
{
	u16 f00;
	u16 f02;
	u16 f04;
	u16 f06;
};
struct SwitchCell
{
	u32 x;
	u32 z;
};
struct SwitchEffect
{
	s32 active;
	u8	unknown_04[0x5f];
	u8	finished;
};
struct T
{
	u8	pad00[30];
	u16 f1e;
};
struct TileRun
{
	s16 id;
	s16 x;
	s16 y;
	s16 vertical;
	s16 unused08;
	s16 unused0a;
};
struct V6
{
	s32 a;
	s32 b;
	s32 c;
	s32 d;
	s32 e;
	s32 f;
};
struct WorldMapVramBlock
{
	u16 base;
	u16 offset;
};
#ifndef OWN_ArutinYama_ActorScript
#endif
extern s16		 ArutinYama_PaletteHold;
extern s16		 ArutinYama_PaletteStep;
#ifndef OWN_BabiFune_CountTicks
#endif
#ifndef OWN_BabiFune_DriftActive
#endif
extern s16		 BabiFune_FadeSlot;
extern s16		 BabiFune_FadeStep;
#ifndef OWN_BabiFune_ShimmerActive
#endif
#ifndef OWN_BabiFune_ShimmerPhase
#endif
#ifndef OWN_BabiFune_SwellActive
#endif
#ifndef OWN_BabiFune_SwellLayerSevenY
#endif
#ifndef OWN_BabiFune_SwellLayerSixY
#endif
extern const u8				 BattleFx_BurstParticleScriptA[];
extern const u8				 BattleFx_BurstParticleScriptB[];
extern u32					 WorldMap_GraphicsResources[][6];
extern const u8				 ObjectMotion_WaitScript[];
extern const u8				 FieldFx_SparkleTiles[];
extern const u8				 Field_TileMessageOffsets[];
extern const u8				 FieldFx_MoteTiles[];
extern s32					 FourObjectMotion_ResourceIds[];
extern s32					 FuneHeya_CueTimer;
#ifndef OWN_FuneHeya_TurnSteps
#endif
extern s16						  *gKorimaMagariLayout;
extern s16						  *gKorimaMagariRecords;
extern u16						  *gKorimaMagariReturned;
#ifndef OWN_gObjectSlots
extern struct ObjectRuntime *gObjectSlots;
#endif
extern u32						   gScanlineAngle;
#ifndef OWN_gScanlinePitch
extern u32 gScanlinePitch;
#endif
extern struct SceneEvent		   gTorebiKyudenEvents2AfterColosso[];
extern const u8					   gWorldMapPackedFrames[];
extern const u8					   gWorldMapPackedTiles[];
extern const u8					   gWorldMapPalettes[];
extern s32						   KareiMachi_DanceStep;
extern u32						   KorimaMagari_ShakeChance;
extern u32						   KorimaMagari_ShakeScroll[];
extern u8						   KorimaPalette_First[];
extern u8						   KorimaPalette_Second[];
extern s32						   Korosseo_CompetitorStartAngle;
extern s32						   Korosseo_CompetitorStartX;
extern s32						   Korosseo_CompetitorStartZ;
extern u16						   Korosseo_GaugePalette[];
extern s32						   Korosseo_MarkerOam[3];
extern s16						   Korosseo_ModeBlendDuration;
extern s16						   Korosseo_ModeBlendStart;
extern s16						   Korosseo_ModeBlendStep;
extern s16						   Korosseo_ModeBlendTarget;
extern s16						   Korosseo_ModeMoveStart;
extern s16						   Korosseo_ModeMoveStep;
extern s16						   Korosseo_ModeScaleDuration;
extern s16						   Korosseo_ModeScaleStart;
extern s16						   Korosseo_ModeScaleStep;
extern s16						   Korosseo_ModeScaleTarget;
extern u32						   Korosseo_ModeTaskSprites[];
#ifndef OWN_KuupuappuHeya_PairScriptP
#endif
#ifndef OWN_KuupuappuHeya_PairScriptR
#endif
extern u16					 KuupuappuHeya_StopTimer;
#ifndef OWN_Makyuri_PillarScript
#endif
extern u8					   Object_UpdateAllCodeSize[];
extern u8					   ObjectGroup_BlinkChildValues[];
extern const u8				   ObjectMotion_ActionKind1Script[];
extern const u8				   ObjectMotion_ActionKind2Script[];
extern const u8				   ObjectMotion_ActionKind3Script[];
extern const u8				   ObjectMotion_ActionKind4Script[];
extern u8					   ObjectMotion_LaunchScript;
extern const u8				   ObjectMotion_MoveTowardTargetScript[];
extern const u8				   ObjectMotion_ResetActionScript[];
extern const u8				   ObjectMotion_TurnTowardLinkedScript[];
extern const u8				   ObjectMotion_VariantScripts[];
#ifndef OWN_RamakanSabaku_SandCounter
#endif
extern u8		 RamakanSabaku_SandEffectTiles[];
extern const u16 RamakanSabaku_SandPalette[16];
#ifndef OWN_RamakanSabaku_SandPhase
extern u16 RamakanSabaku_SandPhase;
#endif
extern u8 RamakanSabaku_SandTileBuffer[];
#ifndef OWN_RamakanSabaku_SandVramSlot
extern u16 RamakanSabaku_SandVramSlot;
#endif
extern struct MenuSprite RamakanSabaku_SandWork[];
#ifndef OWN_Render_DecodeFrame
extern u8 Render_DecodeFrame[];
#endif
extern u32					 StagedActor_FootprintKinds[];
extern s32					 TakaraHashira_ProbePosition[];
extern const u8				 TileMap_DrawRows[];
extern u8					 TileMap_DrawRowsCodeSize[];
extern u8					 Transform_UpdateVerticesSize;
#ifndef OWN_VinasuChojo_BeamOpenAction
#endif
#ifndef OWN_VinasuChojo_ScaleStepAction
#endif
extern s32						   VinasuChojo_TransitionStep;
extern s32						   VinasuChojo_TransitionTimer;
extern const u8					   WorldMap_FootprintActions[];
extern const s16				   WorldMap_KeyHeadings[16];
extern u8						   WorldMap_TerrainBehaviorTable[];
s32	 CalculateFacingAngle(s32, s32);
#ifndef OWN_Camera_ApplyShake
#endif
#ifndef OWN_EffectPosition_ApplyAlternateStepAndYOffset
#endif
#ifndef OWN_EffectPosition_ApplyAnimationAndYOffset
#endif
#ifndef OWN_EffectRuntime_GetCurrentObject
#endif
void Engine_ActorFollow(s32 actor, s32 leader);
void Engine_EventClearInvalidPackedValues(void);
s32	 Engine_PartyRemoveItem(s32 item);
#ifndef OWN_Event_RequestDestination
#endif
s32	 FarCall_BlankLoadTable(s32 entrance);
s32	 FarCall_BlankRunTable(s32 entrance);
#ifndef OWN_Field_RunTileAction
#endif
#ifndef OWN_Field_TryJumpForward
#endif
#ifndef OWN_FieldScene_BuildSceneDescriptorAndInstallTask
#endif
#ifndef OWN_FieldScene_PrepareActors
void FieldScene_PrepareActors(struct SceneRegionEntry *entry);
#endif
#ifndef OWN_FourObjectMotion_InitializeBottomRow
#endif
#ifndef OWN_GameFlag_IsSet
s32 GameFlag_IsSet(s32 flag);
#endif
#ifndef OWN_Makyuri_SpawnLightObjects
#endif
void Map_ResumeAnimation(void);
#ifndef OWN_MapGroupTable_SelectEntry
#endif
s32	 Math_Atan2(s32 dy, s32 dx);
#ifndef OWN_Menu_FindShortcutEntries
#endif
#ifndef OWN_Object_CollectResources
#endif
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
#ifndef OWN_Object_ReplaceResourceEntry
#endif
#ifndef OWN_ObjectDispatch_InitFromTable4WithArgument
#endif
#ifndef OWN_ObjectDispatch_RegisterChildMetadata
#endif
#ifndef OWN_ObjectGroup_UpdateMembers
#endif
#ifndef OWN_ObjectTable_ResetForObject
#endif
#ifndef OWN_ObjectVisual_CopyAttributes
#endif
#ifndef OWN_OverlayObject_PrepareObject
#endif
#ifndef OWN_OverlayObject_WaitUntilSettledAndReset
#endif
#ifndef OWN_RamakanSabaku_EmitSandEffect
#endif
#ifndef OWN_Scene_FadeColorFromWhite
#endif
#ifndef OWN_Scene_PushBlockAlongRun
#endif
#ifndef OWN_SceneActor_PlaceFiveActorsInRow
#endif
#ifndef OWN_SceneActor_UpdateTracking
#endif
#ifndef OWN_SceneData_FindTileRunAt
#endif
#ifndef OWN_SceneEffect_SpawnEffect284AtCell
#endif
#ifndef OWN_SceneEffect_SpawnRandomizedParticleEveryFourFrames
#endif
void ScenePlacement_ClipToView(s32 placements);
void SceneTable_Prepare(void *);
#ifndef OWN_StagedActor_CountdownUntilPositionUnset
#endif
#ifndef OWN_StoryActor_ConfigureSpawnedObject
#endif
#ifndef OWN_TakaraHashira_ReadMapCell
#endif
#endif

#if GAME_TLA
struct AnimationObject;
struct EffectPosition;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct ObjectRuntime;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct ScriptInterpreter;

extern const u8					   DriftScene_SpawnScript[];
extern const struct SceneEntrance  gDeriMuraEntrances[];
extern const struct SceneEntrance  gIdejimaEntrances[];
extern const struct SceneEvent	   gIdejimaEvents[];
extern const struct SceneEvent	   gIdejimaEventsEntrance1[];
extern const struct SceneEvent	   gIdejimaEventsWake[];
extern const u32				   gIdejimaExits[];
extern const struct ScenePlacement gIdejimaPlacements[];
extern const struct SceneEntrance  gSceneEntrances[];
extern const struct SceneEvent	   gSceneEvents[];
extern const u32				   gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct ScenePlacement gScenePlacementsEntrance99[];
extern const struct SceneRegion	   gSceneRegions[];
extern struct ObjectRuntime		   gSnapshotObjects[];
extern const u8					   Idejima_ActorDepartureScript[];
extern u8						   LinkLobby_SlotColumns[];
extern const s32				   LinkLobby_SlotValues[];
extern const u8					   ObjectGroup_BlinkChildValues[4];
extern const u8					   ObjectMotion_StepAngleScript[];
extern u16						   Script_MainScript[];
extern const u8					   WorldMap_TerrainBehaviorTable[];
s32								   Animation_InitializeObjects(struct AnimationObject *);
void							   Animation_SetStateFlags(struct FieldActor *object, s32 flags);
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *, s32);
s32	 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
void BattleFx_ClearChildValueOnMismatch(void);
s32	 BattleFx_DispatchRequestKind(void);
void BattleFx_LoadActionEffectResources(s32 action, s32 flags);
s32	 BattleFx_Run(void);
s32	 BattleFx_SelectBattleCue(s32 row, s32 column);
void BattleFx_SetupObjectPair(s32 actor, s32 target);
s32	 BattleFx_StartBufferInterpolationFar(s32);
void BattleMotion_ProjectScaledPositionFar(s32, struct EffectPosition *);
void BattlePres_RunBurstScene(struct EffectStep *, s32);
void EffectRuntime_StopCurrentObject(void);
s32	 Event_SpawnObjectTable(s32 event_id, s32 state);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second);
void Field_BeginPaletteTransition(s32 mode);
void Field_DispatchTypeHandler(s32 kind);
void Field_EndActorSpriteEffect(s32 mode);
void FieldEvent_RunTypeHandler(void);
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
s32	 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);
void Func_080caa2c(u32 object_id);
void Func_080cee80(u32 object_id);
void Func_080e70f8(u32 object_id);
void Func_081b83c4(void);
void GameFlag_RefreshLureCapFar(void);
void Map_ApplyWorkOriginAndSpanFar(void);
s32	 Map_GetTerrainHeightFar(u8, s32, s32);
void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant);
s32	 Object_IsTargetUnset(void *arg0);
#ifndef OWN_Object_SetPartAttribute
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
#endif
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void ObjectDispatch_InitFromTable4WithArgumentFar(void *object, void *effect);
void ObjectEffect_EndContextEffect(s32 effect);
void ObjectEffect_PrepareContextEffect(s32 effect);
void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset);
void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset);
void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z);
void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant);
void ObjectSystem_Configure(s32 mode);
s32	 Party_AddActiveOwnerFar(s32 event_id);
void Party_RemoveActiveOwnerFar(s32 owner);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale, u16 angle);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale, s32 angle, s32 layer);
s32	 Render_ProjectPoint(s32 *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32	 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
void UiTimedNotice_CloseIfActiveFar(void);
s32	 UiWork_IsIdleFar(s32);
#endif

#endif
