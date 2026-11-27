#ifndef ALCHEMY_VAULT_RANGE_H
#define ALCHEMY_VAULT_RANGE_H

/* @file */
/* Villagers who walk about and stop when someone is close in front of them, */
/* in Vault and Tolbi (shared/vault_range.c). */

#include "types.h"

struct ACTOR;

/* Stops the actor when the target is nearer than range pixels ahead of it; */
/* returns 1 when it stopped. */
s32 scVaultStopIfFacingNear(struct ACTOR *actor, struct ACTOR *target,
							s32 range, s32 force);

/* How near in front of it, in pixels, a walker stops for its partner, and for
 */
/* the leader: STOP_RANGE, or TALK_STOP_RANGE while a talk is pending or Mind */
/* Read is on. Vault's villagers stop so (scVaultStopIfFacingNear), and Tolbi's
 */
/* two running children stop the same way (scTolbiKidStopForTarget). */
#define PARTNER_STOP_RANGE 32
#define STOP_RANGE		   18
#define TALK_STOP_RANGE	   26

#endif
