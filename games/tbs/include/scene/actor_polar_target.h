#ifndef ALCHEMY_ACTOR_POLAR_TARGET_H
#define ALCHEMY_ACTOR_POLAR_TARGET_H

/* @file */
/* Vale's Psynergy sparkles and the main image's field Psynergy */
/* (event_commands_field_psynergy.c), each with its own move-target function. */

/* Sends an actor toward the point magnitude (16.16 pixels) away from it in */
/* direction angle: sets its move target. */
#define DEFINE_SET_TARGET_POSITION_FROM_MAGNITUDE_ANGLE(linkage,               \
														setMoveTarget)         \
	linkage void fdActorSetTargetPosFromMagnitudeAng(                          \
		struct ACTOR *actor, s32 magnitude, s32 angle) {                       \
		s32 pos[3];                                                            \
                                                                               \
		if(actor != NULL) {                                                    \
			SET_VECTOR(pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);   \
			syVectorAddPolarOffset(magnitude, angle, pos);                     \
			setMoveTarget(actor, pos[0], pos[1], pos[2]);                      \
		}                                                                      \
	}

#endif
