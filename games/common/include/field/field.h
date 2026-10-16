#ifndef FIELD_H
#define FIELD_H

// FIELD

#include "types.h"
#include "edition.h"
#include "object_runtime.h"
#if !GAME_TLA
#include "field_runtime.h"
#include "field_scene.h"
#include "menu_list.h"
#include "farcall.h"
#endif

struct DispatchObject;
struct EffectPosition;

#if GAME_TLA
extern const s32 gCamera_FixedViewMatrix[];
#endif
extern s32 gIwram_Data1;
extern s32 gIwram_Data2;
#if GAME_TLA
extern u8 gMap_TileDissolveOrder[];
#endif
extern const s32 gObjectDispatch_DefaultScript[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
extern u8 Render_DecodeFrameCodeSize[];
extern u8 gRuntime_ByteRemapTable[];
#if GAME_TLA
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32 Event_ValidatePackedId(u32 packed_id);
void Map_RenderAnimatedTileFrame(u8 *object, u32 position);
void Camera_FollowActor(s32 id, s32 flag);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
#ifndef OWN_ObjectDispatch_SetSingleChildFlags
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *object,
                                        u32 value);
#endif
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
s32 ObjectTable_FindLastActiveId(void);
void BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);
s32 Unit_KnowsActionFar(s32 group, s32 entry);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
#endif

#if !GAME_TLA
struct AbilityListEntry;
struct BattleEffectArgument;
struct BattleUnit;
struct BurstParticle;
struct DispatchObject;
struct FieldEffectObject;
struct EffectPosition;
struct EffectRecord;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct FieldPosition;
struct MercuryLights;
struct MapActor;
struct MapCell;
struct MenuSprite;
struct ModeRecord;
struct MotionObject;
struct ObjectRuntime;
struct PillarEffect;
struct FieldRecord;
struct SceneEntrance;
struct SceneEntry;
struct SceneEvent;
struct FieldSceneObject;
struct ScenePlacement;
struct SceneRegion;
struct ScriptTable;
struct SparkleFrame;
struct Sub;
struct Swirl;
struct TileRun2;
struct V;
union FieldObject;

struct FacingStateActor {
  u8 reserved00[6];
  u16 facing;
  u8 reserved08[92];
  u16 stateFlags;
};
struct AcceptActor11 {
  u8 reserved00[91];
  u8 accepted;
};
struct AcceptActor13 {
  u8 reserved00[91];
  u8 accepted;
};
struct EffectWork {
  u8 pad[80];
  struct EffectRecord *record;
};
struct Flags35 {
  u8 pad[35];
  u8 flags;
};
struct Flags9 {
  u8 pad[9];
  u8 low : 2;
  u8 mode : 2;
};
struct HeightTrackedObject {
  u8 pad00[12];
  s32 height;
};
struct FieldObjectRecord {
  u8 pad00[0x18];
  s32 f18;
  u8 pad1c[7];
  u8 f23;
  u8 pad24[12];
  s32 f30;
  s32 f34;
  u8 pad38[24];
  struct Sub *f50;
  u8 pad54[1];
  u8 f55;
};
struct OverlayActorPosition {
  u8 pad00[8];
  s32 depthFixed;
};
struct OverlayActorState {
  u8 pad00[35];
  u8 flags;
};
struct OverlayEffectMotion {
  u8 pad00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[28];
  s32 horizontalRate;
  s32 verticalRate;
  s32 shadowX;
  s32 shadowY;
  s32 shadowZ;
  u8 pad44[32];
  s16 mode;
};
struct PathRecorder {
  s16 mode;
  s16 mirror;
  s16 actor;
  u16 pos;
  s16 still;
};
struct Presentation {
  u8 reserved00[9];
  u8 flags;
};
struct PresentationB {
  u8 unk00[9];
  u8 flags;
};
struct GridMarkActor {
  s32 f00;
  s32 f04;
  s32 f08;
  s32 f0c;
  s32 f10;
  u8 pad14[15];
  u8 f23;
};
struct PresentedActor {
  u8 reserved00[35];
  u8 state23;
  u8 reserved24[44];
  struct Presentation *presentation;
};
struct PresentedActorB {
  u8 unk00[35];
  u8 state23;
  u8 unk24[44];
  struct PresentationB *presentation;
};
struct PresentedActorC {
  u8 reserved00[35];
  u8 state23;
  u8 reserved24[44];
  struct Presentation *presentation;
};
struct BranchCounterWork {
  u8 reserved000[472];
  u16 branchCounter;
};
struct Actor18MarkerWork {
  u8 reserved000[370];
  u16 actor18Marker;
};
struct ScrollLayer {
  u8 unk00[8];
  s32 x;
};
struct Slot {
  u16 f00;
  u16 f02;
  u16 f04;
  u16 f06;
};
struct SwitchCell {
  u32 x;
  u32 z;
};
struct SwitchEffect {
  s32 active;
  u8 unk04[0x5f];
  u8 finished;
};
struct T {
  u8 pad00[30];
  u16 f1e;
};
struct TileRun {
  s16 id;
  s16 x;
  s16 y;
  s16 vertical;
  s16 unused08;
  s16 unused0a;
};
struct V6 {
  s32 a;
  s32 b;
  s32 c;
  s32 d;
  s32 e;
  s32 f;
};
struct WorldMapVramBlock {
  u16 base;
  u16 offset;
};
extern s16 gAltinPeak_PaletteHold;
extern s16 gAltinPeak_PaletteStep;
extern const u8 gBattleFx_BurstParticleScriptA[];
extern const u8 gBattleFx_BurstParticleScriptB[];
extern u32 gWorldMap_GraphicsResources[][6];
extern const u8 gObjectMotion_WaitScript[];
extern const u8 gFieldFx_SparkleTiles[];
extern const u8 gField_TileMessageOffsets[];
extern const u8 gFieldFx_MoteTiles[];
extern s32 gFourObjectMotion_ResourceIds[];
extern s32 gTolbiShipInteriors_CueTimer;
extern s16 *gKolimaForestBendLayout;
extern s16 *gKolimaForestBendRecords;
extern u16 *gKolimaForestBendReturned;
#ifndef OWN_gObjectSlots
extern struct ObjectRuntime *gObjectSlots;
#endif
extern u32 gScanlineAngle;
#ifndef OWN_gScanlinePitch
extern u32 gScanlinePitch;
#endif
extern struct SceneEvent gBabiPalaceEvents2AfterColosso[];
extern const u8 gWorldMapPackedFrames[];
extern const u8 gWorldMapPackedTiles[];
extern const u8 gWorldMapPalettes[];
extern s32 gKalay_DanceStep;
extern u32 gKolimaForestBend_ShakeChance;
extern u32 gKolimaForestBend_ShakeScroll[];
extern u8 gKolimaPalette_First[];
extern u8 gKolimaPalette_Second[];
extern s32 gColosso_CompetitorStartAngle;
extern s32 gColosso_CompetitorStartX;
extern s32 gColosso_CompetitorStartZ;
extern u16 gColosso_GaugePalette[];
extern s32 gColosso_MarkerOam[3];
extern s16 gColosso_ModeBlendDuration;
extern s16 gColosso_ModeBlendStart;
extern s16 gColosso_ModeBlendStep;
extern s16 gColosso_ModeBlendTarget;
extern s16 gColosso_ModeMoveStart;
extern s16 gColosso_ModeMoveStep;
extern s16 gColosso_ModeScaleDuration;
extern s16 gColosso_ModeScaleStart;
extern s16 gColosso_ModeScaleStep;
extern s16 gColosso_ModeScaleTarget;
extern u32 gColosso_ModeTaskSprites[];
extern u16 gVaultInteriors_StopTimer;
extern u8 Object_UpdateAllCodeSize[];
extern u8 gObjectGroup_BlinkChildValues[];
extern const u8 gObjectMotion_ActionKind1Script[];
extern const u8 gObjectMotion_ActionKind2Script[];
extern const u8 gObjectMotion_ActionKind3Script[];
extern const u8 gObjectMotion_ActionKind4Script[];
extern u8 gObjectMotion_LaunchScript;
extern const u8 gObjectMotion_MoveTowardTargetScript[];
extern const u8 gObjectMotion_ResetActionScript[];
extern const u8 gObjectMotion_StepAngleScript[];
extern const u8 gObjectMotion_TurnTowardLinkedScript[];
extern const u8 gObjectMotion_VariantScripts[];
extern u8 gLamakanDesert_SandEffectTiles[];
extern const u16 gLamakanDesert_SandPalette[16];
#ifndef OWN_LamakanDesert_SandPhase
extern u16 gLamakanDesert_SandPhase;
#endif
extern u8 gLamakanDesert_SandTileBuffer[];
#ifndef OWN_LamakanDesert_SandVramSlot
extern u16 gLamakanDesert_SandVramSlot;
#endif
extern struct MenuSprite gLamakanDesert_SandWork[];
#ifndef OWN_Render_DecodeFrame
extern u8 Render_DecodeFrame[];
#endif
extern u8 gSolStatue_SpriteRowsGfx[];
extern u32 gStagedActor_FootprintKinds[];
extern const u8 TileMap_DrawRows[];
extern u8 TileMap_DrawRowsCodeSize[];
extern u8 Transform_UpdateVerticesSize;
extern const s32 gWorldMap_FootprintActions[6];
extern const s16 gWorldMap_KeyHeadings[16];
s32 CalculateFacingAngle(s32, s32);
void Engine_ActorFollow(s32 actor, s32 leader);
void Engine_EventClearInvalidPackedValues(void);
s32 Engine_PartyRemoveItem(s32 item);
s32 FarCall_BlankLoadTable(s32 entrance);
s32 FarCall_BlankRunTable(s32 entrance);
#ifndef OWN_FieldScene_PrepareActors
void FieldScene_PrepareActors(struct ScenePlacement *entry);
#endif
#ifndef OWN_GameFlag_IsSet
s32 GameFlag_IsSet(s32 flag);
#endif
void Map_ResumeAnimation(void);
s32 Math_Atan2(s32 dy, s32 dx);
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
void ScenePlacement_ClipToView(struct ScenePlacement *placements);
void SceneTable_Prepare(struct ScenePlacement *placements);
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

extern const u8 gDriftScene_SpawnScript[];
extern const struct SceneEntrance gDailaEntrances[];
extern const struct SceneEntrance gIdejimaEntrances[];
extern const struct SceneEvent gIdejimaEvents[];
extern const struct SceneEvent gIdejimaEventsEntrance1[];
extern const struct SceneEvent gIdejimaEventsWake[];
extern const u32 gIdejimaExits[];
extern const struct ScenePlacement gIdejimaPlacements[];
extern const struct SceneEntrance gSceneEntrances[];
extern const struct SceneEvent gSceneEvents[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct ScenePlacement gScenePlacementsEntrance99[];
extern const struct SceneRegion gSceneRegions[];
extern struct ObjectRuntime gSnapshotObjects[];
extern const u8 gIdejima_ActorDepartureScript[];
extern u8 gLinkLobby_SlotColumns[];
extern const s32 gLinkLobby_SlotValues[];
extern const u8 gObjectGroup_BlinkChildValues[4];
extern const u8 gObjectMotion_StepAngleScript[];
extern u16 gScript_MainScript[];
s32 Animation_InitializeObjects(struct AnimationObject *);
void Animation_SetStateFlags(struct FieldActor *object, s32 flags);
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *, s32);
s32 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
void BattleFx_ClearChildValueOnMismatch(void);
s32 BattleFx_DispatchRequestKind(void);
void BattleFx_LoadActionEffectResources(s32 action, s32 flags);
s32 BattleFx_Run(void);
s32 BattleFx_SelectBattleCue(s32 row, s32 column);
void BattleFx_SetupObjectPair(s32 actor, s32 target);
s32 BattleFx_StartBufferInterpolationFar(s32);
void BattleMotion_ProjectScaledPositionFar(s32, struct EffectPosition *);
void BattlePres_RunBurstScene(struct EffectStep *, s32);
void EffectRuntime_StopCurrentObject(void);
s32 Event_SpawnObjectTable(s32 event_id, s32 state);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first,
                                          struct FacingObject *second);
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
s32 Func_080c88c8(struct ScriptInterpreter *interpreter, s32 value);
void Func_080caa2c(u32 objectId);
void Object_SetSquareSprite(u32 objectId);
void Func_080e70f8(u32 objectId);
void Func_081b83c4(void);
void GameFlag_RefreshLureCapFar(void);
void Map_ApplyWorkOriginAndSpanFar(void);
s32 Map_GetTerrainHeightFar(u8, s32, s32);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
s32 Object_IsTargetUnset(void *arg0);
#ifndef OWN_Object_SetPartAttribute
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
#endif
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void ObjectDispatch_InitFromTable4WithArgumentFar(void *object, void *effect);
void ObjectEffect_EndContextEffect(s32 effect);
void ObjectEffect_PrepareContextEffect(s32 effect);
void ObjectMotion_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void ObjectMotion_CommitPositionAndActivate(u32 objectId, s32 x_offset,
                                            s32 z_offset);
void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 x_offset,
                                         s32 z_offset);
void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z);
void ObjectMotion_SetVariantCallback(u32 objectId, s32 variant);
void ObjectSystem_Configure(s32 mode);
s32 Party_AddActiveOwnerFar(s32 event_id);
void Party_RemoveActiveOwnerFar(s32 owner);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale,
                                    u16 angle);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale,
                                 s32 angle, s32 layer);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
void UiTimedNotice_CloseIfActiveFar(void);
s32 UiWork_IsIdleFar(s32);
#endif

#endif
