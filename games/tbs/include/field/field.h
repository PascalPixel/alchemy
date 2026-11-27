#ifndef ALCHEMY_FIELD_H
#define ALCHEMY_FIELD_H

/* @file */
/* The field engine's shared functions, globals and actor scripts. */

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "field/actor.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "stubs.h"
#include "system/globals.h"
#include "window/menu_list.h"

struct ACTOR;

/* An IWRAM word the scrolling camera's setup clears; nothing in the tree */
/* reads it. */
extern s32 Camera_UnreadWordA;
/* The second IWRAM word the scrolling camera's setup clears; also unread. */
/* With no reader to give them a role, the two are numbered in IWRAM order. */
extern s32 Camera_UnreadWordB;
extern const s32 Actor_DefaultScript[];
extern const u8 WorldMap_BattleEnvironments[];
/* The size of Render_DecodePicture, copied to HEAP_SLOT_FRAME_SCRATCH; an */
/* address equal to the size (ld_script.ld). */
extern u8 Render_DecodePictureCodeSize[];
/* Four 256-byte colour tables (remap0 to remap3), indexed by pixel: the */
/* SPR_COL_MODE tables after SpriteColorTables' five, the */
/* first also remapping the command icons (fdSprColRemapPixels). */
extern const u8 SpriteColorGreyTable[];

struct BATTLE_EFFECT_PARM;
struct FIELD_POS;
struct MENU_SPR;
struct TRIGGER;
struct ACTOR_PLACE;

/* The Colosso opponent's path player, at the start of SceneLocalState: the */
/* path in COLOSSO_RACE_STATE is played back or recorded by */
/* scColossoUpdatePathOpponent. */
typedef struct PATH_RECORDER {
	s16 mode;	 /* a PATH_RECORDER_MODE (scene/colosso_setup.h) */
	s16 mirror;	 /* nonzero mirrors the path across the finish */
	s16 actorId; /* the actor id moved along the path */
	u16 pathIdx; /* the next halfword of COLOSSO_RACE_STATE.path, x then z */
	/* Frames the actor has not moved; from 3 it stands */
	/* (fdActorSelectAnimation 1). */
	s16 still;
} PATH_RECORDER;
/* A Venus Lighthouse switch or pillar socket's map cell. */
typedef struct SWITCH_CELL {
	u32 x; /* 16-pixel map cells */
	u32 z; /* 16-pixel map cells */
} SWITCH_CELL;
/* One of a ledge jump's dust puffs (fdEffectSpawnDustPuff): */
/* SC_WAIT(1) looped 38 times, then SC_RELEASE. */
extern const s32 FieldEffect_BurstParticleScriptA[];
/* The other dust puff: the same, looped 18 times. */
extern const s32 FieldEffect_BurstParticleScriptB[];
extern u32 WorldMap_GraphicsResources[][6];
/* SC_WAIT(40), then SC_RELEASE: the one-off attached effect that ends by */
/* itself (fdEffectSetAttached, ATTACHED_EFFECT_ONCE). */
extern const s32 Actor_WaitScript[];
/* The sparkles' 16 by 16 tiles, packed (fdEffectInitSparkles). */
extern const u8 FieldEffect_SparkleTiles[];
extern const u8 Field_ExamineTriggerMessageOffsets[];
/* The motes of light's 16 by 16 tiles, packed (fdMotesStart). */
extern const u8 FieldEffect_MoteTiles[];
extern s32 DjinnFigures_SpriteIds[];
extern s32 ShipRooms_ShakeTimer;
struct FLOODGATE_LOG;
/* The ACTOR_COUNT actors (HEAP_SLOT_ACTORS), 112 bytes each. */
#define ActorsWork (*(ACTOR **)&HeapSlots[HEAP_SLOT_ACTORS])
/* The lighthouse transition's pictures, packed (tagged LZ), decoded 4 KB */
/* after the tiles (scWorldMapPlayLighthouseTransition). */
extern const u8 WorldMapPackedPictures[];
/* The lighthouse transition's tiles, packed (tagged LZ). */
extern const u8 WorldMapPackedTiles[];
/* The lighthouse transitions' palettes, 32 bytes each, loaded as BG palette */
/* 14; indexed by WORLD_MAP_LIGHTHOUSE. */
extern const u8 WorldMapPalettes[];

/* The lighthouse whose beacon the world map shows lit */
/* (scWorldMapPlayLighthouseTransition), as WorldMapPalettes orders their */
/* colours. */
typedef enum WORLD_MAP_LIGHTHOUSE {
	WORLD_MAP_LIGHTHOUSE_MERCURY,
	WORLD_MAP_LIGHTHOUSE_VENUS,
} WORLD_MAP_LIGHTHOUSE;
/* The race gauge's 16 colours, loaded as OBJ palette 14 (scColossoDrawGauge).
 */
extern u16 Colosso_GaugePalette[];
/* Frames in a row the chase has found a suspect cornered; at */
/* LOG_ROOMS_A_CORNERED_FRAMES the chase raises its event. */
extern u16 LogRoomsA_CorneredFrames;
/* The size of fdActorUpdateAll's code, an address equal to the size */
/* (ld_script.ld). */
extern u8 fdActorUpdateAllCodeSize[];
extern const u8 Actor_BlinkSpriteColors[];
/* The actor motion scripts: movement types 1 to 7, which */
/* fdActorSetMovementType starts by number, and two turning scripts */
/* with no movement type number. */
/* Movement type 1: a lone SC_END. */
extern const s32 MovementType_StandScript[];
/* Movement type 2: wanders within 16 pixels of home (SC_WANDER_HOME) at half a
 */
/* pixel a frame. */
extern const s32 MovementType_WanderScript[];
/* Movement type 3: the same within 8 pixels of home. */
extern const s32 MovementType_WanderNearScript[];
/* Movement type 4: wanders within 6 pixels of home at 0.4 pixels a frame. */
extern const s32 MovementType_WanderCloseScript[];
/* Waits to arrive, then turns by fdActorStepAng until it faces the */
/* kept angle (fdActorLaunchFromLeader). */
extern const s32 Actor_LaunchScript[];
/* Movement type 5: fdActorMoveTowardTarget every frame, forever. */
extern const s32 MovementType_FollowWatchedScript[];
/* Movement type 7: fdActorClearSprAndCollisionFlags once, then SC_END. */
extern const s32 MovementType_ResetActionScript[];
/* Turns by fdActorStepAng until it faces the kept angle, then SC_END. */
extern const s32 Actor_StepAngleScript[];
/* Movement type 6: turns toward the leader every frame, forever. */
extern const s32 MovementType_FaceLeaderScript[];
extern const s32 Actor_VariantScripts[];
/* Lamakan Desert's heat gauge (scHeatGaugeStart and scHeatGaugeDraw). */
/* The gauge's tiles, packed (tagged LZ). */
extern u8 HeatGauge_Tiles[];
/* The gauge's 16 colours, loaded as OBJ palette 14. */
extern const u16 HeatGauge_Palette[16];
/* The sprite picture decoder, ARM code run from a copy in */
/* HEAP_SLOT_FRAME_SCRATCH. */
extern u8 Render_DecodePicture[];
/* The statue scene's three scrolling strips, packed (fdDecompressByteLz). */
extern u8 SolSanctumStatue_SpriteRowsGraphics[];
/* The six large things' sprite animations, which pick their rectangles in */
/* PushedActor_Rectangles; each scene with them defines its own. */
extern u32 PushedActor_RectangleKinds[];
/* The tilemap row drawer, run from a copy in HEAP_SLOT_RAM_CODE_TEMP. */
extern const u8 fdTileMapDrawRows[];
/* The size of fdTileMapDrawRows (picture_decode.s). */
extern u8 fdTileMapDrawRowsCodeSize[];
/* The size of fdWorldMapUpdateAffineLines, copied to HEAP_SLOT_RAM_CODE; an */
/* address equal to the size (ld_script.ld). */
extern u8 fdWorldMapUpdateAffineLinesCodeSize;
extern const s32 WorldMap_FootprintActions[6];
/* The direction pad's headings, defined as u16 in facing.c. Declared here */
/* with the sign, so Up's three quarters of a turn reads negative; the */
/* player's steps keep each one as a u16 again, the same heading. */
extern const s16 WorldMap_KeyHeadings[16];

/* The field effects' scripts and tables (field_sequence_tables.c, */
/* movement_scripts.c, field_effect_tables.c and */
/* graphics/field_effects/spark_tiles.s). */
extern const u16 Input_DpadAngles[];
extern const s8 Halt_FlickerColorModes[];
/* The 32 by 16 spark of Avoid's falling arcs (arc_spark.png), 4 bits a pixel;
 */
/* fdPsynergyAvoid loads it. */
extern const u8 FieldEffect_ArcSparkTiles[];
extern const s32 Growth_LeafScript[];
/* Waits to arrive, sets off fdEffectSpawnHeavyImpact and releases the */
/* actor: Force's fist as it lands. */
extern const s32 Force_FistScript[];
/* The emote bubble's script (fdActorShowEmoteById): eight 2-frame steps */
/* of squash and stretch (SC_ADD to SW_SCALE_X and SW_SCALE_Y), 30 frames */
/* still, then SC_RELEASE. */
extern const s32 Emote_BubbleScript[];
extern const s32 Reveal_MarkerScript[];
extern const s32 ItemDisplay_GrowInScript[];
extern const s32 FieldEffect_ParticleScript[];
extern const u32 FieldEffect_PulseScales[];
extern const s32 FieldEffect_SourceHoldScript[];
extern const s16 FieldEffect_TargetRangeByMode[];
/* A lone SC_RELEASE: an actor that has arrived releases itself */
/* (fdMindReadUpdateCrescent). */
extern const s32 FieldEffect_UntargetedActorScript[];
/* The 32 by 16 spark a field Psynergy's particles draw (spark.png), 4 */
/* bits a pixel; fdPsynergyLoadResources loads it. */
extern const u8 FieldEffect_SparkTiles[];
extern const u8 FieldEffect_QuadrantModes[];
/* The eight tints (PALETTE_TINT_CUSTOM and an RGB555 colour) a field */
/* Psynergy fades the palettes toward, picked by the frame counter or */
/* by the FLAG_PSYNERGY_TINT flags (fdPsynergyFadeToCastCol). */
extern const s32 FieldPsynergy_CastColors[];
extern const s16 Weather_RainSplashFrames[];
/* The ground particles' 16 by 16 tiles, packed (fdDecompressByteLz). */
extern const u8 Weather_ParticleTiles[];
extern const s32 ItemDisplay_PulseScript[];

/* The first five 256-byte colour tables of SPR_COL_MODE, indexed by pixel.
 */
extern const u8 SpriteColorTables[][256];
/* The sprites' shadow, 16 by 8 at 256 colours, loaded into VRAM cache entry */
/* 93. */
extern const u8 Actor_ShadowTiles[];

#include "graphics/render_placement.h"

struct SPR_ANIMATION;
struct SPR;
struct ACTOR;
struct FIELD_POS;
struct WORLD_MAP_AFFINE_PAIR;
struct WORLD_MAP_SCANLINE;
struct SCRIPT_OPERANDS;
struct WORLD_POS;

/* How the actor system runs (fdActorSystemInit): which camera follows */
/* the actors and which update moves them. */
typedef enum ACTOR_SYSTEM_MODE {
	/* A town, a dungeon or the sprite viewer: the scrolling camera. */
	ACTOR_SYSTEM_MAP = 2,
	/* The world map: the fixed camera, which projects 3D points onto the */
	/* screen. */
	ACTOR_SYSTEM_WORLD_MAP = 3,
	/* A battle: the fixed camera, and the battle's own motion update. */
	ACTOR_SYSTEM_BATTLE = 4
} ACTOR_SYSTEM_MODE;

/* SPR_ANIMATION.kind: the facing table a sprite animation's pictures follow
 */
/* (fdSprComposePicture, facing.c). */
typedef enum ANIMATION_FACING_KIND {
	/* The animation script's own picture whatever the facing; zero also marks a
	 */
	/* free sprite animation. */
	ANIM_FACING_NONE = 0,
	ANIM_FACING_ONE_PICTURE =
		1, /* 8 sectors: one picture, mirrored to the left */
	ANIM_FACING_FRONT_BACK = 2,		/* 8 sectors: a front and a back picture */
	ANIM_FACING_THREE_PICTURES = 3, /* 16 sectors: front, side and back */
	ANIM_FACING_FOUR_PICTURES = 4,	/* 64 sectors */
	ANIM_FACING_FIVE_PICTURES = 5,	/* 16 sectors */
	ANIM_FACING_SIX_PICTURES = 6,	/* 64 sectors */
	/* 8 sectors centred on the compass directions, a picture for each. */
	ANIM_FACING_EIGHT_PICTURES = 8,
	/* ANIM_FACING_FRONT_BACK's table for a sprite drawn in two parts, the */
	/* second the next sprite id (the sprite viewer). */
	ANIM_FACING_TWO_PART = 20,
	/* As ANIM_FACING_FRONT_BACK, with the front picture mirrored the other */
	/* way. */
	ANIM_FACING_FRONT_BACK_SWAPPED = 22,
	/* Unused: no sprite's metadata or animation takes this kind. Ringed as
	   eight */
	/* facings by the sprite viewer, but with no facing table: */
	/* fdSprComposePicture draws it as ANIM_FACING_NONE. */
	ANIM_FACING_EIGHT_NO_TABLE = 44,
	/* As ANIM_FACING_EIGHT_PICTURES, with three of the pictures mirrored. */
	ANIM_FACING_EIGHT_PICTURES_MIRRORED = 88
} ANIMATION_FACING_KIND;

/* SPR_ANIMATION.mode, from the metadata's pictureCodec: how a sprite */
/* animation's pictures are stored. Any other value is raw, for the picture */
/* decoder. */
typedef enum SPR_PICTURE_CODEC {
	SPRITE_PICTURE_RAW = 0,
	SPRITE_PICTURE_TAGGED_LZ = 1, /* fdDecompressTaggedLz's format */
	SPRITE_PICTURE_LZ = 3		  /* LZ-compressed */
} SPR_PICTURE_CODEC;

/* SPR.flg. */
typedef enum SPR_FLAG {
	SPRITE_FLAG_SHADOW = 1 << 0, /* its shadow is drawn; set when created */
	/* Drawn as an outline, in ACTOR_SYSTEM_STATE's edge and fill colours. */
	SPRITE_FLAG_OUTLINED = 1 << 1
} SPR_FLAG;

/* ACTOR.sprStorageKind, its low four bits: what the actor's sprite */
/* pointer holds. */
typedef enum ACTOR_SPR_KIND {
	ACTOR_SPRITE_NONE = 0, /* nothing drawn */
	ACTOR_SPRITE_ONE = 1,
	ACTOR_SPRITE_GROUP = 2 /* a list of up to four sprites, empty ones null */
} ACTOR_SPR_KIND;

/* ACTOR.sprStorageKind's low four bits, its ACTOR_SPR_KIND. */
#define ACTOR_SPRITE_KIND_MASK 15

/* fdActorCreate's id / 4096: what sprites the actor is made with. */
typedef enum ACTOR_CREATE {
	ACTOR_CREATE_ONE_SPRITE = 0, /* the id's sprite (ACTOR_SPRITE_ONE) */
	/* The sprites of the id and id + 1 (ACTOR_SPRITE_GROUP). */
	ACTOR_CREATE_GROUP = 2,
	/* No sprite: any kind but the two above, and the one the camera actor */
	/* takes. */
	ACTOR_CREATE_NO_SPRITE = 8
} ACTOR_CREATE;

/* An fdActorCreate id of ACTOR_CREATE `kind`, above its sprite. */
#define ACTOR_CREATE_KIND(kind) ((kind) << 12)

/* The sprite id in fdActorCreate's id, below its ACTOR_CREATE (id / */
/* 4096). */
#define ACTOR_CREATE_SPRITE_MASK 0xfff

/* The world map's collision values (fdWorldMapGetCollision), 1 to 15; each */
/* indexes a row of WorldMap_BattleEnvironments. */
typedef enum WORLD_MAP_COLLISION {
	/* The party walks only on WORLD_MAP_COLLISION_WALKABLE_FIRST to */
	/* WORLD_MAP_COLLISION_WALKABLE_LAST (fdWorldMapIsWalkable). */
	WORLD_MAP_COLLISION_WALKABLE_FIRST = 5,
	/* Shallow water: a ripple follows the leader as it walks. */
	WORLD_MAP_COLLISION_SHALLOW_WATER = 6,
	/* Where neither collision layer has a value. */
	WORLD_MAP_COLLISION_DEFAULT = 7,
	/* Forest: the leader's sprite shows its second layer and no shadow, and */
	/* encounters take a region's forest zone. */
	WORLD_MAP_COLLISION_FOREST = 9,
	WORLD_MAP_COLLISION_WALKABLE_LAST = 12
} WORLD_MAP_COLLISION;

/* A world map battle environment (WorldMap_BattleEnvironments, */
/* fdWorldMapGetBattleEnvironment): what its ground means for battles, named by
 */
/* the battle background fdBattleBackgroundSelectForEnvironment gives it. */
typedef enum BATTLE_ENVIRONMENT {
	BATTLE_ENVIRONMENT_NONE = 0,	  /* Sol Sanctum's background */
	BATTLE_ENVIRONMENT_GRASSLAND = 1, /* ResourceId_GrasslandBackdrop */
	/* ResourceId_ForestBackdrop; encounters take a region's forest zone. */
	BATTLE_ENVIRONMENT_FOREST = 2,
	BATTLE_ENVIRONMENT_DESERT = 3,	  /* ResourceId_DesertBackdrop */
	BATTLE_ENVIRONMENT_BEACH = 4,	  /* ResourceId_BeachBackdrop */
	BATTLE_ENVIRONMENT_SNOWFIELD = 5, /* ResourceId_SnowfieldBackdrop */
	/* ResourceId_SnowfieldBackdrop, for the snowfield's forest. */
	BATTLE_ENVIRONMENT_SNOW_FOREST = 6,
	/* ResourceId_BeachBackdrop, for shallow water */
	/* (WORLD_MAP_COLLISION_SHALLOW_WATER). */
	BATTLE_ENVIRONMENT_SHALLOWS = 7
} BATTLE_ENVIRONMENT;

/* A world map cell's encounter region (bits 30..24 of its block word), which */
/* picks its encounter zones (WorldMap_EncounterRules), each named for the */
/* places whose entrances lie in it. */
typedef enum WORLD_MAP_ENCOUNTER_REGION {
	WORLD_MAP_ENCOUNTER_REGION_VALE_VAULT = 1, /* also Lunpa and Goma Cave */
	/* Also Bilibin Cave and the bridge out of Goma Cave. */
	WORLD_MAP_ENCOUNTER_REGION_BILIBIN_KOLIMA = 2,
	/* Also Lama Temple and the Alpine Crossing. */
	WORLD_MAP_ENCOUNTER_REGION_XIAN_ALTIN = 3,
	/* Also the Lamakan Desert's cave and Gondowan Cave's north end. */
	WORLD_MAP_ENCOUNTER_REGION_KALAY = 4,
	/* Also Kalay's docks, Altmiller Cave and the Gondowan Passage's north end.
	 */
	WORLD_MAP_ENCOUNTER_REGION_TOLBI = 5,
	/* Also Lalivero, the Venus Lighthouse and the road to Babi Lighthouse. */
	WORLD_MAP_ENCOUNTER_REGION_SUHALLA = 6,
	/* Takes the second world map graphics set (fdWorldMapLoadGraphics) and the
	 */
	/* snowfield's battle environments (fdWorldMapGetBattleEnvironment). */
	WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD = 21,
	/* Fuchin Temple, the sign before Mogall Forest and the north end of Kolima
	 */
	/* Bridge. */
	WORLD_MAP_ENCOUNTER_REGION_FUCHIN = 31
} WORLD_MAP_ENCOUNTER_REGION;

/* A map cell's terrain kind: the low four bits of its shape's first byte, */
/* which pick its height function from MapGrid_TerrainHeightFunctions. */
typedef enum TERRAIN_KIND {
	TERRAIN_FLAT = 0,	 /* the first sample everywhere */
	TERRAIN_SLOPE_X = 1, /* from the first sample at x 0 towards the second */
	TERRAIN_SLOPE_Y = 2, /* from the first sample at y 0 towards the second */
	/* A step across the diagonal x + y = 15, the higher sample on it. */
	TERRAIN_STEP_SUM_DIAGONAL = 3,
	/* A step across the diagonal x = y, the higher sample on it. */
	TERRAIN_STEP_DIFFERENCE_DIAGONAL = 4,
	/* Three samples, bending on the diagonal x + y = 15. */
	TERRAIN_BEND_SUM_DIAGONAL = 5,
	/* Three samples, bending on the diagonal x = y. */
	TERRAIN_BEND_DIFFERENCE_DIAGONAL = 6,
	/* A raised or sunken rounded square (Terrain_RoundedSquareMask). */
	TERRAIN_ROUNDED_SQUARE = 7,
	TERRAIN_STEP_X = 8,	 /* the first sample for x below 8, then the second */
	TERRAIN_STEP_Y = 9,	 /* the first sample for y below 8, then the second */
	TERRAIN_BEND_X = 10, /* three samples along x, bending at the middle */
	TERRAIN_BEND_Y = 11, /* three samples along y, read unsigned */
	/* The sample Terrain_SampleIndexTable picks for each pixel. */
	TERRAIN_SAMPLE_TABLE = 12,
	/* As TERRAIN_SAMPLE_TABLE, mirrored left to right. */
	TERRAIN_SAMPLE_TABLE_MIRRORED = 13,
	TERRAIN_FLAT_B = 14, /* flat as TERRAIN_FLAT, by a copy of its function */
	TERRAIN_FLAT_C = 15, /* flat as TERRAIN_FLAT, by another copy */
	TERRAIN_KIND_COUNT = 16
} TERRAIN_KIND;

/* What fdMapGridCheckCollision says of a step to a position. */
typedef enum COLLISION {
	COLLISION_TOO_LOW = -1,	 /* the ground there is more than 12 pixels lower */
	COLLISION_NONE = 0,		 /* the actor can step there */
	COLLISION_TOO_HIGH = 1,	 /* the ground there is more than 8 pixels higher */
	COLLISION_IMPASSABLE = 2 /* the cell there is a wall (CELL_BEHAVIOR_WALL) */
} COLLISION;

/* The field module's functions in the order of its stub table */
/* (src/field/stubs.s). */
void fdActorSystemConfigure(s32 mode);
void fdRenderApplyProjectedPlacement(SPR *sprite, const SPR_PLACEMENT *position,
									 const SPR_SCALE *scale, s32 facing);
void fdRenderPlaceProjectedSpr(SPR *sprite, s32 *point, s32 *scale, s32 facing,
							   s32 depth);
s32 fdSprResetAnimations(SPR *sprite);
s32 fdSprStartAnimation(SPR *sprite, s32 animation);
s32 fdSprSetAnimationTimers(SPR *sprite, s32 frames);
SPR *fdSprCreate(s32 id);
void fdSprDestroy(SPR *sprite);
void fdRenderPlaceSprPartPair(SPR *sprite, FIXED screenX, s32 height,
							  s32 screenY, s32 groundHeight, FIXED *scale);
s32 fdSprAddAnimation(SPR *sprite, s32 id);
void fdSprRemoveAnimation(SPR *sprite, s32 layer);
void fdSprRemoveAnimationAt(SPR *sprite, u32 index);
void fdSprAnimationClear(SPR_ANIMATION *record);
void fdSprAnimationReset(SPR_ANIMATION *layer);
void fdSprAnimationStart(SPR_ANIMATION *layer, s32 animation);
void fdActorSystemInit(s32 controlMode);
void fdActorSelectAnimation(void *rawActor, s32 value);
void fdActorSetAnimationSpeed(ACTOR *rawActor, s32 value);
void fdActorSetAnimationAndSpeed(ACTOR *actor, s32 animation, s32 step);
void fdActorStartScript(ACTOR *actor, const s32 *script);
void fdActorStartPlayerCtrl(ACTOR *actor);
void fdActorStartWorldMapCtrl(ACTOR *actor);
void fdActorStartMoveByKeys(ACTOR *actor);
void fdActorStartKeyCheck(ACTOR *actor);
void *fdActorFindFree(void);
ACTOR *fdActorCreate(s32 encodedSprite, FIXED x, FIXED y, FIXED z);
void fdActorRelease(ACTOR *actor);
void fdActorSetSprRotation(ACTOR *actor, u32 rotation);
void fdActorStartCamera(ACTOR *actor, s32 watchedActorHandle);
void fdActorStartApproachWatched(ACTOR *actor, s32 watchedActorHandle);
void fdActorSetPosAndResetMotion(ACTOR *actor, FIXED x, FIXED y, FIXED z);
void fdSprDissolveStep(u8 *rawSprite, u32 step);
void fdSprDissolveRun(u8 *sprite);
void fdSprDissolveRunMany(u8 **sprites, s32 count);
s32 fdMapGridLoadMap(s32 index);
UNREAD_RESULT fdWorldMapInitPerspective(void);
void fdMapGridSetCameraCenter(FIXED x, FIXED y);
void fdMapGridCenterOnFollowedPoint(void);
void fdWorldMapUpdateCurrentTileBlock(void);
void fdWorldMapLoadGraphics(s32 x, s32 z);
void fdActorStartIdle(ACTOR *actor);
void fdActorWaitForScriptEnd(void *rawActor);
void fdActorSetMoveTarget(ACTOR *actor, FIXED x, FIXED y, FIXED z);
void fdActorWaitForMove(void *actor);
s32 fdMapGridGetScrRelativePos(ACTOR *actor, s32 *screenPosition);
void fdWorldMapUpdateView(void);
void fdWorldMapLoadDefaultCellsAndUpdateBlock(void);
void fdMapGridPlayMapEdit(u16 *steps, s32 destinationX, u32 destinationY);
void fdMapGridCopyCellTilesIndicesRect(s32 sourceX, s32 sourceY, u32 dstX,
									   u32 dstY, u32 width, u32 height);
void fdTilesetAnimResume(u32 channelIndex);
void fdTilesetAnimPause(u32 channelIndex);
void fdBlendScriptResume(void);
void fdBlendScriptSuspend(void);
FIXED fdMapGridGetTerrainHeight(s32 layer, FIXED x, FIXED y);
s32 fdMapGridGetCellBehaviorAt(s32 layer, FIXED x, FIXED y);
void fdMapGridCopyCellTilesCellsRect(s32 sourceX, s32 sourceY, s32 width,
									 s32 height, s32 dstX, s32 dstY);
void fdMapGridCopyCellAttributeRect(s32 sourceX, s32 sourceY, s32 width,
									s32 height, s32 dstX, s32 dstY);
void fdMapGridCopyCellIndexRect(s32 sourceX, s32 sourceY, s32 width, s32 height,
								s32 dstX, s32 dstY);
void fdWorldMapSetGridTile(s32 x, s32 y, s32 cellX, s32 cellY);
s32 fdMapGridCheckCollision(ACTOR *actor, FIELD_POS *position);
void fdActorSetSprFlags(ACTOR *actor, u32 flags);
void fdActorSetBlendMode(ACTOR *actor, u32 mode);
void fdMapGridSetScrShake(FIXED shakeX, FIXED shakeY, FIXED shakeDecay);
void fdMapGridWaitForShakeToSettle(void);
void fdActorSetFirstSprAnimation(ACTOR *actor, s32 animationId);
void fdActorSystemSuspendProjectionAndShowSprs(void);
void fdActorSystemResumeProjectionAndHideLayers(void);
void fdActorSetSprScale(ACTOR *actor, u32 fixedScale);
s32 fdMapGridIsWallAt(struct WORLD_POS *position);
void fdActorRegisterSprMetadata(ACTOR *actor, s32 animationId);
void fdWorldMapStartStillImage(void);
void fdWorldMapLoadAreaGraphics(void);
void fdActorSetSprColMode(ACTOR *actor, u32 colorMode);
void fdSprSetAnimationColModes(void *rawSprite, u32 colorMode);
s32 fdActorScriptCheckOverlap(ACTOR *actor, s32 *values);
void fdMapGridSetShownLayers(u32 layerFlags);
s32 fdSprAnimationSumCommandLengths(s32 id, u32 animation, s32 timedCount);
s32 fdWorldMapGetBattleEnvironment(struct WORLD_POS *position, s32 *region);
s32 fdSprPictureCacheLoad(u32 slot, u32 *buffer, s32 number, u32 variant);
void fdMapGridSetCellBehaviorAt(u32 layer, FIXED x, FIXED y, u32 behavior);
void fdSprSetAnimationSpeed(SPR *sprite, s32 step);
void fdSprColRemapPixels(u8 *bytes, s32 byteCount);
void fdTilesetAnimSuspendAll(void);
void fdTilesetAnimResumeAll(void);
ACTOR *fdActorScriptFindOverlappingActor(ACTOR *actor, s32 *values);
s32 fdMapGridGetTerrainKindAt(s32 layer, FIXED x, FIXED y);
void fdSprViewerRun(void);

u8 *fdDecompressLz(const u8 *source, u8 *destination);
s32 fdSprComposePicture(SPR *sprite, u16 dir);
s32 fdSprPictureCacheFindByTag(s32 tag);
SPR_ANIMATION *fdSprAnimationAlloc(s32 id);
s32 fdActorScriptSetFlagAndTest(ACTOR *interpreter);
s32 fdActorScriptClearFlagAndTest(ACTOR *interpreter);
/* The state values of SC_SET, SC_ADD and SC_CMP, one handler each */
/* (ActorScript_OperandHandlerTable, field/field_script_commands.c): by */
/* operation, each sets one of the actor's fields to value, adds value to it, or
 */
/* compares the two into conditionResult. */
void fdActorScriptSetOrCompareScriptAddress(struct SCRIPT_OPERANDS *state,
											s32 operation, s32 value);
void fdActorScriptSetOrCompareFacing(struct SCRIPT_OPERANDS *state,
									 s32 operation, s32 value);
void fdActorScriptSetOrCompareX(struct SCRIPT_OPERANDS *state, s32 operation,
								s32 value);
void fdActorScriptSetOrCompareY(struct SCRIPT_OPERANDS *state, s32 operation,
								s32 value);
void fdActorScriptSetOrCompareZ(struct SCRIPT_OPERANDS *state, s32 operation,
								s32 value);
void fdActorScriptSetOrCompareScaleX(struct SCRIPT_OPERANDS *state,
									 s32 operation, s32 value);
void fdActorScriptSetOrCompareScaleY(struct SCRIPT_OPERANDS *state,
									 s32 operation, s32 value);
void fdActorScriptSetOrCompareVelocityX(struct SCRIPT_OPERANDS *state,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareVelocityY(struct SCRIPT_OPERANDS *state,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareVelocityZ(struct SCRIPT_OPERANDS *state,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareSpeedLimit(struct SCRIPT_OPERANDS *state,
										 s32 operation, s32 value);
void fdActorScriptSetOrCompareAcceleration(struct SCRIPT_OPERANDS *state,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareTargetX(struct SCRIPT_OPERANDS *state,
									  s32 operation, s32 value);
void fdActorScriptSetOrCompareTargetY(struct SCRIPT_OPERANDS *state,
									  s32 operation, s32 value);
void fdActorScriptSetOrCompareTargetZ(struct SCRIPT_OPERANDS *state,
									  s32 operation, s32 value);
void fdActorScriptSetOrCompareBounce(struct SCRIPT_OPERANDS *state,
									 s32 operation, s32 value);
void fdActorScriptSetOrCompareGravity(struct SCRIPT_OPERANDS *state,
									  s32 operation, s32 value);
void fdActorScriptSetOrCompareTerrainHeight(struct SCRIPT_OPERANDS *state,
											s32 operation, s32 value);
void fdActorScriptSetOrCompareSceneWord(struct SCRIPT_OPERANDS *state,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareSpr(struct SCRIPT_OPERANDS *state, s32 operation,
								  s32 value);
void fdActorScriptSetOrCompareSprStorageKind(struct SCRIPT_OPERANDS *state,
											 s32 operation, s32 value);
void fdActorScriptSetOrCompareMotionFlags(struct SCRIPT_OPERANDS *state,
										  s32 operation, s32 value);
void fdActorScriptSetOrCompareArrivalAxis(struct SCRIPT_OPERANDS *state,
										  s32 operation, s32 value);
void fdActorScriptSetOrCompareComparisonResult(struct SCRIPT_OPERANDS *state,
											   s32 operation, s32 value);
void fdActorScriptSetOrCompareSnapToTarget(struct SCRIPT_OPERANDS *state,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareCollisionFlags(struct SCRIPT_OPERANDS *state,
											 s32 operation, s32 value);
void fdActorScriptSetOrCompareSteeringFlags(struct SCRIPT_OPERANDS *state,
											s32 operation, s32 value);
void fdActorScriptSetOrComparePaused(struct SCRIPT_OPERANDS *state,
									 s32 operation, s32 value);
void fdActorScriptSetOrCompareRepeatCount(struct SCRIPT_OPERANDS *state,
										  s32 operation, s32 value);
void fdActorScriptSetOrCompareTimer(struct SCRIPT_OPERANDS *state,
									s32 operation, s32 value);
void fdActorScriptSetOrCompareUserValue(struct SCRIPT_OPERANDS *state,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareWatchedActor(struct SCRIPT_OPERANDS *state,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareUpdateAddress(struct SCRIPT_OPERANDS *state,
											s32 operation, s32 value);
void fdActorScriptSetOrCompareSceneCounter(struct SCRIPT_OPERANDS *state,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareSceneByte(struct SCRIPT_OPERANDS *state,
										s32 operation, s32 value);
void fdSprViewerSetLayerColMode(s32 slot, s32 value);
void fdSprViewerSetLayerPriority(s32 slot, s32 value);

void fdActorSystemReservedNoOpA(void);
void fdActorSystemReservedNoOpB(void);
s32 fdActorReturnTrue(void);
void fdActorSystemPlaceSprs(void);
s32 fdActorCameraReturnTrue(void);
void fdActorSystemProjectSprs(void);

s32 fdActorScriptSetBase(ACTOR *interpreter);
s32 fdActorScriptHideSprs(ACTOR *interpreter);
s32 fdActorScriptShowSpr(ACTOR *interpreter);
s32 fdActorIsTargetUnset(ACTOR *actor);
void fdActorUpdateAllOnField(void);
void fdActorRunUpdateAllFromHeap(void);
void fdActorUpdateAllMotion(void);
s32 fdActorScriptWait(ACTOR *interpreter);
s32 fdActorScriptWaitForArrival(ACTOR *interpreter);
s32 fdActorScriptInvokeCallback(ACTOR *interpreter);
s32 fdActorScriptFindLabel(ACTOR *interpreter, u32 labelKey);
s32 fdActorScriptRepeatOrJump(ACTOR *interpreter);
s32 fdActorScriptJumpToLabel(ACTOR *interpreter);
s32 fdActorScriptJumpIfTrue(ACTOR *interpreter);
s32 fdActorScriptJumpIfFalse(ACTOR *interpreter);
s32 fdActorScriptEnd(ACTOR *interpreter);
s32 fdActorScriptTestFlag(ACTOR *interpreter);
s32 fdActorScriptToggleFlagAndTest(ACTOR *interpreter);
s32 fdActorScriptSetAnimation(ACTOR *interpreter);
s32 fdActorScriptRelease(ACTOR *interpreter);
s32 fdActorScriptNop(ACTOR *interpreter);
s32 fdActorScriptPlaySnd(ACTOR *interpreter);
s32 fdActorScriptSetPosAndResetMotion(ACTOR *interpreter);
s32 fdActorScriptApplyAbsolutePos(ACTOR *interpreter);
s32 fdActorScriptApplyRelativePos(ACTOR *interpreter);
s32 fdActorScriptStoreAngToWatchedActor(ACTOR *interpreter);
s32 fdActorScriptApplyWatchedActorPos(ACTOR *interpreter);
s32 fdActorScriptApplyLocalOffsetPos(ACTOR *interpreter);
s32 fdCameraFollowLeaderInBounds(ACTOR *interpreter);
s32 fdActorApproachWatchedActor(ACTOR *interpreter);
s32 fdActorWander(ACTOR *interpreter);
s32 fdActorScriptWanderNearHome(ACTOR *interpreter);

void fdActorScriptSetOrCompareCursor(struct SCRIPT_OPERANDS *operands,
									 s32 operation, s32 value);
void fdActorScriptSetOrCompareRadius(struct SCRIPT_OPERANDS *state,
									 s32 operation, s32 value);
void fdActorScriptSetOrCompareWait(struct SCRIPT_OPERANDS *state, s32 operation,
								   s32 value);
s32 fdActorScriptApplyOperandSet(ACTOR *interpreter);
s32 fdActorScriptApplyOperandAdd(ACTOR *interpreter);
s32 fdActorScriptApplyOperandCompare(ACTOR *interpreter);

void fdActorSetUpdateValue(void *rawActor, s32 value);
u32 fdRequestShortcut(u32 encodedAssignment);
s32 fdCheckConfiguredKeys(void);
s32 fdCheckSpheresOverlap(FIXED *firstPosition, s32 radiusA,
						  FIXED *secondPosition, s32 radiusB);
s32 fdActorUpdatePlayerCtrl(ACTOR *interpreter);
s32 fdActorMoveOnWorldMap(ACTOR *interpreter);
s32 fdCheckConfiguredKeysAndCount(ACTOR *interpreter);
s32 fdActorMoveByKeys(ACTOR *interpreter);

void fdTileMapDecodeStagedBuffer(s32 size);
void fdTileMapNumDummyCellTiles(void);
void fdTileMapConvertBuffer(void);
void fdMapGridRenderCellTilesRow(u32 layer, s32 tileX, s32 tileY);
void fdMapGridRenderCellTilesColumn(u32 layer, s32 tileX, s32 tileY);
void fdMapGridUpdateLayerScroll(void);

s32 fdWorldMapWriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
void fdWorldMapApplyAffinePage(void);
void fdWorldMapDrawCellRow(u32 tileRow);
void fdWorldMapDrawCellColumn(u32 tileColumn);
void fdWorldMapPreloadCurrentTileBlock(void);
void fdWorldMapPresentStillImage(void);
void fdWorldMapEndStillImage(void);
void fdTilesetAnimUpdate(void);
void fdTilesetAnimStartAll(const u16 *script);
void fdBlendScriptRun(void);
void fdBlendScriptStart(u16 *script);

void fdPaletteCycleInit(void);
s32 fdPaletteCycleAdd(s16 paletteNumber, s16 firstColor, s16 delay,
					  s16 colorCount);
void fdPaletteCycleStop(void);
void fdPaletteCycleStart(void);
void fdPaletteCycleStep(void);

FIXED fdTerrainGetFlatHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 fdTerrainGetSlopeXHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 fdTerrainGetSlopeYHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
FIXED fdTerrainGetStepSumDiagonalHeight(const u8 *heightBytes, s32 x, s32 y);
FIXED fdTerrainGetStepDifferenceDiagonalHeight(const u8 *heightBytes, s32 x,
											   s32 y);
FIXED fdTerrainGetBendSumDiagonalHeight(const u8 *heightBytes, s32 x, s32 y);
FIXED fdTerrainGetBendDifferenceDiagonalHeight(const u8 *heightBytes, s32 x,
											   s32 y);
FIXED fdTerrainGetRoundedSquareHeight(const u8 *heightBytes, s32 x, s32 y);
s32 fdTerrainGetStepXHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 fdTerrainGetStepYHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
FIXED fdTerrainGetBendXHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
FIXED fdTerrainGetBendYHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
FIXED fdTerrainGetSampleTableHeight(const u8 *heightBytes, s32 pixelX,
									s32 pixelY);
FIXED fdTerrainGetSampleTableMirroredHeight(const u8 *heightBytes, s32 pixelX,
											s32 pixelY);
FIXED fdTerrainGetFlatBHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);
FIXED fdTerrainGetFlatCHeight(const u8 *heightBytes, s32 pixelX, s32 pixelY);

s32 fdMapGridGetCellHighFlags(s32 x, s32 y);
s32 fdWorldMapGetCollision(struct WORLD_POS *position);
s32 fdWorldMapIsWalkable(s32 unused, struct WORLD_POS *position);
/* A screen shake's component left as it is; any negative value does. */
#define SHAKE_KEEP (-1)
void fdTileMapDrawRowsFromRam(const struct WORLD_MAP_AFFINE_PAIR *lines,
							  u8 *pixels);
void fdWorldMapBuildScanlineTable(FIXED depth, s32 *position,
								  struct WORLD_MAP_SCANLINE *scanline);

s32 fdSprViewerStepSprId(s32 spriteId, s32 direction);
void fdSprViewerLayOutRing(s32 centerX, s32 centerY, SPR_PLACEMENT *positions);
void fdSprViewerStartStaggeredAnimations(s32 layerIndex, s32 animationIndex);
void fdSprViewerSetLayerSpr(s32 layerIndex, s32 spriteId);
void fdSprViewerPlaceRing(void);

/* Read as a halfword by fdActorSetOthersPaused: the actor id of the */
/* player's actor, which it leaves alone. */
extern u8 PlayerActorId[];

#endif
