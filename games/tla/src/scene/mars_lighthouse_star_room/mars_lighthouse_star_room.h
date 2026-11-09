#ifndef TLA_MARS_LIGHTHOUSE_STAR_ROOM_H
#define TLA_MARS_LIGHTHOUSE_STAR_ROOM_H

#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "event_inline.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_020043f4(void);
void Func_02004444(void);
void Func_020041cc(struct Actor *object, s32 mode);
void Func_020017c8(void);
void Func_02001a9c();
void Func_02001928(struct Actor *object, s32 priority);
void Func_02001b38(struct Actor *object, s32 priority);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *ObjectCreate_(s32 kind, s32 x, s32 y,
                                                      s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}
#include "main_routine_work.h"
#include "field_sprite.h"
#include "tla_edition.h"

void Func_020041dc(struct Actor *object, const void *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Func_02002738(void);
extern u32 gInput;
extern const s16 gData_020047fc[];

#endif
