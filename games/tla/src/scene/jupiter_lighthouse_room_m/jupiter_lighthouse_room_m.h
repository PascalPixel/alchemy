#ifndef TLA_JUPITER_LIGHTHOUSE_ROOM_M_H
#define TLA_JUPITER_LIGHTHOUSE_ROOM_M_H

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x,
                                      s32 y, s32 z);
void wait(s32 frames);
void Event_Wait(s32 frames);
void Func_020040b4(void);
struct Actor *Func_080d4568(void);

#endif
