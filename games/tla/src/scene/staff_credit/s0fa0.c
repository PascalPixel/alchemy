/// @file
/// Scene code of overlay 0x64c, in ROM order, from s0fa0's offset up to the next object the overlay links.

#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"
#include "frame.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"

/// The blend queue and the cell table reset the world map (overlay 64b) also
/// has, under this overlay's names.
#define Func_020038e4 Func_02000fa0
#define Func_02003960 Func_0200101c
#define Func_020036a8 Func_02000d64
#define gData_02006c08 gData_02003dc0
#include "../world_map/s3960.c"

s32 Func_02001044(s32 x, s32 z) { return x / 0x100000 + (z / 0x100000) * 128; }

s32 Func_02001064(struct Actor *object) {
  struct Actor *anchor = object->watchedObject;
  s32 frame;
  s32 scale;

  object->action++;
  frame = object->action;
  if (frame > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sin(frame << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = anchor->x;
  object->y += 0x10000;
  object->z = anchor->z;
  return 1;
}

/// The spark pair and the random particle the world map (overlay 64b) also
/// has, under this overlay's names.
#define Func_020039f4 Func_020010b0
#define Func_02003aac Func_02001168
#define Func_02004b30 Import_ActorCreate
#define Func_02004b20 Func_02001fc4
#define gData_02004f90 gData_02002e74
#define gData_02004f9c gData_02002e80
#include "../world_map/s39f4.c"

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020010b0(struct Actor *object);

void Func_020011f4(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_020010b0(object);
  }
}

extern s32 gData_02003db8;
extern const s32 gData_02002eac[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02001168(struct Actor *object);

void Func_0200122c(struct Actor *object) {
  if (gFrameCount & 1) {
    Animation_ApplyChildValues(object, gData_02002eac[gData_02003db8]);
    gData_02003db8 = (gData_02003db8 + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    Func_02001168(object);
  }
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020011f4(struct Actor *object);
void Func_0200122c(struct Actor *object);

void Func_02001274(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValues(object, 0);
    Event_PlaySound(390);
  } else if (mode == 1) {
    object->update = (void (*)(void))Func_020011f4;
  } else {
    object->update = (void (*)(void))Func_0200122c;
    Event_PlaySound(291);
  }
}

void Func_02001274(struct Actor *object, s32 mode);
s32 Func_02002004(s32 *position);
s32 Func_020020a4(s32 mode, s32 value);

void Func_020012ac(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(212);
  Func_02001274(object, 1);
  for (i = 0; i < 20; i++) {
    s32 hit = Func_02002004(&object->x);

    if (hit != 0 && Func_020020a4(2, hit) != 0)
      break;
    System_Wait(1);
  }
  Func_02001274(object, 2);
}

void Func_02001274(struct Actor *object, s32 mode);

void Func_020012f8(void) {
  Func_02001274(Actor_Get(FieldParty_GetLeaderActor()), 0);
}

extern u32 gFrameCount;
extern u16 gData_02003fc0;

/// Sets the layer blend, switching its alpha every two frames.
void Func_0200130c(void) {
  u32 alphaRegister = (u32)&REG_BLDALPHA;

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2) {
    QueueIoWriteDelay2(alphaRegister, gData_02003fc0 | 12);
  } else {
    QueueIoWriteDelay2(alphaRegister, gData_02003fc0 | 16);
  }
}

extern u32 gFrameCount;

u32 Random16Far(void);

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which sets the
   * arguments up in their own order: z is shifted before the -1 starts; as
   * direct Camera_MoveWithinMapBounds calls, 6 of 66 instructions differ in
   * tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
void Func_02001364(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16Far() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      Camera_MoveWithinMapBounds((x << 16) + 0x10000, -1, (z << 16) - 0x10000,
                                 1);
      break;
    }
    case 2:
      CameraMove((x << 16) + 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 3:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) - 0x10000, 1);
      break;
    }
  }
}

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];
extern u16 gData_02003fc0;

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002074(void);
void *Heap_AllocEwram(s32 size);
s32 Func_02001f54(const void *source, void *destination);
void Func_0200130c(void);
void Func_02002024(void);
void Func_02001f74(void);
void Heap_Release(void *pointer);

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02001400(s32 mode) {
  u8 *buffer;
  u32 level;

  /* FAKEMATCH(temporary): the blend level is read into level, a u32 local,
   * before each QueueIoWriteDelay2 call; as QueueIoWriteDelay2(0x04000052,
   * gData_02003fc0), 4 of 593 instructions differ in tla-en. */
  System_Wait(1);
  Event_ClearFlag(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(0x20000, 0x30000, 0x10000);
    Event_PlaySound(141);
  }
  Func_02002074();
  buffer = Heap_AllocEwram(0x4000);
  Func_02001f54(gWorldMapPackedTiles, buffer);
  Func_02001f54(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                      0x80000010);
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)buffer;
      q->count = count + 1;
      *entry++ = 0x06001000;
      *entry = 0x84000400;
    }
    IRQ_UNLOCK(saved);
  }
  Task_AddFrame(Func_0200130c, 0x480);
  Func_02002024();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(246);
  gData_02003fc0 = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(2);
  gData_02003fc0 = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_Wait(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(4);
  gData_02003fc0 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    MapGrid_SetShake(0x20000, 0x20000, 0x10000);
    Event_PlaySound(141);
    gData_02003fc0 = 0xd00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_0200130c);
    Func_02001f74();
    level = gData_02003fc0;
    QueueIoWriteDelay2(0x04000052, level);
    QueueIoWriteDelay2(0x04000050, 0);
  } else {
    gData_02003fc0 = 0xd00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xe00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0xf00;
    FieldEvent_Wait(4);
    gData_02003fc0 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_0200130c);
    Func_02001f74();
    level = gData_02003fc0;
    QueueIoWriteDelay2(0x04000052, level);
    QueueIoWriteDelay2(0x04000050, 0);
  }
  Heap_Release(buffer);
  Event_SetFlag(0x101);
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02001ad0(struct Actor *object);

void Func_0200199c(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_02001ad0(object);
  }
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02001ad0(struct Actor *object);

void Func_020019d4(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    Func_02001ad0(object);
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02001a0c(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
}

void Func_02001fd4(struct Actor *object);

void Func_02001a34(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->action;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02001fd4(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z + (rise - scale) * 5 + 0x80000;
}

void Func_02001a80(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->action;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02001fd4(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = -scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z - (rise - scale) * 5 + 0x100000;
}
