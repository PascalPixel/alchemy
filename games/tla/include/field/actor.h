#ifndef ALCHEMY_ACTOR_H
#define ALCHEMY_ACTOR_H

/* @file */
/* Actors, their flags and the screen transition styles. */

#include "field/field_coordinate.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/* How a scene fades in and out: SCENE_TRANSITION's high byte, which */
/* fdScrTransitionStart and fdScrTransitionFinish read. */
typedef enum SCR_TRANSITION_STYLE {
	TRANSITION_FADE = 0,		  /* from or to black */
	TRANSITION_BACKDROP_FADE = 1, /* from or to the backdrop colour */
	TRANSITION_WINDOW = 2,		  /* a window shape the variant chooses */
	TRANSITION_DISSOLVE = 3,
	/* Opening from, or closing to, the centre line; the variant picks how. */
	TRANSITION_CENTRE_LINE = 4
} SCR_TRANSITION_STYLE;

/* TRANSITION_CENTRE_LINE's variants: the lines open or close steadily, or */
/* flicker every other frame as they close (fdScrTransitionUpdateFromCenter). */
typedef enum CENTRE_LINE_VARIANT {
	CENTRE_LINE_STEADY = 0,
	CENTRE_LINE_FLICKERING = 1
} CENTRE_LINE_VARIANT;

/* A scene transition as FIELD_STATE.transition holds it. */
#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))
/* The two bytes of a SCENE_TRANSITION: its SCR_TRANSITION_STYLE, and */
/* the variant that style reads. */
#define SCENE_TRANSITION_STYLE(transition)	 (((transition) >> 8) & 0xff)
#define SCENE_TRANSITION_VARIANT(transition) ((transition) & 0xff)

/* ACTOR.tgtX, targetY and targetZ when it has no movement target. */
#define ACTOR_NO_TARGET ((s32)0x80000000)

/* The actor id that ends a list of actors, or of rows that start with one. */
#define ACTOR_LIST_END 65535

/* The actors of Ram_HeapSlots->actors. */
#define ACTOR_COUNT 64

/* Sets an actor's position in one statement, as SET_POSITION does for a */
/* particle. */
#define SET_ACTOR_POSITION(p, fixedX, fixedY, fixedZ)                          \
	((p)->x.fixed = (fixedX), (p)->y.fixed = (fixedY), (p)->z.fixed = (fixedZ))

/* ACTOR.priFlg bits. */
typedef enum ACTOR_PRIORITY_FLAG {
	/* The sprite takes its priority from the map cell under it. */
	ACTOR_PRIORITY_AUTOMATIC = 0x01,
	/* The depth sort treats it as 320 pixels higher on the screen. */
	ACTOR_PRIORITY_UNDERFOOT = 0x02,
	/* The depth sort treats it as 320 pixels lower on the screen. */
	ACTOR_PRIORITY_OVERHEAD = 0x04,
	/* Drawn at the depth its sceneCnt byte holds, not its own place */
	/* (fdRenderApplyProjectedPlacementAtDepth). */
	ACTOR_PRIORITY_OWN_DEPTH = 0x08,
	ACTOR_PRIORITY_NOT_DRAWN = 0x10, /* placed nowhere, as if off screen */
	/* With ACTOR_PRIORITY_AUTOMATIC, takes the priority of the cells under it
	 */
	/* even where the ground in front rises, which would put it behind. */
	ACTOR_PRIORITY_IGNORES_RISES = 0x20,
	/* The actor steps up onto platform actors a cell below it, as the leader */
	/* does onto pushed blocks and statues. */
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
	/* Unused: nothing sets or reads it; a jump clears it with */
	/* ACTOR_FOLLOWS_TERRAIN. */
	ACTOR_MOTION_SPARE = 0x80,
	/* The motion flags a jump keeps as the actor leaves the ground: all but */
	/* ACTOR_FOLLOWS_TERRAIN and the spare bit. */
	ACTOR_JUMP_KEEPS_MOTION = (u8)~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE)
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
	ACTOR_FACES_MOTION = 0x01 /* turns toward its velocity, 1/16 turn a frame */
} ACTOR_STEERING_FLAG;

/* The actor snapshot (SnapshotObjects), as The Broken Seal's ActorSnapshot: */
/* how many actors it keeps, and the slot id that marks an empty place and */
/* ends its list. */
enum {
	ACTOR_SNAPSHOT_CAPACITY = 32, /* actors a snapshot keeps */
	ACTOR_SNAPSHOT_EMPTY = 255	  /* an empty slot, and the end of the list */
};

/* The axis a moving actor watches to know it has arrived at its target: the */
/* one along which it has the furthest to go. */
typedef enum ARRIVAL_AXIS {
	ARRIVAL_AXIS_NONE = 0, /* no arrival test; set again once it arrives */
	ARRIVAL_AXIS_X = 16,
	ARRIVAL_AXIS_Y = 17,
	ARRIVAL_AXIS_Z = 18
} ARRIVAL_AXIS;

union FIELD_OBJECT;
struct SPR;

/* An actor's per-frame hook, called with the actor (fdActorUpdateAllOnField).
 */
typedef void (*OBJECT_UPDATE)(union FIELD_OBJECT *object);

/* ACTOR.retainSprCache's bits, read by fdActorSystemPlaceSprs. */
typedef enum ACTOR_SPR_CACHE_FLAG {
	/* Keeps the sprite's VRAM cache entry while the field is suspended or the
	 */
	/* actor is off screen (fdActorSetItemIcon sets it). */
	ACTOR_KEEP_SPRITE_VRAM = 0x1,
	/* Keeps composing its picture while it is off screen. */
	ACTOR_COMPOSE_OFF_SCREEN = 0x2
} ACTOR_SPR_CACHE_FLAG;

/* An actor on the field: its script, position, motion toward a target, */
/* sprites and update hook. */
typedef struct ACTOR {
	const s32 *script;	/* the actor script it runs; null when it is free */
	s16 scriptCursor;	/* the script cursor, in words */
	u16 facing;			/* 1/65536 turns */
	FIELD_COORDINATE x; /* 16.16 fixed-point pixels */
	FIELD_COORDINATE y; /* 16.16 fixed-point pixels */
	FIELD_COORDINATE z; /* 16.16 fixed-point pixels */
	FIXED groundHeight; /* 16.16 fixed-point pixels */
	FIXED scaleX;		/* 16.16 fixed point, FIXED_ONE full size */
	FIXED scaleY;		/* 16.16 fixed point, FIXED_ONE full size */
	u16 radius;			/* collision radius in pixels */
	u8 layer;			/* the map layer its ground height is read from */
	/* ACTOR_PRIORITY_FLAG bits; fdActorSetSprPriorityDirect clears */
	/* ACTOR_PRIORITY_AUTOMATIC as it fixes its sprite's priority. */
	u8 priFlg;
	FIXED velX;		  /* 16.16 fixed-point pixels per frame */
	FIXED velY;		  /* 16.16 fixed-point pixels per frame */
	FIXED velZ;		  /* 16.16 fixed-point pixels per frame */
	FIXED speedLimit; /* 16.16 fixed-point pixels per frame */
	/* Added toward the target each frame, or taken off the speed when there is
	 */
	/* none, in 16.16 fixed-point pixels per frame. */
	FIXED acceleration;
	FIXED tgtX; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED tgtY; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED tgtZ; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	/* The share of its falling speed kept as it bounces on landing, 16.16 */
	/* fixed point. */
	FIXED bounce;
	/* Taken off the vertical velocity each frame while it is above the */
	/* ground, in 16.16 fixed-point pixels per frame. */
	FIXED gravity;
	/* Not read by the motion update; field code keeps its own values here (a */
	/* drifting speed, or the mark of a treasure spot that stands for a map */
	/* cell), and scene code a resting height. */
	s32 sceneWord;
	/* Its sprite for ACTOR_SPRITE_ONE (read as a FIELD_SPR or a struct
	 */
	/* Sprite), or its list of sprites for ACTOR_SPRITE_GROUP, which code that
	 */
	/* reads it converts. */
	struct SPR *spr;
	/* ACTOR_SPRITE_ONE or ACTOR_SPRITE_GROUP in the low four bits, which is all
	 */
	/* the field and the scenes test. */
	u8 sprStorageKind;
	u8 motionFlg; /* ACTOR_MOTION_FLAG bits */
	/* The ARRIVAL_AXIS_* watched for arrival at the target; ARRIVAL_AXIS_NONE
	 */
	/* once it arrives, and cleared with the target. */
	u8 arrivalAxis;
	u8 conditionResult; /* the actor script's last flag test */
	u8 snapToTgt;		/* nonzero lands it exactly on its target */
	u8 collisionFlg;	/* ACTOR_COLLISION_FLAG bits */
	u8 steeringFlg;		/* ACTOR_STEERING_FLAG bits */
	u8 paused;
	/* ACTOR_SPR_CACHE_FLAG bits: what its sprite keeps while the field is */
	/* suspended or the actor is off screen (fdActorSystemPlaceSprs). */
	u8 retainSprCache;
	u8 repeatCnt; /* the actor script's loop count */
	s16 wait;	  /* the actor script's wait, in frames */
	/* The field's effects keep a hook here; a scene whose actor bobs keeps its
	 */
	/* own frame count in the third byte instead. */
	union {
		void *func;
		struct {
			u8 refused; /* frames in a row a solid actor has refused its move */
			u8 frozen;
			u8 sceneCnt; /* frames into the bob, its low six bits a cycle */
			/* A block's: nonzero while it blocks the cells ahead of and behind
			 * it. */
			u8 sceneByte;
		};
		/* The ship on the World Map (FieldMoveByKeys): frames to its next wake,
		 */
		/* and a wake's own frames. */
		struct {
			u8 unusedBeforeWake[2]; /* refused and frozen */
			u8 wakeTmr;				/* frames to the ship's next wake */
			u8 wakeFrm;				/* a wake's frames left */
		};
		/* An actor showing an item (fdActorSetItemIcon). */
		struct {
			u8 unusedBeforeIconItem[2]; /* refused and frozen */
			u8 iconItem; /* the item whose icon its sprite shows */
		};
		/* The leader while it crosses a rope or tightrope. */
		struct {
			u8 unusedBeforeSavedPri[2]; /* refused and frozen */
			u8 savedSprPri; /* its sprite's priority, given back after
							 */
		};
		/* A member of the party looking about in the Gabomba Statue */
		/* (scGabombaStatueLookAround). */
		struct {
			u8 unusedBeforeLookTmr[2]; /* refused and frozen */
			u8 lookTmr;				   /* frames to its next turn */
			u8 lookTurned; /* nonzero while it faces lookOtherFacing */
		};
		/* A magical charge running along the Gabomba Statue's channel */
		/* (scGabombaStatueMoveCharge). */
		struct {
			u8 unusedBeforeChargeHeading[2]; /* refused and frozen */
			u8 chargeHeading; /* the quarter turn it runs toward */
			/* 1 while it runs, 2 as it sinks or rises off the channel (its
			 * script */
			/* then sets 0), 7 once it reaches the cell (24, 14). */
			u8 chargeState;
		};
		/* A magical charge circling its orb statue in the Gabomba Statue */
		/* (scGabombaStatueSendCharge): its place on the circle, in 256ths of a
		 */
		/* turn, and its own actor id. */
		struct {
			u8 unusedBeforeCirclePhase[2]; /* refused and frozen */
			u8 circlePhase;
			u8 circleActorId;
		};
		/* A tooth of a cog in the Gabomba Statue (scGabombaStatueTurnTooth):
		 * its */
		/* place in its turn. */
		struct {
			u8 unusedBeforeToothPhase[2]; /* refused and frozen */
			u8 toothPhase;
		};
		/* A fireball flying between a pair of statues at Magma Rock */
		/* (scMagmaRockMoveFireball). */
		struct {
			u8 unusedBeforeRollSide[2]; /* refused and frozen */
			u8 rollSide; /* 0 or 1: which statue of its pair it flies toward */
		};
		/* A part of a statue's rising flame at Magma Rock */
		/* (scMagmaRockRaiseFlamePart). */
		struct {
			u8 unusedBeforeFlameLevel[2]; /* refused and frozen */
			u8 flameLevel; /* its height in the column, 16 pixels a level */
		};
		/* A spark circling in to its centre, at Magma Rock's Blaze tablet */
		/* (scMagmaRockInsideOrbitParent). */
		struct {
			u8 unusedBeforeStepsLeft[2]; /* refused and frozen */
			u8 stepsLeft; /* its steps left before it is released */
		};
		/* An actor that blinks, as Magma Rock's statues and blasted wall do */
		/* (syStatueFlashUnderBurst). */
		struct {
			u8 unusedBeforeFlashMask[3]; /* refused, frozen and sceneCnt */
			u8 flashMask; /* its sprite flashes while FrameCount & this is set
						   */
		};
		/* A statue that launches a fireball (fdActorLaunchAhead), or the
		 * fireball */
		/* an attached actor sends rolling (scActorAttach). */
		struct {
			u8 unusedBeforeLaunchedActor[2]; /* refused and frozen */
			u8 launchedActorId;				 /* the actor sent rolling ahead */
		};
		/* An actor whose update flickers its colour
		 * (scShamanVillageFlickerRock). */
		struct {
			u8 unusedBeforeFlicker[2]; /* refused and frozen */
			u8 flickerCol;	 /* the colour attribute shown on its lit frames */
			u8 flickerShift; /* lit every other frame at 0, every other two at 1
							  */
		};
	};
	/* Two halfwords whose meaning depends on what runs the actor. */
	union {
		struct {
			/* The parameter its actor script reads (a follow distance or an */
			/* angle to face). */
			s16 tmr;
			/* The side of the player's next footprint, or scene scratch. */
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
		/* The two headings a member of the party looking about turns between */
		/* (scGabombaStatueLookAround), 1/65536 turns. */
		struct {
			u16 lookFacing;
			u16 lookOtherFacing;
		};
		/* A charge's cell of the channel (scGabombaStatueMoveCharge). */
		struct {
			s16 chargeColumn;
			s16 chargeRow;
		};
	};
	/* The field object (an actor or a field effect) its actor script follows */
	/* or faces. Scenes also hang other things on it: a rope or tightrope node,
	 */
	/* a hook to call, a sparkle state, a VRAM address or a frame count. */
	union FIELD_OBJECT *watchedActor;
	/* The actor's per-frame hook (an OBJECT_UPDATE), kept as a plain pointer as
	 */
	/* the effect options' hook is. */
	void *update;
	/* What a carrier moved this frame, which fdActorMoveWithCarriers adds to */
	/* an actor riding it, and that actor's carrier: the platform actor it */
	/* stands on, held as a field object. */
	s32 stepX;
	s32 stepY;
	s32 stepZ;
	union FIELD_OBJECT *carrier;
} ACTOR;

STATIC_ASSERT(sizeof(ACTOR) == 128);
STATIC_ASSERT(offsetof(ACTOR, collisionFlg) == 89);

void fdActorMoveHalfwayTowardTarget(ACTOR *actor);
void fdActorMoveHalfwayTowardTargetCopy(ACTOR *actor);

void fdActorStartIdleStub(ACTOR *);
void fdActorSetMoveTargetStub(ACTOR *, s32, s32, s32);
/* Waits up to 600 frames for an actor to reach its movement target. */
void fdActorWaitForMoveStub(ACTOR *);
/* Switches every sprite of an actor, one or a group of four, to an animation.
 */
void fdActorSelectAnimationStub(ACTOR *, s32);
void fdActorWaitForScriptEndStub(ACTOR *);
void fdActorWaitUntilLanded(ACTOR *actor);
void sySndPlayStub(s32);

extern const s32 Actor_FaceWatchedScript[];
extern const s32 Actor_OffsetScript[];
extern const u8 Actor_VariantScripts[];

/* Sets bits in one of an actor's flag bytes. */
static inline void syActorFlagsSet(u8 *flags, u8 bits) { *flags |= bits; }

#endif
