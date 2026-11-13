#ifndef TLA_MARS_LIGHTHOUSE_STAR_ROOM_H
#define TLA_MARS_LIGHTHOUSE_STAR_ROOM_H

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "event_inline.h"
#include "input.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Math_Cosine(s32 angle);
void Func_020041cc(struct Actor *object, s32 mode);
void Func_02001928(struct Actor *object, s32 priority);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *ObjectCreate_(s32 kind, s32 x, s32 y,
                                                      s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}
#include "main_routine_work.h"
#include "field_sprite.h"
#include "tla_edition.h"

void Actor_StartScript(struct Actor *object, const void *script);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
extern const s16 gData_020047fc[];

#endif
