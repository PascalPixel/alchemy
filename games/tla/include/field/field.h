#ifndef ALCHEMY_FIELD_H
#define ALCHEMY_FIELD_H

/* The field module's functions and tables other modules call, and the field
   Psynergy's reach. */

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "field/actor.h"
#include "system.h"
#include "resource_ids.h"
#include "field/particle.h"
#include "field/sprite.h"
#include "field/script_commands.h"
#include "tla_edition.h"
#include "field/field_functions.h"

struct ACTOR;
struct SCREEN_POINT;

extern const FIXED Camera_FixedViewMatrix[];
extern Uint8	   SpriteDissolve_PixelOrder[];
extern const Uint8 WorldMap_TerrainBehaviorTable[];
extern Uint8	   SpriteColorGreyTable[];
Sint32			   btUnitProjectToScreen(Sint32 unit, Sint32 *screen);
Sint32			   fdPartyCheckShortcut(Uint32 packed_id);
void			   fdActorUpdateAllMotion(void);
void			   fdActorUpdateAllOnField(void);
void			   fdActorSystemProjectSprs(void);
Sint32			   fdActorFindLastActiveId(void);
void			   btMotionProjectPosStub(Sint32, Sint32 *);
Sint32			   gmUnitKnowsPsynergyStub(Sint32 group, Sint32 entry);
Sint32			   mnPartyShowJoinedMessageStub(Sint32 event_id, Sint32 value);
Sint32 mnPartyShowPairJoinedMessageStub(Sint32 first, Sint32 second);

/* fdSprDissolveStep clears one pixel of each 8-bit tile a step, in
   SpriteDissolve_PixelOrder's order, so a tile is gone after this many. */
#define DISSOLVE_TILE_PIXELS 64
/* The step the first tile starts to dissolve at; each tile after it starts a
   step sooner. */
#define DISSOLVE_FIRST_STEP	 64
/* The steps a dissolve takes, four a frame: in the field, fdSprDissolveRun and
   fdSprDissolveRunMany's, and in battle a felled enemy's (PLAY_FELLED). */
#define DISSOLVE_STEPS		 (DISSOLVE_FIRST_STEP + DISSOLVE_TILE_PIXELS)

struct SPR;
struct PARTICLE;
struct SCENE_ENTRANCE;
struct CUE;
struct ACTOR_PLACE;
struct SCENE_REGION;

extern Uint32 Emote_PopTiltedScript[];
extern Uint32 SceneExits[];
/* The actors fdActorTableSnapshot saved, up to 32, after their slot numbers. */
extern ACTOR	   SnapshotObjects[];
extern Uint8	   LinkLobby_SlotColumns[];
extern Sint32	   LinkLobby_SlotValues[];
extern const Uint8 Actor_BlinkSpriteColors[4];
extern const Uint8 Actor_StepAngleScript[];
extern Uint16	   ActorScript_IdleScript[];
Sint32			   fdPaletteFadeSetTargetStub(Sint32, Sint32);
void			   fdPsynergyFinishMindRead(void);
void			   fdPsynergyDispatch(void);
void			   fdPsynergyRun(void);
void			   fdEncounterSelectBattleMusic(Sint32 row, Sint32 column);
Sint32			   fdPaletteFadeStartInterpolationStub(Sint32);
Sint32			   btMotionProjectScaledPosStub(Sint32, Sint32 *);
struct BATTLE_EFFECT_PARM;
void btEffectRunFlurry(struct BATTLE_EFFECT_PARM *effect, Sint32 variant);
void fdActorTurnPairToFaceEachOther(ACTOR *first, ACTOR *second);
void fdPsynergyRunExtraHandler(void);

Sint32 fdActorPlaySndInViewStub(ACTOR *interpreter, Sint32 value);
void   fdActorSetSquareSpr(Uint32 objectId);
void   fdSuperLuckyDiceRun(void);
void   gmFlagRefreshLureCapStub(void);
void   fdMapGridCenterOnFollowedPointStub(void);
FIXED  fdMapGridGetTerrainHeightStub(Sint32 layer, FIXED x, FIXED z);
Bool   fdActorIsTargetUnset(ACTOR *actor);
void   fdSetActorSprPriority(ACTOR *actor, Sint32 priority);
void   fdActorStartCameraStub(void *actor, void *effect);
void   fdEntranceEndArrivalAnimation(Sint32 effect);
void   fdExitPrepareAnimation(Sint32 effect);
Sint32 gmPartyAddMemberStub(Sint32 event_id);
void   gmPartyRemoveMemberStub(Sint32 owner);

/* The depth slot fdRenderApplyProjectedPlacement gives an actor's sprite on a
   scrolling map: its screen row on from this slot, so a sprite lower on the
   screen draws in front and those up to this many rows above it keep their
   order; its shadow takes two slots behind. */
#define PLACEMENT_IMAGE_DEPTH 62

void   scSetArrivalFlags(Sint32 unused);
Sint32 mnMessageOpenWindowStub(
	Sint32 message, Sint32 x, Sint32 y, Sint32 packed);
void   mnAreaNameWindowHideStub(void);
Sint32 mnWindowIsClosedStub(Sint32);

/* How far a field Psynergy reaches from its caster, one row a kind in
   FieldPsynergy_Reach, ended by -1: fdMeasurePsynergyHeight and
   fdMeasurePsynergyReach read it. */
typedef struct FIELD_PSYNERGY_REACH {
	Sint16 fieldPsynergy; /* FIELD_PSY; -1 ends the table */
	Sint16 range;		  /* pixels over the ground */
	Sint16 height;		  /* pixels above or below the caster */
	Sint16 width; /* either side of the caster's facing, 1/65536 turns */
} FIELD_PSYNERGY_REACH;

extern const FIELD_PSYNERGY_REACH FieldPsynergy_Reach[];
/* Added to a field Psynergy's kind, the FieldPsynergy_Reach row for the actors
   its shaking reaches (Tremor's, REACH_SHAKEN_OBJECTS +
   FIELD_PSYNERGY_TREMOR). */
#define REACH_SHAKEN_OBJECTS 256

/* ACTOR.sprStorageKind, its low four bits: what the actor's sprite pointer
   holds. */
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
	/* No sprite: any kind but the two above, and the one the camera actor
	   takes. */
	ACTOR_CREATE_NO_SPRITE = 8
} ACTOR_CREATE;

/* The sprite id in fdActorCreate's id, below its ACTOR_CREATE (id / 4096). */
#define ACTOR_CREATE_SPRITE_MASK 0xfff
/* An fdActorCreate id of ACTOR_CREATE `kind`, above its sprite. */
#define ACTOR_CREATE_KIND(kind)	 ((kind) << 12)

/* The world map's still image (story.s): its tiles, packed (tagged LZ). */
extern Uint8 WorldMapPackedTiles[];
/* The still image's tile map, packed (tagged LZ), decoded after its tiles. */
extern Uint8 WorldMapPackedPictures[];
/* The still image's palettes, one for each of its modes. */
extern Uint8 WorldMapPalettes[];

struct CUE_VIEW;

typedef void (*DRAW_FN)(const void *src, void *dst, Sint32 param);

typedef void (*UPLOAD_FN)(const void *src, Uint32 w, Uint32 h, void *vram);

typedef void (*OUTLINE_FN)(
	Uint32 w, Uint32 h, void *pixels, Uint32 edge, Uint32 fill);

/* An actor script command's handler: it takes the actor and returns nonzero to
   go on with the script this frame, zero to stop. The handlers take different
   views of the actor, so the type leaves the parameters unspecified. */
typedef Sint32 (*SCRIPT_COMMAND_FN)();

typedef void (*TARGET_HOOK)(ACTOR *, Sint32 *, Sint32 *);

/* An affine sprite's matrix, of the 32, in its attr01 word. */
#define OBJ_AFFINE_MATRIX(n) ((n) << 25)

/* A party member's unit as the field reads it: its gauges and its HP and PP. */
typedef struct FIELD_UNIT {
	Uint8  unused[20]; /* no code names it */
	Sint16 hpGauge;
	Sint16 ppGauge;
	Uint8  unusedAfterPpGauge[28]; /* no code names it */
	Sint16 hpMax;
	Sint16 ppMax;
	Sint16 hp;
	Sint16 pp;
} FIELD_UNIT;

/* The World Map's metatiles in view (syRamMapBlocks, and the blocks in
   MetatileTiles): 32 by 32 metatiles of 32 pixels, found by a position's
   metatile modulo 32 each way. */
#define WORLD_MAP_VIEW_METATILES  32
#define WORLD_MAP_METATILE_PIXELS 32

/* One 32-pixel metatile of the World Map: its tile and its encounter region;
   syRamMapBlocks holds WORLD_MAP_VIEW_METATILES by WORLD_MAP_VIEW_METATILES of
   them. */
typedef struct WORLD_MAP_METATILE {
	Uint32 tile : 24; /* low halfword: the metatile tiles */
	/* Passed to fdWorldMapUpdateRegionTint; region / 10 picks the graphics
	   set. */
	Uint32 region : 6;
	Uint32 unused : 1; /* no code names it */
	/* A beach: fdWorldMapGetBattleEnvironment fights on it from
	   WorldMap_TerrainBehaviorTable's beach bank. */
	Uint32 beach : 1;
} WORLD_MAP_METATILE;

/* A map metatile's terrain kind: the low four bits of its shape's first byte,
   which pick its height function. */
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
	   below, the third. */
	TERRAIN_SPLIT_TOP_STEP = 14,
	/* Above y 8, the first sample; below, the second left of x 8 and the third
	   right of it. */
	TERRAIN_SPLIT_BOTTOM_STEP = 15,
	TERRAIN_KIND_COUNT = 16,
	/* Not a shape's kind: what fdMapGridGetTerrainKindOrBumpAt gives for a
	   TERRAIN_ROUNDED_SQUARE whose two samples differ, a bump or a hollow. */
	TERRAIN_BUMP = 99
} TERRAIN_KIND;

/* What fdMapGridCheckCollision says of a step to a position. */
typedef enum COLLISION {
	COLLISION_TOO_LOW = -1, /* the ground there is more than 12 pixels lower */
	COLLISION_NONE = 0,		/* the actor can step there */
	COLLISION_TOO_HIGH = 1, /* the ground there is more than 8 pixels higher */
	/* the metatile there is a wall (METATILE_BEHAVIOR_WALL) */
	COLLISION_IMPASSABLE = 2
} COLLISION;

/* One node of a tightrope (fdTightropeInit's state): a point on the rope, and
   the sprite fdTightropeDraw puts between it and the next node. */
typedef struct TIGHTROPE_NODE {
	struct TIGHTROPE_NODE *next;
	Sint32				   x;
	Sint32				   y;
	Sint32				   z;
	Sint8 kind; /* 0 while free; 1 a node the leader can step onto; 2 coiled */
	Sint8 slot; /* the tightrope's index plus one while it is thrown, else 0 */
	Uint8 unusedAfterSlot[2]; /* no code names it */
	ANIMATION_SPR_PART spr;
} TIGHTROPE_NODE;

/* One of the eight tightropes: the two posts it runs between and its wobble. */
typedef struct TIGHTROPE_SLOT {
	Sint16			first;
	Sint16			second;
	Uint32			dir;	   /* the sprites' turn, along the tightrope */
	Sint32			amplitude; /* the wobble's size, decaying each frame */
	Uint8			unusedAfterAmplitude[4]; /* no code names it */
	Sint32			phase;
	Sint32			wobbling;
	TIGHTROPE_NODE *head;
} TIGHTROPE_SLOT;

/* The ropes', tightropes' and hanging vines' states each keep eight slots, each
   a chain of nodes from one pool of 128. */
#define ROPE_SLOT_COUNT			8
#define ROPE_NODE_COUNT			128
/* A squared distance in pixels beyond any rope node, to start a search for the
   nearest. */
#define ROPE_NODE_DISTANCE_NONE (1 << 28)

/* The tightropes' state (HEAP_SLOT_TIGHTROPE). */
typedef struct TIGHTROPE_WORK {
	ACTOR
	*recoilFrom; /* the posts fdTightropeUntie gives fdTightropeRecoil */
	ACTOR		  *recoilTo;
	Sint32		   vramEntry; /* the nodes' picture */
	TIGHTROPE_SLOT slots[ROPE_SLOT_COUNT];
	TIGHTROPE_NODE nodes[ROPE_NODE_COUNT];
} TIGHTROPE_WORK;

#define TIGHTROPE_STATE ((TIGHTROPE_WORK *)Ram_HeapSlots->tightrope)

/* One rising particle: its sprite, world position, drift and frames left. */
typedef struct RISING_PARTICLE {
	/* The sprite: set as three words by the start, drawn field by field. */
	union {
		Uint32 words[3];

		struct {
			Uint8 unusedAfterWords[4]; /* no code names it */
			/* OAM attributes 0 to 2. */
			Uint16 y : 8; /* screen pixels */
			Uint16 attr0 : 2;
			Uint16 objMode : 2;
			Uint16 attr0b : 2;
			Uint16 shape : 2;
			Uint16 x : 9; /* screen pixels */
			Uint16 attr1 : 5;
			Uint16 size : 2;
			Uint16 tile : 10;
			Uint16 pri : 2;
			Uint16 pal : 4;
			Uint8  unusedAfterPal[2]; /* no code names it */
		} attributes;
	} spr;

	Sint32 posX;
	Sint32 posY;
	Sint32 posZ;
	Sint32 drift;
	Uint16 tmr;
	Uint8  unusedAfterTmr[2]; /* no code names it */
} RISING_PARTICLE;

/* HEAP_SLOT_WEATHER while the rising particles run: their tiles' VRAM cache
   entry and first tile, and the particles. */
typedef struct RISING_PARTICLE_STATE {
	Sint32			vramEntry;
	Sint32			tile;
	RISING_PARTICLE particles[64];
} RISING_PARTICLE_STATE;

/* An actor (ACTOR) as the Mars Djinni's sparks read the leader: its position
   and its sprite, typed. */
typedef struct EFFECT_OWNER {
	Uint8	unused[8]; /* no code names it */
	VECTOR3 pos;
	Uint8	unusedAfterPos[60]; /* no code names it */
	SPR	   *spr;
} EFFECT_OWNER;

/* The line scroll's state: two pages of BG1 to BG3 scroll offsets for each of
   the 160 scanlines, and the sine wave that bends them on each axis. */
typedef struct LINE_SCROLL_STATE {
	BG_SCROLL rows[2][160][3]; /* per page and scanline: BG3, BG2, BG1 */
	Uint8	  page; /* the page the HBlank DMA shows; the other is built */
	Uint8	  mode; /* set at the start; not read by name */
	Uint16 frm; /* frames built so far, added to BG3's y for the wave's phase */
	Uint8  unused[4]; /* zeroed with the block; nothing reads or writes it */
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
	   post, every node of a settled tightrope. */
	ROPE_NODE_UNCROSSABLE
} ROPE_NODE_KIND;

/* One node of a rope Lash throws and ties (fdRopeInit's state): a point on the
   rope, and the sprite fdRopeDraw puts between it and the next node. */
typedef struct ROPE_NODE {
	struct ROPE_NODE *next;
	Sint32			  x;
	Sint32			  y;
	Sint32			  z;
	Uint16			  ang;	/* the sprite's turn, along the rope */
	Sint8			  kind; /* ROPE_NODE_KIND */
	Sint8 slot; /* the rope's index plus one while it is thrown, else 0 */
	ANIMATION_SPR_PART spr;
} ROPE_NODE;

/* One of the eight ropes: the two posts it runs between and its swing. */
typedef struct ROPE_SLOT {
	Sint16	   first;	 /* the post it coils at */
	Sint16	   second;	 /* the post it is tied to */
	Sint16	   segments; /* its nodes, less one */
	Sint16	   pri;		 /* the sprites', the first post's */
	Sint16	   flg;		 /* set while the rope is tied */
	Sint16	   state;	 /* 1 once thrown or tied */
	Sint32	   amplitude;
	Uint8	   unusedAfterAmplitude[4]; /* no code names it */
	Sint32	   phase;
	Sint32	   swinging;
	ROPE_NODE *head;
} ROPE_SLOT;

/* The ropes' state (HEAP_SLOT_ROPE). */
typedef struct ROPE_WORK {
	ACTOR	 *recoilFrom; /* the posts fdRopeUntie gives fdRopeRecoil */
	ACTOR	 *recoilTo;
	Sint32	  vramEntry; /* the nodes' picture */
	ROPE_SLOT slots[ROPE_SLOT_COUNT];
	ROPE_NODE nodes[ROPE_NODE_COUNT];
} ROPE_WORK;

#define ROPE_STATE ((ROPE_WORK *)Ram_HeapSlots->rope)

extern const Uint8 SuperLuckyDice_FaceTiles[];
Sint32			   fdSuperLuckyDiceScaleRGB555Buffer(
				Uint16 *source, Uint16 *destination, Sint32 scale, Sint32 count);
void fdSuperLuckyDiceBrightenCols(
	Uint16 *source, Uint16 *destination, Uint32 count, Sint32 angle);
Sint32 fdSuperLuckyDiceScoreDice(Sint32 *dice);
Sint32 fdSuperLuckyDiceDrawNum(Sint32 sprite, Sint32 x, Sint32 y, Sint32 value,
	Sint32 palette, Sint32 digits);
Sint32 fdSuperLuckyDiceDrawLargeNum(Sint32 sprite, Sint32 x, Sint32 y,
	Sint32 value, Sint32 palette, Sint32 digits);

void mnPortraitCloseWindowStub(Sint32 sprite);

extern const Sint32 Growth_LeafScript[];
extern const Sint32 FieldEffect_ShortEffectScript[];
extern const Sint32 FieldEffect_MediumEffectScript[];
extern const Sint32 FieldEffect_LongEffectScript[];

extern const Sint32 FieldEffect_FragmentScript[];

Sint32 fdMapGridCheckCollisionStub(ACTOR *actor, Sint32 *position);

Sint32 fdCheckSpheresOverlapStub(
	Sint32 *a, Sint32 radiusA, Sint32 *b, Sint32 radiusB);

void fdMapGridSetScreenShakeStub(Sint32 x, Sint32 y, Sint32 decay);

Sint32 fdMeasurePsynergyHeight(Sint32 id, ACTOR *actor, Sint32 *position);

Uint32 fdMapGridGetTerrainKindStub(Sint32 terrain, Sint32 x, Sint32 z);

void fdActorUpdateBlink(ACTOR *actor);

/* Every sprite animation's metadata, by number (graphics/sprite/sprites.s). */
extern ANIMATION_METADATA Sprite_AnimationMetadata[];

/* fdRenderDecodePicture's Thumb entry and ARM code (picture_decode.s), which
   btRenderLoadDecoder copies into the frame scratch slot. */
extern Uint8 fdRenderDecodePictureCode[];

/* The end of the colour tables kept after fdRenderDecodePicture's code, where
   its copy stops (picture_decode_data.s). */
extern Uint8 fdRenderDecodePictureCopyEnd[];

/* The size of the frame scratch block for fdRenderDecodePicture and its tables,
   an address equal to the size (ld_script.ld). */
extern Uint8 Render_DecodePictureBlockSize[];

/* The grid outliner's Thumb entry and ARM code (grid_outline_entry.s), which
   RAM_CODE_LOAD copies to RAM. */
extern const Uint8 fdGridDrawOutlineCode[];

/* The size of fdGridDrawOutlineCode, an address equal to the size
   (ld_script.ld). */
extern Uint8 Grid_DrawOutlineCodeSize[];

extern const Uint8 SpriteColorTintTables[];

Uint8 *fdDecompressLZ(const Uint8 *source, Uint8 *destination);

Sint32 fdWorldMapGetCollisionStub(Sint32 *position);

void fdBattleBackgroundSelectForMap(void);

void fdActorTableClear(void);

/* fdPartyAddPp's amount that fills every member's PP: "Everyone's PP was
   replenished!" */
#define PARTY_PP_REPLENISH 999
Sint32 fdPartyApplyPoisonDamage(void);
ACTOR *fdGetPartyActor(void);

Uint32 fdStreamReadU16(Uint8 **cursor);
Sint16 fdStreamReadS16(Uint8 **cursor);
void   fdApplyEntrance(void);

void fdPaletteEditorRun(void);

Sint32 fdRunRaisedCue(Sint32 id);
Sint32 fdRunStepCueOrExit(Sint32 value);
Sint32 fdRunPushMetatile(Sint32 keys);
Sint32 fdExamineMetatile(Sint32 id);

Sint32 fdUsePsynergy(Uint32 code);

Sint16 *fdSceneItemGetRows(void);
void	fdPlaceTreasureSpots(void);
Sint32	fdActorGetSprShape(Uint32 objectId);
void	fdTreasureSpotLaunchMimic(Uint32 objectId);
void	fdTreasureSpotOpenEmptyChest(Uint32 objectId);

void   fdCameraResetViewDefaults(void);
Sint32 fdLuckyDiceScaleRGB555Buffer(
	Uint16 *source, Uint16 *destination, FIXED scale, Sint32 count);

extern const Uint16 LuckyWheels_SparkMetatileOffsets[];

Sint32 btEffectBuildBlitterStub(Sint32 slot, Sint32 widthShift,
	Sint32 heightShift, Sint32 flags, Uint32 mode);

Sint32 fdWorldMapWriteLayerMetatileTile(
	Sint32 layer, Sint32 x, Sint32 y, Sint32 tile, Sint32 update);

void fdPaletteFadeLoadPaletteStub(void *palette);

void fdTilesetAnimStartAll(const Uint16 *script);

void mnIconDrawItemStub(Sint32 item);

Sint32 fdSceneItemFindPos(Sint32 value, Sint32 *out);

void fdMapGridSetMetatileBehaviorAtStub(
	Sint32 terrain, Sint32 x, Sint32 z, Sint32 value);

void fdSprAnimationClearStub(void *record);

void mnDjinnShowJoinedMessageStub(Sint32 owner, Sint32 element, Sint32 djinni);

/* The rope's tiles, tagged LZ (rope_vine_pictures.s). */
extern const Uint8 Rope_Tiles[];
/* The bottom link of a hanging vine's chain, tagged LZ
   (rope_vine_pictures.s). */
extern const Uint8 HangingVine_ChainBottomPicture[];

Sint32 fdMapGridGetTerrainKindAtStub(Sint32 layer, Sint32 x, Sint32 z);

/* The ground particles' tiles, byte LZ (field_effect_particles.s). */
extern const Uint8 Weather_ParticleTiles[];
/* The motes' tiles, byte LZ (field_effect_particles.s). */
extern const Uint8 FieldEffect_MoteTiles[];

extern const Uint32 FieldEffect_PulseScales[];

extern const Sint16 LineScroll_WaveSine[];

extern const Sint16 Weather_RainSplashFrames[];

/* The streaks' tiles, byte LZ (field_effect_particles.s). */
extern const Uint8 FieldEffect_StreakTiles[];

extern const SPARKLE_PICTURE FieldEffect_SparklePictures[];

/* The sparkles' tiles, byte LZ (field_effect_particles.s). */
extern const Uint8 FieldEffect_SparkleTiles[];

void fdActorSpawnWaterSplashStub(ACTOR *actor, Sint32 kind);

Sint32 fdMapGridGetMetatileBehaviorAtStub(Sint32 layer, Sint32 x, Sint32 z);

Sint32 fdMapGridIsWallAtStub(Sint32 *position);

extern const Uint16 Rope_KeyHeadings[];
extern const Uint16 Input_DpadAngles[];

extern const Sint16 GroundParticles_Offsets[];

void fdActorMoveWithCarriers(void);

void fdActorSystemPlaceSprs(void);

extern const Sint32 Actor_KeyCheckScript[];
extern const Sint32 Actor_KeysScript[];
extern const Sint32 Actor_RopeScript[];
extern const Sint32 Actor_TightropeScript[];
extern const Sint32 Actor_SandScript[];
extern const Sint32 Actor_HoverScript[];
extern const Sint32 Actor_WalkingScript[];
extern const Sint32 Actor_PlayerScript[];
extern const Sint32 Actor_WorldMapScript[];
extern const Sint32 Actor_SailOnMapScript[];
extern const Sint32 Actor_SailScript[];
extern const Sint32 Actor_FlyScript[];
extern const Sint32 Actor_SandOnWorldMapScript[];
extern const Sint32 Actor_ApproachWatchedScript[];

Sint32 fdCheckConfiguredKeys(void);

void fdTileMapDecodeStagedBuffer(Sint32 size);
void fdTileMapConvertBuffer(void);

void fdMapGridUpdateLayerScroll(void);

void fdWorldMapApplyAffinePage(void);

Uint32 fdMapReadEditU16(Uint8 **cursor);
void   fdMapApplyFlagEdits(Sint32 flag, Sint32 attributes);

void fdWorldMapMarkPlaceVisited(Sint32 id, Sint32 variant);

#endif
