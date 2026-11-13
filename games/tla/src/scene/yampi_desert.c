/// @file
/// Scene code of overlay 0x661, in ROM order, from s0b84's offset up to the
/// next object the overlay links.

// ---- games/tla/src/scene/yampi_desert/s0b84.c ----

#include "shared/map_cells.h"
#include "shared/actors.h"
#include "types.h"
#include "system.h"
#include "field_effect.h"
#include "event_work.h"
#include "flag_call.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "module/scene/yampi_desert.h"
#include "module/tla.h"
#include "resource.h"
#include "text/msg_ids.h"

void Import_ActorShowEmoteById(s32 actor, s32 a, s32 b);
s32 Import_FieldCameraGetActor();
void Import_ActorSelectAnimation(struct Actor *object, s32 mode);
void Oam_AddSpriteAtDepthClamped(u32 *oam, s32 depth);
/* VIEW(s0b84): routine Func_0200388c as void Func_0200388c(const void *script,
 * s32 a, s32 b); the definition has void Func_0200388c(u16 *list, s32 mode, s32
 * count); tried as the definition's void Func_0200388c(u16 *list, s32 mode, s32
 * count), which the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/yampi_desert.c:1695: conflicting types for
 * `Func_0200388c'. */
extern void Func_0200388c__s0b84(const void *script, s32 a,
                                 s32 b) __asm__("Func_0200388c");
#define Func_0200388c Func_0200388c__s0b84
#include "yampi_desert/scene.h"
#undef Func_0200388c
#include "scene_tables.h"
#include "actor.h"
#include "call.h"
#include "main_routine_work.h"

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
s32 Math_Sine(s32 angle);

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

/// Pound on the first screen's wooden column: the scorpion moving under the
/// sand bursts out and flees to the next screen (flag 0x889).
void YampiDesert_StirScorpion(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Scene_CamBounds(PIXELS(300), -1, PIXELS(248), 1);
  Actor_MoveToAndWaitById(16, 254, 248);
  Actor_MoveToAndWaitById(16, 352, 240);
  Task_RemoveFar(Func_02002180);
  FieldEvent_Wait(10);
  Task_RemoveFar(Func_02001c5c);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  FieldEvent_Wait(40);
  Func_02000b84(0x1600000, 0, 0xf80000);
  Sound_Play(176);
  Actor_GetById(16)->velocityY = 0x60000;
  Actor_SetSpriteColor(Actor_GetById(16), 0);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Actor_MoveToAndWaitById(16, 380, 248);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Sound_Play(241);
  Scene_SetSpeed(16, 0xcccc, 0x6666);
  Actor_WalkToAndWaitById(16, 428, 248);
  Sound_Play(390);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  GameFlag_SetBit(0x889);
  Event_End();
}

/// As YampiDesert_StirScorpion, on the second screen (flag 0x88a).
void YampiDesert_StirScorpionAgain(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  FieldCamera_MoveWithinMapBounds(PIXELS(246), -1, PIXELS(616), 1);
  Scene_SetPositionAndCommit(21, 248, 648);
  Scene_SetPositionAndCommit(21, 248, 576);
  Task_RemoveFar(Func_02002180);
  FieldEvent_Wait(10);
  Task_RemoveFar(Func_02001c5c);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  FieldEvent_Wait(20);
  Func_02000b84(0xf80000, 0, 0x2400000);
  Sound_Play(176);
  Actor_GetById(21)->velocityY = 0x60000;
  Actor_SetSpriteColor(Actor_GetById(21), 0);
  Actor_SetSingleChildFlags(Actor_GetById(21), 1);
  Actor_MoveToAndWaitById(21, 256, 560);
  FieldEvent_Wait(20);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Sound_Play(241);
  Scene_SetSpeed(21, 0xcccc, 0x6666);
  Scene_SetPositionAndReset(21, 248, 496);
  Sound_Play(390);
  Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
  GameFlag_SetBit(0x88a);
  Event_End();
}

void Func_02000e78(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Scene_CamBounds(PIXELS(232), -1, PIXELS(328), 1);
  Scene_SetPositionAndCommit(21, 232, 328);
  FieldCamera_MoveWithinMapBounds(PIXELS(232), -1, PIXELS(200), 1);
  Scene_SetPositionAndCommit(21, 232, 280);
  Actor_MoveToAndWaitById(21, 232, 168);
  Task_RemoveFar(Func_02002180);
  Actor_ArmCallback(21, FACING_SOUTH, 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  FieldEvent_Wait(20);
  GameFlag_SetBit(0x88b);
  Event_End();
}

/* VIEW(yampi_desert): routine Func_02003fc0 as struct Actor *Func_02003fc0(s32
 * kind, s32 x, s32 y, s32 z); module/scene/gabomba_statue.h has s32
 * Func_02003fc0(s32 x0, s32 z0, s32 x1, s32 z1, s32 a, s32 b, s32 c, s32 d);
 * tried as the definition's s32 Func_02003fc0(s32 x0, s32 z0, s32 endX, s32
 * endZ, s32 length, s32 direction, s32 last, s32 bit), it does not compile in
 * tla-ja: games/tla/src/scene/yampi_desert.c:156: too few arguments to function
 * `Func_02003fc0'. */
extern struct Actor *
Func_02003fc0__yampi_desert(s32 kind, s32 x, s32 y,
                            s32 z) __asm__("Func_02003fc0");
void Actor_StartScript(struct Actor *object, const void *script);

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
          Func_02003fc0__yampi_desert(285, (x - 2) << 16, 0, (z - 2) << 16);

      if (object != NULL) {
        object->speedLimit = 0x12000;
        object->acceleration = 0x8000;
        Actor_SetSingleChildFlags((struct Actor *)object, 0);
        Import_ActorSelectAnimation((struct Actor *)object, 1);
        Actor_StartScript(object, gData_02005828);
      }
    }
  }
  MapGrid_CopyCellPictureIndicesRect(74, 71, 77, 9, 3, 3);
}

#define OBJ(p) ((struct Actor *)(p))

/// The trapped scorpion uses Scoop, the leader falls with it, and the King
/// Scorpion's battle begins.
void YampiDesert_TrapKingScorpion(void) {
  struct Actor *leader = OBJ(Actor_GetById(gMainRoutineWork.currentOwner));
  struct Actor *partner = OBJ(Actor_GetById(21));
  u32 i;
  s32 angle;

  if (!GameFlag_Test(0x88b))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
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
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  FieldEvent_Wait(20);
  Scene_CamBounds(PIXELS(232), -1, PIXELS(196), 1);
  Import_ActorShowEmoteById(gMainRoutineWork.currentOwner, 0x101, 0);
  Scene_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 232, 164);
  FieldEvent_Wait(20);
  angle = 0x4000;
  Scene_Call4098(21, 232 << 16, 184 << 16, angle);
  wait(1);
  Func_02000b84(232 << 16, 0, 184 << 16);
  Sound_Play(176);
  partner->velocityY = 0x60000;
  partner->priorityFlags &= ~2;
  Actor_SetSpritePriority(21, 2);
  Actor_SetSpriteColor(Actor_GetById(21), 0);
  Actor_SetSingleChildFlags(Actor_GetById(21), 1);
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Actor_MoveToAndWaitById(21, 216, 184);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, angle, 0);
  Actor_FaceDirectionById(21, 0);
  FieldEvent_Wait(20);
  Func_02002ca0();
  Sound_Play(136);
  Func_02000f24();
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  FieldEvent_Wait(20);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x102);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(10);
  Sound_Play(204);
  i = 0;
step_13:
  leader->scaleX += -0x400;
  leader->scaleY += -0x400;
  leader->y += -0x20000;
  wait(1);
  i++;
  if (i <= 13)
    goto step_13;
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Sound_Play(153);
  partner->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(21), 0);
  Actor_MoveToAndWaitById(21, 232, 168);
  FieldEvent_Wait(3);
  Sound_Play(204);
  i = 0;
step_5:
  partner->scaleX += -0x800;
  partner->scaleY += -0x800;
  partner->y += -0x70000;
  wait(1);
  i++;
  if (i <= 5)
    goto step_5;
  Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004138(4);
}

/// Scoop on the sand pit between the four rocks (flag 0x8ed).
void YampiDesert_ScoopCenterPit(s32 mode) {
  struct Actor *object;
  s32 height;

  if (mode == 1) {
    object = Actor_GetById(9);
    MapGrid_CopyCellPictureIndicesRect(35, 39, 13, 11, 3, 3);
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
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,      \
                                           x_ << 16, z_ << 16);                \
  }

void Func_020012dc(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(64);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (object != NULL)
    Scene_Call4090(64, 154 << 18, 222 << 17);
  MapGrid_CopyCellPictureIndicesRect(82, 82, 15, 40, 5, 6);
  Scene_Call4000(82, 82, 5, 6, 15, 40);
  MapGrid_CopyCellPictureIndicesRect(88, 82, 36, 25, 5, 5);
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

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    struct MainRoutineWork *work = &gMainRoutineWork;                          \
    s32 x = Func_020012d4(column);                                             \
    s32 z = Func_020012d4(row);                                                \
    Actor_SetHorizontalPositionWithTerrain(work->currentOwner, x << 16,        \
                                           z << 16);                           \
  }

void Func_020013f8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (Actor_GetById(64) != NULL)
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  MapGrid_CopyCellPictureIndicesRect(82, 89, 15, 40, 5, 6);
  Scene_Call4000(82, 89, 5, 6, 15, 40);
  MapGrid_CopyCellPictureIndicesRect(88, 89, 36, 25, 5, 5);
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
  struct Actor *object = Actor_GetById(64);
  struct Actor *leader;

  if (object != 0) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    Scene_Call4090(64, PIXELS(288), PIXELS(240));
    if ((leader->x + 0x80000) >> 20 == (object->x + 0x80000) >> 20 &&
        (leader->z + 0x80000) >> 20 == (object->z + 0x80000) >> 20)
      Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                             PIXELS(288), PIXELS(256));
  }
}

void Func_0200157c(void) {
  if (Actor_GetById(64))
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
}

void Func_02001594(void) {
  MapGrid_CopyCellPictureIndicesRect(64, 64, 72, 32, 1, 1);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 32);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 96);
}

/// Scoop on the sand pit that hides the ladder (flag 0x88d).
void YampiDesert_ScoopLadderPit(s32 mode) {
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

DEFINE_ACTORS_SET_HIDDEN(Func_02001650)

s32 Actor_Jump();
s32 Motion_SetModeAndWaitAnimation();

s32 Actor_SetMovementTypeById();

s32 Actor_SetMovementTypeAndWaitById();

extern u8 gData_0200468c[];
extern u8 gData_020046dc[];
extern u8 gData_02004718[];

/// ★ Named from the game: "Hey! Look! Up there!" (MsgHeyLookUp)
void YampiDesert_SeeTravelersAbove(s32 p0) {
  s32 *owner;
  s32 angle;

  Call1((void (*)())Event_Start, p0);
  Call1((void (*)())Event_ResolvePendingActions, 0);
  Call1((void (*)())Event_SetMessageId, (s32)MsgHeyLookUp);
  Call1((void (*)())Actor_Stop, 9);
  Call1((void (*)())wait, 1);
  Value3((s32 (*)())Import_ActorShowEmoteById, 9, 0x100, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0, 20);
  Call2((void (*)())Event_ShowMessageAndWait, 9, 0);
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
  Call3((void (*)())Import_ActorShowEmoteById, *owner, 0x100, 0);
  Call3((void (*)())Actor_ArmCallback, *owner, 0x8000, 0);
  ((struct Actor *)Import_FieldCameraGetActor())->motionFlags = 0;
  Call2((void (*)())Camera_SetSpeedParameters, 0x19999, 0x3333);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, 0xd40000, 0, 0xd40000, 1);
  FieldCamera_WaitForArrival();
  Call2((void (*)())Event_ShowMessageAndWait, 12, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0, 10);
  Call2((void (*)())Event_ShowMessageAndWait, 11, 0);
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
  Call2((void (*)())Camera_SetSpeedParameters, 0xcccc, 0x1999);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, 0xf00000, -1, 0x1040000, 1);
  FieldCamera_WaitForArrival();
  Call1((void (*)())Actor_Stop, 8);
  Call1((void (*)())Actor_Stop, 9);
  Call1((void (*)())Actor_Stop, 10);
  Call1((void (*)())Actor_Stop, 11);
  Call1((void (*)())Actor_Stop, 12);
  Call1((void (*)())Actor_Stop, 13);
  Call2((void (*)())Camera_SetSpeedParameters, 0x26666, 0x4ccc);
  Call4((void (*)())FieldCamera_MoveWithinMapBounds, 0xd40000, -1, 0xd40000, 1);
  FieldCamera_WaitForArrival();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 8, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 20);
  Call2((void (*)())Event_ShowMessageAndWait, 9, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 3);
  Call2((void (*)())Event_ShowMessageAndWait, 8, 0);
  Call3((void (*)())Actor_Jump, 10, 4, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call2((void (*)())Actor_SetModeById, 10, 4);
  Call2((void (*)())Event_ShowMessageAndWait, 10, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x8000, 0);
  Value3((s32 (*)())Import_ActorShowEmoteById, 8, 0x102, 0);
  Import_ActorShowEmoteById(9, 0x102, 80);
  Call2((void (*)())Event_ShowMessageAndWait, 14, 0);
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
    Call2((void (*)())Camera_SetSpeedParameters, 0x6666, 0xccc);
    Call4((void (*)())FieldCamera_MoveWithinMapBounds, 0xf00000, -1, 0x1180000,
          1);
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
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 8, 0, 0);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 12, 0, 0);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 13, 0, 0);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 14, 0, 0);
  angle = 0xb000;
  Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 8,
        0x2080000, 0xf20000, angle);
  {
    s32 facing = 0x8000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 9,
          0x2150000, 0x10f0000, facing);
  }
  {
    s32 facing = 0x3000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 10,
          0x1b30000, 0xe80000, facing);
  }
  {
    s32 facing = 0x5000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 11,
          0x1d30000, 0xf80000, facing);
  }
  {
    s32 facing = 0xd000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 12,
          0x1ce0000, 0x1540000, facing);
    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 13,
          0x1e20000, 0x1540000, facing);
  }
  Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 14,
        0x1e90000, 0x1400000, angle);
  Call2((void (*)())Actor_SetModeById, 8, 1);
  Call2((void (*)())Actor_SetModeById, 9, 1);
  Call2((void (*)())Actor_SetModeById, 10, 1);
  Call2((void (*)())Actor_SetModeById, 11, 1);
  Call2((void (*)())Actor_SetModeById, 12, 1);
  Call2((void (*)())Actor_SetModeById, 13, 1);
  Call2((void (*)())Actor_SetModeById, 14, 1);
  Call1((void (*)())GameFlag_SetBit, 0x8f0);
  Call1((void (*)())GameFlag_SetBit, 0x88f);
  Event_End();
}

/// The Scoop Gem (item 209's icon) as an actor, after the King Scorpion's
/// battle (flag 0x88c).
/// ★ Named from the game: "Scoop Gem" (item 209's name)
struct Actor *YampiDesert_CreateScoopGem(void) {
  struct Actor *object =
      Func_02003fc0__yampi_desert(489, 232 << 16, 32 << 16, 200 << 16);
  u8 *buffer = 0;

  struct YampiDesertVisual *visual;
  s32 mask;

  if (object == 0)
    return 0;
  {
    visual = object->sprite;

    {
      s32 keep = 33;

      keep = -keep;
      visual->flags05 &= keep;
    }
    mask = visual->flags09 & 0x0f;
    {
      s32 clear = 13;

      clear = -clear;
      mask &= clear;
    }
    mask |= 4;
    visual->flags09 = mask;
    object->motionFlags = 0;
    object->unk5c[0] = 1;
    visual->value1a = 0;
    visual->value1b = 0;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(209);
    VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  return object;
}

void YampiDesert_DropScoopGem(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  object = YampiDesert_CreateScoopGem();
  PartyInventory_GiveItem(209, 0);
  if (object != 0) {
    Actor_SetPositionAndResetMotion((struct Actor *)object, 0, 0, 0);
    wait(1);
    Actor_Release(object);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  GameFlag_SetBit(0x88c);
  Event_End();
}

void Func_02001c5c(void) {
  struct Actor *object;
  s16 scene;

  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->unk16a = gData_02004808;
  if (gData_0200480c != 0) {
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Ov661Room4)
      object = Actor_GetById(16);
    else if (scene == (s32)&SceneId_Ov661Room5)
      object = Actor_GetById(21);
    else
      return;
    gData_0200480c->x = object->x;
    gData_0200480c->z = object->z;
    gData_0200480c->facing = object->facing;
  }
}
#undef OBJ
#undef MOVE_LEADER_TO

// ---- games/tla/src/scene/yampi_desert/s1cc4.c ----
// Scene code of overlay 0x661, in ROM order, from s1cc4's offset up to the next
// object the overlay links.

#include "input.h"
#include "frame.h"
#include "field.h"

#define FIELD32(object, offset) (*(s32 *)((u8 *)(object) + (offset)))

struct Actor *Actor_GetById(u32 objectId);
void Actor_StartScript(struct Actor *object, const void *script);
s32 Math_Sine(s32 angle);

void Func_02001cc4(struct Actor *object, s32 dx, s32 dz) {
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  if (gMainRoutineWork.renderMode != 5) {
    u32 odd = gFrameCount & 1;

    if (odd == 0) {
      struct Actor *dust =
          (struct Actor *)Value4((s32 (*)())Func_02003fc0__yampi_desert, 285,
                                 object->x, object->y, object->z);

      if (dust != NULL) {
        struct DustSprite *sprite;

        dust->motionFlags = odd;
        dust->collisionFlags = odd;
        Actor_SetSingleChildFlags(dust, 0);
        Import_ActorSelectAnimation(dust, 1);
        Actor_StartScript(dust, gData_020042f0);
        sprite = dust->sprite;
        sprite->mode = 1;
        /* FAKEMATCH(dead): an empty once-only block after the sprite store
         * keeps gInput's address load after it; as the store alone, 2 of 174
         * instructions differ in tla-en. */
        do {
        } while (0);
        if (gInput.held != 0) {
          FIELD32(dust, 68) =
              Math_Cosine(((Random16Far() << 3) >> 16) << 14) + dx;
          FIELD32(dust, 76) =
              Math_Sine(((Random16Far() << 3) >> 16) << 14) + dz;
          if (gInput.held & 2) {
            FIELD32(dust, 68) += dx;
            FIELD32(dust, 76) += dz;
          }
        } else {
          FIELD32(dust, 68) =
              (Math_Cosine(((Random16Far() << 3) >> 16) << 14) >> 1) + dx;
          FIELD32(dust, 76) =
              (Math_Sine(((Random16Far() << 3) >> 16) << 14) >> 1) + dz;
        }
        FIELD32(dust, 72) = 0;
        dust->timer = ((Random16Far() << 9) >> 16) - 256;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  }
  if (dx != 0) {
    object->x += dx;
    if (!(gInput.held & 240)) {
      s32 z = object->z >> 16;

      object->z += ((8 - z % 16) << 16) >> 3;
    }
  } else {
    object->z += dz;
    if (!(gInput.held & 240)) {
      s32 x = object->x >> 16;

      object->x += ((8 - x % 16) << 16) >> 3;
    }
  }
}

#include "ram_buffer.h"
#include "map.h"
#include "task.h"
#include "event_work.h"
#include "fixed_math.h"
#include "version.h"

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

void Func_02001e58(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 speed = 0x18000;
  struct MapWork *map;
  struct MapCell *here, *ahead, *under;
  s32 index, column, row;
  struct MapCell *cells;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0) {
    Actor_SetAnimationSpeed(leader, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  map = Ram_HeapSlots->mapWork;
  index = Func_020041e0(leader);
  column = leader->x / 0x100000;
  row = (leader->z + 0x40000) / 0x100000;
  cells = map->layers[0].cells;
  here = cells + index;
  ahead = cells + (column + row * 128);
  under = map->layers[1].cells + index;
  Actor_SetAnimationSpeed(leader, 16);
  TilesetAnim_ResumeAll();
  if ((u8)(under->collisionCode - 10) <= 4) {
    if (gMainRoutineWork.renderMode == 5)
      speed = 0;
    else
      Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                1);
  }
  if (ahead->collisionCode == 255)
    speed = 0;
  switch (here->collisionCode) {
  case 10:
    if (gInput.held & 64) {
      Func_02001cc4(leader, 0, -speed >> 2);
      return;
    }
    break;
  case 11:
    if (gInput.held & 128) {
      Func_02001cc4(leader, 0, speed >> 2);
      return;
    }
    break;
  case 12:
    if (gInput.held & 32) {
      Func_02001cc4(leader, -speed >> 2, 0);
      return;
    }
    break;
  case 13:
    if (gInput.held & 16) {
      Func_02001cc4(leader, speed >> 2, 0);
      return;
    }
    break;
  }
  switch (under->collisionCode) {
  case 10:
    Func_02001cc4(leader, 0, -speed);
    break;
  case 11:
    Func_02001cc4(leader, 0, speed);
    break;
  case 12:
    Func_02001cc4(leader, -speed, 0);
    break;
  case 13:
    Func_02001cc4(leader, speed, 0);
    break;
  }
}

void Func_02002010(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x;
  s32 y;
  s32 z;

  y = leader->y;
  if (y > 0x100000)
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3) {
    Actor_SetSpritePriority(18,
                            ((struct FieldSprite *)leader->sprite)->priority);
    x = leader->x;
    if (x > 0x1d80000) {
      z = leader->z;
      if (z <= 0x1e80000)
        return;
      if (leader->y > (s32)0xfff00000)
        gMainRoutineWork.unk2c3 = 1;
      else
        gMainRoutineWork.unk2c3 = 0;
    }
  }
}

void Func_020020a8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->y > -0x100000)
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
}

void Func_020020e4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 index = Func_020041e0(leader);
  struct MapCell *cell = map->layers[0].cells + index;
  struct MainRoutineWork *party;
  s32 y;

  if (cell->collisionCode == 20) {
    y = leader->y;
    if (y > 0x100000) {
      gMainRoutineWork.unk2c3 = 1;
      return;
    }
  }
  party = &gMainRoutineWork;
  party->unk2c3 = 0;
}

void Func_02002140(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 x = leader->x >> 20;
  u32 z = leader->z >> 20;

  if (x > 12 && z > 10 && x <= 15 && z <= 13)
    GameFlag_SetBit(263);
  else
    GameFlag_ClearBit(263);
}

void Func_02002180(void) {
  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 358) == 0 &&
      gFrameCount % 5 == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode != 5 || gInput.held == 0)
      Sound_Play(596);
  }
}

void Func_020021d4(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->unk16a = 0;
  gData_02004808 = 0;
}

s32 Func_020021f0(struct Actor *object) {
  s32 x = object->targetX;

  if (x == ACTOR_NO_TARGET && object->targetZ == x)
    return 1;
  return 0;
}

void Func_02002208(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
  case 0:
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, 140 << 17, 0, 200 << 16);
    *state = 1;
    break;
  case 1:
    if (Func_020021f0(object))
      *state = 2;
    break;
  case 2:
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, 140 << 17, 0, 132 << 17);
    *state = 3;
    break;
  case 3:
    if (Func_020021f0(object))
      *state = 4;
    break;
  case 4:
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, 184 << 16, 0, 132 << 17);
    *state = 5;
    break;
  case 5:
    if (Func_020021f0(object))
      *state = 6;
    break;
  case 6:
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, 184 << 16, 0, 248 << 16);
    *state = 7;
    break;
  case 7:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x203)) {
        object->update = NULL;
        gData_02004808 = 200;
      } else {
        *state = 8;
      }
    }
    break;
  case 8:
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, 184 << 16, 0, 200 << 16);
    *state = 9;
    break;
  case 9:
    if (Func_020021f0(object))
      *state = 0;
    break;
  }
}

/// Readies actors 16 and 17 and the room's callbacks; actor 17 becomes the
/// followed actor (gData_0200480c).
void Func_02002318(void) {
  struct Actor *leader = Actor_GetById(16);
  struct Actor *object;
  struct Actor **followed;

  Scene_TaskAdd(Func_02002180, 0x480);
  followed = &gData_0200480c;
  object = Actor_GetById(17);
  object->priorityFlags |= 2;
  *followed = (struct Actor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Func_020040f8(17);
  (*followed)->motionFlags = 3;
  Actor_SetSpritePriority(17, 3);
  Scene_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(16, 3);
  Func_020040f8(16);
  Actor_GetById(16)->motionFlags = 3;
  if (!GameFlag_Test(0x109)) {
    *((u8 *)leader + 98) = 0;
    leader->update = Func_02002208;
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, TASK_PASS_VBLANK);
}

/// Sends the object toward a point and moves to the next state.
#define WALK(state_, x, z)                                                     \
  case state_:                                                                 \
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, x, 0, z);                    \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the object to arrive before moving to the given state.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (Func_020021f0(object))                                                 \
      *state = next;                                                           \
    break;

void Func_020023cc(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 166 << 18)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 182 << 18)
    ARRIVE(3, 4)
    WALK(4, 246 << 16, 182 << 18)
  case 5:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x200)) {
        object->update = NULL;
        gData_02004808 = 201;
      } else {
        *state = 6;
      }
    }
    break;
    WALK(6, 184 << 16, 182 << 18)
    ARRIVE(7, 8)
    WALK(8, 184 << 16, 178 << 18)
    ARRIVE(9, 10)
    WALK(10, 140 << 17, 178 << 18)
    ARRIVE(11, 12)
    WALK(12, 140 << 17, 190 << 18)
    ARRIVE(13, 14)
    WALK(14, 232 << 16, 190 << 18)
    ARRIVE(15, 16)
    WALK(16, 232 << 16, 186 << 18)
  case 17:
    if (Func_020021f0(object)) {
      if (!GameFlag_Test(0x200))
        *state = 18;
      else
        *state = 32;
    }
    break;
    WALK(18, 232 << 16, 166 << 18)
    ARRIVE(19, 0)
  case 32:
    Func_020021d4();
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, 148 << 17, 0, 186 << 18);
    *state = 33;
    break;
    ARRIVE(33, 34)
    WALK(34, 148 << 17, 190 << 18)
    ARRIVE(35, 36)
    WALK(36, 232 << 16, 190 << 18)
    ARRIVE(37, 38)
    WALK(38, 232 << 16, 186 << 18)
    ARRIVE(39, 32)
  }
}

/// Func_02002318's counterpart for actors 21 and 22: actor 22 becomes the
/// followed actor, and until flag 0x109 actor 21 is placed and given the
/// update Func_020023cc.
void Func_020026a0(void) {
  struct Actor *leader = Actor_GetById(21);
  struct Actor *object;
  struct Actor **followed;

  Scene_TaskAdd(Func_02002180, 0x480);
  followed = &gData_0200480c;
  object = Actor_GetById(22);
  object->priorityFlags |= 2;
  *followed = (struct Actor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Func_020040f8(22);
  (*followed)->motionFlags = 3;
  Actor_SetSpritePriority(22, 3);
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(21, 3);
  Func_020040f8(21);
  Actor_GetById(21)->motionFlags = 3;
  if (!GameFlag_Test(0x109)) {
    Scene_Call4098(21, 232 << 16, 664 << 16, 0);
    *((u8 *)leader + 98) = 0;
    leader->update = Func_020023cc;
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, TASK_PASS_VBLANK);
}

/// Sends the object toward a point and moves to the next state.

/// As WALK, after Func_020021d4.
#define WALK_ON(state_, x, z)                                                  \
  case state_:                                                                 \
    Func_020021d4();                                                           \
    Call4((void (*)())Actor_SetMoveTarget, (s32)object, x, 0, z);                    \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the object to arrive before moving to the given state.

/// Waits for the object to arrive, then turns off by a flag.
#define ARRIVE_OR(state_, flag, turn, next)                                    \
  case state_:                                                                 \
    if (Func_020021f0(object)) {                                               \
      if (GameFlag_Test(flag))                                                 \
        *state = turn;                                                         \
      else                                                                     \
        *state = next;                                                         \
    }                                                                          \
    break;

void Func_02002764(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 132 << 17)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 172 << 17)
    ARRIVE(3, 4)
    WALK(4, 148 << 17, 172 << 17)
    ARRIVE_OR(5, 0x202, 32, 6)
    WALK(6, 168 << 16, 172 << 17)
    ARRIVE(7, 8)
    WALK(8, 168 << 16, 148 << 17)
    ARRIVE(9, 10)
    WALK(10, 184 << 16, 148 << 17)
    ARRIVE_OR(11, 0x201, 48, 12)
    WALK(12, 140 << 17, 148 << 17)
    ARRIVE(13, 14)
    WALK(14, 140 << 17, 164 << 17)
  case 15:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x202)) {
        object->update = NULL;
        gData_02004808 = 202;
      } else {
        *state = 16;
      }
    }
    break;
    WALK(16, 140 << 17, 180 << 17)
    ARRIVE(17, 18)
    WALK(18, 200 << 16, 180 << 17)
    ARRIVE(19, 20)
    WALK(20, 200 << 16, 156 << 17)
    ARRIVE_OR(21, 0x201, 34, 22)
    WALK(22, 200 << 16, 132 << 17)
    ARRIVE(23, 0)
    WALK_ON(32, 148 << 17, 156 << 17)
    ARRIVE(33, 34)
    WALK_ON(34, 164 << 17, 156 << 17)
    ARRIVE(35, 36)
    WALK(36, 164 << 17, 188 << 17)
    ARRIVE(37, 38)
    WALK(38, 148 << 17, 188 << 17)
    ARRIVE(39, 32)
    WALK_ON(48, 184 << 16, 172 << 17)
    ARRIVE(49, 50)
    WALK(50, 168 << 16, 172 << 17)
    ARRIVE(51, 52)
    WALK(52, 168 << 16, 148 << 17)
    ARRIVE(53, 54)
    WALK(54, 184 << 16, 148 << 17)
    ARRIVE(55, 48)
  }
}

/// Func_02002318's counterpart for actors 21 and 22 in Room 5: after flag
/// 0x88b actor 21 is placed facing 0x4000; otherwise, until flag 0x109, it
/// is placed and given the update Func_02002764.
void Func_02002b84(void) {
  struct Actor *leader = Actor_GetById(21);
  struct Actor *object;
  struct Actor **followed;
  s32 angle;

  followed = &gData_0200480c;
  object = Actor_GetById(22);
  object->priorityFlags |= 2;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  *followed = (struct Actor *)object;
  Func_020040f8(22);
  Actor_SetSpritePriority(22, 3);
  (*followed)->motionFlags = 3;
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(21, 3);
  Func_020040f8(21);
  Actor_GetById(21)->motionFlags = 3;
  if (GameFlag_Test(0x88b)) {
    angle = 0x4000;
    Scene_Call4098(21, 232 << 16, 168 << 16, angle);
  } else {
    Scene_TaskAdd(Func_02002180, 0x480);
    if (!GameFlag_Test(0x109)) {
      Scene_Call4098(21, 200 << 16, 264 << 16, 0);
      *((u8 *)leader + 98) = 0;
      leader->update = Func_02002764;
    }
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, TASK_PASS_VBLANK);
}

void Func_02002c6c(void) {
  if (*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 0x7c4) != 0)
    MapGrid_CopyCellPictureIndicesRect(73, 76, 78, 11, 1, 1);
}

void Func_02002ca0(void) {
  BattleEffect_LoadActionEffectResources(136, 1);
  Scene_TaskAdd(Func_02002c6c, 0x480);
  FieldPsynergy_SetupActorPair(21, -1);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_RemoveFar(Func_02002c6c);
  BattleEffect_CleanupSceneObjects();
}

DEFINE_ACTOR_HIDE(Func_02002ce0, Func_020040f8)

DEFINE_SCENE_RUN_UNLESS_FLAG(Func_02002d10, 0x20d, Func_0200163c)

void Func_02002d40(void) {
  Func_02002ce0(11);
  Func_02002ce0(12);
  Func_02002d10();
}

DEFINE_ACTORS_FOR_RANGE(Func_02002d54, 6, Func_02002ce0, 9, Func_02002d10)

void Func_02002d6c(struct Actor *object, s32 x, s32 z, s32 mode) {
  struct MainRoutineWork *party;
  s32 zero;
  u8 *flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  flags = &object->motionFlags;
  zero = 0;
  party = &gMainRoutineWork;
  *flags = zero;
  Actor_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWaitById(party->currentOwner, x, z);
  Actor_FaceDirectionById(party->currentOwner, 0xc000);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  Actor_SetModeById(party->currentOwner, 13);
  Actor_SetMoveTarget(object, object->x, object->y - 0xa0000, object->z);
  Actor_WaitForMove(object);
  Actor_SetModeById(party->currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004138(mode);
}

void Func_02002e14(void) {
  struct Actor *object;
  s32 x;
  s32 z;

  if (GameFlag_Test(0x88d)) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 8) {
      if (z == 31 && !(gInput.held & 0x80))
        return;
      if (z == 33 && !(gInput.held & 0x40))
        return;
    } else {
      if (z != 32)
        return;
      if (x == 7 && !(gInput.held & 0x10))
        return;
      if (x == 9 && !(gInput.held & 0x20))
        return;
    }
    Func_02002d6c(object, 136, 516, 5);
  }
}

void Func_02002e98(void) {
  Func_02002d6c(Actor_GetById(gMainRoutineWork.currentOwner), 408, 432, 2);
}

void Event_SetAfterBattleDestination(s32 scene, s32 entrance);
void Func_02000b84(s32 x, s32 y, s32 z);

void Func_02002ebc(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *leader = Actor_GetById(party->currentOwner);
  struct Actor *actor8 = Actor_GetById(8);
  /* FAKEMATCH(wrapper): the camera, walk and speed calls forwarded through
   * call.h's Call3 and Call4; as direct calls, 28 of 187 instructions differ
   * in tla-en. */

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(78);
  Call4(FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call4(FieldCamera_MoveWithinMapBounds, PIXELS(232), -1, PIXELS(184), 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  leader->motionFlags = 0;
  leader->y = PIXELS(130);
  leader->gravity = Q16_ONE / 2;
  leader->bounce = 0;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  {
    // The angle is held in a local; as a literal argument, its constant is
    // built last.
    s32 angle = TRIG_TURN / 4;

    Call4(Actor_SetHorizontalPositionAndAngleWithTerrain, 8, PIXELS(232),
          PIXELS(148), angle);
  }
  actor8->motionFlags = 0;
  actor8->y = PIXELS(130);
  actor8->gravity = Q16_ONE / 2;
  actor8->bounce = 0;
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x203;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 1);
  FieldEvent_Wait(24);
  Func_02000b84(leader->x, leader->y, leader->z);
  Sound_Play(188);
  FieldEffect_SetAttached(party->currentOwner, 0x101);
  Motion_SetModeAndWaitAnimation(party->currentOwner, 49);
  Sound_Play(204);
  actor8->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  FieldEvent_Wait(24);
  Func_02000b84(actor8->x, actor8->y, actor8->z);
  Sound_Play(188);
  Actor_ArmCallback(party->currentOwner, TRIG_TURN * 3 / 4, 0);
  FieldEvent_Wait(40);
  Sound_Play(147);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(20);
  actor8->velocityY = Q16_ONE * 8;
  actor8->gravity = 0xe666;
  Call3(Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Actor_MoveToAndWaitById(8, 232, 184);
  party->battleRule24b = 2;
  Event_SetAfterBattleDestination((s32)&SceneId_Ov661Room10, 2);
  Field_RequestBattleFromEncounterTable(10, 0);
}

/// Clears the followed actor and runs the entered room's setup, as overlay
/// 654's Func_02002ec4 does; Room7 has none.
s32 Scene_Initialize(void) {
  s16 scene;

  Func_020021d4();
  gData_0200480c = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room1)
    Func_0200316c();
  else if (scene == (s32)&SceneId_Ov661Room2)
    Func_020031b4();
  else if (scene == (s32)&SceneId_Ov661Room3)
    Func_020032f4();
  else if (scene == (s32)&SceneId_Ov661Room4)
    Func_02003364();
  else if (scene == (s32)&SceneId_Ov661Room5)
    Func_02003404();
  else if (scene == (s32)&SceneId_Ov661Room6)
    Func_020035b8();
  else if (scene == (s32)&SceneId_Ov661Room8)
    Func_020035d8();
  else if (scene == (s32)&SceneId_Ov661Room9)
    Func_020036d0();
  else if (scene == (s32)&SceneId_Ov661Room10)
    Func_0200371c();
  else if (scene == (s32)&SceneId_Ov661Room11)
    Func_020037d4();
  else if (scene == (s32)&SceneId_Ov661Room12)
    Func_020037e8();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200316c(void) {
  Func_02003820(gData_02004768);
  Func_02004180(0);
  if (gMainRoutineWork.entrance == 1) {
    GameFlag_SetBit(0xa29);
    GameFlag_ClearBit(0xa2a);
    GameFlag_ClearBit(0xa2b);
  }
}

/// Clears actors 8 to 14 after flag 0x8b3, or places them after flag 0x8f0.
void Func_020031b4(void) {
  s32 angleA;
  s32 angleB;
  s32 angle;

  if (GameFlag_Test(0x8b3)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  } else if (GameFlag_Test(0x8f0)) {
    // The angles go through locals, as overlay 695 writes them (with
    // every angle a constant, differ 38; with the single-use ones
    // constants, they are built after the positions, differ 12).
    angleA = 0xb000;
    Scene_Call4098(8, 520 << 16, 242 << 16, angleA);
    angle = 0x8000;
    Scene_Call4098(9, 533 << 16, 271 << 16, angle);
    angle = 0x3000;
    Scene_Call4098(10, 435 << 16, 232 << 16, angle);
    angle = 0x5000;
    Scene_Call4098(11, 467 << 16, 248 << 16, angle);
    angleB = 0xd000;
    Scene_Call4098(12, 462 << 16, 340 << 16, angleB);
    Scene_Call4098(13, 482 << 16, 340 << 16, angleB);
    Scene_Call4098(14, 489 << 16, 320 << 16, angleA);
  }
  if (GameFlag_Test(0x88d))
    Func_02001594();
  Task_Add(Func_02002010, TASK_PASS_VBLANK);
}

void Func_020032c4(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.currentOwner)
           ->sprite)
          ->priority;

  Actor_SetSpritePriority(13, variant);
  Actor_SetSpritePriority(14, variant);
}

void Func_020032f4(void) {
  Func_02003820(gData_0200478e);
  if (GameFlag_Test(0x20b)) {
    Func_020006ac();
    wait(10);
  }
  Scene_TaskAdd(Func_02002010, 0x480);
  Scene_TaskAdd(Func_020032c4, 0x480);
}

void Func_02003338(s32 id) {
  Actor_GetById(id)->priorityFlags |= 2;
  Actor_SetSpritePriority(id, 3);
  Func_020040f8(id);
  Actor_SetModeById(id, 2);
}

void Func_02003364(void) {
  Func_02004180(0);
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4) {
    if (Scene_FlagTest(0x889))
      Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
    else
      Func_02002318();
  }
  Func_02003820(gData_020047d2);
  if (Scene_FlagTest(0x20b)) {
    Func_020006ac();
    wait(10);
  }
  Func_02003338(12);
  Func_02003338(13);
  Func_02003338(14);
  Func_02003338(15);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    Task_Add(Func_020020a8, TASK_PASS_VBLANK);
}

/// Room 5's setup: from entrances other than 1 and 2 it places actors 11 to
/// 20 and copies the map cells flags 0x88c and 0x8ed select.
void Func_02003404(void) {
  Func_02004180(0);
  Func_02003820(gData_020047ec);
  Func_02003338(11);
  Func_02003338(12);
  Func_02003338(13);
  Func_02003338(14);
  Func_02003338(15);
  Func_02003338(16);
  Func_02003338(17);
  Func_02003338(18);
  Func_02003338(19);
  Func_02003338(20);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    if (GameFlag_Test(0x889) && !GameFlag_Test(0x88a))
      Func_020026a0();
  } else {
    Scene_Call4090(11, 200 << 16, 248 << 16);
    Scene_Call4090(12, 168 << 16, 280 << 16);
    Scene_Call4090(13, 152 << 16, 344 << 16);
    Scene_Call4090(14, 184 << 16, 360 << 16);
    Scene_Call4090(15, 328 << 16, 264 << 16);
    Scene_Call4090(16, 296 << 16, 296 << 16);
    Scene_Call4090(17, 344 << 16, 312 << 16);
    Scene_Call4090(18, 312 << 16, 360 << 16);
    Scene_Call4090(19, 280 << 16, 376 << 16);
    Actor_SetHorizontalPositionWithTerrain(20, 328 << 16, 392 << 16);
    if (GameFlag_Test(0x88c)) {
      if (GameFlag_Test(0x8ed)) {
        MapGrid_CopyCellPictureIndicesRect(68, 77, 13, 9, 3, 3);
        Scene_Call4000(68, 77, 3, 3, 13, 9);
      } else {
        MapGrid_CopyCellPictureIndicesRect(74, 71, 77, 9, 3, 3);
        Scene_Call4000(74, 71, 3, 3, 13, 9);
      }
      Scene_Call4000(63, 77, 3, 3, 13, 73);
    } else if (GameFlag_Test(0x88a)) {
      Func_02002b84();
    }
  }
}

/// A halfword of gMainRoutineWork that has no field name yet.
#define MAIN_ROUTINE_WORK_U16(offset)                                          \
  (*(u16 *)((u8 *)&gMainRoutineWork + (offset)))

void Func_020035b8(void) {
  Func_02004180(0);
  if (GameFlag_Test(0x302) == 0)
    Actor_SetWideSprite(64, 0);
}

static inline void Scene_StoreU16(u16 *field, s32 value) {
  /* FAKEMATCH(wrapper): through the inline the value is a word constant built
   * after the field's address (movs), where stored directly it loads from the
   * literal pool; as the halfword constant stored directly, 24 of 100
   * instructions of Func_020035d8 differ in tla-en. */
  *field = value;
}

/// Room 8's setup. Entered at entrance 4 (not in the Japanese edition), it
/// sets gMainRoutineWork's pair at 0x260 to the room whose flag is set.
void Func_020035d8(void) {
  struct Actor *object = Actor_GetById(64);
  s32 scene;

  Scene_QueueMusic(137);
  Func_020041d0();
  Scene_Call41d8(0, 524, 8, 9);
  if (object != NULL)
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  object = Actor_GetById(10);
  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->radius = 8;
  Func_02002d40();
  Task_Add(Func_02001e58, TASK_PASS_VBLANK);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_ClearBit(0xa29);
    GameFlag_SetBit(0xa2a);
    GameFlag_ClearBit(0xa2b);
  }
#if TLA_PATCH_1
  else if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Test(0xa29)) {
      scene = (s32)&SceneId_Ov661Room1;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 1);
    } else if (GameFlag_Test(0xa2a)) {
      scene = (s32)&SceneId_Ov661Room8;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 2);
    } else if (GameFlag_Test(0xa2b)) {
      scene = (s32)&SceneId_Ov661Room9;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 2);
    }
  }
#endif
}

void Func_020036d0(void) {
  if (Actor_GetById(64) != 0)
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_ClearBit(0xa29);
    GameFlag_ClearBit(0xa2a);
    GameFlag_SetBit(0xa2b);
  }
}

struct RideEntry *Func_02003e3c(s32 slot, s32 id, s32 timer, s32 low, s32 high);

/// Room 10's setup.
void Func_0200371c(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  GameFlag_SetBit(0x144);
  if (gMainRoutineWork.entrance == 1 && !GameFlag_Test(0x88c)) {
    Func_02002ebc();
  } else {
    if (gMainRoutineWork.entrance == 2 && !GameFlag_Test(0x88c))
      YampiDesert_DropScoopGem();
  }
  if (GameFlag_Test(0x8ed)) {
    MapGrid_CopyCellPictureIndicesRect(35, 39, 13, 11, 3, 3);
    Scene_Call4090(9, 232 << 16, 200 << 16);
  }
  Func_02003da0(0, 240);
  Func_02003e3c(0, 9, 0, 0, 64);
  wait(10);
}

void Func_020037d4(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
}

void Func_020037e8(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Scene_QueueMusic(137);
  Func_02003820(gData_02004800);
  Func_02002d54();
  Scene_TaskAdd(Func_02001e58, 0x480);
}

void Func_02003820(const void *script) {
  const u16 *entry;

  if (*(const u16 *)script == 0xffff)
    return;
  entry = script;
loop:
  {
    s32 id = entry[0];
    struct Actor *object = Actor_GetById(id);
    s32 kind = (s16)entry[1];
    s32 flag = (s16)entry[2];

    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_02003910(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}
#undef FIELD32
#undef WALK
#undef ARRIVE
#undef WALK_ON
#undef ARRIVE_OR
#undef MAIN_ROUTINE_WORK_U16

// Finds the current target's table row and shrinks it.
DEFINE_STATUE_SINK(Func_0200388c, Func_02003910)

// ---- games/tla/src/scene/yampi_desert/s3910.c ----
// Scene code of overlay 0x661, in ROM order, from s3910's offset up to the next
// object the overlay links, and the overlay's data.

#include "iwram_call.h"

DEFINE_ACTOR_SET_CELL_OPEN(Func_02003910, Func_020041e0, Import_ActorSelectAnimation)

/// The whole-pixel half of a 16.16 coordinate.
/* VIEW(s3910): macro PIXELS as PIXELS__s3910; another part or header has
 * another definition; tried with the definition the rest of the object has,
 * Func_020039b8 moves: 33 of 212 instructions in tla-ja. */
#define PIXELS__s3910(v) (((s16 *)&(v))[1])

/// One rising platform and what rides it.
struct RideEntry {
  s16 id;
  s16 timer;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 fallAt;
  s16 max;
  s16 min;
  s16 unk10;
  s16 step;
  s16 unk14;
  s16 phase;
  struct Actor *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  void *block;
};

s32 Math_Sine(s32 angle);

// The rising platforms callback: for each of the 16 entries it raises or lowers
// the platform on its timer, writes its height to the collision and cell maps,
// and carries its rider with a bob.
void Func_020039b8(void) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct RideEntry *entry = work->entries;
  s32 value = work->value;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 i = 0;
  s32 *collision = (s32 *)Ram_MapCollision + value;

  for (; i <= 15; collision++, entry++, i++) {
    struct Actor *object;
    struct Actor *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    object = Actor_GetById(entry->id);
    if (object->x == 0 && object->z == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Sound_Play(206);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Sound_Play(395);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Test(0x104)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = (object->x >> 20) + (object->z >> 20) * 128;
    terrain[index] = (u8)work->value + i;
    *collision = ((PIXELS__s3910(object->y) + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= 128;
    if (entry->height != 0)
      cell->attributeB |= 16;
    else
      cell->attributeB = 0;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y = object->y + -0x100000;
    } else {
      s32 wave;

      entry->phase += 0x1000;
      wave = Iwram_MulQ16(0x20000, Math_Sine(entry->phase)) + -0xe0000;
      rider->y = object->y + (entry->height << 16) + wave;
    }
    rider->x = object->x;
    rider->z = object->z;
  }
}

// Draws the rising platforms' columns as sprites, one 16-pixel piece per step
// of height and a bobbing top, up to 80 of them, each an OAM entry queued by
// Oam_AddSpriteAtDepthClamped.
void Func_02003b80(void) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct RideEntry *entry = work->entries;
  s32 i;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, originY;
  struct Actor *object;
  s32 h, x, y, sy, z, depth;
  u32 *p;

  viewX = view[0];
  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->id == 0 || entry->height == 0)
      continue;
    object = Actor_GetById(entry->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    // A goto loop: the ROM moves nothing out of it; as a for loop, 272 of 252
    // instructions differ in tla-en (the sprite's third word moves out).
    h = 0;
    if (h >= entry->height)
      goto top;
  again:
    if (count > 79)
      goto next;
    x = object->x - viewX;
    y = object->y + (h << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    x = (x >> 16) - 8;
    sy = (sy >> 16) - 16;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      goto next;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame) | 0x800;
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  next:
    h += 16;
    if (h < entry->height)
      goto again;
  top:
    if (count > 79)
      continue;
    x = object->x - viewX;
    y = object->y + (entry->height << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    x = (x >> 16) - 8;
    /* FAKEMATCH(wrapper): forwarded, so -0x20000 is built for the call and
     * again for the sum; as a direct call, 276 of 252 instructions differ in
     * tla-en. */
    sy = ((sy +
           Value2((s32 (*)())Iwram_MulQ16, -0x20000, Math_Sine(entry->phase)) +
           -0x20000) >>
          16) -
         4;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  }
}

void Func_02003da0(s32 unused, s32 value) {
  struct YampiDesertRideWork *work =
      (struct YampiDesertRideWork *)(gInventorySnapshot + 0x188);
  void *buffer;

  if (!GameFlag_Test(0x109))
    Call2((void (*)())Iwram_ClearWordsEntry, (s32)work, 456);
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 0x800);
  Decompress_TaggedLz(gField_Tiles64D, buffer);
  work->entry = VramCache_FindFreeEntry();
  VramCache_Load(work->entry, 0x800, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(0x3c0);
  work->value = value;
  Task_Add((s32)Func_020039b8, 0x48d);
  Task_Add((s32)Func_02003b80, 0x480);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

struct RideEntry *Func_02003e3c(s32 slot, s32 id, s32 timer, s32 low,
                                s32 high) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct RideEntry *entry = &work->entries[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *rider;

  Actor_GetById(id);
  Actor_SetSingleChildFlags(object, 0);
  if (!GameFlag_Test(0x109)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = 360;
    entry->max = high;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    entry->step = 0;
    entry->phase = 0;
    entry->unk14 = 0;
    object->priorityFlags |= 2;
  }
  rider = Func_02003fc0__yampi_desert(0x8000, object->x, object->y, object->z);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}

#undef PIXELS__s3910

// clang-format off
u32 gData_02004200[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_0200423c[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02004278[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020042b4[] = {
    0x9800e0, 0x9800e8, 0x9800f0, 0xa000d8, 0xa000e0, 0xa000e8,
    0xa000f0, 0xa000f8, 0xa800d8, 0xa800e0, 0xa800e8, 0xa800f0,
    0xa800f8, 0xb000d8, 0xb000f8,
};

u32 gData_020042f0[] = {
    0, 25, 22, 38, 0, 3,
    0, 1, 38,
};
// clang-format on
#undef PIXELS__s3910
