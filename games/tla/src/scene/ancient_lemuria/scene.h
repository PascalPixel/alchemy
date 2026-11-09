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

static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001a24(srcX, srcY, width, height, destX, destY, layer);
}

#endif
