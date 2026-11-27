#ifndef ALCHEMY_FIELD_SCRIPT_COMMANDS_H
#define ALCHEMY_FIELD_SCRIPT_COMMANDS_H

/* What the script commands share with the field and the scenes: the field
   Psynergy's casts and their states, the Djinn's join effects, the map screen's
   state, and the encounter zones of the story's battles. */

#include "types.h"
#include "system.h"
#include "sprite_ids.h"
#include "resource_ids.h"
#include "field/effect_sprite.h"
#include "field/field_particle.h"
#include "field/actor.h"
#include "stddef.h"
#include "static_assert.h"

struct ACTOR;
struct SPR_ANIMATION;
struct VECTOR3;

/* A hue's degrees, a full turn of the colour wheel. */
#define HUE_DEGREES 360

/* Mind Read's wave (heap slot HEAP_SLOT_MIND_READ): each scanline's BG0 scroll
   on two pages, the hue it tints the screen with, its caster and target, and
   its delay and particle cycle. */
typedef struct MIND_READ_STATE {
	Uint16 lines[2][162];
	Uint16 phase;
	Uint8  page;
	Sint8  red;
	Sint8  green;
	Sint8  blue;
	Uint16 hue;
	Uint16 srcId;
	Uint16 tgtId;
	Uint8  delay;
	Uint8  cnt;
} MIND_READ_STATE;

/* MIND_READ_STATE.cnt held still: the wave sends no particles and plays no
   sound, as while Lonesome George carries the party. */
#define MIND_READ_QUIET 99

/* A metatile a Psynergy touched, in metatiles, as FIELD_STATE.psynergyMetatiles
   lists them (x of -1 ends the list). */
typedef struct TOUCH_METATILE {
	Sint16 x;
	Sint16 z;
} TOUCH_METATILE;

/* A metatile's tiles that Cyclone copied (fdTileCopiesCopyTouchedMetatiles):
   where it went, where it came from and the touched metatile it opened, each in
   metatiles. */
typedef struct TOUCH_COPY {
	Uint8 toX;
	Uint8 toZ;
	Uint8 fromX;
	Uint8 fromZ;
	Uint8 metatileX;
	Uint8 metatileZ;
	Uint8 unusedAfterMetatileZ[2]; /* no code names it */
} TOUCH_COPY;

/* The scene's own state (SceneLocalState) as far as the copies of metatiles
   Cyclone blew away, kept there so that fdTileCopiesRestore makes them again.
   It opens with the second copy of the play time (mnSaveBuildFileSummary) and
   the actor snapshot's slot ids and actors (SnapshotObjects, as The Broken
   Seal's ActorSnapshot); the copies lie in InventorySnapshot's spare space. */
typedef struct TOUCH_COPIES {
	Uint32 savedPlayTime;
	Uint8  snapshotSlots[ACTOR_SNAPSHOT_CAPACITY];
	ACTOR  snapshotObjs[ACTOR_SNAPSHOT_CAPACITY];
	/* the snapshot's sprite state, RareItemStock, SceneStateWork and the start
	   of InventorySnapshot */
	Uint8	   untilCopies[1832];
	TOUCH_COPY copies[64];
	Sint8	   cnt;
} TOUCH_COPIES;

STATIC_ASSERT(offsetof(TOUCH_COPIES, snapshotObjs) == 36);
STATIC_ASSERT(offsetof(TOUCH_COPIES, copies) == 5964);

/* The metatiles Psynergy moved, kept in the scene's own state
   (SceneLocalState). */
#define TouchCopiesWork (*(TOUCH_COPIES *)SceneLocalState)

/* A particle of the particle burst (fdParticleBurstStart): its place, heading,
   speed, spin and the ticks before it starts. */
typedef struct BURST_PARTICLE {
	Sint32 x;
	Sint32 y;
	Sint32 z;
	Sint32 heading;
	Sint32 speed;
	Sint32 spin;
	Sint32 delay;
} BURST_PARTICLE;

/* The particles of the particle burst. */
#define PARTICLE_BURST_COUNT 96

/* The particle burst's state (fdParticleBurstStart): its tiles, 96 sprites and
   particles, its VRAM cache entry, a flash on tick 30 and its tick count. */
typedef struct PARTICLE_BURST_STATE {
	Uint8		   tiles[384];
	EFFECT_SPR	   sprs[PARTICLE_BURST_COUNT];
	BURST_PARTICLE particles[PARTICLE_BURST_COUNT];
	Sint16 cacheEntry; /* the VRAM cache entry holding the leaves' tiles */
	Sint16 tile;	   /* their first tile in OBJ VRAM */
	Sint16 flash;	   /* set on tick 30 only */
	Sint16 frm;
	Sint16 active;
} PARTICLE_BURST_STATE;

/* A metatile the particle burst covers, in metatiles. */
typedef struct BURST_METATILE {
	Sint16 x;
	Sint16 z;
} BURST_METATILE;

/* One step of a sparkle's rise (FieldEffectSparklePictures): its height above
   its spot in screen pixels, and its tile. */
typedef struct SPARKLE_PICTURE {
	Sint16 dy;
	Uint16 tile;
} SPARKLE_PICTURE;

/* gmPartyInventoryFindRoom through its stub call: the first party member with
   room for item, or -1. */
Sint32 gmPartyInventoryFindRoomStub(Sint32 item);
/* mnItemPickerRunToDrop through its stub call: the item picker for an item to
   drop, which sets the member and slot chosen. */
Sint32 mnItemPickerRunToDropStub(Sint32 *member, Sint32 *slot);
/* fdActorScriptCheckOverlap through its stub call: -1 when the actor at
   position would overlap a solid actor, otherwise 0. */
Sint32 fdActorScriptCheckOverlapStub(ACTOR *actor, VECTOR3 *pos);
/* fdActorGetSprId through its stub call: the sprite id the actor is drawn with,
   or 0 for none. */
Sint32 fdActorGetSprIdStub(ACTOR *actor);
/* fdMapGridCopyMetatileTilesMetatilesRect through its stub call: copies width
   by height metatiles as they are from (fromX, fromZ) to (toX, toZ). */
void fdMapGridCopyMetatileTilesMetatilesRectStub(Sint32 toX, Sint32 toZ,
	Sint32 width, Sint32 height, Sint32 fromX, Sint32 fromZ);
/* fdActorScriptFindOverlappingActor through its stub call: the first solid
   actor the actor at position would overlap, or NULL. */
ACTOR *fdActorScriptFindOverlappingActorStub(ACTOR *actor, VECTOR3 *pos);

void   fdPsynergyMove(void);
void   fdPsynergyCarry(void);
void   fdPsynergySteerLiftedTarget(Sint32 tgtId);
void   fdPsynergyForce(void);
void   fdPsynergyLash(void);
void   fdPsynergyLashOnTarget(Sint32 value);
void   fdPsynergyPound(void);
void   fdPsynergyTremor(void);
void   fdPsynergyScoop(void);
void   fdPsynergyGrowth(void);
void   fdPsynergyGrowthOnTarget(Sint32 unused);
void   fdPsynergyWhirlwind(void);
void   fdPsynergyParch(void);
void   fdPsynergySand(void);
void   fdPsynergyEndSand(void);
void   fdPsynergyBurst(void);
void   fdPsynergyBlaze(void);
void   fdPsynergyPly(void);
void   fdPsynergyHalt(void);
Sint32 fdPsynergyFilterHaltTarget(Sint32 id);
void   fdPsynergyCatch(void);
void   fdPsynergyArrow(void);
void   fdPsynergyRetreat(void);
void   fdPsynergyAvoid(void);
void   fdPsynergyShowAvoidWoreOff(void);
void   fdPsynergyEndCloak(void);
void   fdPsynergyResumeCloakCheck(void);
void   fdPsynergyReveal(void);
void   fdPsynergyEndReveal(void);
void   fdPsynergyHover(void);
void   fdPsynergyEndHover(void);
void   fdPsynergyGrind(void);
void   fdPsynergyTeleport(void);
void   fdPsynergyRing(void);
void   fdPsynergyEndRing(void);
void   fdPsynergyWatchLeaderDistance(void);

/* The stage Burst passes its target's script each tick (BURST_STATE.shake). */
typedef enum BURST_STAGE {
	BURST_STAGE_QUIET,		 /* no explosion this tick */
	BURST_STAGE_DYING_DOWN,	 /* the great blast dying down */
	BURST_STAGE_SMALL_BLAST, /* a small explosion shakes the ground */
	BURST_STAGE_GREAT_BLAST	 /* the last and largest blast */
} BURST_STAGE;

/* Mind Read's wave, its tint and the palette it greys. */
void fdPsynergyUpdateMindReadWave(void);
void fdMindReadAdvanceTint(void);
void fdMindReadUpdateCrescent(ACTOR *actor);
void fdMindReadStartRipple(void);

ACTOR *fdEffectSpawnCarryHand(Sint32 x, Sint32 y, Sint32 z, Sint32 ang);
ACTOR *fdEffectSpawnLiftHand(Sint32 x, Sint32 y, Sint32 z, Sint32 ang);
Sint32 fdEffectSpawnHeavyImpact(ACTOR *src);
void   fdEffectSpawnBurstParticleField(void);
void   fdEffectUpdateRandomTargetParticle(FIELD_PARTICLE *effect);
void   fdEffectCheckCloakTile(void);
void   fdParticleBurstUpdate(void);
void   fdWhirlwindUpdateTarget(ACTOR *actor);
void   fdRetreatUpdateSpark(FIELD_PARTICLE *slot);
void   fdTeleportPlayWhirl(void);
void   fdTileCopiesRestore(void);

/* The caster's look while it casts, fdBattleEffectSetObjectUpdateMode's
   mode. */
typedef enum CASTER_LOOK {
	CASTER_LOOK_NORMAL = 0, /* its own colours */
	/* Flashing white, with swelling arcs (fdEffectFlickerActorAndSpawnArcs). */
	CASTER_LOOK_FLASHING_WHITE = 1,
	/* Cycling colours, with swelling arcs (fdEffectCycleActorAndSpawnArcs). */
	CASTER_LOOK_CYCLING_COLORS = 2
} CASTER_LOOK;

void fdBattleEffectDrawCanvasBlock(
	const Uint8 *src, Sint32 x, Sint32 y, Sint32 width, Sint32 height);
extern const Sint32 DustPuffLongScript[];
extern const Sint32 DustPuffShortScript[];
extern const Sint32 FieldEffectSourceHoldScript[];
extern const Sint32 ForceFistScript[];
extern const Uint8	MindReadCasterColorCycle[];
extern const Uint8	MoveHandPushPoses[];

/* Three coordinates of a burst piece's place, x, y and z, filled in turn as
   syVectorAddPolarOffset reads them. */
typedef struct BURST_PARTICLE_VECTOR {
	Sint32 values[3];
} BURST_PARTICLE_VECTOR;

/* One part of a sprite (ANIMATION_SPR_PART) as Mind Read's burst copies its
   image's attributes to its shadow: a byte at a time, under the OAM attributes'
   names. */
typedef struct BURST_PARTICLE_VISUAL {
	Uint8 unused[5];			/* the part's link and attribute 0's y */
	Uint8 affineModeMosaic : 5; /* attribute 0's affine, objMode and mosaic */
	Uint8 fullCol : 1;			/* 256 colours */
	Uint8 shape : 2;			/* 0 square, 1 wide, 2 tall */
	Uint8 unusedAfterShape;		/* attribute 1's x, low byte */
	Uint8 xHighAffineIdx : 6;	/* x bit 8 and the affine matrix */
	Uint8 size : 2;				/* with shape, the sprite's size */

	union {
		struct {
			Uint16 tile : 10; /* first sprite tile in VRAM */
			Uint16 priPal : 6;
		} bits;

		struct {
			Uint8 tileLow;
			Uint8 tileHighPri : 4;
			Uint8 pal : 4;
		} bytes;
	} attributes; /* attribute 2 */
} BURST_PARTICLE_VISUAL;

/* A sprite (SPR) as Mind Read's burst reads it: its image, and its shadow 28
   bytes in. */
typedef struct BURST_PARTICLE_VISUAL_GROUP {
	BURST_PARTICLE_VISUAL primary;
	Uint8				  unusedAfterPrimary[16]; /* no code names them */
	BURST_PARTICLE_VISUAL child;
} BURST_PARTICLE_VISUAL_GROUP;

/* The lifted actor as Lift on a target reads it: ACTOR's y, motionFlags (flag)
   and update (callback); read through ACTOR, the flag store moves after the
   anchor's load. */
typedef struct EFFECT_CHILD {
	Uint8  unused[12]; /* no code names them */
	Sint32 y;
	Uint8  unusedAfterY[69]; /* no code names them */
	Uint8  flg;
	Uint8  unusedAfterFlg[22]; /* no code names them */
	void (*func)(void);
} EFFECT_CHILD;

/* The Djinn: what each element's join effect shows, and settling a Djinni after
   its battle. */
void fdDjinnRunVenusJoinEffect(Sint32 actorId);
void fdDjinnRunMercuryJoinEffect(Sint32 actorId);
void fdDjinnRunMarsJoinEffect(Sint32 actorId);
void fdDjinnRunJupiterJoinEffect(Sint32 actorId);
void fdDjinnResolvePending(Sint32 shown);
void fdDjinnRestoreActorAfterBattle(void);

/* The map screen, which the World Map's Select shows: the leader's point on it
   and the tiles a cast has used. */
void fdMapScreenGetLeaderPoint(Sint32 *screenX, Sint32 *screenY);
void fdPsynergyMarkWorldMapTilesUsed(void);

/* The places' markers' blend, a level for every two ticks of a 64-tick
   cycle. */
extern const Uint8 WorldMapMarkerBlendCycle[];
/* The cursor's heading for each way the +Control Pad is held. */
extern const Uint16 WorldMapCursorDirectionAngles[];

/* The battles the scripts start: the encounter zones of the story's battles,
   the Mad Plants' leap and the transition into a battle. */
void fdStartEncounterTransition(Sint32 unused);
/* fdRequestDjinnBattle's flag: the Djinni does not hop first. */
#define DJINN_NO_HOP 0x800

/* The encounter zone of the hidden Mad Plants' battles, one slot each, in
   FLAG_MAD_PLANT's order. */
#define ENCOUNTER_ZONE_MAD_PLANTS 106

/* The Djinn's encounter zones (rows of EncounterZones), two to an element: the
   first holds the battles of its first fifteen Djinn, who share its eight
   slots, and the second those of its last three and its two spares. */
typedef enum DJINN_ENCOUNTER_ZONE {
	ENCOUNTER_ZONE_VENUS_DJINN = 1,
	ENCOUNTER_ZONE_VENUS_DJINN_LAST,
	ENCOUNTER_ZONE_MERCURY_DJINN,
	ENCOUNTER_ZONE_MERCURY_DJINN_LAST,
	ENCOUNTER_ZONE_MARS_DJINN,
	ENCOUNTER_ZONE_MARS_DJINN_LAST,
	ENCOUNTER_ZONE_JUPITER_DJINN,
	ENCOUNTER_ZONE_JUPITER_DJINN_LAST
} DJINN_ENCOUNTER_ZONE;

/* The encounter zone of the early story battles: the Ruffian waiting for backup
   on Venus Lighthouse's plaza (slot 0), the Ruffian on the Suhalla road (1),
   the three Ruffians at the crevice (2) and the Punch Ant (3) at Suhalla Gate,
   the Chestbeaters in the Kandorean Temple (4) and Echo's lesson battle on the
   World Map (5). */
#define ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY			 9
/* The encounter zone of two story battles: the King Scorpion's in the Yampi
   Desert (slot 0), then Briggs and his crew's on his ship at Alhafra (slot
   1). */
#define ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA 10
/* The encounter zone of the battles below deck of the Lemurian Ship: slot 0 an
   Aqua Jelly, slot 1 the Aqua Hydra. */
#define ENCOUNTER_ZONE_AQUA_JELLIES					 11
/* The encounter zone of three story battles: the Serpent's in Gaia Rock, one
   slot for each beam of sunlight let into its lair (ENEMY_SERPENT to
   ENEMY_SERPENT_FOUR_BEAMS, slots 0 to 4), then Avimander's in Champa (slot 5)
   and Poseidon's at the Sea of Time (slot 6). */
#define ENCOUNTER_ZONE_SERPENT						 12
/* The encounter zone of Moapa and his two Knights at the Trial Road's peak, one
   slot per stage door the party reached first (slots 0 to 4), then Karst and
   Agatio's three battles on Jupiter Lighthouse's aerie (slots 5 to 7). */
#define ENCOUNTER_ZONE_MOAPA						 13
/* The World Map's encounter zone of region 1's beach, shallows and grassland,
   which the Psynergy test room's battles also take (slot 0, one or two Giant
   Bats). */
#define ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND 14

/* The script battles: the Flame Dragons in Mars Lighthouse (slot 0), the Doom
   Dragon on its aerie (1), the gladiators in the Shaman Village Cave (2), the
   World Map's bandits (3) and Agatio and Karst's fight with Isaac in Jupiter
   Lighthouse (4). */
#define ENCOUNTER_ZONE_SCRIPTED 102

/* The encounter zone of the battles with four guardians of hidden places, one
   slot each (HIDDEN_BOSS). */
#define ENCOUNTER_ZONE_HIDDEN_BOSSES 103

/* The slots of ENCOUNTER_ZONE_HIDDEN_BOSSES. */
typedef enum HIDDEN_BOSS {
	/* the Star Magician, at the end of Treasure Isle */
	HIDDEN_BOSS_TREASURE_ISLE,
	HIDDEN_BOSS_VALUKAR,  /* Valukar, in the Yampi Desert Cave */
	HIDDEN_BOSS_SENTINEL, /* the Sentinel, at the end of the Islet Cave */
	/* The shadow at the entrance of the Anemos Inner Sanctum. */
	HIDDEN_BOSS_ANEMOS
} HIDDEN_BOSS;

/* fdActorSetLayersColMode's value that cycles the actor's colours
   (MindReadCasterColorCycle); any other is a colour mode plus one. */
#define LAYERS_COLOR_CYCLING 0
void fdActorSetLayersColMode(void *actor, Sint32 value);
void fdActorSetOthersPaused(
	ACTOR *excludedObject, Sint32 groupMode, Sint32 action);
void fdWeatherUpdateLightning(void);

/* fdActorJumpDownLedge's mode bit that skips the wait for the landing. */
#define LEDGE_JUMP_NO_LANDING_WAIT (1 << 12)

/* What fdInputGetDpadAngEightWay returns when no direction is held, or right
   and left or up and down together: the blank entries of
   InputDpadAnglesEightWay. */
#define DPAD_ANGLE_NONE 65535

#endif
