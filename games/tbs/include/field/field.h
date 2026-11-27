#ifndef ALCHEMY_FIELD_H
#define ALCHEMY_FIELD_H

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

/* An IWRAM word the scrolling camera's setup clears; nothing in the tree reads
   it. */
extern Sint32 CameraUnreadWordA;
/* The second IWRAM word the scrolling camera's setup clears; also unread. With
   no reader to give them a role, the two are numbered in IWRAM order. */
extern Sint32		CameraUnreadWordB;
extern const Sint32 ActorDefaultScript[];
extern const Uint8	WorldMapBattleEnvironments[];
/* The size of fdRenderDecodePicture, copied to HEAP_SLOT_TICK_SCRATCH; an
   address equal to the size (ld_script.ld). */
extern Uint8 RenderDecodePictureCodeSize[];
/* Four 256-byte colour tables (remap0 to remap3), indexed by pixel: the
   SPR_COL_MODE tables after SpriteColorTables' five, the first also remapping
   the command icons (fdSpriteColorRemapPixels). */
extern const Uint8 SpriteColorGreyTable[];

struct BATTLE_EFFECT_PARM;
struct FIELD_POS;
struct MENU_SPR;
struct CUE;
struct ACTOR_PLACE;

/* The Colosso opponent's path player, at the start of SceneLocalState: the path
   in COLOSSO_RACE_STATE is played back or recorded by
   scColossoUpdatePathOpponent. */
typedef struct PATH_RECORDER {
	Sint16 mode;	/* a PATH_RECORDER_MODE (scene/colosso_setup.h) */
	Sint16 mirror;	/* nonzero mirrors the path across the finish */
	Sint16 actorId; /* the actor id moved along the path */
	Uint16
		pathIndex; /* the next halfword of COLOSSO_RACE_STATE.path, x then z */
	/* Ticks the actor has not moved; from 3 it stands (fdActorSelectAnimation
	   1). */
	Sint16 still;
} PATH_RECORDER;

/* A Venus Lighthouse switch or pillar socket's map metatile. */
typedef struct SWITCH_METATILE {
	Uint32 x; /* 16-pixel map metatiles */
	Uint32 z; /* 16-pixel map metatiles */
} SWITCH_METATILE;

/* One of a ledge jump's dust puffs (fdEffectSpawnDustPuff):
   SC_WAIT(1) looped 38 times, then SC_RELEASE. */
extern const Sint32 FieldEffectBurstParticleScriptA[];
/* The other dust puff: the same, looped 18 times. */
extern const Sint32 FieldEffectBurstParticleScriptB[];
extern Uint32		WorldMapGraphicsResources[][6];
/* SC_WAIT(40), then SC_RELEASE: the one-off attached effect that ends by itself
   (fdEffectSetAttached, ATTACHED_EFFECT_ONCE). */
extern const Sint32 ActorWaitScript[];
/* The sparkles' 16 by 16 tiles, packed (fdEffectInitSparkles). */
extern const Uint8 FieldEffectSparkleTiles[];
extern const Uint8 FieldExamineCueMessageOffsets[];
/* The motes of light's 16 by 16 tiles, packed (fdMotesStart). */
extern const Uint8 FieldEffectMoteTiles[];
extern Sint32	   DjinnFiguresSpriteIds[];
extern Sint32	   ShipRoomsShakeTimer;
struct FLOODGATE_LOG;
/* The ACTOR_COUNT actors (HEAP_SLOT_ACTORS), 112 bytes each. */
#define ActorsWork (*(ACTOR **)&HeapSlots[HEAP_SLOT_ACTORS])
/* The lighthouse transition's pictures, packed (tagged LZ), decoded 4 KB after
   the tiles (scWorldMapPlayLighthouseTransition). */
extern const Uint8 WorldMapPackedPictures[];
/* The lighthouse transition's tiles, packed (tagged LZ). */
extern const Uint8 WorldMapPackedTiles[];
/* The lighthouse transitions' palettes, 32 bytes each, loaded as BG palette 14;
   indexed by WORLD_MAP_LIGHTHOUSE. */
extern const Uint8 WorldMapPalettes[];

/* The lighthouse whose beacon the world map shows lit
   (scWorldMapPlayLighthouseTransition), as WorldMapPalettes orders their
   colours. */
typedef enum WORLD_MAP_LIGHTHOUSE {
	WORLD_MAP_LIGHTHOUSE_MERCURY,
	WORLD_MAP_LIGHTHOUSE_VENUS,
} WORLD_MAP_LIGHTHOUSE;

/* The race gauge's 16 colours, loaded as OBJ palette 14
   (scColossoDrawGauge). */
extern Uint16 ColossoGaugePalette[];
/* Ticks in a row the chase has found a suspect cornered; at
   LOG_ROOMS_A_CORNERED_TICKS the chase raises its script. */
extern Uint16 LogRoomsACorneredTicks;
/* The size of fdActorUpdateAll's code, an address equal to the size
   (ld_script.ld). */
extern Uint8	   fdActorUpdateAllCodeSize[];
extern const Uint8 ActorBlinkSpriteColors[];
/* The actor motion scripts: movement types 1 to 7, which fdActorSetMovementType
   starts by number, and two turning scripts with no movement type number.
   Movement type 1: a lone SC_END. */
extern const Sint32 MovementTypeStandScript[];
/* Movement type 2: wanders within 16 pixels of home (SC_WANDER_HOME) at half a
   pixel a tick. */
extern const Sint32 MovementTypeWanderScript[];
/* Movement type 3: the same within 8 pixels of home. */
extern const Sint32 MovementTypeWanderNearScript[];
/* Movement type 4: wanders within 6 pixels of home at 0.4 pixels a tick. */
extern const Sint32 MovementTypeWanderCloseScript[];
/* Waits to arrive, then turns by fdActorStepAngle until it faces the kept angle
   (fdActorLaunchFromLeader). */
extern const Sint32 ActorLaunchScript[];
/* Movement type 5: fdActorMoveTowardTarget every tick, forever. */
extern const Sint32 MovementTypeFollowWatchedScript[];
/* Movement type 7: fdActorClearSpriteAndCollisionFlags once, then SC_END. */
extern const Sint32 MovementTypeResetActionScript[];
/* Turns by fdActorStepAngle until it faces the kept angle, then SC_END. */
extern const Sint32 ActorStepAngleScript[];
/* Movement type 6: turns toward the leader every tick, forever. */
extern const Sint32 MovementTypeFaceLeaderScript[];
extern const Sint32 ActorVariantScripts[];
/* Lamakan Desert's heat gauge (scHeatGaugeStart and scHeatGaugeDraw). The
   gauge's tiles, packed (tagged LZ). */
extern Uint8 HeatGaugeTiles[];
/* The gauge's 16 colours, loaded as OBJ palette 14. */
extern const Uint16 HeatGaugePalette[16];
/* The sprite picture decoder, ARM code run from a copy in
   HEAP_SLOT_TICK_SCRATCH. */
extern Uint8 fdRenderDecodePicture[];
/* The statue scene's three scrolling strips, packed (syDecompressByteLZ). */
extern Uint8 SolSanctumStatueSpriteRowsGraphics[];
/* The six large things' sprite animations, which pick their rectangles in
   PushedActorRectangles; each scene with them defines its own. */
extern Uint32 PushedActorRectangleKinds[];
/* The tilemap row drawer, run from a copy in HEAP_SLOT_RAM_CODE_TEMP. */
extern const Uint8 fdTileMapDrawRows[];
/* The size of fdTileMapDrawRows (picture_decode.s). */
extern Uint8 fdTileMapDrawRowsCodeSize[];
/* The size of fdWorldMapUpdateAffineLines, copied to HEAP_SLOT_RAM_CODE; an
   address equal to the size (ld_script.ld). */
extern Uint8		fdWorldMapUpdateAffineLinesCodeSize;
extern const Sint32 WorldMapFootprintActions[6];
/* The direction pad's headings, defined as Uint16 in facing.c. Declared here
   with the sign, so Up's three quarters of a turn reads negative; the player's
   steps keep each one as a Uint16 again, the same heading. */
extern const Sint16 WorldMapKeyHeadings[16];

/* The field effects' scripts and tables (field_mode_tables.c,
   movement_scripts.c, field_effect_tables.c and
   graphics/field_effects/spark_tiles.s). */
extern const Uint16 InputDpadAngles[];
extern const Sint8	HaltFlickerColorModes[];
/* The 32 by 16 spark of Avoid's falling arcs (arc_spark.png), 4 bits a pixel;
   fdPsynergyAvoid loads it. */
extern const Uint8	FieldEffectArcSparkTiles[];
extern const Sint32 GrowthLeafScript[];
/* Waits to arrive, sets off fdEffectSpawnHeavyImpact and releases the actor:
   Force's fist as it lands. */
extern const Sint32 ForceFistScript[];
/* The emote bubble's script (fdActorShowEmoteById): eight 2-tick steps of
   squash and stretch (SC_ADD to SW_SCALE_X and SW_SCALE_Y), 30 ticks still,
   then SC_RELEASE. */
extern const Sint32 EmoteBubbleScript[];
extern const Sint32 RevealMarkerScript[];
extern const Sint32 ItemDisplayGrowInScript[];
extern const Sint32 FieldEffectParticleScript[];
extern const Uint32 FieldEffectPulseScales[];
extern const Sint32 FieldEffectSourceHoldScript[];
extern const Sint16 FieldEffectTargetRangeByMode[];
/* A lone SC_RELEASE: an actor that has arrived releases itself
   (fdMindReadUpdateCrescent). */
extern const Sint32 FieldEffectUntargetedActorScript[];
/* The 32 by 16 spark a field Psynergy's particles draw (spark.png), 4 bits a
   pixel; fdPsynergyLoadResources loads it. */
extern const Uint8 FieldEffectSparkTiles[];
extern const Uint8 FieldEffectQuadrantModes[];
/* The eight tints (PALETTE_TINT_CUSTOM and an RGB555 colour) a field Psynergy
   fades the palettes toward, picked by the tick counter or by the
   FLAG_PSYNERGY_TINT flags (fdPsynergyFadeToCastColor). */
extern const Sint32 FieldPsynergyCastColors[];
extern const Sint16 WeatherRainSplashFrames[];
/* The ground particles' 16 by 16 tiles, packed (syDecompressByteLZ). */
extern const Uint8	WeatherParticleTiles[];
extern const Sint32 ItemDisplayPulseScript[];

/* The first five 256-byte colour tables of SPR_COL_MODE, indexed by pixel. */
extern const Uint8 SpriteColorTables[][256];
/* The sprites' shadow, 16 by 8 at 256 colours, loaded into VRAM cache entry
   93. */
extern const Uint8 ActorShadowTiles[];

#include "graphics/render_placement.h"

struct SPR_ANIMATION;
struct SPR;
struct ACTOR;
struct FIELD_POS;
struct WORLD_MAP_AFFINE_PAIR;
struct WORLD_MAP_SCANLINE;
struct SCRIPT_OPERANDS;
struct WORLD_POS;

/* How the actor system runs (fdActorSystemInit): which camera follows the
   actors and which update moves them. */
typedef enum ACTOR_SYSTEM_MODE {
	/* A town, a dungeon or the sprite viewer: the scrolling camera. */
	ACTOR_SYSTEM_MAP = 2,
	/* The world map: the fixed camera, which projects 3D points onto the
	   screen. */
	ACTOR_SYSTEM_WORLD_MAP = 3,
	/* A battle: the fixed camera, and the battle's own motion update. */
	ACTOR_SYSTEM_BATTLE = 4
} ACTOR_SYSTEM_MODE;

/* SPR_ANIMATION.kind: the facing table a sprite animation's pictures follow
   (fdSpriteComposePicture, facing.c). */
typedef enum ANIMATION_FACING_KIND {
	/* The animation script's own picture whatever the facing; zero also marks a
	   free sprite animation. */
	ANIM_FACING_NONE = 0,
	/* 8 sectors: one picture, mirrored to the left */
	ANIM_FACING_ONE_PICTURE = 1,
	ANIM_FACING_FRONT_BACK = 2,		/* 8 sectors: a front and a back picture */
	ANIM_FACING_THREE_PICTURES = 3, /* 16 sectors: front, side and back */
	ANIM_FACING_FOUR_PICTURES = 4,	/* 64 sectors */
	ANIM_FACING_FIVE_PICTURES = 5,	/* 16 sectors */
	ANIM_FACING_SIX_PICTURES = 6,	/* 64 sectors */
	/* 8 sectors centred on the compass directions, a picture for each. */
	ANIM_FACING_EIGHT_PICTURES = 8,
	/* ANIM_FACING_FRONT_BACK's table for a sprite drawn in two parts, the
	   second the next sprite id (the sprite viewer). */
	ANIM_FACING_TWO_PART = 20,
	/* As ANIM_FACING_FRONT_BACK, with the front picture mirrored the other
	   way. */
	ANIM_FACING_FRONT_BACK_SWAPPED = 22,
	/* Unused: no sprite's metadata or animation takes this kind. Ringed as
	   eight facings by the sprite viewer, but with no facing table:
	   fdSpriteComposePicture draws it as ANIM_FACING_NONE. */
	ANIM_FACING_EIGHT_NO_TABLE = 44,
	/* As ANIM_FACING_EIGHT_PICTURES, with three of the pictures mirrored. */
	ANIM_FACING_EIGHT_PICTURES_MIRRORED = 88
} ANIMATION_FACING_KIND;

/* SPR_ANIMATION.mode, from the metadata's pictureCodec: how a sprite
   animation's pictures are stored. Any other value is raw, for the picture
   decoder. */
typedef enum SPR_PICTURE_CODEC {
	SPRITE_PICTURE_RAW = 0,
	SPRITE_PICTURE_TAGGED_LZ = 1, /* syDecompressTaggedLZ's format */
	SPRITE_PICTURE_LZ = 3		  /* LZ-compressed */
} SPR_PICTURE_CODEC;

/* SPR.flags. */
typedef enum SPR_FLAG {
	SPRITE_FLAG_SHADOW = 1 << 0, /* its shadow is drawn; set when created */
	/* Drawn as an outline, in ACTOR_SYSTEM_STATE's edge and fill colours. */
	SPRITE_FLAG_OUTLINED = 1 << 1
} SPR_FLAG;

/* ACTOR.spriteStorageKind, its low four bits: what the actor's sprite pointer
   holds. */
typedef enum ACTOR_SPR_KIND {
	ACTOR_SPRITE_NONE = 0, /* nothing drawn */
	ACTOR_SPRITE_ONE = 1,
	ACTOR_SPRITE_GROUP = 2 /* a list of up to four sprites, empty ones null */
} ACTOR_SPR_KIND;

/* ACTOR.spriteStorageKind's low four bits, its ACTOR_SPR_KIND. */
#define ACTOR_SPRITE_KIND_MASK 15

/* fdActorCreate's id / 4096: what sprites the actor is made with. */
typedef enum ACTOR_CREATE {
	ACTOR_CREATE_ONE_SPRITE = 0, /* the id's sprite (ACTOR_SPRITE_ONE) */
	/* The sprites of the id and id + 1 (ACTOR_SPRITE_GROUP). */
	ACTOR_CREATE_GROUP = 2,
	/* No sprite: any kind but the two above, and the one the camera actor
	   takes. */
	ACTOR_CREATE_NO_SPRITE = 8
} ACTOR_CREATE;

/* An fdActorCreate id of ACTOR_CREATE `kind`, above its sprite. */
#define ACTOR_CREATE_KIND(kind) ((kind) << 12)

/* The sprite id in fdActorCreate's id, below its ACTOR_CREATE (id / 4096). */
#define ACTOR_CREATE_SPRITE_MASK 0xfff

/* The world map's collision values (fdWorldMapGetCollision), 1 to 15; each
   indexes a row of WorldMapBattleEnvironments. */
typedef enum WORLD_MAP_COLLISION {
	/* The party walks only on WORLD_MAP_COLLISION_WALKABLE_FIRST to
	   WORLD_MAP_COLLISION_WALKABLE_LAST (fdWorldMapIsWalkable). */
	WORLD_MAP_COLLISION_WALKABLE_FIRST = 5,
	/* Shallow water: a ripple follows the leader as it walks. */
	WORLD_MAP_COLLISION_SHALLOW_WATER = 6,
	/* Where neither collision layer has a value. */
	WORLD_MAP_COLLISION_DEFAULT = 7,
	/* Forest: the leader's sprite shows its second layer and no shadow, and
	   encounters take a region's forest zone. */
	WORLD_MAP_COLLISION_FOREST = 9,
	WORLD_MAP_COLLISION_WALKABLE_LAST = 12
} WORLD_MAP_COLLISION;

/* A world map battle environment (WorldMapBattleEnvironments,
   fdWorldMapGetBattleEnvironment): what its ground means for battles, named by
   the battle background fdBattleBackgroundSelectForEnvironment gives it. */
typedef enum BATTLE_ENVIRONMENT {
	BATTLE_ENVIRONMENT_NONE = 0,	  /* Sol Sanctum's background */
	BATTLE_ENVIRONMENT_GRASSLAND = 1, /* ResourceIdGrasslandBackdrop */
	/* ResourceIdForestBackdrop; encounters take a region's forest zone. */
	BATTLE_ENVIRONMENT_FOREST = 2,
	BATTLE_ENVIRONMENT_DESERT = 3,	  /* ResourceIdDesertBackdrop */
	BATTLE_ENVIRONMENT_BEACH = 4,	  /* ResourceIdBeachBackdrop */
	BATTLE_ENVIRONMENT_SNOWFIELD = 5, /* ResourceIdSnowfieldBackdrop */
	/* ResourceIdSnowfieldBackdrop, for the snowfield's forest. */
	BATTLE_ENVIRONMENT_SNOW_FOREST = 6,
	/* ResourceIdBeachBackdrop, for shallow water
	   (WORLD_MAP_COLLISION_SHALLOW_WATER). */
	BATTLE_ENVIRONMENT_SHALLOWS = 7
} BATTLE_ENVIRONMENT;

/* A world map metatile's encounter region (bits 30..24 of its block word),
   which picks its encounter zones (WorldMapEncounterRules), each named for the
   places whose entrances lie in it. */
typedef enum WORLD_MAP_ENCOUNTER_REGION {
	WORLD_MAP_ENCOUNTER_REGION_VALE_VAULT = 1, /* also Lunpa and Goma Cave */
	/* Also Bilibin Cave and the bridge out of Goma Cave. */
	WORLD_MAP_ENCOUNTER_REGION_BILIBIN_KOLIMA = 2,
	/* Also Lama Temple and the Alpine Crossing. */
	WORLD_MAP_ENCOUNTER_REGION_XIAN_ALTIN = 3,
	/* Also the Lamakan Desert's cave and Gondowan Cave's north end. */
	WORLD_MAP_ENCOUNTER_REGION_KALAY = 4,
	/* Also Kalay's docks, Altmiller Cave and the Gondowan Passage's north
	   end. */
	WORLD_MAP_ENCOUNTER_REGION_TOLBI = 5,
	/* Also Lalivero, the Venus Lighthouse and the road to Babi Lighthouse. */
	WORLD_MAP_ENCOUNTER_REGION_SUHALLA = 6,
	/* Takes the second world map graphics set (fdWorldMapLoadGraphics) and the
	   snowfield's battle environments (fdWorldMapGetBattleEnvironment). */
	WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD = 21,
	/* Fuchin Temple, the sign before Mogall Forest and the north end of Kolima
	   Bridge. */
	WORLD_MAP_ENCOUNTER_REGION_FUCHIN = 31
} WORLD_MAP_ENCOUNTER_REGION;

/* A map metatile's terrain kind: the low four bits of its shape's first byte,
   which pick its height function from MapGridTerrainHeightFunctions. */
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
	/* A raised or sunken rounded square (TerrainRoundedSquareMask). */
	TERRAIN_ROUNDED_SQUARE = 7,
	TERRAIN_STEP_X = 8,	 /* the first sample for x below 8, then the second */
	TERRAIN_STEP_Y = 9,	 /* the first sample for y below 8, then the second */
	TERRAIN_BEND_X = 10, /* three samples along x, bending at the middle */
	TERRAIN_BEND_Y = 11, /* three samples along y, read unsigned */
	/* The sample TerrainSampleIndexTable picks for each pixel. */
	TERRAIN_SAMPLE_TABLE = 12,
	/* As TERRAIN_SAMPLE_TABLE, mirrored left to right. */
	TERRAIN_SAMPLE_TABLE_MIRRORED = 13,
	TERRAIN_FLAT_B = 14, /* flat as TERRAIN_FLAT, by a copy of its function */
	TERRAIN_FLAT_C = 15, /* flat as TERRAIN_FLAT, by another copy */
	TERRAIN_KIND_COUNT = 16
} TERRAIN_KIND;

/* What fdMapGridCheckCollision says of a step to a position. */
typedef enum COLLISION {
	COLLISION_TOO_LOW = -1, /* the ground there is more than 12 pixels lower */
	COLLISION_NONE = 0,		/* the actor can step there */
	COLLISION_TOO_HIGH = 1, /* the ground there is more than 8 pixels higher */
	/* the metatile there is a wall (METATILE_BEHAVIOR_WALL) */
	COLLISION_IMPASSABLE = 2
} COLLISION;

/* The field module's functions in the order of its stub table
   (src/field/stubs.s). */
void fdActorSystemConfigure(Sint32 mode);
void fdRenderApplyProjectedPlacement(SPR *sprite, const SPR_PLACEMENT *pos,
	const SPR_SCALE *scale, Sint32 facing);
void fdRenderPlaceProjectedSprite(
	SPR *sprite, Sint32 *point, Sint32 *scale, Sint32 facing, Sint32 depth);
Sint32 fdSpriteResetAnimations(SPR *sprite);
Sint32 fdSpriteStartAnimation(SPR *sprite, Sint32 animation);
Sint32 fdSpriteSetAnimationTimers(SPR *sprite, Sint32 ticks);
SPR	  *fdSpriteCreate(Sint32 id);
void   fdSpriteDestroy(SPR *sprite);
void   fdRenderPlaceSpritePartPair(SPR *sprite, FIXED screenX, Sint32 height,
	  Sint32 screenY, Sint32 groundHeight, FIXED *scale);
Sint32 fdSpriteAddAnimation(SPR *sprite, Sint32 id);
void   fdSpriteRemoveAnimation(SPR *sprite, Sint32 layer);
void   fdSpriteRemoveAnimationAt(SPR *sprite, Uint32 index);
void   fdSpriteAnimationClear(SPR_ANIMATION *record);
void   fdSpriteAnimationReset(SPR_ANIMATION *layer);
void   fdSpriteAnimationStart(SPR_ANIMATION *layer, Sint32 animation);
void   fdActorSystemInit(Sint32 controlMode);
void   fdActorSelectAnimation(void *rawActor, Sint32 value);
void   fdActorSetAnimationSpeed(ACTOR *rawActor, Sint32 value);
void   fdActorSetAnimationAndSpeed(ACTOR *actor, Sint32 animation, Sint32 step);
void   fdActorStartScript(ACTOR *actor, const Sint32 *script);
void   fdActorStartPlayerControl(ACTOR *actor);
void   fdActorStartWorldMapControl(ACTOR *actor);
void   fdActorStartMoveByKeys(ACTOR *actor);
void   fdActorStartKeyCheck(ACTOR *actor);
void  *fdActorFindFree(void);
ACTOR *fdActorCreate(Sint32 encodedSprite, FIXED x, FIXED y, FIXED z);
void   fdActorRelease(ACTOR *actor);
void   fdActorSetSpriteRotation(ACTOR *actor, Uint32 rotation);
void   fdActorStartCamera(ACTOR *actor, Sint32 watchedActorHandle);
void   fdActorStartApproachWatched(ACTOR *actor, Sint32 watchedActorHandle);
void   fdActorSetPosAndResetMotion(ACTOR *actor, FIXED x, FIXED y, FIXED z);
void   fdSpriteDissolveStep(Uint8 *rawSprite, Uint32 step);
void   fdSpriteDissolveRun(Uint8 *sprite);
void   fdSpriteDissolveRunMany(Uint8 **sprites, Sint32 count);
Sint32 fdMapGridLoadMap(Sint32 index);
UNREAD_RESULT fdWorldMapInitPerspective(void);
void		  fdMapGridSetCameraCenter(FIXED x, FIXED y);
void		  fdMapGridCenterOnFollowedPoint(void);
void		  fdWorldMapUpdateCurrentTileBlock(void);
void		  fdWorldMapLoadGraphics(Sint32 x, Sint32 z);
void		  fdActorStartIdle(ACTOR *actor);
void		  fdActorWaitForScriptEnd(void *rawActor);
void		  fdActorSetMoveTarget(ACTOR *actor, FIXED x, FIXED y, FIXED z);
void		  fdActorWaitForMove(void *actor);
Sint32		  fdMapGridGetScreenRelativePos(ACTOR *actor, Sint32 *screenPos);
void		  fdWorldMapUpdateView(void);
void		  fdWorldMapLoadDefaultMetatilesAndUpdateBlock(void);
void		  fdMapGridPlayMapEdit(Uint16 *steps, Sint32 destX, Uint32 destY);
void		  fdMapGridCopyMetatileTilesIndicesRect(Sint32 srcX, Sint32 srcY,
			 Uint32 destX, Uint32 destY, Uint32 width, Uint32 height);
void		  fdTilesetAnimationResume(Uint32 channelIndex);
void		  fdTilesetAnimationPause(Uint32 channelIndex);
void		  fdBlendScriptResume(void);
void		  fdBlendScriptSuspend(void);
FIXED		  fdMapGridGetTerrainHeight(Sint32 layer, FIXED x, FIXED y);
Sint32		  fdMapGridGetMetatileBehaviorAt(Sint32 layer, FIXED x, FIXED y);
void		  fdMapGridCopyMetatileTilesMetatilesRect(Sint32 srcX, Sint32 srcY,
			 Sint32 width, Sint32 height, Sint32 destX, Sint32 destY);
void fdMapGridCopyMetatileAttributeRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 destX, Sint32 destY);
void fdMapGridCopyMetatileIndexRect(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 destX, Sint32 destY);
void fdWorldMapSetGridTile(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY);
Sint32 fdMapGridCheckCollision(ACTOR *actor, FIELD_POS *pos);
void   fdActorSetSpriteFlags(ACTOR *actor, Uint32 flags);
void   fdActorSetBlendMode(ACTOR *actor, Uint32 mode);
void   fdMapGridSetScreenShake(FIXED shakeX, FIXED shakeY, FIXED shakeDecay);
void   fdMapGridWaitForShakeToSettle(void);
void   fdActorSetFirstSpriteAnimation(ACTOR *actor, Sint32 animationId);
void   fdActorSystemSuspendProjectionAndShowSprites(void);
void   fdActorSystemResumeProjectionAndHideLayers(void);
void   fdActorSetSpriteScale(ACTOR *actor, Uint32 fixedScale);
Sint32 fdMapGridIsWallAt(struct WORLD_POS *pos);
void   fdActorRegisterSpriteMetadata(ACTOR *actor, Sint32 animationId);
void   fdWorldMapStartStillImage(void);
void   fdWorldMapLoadAreaGraphics(void);
void   fdActorSetSpriteColorMode(ACTOR *actor, Uint32 colorMode);
void   fdSpriteSetAnimationColorModes(void *rawSprite, Uint32 colorMode);
Sint32 fdActorScriptCheckOverlap(ACTOR *actor, Sint32 *values);
void   fdMapGridSetShownLayers(Uint32 layerFlags);
Sint32 fdSpriteAnimationSumCommandLengths(
	Sint32 id, Uint32 animation, Sint32 timedCount);
Sint32 fdWorldMapGetBattleEnvironment(struct WORLD_POS *pos, Sint32 *region);
Sint32 fdSpritePictureCacheLoad(
	Uint32 slot, Uint32 *buf, Sint32 number, Uint32 variant);
void fdMapGridSetMetatileBehaviorAt(
	Uint32 layer, FIXED x, FIXED y, Uint32 behavior);
void   fdSpriteSetAnimationSpeed(SPR *sprite, Sint32 step);
void   fdSpriteColorRemapPixels(Uint8 *bytes, Sint32 byteCount);
void   fdTilesetAnimationSuspendAll(void);
void   fdTilesetAnimationResumeAll(void);
ACTOR *fdActorScriptFindOverlappingActor(ACTOR *actor, Sint32 *values);
Sint32 fdMapGridGetTerrainKindAt(Sint32 layer, FIXED x, FIXED y);
void   fdSpriteViewerRun(void);

Uint8		  *fdDecompressLZ(const Uint8 *src, Uint8 *dest);
Sint32		   fdSpriteComposePicture(SPR *sprite, Uint16 direction);
Sint32		   fdSpritePictureCacheFindByTag(Sint32 tag);
SPR_ANIMATION *fdSpriteAnimationAllocate(Sint32 id);
Sint32		   fdActorScriptSetFlagAndTest(ACTOR *interpreter);
Sint32		   fdActorScriptClearFlagAndTest(ACTOR *interpreter);
/* The state values of SC_SET, SC_ADD and SC_CMP, one handler each
   (ActorScriptOperandHandlerTable, field/field_script_commands.c): by
   operation, each sets one of the actor's fields to value, adds value to it, or
   compares the two into conditionResult. */
void fdActorScriptSetOrCompareScriptAddress(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareFacing(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareX(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareY(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareZ(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareScaleX(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareScaleY(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareVelocityX(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareVelocityY(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareVelocityZ(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSpeedLimit(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareAcceleration(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTargetX(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTargetY(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTargetZ(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareBounce(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareGravity(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTerrainHeight(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSceneWord(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSprite(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSpriteStorageKind(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareMotionFlags(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareArrivalAxis(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareComparisonResult(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSnapToTarget(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareCollisionFlags(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSteeringFlags(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrComparePaused(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareRepeatCount(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTimer(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareUserValue(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareWatchedActor(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareUpdateAddress(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSceneCounter(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSceneByte(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdSpriteViewerSetLayerColorMode(Sint32 slot, Sint32 value);
void fdSpriteViewerSetLayerPriority(Sint32 slot, Sint32 value);

void   fdActorSystemReservedNoOperationA(void);
void   fdActorSystemReservedNoOperationB(void);
Sint32 fdActorReturnTrue(void);
void   fdActorSystemPlaceSprites(void);
Sint32 fdActorCameraReturnTrue(void);
void   fdActorSystemProjectSprites(void);

Sint32 fdActorScriptSetBase(ACTOR *interpreter);
Sint32 fdActorScriptHideSprites(ACTOR *interpreter);
Sint32 fdActorScriptShowSprite(ACTOR *interpreter);
Bool   fdActorIsTargetUnset(ACTOR *actor);
void   fdActorUpdateAllOnField(void);
void   fdActorRunUpdateAllFromHeap(void);
void   fdActorUpdateAllMotion(void);
Sint32 fdActorScriptWait(ACTOR *interpreter);
Bool   fdActorScriptWaitForArrival(ACTOR *interpreter);
Bool   fdActorScriptInvokeCallback(ACTOR *interpreter);
Sint32 fdActorScriptFindLabel(ACTOR *interpreter, Uint32 labelKey);
Sint32 fdActorScriptRepeatOrJump(ACTOR *interpreter);
Sint32 fdActorScriptJumpToLabel(ACTOR *interpreter);
Sint32 fdActorScriptJumpIfTrue(ACTOR *interpreter);
Sint32 fdActorScriptJumpIfFalse(ACTOR *interpreter);
Sint32 fdActorScriptEnd(ACTOR *interpreter);
Sint32 fdActorScriptTestFlag(ACTOR *interpreter);
Sint32 fdActorScriptToggleFlagAndTest(ACTOR *interpreter);
Sint32 fdActorScriptSetAnimation(ACTOR *interpreter);
Sint32 fdActorScriptRelease(ACTOR *interpreter);
Sint32 fdActorScriptNoOperation(ACTOR *interpreter);
Sint32 fdActorScriptPlaySound(ACTOR *interpreter);
Sint32 fdActorScriptSetPosAndResetMotion(ACTOR *interpreter);
Sint32 fdActorScriptApplyAbsolutePos(ACTOR *interpreter);
Sint32 fdActorScriptApplyRelativePos(ACTOR *interpreter);
Sint32 fdActorScriptStoreAngleToWatchedActor(ACTOR *interpreter);
Sint32 fdActorScriptApplyWatchedActorPos(ACTOR *interpreter);
Sint32 fdActorScriptApplyLocalOffsetPos(ACTOR *interpreter);
Sint32 fdCameraFollowLeaderInBounds(ACTOR *interpreter);
Bool   fdActorApproachWatchedActor(ACTOR *interpreter);
Bool   fdActorWander(ACTOR *interpreter);
Sint32 fdActorScriptWanderNearHome(ACTOR *interpreter);

void fdActorScriptSetOrCompareCursor(
	struct SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareRadius(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareWait(
	struct SCRIPT_OPERANDS *state, Sint32 operation, Sint32 value);
Sint32 fdActorScriptApplyOperandSet(ACTOR *interpreter);
Sint32 fdActorScriptApplyOperandAdd(ACTOR *interpreter);
Sint32 fdActorScriptApplyOperandCompare(ACTOR *interpreter);

void   fdActorSetUpdateValue(void *rawActor, Sint32 value);
Uint32 fdRequestShortcut(Uint32 encodedAssignment);
Sint32 fdCheckConfiguredKeys(void);
Sint32 fdCheckSpheresOverlap(
	FIXED *firstPos, Sint32 radiusA, FIXED *secondPos, Sint32 radiusB);
Sint32 fdActorUpdatePlayerControl(ACTOR *interpreter);
Sint32 fdActorMoveOnWorldMap(ACTOR *interpreter);
Sint32 fdCheckConfiguredKeysAndCount(ACTOR *interpreter);
Sint32 fdActorMoveByKeys(ACTOR *interpreter);

void fdTileMapDecodeStagedBuffer(Sint32 size);
void fdTileMapNumDummyMetatileTiles(void);
void fdTileMapConvertBuffer(void);
void fdMapGridRenderMetatileTilesRow(Uint32 layer, Sint32 tileX, Sint32 tileY);
void fdMapGridRenderMetatileTilesColumn(
	Uint32 layer, Sint32 tileX, Sint32 tileY);
void fdMapGridUpdateLayerScroll(void);

Sint32 fdWorldMapWriteLayerMetatileTile(
	Sint32 layer, Sint32 x, Sint32 y, Sint32 tile, Sint32 update);
void fdWorldMapApplyAffinePage(void);
void fdWorldMapDrawMetatileRow(Uint32 tileRow);
void fdWorldMapDrawMetatileColumn(Uint32 tileColumn);
void fdWorldMapPreloadCurrentTileBlock(void);
void fdWorldMapPresentStillImage(void);
void fdWorldMapEndStillImage(void);
void fdTilesetAnimationUpdate(void);
void fdTilesetAnimationStartAll(const Uint16 *script);
void fdBlendScriptRun(void);
void fdBlendScriptStart(Uint16 *script);

void   fdPaletteCycleInit(void);
Sint32 fdPaletteCycleAdd(
	Sint16 paletteNumber, Sint16 firstColor, Sint16 delay, Sint16 colorCount);
void fdPaletteCycleStop(void);
void fdPaletteCycleStart(void);
void fdPaletteCycleStep(void);

FIXED fdTerrainGetFlatHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
Sint32 fdTerrainGetSlopeXHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
Sint32 fdTerrainGetSlopeYHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
FIXED fdTerrainGetStepSumDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y);
FIXED fdTerrainGetStepDifferenceDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y);
FIXED fdTerrainGetBendSumDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y);
FIXED fdTerrainGetBendDifferenceDiagonalHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y);
FIXED fdTerrainGetRoundedSquareHeight(
	const Uint8 *heightBytes, Sint32 x, Sint32 y);
Sint32 fdTerrainGetStepXHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
Sint32 fdTerrainGetStepYHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
FIXED fdTerrainGetBendXHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
FIXED fdTerrainGetBendYHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
FIXED fdTerrainGetSampleTableHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
FIXED fdTerrainGetSampleTableMirroredHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
FIXED fdTerrainGetFlatBHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);
FIXED fdTerrainGetFlatCHeight(
	const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);

Sint32 fdMapGridGetMetatileHighFlags(Sint32 x, Sint32 y);
Sint32 fdWorldMapGetCollision(struct WORLD_POS *pos);
Sint32 fdWorldMapIsWalkable(Sint32 unused, struct WORLD_POS *pos);
/* A screen shake's component left as it is; any negative value does. */
#define SHAKE_KEEP (-1)
void fdTileMapDrawRowsFromRam(
	const struct WORLD_MAP_AFFINE_PAIR *lines, Uint8 *pixels);
void fdWorldMapBuildScanlineTable(
	FIXED depth, Sint32 *pos, struct WORLD_MAP_SCANLINE *scanline);

Sint32 fdSpriteViewerStepSpriteId(Sint32 spriteId, Sint32 direction);
void   fdSpriteViewerLayOutRing(
	  Sint32 centerX, Sint32 centerY, SPR_PLACEMENT *positions);
void fdSpriteViewerStartStaggeredAnimations(
	Sint32 layerIndex, Sint32 animationIndex);
void fdSpriteViewerSetLayerSprite(Sint32 layerIndex, Sint32 spriteId);
void fdSpriteViewerPlaceRing(void);

/* Read as a halfword by fdActorSetOthersPaused: the actor id of the player's
   actor, which it leaves alone. */
extern Uint8 PlayerActorId[];

#endif
