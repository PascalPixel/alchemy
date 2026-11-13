/// @file
/// The state of a slot of the chain work, with routines not yet understood.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/system/system.h"

/* VIEW(f0d9790): tag struct ChainNode as struct ChainNode__f0d9790; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5764: structure has no member named `visible'. */
struct ChainNode__f0d9790 {
  struct ChainNode__f0d9790 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

/* VIEW(f0d9790): tag struct ChainSlot as struct ChainSlot__f0d9790; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5725: structure has no member named `active'. */
struct ChainSlot__f0d9790 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  u8 unk0c[12];
  s32 unk18;
  struct ChainNode__f0d9790 *head;
};

/* VIEW(f0d9790): tag struct ChainWork as struct ChainWork__f0d9790; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5749: structure has no member named `from'. */
struct ChainWork__f0d9790 {
  struct Actor *from;
  struct Actor *to;
  u8 unk08[4];
  struct ChainSlot__f0d9790 slots[8];
};

s32 Func_080d9790(s32 index, s32 x, s32 y, s32 z) {
  struct ChainWork__f0d9790 *work = Ram_HeapSlots->unk84[6];
  s32 zero = 0;
  struct ChainSlot__f0d9790 *slot = &work->slots[index];
  struct ChainNode__f0d9790 *node = slot->head;
  struct ChainNode__f0d9790 **cursor;
  s32 i;
  s32 angle;
  s32 radius;
  s32 lastZ;
  s32 segments;
  s32 frame;
  s32 lastX;
  s32 lastY;
  struct ChainNode__f0d9790 **nodes;

  if (slot->active == 0)
    return;
  slot->active = zero;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  slot->unk18 = zero;
  if (slot->flag != 0)
    GameFlag_ClearBit(slot->flag);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 225 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SetMode(work->from, 3);
  Actor_SetMode(work->to, 3);
  wait(4);
  for (frame = 0; frame <= segments; frame++) {
    angle = frame << 12;
    radius = 0x40000;
    for (i = 0; i <= segments; i++) {
      node = nodes[i];
      node->unk13 = 0;
      if (i <= frame) {
        u16 step;

        node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
        lastX = node->x;
        node->y = y;
        lastY = node->y;
        node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
        lastZ = node->z;
        node->visible = 2;
        step = ArcTan2(0x60000, radius);
        angle += step;
        radius += step * 4;
      } else {
        s32 dx = (lastX - node->x) >> 8;
        s32 dy = (lastY - node->y) >> 8;
        s32 dz = (lastZ - node->z) >> 8;

        radius = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                                 Iwram_MulQ16(dz, dz));

        if (radius <= 6)
          break;
        node->x = lastX + (node->x - lastX) * 6 / radius;
        lastX = node->x;
        node->y = lastY + (node->y - lastY) * 6 / radius;
        lastY = node->y;
        node->z = lastZ + (node->z - lastZ) * 6 / radius;
        node->visible = 2;
        lastZ = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}

void Func_080d9974(void) {}

extern s32 gData_080f3944;

void Func_080d9978(struct Actor *object) {
  u8 *parts = object->sprite;

  parts[22] = 253;
  parts[23] = 12;
  if (gInput.repeat & 0x200)
    gData_080f3944 = 1;
  if (gData_080f3944 != 0)
    Func_080d8fa8(0, 12, 13, 0);
}

struct Actor *Actor_GetById(u32 objectId);

s32 Func_080d99b8(s32 firstId, s32 secondId) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  s32 x = owner->x >> 20;
  s32 y = owner->y >> 20;
  s32 z = owner->z >> 20;
  s32 firstX = first->x >> 20;
  s32 firstY = first->y >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondY = second->y >> 20;
  s32 secondZ = second->z >> 20;

  if (y == firstY) {
    if (x == firstX) {
      if (z == firstZ - 1 || z == firstZ + 1 || z == firstZ)
        return 1;
    } else if (z == firstZ) {
      if (x == firstX - 1 || x == firstX + 1 || x == firstX)
        return 1;
    }
  }
  if (y == secondY) {
    if (x == secondX) {
      if (z == secondZ - 1 || z == secondZ + 1 || z == secondZ)
        return 1;
    } else if (z == secondZ) {
      if (x == secondX - 1 || x == secondX + 1 || x == secondX)
        return 1;
    }
  }
  return 0;
}

s32 Func_080d9a74(s32 id, s32 *value, s32 *index) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot = work->slots;
  s32 i;

  for (i = 0; i <= 7; i++) {
    if (slot->head != NULL && slot->id == id) {
      *value = slot->value;
      *index = i;
      return 0;
    }
    slot++;
  }
  return -1;
}

/// Returns the state of slot index of the heap slot 0x9c work, or -1 while the
/// work is not allocated.
s32 Func_080d9ab0(s32 index) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  if (work == NULL)
    return -1;
  slot = &work->slots[index];
  return slot->state;
}

void Func_080d9ad0(s32 index, s32 flag, s32 a2, s32 a3) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  slot = &work->slots[index];
  slot->flag = flag;
  if (!GameFlag_Test(flag))
    Func_080d9104(index, a2, a3);
  else
    Func_080d8d68(index, a2, a3);
}

// XXX: what the work block in heap slot 40 holds, and what this family does;
// its callers and the block's readers would settle it.

void Func_080d9b08(void) {
  struct WorkA0 *work = HeapSlot_AllocEwram(160, sizeof(struct WorkA0));
  void *buffer = Heap_Alloc(0x900);

  Dma_Fill32(0, work, sizeof(struct WorkA0));
  work->vramEntry = Resource_FindFreeEntry();
  Resource_DecodeType01(gData_080f0c04, buffer);
  VramCache_Load(work->vramEntry, 64, buffer);
  Task_Add(Func_080d9b80, TASK_PASS_VBLANK);
  Task_Add(Func_080d9d40, TASK_PASS_VBLANK + 15);
  Heap_Release(buffer);
}
