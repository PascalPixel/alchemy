#ifndef TLA_CONTIGO_H
#define TLA_CONTIGO_H

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct ContigoObject;

void Func_020040e4(s32 actor, s32 target);
void Func_0200406c(void);
void Func_02004074(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_WaitForAnimationChange(s32 actor);
void Func_0200419c(void);
void Func_02003e54(struct ContigoObject *object, const void *effect);
void Func_02003f54(struct ContigoObject *object);
void Func_0200420c(void);
void Func_02004044(void);
void Func_02001938(s32 mode);
void Func_02001c5c(s32 a, s32 b, s32 c, s32 d);
void Func_0200423c(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY, s32 mode);

static inline void Contigo_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH(pin): the height pinned to r1, so that the actor loads after the
   * height and the frames, as the native has; as an unpinned local, 2 of 8
   * instructions differ in tla-en (Func_020007a4). */
  register s32 h __asm__("r1") = height;
  s32 f = frames;
  s32 a = actor;

  Func_080d47b4(a, h, f);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Contigo_Tiles(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 mode) {
  Func_0200423c(srcX, srcY, width, height, destX, destY, mode);
}

#endif
