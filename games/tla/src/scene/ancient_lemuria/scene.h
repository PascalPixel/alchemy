#ifndef TLA_ANCIENT_LEMURIA_SCENE_H
#define TLA_ANCIENT_LEMURIA_SCENE_H

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "field_event.h"
#include "event_inline.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void Func_02000080(s32 id);
void Func_02000838(void);
void Func_020019f4(void);
void Func_02001a24(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY, s32 layer);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  Func_02001a24(srcX, srcY, width, height, destX, destY, layer);
}

#endif
