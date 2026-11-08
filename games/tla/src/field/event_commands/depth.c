#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct DepthPart {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct DepthMapBase {
  u8 unk00[4];
  s32 base;
};

struct DepthScroll {
  s32 unk00;
  s32 offset;
};

struct DepthMap {
  struct DepthMapBase *base;
  u8 unk04[0xe0];
  struct DepthScroll scroll;
};

s32 Render_ProjectPoint(const void *point, s32 *screen);
void Func_080eb01c(void *work);

s32 Func_080db9a8(struct Actor *object, s32 kind) {
  if (kind == 0xc000)
    return ((struct DepthPart *)object->animation)->priority;
  return 0;
}

/// Returns the 2-bit OBJ priority of the object's animation record.
s32 BattleEffect_GetObjectPriority(struct Actor *object) {
  return ((struct DepthPart *)object->animation)->priority;
}

/// Returns the object's sort depth: from its projected z on the world map
/// (mode 3), otherwise from its y and z against the map's scroll.
s32 BattleEffect_GetObjectDepth(struct Actor *object) {
  s32 point[3];
  s32 screen[3];
  s32 depth;
  s32 *out;

  if (*((s8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    point[0] = object->x;
    out = screen;
    /* FAKEMATCH: ⚓️ addresses the screen point before it copies y */
    asm("" : "+l"(out), "+l"(object));
    point[1] = object->y;
    point[2] = object->z;
    Render_ProjectPoint(point, out);
    depth = (0x200 - out[2]) / 2 + 128;
    if (depth <= 0)
      depth = 1;
  } else {
    struct DepthMap *map = (struct DepthMap *)Ram_HeapSlots->mapWork;
    struct DepthScroll *scroll = &map->scroll;
    s32 bias = scroll->offset & 0xffff0000;
    s32 base = map->base->base;

    depth = ((object->y - base + object->z - base - bias) >> 16) + 62;
  }
  return depth;
}
