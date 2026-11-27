#ifndef ALCHEMY_ACTOR_H
#define ALCHEMY_ACTOR_H

/* @file */
/* Actors, the actor snapshot, and the screen transition styles. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/actor_arm_view.h"
#include "field/field_state.h"

/* How a scene fades in and out: SCENE_TRANSITION's high byte, which */
/* fdScrTransitionStart and fdScrTransitionFinish read. */
typedef enum SCR_TRANSITION_STYLE {
	TRANSITION_FADE = 0,		  /* from or to black */
	TRANSITION_BACKDROP_FADE = 1, /* from or to the backdrop colour */
	TRANSITION_WINDOW = 2,		  /* a window shape the variant chooses */
	TRANSITION_DISSOLVE = 3,
	/* Opening from, or closing to, the centre line; the variant picks how. */
	TRANSITION_CENTER_LINE = 4
} SCR_TRANSITION_STYLE;

/* TRANSITION_CENTER_LINE's variants. */
typedef enum CENTER_LINE_TRANSITION {
	CENTER_LINE_STEADY = 0, /* the lines open or close steadily */
	/* The lines show only every other frame, as the World Map closes into a */
	/* battle. */
	CENTER_LINE_FLICKER = 1
} CENTER_LINE_TRANSITION;

/* A scene transition as FIELD_STATE.transition holds it. */
#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))
/* The two bytes of a SCENE_TRANSITION: its SCR_TRANSITION_STYLE, and */
/* the variant that style reads. */
#define SCENE_TRANSITION_STYLE(transition)	 (((transition) >> 8) & 0xff)
#define SCENE_TRANSITION_VARIANT(transition) ((transition) & 0xff)

#include "field/field_coordinate.h"

#include "field/sprite.h"

/* The actors of ActorsWork (HEAP_SLOT_ACTORS). */
#define ACTOR_COUNT 64

struct ACTOR;
union ACTOR_OR_EFFECT;

/* An actor: one of the ACTOR_COUNT people and things on the field (ActorsWork,
 */
/* HEAP_SLOT_ACTORS), with its actor script, position, motion toward a target,
 */
/* sprites and update function. */
typedef struct ACTOR {
	/* The actor script it runs; null when its slot is free. */
	const s32 *script;
	s16 scriptCursor;	/* the script cursor, in words */
	u16 facing;			/* 1/65536 turns */
	FIELD_COORDINATE x; /* 16.16 fixed-point pixels */
	FIELD_COORDINATE y; /* 16.16 fixed-point pixels */
	FIELD_COORDINATE z; /* 16.16 fixed-point pixels */
	FIXED groundHeight; /* 16.16 fixed-point pixels */
	FIXED scaleX;		/* 16.16 fixed point, FIXED_ONE full size */
	FIXED scaleY;		/* 16.16 fixed point, FIXED_ONE full size */
	/* Collision radius in pixels: 16, or half the sprite metadata's footprint,
	 */
	/* when created; the overlap checks use it less 2. */
	u16 radius;
	/* The map layer (0 to 3) its ground height is read from, which the */
	/* scrolling camera updates from the cell under it; the fixed camera passes
	 */
	/* it as the sprite's draw slot. */
	u8 layer;
	/* ACTOR_PRIORITY_FLAG bits, read by the scrolling camera: */
	/* ACTOR_PRIORITY_AUTOMATIC for placed actors, ACTOR_PRIORITY_UNDERFOOT for
	 */
	/* footprints. */
	u8 priFlg;
	FIXED velX;		  /* 16.16 fixed-point pixels per frame */
	FIXED velY;		  /* 16.16 fixed-point pixels per frame */
	FIXED velZ;		  /* 16.16 fixed-point pixels per frame */
	FIXED speedLimit; /* 16.16 fixed-point pixels per frame */
	/* Added toward the target each frame, or taken off the speed when there is
	 */
	/* none, in 16.16 fixed-point pixels per frame; FIXED_ONE when created. */
	FIXED acceleration;
	FIXED tgtX; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED tgtY; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED tgtZ; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	/* The share of its falling speed kept as it bounces on landing, 16.16 */
	/* fixed point (a quarter when created); a floating actor's phase instead.
	 */
	FIXED bounce;
	/* Taken off velocityY each frame while it is above the ground, in 16.16 */
	/* fixed-point pixels per frame (FIXED_ONE when created); also scales a */
	/* floating actor's bob. */
	FIXED gravity;
	/* Zeroed by syFieldActorCreate and not read by the field engine; scene code
	 */
	/* keeps a resting height, a sway or a countdown here. */
	s32 sceneWord;
	/* Its sprite, by spriteStorageKind: one sprite for ACTOR_SPRITE_ONE; for */
	/* ACTOR_SPRITE_GROUP four, empty ones null, read as void **. */
	SPR *spr;
	/* Zero hides sprites; one draws a single sprite, two draws a group. */
	u8 sprStorageKind;
	/* ACTOR_MOTION_FLAG bits; none is free flight in 3D, and */
	/* ACTOR_FOLLOWS_TERRAIN and ACTOR_FALLS are set on creation. */
	u8 motionFlg;
	u8 arrivalAxis;		/* the ARRIVAL_AXIS_* watched for arrival */
	u8 conditionResult; /* the actor script's last flag test */
	u8 snapToTgt;		/* nonzero lands it exactly on its target on arrival */
	/* ACTOR_COLLISION_FLAG bits; placed actors take ACTOR_SOLID. */
	u8 collisionFlg;
	u8 steeringFlg; /* ACTOR_STEERING_FLAG bits */
	u8 paused;		/* nonzero skips its script and motion, not its update */
	/* Nonzero keeps its sprite's VRAM cache entry while sprites are suspended
	 */
	/* or it is off screen (fdActorSystemPlaceSprs). */
	u8 retainSprCache;
	u8 repeatCnt; /* the actor script's SC_LOOP count */
	s16 wait;	  /* the actor script's wait, in frames */
	/* Frames in a row a solid actor has refused its move; SC_SYNC stops */
	/* waiting after 60. */
	u8 refused;
	/* Nonzero skips steering, terrain, gravity and floating; its velocity */
	/* still moves it. */
	u8 frozen;
	u8 sceneCnt;  /* not read by the field engine; scene code's own counter
				   */
	u8 sceneByte; /* not read by the field engine; scene code's own state */
	/* Two halfwords whose meaning depends on what runs the actor. */
	union {
		struct {
			/* The parameter its actor script reads (a follow distance or an
			 * angle */
			/* to face), the player's frames to its next footprint, or scene */
			/* scratch. */
			s16 tmr;
			/* The side of the player's next footprint (2 after a stop), or
			 * scene */
			/* scratch. */
			s16 userValue;
		};
		/* Where SC_WANDER_HOME wanders around, in pixels: where it was created
		 */
		/* or placed. */
		struct {
			s16 homeX;
			s16 homeZ;
		};
		/* Avoid's caster (fdEffectUpdateAvoidCaster). */
		struct {
			s16 arcPhase; /* 1 spins the caster 3/64 turn a frame */
			s16 arcFrm;	  /* frames since the arcs started */
		};
		/* A Venus Djinni's falling motes */
		/* (fdEffectUpdateDescendingOrbitActor). */
		struct {
			u16 orbitFrm; /* frames left of the spiral, 100 at the start */
			s16 orbitAng; /* its start on the circle, 1/65536 turns */
		};
		/* A Whirlwind particle (fdEffectCircleAnchor, */
		/* fdEffectWanderAroundAnchor). */
		struct {
			u16 swirlFrm; /* frames it has circled or wandered so far */
			s16 turnFrm;  /* frames left of a sharp turn */
		};
		/* A spin that speeds up (fdEffectAdvanceAngWithRamp). */
		struct {
			s16 spinRampFrm; /* frames the spin has been speeding up for */
		};
	};
	/* The object (an actor or a field effect) its actor script follows or */
	/* faces; field code also keeps an actor handle here. */
	union ACTOR_OR_EFFECT *watchedActor;
	/* Called each frame before its script, while the actor is live. */
	void (*update)(struct ACTOR *actor);
} ACTOR;

/* Sets an actor's position in one statement, as SET_POSITION does for a */
/* particle. */
#define SET_ACTOR_POSITION(p, fixedX, fixedY, fixedZ)                          \
	((p)->x.fixed = (fixedX), (p)->y.fixed = (fixedY), (p)->z.fixed = (fixedZ))

STATIC_ASSERT(sizeof(ACTOR) == 112);
STATIC_ASSERT(offsetof(ACTOR, arrivalAxis) == 86);
STATIC_ASSERT(offsetof(ACTOR, conditionResult) == 87);
STATIC_ASSERT(offsetof(ACTOR, snapToTgt) == 88);
STATIC_ASSERT(offsetof(ACTOR, userValue) == 102);
STATIC_ASSERT(offsetof(ACTOR, facing) == 6);
STATIC_ASSERT(offsetof(ACTOR, x) == 8);
STATIC_ASSERT(offsetof(ACTOR, z) == 16);
STATIC_ASSERT(offsetof(ACTOR, groundHeight) == 20);
STATIC_ASSERT(offsetof(ACTOR, scaleY) == 28);
STATIC_ASSERT(offsetof(ACTOR, priFlg) == 35);
STATIC_ASSERT(offsetof(ACTOR, motionFlg) == 85);
STATIC_ASSERT(offsetof(ACTOR, collisionFlg) == 89);
STATIC_ASSERT(offsetof(ACTOR, steeringFlg) == 90);
STATIC_ASSERT(offsetof(ACTOR, paused) == 91);
STATIC_ASSERT(offsetof(ACTOR, retainSprCache) == 92);
STATIC_ASSERT(offsetof(ACTOR, repeatCnt) == 93);
STATIC_ASSERT(offsetof(ACTOR, wait) == 94);
STATIC_ASSERT(offsetof(ACTOR, refused) == 96);
STATIC_ASSERT(offsetof(ACTOR, frozen) == 97);
STATIC_ASSERT(offsetof(ACTOR, sceneByte) == 99);
STATIC_ASSERT(offsetof(ACTOR, bounce) == 68);
STATIC_ASSERT(offsetof(ACTOR, gravity) == 72);
STATIC_ASSERT(offsetof(ACTOR, sceneWord) == 76);
STATIC_ASSERT(offsetof(ACTOR, velY) == 40);
STATIC_ASSERT(offsetof(ACTOR, speedLimit) == 48);
STATIC_ASSERT(offsetof(ACTOR, tgtX) == 56);
STATIC_ASSERT(offsetof(ACTOR, spr) == 80);
STATIC_ASSERT(offsetof(ACTOR, update) == 108);

/* Preserves the current action and motion when assigning a watched actor. */
typedef enum ACTOR_WATCH_FLAG {
	ACTOR_WATCH_KEEP_MOTION = 0x10000
} ACTOR_WATCH_FLAG;

/* ACTOR.priFlg bits. */
typedef enum ACTOR_PRIORITY_FLAG {
	/* The sprite takes its priority from the map cell under it. */
	ACTOR_PRIORITY_AUTOMATIC = 0x01,
	/* The depth sort treats it as 320 pixels higher on the screen. */
	ACTOR_PRIORITY_UNDERFOOT = 0x02,
	/* The depth sort treats it as 320 pixels lower on the screen. */
	ACTOR_PRIORITY_OVERHEAD = 0x04
} ACTOR_PRIORITY_FLAG;

/* ACTOR.collisionFlg bits. */
typedef enum ACTOR_COLLISION_FLAG {
	ACTOR_SOLID = 0x01,			/* blocks other actors and the player */
	ACTOR_PUSHABLE = 0x02,		/* the player pushes it */
	ACTOR_NEAR_TALK = 0x04,		/* nearer to talk to */
	ACTOR_NOT_TARGETED = 0x08,	/* never a talk or Psynergy target */
	ACTOR_NEAR_PSYNERGY = 0x10, /* nearer for Psynergy targets */
	/* Refuses to move into a solid actor (ACTOR.refused). */
	ACTOR_BLOCKED_BY_SOLID = 0x80
} ACTOR_COLLISION_FLAG;

enum {
	ACTOR_SNAPSHOT_CAPACITY = 32, /* actors a snapshot keeps */
	ACTOR_SNAPSHOT_EMPTY = 255	  /* an empty slot, and the end of the list */
};

/* Actors kept across a battle or a saved scene, with parallel sprite */
/* state. The slot list ends in ACTOR_SNAPSHOT_EMPTY unless every entry */
/* is filled. */
typedef struct ACTOR_SNAPSHOT {
	u8 slots[ACTOR_SNAPSHOT_CAPACITY];		/* actor ids, or */
											/* ACTOR_SNAPSHOT_EMPTY */
	ACTOR actors[ACTOR_SNAPSHOT_CAPACITY];	/* copied by DMA */
	u8 anms[ACTOR_SNAPSHOT_CAPACITY];		/* each sprite's lastNo */
	u8 sprFlg[ACTOR_SNAPSHOT_CAPACITY];		/* restored through the */
											/* dispatcher */
	u8 priorities[ACTOR_SNAPSHOT_CAPACITY]; /* both image and shadow */
											/* priority */
} ACTOR_SNAPSHOT;

STATIC_ASSERT(offsetof(ACTOR_SNAPSHOT, actors) == 32);
STATIC_ASSERT(offsetof(ACTOR_SNAPSHOT, anms) == 3616);
STATIC_ASSERT(offsetof(ACTOR_SNAPSHOT, sprFlg) == 3648);
STATIC_ASSERT(offsetof(ACTOR_SNAPSHOT, priorities) == 3680);
STATIC_ASSERT(sizeof(ACTOR_SNAPSHOT) == 3712);

/* The one snapshot, in EWRAM outside the heap so that it outlives the scene */
/* change into and out of a battle. */
extern ACTOR_SNAPSHOT ActorSnapshotWork;

/* Sets bits in one of an actor's flag bytes. */
static inline void syActorFlagsSet(u8 *flags, u8 bits) { *flags |= bits; }

#endif
