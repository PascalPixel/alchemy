/// @file
/// The scene code of overlay 0x69c that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_3/.

#include "jupiter_lighthouse_room_3/local.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(struct Actor *object) {
  if (object->x >> 20 == 14 && object->z >> 20 == 12)
    Actor_SetSpritePriorityDirect(object, 2);
}

void Func_02000070(void) {
  struct Actor *object = Actor_Get(10);
  struct PushedBlock block;
  s32 i;
  s32 x;

  object->update = (void (*)(void))Func_02000054;
  if (Func_02000dcc(&block))
    Func_02001050(block);
  if (object->x >> 20 == 16 && object->z >> 20 == 12 &&
      GameFlag_Test_(0x200) == 0) {
    Event_SetFlag(0x200);
    FieldActor_SetSpritePriority(10, 3);
    object->motionFlags = 3;
    System_Wait(5);
    Event_PlaySound(134);
    object->motionFlags = 2;
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      MapGrid_SetCellBehavior(0x1000000, x, 0, 0);
      if (i <= 1)
        Func_02002460(0x1000000, x, 0, 128);
    }
  }
  object->update = 0;
}

void Data_02000130(void) {}

/// Sets flag 0xa04 or 0xa05 when the object stands on cell (20, 36) or
/// (32, 36), then waits up to 30 frames for it to land and lifts it there.
void Func_02000134(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 placed = 0;
  s32 i;
  if (object->x >> 20 == 20 && object->z >> 20 == 36) {
    Event_SetFlag(0xa04);
    placed = 1;
  }
  if (object->x >> 20 == 32 && object->z >> 20 == 36) {
    Event_SetFlag(0xa05);
    placed = 1;
  }
  if (placed) {
    System_Wait(10);
    Event_PlaySound(134);
    for (i = 0; i < 30; i++) {
      if (object->y <= object->terrainHeight && object->velocityY == 0)
        break;
      System_Wait(1);
    }
    object->motionFlags = 0;
    Func_02002368(object->x, object->z, 0, 0);
  }
}

void Func_020001c0(void) { Import_EventSetExitRequest(12); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Func_02002440(12, 1);
  Func_02002440(13, 1);
  if (Event_GetFlag(0x109) == 0 && gMainRoutineWork.entrance == 12)
    Func_02002468(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(0x109) == 0 && gMainRoutineWork.entrance == 13)
    Func_02002468(gMainRoutineWork.currentOwner);
  return 0;
}

s32 Scene_PrepareMap(void) {
  struct Actor *object;
  s32 i;
  s32 x;

  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(0xa04)) {
    object = Actor_Get(14);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(14, PIXELS(328), PIXELS(584));
    Func_02002368(object->x, object->z, 0, 0);
  }
  if (Event_GetFlag(0xa05)) {
    object = Actor_Get(15);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(15, PIXELS(520), PIXELS(584));
    Func_02002368(object->x, object->z, 0, 0);
  }
  Actor_Get(8)->priorityFlags |= 32;
  Actor_Get(9)->priorityFlags |= 32;
  Actor_Get(10)->priorityFlags |= 32;
  Actor_Get(11)->priorityFlags |= 32;
  FieldActor_SetSpritePriority(8, 3);
  FieldActor_SetSpritePriority(10, 3);
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(11, 3);
  Actor_Get(10)->priorityFlags |= 2;
  Func_020023d0(8);
  Func_020023d0(9);
  Func_020023d0(10);
  Func_020023d0(11);
  Func_02000c78(8);
  Func_02000c78(9);
  Func_02000c78(10);
  Func_02000c78(11);
  if (Event_GetFlag(0x200)) {
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      MapGrid_SetCellBehavior(0x1000000, x, 0, 0);
      if (i <= 1)
        Func_02002460(0x1000000, x, 0, 128);
    }
  }
  return 0;
}

void Func_020003c4(void) { Func_02002400(); }

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "event_inline.h"

void Func_020003f8(s32 value) { gData_02002cc4 = value; }

void Func_02000404(void) {
  u16 colors[16];
  s32 phase = (gData_02002cc0 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002cc4 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02002cec;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02002cc0++;
}

void Func_020004c0(s32 mode) {
  gData_02002cc4 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02002cec, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002d0c, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_02002cc0 = zero;
    Task_Add(Func_02000404, 1152);
  } else {
    Dma_Set(gData_02002d10, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_02000544(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  object->priorityFlags |= 128;
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02002368(object->x, object->z, 0, (height >> 19) + 6);
  Func_02002460(object->x, object->z, 0, 128);
}

#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "event_work.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Event_Start(void);
void Event_End(void);

void Func_02000598(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = 0;
  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);
  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  facing = linked->facing;
  actor->x = (((linked->x >> 19) & ~1) + (facing >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((facing >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {
    i++;
    System_Wait(1);
  }
  Event_PlaySound(127);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    System_Wait(1);
  }
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Event_End();
}

s32 Func_02000784(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;
  s32 dxsq = dx * dx;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dzsq);
}
