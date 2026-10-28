/// @file
/// Objects: create and release field objects, and render them each frame.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "objdisp.h"
#include "resource.h"
#include "metadata_lookup.h"
#include "actor.h"
#include "object_dispatch.h"
#include "io_reg.h"
#include "dma.h"
#include "ramcode.h"
#include "battle_system.h"
#include "transform.h"
#include "macros.h"

#include "ram_buffer.h"

/// Returns an unused object (one without a script) from the 64 slots, or 0
/// if none is free.
void *Actor_FindFreeObject(void) {
  struct Actor *obj = Ram_HeapSlots->scriptObjects;
  void *ret = 0;
  s32 n = 0;

  while (n <= 63) {
    if (obj->script == 0) {
      ret = obj;
      break;
    }
    n++;
    obj++;
  }
  return ret;
}

/// Releases the object's graphics and zeroes the object to free it.
/// obj: object.
void Actor_Release(struct DispatchObject *obj) {
  volatile u32 zero;
  volatile u32 *src;
  s32 n;
  struct ResourceObjectWork **child;

  if (obj) {
    switch (obj->kind & 15) {
    case 1: // One graphic.
      ResourceObject_Release((struct ResourceObjectWork *)obj->target.child);
      break;
    case 2: // Up to four graphics.
      child = (struct ResourceObjectWork **)obj->target.children;
      n = 3;
      do {
        struct ResourceObjectWork *res = *child++;
        if (res)
          ResourceObject_Release(res);
      } while (--n >= 0);
      break;
    }
    src = &zero;
    *src = 0;
    Dma_Set(src, obj, 0x85000000 | (sizeof(struct Actor) / 4),
            REG_DMA3);
  }
}

