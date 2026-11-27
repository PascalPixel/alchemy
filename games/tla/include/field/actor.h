#ifndef ALCHEMY_ACTOR_H
#define ALCHEMY_ACTOR_H

/* Actors, their flags and the screen transition styles. */

#include "field/field_coordinate.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/* How a scene fades in and out: SCENE_TRANSITION's high byte, which
   fdScreenTransitionStart and fdScreenTransitionFinish read. */
typedef enum SCREEN_TRANSITION_STYLE {
	TRANSITION_FADE = 0,		  /* from or to black */
	TRANSITION_BACKDROP_FADE = 1, /* from or to the backdrop colour */
	TRANSITION_WINDOW = 2,		  /* a window shape the variant chooses */
	TRANSITION_DISSOLVE = 3,
	/* Opening from, or closing to, the centre line; the variant picks how. */
	TRANSITION_CENTRE_LINE = 4
} SCREEN_TRANSITION_STYLE;

/* TRANSITION_CENTRE_LINE's variants: the lines open or close steadily, or
   flicker every other tick as they close
   (fdScreenTransitionUpdateFromCenter). */
typedef enum CENTRE_LINE_VARIANT {
	CENTRE_LINE_STEADY = 0,
	CENTRE_LINE_FLICKERING = 1
} CENTRE_LINE_VARIANT;

/* A scene transition as FIELD_STATE.transition holds it. */
#define SCENE_TRANSITION(style, variant)	 ((style) << 8 | (variant))
/* The two bytes of a SCENE_TRANSITION: its SCREEN_TRANSITION_STYLE, and the
   variant that style reads. */
#define SCENE_TRANSITION_STYLE(transition)	 (((transition) >> 8) & 0xff)
#define SCENE_TRANSITION_VARIANT(transition) ((transition) & 0xff)

/* ACTOR.tgtX, targetY and targetZ when it has no movement target. */
#define ACTOR_NO_TARGET ((Sint32)0x80000000)

/* The actor id that ends a list of actors, or of rows that start with one. */
#define ACTOR_LIST_END 65535

/* The actors of RamHeapSlots->actors. */
#define ACTOR_COUNT 64

/* Sets an actor's position in one statement, as SET_POSITION does for a
   particle. */
#define SET_ACTOR_POSITION(p, fixedX, fixedY, fixedZ) \
	((p)->x.fixed = (fixedX), (p)->y.fixed = (fixedY), (p)->z.fixed = (fixedZ))

/* ACTOR.priFlg bits. */
typedef enum ACTOR_PRIORITY_FLAG {
	/* The sprite takes its priority from the map metatile under it. */
	ACTOR_PRIORITY_AUTOMATIC = 0x01,
	/* The depth sort treats it as 320 pixels higher on the screen. */
	ACTOR_PRIORITY_UNDERFOOT = 0x02,
	/* The depth sort treats it as 320 pixels lower on the screen. */
	ACTOR_PRIORITY_OVERHEAD = 0x04,
	/* Drawn at the depth its sceneCnt byte holds, not its own place
	   (fdRenderApplyProjectedPlacementAtDepth). */
	ACTOR_PRIORITY_OWN_DEPTH = 0x08,
	ACTOR_PRIORITY_NOT_DRAWN = 0x10, /* placed nowhere, as if off screen */
	/* With ACTOR_PRIORITY_AUTOMATIC, takes the priority of the metatiles under
	   it even where the ground in front rises, which would put it behind. */
	ACTOR_PRIORITY_IGNORES_RISES = 0x20,
	/* The actor steps up onto platform actors a metatile below it, as the
	   leader does onto pushed blocks and statues. */
	ACTOR_PRIORITY_WALKS_ON_PLATFORMS = 0x40,
	/* Others with ACTOR_PRIORITY_WALKS_ON_PLATFORMS may stand on it. */
	ACTOR_PRIORITY_PLATFORM = 0x80
} ACTOR_PRIORITY_FLAG;

/* ACTOR.motionFlg bits. */
typedef enum ACTOR_MOTION_FLAG {
	ACTOR_FOLLOWS_TERRAIN = 0x01, /* moves with the ground height under it */
	ACTOR_FALLS = 0x02,			  /* falls by its gravity and bounces */
	ACTOR_FLOATS = 0x04,		  /* bobs up and down by its gravity */
	ACTOR_FLOATS_HIGH = 0x08,	  /* with ACTOR_FLOATS, four times as far */
	/* Keeps its speed on slopes, which slow others down. */
	ACTOR_KEEPS_SPEED_ON_SLOPES = 0x10,
	/* Unused: nothing sets or reads it; a jump clears it with
	   ACTOR_FOLLOWS_TERRAIN. */
	ACTOR_MOTION_SPARE = 0x80,
	/* The motion flags a jump keeps as the actor leaves the ground: all but
	   ACTOR_FOLLOWS_TERRAIN and the spare bit. */
	ACTOR_JUMP_KEEPS_MOTION =
		(Uint8)~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE)
} ACTOR_MOTION_FLAG;

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

/* ACTOR.steeringFlg bits. */
typedef enum ACTOR_STEERING_FLAG {
	ACTOR_FACES_MOTION = 0x01 /* turns toward its velocity, 1/16 turn a tick */
} ACTOR_STEERING_FLAG;

/* The actor snapshot (SnapshotObjects), as The Broken Seal's ActorSnapshot: how
   many actors it keeps, and the slot id that marks an empty place and ends its
   list. */
enum {
	ACTOR_SNAPSHOT_CAPACITY = 32, /* actors a snapshot keeps */
	ACTOR_SNAPSHOT_EMPTY = 255	  /* an empty slot, and the end of the list */
};

/* The axis a moving actor watches to know it has arrived at its target: the one
   along which it has the furthest to go. */
typedef enum ARRIVAL_AXIS {
	ARRIVAL_AXIS_NONE = 0, /* no arrival test; set again once it arrives */
	ARRIVAL_AXIS_X = 16,
	ARRIVAL_AXIS_Y = 17,
	ARRIVAL_AXIS_Z = 18
} ARRIVAL_AXIS;

union FIELD_OBJECT;
struct SPR;

/* An actor's per-tick hook, called with the actor
   (fdActorUpdateAllOnField). */
typedef void (*OBJECT_UPDATE)(union FIELD_OBJECT *object);

/* ACTOR.retainSprCache's bits, read by fdActorSystemPlaceSprs. */
typedef enum ACTOR_SPR_CACHE_FLAG {
	/* Keeps the sprite's VRAM cache entry while the field is suspended or the
	   actor is off screen (fdActorSetItemIcon sets it). */
	ACTOR_KEEP_SPRITE_VRAM = 0x1,
	/* Keeps composing its picture while it is off screen. */
	ACTOR_COMPOSE_OFF_SCREEN = 0x2
} ACTOR_SPR_CACHE_FLAG;

/* An actor on the field: its script, position, motion toward a target, sprites
   and update hook. */
typedef struct ACTOR {
	const Sint32 *script; /* the actor script it runs; null when it is free */
	Sint16		  scriptCursor;	   /* the script cursor, in words */
	Uint16		  facing;		   /* 1/65536 turns */
	FIELD_COORDINATE x;			   /* 16.16 fixed-point pixels */
	FIELD_COORDINATE y;			   /* 16.16 fixed-point pixels */
	FIELD_COORDINATE z;			   /* 16.16 fixed-point pixels */
	FIXED			 groundHeight; /* 16.16 fixed-point pixels */
	FIXED			 scaleX;	   /* 16.16 fixed point, toFIXED(1) full size */
	FIXED			 scaleY;	   /* 16.16 fixed point, toFIXED(1) full size */
	Uint16			 radius;	   /* collision radius in pixels */
	Uint8			 layer; /* the map layer its ground height is read from */
	/* ACTOR_PRIORITY_FLAG bits; fdActorSetSprPriorityDirect clears
	   ACTOR_PRIORITY_AUTOMATIC as it fixes its sprite's priority. */
	Uint8 priFlg;
	FIXED velX;		  /* 16.16 fixed-point pixels per tick */
	FIXED velY;		  /* 16.16 fixed-point pixels per tick */
	FIXED velZ;		  /* 16.16 fixed-point pixels per tick */
	FIXED speedLimit; /* 16.16 fixed-point pixels per tick */
	/* Added toward the target each tick, or taken off the speed when there is
	   none, in 16.16 fixed-point pixels per tick. */
	FIXED acceleration;
	FIXED tgtX; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED tgtY; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED tgtZ; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	/* The share of its falling speed kept as it bounces on landing, 16.16 fixed
	   point. */
	FIXED bounce;
	/* Taken off the vertical velocity each tick while it is above the ground,
	   in 16.16 fixed-point pixels per tick. */
	FIXED gravity;
	/* Not read by the motion update; field code keeps its own values here (a
	   drifting speed, or the mark of a treasure spot that stands for a map
	   metatile), and scene code a resting height. */
	Sint32 sceneWord;
	/* Its sprite for ACTOR_SPRITE_ONE (read as a FIELD_SPR or a struct Sprite),
	   or its list of sprites for ACTOR_SPRITE_GROUP, which code that reads it
	   converts. */
	struct SPR *spr;
	/* ACTOR_SPRITE_ONE or ACTOR_SPRITE_GROUP in the low four bits, which is all
	   the field and the scenes test. */
	Uint8 sprStorageKind;
	Uint8 motionFlg; /* ACTOR_MOTION_FLAG bits */
	/* The ARRIVAL_AXIS_* watched for arrival at the target; ARRIVAL_AXIS_NONE
	   once it arrives, and cleared with the target. */
	Uint8 arrivalAxis;
	Uint8 conditionResult; /* the actor script's last flag test */
	Uint8 snapToTgt;	   /* nonzero lands it exactly on its target */
	Uint8 collisionFlg;	   /* ACTOR_COLLISION_FLAG bits */
	Uint8 steeringFlg;	   /* ACTOR_STEERING_FLAG bits */
	Uint8 paused;
	/* ACTOR_SPR_CACHE_FLAG bits: what its sprite keeps while the field is
	   suspended or the actor is off screen (fdActorSystemPlaceSprs). */
	Uint8  retainSprCache;
	Uint8  repeatCnt; /* the actor script's loop count */
	Sint16 wait;	  /* the actor script's wait, in ticks */

	/* The field's effects keep a hook here; a scene whose actor bobs keeps its
	   own tick count in the third byte instead. */
	union {
		void *func;

		struct {
			/* ticks in a row a solid actor has refused its move */
			Uint8 refused;
			Uint8 frozen;
			Uint8 sceneCnt; /* ticks into the bob, its low six bits a cycle */
			/* A block's: nonzero while it blocks the metatiles ahead of and
			   behind it. */
			Uint8 sceneByte;
		};

		/* The ship on the World Map (FieldMoveByKeys): ticks to its next wake,
		   and a wake's own ticks. */
		struct {
			Uint8 unusedBeforeWake[2]; /* refused and frozen */
			Uint8 wakeTmr;			   /* ticks to the ship's next wake */
			Uint8 wakeFrm;			   /* a wake's ticks left */
		};

		/* An actor showing an item (fdActorSetItemIcon). */
		struct {
			Uint8 unusedBeforeIconItem[2]; /* refused and frozen */
			Uint8 iconItem; /* the item whose icon its sprite shows */
		};

		/* The leader while it crosses a rope or tightrope. */
		struct {
			Uint8 unusedBeforeSavedPri[2]; /* refused and frozen */
			/* its sprite's priority, given back after */
			Uint8 savedSprPri;
		};

		/* A member of the party looking about in the Gabomba Statue
		   (scGabombaStatueLookAround). */
		struct {
			Uint8 unusedBeforeLookTmr[2]; /* refused and frozen */
			Uint8 lookTmr;				  /* ticks to its next turn */
			Uint8 lookTurned; /* nonzero while it faces lookOtherFacing */
		};

		/* A magical charge running along the Gabomba Statue's channel
		   (scGabombaStatueMoveCharge). */
		struct {
			Uint8 unusedBeforeChargeHeading[2]; /* refused and frozen */
			Uint8 chargeHeading; /* the quarter turn it runs toward */
			/* 1 while it runs, 2 as it sinks or rises off the channel (its
			   script then sets 0), 7 once it reaches the metatile (24, 14). */
			Uint8 chargeState;
		};

		/* A magical charge circling its orb statue in the Gabomba Statue
		   (scGabombaStatueSendCharge): its place on the circle, in 256ths of a
		   turn, and its own actor id. */
		struct {
			Uint8 unusedBeforeCirclePhase[2]; /* refused and frozen */
			Uint8 circlePhase;
			Uint8 circleActorId;
		};

		/* A tooth of a cog in the Gabomba Statue (scGabombaStatueTurnTooth):
		   its place in its turn. */
		struct {
			Uint8 unusedBeforeToothPhase[2]; /* refused and frozen */
			Uint8 toothPhase;
		};

		/* A fireball flying between a pair of statues at Magma Rock
		   (scMagmaRockMoveFireball). */
		struct {
			Uint8 unusedBeforeRollSide[2]; /* refused and frozen */
			/* 0 or 1: which statue of its pair it flies toward */
			Uint8 rollSide;
		};

		/* A part of a statue's rising flame at Magma Rock
		   (scMagmaRockRaiseFlamePart). */
		struct {
			Uint8 unusedBeforeFlameLevel[2]; /* refused and frozen */
			Uint8 flameLevel; /* its height in the column, 16 pixels a level */
		};

		/* A spark circling in to its centre, at Magma Rock's Blaze tablet
		   (scMagmaRockInsideOrbitParent). */
		struct {
			Uint8 unusedBeforeStepsLeft[2]; /* refused and frozen */
			Uint8 stepsLeft; /* its steps left before it is released */
		};

		/* An actor that blinks, as Magma Rock's statues and blasted wall do
		   (syStatueFlashUnderBurst). */
		struct {
			Uint8 unusedBeforeFlashMask[3]; /* refused, frozen and sceneCnt */
			/* its sprite flashes while TickCount & this is set */
			Uint8 flashMask;
		};

		/* A statue that launches a fireball (fdActorLaunchAhead), or the
		   fireball an attached actor sends rolling (scActorAttach). */
		struct {
			Uint8 unusedBeforeLaunchedActor[2]; /* refused and frozen */
			Uint8 launchedActorId; /* the actor sent rolling ahead */
		};

		/* An actor whose update flickers its colour
		   (scShamanVillageFlickerRock). */
		struct {
			Uint8 unusedBeforeFlicker[2]; /* refused and frozen */
			Uint8 flickerCol; /* the colour attribute shown on its lit ticks */
			/* lit every other tick at 0, every other two * at 1 */
			Uint8 flickerShift;
		};
	};

	/* Two halfwords whose meaning depends on what runs the actor. */
	union {
		struct {
			/* The parameter its actor script reads (a follow distance or an
			   angle to face). */
			Sint16 tmr;
			/* The side of the player's next footprint, or scene scratch. */
			Sint16 userValue;
		};

		/* Where SC_WANDER_HOME wanders around, in pixels: where it was created
		   or placed. */
		struct {
			Sint16 homeX;
			Sint16 homeZ;
		};

		/* Avoid's caster (fdEffectUpdateAvoidCaster). */
		struct {
			Sint16 arcPhase; /* 1 spins the caster 3/64 turn a tick */
			Sint16 arcFrm;	 /* ticks since the arcs started */
		};

		/* A Venus Djinni's falling motes
		   (fdEffectUpdateDescendingOrbitActor). */
		struct {
			Uint16 orbitFrm; /* ticks left of the spiral, 100 at the start */
			Sint16 orbitAng; /* its start on the circle, 1/65536 turns */
		};

		/* A Whirlwind particle (fdEffectCircleAnchor,
		   fdEffectWanderAroundAnchor). */
		struct {
			Uint16 swirlFrm; /* ticks it has circled or wandered so far */
			Sint16 turnFrm;	 /* ticks left of a sharp turn */
		};

		/* A spin that speeds up (fdEffectAdvanceAngWithRamp). */
		struct {
			Sint16 spinRampFrm; /* ticks the spin has been speeding up for */
		};

		/* The two headings a member of the party looking about turns between
		   (scGabombaStatueLookAround), 1/65536 turns. */
		struct {
			Uint16 lookFacing;
			Uint16 lookOtherFacing;
		};

		/* A charge's metatile of the channel (scGabombaStatueMoveCharge). */
		struct {
			Sint16 chargeColumn;
			Sint16 chargeRow;
		};
	};
	/* The field object (an actor or a field effect) its actor script follows or
	   faces. Scenes also hang other things on it: a rope or tightrope node, a
	   hook to call, a sparkle state, a VRAM address or a tick count. */
	union FIELD_OBJECT *watchedActor;
	/* The actor's per-tick hook (an OBJECT_UPDATE), kept as a plain pointer as
	   the effect options' hook is. */
	void *update;
	/* What a carrier moved this tick, which fdActorMoveWithCarriers adds to an
	   actor riding it, and that actor's carrier: the platform actor it stands
	   on, held as a field object. */
	Sint32				stepX;
	Sint32				stepY;
	Sint32				stepZ;
	union FIELD_OBJECT *carrier;
} ACTOR;

STATIC_ASSERT(sizeof(ACTOR) == 128);
STATIC_ASSERT(offsetof(ACTOR, collisionFlg) == 89);

void fdActorMoveHalfwayTowardTarget(ACTOR *actor);
void fdActorMoveHalfwayTowardTargetCopy(ACTOR *actor);

void fdActorStartIdleStub(ACTOR *);
void fdActorSetMoveTargetStub(ACTOR *, Sint32, Sint32, Sint32);
/* Waits up to 600 ticks for an actor to reach its movement target. */
void fdActorWaitForMoveStub(ACTOR *);
/* Switches every sprite of an actor, one or a group of four, to an
   animation. */
void fdActorSelectAnimationStub(ACTOR *, Sint32);
void fdActorWaitForScriptEndStub(ACTOR *);
void fdActorWaitUntilLanded(ACTOR *actor);
void sySndPlayStub(Sint32);

extern const Sint32 ActorFaceWatchedScript[];
extern const Sint32 ActorOffsetScript[];
extern const Uint8	ActorVariantScripts[];

/* Sets bits in one of an actor's flag bytes. */
static inline void syActorFlagsSet(Uint8 *flg, Uint8 bits)
{
	*flg |= bits;
}

#endif
