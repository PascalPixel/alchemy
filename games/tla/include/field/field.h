#ifndef ALCHEMY_FIELD_H
#define ALCHEMY_FIELD_H

/* @file */
/* The field module's functions and tables other modules call, and the field */
/* Psynergy's reach. */

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "field/actor.h"
#include "system.h"
#include "resource_ids.h"
#include "field/particle.h"
#include "field/sprite.h"
#include "field/event_commands.h"
#include "tla_edition.h"
#include "field/field_functions.h"

struct ACTOR;
struct SCR_POINT;

extern const FIXED Camera_FixedViewMatrix[];
extern u8 SpriteDissolve_PixelOrder[];
extern const u8 WorldMap_TerrainBehaviorTable[];
extern u8 SpriteColorGreyTable[];
s32 btUnitProjectToScr(s32 unit, s32 *screen);
s32 fdPartyCheckShortcut(u32 packed_id);
void fdActorUpdateAllMotion(void);
void fdActorUpdateAllOnField(void);
void fdActorSystemProjectSprs(void);
s32 fdActorFindLastActiveId(void);
void btMotionProjectPosStub(s32, s32 *);
s32 gmUnitKnowsPsynergyStub(s32 group, s32 entry);
s32 gmPartyShowJoinedMessageStub(s32 event_id, s32 value);
s32 gmPartyShowPairJoinedMessageStub(s32 first, s32 second);

/* fdSprDissolveStep clears one pixel of each 8-bit tile a step, in */
/* SpriteDissolve_PixelOrder's order, so a tile is gone after this many. */
#define DISSOLVE_TILE_PIXELS 64
/* The step the first tile starts to dissolve at; each tile after it starts a */
/* step sooner. */
#define DISSOLVE_FIRST_STEP 64
/* The steps a dissolve takes, four a frame: in the field, fdSprDissolveRun */
/* and fdSprDissolveRunMany's, and in battle a felled enemy's (PLAY_FELLED). */
#define DISSOLVE_STEPS (DISSOLVE_FIRST_STEP + DISSOLVE_TILE_PIXELS)

struct SPR;
struct PARTICLE;
struct SCENE_ENTRANCE;
struct TRIGGER;
struct ACTOR_PLACE;
struct SCENE_REGION;

extern u32 Emote_PopTiltedScript[];
extern u32 SceneExits[];
/* The actors fdActorTableSnapshot saved, up to 32, after their slot numbers. */
extern ACTOR SnapshotObjects[];
extern u8 LinkLobby_SlotColumns[];
extern s32 LinkLobby_SlotValues[];
extern const u8 Actor_BlinkSpriteColors[4];
extern const u8 Actor_StepAngleScript[];
extern u16 ActorScript_IdleScript[];
s32 fdPaletteFadeSetTargetStub(s32, s32);
void fdPsynergyFinishMindRead(void);
void fdPsynergyDispatch(void);
void fdPsynergyRun(void);
void fdEncounterSelectBattleMusic(s32 row, s32 column);
s32 fdPaletteFadeStartInterpolationStub(s32);
s32 btMotionProjectScaledPosStub(s32, s32 *);
struct BATTLE_EFFECT_PARM;
void btEffectRunFlurry(struct BATTLE_EFFECT_PARM *effect, s32 variant);
void fdActorTurnPairToFaceEachOther(ACTOR *first, ACTOR *second);
void fdPsynergyRunExtraHandler(void);

s32 fdActorPlaySndInViewStub(ACTOR *interpreter, s32 value);
void fdActorSetSquareSpr(u32 objectId);
void fdSuperLuckyDiceRun(void);
void syGameFlagRefreshLureCapStub(void);
void fdMapGridCenterOnFollowedPointStub(void);
FIXED fdMapGridGetTerrainHeightStub(s32 layer, FIXED x, FIXED z);
s32 fdActorIsTargetUnset(ACTOR *actor);
void scSetActorSprPriority(ACTOR *actor, s32 priority);
void fdActorStartCameraStub(void *actor, void *effect);
void fdEntranceEndArrivalAnimation(s32 effect);
void fdExitPrepareAnimation(s32 effect);
s32 gmPartyAddMemberStub(s32 event_id);
void gmPartyRemoveMemberStub(s32 owner);

/* The depth slot fdRenderApplyProjectedPlacement gives an actor's sprite on a
 */
/* scrolling map: its screen row on from this slot, so a sprite lower on the */
/* screen draws in front and those up to this many rows above it keep their */
/* order; its shadow takes two slots behind. */
#define PLACEMENT_IMAGE_DEPTH 62

void scSetArrivalFlags(s32 unused);
s32 mnMessageOpenWindowStub(s32 message, s32 x, s32 y, s32 packed);
void mnAreaNameWindowHideStub(void);
s32 mnWindowIsClosedStub(s32);

/* How far a field Psynergy reaches from its caster, one row a kind in */
/* FieldPsynergy_Reach, ended by -1: fdMeasurePsynergyHeight and */
/* fdMeasurePsynergyReach read it. */
typedef struct FIELD_PSYNERGY_REACH {
	s16 fieldPsynergy; /* FIELD_PSY; -1 ends the table */
	s16 range;		   /* pixels over the ground */
	s16 height;		   /* pixels above or below the caster */
	s16 width;		   /* either side of the caster's facing, 1/65536 turns */
} FIELD_PSYNERGY_REACH;

extern const FIELD_PSYNERGY_REACH FieldPsynergy_Reach[];
/* Added to a field Psynergy's kind, the FieldPsynergy_Reach row for the */
/* actors its shaking reaches (Tremor's, */
/* REACH_SHAKEN_OBJECTS + FIELD_PSYNERGY_TREMOR). */
#define REACH_SHAKEN_OBJECTS 256

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

/* The sprite id in fdActorCreate's id, below its ACTOR_CREATE (id / */
/* 4096). */
#define ACTOR_CREATE_SPRITE_MASK 0xfff
/* An fdActorCreate id of ACTOR_CREATE `kind`, above its sprite. */
#define ACTOR_CREATE_KIND(kind) ((kind) << 12)

/* The world map's still image (story.s): its tiles, packed (tagged LZ). */
extern u8 WorldMapPackedTiles[];
/* The still image's tile map, packed (tagged LZ), decoded after its tiles. */
extern u8 WorldMapPackedPictures[];
/* The still image's palettes, one for each of its modes. */
extern u8 WorldMapPalettes[];

struct TRIGGER_VIEW;

typedef void (*DRAW_FN)(const void *src, void *dst, s32 param);

typedef void (*UPLOAD_FN)(const void *src, u32 w, u32 h, void *vram);

typedef void (*OUTLINE_FN)(u32 w, u32 h, void *pixels, u32 edge, u32 fill);

/* An actor script command's handler: it takes the actor and returns nonzero */
/* to go on with the script this frame, zero to stop. The handlers take */
/* different views of the actor, so the type leaves the parameters */
/* unspecified. */
typedef s32 (*SCRIPT_COMMAND_FN)();

typedef void (*TARGET_HOOK)(ACTOR *, s32 *, s32 *);

/* An affine sprite's matrix, of the 32, in its attr01 word. */
#define OBJ_AFFINE_MATRIX(n) ((n) << 25)

/* A party member's unit as the field reads it: its gauges and its HP and PP. */
typedef struct FIELD_UNIT {
	u8 unused[20]; /* no code names it */
	s16 hpGauge;
	s16 ppGauge;
	u8 unusedAfterPpGauge[28]; /* no code names it */
	s16 hpMax;
	s16 ppMax;
	s16 hp;
	s16 pp;
} FIELD_UNIT;

/* The World Map's cells in view (syRamMapBlocks, and the blocks in */
/* CellTiles): 32 by 32 cells of 32 pixels, found by a position's cell modulo */
/* 32 each way. */
#define WORLD_MAP_VIEW_CELLS  32
#define WORLD_MAP_CELL_PIXELS 32

/* One 32-pixel cell of the World Map: its tile and its encounter region; */
/* syRamMapBlocks holds WORLD_MAP_VIEW_CELLS by WORLD_MAP_VIEW_CELLS of them. */
typedef struct WORLD_MAP_CELL {
	u32 tile : 24; /* low halfword: the cell tiles */
	/* Passed to fdWorldMapUpdateRegionTint; region / 10 picks the graphics set.
	 */
	u32 region : 6;
	u32 unused : 1; /* no code names it */
	/* A beach: fdWorldMapGetBattleEnvironment fights on it from */
	/* WorldMap_TerrainBehaviorTable's beach bank. */
	u32 beach : 1;
} WORLD_MAP_CELL;

/* A map cell's terrain kind: the low four bits of its shape's first byte, */
/* which pick its height function. */
typedef enum TERRAIN_KIND {
	TERRAIN_FLAT = 0,
	TERRAIN_SLOPE_X = 1,
	TERRAIN_SLOPE_Y = 2,
	TERRAIN_STEP_SUM_DIAGONAL = 3,
	TERRAIN_STEP_DIFFERENCE_DIAGONAL = 4,
	TERRAIN_BEND_SUM_DIAGONAL = 5,
	TERRAIN_BEND_DIFFERENCE_DIAGONAL = 6,
	TERRAIN_ROUNDED_SQUARE = 7,
	TERRAIN_STEP_X = 8,
	TERRAIN_STEP_Y = 9,
	/* As TERRAIN_SAMPLE_TABLE with x and y swapped. */
	TERRAIN_SAMPLE_TABLE_TRANSPOSED = 10,
	/* As TERRAIN_SAMPLE_TABLE_MIRRORED with x and y swapped. */
	TERRAIN_SAMPLE_TABLE_TRANSPOSED_MIRRORED = 11,
	/* The sample Terrain_SampleIndexTable picks for each pixel. */
	TERRAIN_SAMPLE_TABLE = 12,
	/* As TERRAIN_SAMPLE_TABLE, mirrored left to right. */
	TERRAIN_SAMPLE_TABLE_MIRRORED = 13,
	/* Above y 8, the first sample left of x 8 and the second right of it;
	   below, */
	/* the third. */
	TERRAIN_SPLIT_TOP_STEP = 14,
	/* Above y 8, the first sample; below, the second left of x 8 and the third
	 */
	/* right of it. */
	TERRAIN_SPLIT_BOTTOM_STEP = 15,
	TERRAIN_KIND_COUNT = 16,
	/* Not a shape's kind: what fdMapGridGetTerrainKindOrBumpAt gives */
	/* for a TERRAIN_ROUNDED_SQUARE whose two samples differ, a bump or a
	   hollow. */
	TERRAIN_BUMP = 99
} TERRAIN_KIND;

/* What fdMapGridCheckCollision says of a step to a position. */
typedef enum COLLISION {
	COLLISION_TOO_LOW = -1,	 /* the ground there is more than 12 pixels lower */
	COLLISION_NONE = 0,		 /* the actor can step there */
	COLLISION_TOO_HIGH = 1,	 /* the ground there is more than 8 pixels higher */
	COLLISION_IMPASSABLE = 2 /* the cell there is a wall (CELL_BEHAVIOR_WALL) */
} COLLISION;

/* One node of a tightrope (fdTightropeInit's state): a point on the rope, */
/* and the sprite fdTightropeDraw puts between it and the next node. */
typedef struct TIGHTROPE_NODE {
	struct TIGHTROPE_NODE *next;
	s32 x;
	s32 y;
	s32 z;
	s8 kind; /* 0 while free; 1 a node the leader can step onto; 2 coiled */
	s8 slot; /* the tightrope's index plus one while it is thrown, else 0 */
	u8 unusedAfterSlot[2]; /* no code names it */
	ANIMATION_SPR_PART spr;
} TIGHTROPE_NODE;

/* One of the eight tightropes: the two posts it runs between and its wobble. */
typedef struct TIGHTROPE_SLOT {
	s16 first;
	s16 second;
	u32 dir;					/* the sprites' turn, along the tightrope */
	s32 amplitude;				/* the wobble's size, decaying each frame */
	u8 unusedAfterAmplitude[4]; /* no code names it */
	s32 phase;
	s32 wobbling;
	TIGHTROPE_NODE *head;
} TIGHTROPE_SLOT;

/* The ropes', tightropes' and hanging vines' states each keep eight slots, */
/* each a chain of nodes from one pool of 128. */
#define ROPE_SLOT_COUNT 8
#define ROPE_NODE_COUNT 128
/* A squared distance in pixels beyond any rope node, to start a search for */
/* the nearest. */
#define ROPE_NODE_DISTANCE_NONE (1 << 28)

/* The tightropes' state (HEAP_SLOT_TIGHTROPE). */
typedef struct TIGHTROPE_WORK {
	ACTOR
	*recoilFrom; /* the posts fdTightropeUntie gives fdTightropeRecoil */
	ACTOR *recoilTo;
	s32 vramEntry; /* the nodes' picture */
	TIGHTROPE_SLOT slots[ROPE_SLOT_COUNT];
	TIGHTROPE_NODE nodes[ROPE_NODE_COUNT];
} TIGHTROPE_WORK;

#define TIGHTROPE_STATE ((TIGHTROPE_WORK *)Ram_HeapSlots->tightrope)

/* One rising particle: its sprite, world position, drift and frames left. */
typedef struct RISING_PARTICLE {
	/* The sprite: set as three words by the start, drawn field by field. */
	union {
		u32 words[3];
		struct {
			u8 unusedAfterWords[4]; /* no code names it */
			/* OAM attributes 0 to 2. */
			/* @{ */
			u16 y : 8; /* screen pixels */
			u16 attr0 : 2;
			u16 objMode : 2;
			u16 attr0b : 2;
			u16 shape : 2;
			u16 x : 9; /* screen pixels */
			u16 attr1 : 5;
			u16 size : 2;
			u16 tile : 10;
			u16 pri : 2;
			u16 pal : 4;
			/* @} */
			u8 unusedAfterPal[2]; /* no code names it */
		} attributes;
	} spr;
	s32 posX;
	s32 posY;
	s32 posZ;
	s32 drift;
	u16 tmr;
	u8 unusedAfterTmr[2]; /* no code names it */
} RISING_PARTICLE;

/* HEAP_SLOT_WEATHER while the rising particles run: their tiles' VRAM cache */
/* entry and first tile, and the particles. */
typedef struct RISING_PARTICLE_STATE {
	s32 vramEntry;
	s32 tile;
	RISING_PARTICLE particles[64];
} RISING_PARTICLE_STATE;

/* An actor (ACTOR) as the Mars Djinni's sparks read the leader: its */
/* position and its sprite, typed. */
typedef struct EFFECT_OWNER {
	u8 unused[8]; /* no code names it */
	VECTOR3 pos;
	u8 unusedAfterPos[60]; /* no code names it */
	SPR *spr;
} EFFECT_OWNER;

/* The line scroll's state: two pages of BG1 to BG3 scroll offsets for each of
 */
/* the 160 scanlines, and the sine wave that bends them on each axis. */
typedef struct LINE_SCROLL_STATE {
	BG_SCROLL rows[2][160][3]; /* per page and scanline: BG3, BG2, BG1 */
	u8 page; /* the page the HBlank DMA shows; the other is built */
	u8 mode; /* set at the start; not read by name */
	u16 frm; /* frames built so far, added to BG3's y for the wave's phase */
	u8 unused[4]; /* zeroed with the block; nothing reads or writes it */
	FIXED freqX; /* phase per frame and per BG3 row, 16.16 wave table entries */
	FIXED freqY; /* phase per frame and per BG3 row, 16.16 wave table entries */
	FIXED stepX; /* 16.16 wave table entries a scanline, 256 a cycle */
	FIXED stepY; /* 16.16 wave table entries a scanline, 256 a cycle */
	FIXED
	ampX; /* 16.16 scale on the sine entry, which is then divided by 256 */
	FIXED
	ampY; /* 16.16 scale on the sine entry, which is then divided by 256 */
} LINE_SCROLL_STATE;

/* ROPE_NODE.kind and TIGHTROPE_NODE.kind. */
typedef enum ROPE_NODE_KIND {
	ROPE_NODE_FREE,		 /* in the pool, on no rope */
	ROPE_NODE_CROSSABLE, /* laid along a rope, for the leader to step onto */
	/* Laid but not to be crossed: a coil, a tied rope's end wound round its far
	 */
	/* post, every node of a settled tightrope. */
	ROPE_NODE_UNCROSSABLE
} ROPE_NODE_KIND;

/* One node of a rope Lash throws and ties (fdRopeInit's state): a point */
/* on the rope, and the sprite fdRopeDraw puts between it and the next node. */
typedef struct ROPE_NODE {
	struct ROPE_NODE *next;
	s32 x;
	s32 y;
	s32 z;
	u16 ang; /* the sprite's turn, along the rope */
	s8 kind; /* ROPE_NODE_KIND */
	s8 slot; /* the rope's index plus one while it is thrown, else 0 */
	ANIMATION_SPR_PART spr;
} ROPE_NODE;

/* One of the eight ropes: the two posts it runs between and its swing. */
typedef struct ROPE_SLOT {
	s16 first;	  /* the post it coils at */
	s16 second;	  /* the post it is tied to */
	s16 segments; /* its nodes, less one */
	s16 pri;	  /* the sprites', the first post's */
	s16 flg;	  /* set while the rope is tied */
	s16 state;	  /* 1 once thrown or tied */
	s32 amplitude;
	u8 unusedAfterAmplitude[4]; /* no code names it */
	s32 phase;
	s32 swinging;
	ROPE_NODE *head;
} ROPE_SLOT;

/* The ropes' state (HEAP_SLOT_ROPE). */
typedef struct ROPE_WORK {
	ACTOR *recoilFrom; /* the posts fdRopeUntie gives fdRopeRecoil */
	ACTOR *recoilTo;
	s32 vramEntry; /* the nodes' picture */
	ROPE_SLOT slots[ROPE_SLOT_COUNT];
	ROPE_NODE nodes[ROPE_NODE_COUNT];
} ROPE_WORK;

#define ROPE_STATE ((ROPE_WORK *)Ram_HeapSlots->rope)

extern const u8 SuperLuckyDice_FaceTiles[];
s32 fdSuperLuckyDiceScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
									  s32 count);
void fdSuperLuckyDiceBrightenCols(u16 *source, u16 *destination, u32 count,
								  s32 angle);
s32 fdSuperLuckyDiceScoreDice(s32 *dice);
s32 fdSuperLuckyDiceDrawNum(s32 sprite, s32 x, s32 y, s32 value, s32 palette,
							s32 digits);
s32 fdSuperLuckyDiceDrawLargeNum(s32 sprite, s32 x, s32 y, s32 value,
								 s32 palette, s32 digits);

void mnPortraitCloseWindowStub(s32 sprite);

extern const s32 Growth_LeafScript[];
extern const s32 FieldEffect_ShortEffectScript[];
extern const s32 FieldEffect_MediumEffectScript[];
extern const s32 FieldEffect_LongEffectScript[];

extern const s32 FieldEffect_FragmentScript[];

s32 fdMapGridCheckCollisionStub(ACTOR *actor, s32 *position);

s32 fdCheckSpheresOverlapStub(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

void fdMapGridSetScrShakeStub(s32 x, s32 y, s32 decay);

s32 fdMeasurePsynergyHeight(s32 id, ACTOR *actor, s32 *position);

u32 fdMapGridGetTerrainKindStub(s32 terrain, s32 x, s32 z);

void fdActorUpdateBlink(ACTOR *actor);

/* Every sprite animation's metadata, by number (graphics/sprite/sprites.s). */
extern ANIMATION_METADATA Sprite_AnimationMetadata[];

/* Render_DecodePicture's Thumb entry and ARM code (picture_decode.s), which */
/* btRenderLoadDecoder copies into the frame scratch slot. */
extern u8 Render_DecodePictureCode[];

/* The end of the colour tables kept after Render_DecodePicture's code, where */
/* its copy stops (picture_decode_data.s). */
extern u8 Render_DecodePictureCopyEnd[];

/* The size of the frame scratch block for Render_DecodePicture and its tables,
 */
/* an address equal to the size (ld_script.ld). */
extern u8 Render_DecodePictureBlockSize[];

/* The grid outliner's Thumb entry and ARM code (grid_outline_entry.s), which */
/* syRamCodeLoad copies to RAM. */
extern const u8 fdGridDrawOutlineCode[];

/* The size of fdGridDrawOutlineCode, an address equal to the size */
/* (ld_script.ld). */
extern u8 Grid_DrawOutlineCodeSize[];

extern const u8 SpriteColorTintTables[];

u8 *fdDecompressLz(const u8 *source, u8 *destination);

s32 scWorldMapGetCollisionStub(s32 *position);

void fdBattleBackgroundSelectForMap(void);

void fdActorTableClear(void);

/* fdPartyAddPp's amount that fills every member's PP: "Everyone's PP was */
/* replenished!" */
#define PARTY_PP_REPLENISH 999
s32 fdPartyApplyPoisonDamage(void);
ACTOR *fdGetPartyActor(void);

u32 fdStreamReadU16(u8 **cursor);
s16 fdStreamReadS16(u8 **cursor);
void fdApplyEntrance(void);

void fdPaletteEditorRun(void);

s32 scRunRaisedTrigger(s32 id);
s32 scRunStepTriggerOrExit(s32 value);
s32 fdRunPushCell(s32 keys);
s32 fdExamineCell(s32 id);

s32 fdUsePsynergy(u32 code);

s16 *fdSceneItemGetRows(void);
void fdPlaceTreasureSpots(void);
s32 fdActorGetSprShape(u32 objectId);
void fdTreasureSpotLaunchMimic(u32 objectId);
void fdTreasureSpotOpenEmptyChest(u32 objectId);

void fdCameraResetViewDefaults(void);
s32 fdLuckyDiceScaleRgb555Buffer(u16 *source, u16 *destination, FIXED scale,
								 s32 count);

extern const u16 LuckyWheels_SparkCellOffsets[];

s32 btEffectBuildBlitterStub(s32 slot, s32 widthShift, s32 heightShift,
							 s32 flags, u32 mode);

s32 fdWorldMapWriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void fdPaletteFadeLoadPaletteStub(void *palette);

void fdTilesetAnimStartAll(const u16 *script);

void mnIconDrawItemStub(s32 item);

s32 fdSceneItemFindPos(s32 value, s32 *out);

void fdMapGridSetCellBehaviorAtStub(s32 terrain, s32 x, s32 z, s32 value);

void fdSprAnimationClearStub(void *record);

void gmDjinnShowJoinedMessageStub(s32 owner, s32 element, s32 djinni);

/* The rope's tiles, tagged LZ (rope_vine_pictures.s). */
extern const u8 Rope_Tiles[];
/* The bottom link of a hanging vine's chain, tagged LZ (rope_vine_pictures.s).
 */
extern const u8 HangingVine_ChainBottomPicture[];

s32 fdMapGridGetTerrainKindAtStub(s32 layer, s32 x, s32 z);

/* The ground particles' tiles, byte LZ (field_effect_particles.s). */
extern const u8 Weather_ParticleTiles[];
/* The motes' tiles, byte LZ (field_effect_particles.s). */
extern const u8 FieldEffect_MoteTiles[];

extern const u32 FieldEffect_PulseScales[];

extern const s16 LineScroll_WaveSine[];

extern const s16 Weather_RainSplashFrames[];

/* The streaks' tiles, byte LZ (field_effect_particles.s). */
extern const u8 FieldEffect_StreakTiles[];

extern const SPARKLE_PICTURE FieldEffect_SparklePictures[];

/* The sparkles' tiles, byte LZ (field_effect_particles.s). */
extern const u8 FieldEffect_SparkleTiles[];

void fdActorSpawnWaterSplashStub(ACTOR *actor, s32 kind);

s32 fdMapGridGetCellBehaviorAtStub(s32 layer, s32 x, s32 z);

s32 fdMapGridIsWallAtStub(s32 *position);

extern const u16 Rope_KeyHeadings[];
extern const u16 Input_DpadAngles[];

extern const s16 GroundParticles_Offsets[];

void fdActorMoveWithCarriers(void);

void fdActorSystemPlaceSprs(void);

extern const s32 Actor_KeyCheckScript[];
extern const s32 Actor_KeysScript[];
extern const s32 Actor_RopeScript[];
extern const s32 Actor_TightropeScript[];
extern const s32 Actor_SandScript[];
extern const s32 Actor_HoverScript[];
extern const s32 Actor_WalkingScript[];
extern const s32 Actor_PlayerScript[];
extern const s32 Actor_WorldMapScript[];
extern const s32 Actor_SailOnMapScript[];
extern const s32 Actor_SailScript[];
extern const s32 Actor_FlyScript[];
extern const s32 Actor_SandOnWorldMapScript[];
extern const s32 Actor_ApproachWatchedScript[];

s32 fdCheckConfiguredKeys(void);

void fdTileMapDecodeStagedBuffer(s32 size);
void fdTileMapConvertBuffer(void);

void fdMapGridUpdateLayerScroll(void);

void fdWorldMapApplyAffinePage(void);

u32 fdMapReadEditU16(u8 **cursor);
void fdMapApplyFlagEdits(s32 flag, s32 attributes);

void fdWorldMapMarkPlaceVisited(s32 id, s32 variant);

#endif
