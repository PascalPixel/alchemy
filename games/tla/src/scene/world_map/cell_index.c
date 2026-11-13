/// @file
/// Scene code of overlay 0x64b, in ROM order, from cell_index's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "frame.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/scene/staff_credit.h"
#include "module/scene/world_map.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);
/* VIEW(cell_index): routine Resource_DecodeType01 as void Resource_DecodeType01(const u8 *a, u8 *b); resource.h has s32 Resource_DecodeType01(const void *source, void *destination). */
extern void Resource_DecodeType01__cell_index(const u8 *a, u8 *b) __asm__("Resource_DecodeType01");
u32 __umodsi3(u32 a, u32 b);

s32 Func_02003988(s32 x, s32 z) { return x / 0x100000 + (z / 0x100000) * 128; }

s32 Func_020039a8(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(step << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = parent->x;
  object->y += 0x10000;
  object->z = parent->z;
  return 1;
}
