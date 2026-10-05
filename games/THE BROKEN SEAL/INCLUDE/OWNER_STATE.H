#ifndef ALCHEMY_OWNER_STATE_H
#define ALCHEMY_OWNER_STATE_H

#include "BATTLE_RUNTIME.H"

/* Some callers pass the owner id already held in r0 without a C argument. */

void *Owner_GetState();

#endif
