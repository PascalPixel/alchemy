/* The actor scripts the field's commands start: launching and turning, the dust
   puffs, the emote bubble and the movement types fdActorSetMovementType starts
   by number; and, among them, the message voice's rates. */

#include "battle/battle.h"
#include "field/actor_script.h"
#include "field/script_commands.h"
#include "field/field.h"
#include "window/message_speed.h"

/* clang-format off */
const Sint32 Actor_LaunchScript[] = {
	SC_SYNC,
	SC_WAIT(1),
	SC_ANIMATION(1),
	SC_CALL(fdActorStepAng),
	SC_END,
};
/* clang-format on */

const Sint32 FieldEffect_BurstParticleScriptA[] = {
	SC_WAIT(1),
	SC_LOOP(38, SC_TOP),
	SC_RELEASE,
};

const Sint32 FieldEffect_BurstParticleScriptB[] = {
	SC_WAIT(1),
	SC_LOOP(18, SC_TOP),
	SC_RELEASE,
};

const Sint32 Actor_StepAngleScript[] = {
	SC_CALL(fdActorStepAng),
	SC_END,
};

/* The frames between the voice's sounds, by message speed option. */
const Uint8 MessageSpeed_VoiceRates[] = {
	[MESSAGE_SPEED_SLOW] = 1,
	[MESSAGE_SPEED_NORMAL] = 3,
	[MESSAGE_SPEED_FAST] = 10,
};

/* The emote bubble's pop: it appears squashed to half its width and a quarter
   taller, springs back in smaller swings every 2 frames until it is square,
   stays 30 frames and is released. */
const Sint32 Emote_BubbleScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 2),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 4),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 64),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(2),
	SC_WAIT(30),
	SC_RELEASE,
};

const Sint32 Actor_WaitScript[] = {
	SC_WAIT(40),
	SC_RELEASE,
};

const Sint32 MovementType_WanderScript[] = {
	SC_SPEED(0.5),
	SC_ACCELERATION(0.05),
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
	SC_LABEL(1),
	SC_ANIMATION(2),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

const Sint32 MovementType_StandScript[] = {SC_END};

const Sint32 MovementType_ResetActionScript[] = {
	SC_CALL(fdActorClearSprAndCollisionFlags),
	SC_END,
};

const Sint32 MovementType_WanderNearScript[] = {
	SC_SPEED(0.5),
	SC_ACCELERATION(0.05),
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
	SC_LABEL(1),
	SC_ANIMATION(2),
	SC_WANDER_HOME(2, 2, 8),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_WANDER_HOME(2, 2, 8),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_WANDER_HOME(2, 2, 8),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

const Sint32 MovementType_WanderCloseScript[] = {
	SC_SPEED(0.4),
	SC_ACCELERATION(0.2),
	SC_ANIMATION(2),
	SC_LABEL(1),
	SC_WANDER_HOME(2, 2, 6),
	SC_SYNC,
	SC_WAIT(4),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

const Sint32 MovementType_FollowWatchedScript[] = {
	SC_CALL(fdActorMoveTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const Sint32 MovementType_FaceLeaderScript[] = {
	SC_CALL(fdActorTurnTowardWatchedActor),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const Sint32 Actor_FaceWatchedScript[] = {
	SC_FACE_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
