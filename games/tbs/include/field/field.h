#ifndef ALCHEMY_FIELD_FIELD_H
#define ALCHEMY_FIELD_FIELD_H

/// @file
/// The field engine's shared routines, globals and object scripts.

#include "types.h"
#include "edition.h"
#include "object_runtime.h"
#include "field_runtime.h"
#include "field_scene.h"
#include "menu_list.h"
#include "farcall.h"

struct DispatchObject;
struct EffectPosition;

extern s32 gIwram_Data1;
extern s32 gIwram_Data2;
extern const s32 gObjectDispatch_DefaultScript[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
extern u8 Render_DecodeFrameCodeSize[];
extern u8 gRuntime_ByteRemapTable[];

struct AbilityListEntry;
struct BattleEffectArgument;
struct BattleUnit;
struct BurstParticle;
struct DispatchObject;
struct FieldEffectObject;
struct EffectPosition;
struct EffectStep;
struct FacingObject;
struct FieldActor;
struct FieldPosition;
struct MercuryLighthouseLights;
struct MapActor;
struct MapCell;
struct MenuSprite;
struct ModeRecord;
struct MotionObject;
struct ObjectRuntime;
struct FieldRecord;
struct SceneEntrance;
struct SceneEntry;
struct SceneEvent;
struct FieldSceneObject;
struct ScenePlacement;
struct SceneRegion;
struct SparkleFrame;
struct Swirl;
struct TileRun2;
struct V;
union FieldObject;

/// An actor seen only as its priority flags at 0x23 (enum ActorPriorityFlag).
struct Flags35 {
  u8 pad[35];
  u8 flags;
};
/// An actor seen only as the coordinate at 0x08, which Goma Cave's stump
/// reads in 16-pixel map cells.
struct OverlayActorPosition {
  u8 pad00[8];
  s32 depthFixed; ///< 16.16 fixed-point pixels
};
/// An actor seen only as its priority flags at 0x23 (enum ActorPriorityFlag).
struct OverlayActorState {
  u8 pad00[35];
  u8 flags;
};
/// The Colosso rival's path player in the scene state: its mode (1 playing, 2
/// recording, 9 finished), whether it mirrors the path, the actor, its place
/// in the path and how long the actor has stood still.
struct PathRecorder {
  s16 mode;
  s16 mirror;
  s16 actor;
  u16 pos;
  s16 still; ///< frames
};
struct Slot {
  u16 f00;
  u16 f02;
  u16 f04;
  u16 f06;
};
/// A Venus Lighthouse switch or pillar socket's map cell.
struct SwitchCell {
  u32 x; ///< 16-pixel map cells
  u32 z; ///< 16-pixel map cells
};
/// A Venus Lighthouse gate effect, seen as whether it still runs and whether
/// it has finished.
struct SwitchEffect {
  s32 active;
  u8 unk04[0x5f];
  u8 finished;
};
struct T {
  u8 pad00[30];
  u16 f1e;
};
/// A log in Kolima Forest lying across four map cells, from its first cell
/// across or, when vertical, down.
struct TileRun {
  s16 id;
  s16 x; ///< map cells
  s16 y; ///< map cells
  s16 vertical;
  s16 unused08;
  s16 unused0a;
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
extern s32 gTolbiShipInteriors_ShakeTimer;
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
extern struct SceneEvent gBabiPalace_Events2AfterColosso[];
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
extern u32 gColosso_LetteringSprites[];
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
extern u8 gSolSanctumStatue_SpriteRowsGfx[];
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
void WorldMap_BeginStillView(void);
s32 Math_Atan2(s32 dy, s32 dx);
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
void ScenePlacement_ClipToView(struct ScenePlacement *placements);
void SceneTable_Prepare(struct ScenePlacement *placements);

#endif
