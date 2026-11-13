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
#include "far_call.h"
#include "heap.h"

struct Actor;

/// An IWRAM word the scrolling camera's setup clears; nothing in the tree
/// reads it.
extern s32 gCamera_UnreadWord1;
/// The second IWRAM word the scrolling camera's setup clears; also unread.
/// With no reader to give them a role, the two are numbered in IWRAM order.
extern s32 gCamera_UnreadWord2;
extern const s32 gActor_DefaultScript[];
extern const u8 gWorldMap_BattleEnvironments[];
/// The size of Render_DecodePicture, copied to HEAP_SLOT_FRAME_SCRATCH; an
/// address equal to the size (ld_script.ld).
extern u8 Render_DecodePictureCodeSize[];
/// Four 256-byte colour tables (remap0 to remap3), indexed by pixel: the
/// SpriteColorMode tables after gSpriteColorTables' five, the
/// first also remapping the command icons (SpriteColor_RemapPixels).
extern const u8 gSpriteColorRemapTables[];

struct BattleEffectParams;
struct FieldPosition;
struct MenuSprite;
struct Trigger;
struct ActorPlacement;

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
/// One of a ledge jump's dust puffs (FieldEffect_SpawnDustPuff):
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
/// Heap slot 5: the 64 actors, 112 bytes each.
#define gActors (*(struct Actor **)&gHeapSlots[HEAP_SLOT_ACTORS])
/// The lighthouse transition's pictures, packed (tagged LZ), decoded 4 KB
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
/// The healthy colours: the first 448 colours (896 bytes) of the colour buffer,
/// kept by KolimaPalette_SaveHealthy; the array is 1792 bytes.
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
// Target.
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
/// (ld_script.ld).
extern u8 Actor_UpdateAllCodeSize[];
extern const u8 gActor_BlinkSpriteColors[];
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
extern const u8 gMovementType_FollowWatchedScript[];
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
/// 1 while the gauge is held on screen (HeatGauge_SetHeld).
extern u16 gHeatGauge_Held;
/// The gauge's decoded tiles, which each frame's drawing starts from.
extern u8 gHeatGauge_TileBuffer[];
/// The VRAM cache entry holding the gauge's tiles.
extern u16 gHeatGauge_VramSlot;
/// The gauge's five sprites: four 16x32 ones down the screen's left edge and a
/// 16x16 cap at its foot.
extern struct MenuSprite gHeatGauge_Sprites[];
/// The sprite picture decoder, ARM code run from a copy in
/// HEAP_SLOT_FRAME_SCRATCH.
extern u8 Render_DecodePicture[];
/// The statue scene's three scrolling strips, packed (Decompress_ByteLz).
extern u8 gSolSanctumStatue_SpriteRowsGraphics[];
/// The six large things' sprite animations, which pick their footprints in
/// gPushedActor_FootprintBounds; each scene with them defines its own.
extern u32 gPushedActor_FootprintKinds[];
/// The tilemap row drawer, run from a copy in HEAP_SLOT_RAM_CODE_TEMP.
extern const u8 TileMap_DrawRows[];
/// The size of TileMap_DrawRows (picture_decode.s).
extern u8 TileMap_DrawRowsCodeSize[];
/// The size of Transform_UpdateVertices, copied to heap slot 46; an address
/// equal to the size (ld_script.ld).
extern u8 Transform_UpdateVerticesSize;
extern const s32 gWorldMap_FootprintActions[6];
extern const s16 gWorldMap_KeyHeadings[16];
s32 FarCall_LuckyWheelsTable(s32 entrance);
s32 FarCall_LuckyDiceTable(s32 entrance);

// The field effects' scripts and tables (field/field_sequence_tables.c,
// scripts.s and spark_tiles.s).
extern const u16 gFieldEffect_CyclePatternWords[];
extern const s8 gFieldEffect_RandomChildValues[];
/// The 32 by 16 spark of Avoid's falling arcs (arc_spark.png), 4 bits a pixel;
/// FieldPsynergy_Avoid loads it.
extern const u8 gFieldEffect_ArcSparkTiles[];
extern const s32 gFieldEffect_BurstParticleActorScript[];
/// The emote bubble's script (Actor_ShowEmoteById): eight 2-frame steps
/// of squash and stretch (SC_ADD to SW_SCALE_X and SW_SCALE_Y), 30 frames
/// still, then SC_RELEASE.
extern const s32 gFieldEffect_WatchedActorScript[];
extern const s32 gFieldEffect_MarkerParticleScript[];
extern const s32 gFieldEffect_ParticleEmitterScript[];
extern const s32 gFieldEffect_ParticleScript[];
extern const u32 gFieldEffect_PulseScales[];
extern const s32 gFieldEffect_SourceHoldScript[];
extern const s16 gFieldEffect_TargetRangeByMode[];
/// A lone SC_RELEASE: an actor that has arrived releases itself
/// (FieldEffect_ReleaseOnArrival).
extern const s32 gFieldEffect_UntargetedActorScript[];
/// The 32 by 16 spark a field Psynergy's particles draw (spark.png), 4
/// bits a pixel; FieldPsynergy_LoadResources loads it.
extern const u8 gFieldEffect_SparkTiles[];
extern const u8 gFieldEffect_QuadrantModes[];
/// The eight tints (PALETTE_TINT_CUSTOM and an RGB555 colour) a field
/// Psynergy fades the palettes toward, picked by the frame counter or
/// by the FLAG_PSYNERGY_TINT flags (FieldPsynergy_FadeToCastColor).
extern const s32 gFieldEffect_TargetColors[];
extern const s16 gFieldEffect_GroundParticleFrames[];
/// The ground particles' 16 by 16 tiles, packed (Decompress_ByteLz).
extern const u8 gFieldEffect_GroundParticleTiles[];
extern const s32 gFieldEffect_RisingPulseScript[];

/// The first five 256-byte colour tables of SpriteColorMode, indexed by pixel.
extern const u8 gSpriteColorTables[][256];
/// The sprites' shadow, 16 by 8 at 256 colours, loaded into VRAM cache entry
/// 93.
extern const u8 gActor_ShadowTiles[];


#include "render_placement.h"

#include "types.h"

struct SpriteAnimation;
struct Sprite;
struct Actor;
struct FieldPosition;
struct WorldMapAffinePair;
struct WorldMapScanline;
struct ScriptOperands;
struct WorldPosition;

/// How the actor system runs (ActorSystem_Initialize): which camera follows
/// the actors and which update moves them.
enum ActorSystemMode {
  /// A town, a dungeon or the sprite viewer: the scrolling camera.
  ACTOR_SYSTEM_MAP = 2,
  /// The world map: the fixed camera, which projects 3D points onto the
  /// screen.
  ACTOR_SYSTEM_WORLD_MAP = 3,
  /// A battle: the fixed camera, and the battle's own motion update.
  ACTOR_SYSTEM_BATTLE = 4
};

/// SpriteAnimation.kind: the facing table a sprite animation's pictures follow
/// (Sprite_ComposePicture, facing.c).
enum AnimationFacingKind {
  /// The animation script's own picture whatever the facing; zero also marks a
  /// free
  /// sprite animation.
  ANIM_FACING_NONE = 0,
  ANIM_FACING_ONE_PICTURE = 1, ///< 8 sectors: one picture, mirrored to the left
  ANIM_FACING_FRONT_BACK = 2,  ///< 8 sectors: a front and a back picture
  ANIM_FACING_THREE_PICTURES = 3, ///< 16 sectors: front, side and back
  ANIM_FACING_FOUR_PICTURES = 4,  ///< 64 sectors
  ANIM_FACING_FIVE_PICTURES = 5,  ///< 16 sectors
  ANIM_FACING_SIX_PICTURES = 6,   ///< 64 sectors
  /// 8 sectors centred on the compass directions, a picture for each.
  ANIM_FACING_EIGHT_PICTURES = 8,
  /// ANIM_FACING_FRONT_BACK's table for a sprite drawn in two parts, the
  /// second the next sprite id (the sprite viewer).
  ANIM_FACING_TWO_PART = 20,
  /// As ANIM_FACING_FRONT_BACK, with the front picture mirrored the other
  /// way.
  ANIM_FACING_FRONT_BACK_SWAPPED = 22,
  /// Unused: no sprite's metadata or animation takes this kind. Ringed as eight
  /// facings by the sprite viewer, but with no facing table:
  /// Sprite_ComposePicture draws it as ANIM_FACING_NONE.
  ANIM_FACING_EIGHT_NO_TABLE = 44,
  /// As ANIM_FACING_EIGHT_PICTURES, with three of the pictures mirrored.
  ANIM_FACING_EIGHT_PICTURES_MIRRORED = 88
};

/// SpriteAnimation.mode, from the metadata's pictureCodec: how a sprite
/// animation's pictures are stored. Any other value is raw, for the picture
/// decoder.
enum SpritePictureCodec {
  SPRITE_PICTURE_RAW = 0,
  SPRITE_PICTURE_TAGGED_LZ = 1, ///< Decompress_TaggedLz's format
  SPRITE_PICTURE_LZ = 3         ///< LZ-compressed
};

/// Sprite.flags.
enum SpriteFlag {
  SPRITE_FLAG_SHADOW = 1 << 0, ///< its shadow is drawn; set when created
  /// Drawn as an outline, in ActorSystemWork's edge and fill colours.
  SPRITE_FLAG_OUTLINED = 1 << 1
};

/// Actor.spriteStorageKind, its low four bits: what the actor's sprite
/// pointer holds.
enum ActorSpriteKind {
  ACTOR_SPRITE_NONE = 0, ///< nothing drawn
  ACTOR_SPRITE_ONE = 1,
  ACTOR_SPRITE_GROUP = 2 ///< a list of up to four sprites, empty ones null
};

/// Actor.spriteStorageKind's low four bits, its enum ActorSpriteKind.
#define ACTOR_SPRITE_KIND_MASK 15

/// Actor_Create's id / 4096: what sprites the actor is made with.
enum ActorCreateKind {
  ACTOR_CREATE_ONE_SPRITE = 0, ///< the id's sprite (ACTOR_SPRITE_ONE)
  /// The sprites of the id and id + 1 (ACTOR_SPRITE_GROUP).
  ACTOR_CREATE_GROUP = 2,
  /// No sprite: any kind but the two above, and the one the camera actor
  /// takes.
  ACTOR_CREATE_NO_SPRITE = 8
};

/// The sprite id in Actor_Create's id, below its enum ActorCreateKind (id /
/// 4096).
#define ACTOR_CREATE_SPRITE_MASK 0xfff

/// The world map's collision values (WorldMap_GetCollision), 1 to 15; each
/// indexes a row of gWorldMap_BattleEnvironments.
enum WorldMapCollision {
  /// The party walks only on WORLD_MAP_COLLISION_WALKABLE_FIRST to
  /// WORLD_MAP_COLLISION_WALKABLE_LAST (WorldMap_IsWalkable).
  WORLD_MAP_COLLISION_WALKABLE_FIRST = 5,
  /// Shallow water: a ripple follows the leader as it walks.
  WORLD_MAP_COLLISION_SHALLOW_WATER = 6,
  /// Where neither collision layer has a value.
  WORLD_MAP_COLLISION_DEFAULT = 7,
  /// Forest: the leader's sprite shows its second layer and no shadow, and
  /// encounters take a region's forest zone.
  WORLD_MAP_COLLISION_FOREST = 9,
  WORLD_MAP_COLLISION_WALKABLE_LAST = 12
};

/// A world map battle environment (gWorldMap_BattleEnvironments,
/// WorldMap_GetBattleEnvironment): what its ground means for battles, named by
/// the battle background BattleBackground_SelectForEnvironment gives it.
enum BattleEnvironment {
  BATTLE_ENVIRONMENT_NONE = 0,      ///< Sol Sanctum's background
  BATTLE_ENVIRONMENT_GRASSLAND = 1, ///< ResourceId_GrasslandBackdrop
  /// ResourceId_ForestBackdrop; encounters take a region's forest zone.
  BATTLE_ENVIRONMENT_FOREST = 2,
  BATTLE_ENVIRONMENT_DESERT = 3,    ///< ResourceId_DesertBackdrop
  BATTLE_ENVIRONMENT_BEACH = 4,     ///< ResourceId_BeachBackdrop
  BATTLE_ENVIRONMENT_SNOWFIELD = 5, ///< ResourceId_SnowfieldBackdrop
  /// ResourceId_SnowfieldBackdrop, for the snowfield's forest.
  BATTLE_ENVIRONMENT_SNOW_FOREST = 6,
  /// ResourceId_BeachBackdrop, for shallow water
  /// (WORLD_MAP_COLLISION_SHALLOW_WATER).
  BATTLE_ENVIRONMENT_SHALLOWS = 7
};

/// A world map cell's encounter region (bits 30..24 of its block word), which
/// picks its encounter zones (gWorldMap_EncounterRules).
enum WorldMapEncounterRegion {
  /// Takes the second world map graphics set (WorldMap_LoadGraphics) and the
  /// snowfield's battle environments (WorldMap_GetBattleEnvironment).
  WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD = 21
};

/// A map cell's terrain kind: the low four bits of its shape's first byte,
/// which pick its height function from gMapGrid_TerrainHeightFunctions.
enum TerrainKind {
  TERRAIN_FLAT = 0,    ///< the first sample everywhere
  TERRAIN_SLOPE_X = 1, ///< from the first sample at x 0 towards the second
  TERRAIN_SLOPE_Y = 2, ///< from the first sample at y 0 towards the second
  /// A step across the diagonal x + y = 15, the higher sample on it.
  TERRAIN_STEP_SUM_DIAGONAL = 3,
  /// A step across the diagonal x = y, the higher sample on it.
  TERRAIN_STEP_DIFFERENCE_DIAGONAL = 4,
  /// Three samples, bending on the diagonal x + y = 15.
  TERRAIN_BEND_SUM_DIAGONAL = 5,
  /// Three samples, bending on the diagonal x = y.
  TERRAIN_BEND_DIFFERENCE_DIAGONAL = 6,
  /// A raised or sunken rounded square (gTerrain_LerpWeightTable).
  TERRAIN_ROUNDED_SQUARE = 7,
  TERRAIN_STEP_X = 8,  ///< the first sample for x below 8, then the second
  TERRAIN_STEP_Y = 9,  ///< the first sample for y below 8, then the second
  TERRAIN_BEND_X = 10, ///< three samples along x, bending at the middle
  TERRAIN_BEND_Y = 11, ///< three samples along y, read unsigned
  /// The sample gTerrain_SampleIndexTable picks for each pixel.
  TERRAIN_SAMPLE_TABLE = 12,
  /// As TERRAIN_SAMPLE_TABLE, mirrored left to right.
  TERRAIN_SAMPLE_TABLE_MIRRORED = 13,
  TERRAIN_FLAT_B = 14,
  TERRAIN_FLAT_C = 15,
  TERRAIN_KIND_COUNT = 16
};

/// What MapGrid_CheckCollision says of a step to a position.
enum Collision {
  COLLISION_TOO_LOW = -1,  ///< the ground there is more than 12 pixels lower
  COLLISION_NONE = 0,      ///< the actor can step there
  COLLISION_TOO_HIGH = 1,  ///< the ground there is more than 8 pixels higher
  COLLISION_IMPASSABLE = 2 ///< the cell there is a wall (CELL_BEHAVIOR_WALL)
};

u8 *Decompress_Lz(const u8 *source, u8 *destination);
s32 Sprite_ComposePicture(struct Sprite *animation, u16 dir);
void Render_PlaceSpritePartPair(struct Sprite *sprite, s32 screenX, s32 height,
                                s32 screenY, s32 groundHeight, s32 *scale);
void Render_ApplyProjectedPlacement(struct Sprite *sprite,
                                    const struct SpritePlacement *position,
                                    const struct SpriteScale *scale,
                                    s32 facing);
void Render_PlaceProjectedSprite(struct Sprite *sprite, s32 *point, s32 *scale,
                                 s32 facing, s32 depth);
void Sprite_SetAnimationColorModes(void *rawSprite, u32 value);
s32 SpritePictureCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant);
s32 SpritePictureCache_FindByTag(s32 tag);
s32 Sprite_ResetAnimations(struct Sprite *sprite);
void SpriteAnimation_Reset(struct SpriteAnimation *layer);
s32 Sprite_AddAnimation(struct Sprite *sprite, s32 id);
void Sprite_RemoveAnimation(struct Sprite *sprite, s32 layer);
void Sprite_RemoveAnimationAt(struct Sprite *sprite, u32 index);
void SpriteAnimation_Start(struct SpriteAnimation *layer, s32 animation);
s32 Sprite_StartAnimation(struct Sprite *sprite, s32 animation);
s32 Sprite_SetAnimationTimers(struct Sprite *sprite, s32 frames);
void Sprite_SetAnimationSpeed(struct Sprite *sprite, s32 step);
void ActorSystem_Configure(s32 mode);
struct SpriteAnimation *SpriteAnimation_Alloc(s32 id);
void SpriteAnimation_Clear(struct SpriteAnimation *record);
struct Sprite *Sprite_Create(s32 id);
void Sprite_Destroy(struct Sprite *sprite);
s32 SpriteAnimation_SumCommandLengths(s32 id, u32 animation, s32 frameCount);
void SpriteDissolve_Step(u8 *obj, u32 pos);
void SpriteDissolve_Run(u8 *sprite);
void SpriteDissolve_RunMany(u8 **sprites, s32 count);
void Actor_SelectAnimation(void *raw, s32 no);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 val);
s32 ActorScript_SetFlagAndTest(struct Actor *interpreter);
s32 ActorScript_ClearFlagAndTest(struct Actor *interpreter);
// The work values of SC_SET, SC_ADD and SC_CMP, one handler each
// (gActorScript_OperandHandlerTable, field/field/script_commands.c): by
// operation, each sets one of the actor's fields to value, adds value to it, or
// compares the two into conditionResult.
void ActorScript_SetOrCompareScriptAddress(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ActorScript_SetOrCompareFacing(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                               s32 value);
void ActorScript_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                               s32 value);
void ActorScript_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                               s32 value);
void ActorScript_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareVelocityX(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareVelocityY(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareVelocityZ(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareSpeedLimit(struct ScriptOperands *state,
                                        s32 operation, s32 value);
void ActorScript_SetOrCompareAcceleration(struct ScriptOperands *state,
                                          s32 operation, s32 value);
void ActorScript_SetOrCompareTargetX(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareTargetY(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareTargetZ(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareVerticalMotion(struct ScriptOperands *state,
                                            s32 operation, s32 value);
void ActorScript_SetOrCompareGravity(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareTerrainHeight(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ActorScript_SetOrCompareRiseBaseY(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareSprite(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareSpriteStorageKind(struct ScriptOperands *state,
                                               s32 operation, s32 value);
void ActorScript_SetOrCompareMotionFlags(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareArrivalAxis(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareComparisonResult(struct ScriptOperands *state,
                                              s32 operation, s32 value);
void ActorScript_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                          s32 operation, s32 value);
void ActorScript_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                            s32 operation, s32 value);
void ActorScript_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ActorScript_SetOrComparePaused(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareRepeatCount(struct ScriptOperands *work,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareTimer(struct ScriptOperands *work, s32 operation,
                                   s32 value);
void ActorScript_SetOrCompareUserValue(struct ScriptOperands *work,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareWatchedActor(struct ScriptOperands *work,
                                          s32 operation, s32 value);
void ActorScript_SetOrCompareUpdateAddress(struct ScriptOperands *work,
                                           s32 operation, s32 value);
void ActorScript_SetOrCompareRiseCounter(struct ScriptOperands *work,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareRiseEnabled(struct ScriptOperands *work,
                                         s32 operation, s32 value);
void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value);
void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value);

s32 MapGrid_GetScreenRelativePosition(struct Actor *actor, s32 *screenPosition);
void ActorSystem_Initialize(s32 controlMode);
void ActorSystem_ReservedNoOpA(void);
void ActorSystem_ReservedNoOpB(void);
void *Actor_FindFree(void);
void Actor_Release(struct Actor *actor);
struct Actor *Actor_Create(s32 encodedSprite, s32 x, s32 y, s32 z);
void Actor_StartScript(struct Actor *actor, const s32 *script);
void Actor_SetAnimationAndSpeed(struct Actor *actor, s32 animation, s32 step);
void Actor_SetSpriteRotation(struct Actor *actor, u32 rotation);
void Actor_SetFirstSpriteAnimation(void *rawDispatchActor, s32 animationId);
void Actor_RegisterSpriteMetadata(struct Actor *actor, s32 animationId);
void Actor_StartApproachWatched(struct Actor *actor, s32 watchedActorHandle);
void Actor_StartKeyCheck(struct Actor *actor);
void Actor_StartMoveByKeys(struct Actor *actor);
void Actor_StartPlayerControl(struct Actor *actor);
void Actor_StartWorldMapControl(struct Actor *actor);
void Actor_StartIdle(struct Actor *actor);
void Actor_StartCamera(struct Actor *actor, s32 watchedActorHandle);
void Actor_WaitForScriptEnd(void *rawActor);
void Actor_SetSpriteFlags(struct Actor *actor, u32 flags);
void Actor_SetBlendMode(struct Actor *actor, u32 mode);
void Actor_SetSpriteScale(struct Actor *actor, u32 fixedScale);
void Actor_SetSpriteColorMode(struct Actor *actor, u32 param);
void ActorSystem_SuspendProjectionAndShowSprites(void);
void ActorSystem_ResumeProjectionAndHideLayers(void);
s32 Actor_ReturnTrue(void);
void ActorSystem_PlaceSprites(void);
s32 ActorCamera_ReturnTrue(void);
void ActorSystem_ProjectSprites(void);

s32 ActorScript_SetBase(struct Actor *interpreter);
s32 ActorScript_HideSprites(struct Actor *interpreter);
s32 ActorScript_ShowSprite(struct Actor *interpreter);
void Actor_WaitForMove(void *actor);
s32 Actor_IsTargetUnset(struct Actor *actor);
void Actor_UpdateAllThumb(void);
void Actor_SetPositionAndResetMotion(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_RunUpdateAllFromHeap(void);
void Actor_UpdateAllMotion(void);
s32 ActorScript_Wait(struct Actor *interpreter);
s32 ActorScript_WaitForArrival(struct Actor *interpreter);
s32 ActorScript_InvokeCallback(struct Actor *interpreter);
s32 ActorScript_FindLabel(struct Actor *interpreter, u32 labelKey);
s32 ActorScript_RepeatOrJump(struct Actor *interpreter);
s32 ActorScript_JumpToLabel(struct Actor *interpreter);
s32 ActorScript_JumpIfTrue(struct Actor *interpreter);
s32 ActorScript_JumpIfFalse(struct Actor *interpreter);
s32 ActorScript_End(struct Actor *interpreter);
s32 ActorScript_TestFlag(struct Actor *interpreter);
s32 ActorScript_ToggleFlagAndTest(struct Actor *interpreter);
s32 ActorScript_SetAnimation(struct Actor *interpreter);
s32 ActorScript_Release(struct Actor *interpreter);
s32 ActorScript_Nop(struct Actor *interpreter);
s32 ActorScript_PlaySound(struct Actor *interpreter);
s32 ActorScript_CheckOverlap(struct Actor *actor, s32 *values);
struct Actor *ActorScript_FindOverlappingEntry(struct Actor *actor,
                                               s32 *values);
s32 ActorScript_SetPositionAndResetMotion(struct Actor *interpreter);
s32 ActorScript_ApplyAbsolutePosition(struct Actor *interpreter);
s32 ActorScript_ApplyRelativePosition(struct Actor *interpreter);
s32 ActorScript_StoreAngleToWatchedActor(struct Actor *interpreter);
s32 ActorScript_ApplyWatchedActorPosition(struct Actor *interpreter);
s32 ActorScript_ApplyLocalOffsetPosition(struct Actor *interpreter);
s32 FieldCamera_FollowLeaderInBounds(struct Actor *interpreter);
s32 Actor_ApproachWatchedActor(struct Actor *interpreter);
s32 Actor_Wander(struct Actor *interpreter);
s32 ActorScript_WanderNearHome(struct Actor *interpreter);

void ActorScript_SetOrCompareCursor(struct ScriptOperands *operands,
                                    s32 operation, s32 value);
void ActorScript_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareWait(struct ScriptOperands *work, s32 operation,
                                  s32 value);
s32 ActorScript_ApplyOperandSet(struct Actor *interpreter);
s32 ActorScript_ApplyOperandAdd(struct Actor *interpreter);
s32 ActorScript_ApplyOperandCompare(struct Actor *interpreter);

void Actor_SetUpdateValue(void *rawActor, s32 value);
u32 Field_RequestShortcut(u32 encodedAssignment);
s32 Field_CheckConfiguredKeys(void);
s32 Field_CheckSpheresOverlap(s32 *firstPosition, s32 radiusA,
                              s32 *secondPosition, s32 radiusB);
s32 Actor_UpdatePlayerControl(struct Actor *interpreter);
s32 Actor_MoveOnWorldMap(struct Actor *interpreter);
s32 Field_CheckConfiguredKeysAndCount(struct Actor *interpreter);
s32 Actor_MoveByKeys(struct Actor *interpreter);

void SpriteColor_RemapPixels(u8 *bytes, s32 byteCount);
void TileMap_DecodeStagedBuffer(s32 size);
void TileMap_NumberPlaceholderCellPictures(void);
void TileMap_ConvertBuffer(void);
s32 MapGrid_LoadMap(s32 index);
void MapGrid_CenterOnFollowedPoint(void);
void MapGrid_RenderCellPictureRow(u32 layer, s32 tileX, s32 tileY);
void MapGrid_RenderCellPictureColumn(u32 layer, s32 tileX, s32 tileY);
void MapGrid_UpdateLayerScroll(void);
void MapGrid_SetCameraCenter(s32 x, s32 y);
void MapGrid_CopyCellPictureIndicesRect(s32 sourceX, s32 sourceY, u32 dstX,
                                        u32 dstY, u32 width, u32 height);
void MapGrid_PlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY);
void MapGrid_CopyCellPictureCellsRect(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 dstX, s32 dstY);
void MapGrid_CopyCellAttributeRect(s32 sourceX, s32 sourceY, s32 width,
                                   s32 height, s32 dstX, s32 dstY);
void MapGrid_CopyCellIndexRect(s32 sourceX, s32 sourceY, s32 width, s32 height,
                               s32 dstX, s32 dstY);
void MapGrid_SetShownLayers(u32 layerFlags);

s32 WorldMap_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
UNREAD_RESULT WorldMap_InitializePerspective(void);
void WorldMap_SetWindowCellTile(s32 x, s32 y, s32 cellX, s32 cellY);
void WorldMap_LoadGraphics(s32 x, s32 z);
void WorldMap_ApplyAffinePage(void);
void WorldMap_RenderPaletteMappedRow(u32 tileRow);
void WorldMap_RenderPaletteMappedColumn(u32 tileColumn);
void WorldMap_UpdateView(void);
void WorldMap_UpdateCurrentTileBlock(void);
void WorldMap_PreloadCurrentTileBlock(void);
void WorldMap_PresentStillPicture(void);
void WorldMap_StartStillPicture(void);
void WorldMap_ShowBg1FromBuffer(void);
void WorldMap_LoadAreaGraphics(void);
void WorldMap_LoadDefaultCellsAndUpdateBlock(void);
void TilesetAnim_Update(void);
void TilesetAnim_Resume(u32 channelIndex);
void TilesetAnim_Pause(u32 channelIndex);
void TilesetAnim_StartAll(const u16 *script);
void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void BlendScript_Run(void);
void BlendScript_Start(u16 *script);
void BlendScript_Suspend(void);
void BlendScript_Resume(void);

void PaletteCycle_Initialize(void);
s32 PaletteCycle_Add(s16 paletteNumber, s16 firstColor, s16 delay,
                     s16 colorCount);
void PaletteCycle_Stop(void);
void PaletteCycle_Start(void);
void PaletteCycle_Step(void);

s32 Terrain_GetFlatHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetSlopeXHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetSlopeYHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetStepSumDiagonalHeight(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_GetStepDifferenceDiagonalHeight(const u8 *heightBytes, s32 x,
                                            s32 y);
s32 Terrain_GetBendSumDiagonalHeight(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_GetBendDifferenceDiagonalHeight(const u8 *heightBytes, s32 x,
                                            s32 y);
s32 Terrain_GetRoundedSquareHeight(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_GetStepXHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetStepYHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetBendXHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetBendYHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetSampleTableHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetSampleTableMirroredHeight(const u8 *heightBytes, s32 pixelX,
                                         s32 pixelY);
s32 Terrain_GetFlatBHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetFlatCHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);

s32 MapGrid_GetTerrainHeight(s32 layer, s32 x, s32 y);
s32 MapGrid_GetCellAttributeLowNibble(s32 layer, s32 x, s32 y);
s32 MapGrid_GetCellBehaviorAt(s32 layer, s32 x, s32 y);
void MapGrid_SetCellBehaviorAt(u32 layer, s32 x, s32 y, u32 behavior);
s32 MapGrid_GetCellHighFlags(s32 x, s32 y);
s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);
s32 MapGrid_IsWallAt(struct WorldPosition *position);
s32 WorldMap_GetCollision(struct WorldPosition *position);
s32 WorldMap_IsWalkable(s32 unused, struct WorldPosition *position);
s32 WorldMap_GetBattleEnvironment(struct WorldPosition *position,
                                  s32 *terrainKind);
/// A screen shake's component left as it is; any negative value does.
#define SHAKE_KEEP (-1)
void MapGrid_SetScreenShake(s32 shakeX, s32 shakeY, s32 shakeDecay);
void MapGrid_WaitForShakeToSettle(void);
void TileMap_DrawRowsFromRam(const struct WorldMapAffinePair *lines,
                             u8 *pixels);
void WorldMap_BuildScanlineTable(s32 depth, s32 *position,
                                 struct WorldMapScanline *scanline);

void SpriteViewer_Run(void);
s32 SpriteViewer_FindNextSpriteWithPictures(s32 spriteId, s32 direction);
void SpriteViewer_BuildProbeRing(s32 centerX, s32 centerY,
                                 struct SpritePlacement *positions);
void SpriteViewer_StartStaggeredAnimations(s32 layerIndex, s32 animationIndex);
void SpriteViewer_SetGridColumnNumber(s32 layerIndex, s32 spriteId);
void SpriteViewer_PlaceRing(void);


#endif
