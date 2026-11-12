/// @file
/// Scene code of overlay 0x674, in ROM order, from s14e0's offset up to the next object the overlay links.

#include "text/msg_ids.h"
#include "islets.h"

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "version.h"
#include "event_inline.h"

// The value stored at the event work's 0x1ac: 521 in the Japanese release,
// 513 from patch 1.
#if TLA_PATCH_1
#define STORED_AT_1AC 513
#else
#define STORED_AT_1AC 521
#endif

void Func_02003fc4(void);
void Func_02003fbc(void);
void Func_02003dfc(struct Actor *object);
void Func_0200129c(struct Actor *object);
void *HeapSlot_AllocEwram(s32 slot, s32 size);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1,                                    \
   Actor_WalkToAndWait(actor, x, z),                  \
   FieldEvent_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = FieldActor_Create(163, 0x1280000, \
                               0, 0x1a80000);                                  \
    b = FieldActor_Create(14, 0x1280000, \
                               0, 0x1a80000);                                  \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSingleChildFlags(a, 0);                                         \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = 0x20000;                                                     \
      Actor_SetSingleChildFlags(b, 0);                                         \
      Func_02003de4(b, 1);                                                     \
      Func_02003dec(b, gData_020044f0);                                        \
    }                                                                          \
  }

void Func_020014e0(void) {
  struct Actor *leader, *a, *b, *other;
  const u8 *message;
  s32 answer;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  if (PartyInventory_FindOwner(455) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (GameFlag_Test(513) != 0) {
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      /* FAKEMATCH(temporary): the form kept; as leader->update = Func_02000df8,
       * 94 of 569 instructions differ in tla-en. */
      a = (struct Actor *)Func_02000df8;
      leader->update = (void (*)(void))a;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      message = MsgOhYouRe;
      Event_SetMessageId(message);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      answer =
          Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
      if (answer == 0) {
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
        leader->update = (void (*)(void))a;
        STEP(gMainRoutineWork.currentOwner, 302, 424);
        SPAWN(a, b, answer)
        STEP(gMainRoutineWork.currentOwner, 318, 424);
        leader->update = 0;
        Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x6000, 0);
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Event_SetMessageId(message - 1);
        goto refused;
      }
    } else {
      Event_SetMessageId(MsgIMSoVery);
      Event_ShowMessage(9, 0);
      Func_080d47b4(gMainRoutineWork.currentOwner, 262, 20);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      leader->update = Func_02000df8;
      // Default: a is set for real when object 163 is made.
      a = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      leader->update = Func_02000df8;
      STEP(gMainRoutineWork.currentOwner, 302, 424);
      SPAWN(a, b, 0)
      STEP(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Func_080d47b4(9, 256, 20);
      Actor_FaceDirectionAndWait(9, 0xb000);
      other = Actor_GetById(9);
      other->update = Func_02000df8;
      FieldActor_Jump(9, 4, 20);
      other->update = 0;
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Func_080d47b4(9, 265, 0);
        Event_ShowMessage(9, 0);
        Actor_FaceDirection(9, 0x5000, 0);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = Func_02000df8;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 302, 424);
        leader->update = 0;
        if (a != 0)
          Func_02003dfc(a);
        Event_SetFlag(513);
      }
    }
  } else if (GameFlag_Test(0xaa5)) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId(MsgWantToGo);
    Event_ShowMessage(9, 0);
    Func_02003fc4();
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Func_02003fbc();
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Event_ShowMessage(9, 0);
      ((u8 *)HeapSlot_AllocEwram(88, 664))[0x295] = 99;
      FieldActor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
      Actor_FaceDirectionAndWait(9, 0x5000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 296, 452);
      leader->update = Func_02001278;
      Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
      Actor_SetAnimation(9, 3);
      FieldEvent_Wait(20);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      Actor_SetSpeed(9, 0xcccc, 0x6666);
      Actor_SetSpeed(10, 0xcccc, 0x6666);
      Actor_GetById(9)->update = Func_02000df8;
      Actor_SetDestination(9, 288, 538);
      FieldEvent_Wait(40);
      Func_02003fc4();
      Func_020034ec();
      Actor_GetById(10)->update = Func_02000df8;
      Actor_WalkTo(10, 288, 538);
      FieldEvent_Wait(20);
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = STORED_AT_1AC;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Func_02003fbc();
      FieldEvent_SetExitRequest(3);
    } else {
      Func_02003fbc();
    refused:
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0x5000);
    }
  } else {
    Event_SetMessageId(MsgIMSoVeryVery);
    Event_ShowMessage(9, 0);
  }
  Func_02003e6c();
}

#include "islets.h"
#include "scene_tables.h"

extern const u8 gData_020045a4[];
extern const u8 gData_020045f0[];

void Func_02001a68(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);
  void (*update)(void);
  s32 result;
  u8 *done;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgYouAllSet);
  Event_ShowMessage(9, 0);
  Func_02003fc4();
  result = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result == 0) {
    Func_02003fbc();
    Event_ShowMessage(9, 0);
    ((struct {
      u8 unk000[0x295];
      u8 unk295;
    } *)HeapSlot_AllocEwram(88, 0x298))
        ->unk295 = 99;
    FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 768);
    leader->update = (void (*)(void))Func_02001278;
    Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
    Actor_SetAnimation(9, 3);
    FieldEvent_Wait(20);
    Func_02003fc4();
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_SetSpeed(10, 0x19999, 0xcccc);
    update = (void (*)(void))Func_02000df8;
    done = (u8 *)object + 99;
    *done = 0;
    object->update = update;
    Actor_SetScript(9, gData_020045a4);
    FieldEvent_Wait(30);
    Actor_Get(10)->update = update;
    Actor_SetScript(10, gData_020045f0);
    do {
      System_Wait(1);
    } while (*done == 0);
    Func_020034ec();
    System_Wait(1);
    ((struct IsletsEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x201;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003fbc();
    FieldEvent_SetExitRequest(4);
  } else {
    Func_02003fbc();
    FieldEvent_SetMessageId((s32)MsgYouAllSet + 4);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  }
  Func_02003e6c();
}

void Func_02001bf4(void) {
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgMyPopS);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Func_02003e6c();
}

void Func_02001c38(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  work->field15a = 1;
}

void Func_02001c4c(void) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y < 0x280000) {
    Actor_Get(11)->collisionFlags |= 8;
    Actor_Get(12)->collisionFlags |= 8;
  } else {
    Actor_Get(11)->collisionFlags &= ~8;
    Actor_Get(12)->collisionFlags &= ~8;
  }
}

#include "dma.h"
#include "io_reg.h"

extern u8 gData_02004f04;
extern s16 gData_02004f10[2][160];

/// Applies BG1 horizontal line scrolling with HBlank DMA.
void Func_02001cac(void) {
  s16 *lines = gData_02004f10[gData_02004f04];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH(barrier): an asm reading the first line keeps its load before the
   * register address is built; as no asm, 6 of 34 instructions differ in
   * tla-en. */
  __asm__("" : : "r"(first));
  scroll = &REG_BG1HOFS;
  *scroll = first;
  dma = REG_DMA0;

  dma[5] &= 0xc5ff; // Clear start timing and repeat.
  dma[5] &= 0x7fff; // Stop.
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

void Func_02001cfc(void) {
  s16 *layer = (s16 *)&Ram_HeapSlots->mapWork->layers[2];
  s16 *lines = gData_02004f10[gData_02004f04 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 value;
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    /* FAKEMATCH(dead): the sine's call and store in a once-only block; as plain
     * statements, 2 of 65 instructions differ in tla-en. */
    do {
      sine = Math_Sin((*count + *(u8 *)&gFrameCount + layer[3]) << 9);
      v[0] = sine;
    } while (0);
    value = layer[1] + (sine >> 15);
    value += *count % 3;
    *lines++ = value - 1;
  }
  gData_02004f04 ^= 1;
}

void Func_02001d90(void) {
  Task_AddFrame(Func_02001cfc, 0x480);
  Task_AddFrame(Func_02001cac, 0xc80);
}

/// When object 19 stands in its target area, actor 9 and the leader react and
/// the party is given item 0x1c4.
void Func_02001db0(void) {
  struct Actor *object = Actor_GetById(19);
  struct Actor *leader;
  struct Actor *item;
  s32 z;

  if (object->x < 0xe40000 || object->x > 0xec0000)
    return;
  z = object->z;
  if (z < 0xa40000)
    return;
  if (z > 0xac0000)
    return;
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(19, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 216, 184);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgThankYouSo);
  FieldActor_Jump(9, 2, 10);
  FieldActor_Jump(9, 4, 10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 10);
  Event_ShowMessage(9, 0);
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 222, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 232, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  item = Actor_GetById(20);
  item->motionFlags = 0;
  Actor_SetItemIcon(item, 0x1c4);
  FieldActor_SetPositionAndResetMotion(item, leader->x, leader->y + PIXELS(24),
                                  leader->z);
  FieldEvent_Wait(20);
  Func_02003fc4();
  Party_GiveItem(0x1c4, 0);
  Func_02003fbc();
  Actor_SetPosition(20, 0, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Event_SetFlag(0xaa2);
  Func_02003e6c();
}

void Func_02001f58(void) {
  if ((gFrameCount & 15) == 0)
    Event_PlaySound(118);
}

void Func_02001f74(void) {
  struct Actor *object = Actor_Get(10);

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(10);
  System_Wait(1);
  object->motionFlags = 0;
  Actor_SetSpeed(10, 0x19999, 0xcccc);
  FieldActor_SetPositionAndResetMotion(object, PIXELS(268), PIXELS(64), PIXELS(390));
  System_Wait(1);
  object->terrainHeight = PIXELS(128);
  System_Wait(1);
  object->y = PIXELS(160);
  System_Wait(1);
  FieldTask_AddFrame(Func_02001f58, 0x480);
  FieldActor_SetMoveTarget(object, PIXELS(132), PIXELS(128), PIXELS(320));
  Actor_WaitForMove(object);
  FieldTask_Remove(Func_02001f58);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 3;
  FieldActor_SetSpritePriority(10, 2);
  Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(10, 4, 0);
  Actor_MoveToAndWait(10, 120, 312);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 0;
  Actor_FaceDirectionAndWait(10, FACING_EAST);
  Event_SetFlag(0x215);
  System_Wait(1);
  Func_02003e6c();
}

#include "event_work.h"
#include "text/msg_ids.h"

void Func_02003f0c();

void Func_0200208c(void) {
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Func_02003f0c(gMainRoutineWork.currentOwner, 6, 1);
  Func_02003e6c();
  return;
}

void Func_020020b4(void) {
  struct Actor *object0;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  object0->terrainId = 2;
  return;
}

void Func_020020d0(void) {
  struct Actor *object0;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  object0->terrainId = 0;
  return;
}

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells0 = map->layers[0].cells;
  struct MapCell *cells2 = map->layers[2].cells;
  s32 index = x / 0x100000 + (z / 0x100000) * 128;

  if (layers & 1) {
    cells0 += index;
    cells0->collisionCode = value;
  }
  if (layers & 2) {
    cells2 += index;
    cells2->collisionCode = value;
  }
}

/// Records objects 12 and 13's positions; with them on cells (7, 29) and
/// (7, 28), clears layer 2's collision under object 12, and with them on
/// (12, 25) and (12, 26), under object 13.
void Func_02002144(void) {
  struct Actor *object;
  s32 *x0;
  s32 *z0;
  s32 x1;
  s32 z1;

  object = Actor_Get(12);
  x0 = &gData_0200519c;
  z0 = &gData_02005194;
  *x0 = object->x;
  *z0 = object->z;
  object = Actor_Get(13);
  gData_02005198 = x1 = object->x;
  {
    /* FAKEMATCH(pin): object 13's z in r2, as the native keeps it for its
     * test; as a plain local, 31 of 58 instructions differ in tla-en. */
    register s32 z asm("r2");

    z = object->z;
    gData_02005190 = z;
    z1 = z;
  }
  if (*x0 >> 20 == 7 && *z0 >> 20 == 29 && x1 >> 20 == 7 && z1 >> 20 == 28)
    Func_020020ec(*x0, *z0, 2, 0);
  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 25 &&
      gData_02005198 >> 20 == 12 && gData_02005190 >> 20 == 26)
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
}

extern s32 gData_0200519c, gData_02005194, gData_02005198, gData_02005190;

void Func_020021d0(void) {
  struct Actor *first = Actor_Get(12);
  struct Actor *second = Actor_Get(13);
  s32 firstX = first->x >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondZ = second->z >> 20;
  s32 both;

  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 24) {
    Func_020020ec(gData_0200519c, gData_02005194, 1, 21);
    Func_020020ec(gData_0200519c, gData_02005194, 2, 0);
  } else {
    s32 *z = &gData_02005194;

    Func_020020ec(gData_0200519c, *z, 3, 0);
  }
  if (gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 27) {
    Func_020020ec(gData_02005198, gData_02005190, 1, 21);
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
  } else {
    s32 *z = &gData_02005190;

    Func_020020ec(gData_02005198, *z, 3, 0);
  }
  Func_020020ec(first->x, first->z, 3, 255);
  Func_020020ec(second->x, second->z, 3, 255);
  if (!Event_GetFlag(0x210) && firstX == 12 && firstZ == 26) {
    both = 0;
    Func_02003e64();
    FieldEvent_ResolvePendingActions(0);
    if (secondX == 12 && secondZ == 26) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    first->motionFlags = 3;
    System_Wait(10);
    Event_PlaySound(161);
    System_Wait(10);
    first->motionFlags = 0;
    Event_SetFlag(0x210);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 12, 26);
      Event_SetFlag(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Func_02003e6c();
  }
  if (!Event_GetFlag(0x211) && secondX == 7 && secondZ == 29) {
    both = 0;
    Func_02003e64();
    FieldEvent_ResolvePendingActions(0);
    if (firstX == 7 && firstZ == 29) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    second->motionFlags = 3;
    System_Wait(10);
    Event_PlaySound(161);
    System_Wait(10);
    second->motionFlags = 0;
    Event_SetFlag(0x211);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, 29);
      Event_SetFlag(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Func_02003e6c();
  }
}

#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02002430(void) {
  struct Actor *first = Actor_Get(12);
  struct Actor *second = Actor_Get(13);

  gData_0200519c = first->x;
  gData_02005194 = first->z;
  gData_02005198 = second->x;
  gData_02005190 = second->z;
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  if (gData_0200519c >> 20 == 7 && gData_02005194 >> 20 == 29 &&
      gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 28) {
    ((u8 *)first)[0x62] = 0;
    ((void (*)())Func_020020ec)(gData_0200519c, gData_02005194, 2);
  }
  Func_02003fe4();
  Func_020021d0();
  Func_02003e6c();
}

void Func_020024c8(void) {
  MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
  Event_SetFlag(0x213);
}

void Func_020024f0(void) {
  struct Actor *object;

  if (gInput.held & 0x40) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    Func_02003e64();
    FieldEvent_ResolvePendingActions(0);
    object->motionFlags = 0;
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 280);
    object->motionFlags = 3;
    Func_02003e6c();
  }
}

void Func_0200253c(void) {
  struct Actor *object;
  u16 *state;

  if (gInput.held & 0x80) {
    state = gSceneState;
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if ((u16)(*state + 0xfff6) > 200) {
      Func_02003e64();
      FieldEvent_ResolvePendingActions(0);
      object->motionFlags = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 296);
      object->motionFlags = 3;
      Func_02003e6c();
    }
  }
}

#include "call.h"

void Func_02003fec();

void Func_020025a4(void) {
  Event_SetFlag(0x218);
  return;
}

void Func_020025b0(void) {
  Event_ClearFlag(0x218);
  return;
}

// The following six are from patch 1.
#if TLA_PATCH_1
void Func_020025bc(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 4, 4, 65, 10, 1);
  return;
}

void Func_020025dc(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 2, 2, 71, 10, 1);
  return;
}

void Func_020025fc(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 1, 1, 68, 15, 1);
  return;
}

void Func_0200261c(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 2, 3, 75, 20, 1);
  return;
}

void Func_0200263c(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 4, 1, 78, 22, 1);
  return;
}

void Func_0200265c(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 1, 1, 79, 23, 1);
  return;
}
#endif

void Func_0200267c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s16 *exit;
  s32 index;

  object = Actor_Get(party->currentOwner);
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
  FieldActor_SetMoveTarget(object, ((8 - (object->x >> 16) % 16) << 16) + object->x,
                      object->y, object->z);
  Actor_WaitForMove(object);
  object->angle = FACING_NORTH;
  exit = &work->exit;
  index = *exit;
  Event_PlaySound(158);
  index--;
  MapGrid_AnimateCells(gData_020046bc[index].value, gData_020046bc[index].pair.first, gData_020046bc[index].pair.second);
  Actor_SetScript(party->currentOwner, gData_02004028);
  System_Wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(*exit);
}

void Func_0200274c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Func_020034ec();
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02002780(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;

  object = Actor_Get(party->currentOwner);
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
  FieldActor_SetMoveTarget(object, ((8 - (object->x >> 16) % 16) << 16) + object->x,
                      object->y, object->z);
  Actor_WaitForMove(object);
  object->angle = FACING_NORTH;
  Actor_SetScript(party->currentOwner, gData_02004028);
  System_Wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02002830(void) {
  struct IsletsEventWork *work;
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);

  Func_02003eec(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(273), PIXELS(520));
  System_Wait(1);
  object->angle = FACING_NORTH;
  owner->update = (void (*)(void))Func_02001278;
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0x9999, 0x4ccc);
  Actor_SetSpeed(10, 0x9999, 0x4ccc);
  Actor_Get(10)->update = (void (*)(void))Func_02000df8;
  object->update = (void (*)(void))Func_02000df8;
  Actor_WalkTo(10, 280, 468);
  Actor_MoveToAndWait(9, 296, 452);
  Actor_SetAnimation(10, 1);
  owner->update = 0;
  object->update = 0;
  Actor_Get(10)->update = 0;
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgHereWeAre);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 312, 440);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  Event_ShowMessage(9, 0);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x100;
  Func_020034d8();
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) ((struct Actor *)((s32)Actor_Get((id))))

extern const u8 gData_02004508[];
extern const u8 gData_02004554[];
void Func_02003fb4(void);

/// The Islets' arrival by entrance 4: actor 9 joins the leader and actor 10
/// is placed, the screen fades in, both run their action scripts
/// (Func_02000df8 updates) until actor 9's byte 0x63 is set, then actor 9
/// speaks (MsgWellHereWe) and the leader is launched up to (152, 742) and
/// turned; ends with Func_020034d8. The Japanese edition does not set the
/// event work's transition word 0x1ac.
void Func_020029c4(void) {
  struct Actor *leader = OBJECT(gMainRoutineWork.currentOwner);
  struct Actor *guide = OBJECT(9);
  u8 *done;

  Func_02003eec(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(240), PIXELS(540));
  System_Wait(1);
  guide->angle = FACING_SOUTH;
  leader->update = Func_02001278;
  Actor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)),
                         15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
#if TLA_PATCH_1 /* The Japanese release leaves the transition word. */
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  OBJECT(10)->update = Func_02000df8;
  guide->update = Func_02000df8;
  done = (u8 *)guide + 0x63;
  *done = 0;
  Func_02003dec((s32)guide, gData_02004508);
  Actor_SetScript(10, gData_02004554);
  do
    System_Wait(1);
  while (*done == 0);
  leader->update = 0;
  guide->update = 0;
  OBJECT(10)->update = 0;
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId(MsgWellHereWe);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)),
                         0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 742);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_ShowMessage(9, 0);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  Func_020034d8();
#if TLA_PATCH_1
  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
#endif
}

s32 Func_02002b64(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  s16 scene;

  work->field1ac = 0x100;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov674Room1)
    Func_02002c38();
  else if (scene == (s32)&SceneId_Ov674Room2)
    Func_02002f1c();
  else if (scene == (s32)&SceneId_Ov674Room3)
    Func_020030b4();
  else if (scene == (s32)&SceneId_Ov674Room4)
    Func_02003164();
  else if (scene == (s32)&SceneId_Ov674Room5)
    Func_0200329c();
  return 0;
}

void Func_02002bd8(void) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y > 0x80000) {
    Actor_Get(9)->collisionFlags &= ~8;
    Actor_Get(10)->collisionFlags &= ~8;
  } else {
    Actor_Get(9)->collisionFlags |= 8;
    Actor_Get(10)->collisionFlags |= 8;
  }
}

void Func_02002c38(void) {
  struct IsletsMapFlags *map = (struct IsletsMapFlags *)Ram_HeapSlots->mapWork;
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  map->mode = 1;
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_Get(10)->collisionFlags |= 0x80;
  if (Event_GetFlag(0xaa1)) {
    spawned = FieldActor_Create(489, 0x1180000, 0, 0x3480000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Actor_SetItemIcon(spawned, 454);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->collisionFlags = 9;
      spawned->unk5c[0] = 1;
    }
    Actor_Get(11)->angle = FACING_NORTH + FACING_STEP;
  } else {
    Actor_SetAnimation(11, 5);
  }
  Task_AddFrame(Func_02002bd8, 0x480);
}

void Func_02002d0c(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSingleChildFlags(object, 0);
  object->unk5c[0] = 3;
  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = 0x2666;
  object->speedLimit = 0x4ccc;
}

void Func_02002d48(s32 id, s32 column, s32 row) {
  struct Actor *object = Actor_Get(id);
  s32 x = (column * 32 + object->action) << 16;
  s32 z = (row * 32 + (s16)object->unk66) << 16;

  FieldActor_SetMoveTarget(object, x, object->y, z);
}

// The Islets' marching guards (objects 11 to 18): while the event work is busy
// or flag 0x218 is set they stand still; once the controlled character drops
// below height -12 they stand and the event work's counter 0x16a becomes 200;
// otherwise, for each frame owed (gSceneState's halfword 2) while no event
// runs, a 260-frame routine sets them walking with their facing steps
// (Func_02002d48) at 0, still at 100 and 220, and walking in place at 130.
void Func_02002d7c(void) {
  u16 *step = (u16 *)((u8 *)gSceneState);
  u8 *work = Ram_HeapSlots->eventWork;
  u16 *owed = (u16 *)(((u8 *)gSceneState) + 2);
  s32 i;
  s32 count;

  if (*(s16 *)(work + 0x166) != 0 || GameFlag_Test(0x218)) {
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    return;
  }
  for (i = 11; i <= 18; i++)
    Actor_GetById(i)->movementState = 0;
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < -0xc0000) {
    work = Ram_HeapSlots->eventWork;
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    {
      s16 *countdown = (s16 *)(work + 0x16a);
      s32 ticks = 200;

      *countdown = ticks;
    }
    return;
  }
  count = *owed + 1;
  *owed = count;
  if (*(s16 *)(work + 0x156) != 0 || (u16)count == 0)
    return;
  do {
    switch (*step) {
    case 0:
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 2);
      Func_02002d48(11, 0, 1);
      Func_02002d48(12, 0, -1);
      Func_02002d48(13, 0, 1);
      Func_02002d48(14, 0, 1);
      Func_02002d48(15, -1, 0);
      Func_02002d48(16, 0, 1);
      Func_02002d48(17, 1, 0);
      Func_02002d48(18, 0, -1);
      break;
    case 100:
    case 220:
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 1);
      break;
    case 130:
      for (i = 11; i <= 18; i++) {
        Actor_SetAnimation(i, 2);
        Func_02002d48(i, 0, 0);
      }
      break;
    }
    if (++*step == 260)
      *step = 0;
  } while (--*owed != 0);
}

void Func_02003684(s32 actor);
extern const u8 gData_02004274[];

/// The room's setup: flags, the objects' places and the scene state.
void Func_02002f1c(void) {
  struct Actor *object;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 angle;
  s32 i;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  i = 11;
  do {
    Func_02002d0c(i++);
  } while (i <= 18);
  state = gSceneState;
  next = &gSceneState[1];
  if (!Event_GetFlag(0x109)) {
    *state = 0;
    *next = 0;
  }
  FieldTask_AddFrame(Func_02002d7c, 0x480);
  if (Event_GetFlag(0xa2c)) {
    object = Actor_Get(10);
    Actor_SetPosition(10, PIXELS(176), PIXELS(168));
    Actor_SetAnimation(10, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->y = PIXELS(-8);
    object->terrainHeight = PIXELS(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
  } else {
    Func_02003684(10);
  }
  if (!Event_GetFlag(0xaa2)) {
    FieldActor_WatchAndSetScript(9, 19);
    Actor_SetAnimation(19, 0);
    Actor_Get(19)->actionFlags &= ~1;
  } else if (!Event_GetFlag(0x109)) {
    other = Actor_Get(9);
    other->angle = FACING_SOUTH + FACING_STEP;
    other->collisionFlags |= 0x80;
    /* FAKEMATCH(temporary): the angle held in a local is built before the
     * position; as the constant argument 0x3000, 4 of 166 instructions differ
     * in tla-en. */
    angle = 0x3000;
    Actor_SetPositionAndFacing(19, PIXELS(234), PIXELS(204),
                                                   angle);
    Actor_Get(19)->collisionFlags |= 0x80;
    Actor_SetSpeed(9, 0x20000, 0x10000);
    Actor_SetSpeed(19, 0x19999, 0xcccc);
    Actor_SetScript(9, gData_02004274);
    Actor_SetScript(19, gData_02004274);
    Event_SetFlag(0x200);
  }
}

void Func_020030b4(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 3)
    Event_ClearFlag(0x12f);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  Actor_SetAnimation(10, 4);
  if (Event_GetFlag(0xaa3)) {
    spawned = FieldActor_Create(489, 0x680000, 0x180000, 0xa00000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Actor_SetItemIcon(spawned, 453);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->unk5c[0] = 1;
    }
    Actor_SetPosition(10, PIXELS(104), PIXELS(204));
  }
  FieldActor_SetSpritePriority(11, 1);
  Actor_SetSpriteColor(Actor_Get(11), 15);
  Actor_Get(11)->priorityFlags |= 2;
}

// A room's setup.

void Func_02003f4c(s32 actor);

struct IsletsMapWork {
  u8 unk000[0x190];
  s32 value190;
  s32 value194;
};

/// Sets up the room's actors and map work.
void Func_02003164(void) {
  struct IsletsMapWork *map = (struct IsletsMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(10);
  struct Actor *last;
  s32 set;
  s32 zero = 0;

  map->value190 = zero;
  map->value194 = zero;
  if (gMainRoutineWork.entrance == 11)
    Event_ClearFlag(0x12f);
  if (!Event_GetFlag(0x109)) {
    object->motionFlags = zero;
    FieldActor_SetSpritePriority(10, 1);
  }
  Actor_SetPosition(11, 0, 0);
  Func_02003f4c(12);
  Func_02003f4c(13);
  set = Event_GetFlag(0x212);
  if (set) {
    struct Actor *first = Actor_Get(12);
    struct Actor *second = Actor_Get(13);

    first->motionFlags = zero;
    second->motionFlags = zero;
    ((u8 *)first)[0x62] = 1;
    ((u8 *)second)[0x62] = 1;
    {
      s32 held = 29;

      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, held);
    }
  } else {
    struct Actor *first = Actor_Get(12);
    struct Actor *second = Actor_Get(13);

    first->motionFlags = 0;
    second->motionFlags = 0;
    Func_020020ec(first->x, first->z, 3, 255);
    Func_020020ec(second->x, second->z, 3, 255);
  }
  Func_02003f4c(14);
  last = Actor_Get(14);
  {
    // A local that holds 0, as the native keeps its own register for it.
    s32 still = 0;

    ((u8 *)last)[0x62] = 1;
    last->motionFlags = still;
  }
  Func_020020ec(last->x, last->z, 1, 255);
  if (Event_GetFlag(0x213))
    MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
}

#if TLA_PATCH_1

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003410(void);
void PaletteQueue_Add(s32 a, s32 b, s32 c, s32 d);
void Func_02001c4c(void);
void Func_02002830(void);
void Func_020029c4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020034d8(void);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
  Func_02003410();
  {
    s32 a1 = 12;
    s32 a2 = 5;
    s32 a3 = 4;
    s32 a0 = 3;

    /* FAKEMATCH(barrier): the native loads the first argument last; as the
     * plain call, 2 of 145 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2), "+l"(a3));
    PaletteQueue_Add(a0, a1, a2, a3);
  }
  PaletteQueue_Add(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) && (done = Event_GetFlag(265)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(265)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(265)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else if (entrance != 5) {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Func_02003e6c();
}

#else

// The Japanese release's older form.

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003410(void);
s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count);
void Func_02001c4c(void);
void Func_02002830(void);
void Func_020029c4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020034d8(void);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
  Func_02003410();
  PaletteQueue_Add(3, 12, 5, 4);
  PaletteQueue_Add(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) && (done = Event_GetFlag(265)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(265)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(265)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Func_02003e6c();
}

#endif

s32 Scene_PrepareMap(void) { return 0; }
