// The actor scripts the field's commands start: launching and turning, the dust puffs, the emote
// bubble and the movement types fdActorSetMovementType starts by number; and, among them, the
// message voice's rates.

#include "battle/battle.h"
#include "field/actor_script.h"
#include "field/script_commands.h"
#include "field/field.h"
#include "window/message_speed.h"

// clang-format off
const Sint32 ActorLaunchScript[] = {
	SC_SYNC,
	SC_WAIT(1),
	SC_ANIMATION(1),
	SC_CALL(fdActorStepAngle),
	SC_END,
};
// clang-format on

const Sint32 FieldEffectBurstParticleScriptA[] = {
	SC_WAIT(1),
	SC_LOOP(38, SC_TOP),
	SC_RELEASE,
};

const Sint32 FieldEffectBurstParticleScriptB[] = {
	SC_WAIT(1),
	SC_LOOP(18, SC_TOP),
	SC_RELEASE,
};

const Sint32 ActorStepAngleScript[] = {
	SC_CALL(fdActorStepAngle),
	SC_END,
};

// The ticks between the voice's sounds, by message speed option.
const Uint8 MessageSpeedVoiceRates[] = {
	[MESSAGE_SPEED_SLOW]   = 1,
	[MESSAGE_SPEED_NORMAL] = 3,
	[MESSAGE_SPEED_FAST]   = 10,
};

// The emote bubble's pop: it appears squashed to half its width and a quarter taller, springs back
// in smaller swings every 2 ticks until it is square, stays 30 ticks and is released.
const Sint32 EmoteBubbleScript[] = {
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

const Sint32 ActorWaitScript[] = {
	SC_WAIT(40),
	SC_RELEASE,
};

const Sint32 MovementTypeWanderScript[] = {
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

const Sint32 MovementTypeStandScript[]       = {SC_END};

const Sint32 MovementTypeResetActionScript[] = {
	SC_CALL(fdActorClearSpriteAndCollisionFlags),
	SC_END,
};

const Sint32 MovementTypeWanderNearScript[] = {
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

const Sint32 MovementTypeWanderCloseScript[] = {
	SC_SPEED(0.4),
	SC_ACCELERATION(0.2),
	SC_ANIMATION(2),
	SC_LABEL(1),
	SC_WANDER_HOME(2, 2, 6),
	SC_SYNC,
	SC_WAIT(4),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

const Sint32 MovementTypeFollowWatchedScript[] = {
	SC_CALL(fdActorMoveTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const Sint32 MovementTypeFaceLeaderScript[] = {
	SC_CALL(fdActorTurnTowardWatchedActor),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const Sint32 ActorFaceWatchedScript[] = {
	SC_FACE_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
