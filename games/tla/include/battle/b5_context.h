#ifndef ALCHEMY_B5_CONTEXT_H
#define ALCHEMY_B5_CONTEXT_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

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

STATIC_ASSERT(offsetof(struct B5Context, object) == 0x00);
STATIC_ASSERT(offsetof(struct B5Context, word0c) == 0x0c);
STATIC_ASSERT(offsetof(struct B5Context, word10) == 0x10);
STATIC_ASSERT(offsetof(struct B5Context, suppressUpdates) == 0x2a);

void BattleEffect_TintBattler(s32 set_id, s32 object_value, s32 group_value,
                              s32 state_slot, s32 state_value);

#endif
