#ifndef ALCHEMY_FIELD_FIELD_H
#define ALCHEMY_FIELD_FIELD_H

/// @file
/// The field's shared routines, globals and actor scripts.

#include "types.h"
#include "edition.h"
#include "actor.h"
#include "scene.h"
#include "scene_tables.h"
#include "menu_list.h"
#include "farcall.h"

struct Actor;
struct ScreenPoint;

/// An IWRAM word the scrolling camera's setup clears; nothing in the tree
/// reads it.
extern s32 gCamera_UnreadWord1;
/// The second IWRAM word the scrolling camera's setup clears; also unread.
/// With no reader to give them a role, the two are numbered in IWRAM order.
extern s32 gCamera_UnreadWord2;
extern const s32 gActor_DefaultScript[];
extern const u8 gWorldMap_BattleEnvironments[];
/// The size of Render_DecodePicture, copied to HEAP_SLOT_FRAME_SCRATCH; an
/// address equal to the size (constants.ld).
extern u8 Render_DecodePictureCodeSize[];
/// Four 256-byte colour tables (remap0 to remap3), indexed by pixel: the
/// SpriteColorMode tables after gSpriteColorTables' five, the
/// first also remapping the command icons (SpriteColor_RemapPixels).
extern const u8 gSpriteColorRemapTables[];

struct AbilityListEntry;
struct BattleAnimArgs;
struct Unit;
struct BurstParticle;
struct ScreenPoint;
struct Particle;
struct Actor;
struct FieldPosition;
struct MercuryLighthouseLights;
struct MapActor;
struct MapGridCell;
struct MenuSprite;
struct ModeRecord;
struct FieldRecord;
struct SceneEntrance;
struct MapEntry;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;
struct SparklePicture;
struct Swirl;
struct Actor;

/// The Colosso opponent's path player, at the start of gSceneLocalState: the
/// path in ColossoRaceWork is played back or recorded by
/// Colosso_UpdatePathOpponent.
struct PathRecorder {
  s16 mode;      ///< 1 plays the path, 2 records it, 9 finished
  s16 mirror;    ///< nonzero mirrors the path across the finish
  s16 actorId;   ///< the actor id moved along the path
  u16 pathIndex; ///< the next halfword of ColossoRaceWork.path, x then z
  /// Frames the actor has not moved; from 3 it stands
  /// (Actor_SelectAnimation 1).
  s16 still;
};
/// A Venus Lighthouse switch or pillar socket's map cell.
struct SwitchCell {
  u32 x; ///< 16-pixel map cells
  u32 z; ///< 16-pixel map cells
};
/// Frames left on the water palette's current step (gAltinPeak_PaletteScript).
extern s16 gAltinPeak_PaletteHold;
/// The next byte of gAltinPeak_PaletteScript; 0 again at its -1.
extern s16 gAltinPeak_PaletteStep;
/// One of a ledge jump's dust puffs (FieldEffect_SpawnBurstParticle):
/// SC_WAIT(1) looped 38 times, then SC_RELEASE.
extern const u8 gFieldEffect_BurstParticleScriptA[];
/// The other dust puff: the same, looped 18 times.
extern const u8 gFieldEffect_BurstParticleScriptB[];
extern u32 gWorldMap_GraphicsResources[][6];
/// SC_WAIT(40), then SC_RELEASE: the one-off attached effect that ends by
/// itself (FieldEffect_SetAttached, ATTACHED_EFFECT_ONCE).
extern const u8 gActor_WaitScript[];
/// The sparkles' 16 by 16 tiles, packed (FieldEffect_InitializeSparkles).
extern const u8 gFieldEffect_SparkleTiles[];
extern const u8 gField_ExamineTriggerMessageOffsets[];
/// The motes of light's 16 by 16 tiles, packed (FieldMotes_Start).
extern const u8 gFieldEffect_MoteTiles[];
extern s32 gDjinnFigures_SpriteIds[];
extern s32 gShipRooms_ShakeTimer;
/// The bend's water level in gSceneLocalState's second halfword: 1 high (the
/// logs float), 0 low; the floodgate switch flips it.
extern s16 *gKolimaForestFloodgateMap;
struct FloodgateLog;
/// The bend's logs in gSceneLocalState from byte 4, so they survive a battle.
extern struct FloodgateLog *gKolimaForestFloodgateRecords;
/// The short log's place in gSceneLocalState's first halfword: 1 west, 0 east.
extern u16 *gKolimaForestFloodgateReturned;
#ifndef OWN_gActors
/// Heap slot 5: the 64 actors, 0x70 bytes each.
extern struct Actor *gActors;
#endif
/// The world map camera's yaw the scanline parameters were built for; the
/// vertex update rebuilds them when it changes.
extern u32 gScanlineAngle;
#ifndef OWN_gScanlinePitch
/// The world map camera's pitch the scanline parameters were built for.
extern u32 gScanlinePitch;
#endif
/// The lighthouse transition's pictures, packed (tagged LZ), decoded 0x1000
/// bytes after the tiles (WorldMap_PlayLighthouseTransition).
extern const u8 gWorldMapPackedPictures[];
/// The lighthouse transition's tiles, packed (tagged LZ).
extern const u8 gWorldMapPackedTiles[];
/// The lighthouse transitions' palettes, 32 bytes each, loaded as BG palette
/// 14; indexed by enum WorldMapLighthouse.
extern const u8 gWorldMapPalettes[];

/// The lighthouse whose beacon the world map shows lit
/// (WorldMap_PlayLighthouseTransition), as gWorldMapPalettes orders their
/// colours.
enum WorldMapLighthouse {
  WORLD_MAP_LIGHTHOUSE_MERCURY,
  WORLD_MAP_LIGHTHOUSE_VENUS,
};
/// Which of the two guides' turns Kalay_AlternateGuidesPacing is at, 0 to 4.
extern s32 gKalay_DanceStep;
/// The percent chance that a line above the split shows the shaken scroll: 0
/// when the rumble starts (SE_WATER_LEVEL), then a point higher each frame up
/// to 101 (KolimaForestFloodgate_FlipFloodgateSwitch).
extern u32 gKolimaForestFloodgate_ShakeChance;
/// The background scroll words captured each frame for the shaken lines.
extern u32 gKolimaForestFloodgate_ShakeScroll[];
/// The healthy colours: the first 448 colours (896 bytes) of the colour
/// buffer, kept by KolimaPalette_SaveHealthy; the array is 1792 bytes.
/// SceneId_KolimaForestGlade and SceneId_KolimaForestCliff each define one.
extern u8 gKolimaPalette_Healthy[];
/// The cursed colours, kept by KolimaPalette_SaveCursed.
extern u8 gKolimaPalette_Cursed[];
/// Isaac's facing before Colosso_FadeInWarrior, which
/// Colosso_RestoreWarrior puts back.
extern s32 gColosso_WarriorStartAngle;
/// Isaac's x before he is shown on the stage, 16.16 fixed-point pixels.
extern s32 gColosso_WarriorStartX;
/// Isaac's z before he is shown on the stage, 16.16 fixed-point pixels.
extern s32 gColosso_WarriorStartZ;
/// The race gauge's 16 colours, loaded as OBJ palette 14 (Colosso_DrawGauge).
extern u16 gColosso_GaugePalette[];
/// The blinking pointer's sprite part: the queue link and attributes 0 to 2.
extern s32 gColosso_PointerOam[3];
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
/// LOG_ROOMS_A_CORNERED_FRAMES the chase raises its event.
extern u16 gLogRoomsA_StopTimer;
/// The size of Actor_UpdateAll's code, an address equal to the size
/// (constants.ld). Only the unused Actor_RunUpdateAllFromHeap reads it, to
/// copy the routine to the heap and run it once.
extern u8 Actor_UpdateAllCodeSize[];
extern u8 gActor_BlinkSpriteColors[];
// The actor motion scripts: movement types 1 to 7, which
// Actor_SetMovementType starts by number, and two turning scripts
// with no movement type number.
/// Movement type 1: a lone SC_END.
extern const u8 gMovementType_StandScript[];
/// Movement type 2: wanders within 16 pixels of home (SC_WANDER_HOME) at half a
/// pixel a frame.
extern const u8 gMovementType_WanderScript[];
/// Movement type 3: the same within 8 pixels of home.
extern const u8 gMovementType_WanderNearScript[];
/// Movement type 4: wanders within 6 pixels of home at 0.4 pixels a frame.
extern const u8 gMovementType_WanderCloseScript[];
/// Waits to arrive, then turns by Actor_StepAngle until it faces the
/// kept angle (Actor_LaunchFromLeader).
extern u8 gActor_LaunchScript;
/// Movement type 5: Actor_MoveTowardTarget every frame, forever.
extern const u8 gMovementType_FollowLinkedScript[];
/// Movement type 7: Actor_ClearChildAndCollisionFlags once, then SC_END.
extern const u8 gMovementType_ResetActionScript[];
/// Turns by Actor_StepAngle until it faces the kept angle, then SC_END.
extern const u8 gActor_StepAngleScript[];
/// Movement type 6: turns toward the leader every frame, forever.
extern const u8 gMovementType_FaceLeaderScript[];
extern const u8 gActor_VariantScripts[];
// Lamakan Desert's heat gauge (HeatGauge_Start and HeatGauge_Draw).
/// The gauge's tiles, packed (tagged LZ).
extern u8 gHeatGauge_Tiles[];
/// The gauge's 16 colours, loaded as OBJ palette 14.
extern const u16 gHeatGauge_Palette[16];
#ifndef OWN_HeatGauge_Held
/// 1 while the gauge is held on screen (HeatGauge_SetHeld).
extern u16 gHeatGauge_Held;
#endif
/// The gauge's decoded tiles, which each frame's drawing starts from.
extern u8 gHeatGauge_TileBuffer[];
#ifndef OWN_HeatGauge_VramSlot
/// The VRAM cache entry holding the gauge's tiles.
extern u16 gHeatGauge_VramSlot;
#endif
/// The gauge's five sprites: four 16x32 ones down the screen's left edge and a
/// 16x16 cap at its foot.
extern struct MenuSprite gHeatGauge_Sprites[];
#ifndef OWN_Render_DecodePicture
/// The sprite picture decoder, ARM code run from a copy in
/// HEAP_SLOT_FRAME_SCRATCH.
extern u8 Render_DecodePicture[];
#endif
/// The statue scene's three scrolling strips, packed (Decompress_ByteLz).
extern u8 gSolSanctumStatue_SpriteRowsGraphics[];
/// The six large things' sprite animations, which pick their footprints in
/// gPushedActor_FootprintBounds; each scene with them defines its own.
extern u32 gPushedActor_FootprintKinds[];
/// The tilemap row drawer, run from a copy in HEAP_SLOT_RAM_CODE_TEMP.
extern const u8 TileMap_DrawRows[];
/// The size of TileMap_DrawRows (decode.s).
extern u8 TileMap_DrawRowsCodeSize[];
/// The size of Transform_UpdateVertices, copied to heap slot 46; an address
/// equal to the size (constants.ld).
extern u8 Transform_UpdateVerticesSize;
extern const s32 gWorldMap_FootprintActions[6];
extern const s16 gWorldMap_KeyHeadings[16];
s32 FarCall_LuckyWheelsTable(s32 entrance);
s32 FarCall_LuckyDiceTable(s32 entrance);
#ifndef OWN_Map_PrepareActors
#endif
void WorldMap_StartStillView(void);
#ifndef OWN_Actor_Destroy
#endif

// The field effects' scripts and tables (overworld/fxdata.c, scripts.s and
// arcspark.s).
extern u16 gFieldEffect_CyclePatternWords[];
extern s8 gFieldEffect_RandomChildValues[];
/// The 32 by 16 spark of Avoid's falling arcs (spark_b), 4 bits a pixel;
/// FieldPsynergy_Avoid loads it.
extern const u8 gFieldEffect_ArcSparkTiles[];
extern const s32 gFieldEffect_BurstParticleActorScript[];
/// The emote bubble's script (FieldEffect_SpawnAttached): eight 2-frame steps
/// of squash and stretch (SC_ADD to SW_SCALE_X and SW_SCALE_Y), 30 frames
/// still, then SC_RELEASE.
extern const s32 gFieldEffect_LinkedActorScript[];
extern const s32 gFieldEffect_MarkerParticleScript[];
extern const s32 gFieldEffect_ParticleEmitterScript[];
extern const s32 gFieldEffect_ParticleScript[];
extern u32 gFieldEffect_PulseScales[];
extern const s32 gFieldEffect_SourceHoldScript[];
extern s16 gFieldEffect_TargetRangeByMode[];
/// A lone SC_RELEASE: an actor that has arrived releases itself
/// (FieldEffect_SetCallbackWhenTargetUnset).
extern const s32 gFieldEffect_UntargetedActorScript[];
/// The 32 by 16 spark a field Psynergy's particles draw (spark_a), 4
/// bits a pixel; FieldPsynergy_LoadResources loads it.
extern const u8 gFieldEffect_SparkTiles[];
extern const u8 gFieldEffect_QuadrantModes[];
/// The eight tints (PALETTE_TINT_CUSTOM and an RGB555 colour) a field
/// Psynergy fades the palettes toward, picked by the frame counter or
/// by the FLAG_PSYNERGY_TINT flags (FieldPsynergy_FadeToCastColor).
extern const s32 gFieldEffect_TargetColors[];
extern s16 gFieldEffect_GroundParticleFrames[];
/// The ground particles' 16 by 16 tiles, packed (Decompress_ByteLz).
extern const u8 gFieldEffect_GroundParticleTiles[];
extern const s32 gFieldEffect_RisingPulseScript[];

#endif
