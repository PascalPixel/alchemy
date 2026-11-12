/// @file
/// Scene code of overlay 0x668, in ROM order, from s0514's offset up to the next object the overlay links.

#include "field/field.h"

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct Unk_02004240 {
  s32 value[6];
};

void Func_02000fa4(void);
void Func_0200112c(void);
void Func_020017ec(void);
void Func_02001cb0(void);
void Func_02003f48(s32, s32, s32, s32);
void Func_02004004(void);
void Func_020040c0(void);
s32 Func_02004240(struct Unk_02004240 *);
void Func_020044c4(struct Unk_02004240);
void Func_02004868(void *, s32, s32);
void Func_02004a0c(struct SpawnObject *object, s32 mode);
s32 Func_02004a24();
s32 Func_02004a7c();
s32 Func_02004a84();
void Func_02004a94();
void Func_02004a9c();
s32 Func_02004bb4();
void Func_02004c24(s32 slotNo,s32 firstId,s32 secondId);
s32 Func_02004c44();
s32 Func_02004c4c();
s32 Func_02004c8c();

void Func_02000514(void) {
  struct Actor *actor15;
  struct Actor *actor16;
  struct Unk_02004240 unk;
  s32 facing;
  s32 i;

  actor15 = Actor_GetById(15);
  actor16 = Actor_GetById(16);
  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (actor16 != NULL && Event_GetFlag(0x8a4) == 0)
    Func_080dbdd4(actor16->x, actor16->z, 2, 255);
  if (Func_02004240(&unk) != 0) {
    if (Event_GetFlag(0x8a4) == 0 || facing == 0xc000) {
      Func_020044c4(unk);
    } else {
      Event_SetFlag(0x8a6);
      Func_02004a94();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(560), 1);
      Func_020044c4(unk);
      Func_080dbdd4(actor15->x - PIXELS(24), actor15->z, 0, 0);
      Func_080dbdd4(actor15->x + PIXELS(8), actor15->z, 0, 0);
      Func_080dbdd4(actor15->x - PIXELS(24), actor15->z, 2, 0);
      Func_080dbdd4(actor15->x - PIXELS(8), actor15->z, 2, 0);
      Func_080dbdd4(actor15->x + PIXELS(8), actor15->z, 2, 0);
      Func_080dbdd4(actor15->x + PIXELS(24), actor15->z, 2, 0);
      Actor_SetAnimation(15, 3);
      Actor_SetDestinationOffset(15, 0, 16);
      FieldEvent_Wait(20);
      actor15->gravity = 0xccc;
      actor15->motionFlags = 3;
      i = 0;
      if (actor15->y > 0) {
        do {
          System_Wait(1);
          i++;
        } while (i <= 59 && actor15->y > 0);
      }
      Func_02000fa4();
      Event_PlaySound(240);
      Actor_SetAnimation(15, 8);
      FieldEvent_Wait(60);
      FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Func_02004a9c();
    }
  }
  if (actor16 != NULL && Event_GetFlag(0x8a4) == 0)
    Func_080dbdd4(actor16->x, actor16->z, 2, 0);
}

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

extern struct DoorStep gData_02005290[];

void Func_02000394(s32, s32);
void Func_0200042c(s32, s32);

void Func_020006ec(s32 mode) {
  s32 tile;

  tile = Func_02004bb4(gMainRoutineWork.currentOwner);
  if (tile == 16) {
    Func_02000394(mode, 16);
    if (Func_02004c44() != 0)
      Func_02004c4c();
    Func_0200042c(mode, 16);
  } else if (tile == 15) {
    Func_02000514();
  }
}

void Func_02000730(void) {
  Event_SetFlag(0x8a8);
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgWhatSYour);
  Actor_ShowEmote(22, 0x101, 40);
  Actor_FaceActor(22, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(22, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(22, 0, 0);
  } else {
    FieldEvent_Wait(20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_OpenMessage(22, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
      FieldEvent_Wait(30);
      FieldEvent_ShowMessageThenWait(22, 0, 0);
    } else {
      FieldEvent_Wait(20);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
        FieldEvent_Wait(30);
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      } else {
        FieldEvent_Wait(20);
        ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      }
    }
  }
  Actor_FaceDirection(22, FACING_EAST, 0);
  Func_02004a9c();
}

void Func_02000838(void) {
  if (Event_GetFlag(0x8a7)) {
    if (Event_GetFlag(0x904) == 0) {
      Event_SetFlag(0x904);
      Func_02001cb0();
    }
  } else if (Event_GetFlag(0x8ab)) {
    if (Event_GetFlag(0x8a0) && Func_02004a84(151) == -1 &&
        Event_GetFlag(0x8ad) == 0) {
      Event_SetFlag(0x8ad);
      Func_020017ec();
    }
  } else if (Event_GetFlag(0x8a9) == 0) {
    Event_SetFlag(0x8a9);
    Func_0200112c();
  }
}

void Func_020008cc(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004a9c();
}

void Func_02000940(s32 countdown) {
  Func_020008cc(gData_02005290, countdown, 0);
}

void Func_02000954(void) {}

void Func_02000958(void) { GameFlag_SetBit(0x8be); }

void Func_02000968(void) {
  struct Actor *leader;

  goto check;
run:
  if (Func_02004a84(151) != -1)
    return;
  Event_SetFlag(0x8bf);
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(4, 488, 440);
  FieldActor_LaunchFromLeader(5, -16, 0, 0);
  Actor_FaceDirectionAndWait(4, FACING_WEST);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIDonTKnowWhatWe);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_SetAnimation(5, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_SetDestination(5, ((s16 *)leader)[5], ((s16 *)leader)[9]);
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Func_02004a9c();
  return;
check:
  if (Event_GetFlag(0x8ab) && Event_GetFlag(0x8a7) == 0 && Event_GetFlag(0x8be))
    goto run;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "io_reg.h"
#include "dma.h"
#include "map.h"
#include "frame.h"

extern u8 gData_020054f0[];
extern u16 gData_020056e0[2][160];

// BG1's wave: each frame the finished table of scanline offsets is handed to
// an HBlank DMA, while the other table is filled for the next frame.

/// Starts DMA 0 copying the finished table into BG1's horizontal offset at
/// each HBlank.
// With ☀️'s way of stopping DMA 0.
void Func_02000a44(void) {
  u16 *line = gData_020056e0[gData_020054f0[0] ^ 1];
  volatile u16 *channel;

  REG_BG1VOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  channel[5] &= ~DMA_ENABLE;
  line++;
  (void)channel[5];
  Dma_Set(line, (void *)&REG_BG1VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// Fills the other table: each of the 160 lines takes layer 2's scroll plus a
/// sine of the frame count less the line.
void Func_02000a98(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  u16 *line;
  s32 i = 0;

  line = gData_020056e0[gData_020054f0[0] ^ 1];
  for (i = 0; i != 160; i++) {
    *line++ =
        (layer->y >> 16) +
        (Math_Sin(((u8)gFrameCount - i - (layer->y >> 16) + 160) << 9) >> 15);
  }
  gData_020054f0[0] ^= 1;
}

void Func_02000af8(void) {
  Task_AddFrame(Func_02000a98, 0x480);
  Task_AddFrame(Func_02000a44, 0xc80);
}

#include "actor.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) (Actor_Get((id)))

extern const u8 gData_02004d0c[];
void Func_02004c5c();
void Func_02003274(void);
void Func_020040ec();
void Func_02004c14(void);
void Func_02004c2c();
void Func_02004c1c(void);
void Func_020047fc();
void Func_02004c94();

s32 Func_02000b18(void) {
  struct Actor *object;
  s32 flag;

  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Func_02000af8();
  OBJECT(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(0x8a7) != 0) {
    if (Event_GetFlag(0x905) != 0 &&
        Event_GetFlag(0x90f) == 0) {
      Actor_SetPosition(30, PIXELS(184), PIXELS(616));
      Actor_SetPosition(31, PIXELS(136), PIXELS(600));
      Actor_SetPosition(28, PIXELS(168), PIXELS(584));
      Actor_FaceDirection(28, FACING_SOUTH, 0);
      Func_02004c5c(30, 0, 13);
      Call3(Func_02004c5c, 31, 0x8000, 13);
      Func_02004c5c(28, 0, 13);
      Actor_SetPosition(25, PIXELS(112), PIXELS(432));
      Actor_SetPosition(27, PIXELS(176), PIXELS(408));
      Actor_SetPosition(29, PIXELS(152), PIXELS(376));
      Actor_FaceDirection(29, FACING_SOUTH, 0);
      Actor_SetSpriteRotationById(25, 0x4000);
      Actor_SetSingleChildFlags((s32)Actor_Get(25), 0);
      Actor_SetSpriteRotationById(27, 0xc000);
      Actor_SetSingleChildFlags((s32)Actor_Get(27), 0);
      Actor_SetSpriteRotationById(29, 0x4000);
      Actor_SetSingleChildFlags((s32)Actor_Get(29), 0);
      Actor_SetAnimation(25, 13);
      Actor_SetAnimation(27, 13);
      Actor_SetAnimation(29, 13);
    }
    if (Event_GetFlag(0x905) != 0) {
      MapGrid_CopyCellsLayered(40, 0, 24, 31, 11, 6);
      MapGrid_CopyCellsLayered(84, 15, 10, 10, 75, 15);
      MapGrid_CopyCellAttributes(10, 24, 1, 1, 11, 24);
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(8, 0, 0);
      Actor_SetPosition(10, 0, 0);
      Actor_SetPosition(11, 0, 0);
      Actor_SetPosition(15, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 99) {
    gMainRoutineWork.secondary = 1;
    gMainRoutineWork.entrance = 1;
    Actor_SetPosition(4, PIXELS(136), PIXELS(552));
    Actor_SetPosition(26, PIXELS(120), PIXELS(552));
    Actor_SetPosition(30, PIXELS(184), PIXELS(616));
    Actor_SetPosition(31, PIXELS(136), PIXELS(600));
    Actor_SetPosition(28, PIXELS(160), PIXELS(584));
    Actor_SetSpriteRotationById(30, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(30), 0);
    Actor_SetSpriteRotationById(31, 0xc000);
    Actor_SetSingleChildFlags(Actor_Get(31), 0);
    Actor_SetSpriteRotationById(28, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(28), 0);
    FieldActor_SetSpritePriority(27, 2);
    FieldActor_SetSpritePriority(25, 2);
    FieldActor_SetSpritePriority(29, 2);
    Actor_SetPosition(25, PIXELS(160), PIXELS(392));
    Actor_SetPosition(27, PIXELS(176), PIXELS(408));
    Actor_SetPosition(29, PIXELS(152), PIXELS(376));
    Actor_SetSpriteRotationById(27, 0xc000);
    Actor_SetSingleChildFlags(Actor_Get(27), 0);
    Actor_SetSpriteRotationById(29, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(29), 0);
    Actor_FaceDirection(25, 0xd000, 0);
    Actor_FaceDirection(29, FACING_SOUTH, 0);
    Actor_SetAnimation(27, 13);
    Actor_SetAnimation(29, 13);
    Actor_SetAnimation(30, 13);
    Actor_SetAnimation(31, 13);
    Actor_SetAnimation(28, 13);
    Func_02003274();
  }
  Func_02000fa4();
  if (Event_GetFlag(0x8a6) == 0)
    Func_020040ec(15);
  Func_02004c14();
  Func_02004c2c(0, 0x8a3, 8, 9);
  OBJECT(8)->priorityFlags |= 32;
  OBJECT(9)->priorityFlags |= 32;
  Func_02004c1c();
  Func_02004c24(0, 12, 13);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  Actor_SetSingleChildFlags(Actor_Get(13), 0);
  Func_020047fc(gData_02004d0c);
  Actor_SetSingleChildFlags(Actor_Get(14), 0);
  flag = Event_GetFlag(0x8a2);
  if (flag == 0) {
    object = OBJECT(18);
    /* FAKEMATCH(dead): the solid bit set in a once-only block, which lays out
     * the branch over it as the native does; as the plain statement, 18 of 467
     * instructions differ in tla-en. */
    do {
      object->collisionFlags |= 8;
    } while (0);
    object = OBJECT(19);
    object->motionFlags = flag;
    object->y = 0x100000;
    Func_02004c24(1, 18, 19);
    Func_02004c94(1);
    object = OBJECT(20);
    object->collisionFlags |= 8;
    object = OBJECT(21);
    object->motionFlags = flag;
    object->y = 0x100000;
    Func_02004c24(2, 20, 21);
  }
  Actor_SetSingleChildFlags(Actor_Get(16), 0);
  Actor_SetSingleChildFlags(Actor_Get(17), 0);
  FieldActor_SetSpritePriority(10, 2);
  ((u8 *)OBJECT(23))[0x62] = 1;
  if (Event_GetFlag(0x8ab) != 0 && Event_GetFlag(0x8a7) == 0) {
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(23, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_SetPosition(25, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_02004c84();

void Func_02000fa4(void) {
  s32 t1, t2, t8, t10, t12, t13, t18, t19, t24, v_r5;

  t1 = Event_GetFlag(0x8a6);
  if (t1 == 0)
    goto L_02009022;
  t2 = ((s32)Actor_Get(15));
  v_r5 = t2;
  Actor_SetPosition(15, PIXELS(336), PIXELS(536));
  Actor_SetAnimation(15, 4);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  FIELD_AT(v_r5, u8, 85) = 2;
  FIELD_AT(v_r5, s32, 40) = 0;
  FIELD_AT(v_r5, s32, 12) = -0x40000;
  FIELD_AT(v_r5, s32, 20) = -0x40000;
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 0, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x2100000, 0, 0);
L_02009022:
  t8 = Event_GetFlag(0x8a4);
  if (t8 == 0)
    goto L_0200903a;
  Actor_SetPosition(16, 0, 0);
L_0200903a:
  t10 = Event_GetFlag(0x8a5);
  if (t10 == 0)
    goto L_02009052;
  Actor_SetPosition(17, 0, 0);
L_02009052:
  t12 = Event_GetFlag(0x8a0);
  if (t12 == 0)
    goto L_020090a8;
  t13 = ((s32)Actor_Get(10));
  v_r5 = t13;
  Actor_SetPosition(10, PIXELS(504), PIXELS(520));
  Actor_SetAnimation(10, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  do {
  } while (0);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  MapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          0);
  Func_02004c84(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0, 128);
L_020090a8:
  t18 = Event_GetFlag(0x8a1);
  if (t18 == 0)
    goto L_02009100;
  t19 = ((s32)Actor_Get(11));
  v_r5 = t19;
  Actor_SetPosition(11, PIXELS(408), PIXELS(536));
  Actor_SetAnimation(11, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  do {
  } while (0);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  MapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          0);
  Func_02004c84(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0, 128);
L_02009100:
  t24 = Event_GetFlag(0x8a7);
  if (t24 == 0)
    goto L_02009122;
  MapGrid_CopyCells(32, 24, 2, 2, 30, 24);
L_02009122:
}

#include "field_sprite.h"

void Func_02004b94();

void Func_0200112c(void) {
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;

  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgItSIt);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_WalkToAndWait(4, 232, 392);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x2000);
  FieldActor_LaunchFromLeader(5, 0, 0, 0x2000);
  FieldActor_LaunchFromLeader(6, 0, 16, 0x2000);
  Actor_WalkByAndWait(4, 16, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveToActor(22, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldCamera_MoveToActor(23, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(22, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(11, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldCamera_MoveToActor(24, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(24, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(25, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(22, FACING_EAST, 0);
  Actor_FaceDirection(23, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(14, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveToActor(22, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_ShowEmote(22, 0x107, 40);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldCamera_MoveTo(PIXELS(488), 0, PIXELS(416), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_ShowEmote(23, 0x102, 50);
  Actor_FaceDirection(23, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveTo(PIXELS(392), PIXELS(16), PIXELS(416), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(22, 0x105, 0);
  Actor_ShowEmote(23, 0x105, 0);
  Actor_ShowEmote(25, 0x105, 50);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 20);
  Actor_SetAnimation(22, 3);
  Actor_SetAnimation(23, 3);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, FACING_EAST, 0);
  Actor_FaceDirection(23, 0x5000, 0);
  Actor_FaceDirection(24, FACING_NORTH, 0);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(gMainRoutineWork.currentOwner, 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object0 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object0 == 0)
    goto L_020094c6;
  Actor_SetDestination(5, *(s16 *)((u8 *)object0 + 10), *(s16 *)((u8 *)object0 + 18));
L_020094c6:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object1 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object1 == 0)
    goto L_02009504;
  Actor_SetDestination(6, *(s16 *)((u8 *)object1 + 10), *(s16 *)((u8 *)object1 + 18));
L_02009504:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object2 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object2 == 0)
    goto L_02009550;
  Actor_SetDestination(26, *(s16 *)((u8 *)object2 + 10), *(s16 *)((u8 *)object2 + 18));
L_02009550:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004b94();
  Func_02004a9c();
  return;
}

void Func_02001574(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  result0 = Event_GetFlag(0x8ab);
  if (result0 != 0)
    goto L_02009586;
  goto L_020097dc;
L_02009586:
  result1 = Event_GetFlag(0x8a7);
  if (result1 == 0)
    goto L_02009596;
  goto L_020097dc;
L_02009596:
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgYouDoPlan);
  Event_SetFlag(0x8ac);
  Actor_WalkToAndWait(4, 264, 448);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x8000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x6000);
  result2 = Event_GetFlag(7);
  if (result2 == 0)
    goto L_02009604;
  FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
L_02009604:
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x102, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(5, FACING_WEST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(26, 0x106, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  result3 = Event_GetFlag(7);
  if (result3 == 0)
    goto L_02009716;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object4 == 0)
    goto L_02009706;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                            *(s16 *)((u8 *)object4 + 18));
L_02009706:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_02009716:
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 == 0)
    goto L_0200974c;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                            *(s16 *)((u8 *)object5 + 18));
L_0200974c:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 == 0)
    goto L_0200978a;
  Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                            *(s16 *)((u8 *)object6 + 18));
L_0200978a:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 == 0)
    goto L_020097c8;
  Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                            *(s16 *)((u8 *)object7 + 18));
L_020097c8:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  Func_02004a9c();
L_020097dc:
  return;
}

void Func_020017ec(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAreYouFinished);
  Actor_WalkToAndWait(4, 232, 392);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x8000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x6000);
  result0 = Event_GetFlag(7);
  if (result0 == 0)
    goto L_02009850;
  FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
L_02009850:
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, 0xc000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(26, 0);
  result1 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result1 != 0)
    goto L_020098ea;
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x100, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  goto L_02009938;
L_020098ea:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(6, 3);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
L_02009938:
  result2 = ((s32 (*)())GameFlag_Test)(7);
  if (result2 == 0)
    goto L_0200996c;
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  FieldEvent_Wait(10);
  goto L_02009a2e;
L_0200996c:
  ((s32 (*)())Func_080d47b4)(26, 0x102, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(26, 0x101, 30);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  ((s32 (*)())Func_080d47b4)(26, 0x109, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
L_02009a2e:
  FieldEvent_SetMessageId((s32)MsgSoIThink);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  result3 = Event_GetFlag(7);
  if (result3 == 0)
    goto L_02009ac2;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object4 == 0)
    goto L_02009ab2;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                            *(s16 *)((u8 *)object4 + 18));
L_02009ab2:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_02009ac2:
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 == 0)
    goto L_02009af8;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                            *(s16 *)((u8 *)object5 + 18));
L_02009af8:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 == 0)
    goto L_02009b36;
  Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                            *(s16 *)((u8 *)object6 + 18));
L_02009b36:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 == 0)
    goto L_02009b74;
  Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                            *(s16 *)((u8 *)object7 + 18));
L_02009b74:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  Func_02004a9c();
  return;
}

struct SpawnObject {
  u8 unk00[0x50];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
};

extern const u8 gData_020056d4[];

void Func_02004a14(struct SpawnObject *object, const void *script);
void Func_02004a74(struct SpawnObject *object, s32 priority);

/// Spawns object type 0x1e8 at x, y, z with its sprite turned to value and
/// its timer set.
void Func_02001b9c(s32 x, s32 y, s32 z, s16 value, s32 timer) {
  struct SpawnObject *object;
  struct FieldSprite *sprite;

  object = ((struct SpawnObject *)FieldActor_Create(0x1e8, x, y, z));
  if (object != 0) {
    sprite = object->sprite;
    Func_02004a0c(object, 1);
    Func_02004a14(object, gData_020056d4);
    Func_02004a74(object, 1);
    object->motionFlags = 0;
    {
      /* FAKEMATCH(signature): the timer copied into an s64 local, so it loads
       * before the pooled zero for nextFrame; as object->unk5e = timer, 2 of 35
       * instructions differ in tla-en. */
      s64 wide = timer;

      object->unk5e = wide;
    }
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
