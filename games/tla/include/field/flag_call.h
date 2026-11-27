#ifndef ALCHEMY_FLAG_CALL_H
#define ALCHEMY_FLAG_CALL_H

/* @file */
/* The flag tests and changes Izumo and the World Map's placed actors make */
/* through small inlines of their own, where the rest call event.h's */
/* fdEventGetFlag, fdEventSetFlag and fdEventClearFlag: written through those,
 */
/* Izumo's stone and its guards and scWorldMapUpdatePlacedActors compile */
/* otherwise. */

#include "types.h"
#include "system.h"

/* Whether a game flag is clear or set, and setting or clearing it. */
static inline s32 syFlagIsClear(s32 flag) { return !syGameFlagGet(flag); }
static inline s32 syFlagIsSet(s32 flag) { return syGameFlagGet(flag); }

/* Sets a game flag. The scenes and the field call it for no result, though */
/* the function returns the flag's old state: the ROM's callers are compiled */
/* against this form. */
void syGameFlagSet(s32 flag);
static inline void syFlagSet(s32 flag) { syGameFlagSet(flag); }

static inline void syFlagClear(s32 flag) { syGameFlagClear(flag); }

#endif
