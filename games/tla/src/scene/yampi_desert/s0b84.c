/// @file
/// Scene code of overlay 0x661, in ROM order, from s0b84's offset up to the next object the overlay links.

#include "types.h"
#include "field_effect.h"
#include "scene.h"
#include "scene_tables.h"
#include "actor.h"
#include "call.h"
#include "main_routine_work.h"

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);

void Func_02000b84(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Math_Cosine(i << 14);
    velocity[1] = 0;
    velocity[2] = Math_Sine(i << 14);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((u32)(Random16Far() << 17) >> 16) + -0x10000;
    velocity[2] += (u16)Random16Far() + -0x8000;
    Func_020000b8(x, y, z, velocity[0], velocity[1], velocity[2], 0x01000001,
                  &options);
  }
}

void Func_02001c5c(void);

void Func_02000c34(void) {
  Func_02004040();
  Func_020041b8(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  Func_02004118(0x26666, 0x4ccc);
  Scene_CamBounds(PIXELS(300), -1, PIXELS(248), 1);
  Actor_SetPositionAndCommit(16, 254, 248);
  Actor_SetPositionAndCommit(16, 352, 240);
  Task_RemoveFar(Func_02002180);
  FieldEvent_Wait(10);
  Task_RemoveFar(Func_02001c5c);
  wait(1);
  Func_02004090(17, 0, 0);
  FieldEvent_Wait(40);
  Func_02000b84(0x1600000, 0, 0xf80000);
  Func_020041f8(176);
  Actor_GetById(16)->velocityY = 0x60000;
  Actor_SetPartAttribute(Actor_GetById(16), 0);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Actor_SetPositionAndCommit(16, 380, 248);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Func_020041f8(241);
  Scene_SetSpeed(16, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(16, 428, 248);
  Func_020041f8(390);
  Func_02004090(16, 0, 0);
  GameFlag_SetBit(0x889);
  Func_02004048();
}

void Func_02000d50(void) {
  Func_02004040();
  Func_020041b8(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  Func_02004118(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(246), -1, PIXELS(616), 1);
  Scene_SetPositionAndCommit(21, 248, 648);
  Scene_SetPositionAndCommit(21, 248, 576);
  Task_RemoveFar(Func_02002180);
  FieldEvent_Wait(10);
  Task_RemoveFar(Func_02001c5c);
  wait(1);
  Func_02004090(22, 0, 0);
  FieldEvent_Wait(20);
  Func_02000b84(0xf80000, 0, 0x2400000);
  Func_020041f8(176);
  Actor_GetById(21)->velocityY = 0x60000;
  Actor_SetPartAttribute(Actor_GetById(21), 0);
  Actor_SetSingleChildFlags(Actor_GetById(21), 1);
  Actor_SetPositionAndCommit(21, 256, 560);
  FieldEvent_Wait(20);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Func_020041f8(241);
  Scene_SetSpeed(21, 0xcccc, 0x6666);
  Scene_SetPositionAndReset(21, 248, 496);
  Func_020041f8(390);
  Func_02004090(21, 0, 0);
  GameFlag_SetBit(0x88a);
  Func_02004048();
}

void Func_02000e78(void) {
  Func_02004040();
  Func_020041b8(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  Func_02004118(0x26666, 0x4ccc);
  Scene_CamBounds(PIXELS(232), -1, PIXELS(328), 1);
  Scene_SetPositionAndCommit(21, 232, 328);
  Camera_MoveWithinMapBounds(PIXELS(232), -1, PIXELS(200), 1);
  Scene_SetPositionAndCommit(21, 232, 280);
  Actor_SetPositionAndCommit(21, 232, 168);
  Task_RemoveFar(Func_02002180);
  Actor_ArmCallback(21, FACING_SOUTH, 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  FieldEvent_Wait(20);
  GameFlag_SetBit(0x88b);
  Func_02004048();
}

extern u32 gData_020042b4[];
extern u32 gData_02005828[];

struct Actor *Func_02003fc0(s32 kind, s32 x, s32 y, s32 z);
void Func_02003fb8(struct Actor *object, const void *script);

/// Spawns an object of kind 285 at each of the fifteen cells of
/// gData_020042b4, twice, each moved by -2 to +1 pixels at random.
void Func_02000f24(void) {
  u32 pass;
  u32 i;

  for (pass = 0; pass <= 1; pass++) {
    u16 *cell;

    cell = (u16 *)gData_020042b4;
    for (i = 0; i <= 14; i++) {
      s32 x = *cell++ + ((Random16Far() << 2) >> 16);
      s32 z = *cell++ + ((Random16Far() << 2) >> 16);
      struct Actor *object =
          Func_02003fc0(285, (x - 2) << 16, 0, (z - 2) << 16);

      if (object != NULL) {
        object->speedLimit = 0x12000;
        object->acceleration = 0x8000;
        Actor_SetSingleChildFlags((struct FieldActor *)object, 0);
        Func_02003fa8((struct FieldActor *)object, 1);
        Func_02003fb8(object, gData_02005828);
      }
    }
  }
  Func_02003ff0(74, 71, 77, 9, 3, 3);
}

void Func_02002ca0(void);
void Func_02004100(s32 actor, s32 a, s32 b);
void Func_02004108(s32 actor, s32 a);

#define OBJ(p) ((struct Actor *)(p))

void Func_02000fc8(void) {
  struct Actor *leader = OBJ(Actor_GetById(gMainRoutineWork.currentOwner));
  struct Actor *partner = OBJ(Actor_GetById(21));
  u32 i;
  s32 angle;

  if (!GameFlag_Test(0x88b))
    return;
  Func_02004040();
  Func_020041b8(0);
loop_1:
  OBJ(gData_0200480c)->scaleX += -0x147a;
  OBJ(gData_0200480c)->scaleY += -0x147a;
  wait(1);
  if (0xcccb < OBJ(gData_0200480c)->scaleX)
    goto loop_1;
loop_2:
  OBJ(gData_0200480c)->scaleX += 0x1999;
  OBJ(gData_0200480c)->scaleY += 0x1999;
  wait(1);
  if (0x1cccc >= OBJ(gData_0200480c)->scaleX)
    goto loop_2;
loop_3:
  OBJ(gData_0200480c)->scaleX += -0xa3d;
  OBJ(gData_0200480c)->scaleY += -0xa3d;
  wait(1);
  if (0x3332 < OBJ(gData_0200480c)->scaleX)
    goto loop_3;
  Func_02004090(22, 0, 0);
  FieldEvent_Wait(20);
  Scene_CamBounds(PIXELS(232), -1, PIXELS(196), 1);
  Func_02004100(gMainRoutineWork.currentOwner, 0x101, 0);
  Scene_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 232, 164);
  FieldEvent_Wait(20);
  angle = 0x4000;
  Scene_Call4098(21, 232 << 16, 184 << 16, angle);
  wait(1);
  Func_02000b84(232 << 16, 0, 184 << 16);
  Func_020041f8(176);
  partner->velocityY = 0x60000;
  partner->priorityFlags &= ~2;
  Actor_SetSpritePriority(21, 2);
  Actor_SetPartAttribute(Actor_GetById(21), 0);
  Actor_SetSingleChildFlags(Actor_GetById(21), 1);
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Actor_SetPositionAndCommit(21, 216, 184);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, angle, 0);
  Func_020040e8(21, 0);
  FieldEvent_Wait(20);
  Func_02002ca0();
  Func_020041f8(136);
  Func_02000f24();
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  FieldEvent_Wait(20);
  Func_02004108(gMainRoutineWork.currentOwner, 0x102);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(10);
  Func_020041f8(204);
  i = 0;
step_13:
  leader->scaleX += -0x400;
  leader->scaleY += -0x400;
  leader->y += -0x20000;
  wait(1);
  i++;
  if (i <= 13)
    goto step_13;
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Func_020041f8(153);
  partner->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(21), 0);
  Actor_SetPositionAndCommit(21, 232, 168);
  FieldEvent_Wait(3);
  Func_020041f8(204);
  i = 0;
step_5:
  partner->scaleX += -0x800;
  partner->scaleY += -0x800;
  partner->y += -0x70000;
  wait(1);
  i++;
  if (i <= 5)
    goto step_5;
  Func_02004090(21, 0, 0);
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004138(4);
}

void Func_02001288(s32 mode) {
  struct FieldActor *object;
  s32 height;

  if (mode == 1) {
    object = Actor_GetById(9);
    Func_02003ff0(35, 39, 13, 11, 3, 3);
    Scene_Call4090(9, PIXELS(232), PIXELS(200));
    height = Map_GetTerrainHeight(0, object->x, object->z);
    object->y = height;
    object->terrainHeight = height;
    GameFlag_SetBit(0x8ed);
  }
}

s32 Func_020012d4(s32 cell) { return cell * 16 + 8; }

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    s32 x_ = Func_020012d4(column);                                            \
    s32 z_ = Func_020012d4(row);                                               \
    Func_02004090(gMainRoutineWork.currentOwner, x_ << 16, z_ << 16);               \
  }

void Func_020012dc(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldActor *object = Actor_GetById(64);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (object != NULL)
    Scene_Call4090(64, 154 << 18, 222 << 17);
  Func_02003ff0(82, 82, 15, 40, 5, 6);
  Scene_Call4000(82, 82, 5, 6, 15, 40);
  Func_02003ff0(88, 82, 36, 25, 5, 5);
  Scene_Call4000(88, 82, 5, 5, 36, 25);
  if (column == 15) {
    if (row == 41)
      MOVE_LEADER_TO(14, 41)
    else if (row == 43)
      MOVE_LEADER_TO(16, 43)
  } else if (column == 16) {
    if (row == 40)
      MOVE_LEADER_TO(16, 39)
    else if (row == 41)
      MOVE_LEADER_TO(16, 42)
  } else if (column == 18) {
    if (row == 40)
      MOVE_LEADER_TO(18, 41)
    else if (row == 43)
      MOVE_LEADER_TO(17, 43)
  }
}

// Const: the cell helper only computes from its argument.
s32 Func_020012d4(s32 cell) __attribute__((const));

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    struct MainRoutineWork *work = &gMainRoutineWork;                          \
    s32 x = Func_020012d4(column);                                             \
    s32 z = Func_020012d4(row);                                                \
    Func_02004090(work->currentOwner, x << 16, z << 16);                       \
  }

void Func_020013f8(void) {
  struct Actor *leader =
      (struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (Actor_GetById(64) != NULL)
    Func_02004090(64, 0, 0);
  Func_02003ff0(82, 89, 15, 40, 5, 6);
  Scene_Call4000(82, 89, 5, 6, 15, 40);
  Func_02003ff0(88, 89, 36, 25, 5, 5);
  Scene_Call4000(88, 89, 5, 5, 36, 25);
  if (column == 15 && row == 44)
    MOVE_LEADER_TO(15, 43)
  else if (column == 17 && row == 44)
    MOVE_LEADER_TO(17, 43)
  else if (column == 19) {
    if (row == 40)
      MOVE_LEADER_TO(19, 41)
    else if (row == 42)
      MOVE_LEADER_TO(18, 42)
  }
}

void Func_02001518(void) {
  struct FieldActor *object = Actor_GetById(64);
  struct FieldActor *leader;

  if (object != 0) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    Scene_Call4090(64, PIXELS(288), PIXELS(240));
    if ((leader->x + 0x80000) >> 20 == (object->x + 0x80000) >> 20 &&
        (leader->z + 0x80000) >> 20 == (object->z + 0x80000) >> 20)
      Func_02004090(gMainRoutineWork.currentOwner, PIXELS(288), PIXELS(256));
  }
}

void Func_0200157c(void) {
  if (Actor_GetById(64))
    Func_02004090(64, 0, 0);
}

void Func_02001594(void) {
  Func_02003ff0(64, 64, 72, 32, 1, 1);
  Func_02004000(64, 64, 1, 1, 8, 32);
  Func_02004000(64, 64, 1, 1, 8, 96);
}

void Func_020015d4(s32 mode) {
  if (mode == 1) {
    Func_02001594();
    GameFlag_SetBit(0x88d);
  }
}

void Func_020015ec(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov661Room8)
    Func_02001650(show, 11, 2);
  else if (scene == (s32)&SceneId_Ov661Room12)
    Func_02001650(show, 9, 7);
}

void Func_02001628(void) {
  Func_020015ec(1);
  GameFlag_SetBit(0x20d);
}

void Func_0200163c(void) {
  Func_020015ec(0);
  GameFlag_ClearBit(0x20d);
}

void Func_02001650(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct FieldActor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

s32 Func_02004130();

void Func_02004128();
s32 Actor_Jump();
s32 Motion_SetModeAndWaitAnimation();

s32 Actor_SetMovementTypeById();

s32 Actor_SetMovementTypeAndWaitById();

extern u8 MsgHeyLookUp[];
extern u8 gData_0200468c[];
extern u8 gData_020046dc[];
extern u8 gData_02004718[];

void Func_02001690(s32 p0) {
  s32 *owner;
  s32 angle;

  Call1((void (*)())Func_02004040, p0);
  Call1((void (*)())Func_020041b8, 0);
  Call1((void (*)())Func_020040c8, (s32)MsgHeyLookUp);
  Call1((void (*)())Actor_Stop, 9);
  Call1((void (*)())wait, 1);
  Value3((s32 (*)())Func_02004100, 9, 0x100, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0, 20);
  Call2((void (*)())Func_020040d0, 9, 0);
  Call1((void (*)())Actor_Stop, 8);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Actor_ArmCallback, 8, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 13, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 14, 0xd000, 0);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  Call3((void (*)())Func_02004100, *owner, 0x100, 0);
  Call3((void (*)())Actor_ArmCallback, *owner, 0x8000, 0);
  ((struct Actor *)Func_02004130())->motionFlags = 0;
  Call2((void (*)())Func_02004118, 0x19999, 0x3333);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xd40000, 0, 0xd40000, 1);
  Func_02004128();
  Call2((void (*)())Func_020040d0, 12, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0, 10);
  Call2((void (*)())Func_020040d0, 11, 0);
  Call3((void (*)())Actor_ArmCallback, 13, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 14, 0x5000, 20);
  Call3((void (*)())Actor_Jump, 14, 4, 40);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 13, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 14, 3);
  Call2((void (*)())Actor_WatchAndSetScript, 8, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 9, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 10, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 11, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 12, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 13, 14);
  Call3((void (*)())Actor_SetSpeedParameters, 13, 0x10000, 0x8000);
  Call2((void (*)())Actor_SetMovementTypeById, 14, (s32)gData_0200468c);
  Call2((void (*)())Func_02004118, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xf00000, -1, 0x1040000, 1);
  Func_02004128();
  Call1((void (*)())Actor_Stop, 8);
  Call1((void (*)())Actor_Stop, 9);
  Call1((void (*)())Actor_Stop, 10);
  Call1((void (*)())Actor_Stop, 11);
  Call1((void (*)())Actor_Stop, 12);
  Call1((void (*)())Actor_Stop, 13);
  Call2((void (*)())Func_02004118, 0x26666, 0x4ccc);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xd40000, -1, 0xd40000, 1);
  Func_02004128();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 8, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 20);
  Call2((void (*)())Func_020040d0, 9, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 3);
  Call2((void (*)())Func_020040d0, 8, 0);
  Call3((void (*)())Actor_Jump, 10, 4, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call2((void (*)())Actor_SetModeById, 10, 4);
  Call2((void (*)())Func_020040d0, 10, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x8000, 0);
  Value3((s32 (*)())Func_02004100, 8, 0x102, 0);
  Func_02004100(9, 0x102, 80);
  Call2((void (*)())Func_020040d0, 14, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 13, 0x8000, 20);
  Call2((void (*)())Actor_SetModeById, 8, 3);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call2((void (*)())Actor_SetModeById, 10, 3);
  Call2((void (*)())Actor_SetModeById, 11, 3);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 13, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 13, 0x19999, 0xcccc);
  Call2((void (*)())Actor_WatchAndSetScript, *owner, 13);
  {
    s32 path = (s32)gData_020046dc;

    Call2((void (*)())Actor_SetMovementTypeById, 13, path);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call2((void (*)())Actor_SetMovementTypeById, 12, path);
    Call2((void (*)())Func_02004118, 0x6666, 0xccc);
    Call4((void (*)())Camera_MoveWithinMapBounds, 0xf00000, -1, 0x1180000, 1);
    Call2((void (*)())Actor_SetMovementTypeById, 8, path);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call2((void (*)())Actor_SetMovementTypeById, 9, path);
  }
  {
    s32 path = (s32)gData_02004718;

    Call2((void (*)())Actor_SetMovementTypeById, 10, path);
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Actor_SetMovementTypeAndWaitById, 11, path);
  }
  Call1((void (*)())Actor_Stop, *owner);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_02004090, 8, 0, 0);
  Call3((void (*)())Func_02004090, 9, 0, 0);
  Call3((void (*)())Func_02004090, 10, 0, 0);
  Call3((void (*)())Func_02004090, 11, 0, 0);
  Call3((void (*)())Func_02004090, 12, 0, 0);
  Call3((void (*)())Func_02004090, 13, 0, 0);
  Call3((void (*)())Func_02004090, 14, 0, 0);
  angle = 0xb000;
  Call4((void (*)())Func_02004098, 8, 0x2080000, 0xf20000, angle);
  {
    s32 facing = 0x8000;

    Call4((void (*)())Func_02004098, 9, 0x2150000, 0x10f0000, facing);
  }
  {
    s32 facing = 0x3000;

    Call4((void (*)())Func_02004098, 10, 0x1b30000, 0xe80000, facing);
  }
  {
    s32 facing = 0x5000;

    Call4((void (*)())Func_02004098, 11, 0x1d30000, 0xf80000, facing);
  }
  {
    s32 facing = 0xd000;

    Call4((void (*)())Func_02004098, 12, 0x1ce0000, 0x1540000, facing);
    Call4((void (*)())Func_02004098, 13, 0x1e20000, 0x1540000, facing);
  }
  Call4((void (*)())Func_02004098, 14, 0x1e90000, 0x1400000, angle);
  Call2((void (*)())Actor_SetModeById, 8, 1);
  Call2((void (*)())Actor_SetModeById, 9, 1);
  Call2((void (*)())Actor_SetModeById, 10, 1);
  Call2((void (*)())Actor_SetModeById, 11, 1);
  Call2((void (*)())Actor_SetModeById, 12, 1);
  Call2((void (*)())Actor_SetModeById, 13, 1);
  Call2((void (*)())Actor_SetModeById, 14, 1);
  Call1((void (*)())GameFlag_SetBit, 0x8f0);
  Call1((void (*)())GameFlag_SetBit, 0x88f);
  Func_02004048();
}
