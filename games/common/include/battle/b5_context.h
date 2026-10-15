#ifndef ALCHEMY_B5_CONTEXT_H
#define ALCHEMY_B5_CONTEXT_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

struct B5Context {
  void *object;
  u8 padding04[8];
  s32 word0c;
  s32 word10;
  u8 padding14[0x0c];
  void *excluded20;
  void *excluded24;
  u8 padding28[2];
  s16 suppressUpdates;
};

LAYOUT_OFFSET_GUARD(B5Context_ObjectOffset, struct B5Context, object, 0x00);
LAYOUT_OFFSET_GUARD(B5Context_Word0cOffset, struct B5Context, word0c, 0x0c);
LAYOUT_OFFSET_GUARD(B5Context_Word10Offset, struct B5Context, word10, 0x10);
LAYOUT_OFFSET_GUARD(B5Context_SuppressOffset, struct B5Context, suppressUpdates,
                    0x2a);

#if GAME_TLA
void ObjectGroup_UpdateMembers(s32 set_id, s32 object_value, s32 group_value,
                               s32 state_slot, s32 state_value);
#endif

#endif
