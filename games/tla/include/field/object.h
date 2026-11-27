#ifndef ALCHEMY_FIELD_OBJECT_H
#define ALCHEMY_FIELD_OBJECT_H

/* @file */
/* The field's cells, actors, ropes, tightropes and hanging vines, field */
/* effects, weather and palette fades, as the scenes and the event commands */
/* call them. */

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/scene_tables.h"

struct ACTOR;
struct FIELD_PARTICLE;
struct PARTICLE_EMITTER;
struct HANGING_VINE_SLOT;

s32 fdMapGridIsWaterCell(s32 x, s32 z, s32 layer);
s32 fdMapGridGetCycloneCellKind(s32 x, s32 z, s32 layer);

/* What Cyclone does at a cell (fdMapGridGetCycloneCellKind). */
typedef enum CYCLONE_CELL_KIND {
	CYCLONE_CELL_NONE = 0,
	CYCLONE_CELL_BURST = 1, /* blown away, with the particle burst */
	CYCLONE_CELL_CARRY = 2, /* the whirlwind carries the leader to it */
	CYCLONE_CELL_LIFT = 3	/* the whirlwind lifts the leader to it */
} CYCLONE_CELL_KIND;

/* The actors' sprites: their depth and priority, a silhouette, a sprite id */
/* and graphics copied from another actor. */
void fdActorSetTargetPosFromMagnitudeAng(struct ACTOR *actor, s32 magnitude,
										 s32 angle);
void fdActorSetSprPriorityDirectStub(struct ACTOR *actor, s32 priority);
struct ACTOR *fdActorCreateSilhouette(s32 x, s32 y, s32 z, s32 type);

/* A facing table entry: the picture to draw, counted from the sprite */
/* animation's base picture (bits 0-2), mirrored when bit 7 is set. */
#define FACING_MIRRORED_BIT 0x80
#define MIRRORED(picture)	(FACING_MIRRORED_BIT | (picture))

/* The facing tables: the picture each heading draws, by how many pictures a */
/* sprite animation has for its facings. */
extern const u8 AnimationFacing_OnePicture[16];
extern const u8 AnimationFacing_FrontBackSwapped[8];
extern const u8 AnimationFacing_FrontBack[8];
extern const u8 AnimationFacing_ThreePictures[16];
extern const u8 AnimationFacing_FivePictures[64];
extern const u8 AnimationFacing_EightPictures[8];
extern const s32 ActorSprite_FullScale[2];

/* The actors' own scripts: launched, leaving footprints, ripples, sand */
/* grains, a wake and a splash. */
extern const s32 Actor_LaunchScript[];
extern const s32 Actor_FootprintScript[];
extern const s32 Actor_RippleScript[];
extern const s32 Sand_GrainScript[];
extern const s32 Actor_WakeScript[];
extern const s32 Actor_SplashScript[];

extern const ACTOR_PLACE Field_DefaultPlacements[];

/* The arrival fdEntrancePlayArrival played, for the way the last scene was */
/* left. */
typedef enum ENTRANCE_ARRIVAL {
	ARRIVAL_NONE = 0,		 /* none: the scene plays its own */
	ARRIVAL_STAIRS_UP = 1,	 /* up stairs, ANIM_LEADER_ARRIVE */
	ARRIVAL_STAIRS_DOWN = 2, /* down stairs, ANIM_LEADER_ARRIVE_ALTERNATE */
	ARRIVAL_FALLING = 3		 /* falling in from above */
} ENTRANCE_ARRIVAL;

/* Arriving in a scene, leaving by stairs, and the leader's jumps and climbs. */
s32 fdEntrancePlayArrival(void);
void fdExitAddHiddenLeaderLayer(void);

/* A rope's state (ROPE_SLOT.state), which fdRopeGetState returns. */
typedef enum ROPE_SLOT_STATE {
	ROPE_COILED, /* at its first post, or pulled back there */
	ROPE_THROWN	 /* thrown or laid tied from post to post */
} ROPE_SLOT_STATE;
s32 fdRopeFindById(s32 id, s32 *value, s32 *index);
void fdRopeSwingBetween(s32 index, s32 fromId, s32 toId, FIXED height);
s32 fdRopeIsLeaderAtEnd(s32 firstId, s32 secondId);
s32 fdRopeLayTied(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);
s32 fdRopeLayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);
s32 fdRopeLayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
				   FIXED height);
void fdRopeDraw(void);
void fdRopeUpdate(void);

/* The tightropes the party walks across, laid between two points or two */
/* posts as the ropes are. */
void fdTightropeWobbleBetween(s32 index, s32 fromId, s32 toId, FIXED height);
s32 fdTightropeLay(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);
s32 fdTightropeLayBowed(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
						s32 toZ, FIXED height);
s32 fdTightropeLayCoil(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY,
					   s32 toZ);
s32 fdTightropeRecoil(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
					  s32 wave);
void fdTightropeDraw(void);
void fdTightropeUpdate(void);

struct HANGING_VINE_SLOT *fdHangingVineFindSlot(s32 id);
void fdHangingVineDraw(void);
void fdHangingVineUpdate(void);

/* The field effects the Psynergy and the found items show. */
s32 fdPsynergyCheckOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void fdPsynergyGrowFromCaster(struct ACTOR *actor, s32 x, s32 y, s32 z);
void fdPsynergyDrawCanvasSpr(void);
void fdPsynergyReleaseSceneSpr(void);
struct ACTOR *fdBattleEffectSpawnScatterParticle(s32 x, s32 y, s32 z);
void fdEffectSpawnRevealMarkers(void);
void fdEffectShrinkActorAndDestroyFast(void *obj);
void fdEffectShrinkActorAndDestroySlow(void *obj);
void fdEffectUpdateRadialSpread(struct FIELD_PARTICLE *effect);
void fdEffectPulseActorScale(struct ACTOR *actor);
void fdEffectAdvanceAngWithRamp(struct ACTOR *actor);
void fdEffectUpdateMarsSpark(struct FIELD_PARTICLE *effect);
void fdItemDispEmitSparkles(struct ACTOR *emitter);

/* The weather and the ground's particles, each frame. */
void fdGroundParticlesUpdate(void);
void fdMotesUpdate(void);
void fdWeatherUpdateRain(void);

/* A Djinni's hop as its battle starts, and its spin. */
void fdDjinnRunBattleStartHop(s32 djinniId);
void fdDjinnAdvanceSpinAng();

s32 fdPaletteFadeClampComponent(s32 value);
s32 fdPaletteFadeClampShiftedComponent(s32 value);

/* The World Map's graphics, its fixed camera and the yaw its camera leans to */
/* for each state of the direction pad. */
extern const u32 WorldMap_GraphicsResources[4][9];
extern const FIXED Camera_FixedViewMatrix[12];
extern const s32 WorldMap_KeyYaws[16];

/* WorldMap_KeyHeadings' entry when no direction is held, or only an */
/* opposite pair: no heading, since every real one is a multiple of an eighth */
/* turn. */
#define WORLD_MAP_KEY_HEADING_NONE 65535

#endif
