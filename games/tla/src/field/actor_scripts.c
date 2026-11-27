/* @file */
/* The actor scripts the field and its scenes run. */

#include "field/actor.h"
#include "field/field_sequence_functions.h"
#include "field/actor_script.h"
#include "field/sprite.h"

/* Each SC_CALL function takes the actor and returns nonzero to go on with the
 */
/* script this frame. They take different views of the actor, so the */
/* declarations leave the parameters unspecified. */
s32 fdActorStepAng();
s32 fdActorClearSprAndCollisionFlags();
s32 fdActorClearSprFlags();
s32 fdActorIgnoreRises();
s32 fdActorMoveTowardTarget();
s32 fdActorTurnTowardWatchedActor();

/* Unused: three rows of 0, 2, 4, 2 that nothing reads. */
/* clang-format off */
const u8 ScreenTransition_ShapesTail[12] = {
    0, 2, 4, 2,
    0, 2, 4, 2,
    0, 2, 4, 2,
};

/* {key, value} rows ended by a key of -1; this list has only the end. */
const s32 PairTable[2] = {
    -1, -1,
};

/* The pixels of an 8 by 8 tile (a pixel's row times 8 plus its column), one */
/* diagonal at a time from the top left corner, each diagonal ended by -1; */
/* -4 follows the longest diagonal and -9 the last. fdHeadingClose clears a */
/* diagonal of every tile each two frames (fdHeadingWipeTilePixel) and tests */
/* only for a negative entry and for -9; the last entry pads the table. */
const s16 Heading_WipeSteps[80] = {
    0, -1,
    1, 8, -1,
    2, 9, 16, -1,
    3, 10, 17, 24, -1,
    4, 11, 18, 25, 32, -1,
    5, 12, 19, 26, 33, 40, -1,
    6, 13, 20, 27, 34, 41, 48, -1,
    7, 14, 21, 28, 35, 42, 49, 56, -4,
    15, 22, 29, 36, 43, 50, 57, -1,
    23, 30, 37, 44, 51, 58, -1,
    31, 38, 45, 52, 59, -1,
    39, 46, 53, 60, -1,
    47, 54, 61, -1,
    55, 62, -1,
    63, -9,
    0,
};
/* clang-format on */

/* The walk-by script (fdActorWalkByById): waits for the actor to arrive, then
 */
/* stands it in its first animation. */
const s32 Actor_OffsetScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The launch script (fdActorLaunchFromLeader): waits for the actor to arrive */
/* and a frame more, stands it, then turns it a step toward the facing in its */
/* timer. */
const s32 Actor_LaunchScript[] = {
	SC_SYNC, SC_WAIT(1), SC_ANIMATION(ANIM_STAND), SC_CALL(fdActorStepAng),
	SC_END,
};

/* Turns the actor a step toward the facing in its timer. */
const s32 Actor_StepAngleScript[] = {
	SC_CALL(fdActorStepAng),
	SC_END,
};

/* The countdown between the sounds of a message's voice as it types, by the */
/* message speed the player chose (fdMessageSetVoiceFromActor). */
const u8 MessageSpeed_VoiceRates[4] = {
	1,
	3,
	10,
	0,
};

/* An emote bubble's script (fdActorShowEmoteById): it pops up, squashing and */
/* stretching, stays 30 frames and is released. */
const s32 Emote_BubbleScript[] = {
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 2),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 4),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 4),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 32),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 64),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 32),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
	SC_WAIT(2),
	SC_WAIT(30),
	SC_RELEASE,
};

/* Waits 40 frames, then releases the actor: the one-off effect */
/* fdEffectSetAttached shows over an actor. */
const s32 Actor_WaitScript[] = {
	SC_WAIT(40),
	SC_RELEASE,
};

/* MOVEMENT_TYPE_WANDER: wanders near home, pausing between steps. */
const s32 MovementType_WanderScript[] = {
	SC_SPEED(0.5),
	SC_ACCELERATION(0.05),
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* MOVEMENT_TYPE_STAND: does nothing, as The Broken Seal's Actor_IdleScript. */
const s32 MovementType_StandScript[] = {
	SC_END,
};

/* MOVEMENT_TYPE_CLEAR_COLLISION: clears the actor's sprite and collision */
/* flags. */
const s32 MovementType_ClearCollisionScript[] = {
	SC_CALL(fdActorClearSprAndCollisionFlags),
	SC_END,
};

/* MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS: clears the actor's sprite flags. */
const s32 MovementType_ClearSpriteFlagsScript[] = {
	SC_CALL(fdActorClearSprFlags),
	SC_END,
};

/* MOVEMENT_TYPE_IGNORE_RISES: the actor takes its cells' priority even where */
/* the ground in front rises. */
const s32 MovementType_IgnoreRisesScript[] = {
	SC_CALL(fdActorIgnoreRises),
	SC_END,
};

/* MOVEMENT_TYPE_WANDER_NEAR: wanders a shorter way near home. */
const s32 MovementType_WanderNearScript[] = {
	SC_SPEED(0.5),
	SC_ACCELERATION(0.05),
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(2, 2, 8),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(2, 2, 8),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(2, 2, 8),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* MOVEMENT_TYPE_WANDER_CLOSE: wanders near home without pausing. */
const s32 MovementType_WanderCloseScript[] = {
	SC_SPEED(0.4), SC_ACCELERATION(0.2),		  SC_ANIMATION(ANIM_WALK),
	SC_LABEL(1),   SC_WANDER_HOME(2, 2, 6),		  SC_SYNC,
	SC_WAIT(4),	   SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/* MOVEMENT_TYPE_SQUASH: squashes and stretches, two bounces at a time, for */
/* ever. */
const s32 MovementType_SquashScript[] = {
	SC_LABEL(1),
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(4),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/* MOVEMENT_TYPE_FOLLOW_WATCHED: moves the actor toward its watched actor */
/* every frame. */
const s32 MovementType_FollowWatchedScript[] = {
	SC_CALL(fdActorMoveTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* MOVEMENT_TYPE_FACE_LEADER: turns the actor toward its watched actor, the */
/* leader, every frame. */
const s32 MovementType_FaceLeaderScript[] = {
	SC_CALL(fdActorTurnTowardWatchedActor),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* MOVEMENT_TYPE_TAKE_LEADER_LAYER: keeps the leader's display layer, every */
/* frame. */
const s32 MovementType_TakeLeaderLayerScript[] = {
	SC_CALL(fdActorTakeLeaderLayer),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Faces the watched actor every frame, forever. */
const s32 Actor_FaceWatchedScript[] = {
	SC_FACE_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
