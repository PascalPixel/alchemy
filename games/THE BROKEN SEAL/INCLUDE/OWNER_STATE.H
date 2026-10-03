#ifndef ALCHEMY_OWNER_STATE_H
#define ALCHEMY_OWNER_STATE_H

#include "BATTLE_RUNTIME.H"

/*
 * Some exact callers deliberately leave the owner id in r0 instead of
 * spelling it as a C argument.  Keep the legacy declaration non-prototyped
 * so those compiler-shaped calls remain representable.
 */

void *Owner_GetState();

#endif
