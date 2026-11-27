#ifndef ALCHEMY_FIELD_OBJECT_H
#define ALCHEMY_FIELD_OBJECT_H

/* The field's metatiles, actors, ropes, tightropes and hanging vines, field
   effects, weather and palette fades, as the scenes and the script commands
   call them. */

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/scene_tables.h"

struct ACTOR;
struct FIELD_PARTICLE;
struct PARTICLE_EMITTER;
struct HANGING_VINE_SLOT;

Sint32 fdMapGridIsWaterMetatile(Sint32 x, Sint32 z, Sint32 layer);
Sint32 fdMapGridGetCycloneMetatileKind(Sint32 x, Sint32 z, Sint32 layer);

/* What Cyclone does at a metatile (fdMapGridGetCycloneMetatileKind). */
typedef enum CYCLONE_METATILE_KIND {
	CYCLONE_METATILE_NONE = 0,
	CYCLONE_METATILE_BURST = 1, /* blown away, with the particle burst */
	CYCLONE_METATILE_CARRY = 2, /* the whirlwind carries the leader to it */
	CYCLONE_METATILE_LIFT = 3	/* the whirlwind lifts the leader to it */
} CYCLONE_METATILE_KIND;

/* The actors' sprites: their depth and priority, a silhouette, a sprite id and
   graphics copied from another actor. */
void fdActorSetTargetPosFromMagnitudeAng(
	struct ACTOR *actor, Sint32 magnitude, Sint32 angle);
void fdActorSetSprPriorityDirectStub(struct ACTOR *actor, Sint32 priority);
struct ACTOR *fdActorCreateSilhouette(
	Sint32 x, Sint32 y, Sint32 z, Sint32 type);

/* A facing table entry: the picture to draw, counted from the sprite
   animation's base picture (bits 0-2), mirrored when bit 7 is set. */
#define FACING_MIRRORED_BIT 0x80
#define MIRRORED(picture)	(FACING_MIRRORED_BIT | (picture))

/* The facing tables: the picture each heading draws, by how many pictures a
   sprite animation has for its facings. */
extern const Uint8	AnimationFacing_OnePicture[16];
extern const Uint8	AnimationFacing_FrontBackSwapped[8];
extern const Uint8	AnimationFacing_FrontBack[8];
extern const Uint8	AnimationFacing_ThreePictures[16];
extern const Uint8	AnimationFacing_FivePictures[64];
extern const Uint8	AnimationFacing_EightPictures[8];
extern const Sint32 ActorSprite_FullScale[2];

/* The actors' own scripts: launched, leaving footprints, ripples, sand grains,
   a wake and a splash. */
extern const Sint32 Actor_LaunchScript[];
extern const Sint32 Actor_FootprintScript[];
extern const Sint32 Actor_RippleScript[];
extern const Sint32 Sand_GrainScript[];
extern const Sint32 Actor_WakeScript[];
extern const Sint32 Actor_SplashScript[];

extern const ACTOR_PLACE Field_DefaultPlacements[];

/* The arrival fdEntrancePlayArrival played, for the way the last scene was
   left. */
typedef enum ENTRANCE_ARRIVAL {
	ARRIVAL_NONE = 0,		 /* none: the scene plays its own */
	ARRIVAL_STAIRS_UP = 1,	 /* up stairs, ANIM_LEADER_ARRIVE */
	ARRIVAL_STAIRS_DOWN = 2, /* down stairs, ANIM_LEADER_ARRIVE_ALTERNATE */
	ARRIVAL_FALLING = 3		 /* falling in from above */
} ENTRANCE_ARRIVAL;

/* Arriving in a scene, leaving by stairs, and the leader's jumps and climbs. */
Sint32 fdEntrancePlayArrival(void);
void   fdExitAddHiddenLeaderLayer(void);

/* A rope's state (ROPE_SLOT.state), which fdRopeGetState returns. */
typedef enum ROPE_SLOT_STATE {
	ROPE_COILED, /* at its first post, or pulled back there */
	ROPE_THROWN	 /* thrown or laid tied from post to post */
} ROPE_SLOT_STATE;

Sint32 fdRopeFindById(Sint32 id, Sint32 *value, Sint32 *index);
void fdRopeSwingBetween(Sint32 index, Sint32 fromId, Sint32 toId, FIXED height);
Bool fdRopeIsLeaderAtEnd(Sint32 firstId, Sint32 secondId);
Sint32 fdRopeLayTied(Sint32 index, Sint32 x1, Sint32 y1, Sint32 z1, Sint32 x2,
	Sint32 y2, Sint32 z2);
Sint32 fdRopeLayCoil(Sint32 index, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ);
Sint32 fdRopeLayBowed(Sint32 index, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ, FIXED height);
void   fdRopeDraw(void);
void   fdRopeUpdate(void);

/* The tightropes the party walks across, laid between two points or two posts
   as the ropes are. */
void fdTightropeWobbleBetween(
	Sint32 index, Sint32 fromId, Sint32 toId, FIXED height);
Sint32 fdTightropeLay(Sint32 index, Sint32 x, Sint32 y, Sint32 z, Sint32 toX,
	Sint32 toY, Sint32 toZ);
Sint32 fdTightropeLayBowed(Sint32 index, Sint32 x, Sint32 y, Sint32 z,
	Sint32 toX, Sint32 toY, Sint32 toZ, FIXED height);
Sint32 fdTightropeLayCoil(Sint32 index, Sint32 x, Sint32 y, Sint32 z,
	Sint32 toX, Sint32 toY, Sint32 toZ);
Sint32 fdTightropeRecoil(Sint32 slot, Sint32 x1, Sint32 y1, Sint32 z1,
	Sint32 x2, Sint32 y2, Sint32 z2, Sint32 wave);
void   fdTightropeDraw(void);
void   fdTightropeUpdate(void);

struct HANGING_VINE_SLOT *fdHangingVineFindSlot(Sint32 id);
void					  fdHangingVineDraw(void);
void					  fdHangingVineUpdate(void);

/* The field effects the Psynergy and the found items show. */
Sint32 fdPsynergyCheckOverlap(
	Sint32 *a, Sint32 radiusA, Sint32 *b, Sint32 radiusB);
void fdPsynergyGrowFromCaster(
	struct ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);
void		  fdPsynergyDrawCanvasSpr(void);
void		  fdPsynergyReleaseSceneSpr(void);
struct ACTOR *fdBattleEffectSpawnScatterParticle(Sint32 x, Sint32 y, Sint32 z);
void		  fdEffectSpawnRevealMarkers(void);
void		  fdEffectShrinkActorAndDestroyFast(void *obj);
void		  fdEffectShrinkActorAndDestroySlow(void *obj);
void		  fdEffectUpdateRadialSpread(struct FIELD_PARTICLE *effect);
void		  fdEffectPulseActorScale(struct ACTOR *actor);
void		  fdEffectAdvanceAngWithRamp(struct ACTOR *actor);
void		  fdEffectUpdateMarsSpark(struct FIELD_PARTICLE *effect);
void		  fdItemDispEmitSparkles(struct ACTOR *emitter);

/* The weather and the ground's particles, each frame. */
void fdGroundParticlesUpdate(void);
void fdMotesUpdate(void);
void fdWeatherUpdateRain(void);

/* A Djinni's hop as its battle starts, and its spin. */
void fdDjinnRunBattleStartHop(Sint32 djinniId);
void fdDjinnAdvanceSpinAng();

Sint32 fdPaletteFadeClampComponent(Sint32 value);
Sint32 fdPaletteFadeClampShiftedComponent(Sint32 value);

/* The World Map's graphics, its fixed camera and the yaw its camera leans to
   for each state of the direction pad. */
extern const Uint32 WorldMap_GraphicsResources[4][9];
extern const FIXED	Camera_FixedViewMatrix[12];
extern const Sint32 WorldMap_KeyYaws[16];

/* WorldMap_KeyHeadings' entry when no direction is held, or only an opposite
   pair: no heading, since every real one is a multiple of an eighth turn. */
#define WORLD_MAP_KEY_HEADING_NONE 65535

#endif
