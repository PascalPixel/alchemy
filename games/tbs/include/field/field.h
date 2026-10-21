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

/// An IWRAM word the scrolling camera's setup clears; nothing in the tree
/// reads it (see ObjectSystem_Initialize's XXX).
extern s32 gIwram_Data1;
/// The second IWRAM word the scrolling camera's setup clears; also unread.
extern s32 gIwram_Data2;
extern const s32 gObjectDispatch_DefaultScript[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
/// The size of Render_DecodeFrame, copied to HEAP_SLOT_FRAME_SCRATCH; an
/// address equal to the size (constants.ld).
extern u8 Render_DecodeFrameCodeSize[];
/// Four 256-byte colour tables (remap0 to remap3), indexed by pixel: the
/// SpriteColorMode tables after gResourceSlot_ConversionTables' five, the
/// first also remapping the command icons (Runtime_RemapBytesByTable).
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
/// No code in the tree uses it.
struct Flags35 {
  u8 pad[35];
  u8 flags;
};
/// An actor seen only as the coordinate at 0x08. No code in the tree uses it.
struct OverlayActorPosition {
  u8 pad00[8];
  s32 depthFixed; ///< 16.16 fixed-point pixels
};
/// An actor seen only as its priority flags at 0x23 (enum ActorPriorityFlag).
/// No code in the tree uses it.
struct OverlayActorState {
  u8 pad00[35];
  u8 flags;
};
/// The Colosso rival's path player, at the start of gSceneState: the path in
/// ColossoRaceWork is played back or recorded by Colosso_UpdatePathRival.
struct PathRecorder {
  s16 mode;   ///< 1 plays the path, 2 records it, 9 finished
  s16 mirror; ///< nonzero mirrors the path across the finish
  s16 actor;  ///< the actor id moved along the path
  u16 pos;    ///< the next halfword of ColossoRaceWork.path, x then z
  /// Frames the actor has not moved; from 3 it stands (Object_SetMode 1).
  s16 still;
};
/// A placeholder layout no code in the tree uses.
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
/// A placeholder layout that only struct S, itself unused, points to.
struct T {
  u8 pad00[30];
  u16 f1e;
};
/// A log in Kolima Forest lying across four map cells, from its first cell
/// across or, when vertical, down. No code in the tree uses it; the bend reads
/// its logs as struct FloodgateLog (kolima_forest_floodgate.h), of the same 12
/// bytes.
struct TileRun {
  s16 id;
  s16 x; ///< map cells
  s16 y; ///< map cells
  s16 vertical;
  s16 unused08;
  s16 unused0a;
};
/// Frames left on the water palette's current step (gAltinPeak_PaletteScript).
extern s16 gAltinPeak_PaletteHold;
/// The next byte of gAltinPeak_PaletteScript; 0 again at its -1.
extern s16 gAltinPeak_PaletteStep;
/// One of a ledge jump's dust puffs (FieldFx_SpawnBurstParticle): SC_WAIT(1)
/// looped 38 times, then SC_FREE.
extern const u8 gBattleFx_BurstParticleScriptA[];
/// The other dust puff: the same, looped 18 times.
extern const u8 gBattleFx_BurstParticleScriptB[];
extern u32 gWorldMap_GraphicsResources[][6];
/// SC_WAIT(40), then SC_FREE: the one-off attached effect that ends by itself
/// (BattleFx_ConfigureLinkedObject, ATTACHED_EFFECT_ONCE).
extern const u8 gObjectMotion_WaitScript[];
/// The sparkles' 16 by 16 tiles, packed (FieldFx_InitSparkles).
extern const u8 gFieldFx_SparkleTiles[];
extern const u8 gField_TileMessageOffsets[];
/// The motes of light's 16 by 16 tiles, packed (FieldMotes_Start).
extern const u8 gFieldFx_MoteTiles[];
extern s32 gFourObjectMotion_ResourceIds[];
extern s32 gTolbiShipInteriors_ShakeTimer;
/// The bend's water level in gSceneState's second halfword: 1 high (the logs
/// float), 0 low; the floodgate switch flips it.
extern s16 *gKolimaForestFloodgateLayout;
struct FloodgateLog;
/// The bend's logs in gSceneState from byte 4, so they survive a battle.
extern struct FloodgateLog *gKolimaForestFloodgateRecords;
/// The short log's place in gSceneState's first halfword: 1 west, 0 east.
extern u16 *gKolimaForestFloodgateReturned;
#ifndef OWN_gObjectSlots
/// Work slot 5: the 64 field objects, 0x70 bytes each.
extern struct ObjectRuntime *gObjectSlots;
#endif
/// The world map camera's yaw the scanline parameters were built for; the
/// vertex update rebuilds them when it changes.
extern u32 gScanlineAngle;
#ifndef OWN_gScanlinePitch
/// The world map camera's pitch the scanline parameters were built for.
extern u32 gScanlinePitch;
#endif
/// The events of Babi's palace's second map once FLAG_COLOSSO_OVER is set
/// (Scene_GetEvents).
extern struct SceneEvent gBabiPalace_Events2AfterColosso[];
/// The lighthouse transition's frames, packed (type 01), decoded 0x1000 bytes
/// after the tiles (WorldMap_PlayLighthouseTransition).
extern const u8 gWorldMapPackedFrames[];
/// The lighthouse transition's tiles, packed (type 01).
extern const u8 gWorldMapPackedTiles[];
/// The lighthouse transitions' palettes, 32 bytes each, loaded as BG palette
/// 14.
extern const u8 gWorldMapPalettes[];
/// Which of the two guides' turns Kalay_AlternateGuidesPacing is at, 0 to 4.
extern s32 gKalay_DanceStep;
/// The percent chance that a line above the split shows the shaken scroll: 0
/// when the rumble starts (SE_231), then a point higher each frame up to 101
/// (KolimaForestFloodgate_FlipFloodgateSwitch).
extern u32 gKolimaForestFloodgate_ShakeChance;
/// The background scroll words captured each frame for the shaken lines.
extern u32 gKolimaForestFloodgate_ShakeScroll[];
/// The healthy colours: the first 448 colours (896 bytes) of the colour
/// buffer, kept by KolimaPalette_SaveFirst; the array is 1792 bytes. Kolima
/// Forest's scenes 41 and 42 each define one.
extern u8 gKolimaPalette_First[];
/// The cursed colours, kept by KolimaPalette_SaveSecond.
extern u8 gKolimaPalette_Second[];
/// Isaac's facing before Colosso_FadeInCompetitor, which
/// Colosso_RestoreCompetitor puts back.
extern s32 gColosso_CompetitorStartAngle;
/// Isaac's x before he is shown on the stage, 16.16 fixed-point pixels.
extern s32 gColosso_CompetitorStartX;
/// Isaac's z before he is shown on the stage, 16.16 fixed-point pixels.
extern s32 gColosso_CompetitorStartZ;
/// The race gauge's 16 colours, loaded as OBJ palette 14 (Colosso_DrawGauge).
extern u16 gColosso_GaugePalette[];
/// The blinking pointer's sprite part: the queue link and attributes 0 to 2.
extern s32 gColosso_MarkerOam[3];
// The lettering's three eases (Colosso_UpdateLettering): each value goes from
// Start to Target over Duration frames, Step counting them; Duration 0 holds
// Target. Blend is the lettering's BLDALPHA weight out of 16, Scale its size
// with 256 for full, and Move what is added to its x each frame, in 1/256
// pixels.
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
/// The lettering's sprite parts, three words each, written every frame.
extern u32 gColosso_LetteringSprites[];
/// Frames in a row the chase has found a suspect cornered; at
/// VAULT_INTERIORS_CORNERED_FRAMES the chase raises its event.
extern u16 gVaultInteriors_StopTimer;
/// The size of Object_UpdateAll's code, an address equal to the size
/// (constants.ld). Only the unused Object_RunUpdateAllFromHeap reads it, to
/// copy the routine to the heap and run it once.
extern u8 Object_UpdateAllCodeSize[];
extern u8 gObjectGroup_BlinkChildValues[];
// The object motion scripts: actions 1 to 7, which
// ObjectMotion_SetActionCallback starts by number, and two turning scripts
// with no action number.
/// Action 1: a lone SC_END.
extern const u8 gObjectMotion_ActionKind1Script[];
/// Action 2: wanders within 16 pixels of home (SC_WANDER_HOME) at half a
/// pixel a frame.
extern const u8 gObjectMotion_ActionKind2Script[];
/// Action 3: the same within 8 pixels of home.
extern const u8 gObjectMotion_ActionKind3Script[];
/// Action 4: wanders within 6 pixels of home at 0.4 pixels a frame.
extern const u8 gObjectMotion_ActionKind4Script[];
/// Waits to arrive, then turns by ObjectMotion_StepAngle until it faces the
/// kept angle (Motion_LaunchFromFocusedObject).
extern u8 gObjectMotion_LaunchScript;
/// Action 5: ObjectMotion_MoveTowardTarget every frame, forever.
extern const u8 gObjectMotion_MoveTowardTargetScript[];
/// Action 7: Object_ClearChildAndCollisionFlags once, then SC_END.
extern const u8 gObjectMotion_ResetActionScript[];
/// Turns by ObjectMotion_StepAngle until it faces the kept angle, then SC_END.
extern const u8 gObjectMotion_StepAngleScript[];
/// Action 6: turns toward the leader every frame, forever.
extern const u8 gObjectMotion_TurnTowardLinkedScript[];
extern const u8 gObjectMotion_VariantScripts[];
// Lamakan Desert's heat gauge (HeatGauge_Start and HeatGauge_Draw).
/// The gauge's tiles, packed (type 01).
extern u8 gLamakanDesert_SandEffectTiles[];
/// The gauge's 16 colours, loaded as OBJ palette 14.
extern const u16 gLamakanDesert_SandPalette[16];
#ifndef OWN_LamakanDesert_SandPhase
/// 1 while the gauge is held on screen (HeatGauge_SetHeld).
extern u16 gLamakanDesert_SandPhase;
#endif
/// The gauge's decoded tiles, which each frame's drawing starts from.
extern u8 gLamakanDesert_SandTileBuffer[];
#ifndef OWN_LamakanDesert_SandVramSlot
/// The VRAM cache entry holding the gauge's tiles.
extern u16 gLamakanDesert_SandVramSlot;
#endif
/// The gauge's five sprites: four wide ones and a narrow cap at its right end.
extern struct MenuSprite gLamakanDesert_SandWork[];
#ifndef OWN_Render_DecodeFrame
/// The sprite frame decoder, ARM code run from a copy in
/// HEAP_SLOT_FRAME_SCRATCH.
extern u8 Render_DecodeFrame[];
#endif
/// The statue scene's three scrolling strips, packed (Resource_DecodeByteLz).
extern u8 gSolSanctumStatue_SpriteRowsGfx[];
/// The six large things' sprite animations, which pick their footprints in
/// gStagedActor_FootprintBounds; each scene with them defines its own.
extern u32 gStagedActor_FootprintKinds[];
/// The tilemap row drawer, run from a copy in HEAP_SLOT_TEMP_RENDERER.
extern const u8 TileMap_DrawRows[];
/// The size of TileMap_DrawRows (decode.s).
extern u8 TileMap_DrawRowsCodeSize[];
/// The size of Transform_UpdateVertices, copied to work slot 46; an address
/// equal to the size (constants.ld).
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
