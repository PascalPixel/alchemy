/// @file
/// Scene code of overlay 0x670, in ROM order, from s2ad8's offset up to the next object the overlay links.

#include "kibombo.h"

struct KibomboWork90 {
  u8 unk00[0x18];
  struct KibomboObject *leader;
};

void Func_02002ad8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  work->leader = Actor_GetById(gMainRoutineWork.currentOwner);
}

void Func_02002af8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  work->leader = 0;
}

s32 Func_02002b08(s32 value) {
  if (value > 31)
    value = 31;
  if (value < 0)
    value = 0;
  return value;
}

#include "io_reg.h"

extern u8 gData_0200635c[];
extern s16 gData_02007948[];
extern struct KibomboObject *gData_02007950;

void Func_02001318(void);
s32 Func_02002b08(s32 value);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);

void Func_02002b18(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta;
  g += delta >> 1;
  b += delta >> 2;

  r = Func_02002b08(r);
  g = Func_02002b08(g);
  b = Func_02002b08(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

void Func_02002ba4(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta + 4;
  delta -= 4;
  g += delta >> 1;
  b += delta >> 2;

  r = Func_02002b08(r);
  g = Func_02002b08(g);
  b = Func_02002b08(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

void Func_02002c34(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(9, 0, ((Random16Far() * 10) >> 16) - 5);
    Func_02002b18(87, 1, ((Random16Far() * 8) >> 16) - 4);
    Func_02002b18(103, 2, ((Random16Far() * 4) >> 16) - 2);
    Func_02002b18(119, 3, ((Random16Far() * 4) >> 16) - 2);
  }
}

void Func_02002ccc(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(9, 0, ((Random16Far() * 10) >> 16) - 5);
    Func_02002b18(7, 1, ((Random16Far() * 6) >> 16) - 3);
    Func_02002b18(6, 2, ((Random16Far() * 4) >> 16) - 2);
    Func_02002ba4(76, 3, ((Random16Far() * 8) >> 16) - 4);
  }
}

void Func_02002d64(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(169, 0, ((Random16Far() * 4) >> 16) - 2);
    Func_02002b18(167, 1, ((Random16Far() * 2) >> 16) - 1);
    Func_02002b18(166, 2, Random16Far() >> 16);
  }
}

#include "scene_tables.h"

void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Icon_DrawItem(s32 resource);
void VramCache_Load(s32 handle, s32 size, void *source);

void Func_02002de0(void) {
  struct KibomboObject **slot = &gData_02007950;
  /* FAKEMATCH(temporary): one local zero, kept in r7 for the three byte stores
   * below; as plain zeros, built in r2 where they are stored, 22 of 55
   * instructions differ in tla-en. */
  s32 zero = 0;
  struct KibomboObject *object =
      Import_ActorCreate(0x1e9, 0x1090000, 0x240000, 0x1580000);

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = zero;
    object->field5c = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(242);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02002e60(void) {
  struct KibomboObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->y > 0x100000) {
    Actor_SetHorizontalPositionWithTerrain(29, 0, 0);
    Actor_SetSpritePriority(30, 2);
  } else {
    Kibombo_ObjSetPos(Actor_GetById(29), PIXELS(120), 0, PIXELS(168));
    Actor_SetSpritePriority(30, 3);
  }
}

void Func_02002eb0(void) {
  if (gData_02007950 != 0) {
    struct KibomboObject *object = Actor_GetById(27);

    Actor_SetPositionAndResetMotion(gData_02007950, object->x,
                                    object->y + PIXELS(12), object->z);
  }
}

