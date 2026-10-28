#include "shrine_of_the_sea_god.h"
#include "macros.h"
#include "call.h"

extern const u8 gData_02003cbc[];

void Func_02003988(void);
void Func_020039b8(s32 a, s32 b, s32 c);
void Func_02003ad8(s32 a);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);

/// The leader is placed at y -50 and map layer 1 moved by -50; then both move
/// by 0x8000 in y a frame for 100 frames as a screen transition starts.
void Func_02000ce8(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer;
  struct ShrineOfTheSeaGodObject *leader;
  u32 i;
  s32 frames;

  /* FAKEMATCH(barrier): the layer's address waits for gSequenceParentWork's; without
   * it, 2 of 142 instructions differ in tla-en. */
  asm("" : "+l"(map) : "l"(&gSequenceParentWork));
  layer = &map->layers[1];
  leader = Actor_GetById(gSequenceParentWork.currentOwner);

  Func_020039d8();
  Func_02003b08(0);
  Func_02003b48(244);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  ShrineOfTheSeaGod_CamMove(172 << 17, -1, 248 << 16, 0);
  ShrineOfTheSeaGod_CopyRect(62, 77, 62, 18, 19, 6);
  wait(1);
  Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 2);
  leader->motionFlags = 0;
  leader->y = -50 << 16;
  leader->height = -50 << 16;
  layer->y += -50 << 16;
  Func_02003988();
  wait(1);
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 515;
  Event_StartScreenTransition();
  Call3((void (*)())Func_020039b8, 0x10000, 0x10000, 0x10000);
  // A goto loop: as a for, while or do loop, loop.c hoists the 0x8000 the
  // bytes rebuild on every pass (differ 64).
  i = 0;
loop:
  leader->y += 0x8000;
  frames = 2;
  leader->height += 0x8000;
  /* FAKEMATCH(barrier): wait()'s argument is set before the count; as
   * wait(2), 4 of 142 instructions differ in tla-en. */
  asm volatile("" : "+l"(frames));
  i++;
  /* FAKEMATCH(barrier): the layer is read after the count; without it, 4 of
   * 142 instructions differ in tla-en. */
  asm("" : "+l"(layer) : "l"(i));
  layer->y += 0x8000;
  wait(frames);
  if (i <= 99)
    goto loop;
  Func_02003b48(288);
  Func_02003b48(390);
  Call3((void (*)())Func_020039b8, 0, 0, 0);
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 521;
  Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags |= 1;
  ShrineOfTheSeaGod_CopyRect(62, 69, 62, 18, 19, 6);
  wait(1);
  Func_020039e0();
}

/// The leader turns, then moves with map layer 1 by -0x8000 in y a frame for
/// 100 frames; at frame 90 the screen transition finishes.
void Func_02000e38(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer;
  struct ShrineOfTheSeaGodObject *object;
  u32 i;

  /* FAKEMATCH(barrier): the map pointer is ready before the layer offset; with
   * only the next barrier, 4 of 95 instructions differ in tla-en. */
  asm("" : "+l"(map));
  /* FAKEMATCH(barrier): the layer's address waits for gSequenceParentWork's; with only
   * the barrier above, 4 of 95 differ, and with neither 2, in tla-en. */
  asm("" : "+l"(map) : "l"(&gSequenceParentWork));
  layer = &map->layers[1];
  object = Actor_GetById(gSequenceParentWork.currentOwner);

  Func_020039d8();
  Func_02003b08(0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  ShrineOfTheSeaGod_CamMove(344 << 16, -1, 248 << 16, 1);
  ShrineOfTheSeaGod_CopyRect(62, 77, 62, 18, 19, 6);
  ShrineOfTheSeaGod_ObjTurn(gSequenceParentWork.currentOwner, 0x4000, 20);
  Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 2);
  object->motionFlags = 0;
  Func_02003b48(245);
  Call3((void (*)())Func_020039b8, 0x10000, 0x10000, 0x10000);
  // A goto loop: the bytes reload the step on every pass, and a for loop
  // hoists it (differ 30).
  i = 0;
loop:
  object->y -= 0x8000;
  object->height -= 0x8000;
  layer->y -= 0x8000;
  wait(2);
  if (i == 90) {
    ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = 0x203;
    Event_FinishScreenTransition();
  }
  i++;
  if (i <= 99)
    goto loop;
  Func_02003ad8(1);
}

void Func_02000f18(s32 actor) {
  Func_02003aa0(actor);
  Actor_GetById(actor)->priorityFlags |= 0x20;
}

void Func_02000f34(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 8, 18);
  GameFlag_SetBit(0x200);
}

void Func_02000f58(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 18);
  GameFlag_SetBit(0x201);
}

void Func_02000f80(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 17);
  GameFlag_SetBit(0x202);
}

void Func_02000fa8(void) { Func_02003b38(gData_02003cbc); }

void Func_02000fb8(void) {
  struct ShrineOfTheSeaGodObject *object;

  Func_02003b40();
  object = Actor_GetById(8);
  if (object->x >> 20 == 21 && object->z >> 20 == 13)
    GameFlag_SetBit(0x207);
  else
    GameFlag_ClearBit(0x207);
}

void Func_02000fec(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(8);

  Func_02003b40();
  if (object->x >> 20 == 12)
    GameFlag_SetBit(0x84c);
}

void Func_02001010(struct ShrineOfTheSeaGodObject *object) {
  object->motionFlags = 3;
  do {
    wait(1);
  } while (object->speed != 0);
  Func_02003b48(188);
  wait(10);
  object->motionFlags = 0;
}

void Func_02001038(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(8);
  s32 z = object->z >> 20;

  if (z == 16) {
    Func_020039d8();
    Func_02003b08(0);
    Func_02001010(object);
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, z);
    Actor_SetSpritePriority(8, 3);
    Func_020039e0();
    GameFlag_SetBit(0x301);
  }
}

void Func_02001088(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gSequenceParentWork.currentOwner);
  u32 id;

  for (id = 9; id <= 13;) {
    u8 *flags = &Actor_GetById(id)->priorityFlags;
    u32 next = id + 1;

    if (leader->y > 0x100000) {
      Actor_SetSpritePriority(id, 2);
      /* FAKEMATCH: the native ties the flag or to the constant's register. */
      *flags = *flags + 2 - (*flags & 2);
    } else {
      Actor_SetSpritePriority(id, 1);
      *flags &= ~2;
    }
    id = next;
  }
}

void Func_020010e8(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gSequenceParentWork.currentOwner);
  s32 y;

  Func_02003b28();
  y = leader->y;
  /* FAKEMATCH: loads the leader height before it builds the threshold. */
  __asm__("");
  if (y < 0x100000)
    ((u8 *)&gSequenceParentWork)[0x2c3] = 0;
  else
    ((u8 *)&gSequenceParentWork)[0x2c3] = 1;
}

void Func_02001128(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gSequenceParentWork.currentOwner);
  s32 y;
  s32 z;

  y = leader->y;
  /* FAKEMATCH: loads the leader height before it builds the threshold. */
  __asm__("");
  if (y < 0x100000) {
    z = leader->z;
    /* FAKEMATCH: loads the leader depth before it builds the threshold. */
    __asm__("");
    if (z < 0x1280000) {
      Actor_SetSpritePriority(10, 1);
      Actor_SetSpritePriority(11, 1);
      Actor_SetSpritePriority(12, 2);
    } else {
      Actor_SetSpritePriority(10, 2);
      Actor_SetSpritePriority(11, 2);
      Actor_SetSpritePriority(12, 3);
    }
  } else {
    Actor_SetSpritePriority(10, 2);
    Actor_SetSpritePriority(11, 2);
    Actor_SetSpritePriority(12, 3);
  }
}

void Func_020011a0(void) {
  struct ShrineOfTheSeaGodObject *first = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *second = Actor_GetById(9);

  SET_POSITION(second, first->x, 0x140000, first->z);
}

void Func_020011c0(void) {
  Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags &= ~0x20;
}

void Func_020011e0(void) {
  Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags |= 0x20;
}
