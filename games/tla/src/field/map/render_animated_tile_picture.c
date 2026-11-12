#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "system.h"

#include "vram_tab.h"
#include "ram_buffer.h"

struct AnimTrack {
  u8 unk00[0x10];
  u8 *script;
  u8 index;
};

struct AnimObject {
  u8 unk00[0x18];
  u8 frame;
  u8 unk19[0xf];
  struct AnimTrack *track;
};

void Animation_WaitForTrackEnd(struct AnimObject *object) {
  u8 frame = object->frame;
  struct AnimTrack *track = object->track;
  s32 i;

  if (object != 0) {
    i = 0;
    if (track->script[track->index] != 241) {
      do {
        wait(1);
        i++;
        if (i > 89)
          break;
        if (track->script[track->index] == 241)
          break;
      } while (object->frame == frame);
    }
  }
}

void Map_RenderAnimatedTilePicture(u8 *object, u32 position) {
  u16 *destination;
  s32 count;
  u32 row;
  u8 *table;
  u32 highMask;
  u32 indexMask;
  u8 offsetMask;

  destination =
      (u16 *)(0x06010000 + gResourceTableEntries[object[0x10]].offset);
  count = (object[0x14] * object[0x15]) / 64;
  row = 0;

  if (row < (u32)count) {
    table = gMap_TileDissolveOrder;
    highMask = 0xFF00;
    indexMask = 0x3F;
    offsetMask = 0x3E;

    for (; row < (u32)count; row++, destination += 32, position++) {
      if ((u32)(position - 0x40) <= 0x3F) {
        u8 entry;
        u16 *cell;
        u32 parity;

        entry = table[(position + row * 16) & indexMask];
        cell = (u16 *)((u8 *)destination + (entry & offsetMask));
        parity = 1;
        parity &= entry;
        if (parity != 0)
          *cell = *(u8 *)cell;
        else
          *cell &= highMask;
      }
    }
  }
}

void Map_RenderAnimatedTilePicturesForObject(u8 *object) {
  u32 pos;

  pos = 0;
  do {
    Map_RenderAnimatedTilePicture(object, pos);
    Map_RenderAnimatedTilePicture(object, pos + 1);
    Map_RenderAnimatedTilePicture(object, pos + 2);
    Map_RenderAnimatedTilePicture(object, pos + 3);
    pos += 4;
    wait(1);
  } while (pos <= 0x7f);
}

void Map_RenderAllAnimatedTilePictures(u8 **table, s32 cnt) {
  u8 **top;
  u8 **p;
  s32 n;
  u32 pos;
  u32 pos1;
  u32 pos2;
  u32 pos3;

  top = table;
  pos = 0;
  do {
    if (cnt > 0) {
      pos1 = pos + 1;
      p = top;
      pos2 = pos + 2;
      pos3 = pos + 3;
      n = cnt;
      do {
        Map_RenderAnimatedTilePicture(*p, pos);
        Map_RenderAnimatedTilePicture(*p, pos1);
        Map_RenderAnimatedTilePicture(*p, pos2);
        n -= 1;
        Map_RenderAnimatedTilePicture(*p++, pos3);
      } while (n != 0);
    }
    wait(1U);
    pos += 4;
  } while (pos <= 0x7FU);
}

struct Thing {
  u8 filler0[8];
  s32 field8;
  u8 filler12[4];
  s32 field16;
};

s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out) {
  u8 *state = (u8 *)Ram_HeapSlots->mapWork;
  s32 *org = (s32 *)(state + 228);
  s32 a;
  s32 b;
  s32 x;
  s32 y;

  a = org[0] & 0xffff0000;
  b = org[1] & 0xffff0000;
  x = obj->field8 - a;
  y = obj->field16 - b;
  if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
    *out++ = x >> 16;
    *out = y >> 16;
    return 0;
  }
  *out++ = 0;
  *out = 0;
  return -1;
}

#include "heap_release.h"
#include "dma.h"
#include "io_reg.h"
#include "task.h"

struct MapAnimationWork {
  u8 unk00[6];
  u8 unk06;
  u8 unk07;
  u8 unk08[84];
};

void Func_0802493c(void);
void Func_08023f9c(void);
void Func_080246b8(void);
void Func_0802386c(void);

/// Sets up the map animation work for map mode: its 92-byte work and an 8 KB
/// buffer, both cleared by DMA from one zero word, and the animation callbacks
/// (mode 4 runs its own; modes 3 and 4 take ObjectSystem_UpdateCameraFixed at
/// 0x480, the others Func_0802386c).
// No ☀️ twin found; ☀️'s field.c starts its map animations the same way.
void Func_080230e0(s32 mode) {
  struct MapAnimationWork *work;
  void *buffer;
  // The fill word Dma_Fill32Cell reads its zero from, one for both fills.
  volatile u32 zero;

  work = HeapSlot_AllocEwram(24, sizeof(*work));
  buffer = HeapSlot_AllocEwram(20, 0x2000);
  ObjectSystem_Configure(mode);
  Dma_Fill32Cell(zero, 0, buffer, 0x2000);
  Dma_Fill32Cell(zero, 0, work, sizeof(*work));
  if (mode == 4) {
    Task_Add(Func_0802493c, TASK_PASS_VBLANK + 10);
  } else {
    Task_Add(Func_08023f9c, TASK_PASS_VBLANK + 11);
    Task_Add(Func_080246b8, TASK_PASS_VBLANK + 10);
  }
  if (mode == 3 || mode == 4)
    Task_Add(ObjectSystem_UpdateCameraFixed, TASK_PASS_VBLANK);
  else
    Task_Add(Func_0802386c, TASK_PASS_VBLANK);
  work->unk06 = 15;
  work->unk07 = 0;
}

/// Two empty functions follow; nothing in the ROM refers to either.
void Func_0802319c(void) {}

void Func_080231a0(void) {}

// Objects: create and release field objects, and render them each frame.

#include "edition.h"
#include "objdisp.h"
#include "resource.h"
#include "sprite_metadata.h"
#include "actor.h"
#include "actor_view.h"
#include "ram_code.h"
#include "battle_system.h"
#include "transform.h"
#include "macros.h"

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
void Actor_Release(struct ActorView *obj) {
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
    Dma_Set(src, obj,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
                (sizeof(struct Actor) / 4),
            REG_DMA3);
  }
}

#include "sprite.h"

struct Actor *Func_08023220(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object;
  void *sprite;
  s32 kind;
  void **entry;
  volatile u32 zero;

  kind = id / 4096;
  id &= 0xfff;
  object = Actor_FindFreeObject();
  if (object != NULL) {
    object->radius = 16;
    switch (kind) {
    case 0:
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        object->animationKind = 1;
        object->animation = sprite;
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
      } else {
        object->animationKind = 0;
      }
      break;
    case 2:
      entry = (void **)((u32 *)Ram_HeapSlots->objectSystem +
                        Ram_HeapSlots->objectSystem->count++ + 2);
      object->animationKind = kind;
      object->animation = entry;
      zero = 0;
      Dma_Set(&zero, entry, 0x85000004, REG_DMA3);
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
        *entry = sprite;
        entry++;
      }
      sprite = ResourceObject_Create(id + 1);
      if (sprite != NULL)
        *entry = sprite;
      break;
    }
  }
  if (object != NULL) {
    Actor_SetPositionAndResetMotion(object, x, y, z);
    object->script = (s32 *)gActorScript_EndScript;
    object->speedLimit = 0x20000;
    object->step = 0;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    object->acceleration = 0x10000;
    object->motionFlags = 3;
    object->gravity = 0x10000;
    object->bounce = 0x4000;
    object->collisionFlags = 0;
    object->actionFlags = 1;
    object->angle = 0x4000;
    object->unk4c = 0;
    object->action = x / 65536;
    object->unk66 = z / 65536;
    *(s32 *)&object->unk70[12] = 0;
    *(s32 *)&object->unk70[0] = 0;
    *(s32 *)&object->unk70[4] = 0;
    *(s32 *)&object->unk70[8] = 0;
  }
  return object;
}

/// Points an object at its script and clears its script state.
void Actor_StartScript(struct ActorView *object, const s32 *script) {
  if (object != 0) {
    object->value04 = 0;
    object->value00 = (u32)script;
    object->value5b = 0;
    object->value5d = 0;
    object->value57 = 0;
  }
}

void Actor_SelectAnimation(struct ActorView *object, s32 argument) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
  s32 count;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(object->target.child, argument);
      break;
    case 2:
      items = object->target.children;
      count = 3;
      do {
        item = *items++;
        if (item != 0)
          Animation_ApplyChildArgument(item, argument);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationSpeed(struct ActorView *object, s32 value) {
  s32 count;
  struct ActorSpriteView *child;
  struct ActorSpriteView **children;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildValue(object->target.child, value);
      return;
    case 2:
      children = object->target.children;
      count = 3;
      do {
        child = *children++;
        if (child != 0)
          Animation_ApplyChildValue(child, value);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationAndSpeed(struct ActorView *object, s32 argument,
                                s32 value) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
  s32 count;

  if (object != 0) {
    switch (object->kind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(object->target.child, argument);
      Animation_ApplyChildValue(object->target.child, value);
      break;
    case 2:
      items = object->target.children;
      for (count = 3; count >= 0; count--) {
        item = *items++;
        if (item != 0) {
          Animation_ApplyChildArgument(item, argument);
          Animation_ApplyChildValue(item, value);
        }
      }
      break;
    }
  }
}

void Actor_SetSpriteRotation(struct ActorView *object, u32 value) {
  if (object != 0 && (object->kind & 0xf) == 1)
    object->target.child->value12 = value;
}

void Animation_SetIndexAndInitializeObjects(struct ActorView *object,
                                            s32 index) {
  struct ActorSpriteView *child;

  if (object != NULL && (object->kind & 0xf) == 1) {
    child = object->target.child;
    if (index >= 0) {
      child->entries[0]->animId = index;
      InitializeAnimationObjects(child);
    }
  }
}

void Actor_RegisterSpriteMetadata(struct ActorView *object, s32 value) {
  if (object != 0 && (object->kind & 0xf) == 1) {
    struct ActorSpriteView *child = object->target.child;
    if (value >= 0)
      ResourceMetadata_Register(child, value);
  }
}

#include "main_routine_work.h"

extern const s32 gData_0802f098[];
extern const s32 gData_0802f0b0[];
extern const s32 gData_0802f0c8[];
extern const s32 gData_0802f0e0[];
extern const s32 gData_0802f0f8[];
extern const s32 gData_0802f110[];
extern const s32 gData_0802f128[];
extern const s32 gData_0802f140[];
extern const s32 gData_0802f158[];
extern const s32 gData_0802f170[];
extern const s32 gData_0802f188[];
extern const s32 gData_0802f1a0[];
extern const s32 gData_0802f1b8[];
extern const s32 gData_0802f1e8[];

/// Starts an actor on the script that approaches its watched actor
/// (SC_APPROACH_WATCHED), watching watchedActorHandle.
void Actor_StartApproachWatched(struct ActorView *actor,
                                s32 watchedActorHandle) {
  Actor_StartScript(actor, gData_0802f1e8);
  actor->argument = watchedActorHandle;
}

/// Starts the dispatch script for gMainRoutineWork's render mode, waiting a
/// frame first in mode 9.
void Func_08023524(struct ActorView *object) {
  const s32 *script;
  struct MainRoutineWork *party = &gMainRoutineWork;

  switch (party->renderMode) {
  case 0:
    script = gData_0802f140;
    break;
  case 1:
    script = gData_0802f158;
    break;
  case 2:
    script = gData_0802f0b0;
    break;
  case 3:
    script = gData_0802f0c8;
    break;
  case 4:
    script = gData_0802f0e0;
    break;
  case 5:
    script = gData_0802f0f8;
    break;
  case 6:
    script = gData_0802f170;
    break;
  case 7:
    script = gData_0802f188;
    break;
  case 8:
    script = gData_0802f1a0;
    break;
  case 9:
    wait(1);
    script = gData_0802f1b8;
    break;
  case 10:
    script = gData_0802f110;
    break;
  case 11:
    script = gData_0802f128;
    break;
  case 12:
    script = gActorScript_EndScript;
    break;
  default:
    script = gData_0802f098;
    break;
  }
  Actor_StartScript(object, script);
}

void Actor_StartIdle(struct ActorView *object) {
  Actor_StartScript(object, (u32)gActorScript_EndScript);
}

void Actor_StartCamera(struct ActorView *object, s32 argument) {
  Actor_StartScript(object, (u32)gActor_CameraScript);
  if (argument != 0) {
    object->value34 = 0x8000;
    object->value30 = 0x40000;
    object->argument = argument;
    object->value64 = 0;
  }
}

void Actor_WaitForCommandEnd(struct ActorView *object) {
  s32 cnt = 0;

  if (object->value00 != 0 &&
      ((const s32 *)object->value00)[object->value04] != 17) {
    do {
      wait(1);
      cnt++;
      if (cnt > 599)
        break;
    } while (object->value00 != 0 &&
             ((const s32 *)object->value00)[object->value04] != 17);
  }
}

void Actor_SetChildByteValue(struct ActorView *object, u32 value) {
  if (object != NULL && (object->kind & 0xf) == 1)
    object->target.child->value1a = value;
}
