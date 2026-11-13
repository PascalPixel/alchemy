/// @file
/// The scene code of overlay 0x67e, in ROM order.

#include "shared/tables.h"
#include "types.h"
#include "task.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "scene_tables.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s16 gData_02000e10[];
extern struct ActorPlacement gData_02001644[];
extern struct ActorPlacement gData_02001494[];
extern s32 gData_02001b00[];
extern struct Trigger gData_020018b4[];
extern u8 MsgCanYouImagine[];
extern u8 MsgSeeThisRing[];
extern u8 MsgButItS[];
extern u8 MsgSeeThisRingOf[];
extern u8 MsgLadyUzumeAnd[];
extern u8 MsgWouldYouBe[];
extern u8 MsgTheLegendSays[];
extern u8 MsgWait2[];
extern u8 MsgThisTokenIsn[];
extern u8 MsgTheMisalignedStone[];

struct Actor *Actor_GetById(u32 objectId);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Camera_FollowActor(s32 id, s32 mode);
void Actor_SetDestinationOffsetById(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_WalkToById(s32 id, s32 x, s32 z);
void Actor_FaceActorById(s32 id, s32 target, s32 frames);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Map_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Func_02000d60(const u8 *message);
void Event_ShowMessageAndWait(s32 id, s32 flags);
void Func_02000d88(s32 id, s32 a, s32 b);
void Func_02000d90(s32 id, s32 a);
void Func_02000da0(void);
void Func_02000da8(s32 id, s32 value);
void Func_02000db0(s32 value);
void Event_ResolvePendingActions(s32 value);
void Func_02000de0(const s16 *data);
void Func_02000de8(const s16 *data);
void Func_02000df0(void);
void Func_02000df8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Sound_Play(s32 cue);
void Func_0200060c(void);
void Func_020006a4(void);
void Func_02000814(void);
#include "call.h"
#include "flag_call.h"
#include "scene_tables.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "task.h"
#include "version.h"
#include "frame.h"
#include "module/scene/izumo.h"

void Func_02000038(void) { Func_02000de8(gData_02000e10); }

void Func_02000048(void) {
  struct Actor *actor = Actor_GetById(12);
  s32 x = actor->x >> 20;
  s32 z = actor->z >> 20;

  /* FAKEMATCH(wrapper): the flag commands go through flag_call.h's inlines,
   * which build each flag id again, and the block copies through call.h's
   * Call6; as direct calls to GameFlag_Test, GameFlag_SetBit,
   * GameFlag_ClearBit and Map_CopyCellAttributeRect, 27 of 135 instructions
   * differ in tla-en. */
  Func_02000df0();
  Sound_Play(132);
  if (x == 20 && z == 9) {
    if (Flag_IsSet(0x301)) {
      Map_CopyCellPictureIndicesRect(36, 12, 22, 8, 3, 3);
      Call6((void (*)())Map_CopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
    }
    if (!Flag_IsSet(0x924)) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Camera_FollowActor(22, 1);
      Func_02000da0();
      Func_02000d88(22, 257, 0);
      FieldEvent_Wait(60);
      Actor_ArmCallback(22, FACING_WEST, 0);
      FieldEvent_Wait(60);
      Func_02000d88(22, 256, 0);
      FieldEvent_Wait(60);
      Func_02000d60(MsgTheMisalignedStone);
      Event_ShowMessageAndWait(22, 0);
      FieldEvent_Wait(30);
      Func_02000d88(22, 262, 0);
      FieldEvent_Wait(30);
      Event_ShowMessageAndWait(22, 0);
      Actor_ArmCallback(22, FACING_SOUTH, 0);
      Event_End();
      Flag_Set(0x924);
    }
    Flag_Set(0x250);
  } else if (x == 19 && z == 9) {
    Flag_Clear(0x250);
    if (Flag_IsSet(0x301)) {
      Map_CopyCellPictureIndicesRect(36, 8, 22, 8, 3, 3);
      Call6((void (*)())Map_CopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
    }
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (GameFlag_Test(2343))
    return gData_02001644;
  return gData_02001494;
}

void Func_020001cc(void) {
  Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
}

void Func_020001ec(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Test(3999)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 64, PIXELS(456),
          PIXELS(584));
    if (object->x >> 20 == 28 && object->z >> 20 == 36) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Func_02000d90(gMainRoutineWork.currentOwner, 258);
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, -16, 0);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_0200027c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())Func_02000df8, 53, 5, 8, 9, 23, 30, 0);
  if (!GameFlag_Test(4000)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 65, PIXELS(472),
          PIXELS(552));
    if (object->x >> 20 == 29 && object->z >> 20 == 34) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Func_02000d90(gMainRoutineWork.currentOwner, 258);
      Call3((void (*)())Actor_SetDestinationOffsetById,
            gMainRoutineWork.currentOwner, 0, -16);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_0200030c(void) {
  Call7((void (*)())Func_02000df8, 44, 5, 8, 5, 7, 32, 0);
}

void Func_0200032c(void) {
  Call7((void (*)())Func_02000df8, 55, 0, 4, 2, 16, 23, 0);
}

void Func_0200034c(void) {
  Call7((void (*)())Func_02000df8, 42, 0, 6, 2, 9, 18, 0);
}

void Func_0200036c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7((void (*)())Func_02000df8, 42, 0, 6, 2, 9, 18, 0);
  if (!GameFlag_Test(4001)) {
    Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 66, PIXELS(168),
          PIXELS(328));
    if (object->x >> 20 == 10 && object->z >> 20 == 20) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Func_02000d90(gMainRoutineWork.currentOwner, 258);
      Actor_SetDestinationOffsetById(gMainRoutineWork.currentOwner, 16, 0);
      object->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void Func_020003f8(void) {
  Call7((void (*)())Func_02000df8, 49, 0, 5, 4, 24, 16, 0);
}

void Func_02000418(void) {
  Call7((void (*)())Func_02000df8, 36, 0, 5, 3, 16, 9, 0);
}

void Func_02000438(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Sound_Play(123);
  Func_02000db0(*(s16 *)(work + 340));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

const void *Scene_GetEvents(void) {
  if (GameFlag_Test(2343))
    return gData_02001b00;
  return gData_020018b4;
}

static inline s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH(wrapper): the flag test forwarded through the inline, as overlay
   * 679's GameFlag_Test_ does, keeps the actor in r5 and the message in r6; as
   * a direct call to GameFlag_Test the two swap; as that direct call, 16 of 42
   * instructions of Func_020004bc differ in tla-en, as do 16 of 42 of
   * Func_02000538. */
  return GameFlag_Test(flag);
}

/// The ring of stones: one line after the flag is set, otherwise the full talk
/// and its answer.
void Func_020004bc(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test_(0x924)) {
    Func_02000d60(MsgCanYouImagine);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRing;

    Func_02000d60(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02000d60(message + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02000d60(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

void Func_02000538(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test_(0x924)) {
    Func_02000d60(MsgButItS);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    const u8 *message = MsgSeeThisRingOf;

    Func_02000d60(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02000d60(message + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02000d60(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

void Func_020005b4(s32 actor) {
  const u8 *message;

  Event_Start();
  Event_ResolvePendingActions(0);
  message = MsgLadyUzumeAnd;
  Func_02000d60(message);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02000d60(message + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02000d60(message + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
  Event_End();
}

/// Every fourth frame, colours 1 to 14 of palettes 3, 4 and 5 rotate up by one.
void Func_0200060c(void) {
  volatile u16 *palette;
  s16 saved;
  s32 index;

  // The three rotations are goto loops, which loop.c leaves alone, as the
  // native's are; as do-while, while or for loops, 90 instructions differ in
  // tla-en.
  if ((gFrameCount & 3) == 0) {
    saved = *(volatile u16 *)&BG_PLTT_COLOR(3, 14);
    index = 14;
  first:
    palette = (u16 *)&BG_PLTT_COLOR(3, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)&BG_PLTT_COLOR(3, 1) = saved;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(4, 14);
    index = 14;
  second:
    palette = (u16 *)&BG_PLTT_COLOR(4, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)&BG_PLTT_COLOR(4, 1) = saved;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(5, 14);
    index = 14;
  third:
    palette = (u16 *)&BG_PLTT_COLOR(5, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto third;
    *(volatile u16 *)&BG_PLTT_COLOR(5, 1) = saved;
  }
}

void Func_020006a4(void) {
  /* FAKEMATCH(signature): six routines called through call.h's Call3 and Value
   * with function types they do not have (Value3 calls the void Func_02000d88
   * as returning a value), so each call's arguments are set up in the native's
   * order; as all six direct calls, 12 of 140 instructions differ in tla-en. */
  GameFlag_SetBit(0x929);
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_02000d60(MsgWait2);
  Actor_WalkToById(gMainRoutineWork.currentOwner, 200, 264);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Event_ShowMessageAndWait(32, 0);
  FieldEvent_Wait(15);
  Value3((s32 (*)())Func_02000d88, gMainRoutineWork.currentOwner, 256, 30);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 32, PIXELS(200),
        PIXELS(216));
  Actor_WalkToById(32, 200, 248);
  Actor_WaitForMoveById(32);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, 32, 0);
  FieldEvent_Wait(30);
  Actor_SetModeById(32, 4);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Value3((s32 (*)())Func_02000d88, gMainRoutineWork.currentOwner, 261, 60);
  Func_02000d88(32, 262, 30);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(15);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 32, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    FieldEvent_Wait(90);
    Call3((void (*)())Actor_SetSpeedParameters, 32, 0x6666, 0x3333);
    Actor_WalkToById(32, 200, 216);
    Actor_WaitForMoveById(32);
    Func_02000da8(32, 1);
    FieldEvent_Wait(60);
    Call3((void (*)())Actor_ArmCallback, 32, FACING_SOUTH, 0);
    Func_02000d88(32, 265, 30);
    Event_ShowMessageAndWait(32, 0);
    Actor_SetSpritePriority(32, 3);
    Actor_GetById(32)->motionFlags |= 2;
    GameFlag_SetBit(0x302);
  } else {
    Func_02000814();
  }
  Event_End();
}

void Func_02000814(void) {
  Func_02000d60(MsgThisTokenIsn);
  Func_02000d88(32, 264, 30);
  Event_ShowMessageAndWait(32, 0);
  Actor_ArmCallback(32, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceActorById(32, gMainRoutineWork.currentOwner, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_StartRepeatedMotion(32, 1);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(32, 4);
  FieldEvent_Wait(30);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(15);
  Func_02000d88(32, 265, 30);
  Event_ShowMessageAndWait(32, 0);
  Actor_FaceActorById(32, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(15);
  Event_ShowMessageAndWait(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_Wait(15);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Actor_SetSpeedParameters(32, 0x3333, 0x1999);
  Actor_GetById(32)->motionFlags = 0;
  Actor_SetSpritePriority(32, 3);
  Actor_WalkToById(32, 200, 208);
  Actor_WaitForMoveById(32);
  Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
  GameFlag_SetBit(0x92a);
  GameFlag_ClearBit(0x302);
}

void Func_02000954(void) {
  if (!GameFlag_Test(2346)) {
    const u8 *message;

    Event_Start();
    Event_ResolvePendingActions(0);
    Func_02000d88(32, 256, 60);
    message = MsgWouldYouBe;
    Func_02000d60(message);
    /* FAKEMATCH(signature): FieldMessage_ShowAtActor called as returning a
     * value, so the flags load before the actor as the native schedules it;
     * as FieldMessage_ShowAtActor(32, 0) through the header's void prototype,
     * 2 of 43 instructions differ in tla-en. */
    ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(32, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Func_02000814();
    } else {
      Func_02000d60(message - 1);
      Event_ShowMessageAndWait(32, 0);
    }
    Event_End();
  } else {
    Event_ShowMessageAndWait((s32)MsgTheLegendSays, 0);
  }
}

s32 Scene_Initialize(void) {
  GameFlag_ClearBit(0x302);
  WORD_AT(Ram_HeapSlots->eventWork, 428) = 0x100;
  Task_Add(Func_0200060c, TASK_PASS_FRAME);
  Func_02000de0(gData_02000e10);
  if (GameFlag_Test(0x927)) {
    BYTE_AT(Actor_GetById(15), 90) = 2;
    Actor_SetModeById(25, 5);
    Actor_SetModeById(26, 5);
    Actor_SetModeById(27, 5);
    Actor_SetModeById(28, 5);
    Actor_SetModeById(29, 6);
    Actor_SetModeById(30, 6);
    Actor_SetModeById(31, 5);
    if (gMainRoutineWork.entrance == 8 && GameFlag_Test(0x928) &&
        !GameFlag_Test(0x929) && !GameFlag_Test(0x92a)) {
      Event_StartScreenTransition();
      Func_020006a4();
    }
    if (GameFlag_Test(0x929) && !GameFlag_Test(0x92a)) {
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 32, PIXELS(200),
            PIXELS(216));
      Actor_SetSpritePriority(32, 3);
      Actor_GetById(32)->motionFlags |= 2;
      GameFlag_SetBit(0x302);
    }
  }
  Actor_SetSpritePriority(24, 3);
  Actor_GetById(24)->motionFlags |= 2;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000b08(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  GameFlag_SetBit(769);
  if (GameFlag_Test(592)) {
    Map_CopyCellPictureIndicesRect(36, 12, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 12, 3, 3, 22, 8);
    if (x == 23 && z == 9)
      Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                             PIXELS(376), PIXELS(136));
  } else {
    Map_CopyCellPictureIndicesRect(36, 8, 22, 8, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 36, 8, 3, 3, 22, 8);
  }
}

void Func_02000ba4(void) {
  GameFlag_ClearBit(769);
  Map_CopyCellPictureIndicesRect(40, 8, 22, 8, 3, 3);
  Call6((void (*)())Map_CopyCellAttributeRect, 40, 8, 3, 3, 22, 8);
}

void Func_02000bdc(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 340);

  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  BYTE_AT(Actor_GetById(4), 90) &= ~1;
  Actor_SetDestinationOffsetById(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
#if TLA_PATCH_1 /* From patch 1. */
  Sound_Play(129);
#endif
  Actor_SetDestinationOffsetById(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  FieldEvent_Wait(14);
  Func_02000db0(value);
}

void Func_02000c58(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, 4, 0, -14);
  Actor_WalkByAndWaitById(4, 16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000c7c(void) {
  Call3((void (*)())Actor_WalkByAndWaitById, 4, 0, -14);
  Actor_WalkByAndWaitById(4, -16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000ca4(void) {
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

#include "text/msg_ids.h"
#include "objcmd.h"

s16 gData_02000e10[] = {
    12,
    -1,
};

// clang-format off
DEFINE_G_DATA_02000E14(gData_02000e14)

DEFINE_G_DATA_02000F14(gData_02000f14)

DEFINE_G_DATA_02001014(gData_02001014, 0x3333)

DEFINE_G_DATA_02000E14(gData_02001114)

DEFINE_G_DATA_02000F14(gData_02001214)

DEFINE_G_DATA_02001014(gData_02001314, 0x4ccc)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(171),
    SCENE_EXIT_IF(1, 2, 38, -1),
    SCENE_EXIT_IF(2, 170, 1, -1),
    SCENE_EXIT_IF(3, 170, 2, -1),
    SCENE_EXIT_IF(4, 170, 3, -1),
    SCENE_EXIT_IF(5, 170, 4, -1),
    SCENE_EXIT_IF(6, 170, 5, -1),
    SCENE_EXIT_IF(7, 170, 6, -1),
    SCENE_EXIT_IF(8, 170, 7, -1),
    SCENE_EXIT_IF(10, 172, 1, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001494[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {117, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(488), FACING_WEST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(488), FACING_SOUTH, 0, 0},
    {118, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(568), FACING_SOUTH, 1, 0},
    {122, CONDITION_ALWAYS, 3, PIXELS(248), 0, PIXELS(464), FACING_NORTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(344), FACING_SOUTH, 0, 0},
    {125, CONDITION_ALWAYS, 3, PIXELS(424), 0, PIXELS(600), FACING_SOUTH, 1, 0},
    {126, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(488), FACING_NORTHEAST, 1, 0},
    {127, CONDITION_ALWAYS, 3, PIXELS(360), 0, PIXELS(392), FACING_WEST, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(117), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gData_02001644[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(184), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(152), FACING_NORTH, 2, 0},
    {117, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(488), FACING_WEST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(552), FACING_SOUTH, 2, 0},
    {118, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(568), FACING_SOUTH, 1, 0},
    {122, CONDITION_ALWAYS, 3, PIXELS(472), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(344), FACING_SOUTH, 0, 0},
    {125, CONDITION_ALWAYS, 3, PIXELS(424), 0, PIXELS(568), FACING_SOUTH, 1, 0},
    {126, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(488), FACING_NORTHEAST, 1, 0},
    {127, CONDITION_ALWAYS, 3, PIXELS(360), 0, PIXELS(392), FACING_WEST, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(117), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {117, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(155), FACING_SOUTH, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gData_02000e14, PIXELS(256), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gData_02000f14, PIXELS(264), 0, PIXELS(504), FACING_SOUTH, 1, 0},
    {120, CONDITION_ALWAYS, (s32)gData_02001014, PIXELS(360), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(472), FACING_NORTHEAST, 1, 0},
    {124, CONDITION_ALWAYS, (s32)gData_02001114, PIXELS(232), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {124, CONDITION_ALWAYS, (s32)gData_02001214, PIXELS(328), 0, PIXELS(504), FACING_SOUTH, 1, 0},
    {125, CONDITION_ALWAYS, (s32)gData_02001314, PIXELS(336), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {29, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_020018b4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {0x4602, 10, CONDITION_ALWAYS, EVENT(Func_02000ca4)},
    {0x8602, 10, CONDITION_ALWAYS, EVENT(Func_02000c7c)},
    {0x602, 10, CONDITION_ALWAYS, EVENT(Func_02000c58)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgWeVeHad)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgMyNameS)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeDoesn)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgSusaSGoing)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAsAChild)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgChoosingTheNext)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTheDaughtersOf)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgABraveWarrior)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Func_020004bc)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgYeahSusaCame)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgEvenIfSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgTheGreatSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIMNotGoing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgIMSureThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgWhatIfLosing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeLoves)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgSacrificesAsGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgKushinadaSAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgWhyWonT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoesLady)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgYouKnowI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeSays)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000048)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     CONDITION_ALWAYS, EVENT(Func_02000038)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS,
     EVENT(Func_02000048)},
    {0x50008905, 21, CONDITION_ALWAYS, EVENT(Func_020001cc)},
    {0x50008905, 22, CONDITION_ALWAYS, EVENT(Func_020001ec)},
    {0x50008905, 23, CONDITION_ALWAYS, EVENT(Func_0200027c)},
    {0x50008905, 24, CONDITION_ALWAYS, EVENT(Func_0200030c)},
    {0x50008905, 25, CONDITION_ALWAYS, EVENT(Func_0200032c)},
    {0x50008905, 26, CONDITION_ALWAYS, EVENT(Func_0200034c)},
    {0x50008905, 27, CONDITION_ALWAYS, EVENT(Func_0200036c)},
    {0x50008905, 28, CONDITION_ALWAYS, EVENT(Func_020003f8)},
    {0x50008905, 29, CONDITION_ALWAYS, EVENT(Func_02000418)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_02000b08)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_02000ba4)},
    {TRIGGERS_END},
};

s32 gData_02001b00[] = {
    1,
    (s32)0xffff0001,
    1,
    0xc602,
    (s32)0xffff0002,
    (s32)Func_02000438,
    0xc602,
    (s32)0xffff0003,
    (s32)Func_02000438,
    0xc602,
    (s32)0xffff0004,
    (s32)Func_02000438,
    0xc602,
    (s32)0xffff0005,
    (s32)Func_02000438,
    0xc602,
    (s32)0xffff0006,
    (s32)Func_02000438,
    0xc602,
    (s32)0xffff0007,
    (s32)Func_02000438,
    0xc602,
    0x3020008,
    (s32)Func_02000438,
    0x4602,
    (s32)0xffff000a,
    (s32)Func_02000ca4,
    0x8602,
    (s32)0xffff000a,
    (s32)Func_02000c7c,
    0x602,
    (s32)0xffff000a,
    (s32)Func_02000c58,
    0,
    (s32)0xffff000e,
    MSGNO(MsgThoseScreamsFrom),
    0,
    (s32)0xffff000f,
    MSGNO(MsgWeRePlanning),
    0,
    (s32)0xffff0010,
    MSGNO(MsgSusaHasChanged),
    0,
    (s32)0xffff0011,
    MSGNO(MsgKushinadaHasBeen),
    0,
    (s32)0xffff0012,
    MSGNO(MsgIVeHeardPeople),
    0,
    (s32)0xffff0013,
    MSGNO(MsgSusaHasGrown),
    0,
    (s32)0xffff0014,
    MSGNO(MsgSusaHasPut),
    0,
    (s32)0xffff0015,
    MSGNO(MsgLadyUzumePredicted),
    0,
    (s32)0xffff0016,
    (s32)Func_02000538,
    0,
    (s32)0xffff0017,
    MSGNO(MsgIfYouReLooking),
    0,
    (s32)0xffff0018,
    MSGNO(MsgDidYouHear),
    0,
    (s32)0xffff0019,
    MSGNO(MsgIWonderIf),
    0,
    (s32)0xffff001a,
    MSGNO(MsgDancingLikeThis),
    0,
    (s32)0xffff001b,
    MSGNO(MsgEveryoneSaysThat),
    0,
    (s32)0xffff001c,
    MSGNO(MsgWeMayNever),
    0,
    (s32)0xffff001d,
    MSGNO(MsgOurIslandIs),
    0,
    (s32)0xffff001e,
    MSGNO(MsgTheSerpentTaught),
    0,
    (s32)0xffff001f,
    (s32)Func_020005b4,
    0,
    (s32)0xffff0020,
    (s32)Func_02000954,
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgIHadAlways),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgThisCelebrationWill),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgItSOdd),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgKushinadaIsSuch),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgWhatWouldMake),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgAllTheGirls),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgEveryoneSBeen),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgSoSusaReally),
    0x8d15,
    (s32)0xffff0016,
    MSGNO(MsgSusaDidA),
    0x8d15,
    (s32)0xffff0017,
    MSGNO(MsgLadyUzumeAndKushinada),
    0x8d15,
    (s32)0xffff0018,
    MSGNO(MsgKushinadaMustBe),
    0x8d15,
    (s32)0xffff0019,
    MSGNO(MsgAllAnyoneTalks),
    0x8d15,
    (s32)0xffff001a,
    MSGNO(MsgADragonA),
    0x8d15,
    (s32)0xffff001b,
    MSGNO(MsgSusaSaidThe),
    0x8d15,
    (s32)0xffff001c,
    MSGNO(MsgWeNeedTo),
    0x8d15,
    (s32)0xffff001d,
    MSGNO(MsgDancingIsGood),
    0x8d15,
    (s32)0xffff001e,
    MSGNO(MsgThatSusaUsed),
    0x8d15,
    (s32)0xffff001f,
    MSGNO(MsgAsLongAsWe),
    0x8d15,
    (s32)0xffff0420,
    (s32)Func_02000954,
    0x10008c15,
    (s32)0xffff000c,
    (s32)Func_02000038,
    0x8c15,
    (s32)0xffff000c,
    (s32)Func_02000048,
    8,
    -0x10000,
    (s32)Func_02000038,
    9,
    -0x10000,
    (s32)Func_02000048,
    0x50009085,
    -0x10000,
    (s32)Func_02000b08,
    0x40009085,
    -0x10000,
    (s32)Func_02000ba4,
#if TLA_PATCH_1 /* From patch 1. */
    0x50008905,
    (s32)0xffff0015,
    (s32)Func_020001cc,
    0x50008905,
    (s32)0xffff0016,
    (s32)Func_020001ec,
    0x50008905,
    (s32)0xffff0017,
    (s32)Func_0200027c,
    0x50008905,
    (s32)0xffff0018,
    (s32)Func_0200030c,
    0x50008905,
    (s32)0xffff0019,
    (s32)Func_0200032c,
    0x50008905,
    (s32)0xffff001a,
    (s32)Func_0200034c,
    0x50008905,
    (s32)0xffff001b,
    (s32)Func_0200036c,
    0x50008905,
    (s32)0xffff001c,
    (s32)Func_020003f8,
    0x50008905,
    (s32)0xffff001d,
    (s32)Func_02000418,
#endif
    -1,
    0,
    0,
};

u16 gData_02001e0c[] = {
    1,
};
