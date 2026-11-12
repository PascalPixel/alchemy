#ifndef TLA_MADRA_INTERIORS_H
#define TLA_MADRA_INTERIORS_H

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern struct ActorPlacement gData_02002240[];
extern struct ActorPlacement gData_02002480[];
extern struct ActorPlacement gData_02002750[];
extern struct ActorPlacement gData_02002828[];
extern struct Trigger gData_020028a0[];
extern struct Trigger gData_02002bb8[];
extern struct Trigger gData_02002c9c[];
extern struct Trigger gData_02002da4[];
extern struct Trigger gData_0200308c[];

void Func_02001d50(void);
void Func_02001d58(void);
void Func_02001f00(s32 actor);
void Func_02001f08(s32 slot, s32 actor);
void Func_020007dc(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 FlagCheck(s32 flag) {
  return GameFlag_Test(flag);
}

static inline void ActorTurn(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static inline void ActorPlace(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

#endif
