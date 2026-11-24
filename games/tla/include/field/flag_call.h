#ifndef ALCHEMY_FLAG_CALL_H
#define ALCHEMY_FLAG_CALL_H

/// @file
/// The flag tests and changes Izumo and the World Map's placed actors make
/// through small inlines of their own, where the rest call event.h's
/// Event_GetFlag, Event_SetFlag and Event_ClearFlag: written through those,
/// Izumo's stone and its guards and WorldMap_UpdatePlacedActors compile
/// otherwise.

#include "types.h"

s32 GameFlag_Get(s32 flag);
void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);

/// Whether a game flag is clear or set, and setting or clearing it.
static inline s32 Flag_IsClear(s32 flag) { return !GameFlag_Get(flag); }
static inline s32 Flag_IsSet(s32 flag) { return GameFlag_Get(flag); }

static inline void Flag_Set(s32 flag) { GameFlag_Set(flag); }

static inline void Flag_Clear(s32 flag) { GameFlag_Clear(flag); }

#endif
