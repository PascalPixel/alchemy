/// @file
/// Scene code of overlay 0x654, in ROM order, from s1340's offset up to the next object the overlay links.

#include "shrine_of_the_sea_god.h"
#include "system.h"
#include "macros.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "frame.h"
#include "module/scene/scene.h"
#include "module/scene/shrine_of_the_sea_god.h"

extern u32 gData_02004a18[];

void Func_02001340(struct ShrineOfTheSeaGodObject *object,
                   struct ShrineOfTheSeaGodObject *other) {
  struct ShrineOfTheSeaGodSprite *sprite = object->sprite;
  u32 n;
  u32 i;

  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 258);
  Sound_Play(210);
  object->motionFlags = 0;
  for (n = 0; n <= 1; n++) {
    other->y -= 0x10000;
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
    other->y += 0x10000;
    for (i = 0; i <= 7; i++) {
      sprite->rotation -= 128;
      wait(1);
    }
    other->y -= 0x10000;
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
  }
  other->y += 0x10000;
  wait(20);
  gData_02004a18[0] = other->x - object->x;
  gData_02004a18[1] = other->y - object->y;
  gData_02004a18[2] = other->z - object->z;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
}

void Func_0200142c(struct ShrineOfTheSeaGodObject *object) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  s32 *offset = gData_02004a18;

  SET_POSITION(object, leader->x + offset[0], leader->y + offset[1],
               leader->z + offset[2]);
}

void Func_02001464(void) { Func_0200142c(Actor_GetById(14)); }

/// field_effect.h's EffectOptions, which this overlay's headers cannot include.
struct EffectOptions {
  s32 priority;
  s32 palette;
  s32 startScaleX;
  s32 startScaleY;
  s32 targetScaleX;
  s32 targetScaleY;
  s16 type;
  const s32 *script;
  u16 rotation;
  u16 spin;
  void (*update)(void *object);
};

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Effect_AdvanceMotion(union FieldObject *object);
void Map_SetScreenShake(s32 a, s32 b, s32 c);
void Map_WaitForShakeToSettle(void);
s32 Math_Sine(s32 angle);
void Task_RemoveFar(void (*func)(void));

void Func_02001474(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *stone = Actor_GetById(14);
  struct ShrineOfTheSeaGodSprite *sprite = stone->sprite;
  struct EffectOptions options;
  u32 i;
  s32 spin;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 408, 328);
  Func_02001340(leader, stone);
  ShrineOfTheSeaGod_BlendStart(0, 50, 3, 1, 24, 70);
  GameFlag_SetBit(0x230);
  wait(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(14, 3);
  stone->priorityFlags |= 2;
  Task_Add(Func_02001464, 0x480);
  do
    wait(1);
  while (*(s32 *)((u8 *)leader + 40) != 0);
  Sound_Play(227);
  Call3((void (*)())Map_SetScreenShake, 0x10000, 0x20000, 0x10000);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  for (i = 0; i <= 16;) {
    s32 velocity[3];

    velocity[0] = Math_Cosine(i << 12);
    velocity[1] = 0;
    velocity[2] = Math_Sine(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14) >> 16;
    i++;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
    Func_020000b8(leader->x, leader->y + -0x40000, leader->z, velocity[0],
                  velocity[1], velocity[2], 0x01090001, &options);
  }
  *(s32 *)((u8 *)leader + 40) = 0x40000;
  wait(2);
  Call3((void (*)())Map_SetScreenShake, -1, -1, 0xe666);
  Task_RemoveFar(Func_02001464);
  spin = 0x200;
  for (i = 0; i <= 7;) {
    sprite->rotation += spin;
    /* FAKEMATCH(dead): an empty once-only block after the rotation's store,
     * which the next load of the stone follows; as no block, 2 of 199
     * instructions differ in tla-en. */
    do {
    } while (0);
    stone->z += 0x10000;
    i++;
    wait(1);
  }
  Map_WaitForShakeToSettle();
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  leader->layer = 0;
  wait(10);
  Event_End();
}

void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base) {
  do {
    wait(1);
  } while (object->speed != 0);
  Sound_Play(240);
  Actor_SetSpritePriority(base + 21, 3);
  Actor_SetSpritePriority(base + 22, 3);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 21), object->x, 0,
                                   object->z);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 22), object->x, 0,
                                   object->z + -0x100000);
  Actor_SetModeById(base + 21, 1);
  Actor_SetModeById(base + 22, 1);
  wait(20);
}

void Func_020016c8(void) { Func_0200142c(Actor_GetById(15)); }

void Func_020016d8(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *pillar = Actor_GetById(15);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 328, 216);
  Func_02001340(leader, pillar);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
  wait(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(15, 3);
  pillar->priorityFlags |= 2;
  Task_Add(Func_020016c8, 0x480);
  Func_02001658(leader, 0);
  Actor_SetSpritePriority(9, 1);
  Actor_GetById(9)->priorityFlags &= ~2;
  Actor_SetSpritePriority(11, 1);
  Actor_GetById(11)->priorityFlags &= ~2;
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x9999, 0x1999);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 328, 224);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 256);
  Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 320);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 400);
  ShrineOfTheSeaGod_ObjCommit(gMainRoutineWork.currentOwner, 321, 480);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 321, 536);
  Sound_Play(161);
  Actor_SetSpritePriority(9, 2);
  Actor_GetById(9)->priorityFlags |= 2;
  Actor_SetSpritePriority(11, 2);
  Actor_GetById(11)->priorityFlags |= 2;
  leader->motionFlags = 3;
  Task_RemoveFar(Func_020016c8);
  wait(1);
  Event_End();
  GameFlag_SetBit(0x231);
}

void Func_020018ac(void) { Func_0200142c(Actor_GetById(16)); }

void Func_020018bc(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *pillar = Actor_GetById(16);

  Event_Start();
  Event_ResolvePendingActions(0);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 408, 360);
  Func_02001340(leader, pillar);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
  wait(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(16, 3);
  pillar->priorityFlags |= 2;
  Task_Add(Func_020018ac, 0x480);
  Func_02001658(leader, 2);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x9999, 0x1999);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 406, 360);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 400, 392);
  Sound_Play(161);
  Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Task_RemoveFar(Func_020018ac);
  wait(1);
  Event_End();
  GameFlag_SetBit(0x232);
}

void Func_020019dc(void) {
  ShrineOfTheSeaGod_CopyRect(42, 1, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, 14);
}

void Func_02001a0c(void) {
  ShrineOfTheSeaGod_CopyRect(15, 9, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(14, 14, 1, 1, 15, 14);
}

void Func_02001a3c(void) {
  struct ShrineOfTheSeaGodObject *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgFieldRopeThrownAgain);
  Event_ShowMessageAndWait(0x200c, 0);
  ShrineOfTheSeaGod_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjSpeed(14, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 272, 336);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 272 << 16, 336 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjWalk(14, 256, 336);
  Actor_ArmCallback(14, 0, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  ShrineOfTheSeaGod_ObjTurn(14, 0xd000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(288 << 16, -1, 126 << 16, 1);
  Func_02003ac8();
  Actor_Jump(13, 4, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(12, 2);
  Actor_Jump(12, 6, 80);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  Actor_SetModeById(12, 4);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 0);
  ShrineOfTheSeaGod_Jump(13, 265, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  Actor_ArmCallback(13, 0, 0);
  FieldEffect_SetAttached(13, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(0x800d, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_ShowMessageAndWait(0x400c, 0);
  Camera_SetSpeedParameters(0x33333, 0x6666);
  ShrineOfTheSeaGod_CamMove(420 << 16, -1, 200 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(12, 346, 186);
  ShrineOfTheSeaGod_ObjWalk(12, 374, 192);
  ShrineOfTheSeaGod_ObjWalk(12, 392, 224);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 246);
  ShrineOfTheSeaGod_ObjWalk(12, 420, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 456, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 240);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 226);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(264 << 16, -1, 320 << 16, 1);
  Func_02003ac8();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(14, 0, 20);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(14, 3);
  Actor_SetModeById(14, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_ResetAndSetPosition(14, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  GameFlag_SetBit(0x872);
  Event_End();
}

extern const u8 gData_02003c58[];

void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_ArmCallbackAndRefresh(s32 actor, s32 angle);

static inline void ShrineOfTheSeaGod_ObjAngle(s32 actor, s32 angle) {
  Actor_ArmCallbackAndRefresh(actor, angle);
}
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);

void Func_02001d24(void) {
  struct ShrineOfTheSeaGodObject *object;
  struct ShrineOfTheSeaGodObject *leader;
  struct ShrineOfTheSeaGodSprite *sprite;
  u8 *motionFlags;
  const void *callback;

  if (ShrineOfTheSeaGod_FlagCheck(0x873))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_080d4568()->motionFlags = 0;
  ShrineOfTheSeaGod_CamMove(170 << 17, -1, 176 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjSpeed(14, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 344, 184);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x2000, 0);
  ShrineOfTheSeaGod_ObjPlace(14, 344 << 16, 184 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjWalk(14, 356, 196);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through the inline
   * ShrineOfTheSeaGod_ObjAngle, here and below; as direct calls, 128 of 656
   * instructions differ in tla-en. */
  ShrineOfTheSeaGod_ObjAngle(14, 0xb000);
  Event_SetMessageId((s32)MsgFieldPraiseRopeWork);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  ShrineOfTheSeaGod_MessagePut(13, 0);
  ShrineOfTheSeaGod_ObjPlace(13, 184 << 16, 168 << 16);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x8000, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(232 << 16, 64 << 16, 176 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(13, 232, 168);
  Event_Wait(20);
  ShrineOfTheSeaGod_MessagePut(13, 0);
  ShrineOfTheSeaGod_CamSpeed(0x13333, 0x2666);
  ShrineOfTheSeaGod_CamMove(174 << 17, -1, 216 << 16, 1);
  object = Actor_GetById(13);
  sprite = object->sprite;
  Actor_SetSpritePriority(13, 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  Actor_Jump(13, 4, 0);
  ShrineOfTheSeaGod_ObjCommit(13, 248, 168);
  Actor_SetModeById(13, 6);
  /* FAKEMATCH(temporary): the first motion flags store written through a byte
   * pointer, so it may alias the rotation and stays after it; as
   * object->motionFlags, which keeps the struct's alias set, 12 of 656
   * instructions differ in tla-en. */
  motionFlags = &object->motionFlags;
  /* FAKEMATCH(signature): field18 written through a one-member union, so both
   * its stores may alias the sprite's rotation and stay ahead of it; as
   * object->field18, 4 of 656 instructions differ in tla-en. */
  ((union ObjectWord *)&object->field18)->value = -0x10000;
  sprite->rotation = 0xd000;
  *motionFlags = 0;
  object->y += 0x60000;
  Actor_SetSingleChildFlags(Actor_GetById(13), 0);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x6666, 0x3333);
  /* FAKEMATCH(wrapper): Actor_SetMoveTarget called through Call4; as a direct
   * call, 2 of 656 instructions differ in tla-en. */
  Call4((void (*)())Actor_SetMoveTarget, (s32)object, 164 << 17, 152 << 14,
        168 << 16);
  // A once-only block: its end holds the order around it (as the plain
  // statement, 2 of 656 differ in tla-en).
  do {
    Actor_WaitForMoveById(13);
  } while (0);
  object->motionFlags = 3;
  object->height = 0x200000;
  ((union ObjectWord *)&object->field18)->value = 0x10000;
  sprite->rotation = 0;
  Actor_SetModeById(13, 1);
  Actor_SetSingleChildFlags(Actor_GetById(13), 1);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x19999, 0xcccc);
  Actor_Jump(13, 4, 0);
  ShrineOfTheSeaGod_ObjCommit(13, 328, 184);
  ShrineOfTheSeaGod_ObjTurn(13, 0, 20);
  ShrineOfTheSeaGod_Jump(13, 256, 40);
  Actor_SetSpritePriority(13, 2);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_MessagePut(0x200e, 0);
  ShrineOfTheSeaGod_Jump(13, 257, 20);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0xb000, 40);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 0);
  Event_ShowMessageThenWait(0x200e, 0, 60);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_SetModeById(13, 3);
  ShrineOfTheSeaGod_ObjPlace(12, 394 << 16, 286 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 248);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  Actor_Jump(12, 4, 20);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_Jump(13, 256, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0x2000, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x3000, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(188 << 17, -1, 264 << 16, 1);
  Func_02003ac8();
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_CamSpeed(0x26666, 0x4ccc);
  ShrineOfTheSeaGod_CamMove(174 << 17, -1, 216 << 16, 1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 12);
  Actor_WatchAndSetScript(14, 12);
  Actor_WatchAndSetScript(13, 12);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 220);
  ShrineOfTheSeaGod_ObjWalk(12, 370, 198);
  ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjCommit(14, 368, 180);
  Actor_WatchAndSetScript(14, 12);
  ShrineOfTheSeaGod_ObjWalk(12, 336, 198);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  Actor_Stop(13);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_ArmCallback(12, 0xd000, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_ObjAngle(12, 0xb000);
  Actor_Jump(12, 4, 20);
  Event_ShowMessageThenWait(12, 0, 20);
  ShrineOfTheSeaGod_Jump(13, 261, 40);
  ShrineOfTheSeaGod_Jump(12, 257, 60);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Actor_SetModeById(13, 4);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_Jump(12, 256, 20);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x8000, 20);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_ObjTurn(12, 0x8000, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Motion_SetModeAndWaitAnimation(13, 4);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_Jump(12, 265, 40);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  ShrineOfTheSeaGod_Jump(12, 256, 0);
  ShrineOfTheSeaGod_Jump(13, 256, 0);
  ShrineOfTheSeaGod_ObjTurn(12, 0xb000, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 40);
  Actor_Jump(13, 4, 40);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  Actor_SetModeById(12, 3);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  ShrineOfTheSeaGod_MessagePut(0x400d, 0);
  ShrineOfTheSeaGod_MessagePut(12, 0);
  Actor_SetModeById(12, 3);
  Motion_SetModeAndWaitAnimation(13, 3);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x13333, 0x9999);
  ShrineOfTheSeaGod_ObjSpeed(13, 0x13333, 0x9999);
  callback = gData_02003c58;
  Actor_SetMovementTypeById(12, callback);
  Event_Wait(10);
  Actor_SetMovementTypeAndWaitById(13, callback);
  Event_Wait(40);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(14);
  wait(1);
  Event_ShowMessageThenWait(14, 0, 40);
  ShrineOfTheSeaGod_ObjPlace(13, 0, 0);
  ShrineOfTheSeaGod_ObjPlace(12, 0, 0);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, 0, 0);
  ShrineOfTheSeaGod_ObjTurn(14, 0x8000, 20);
  ShrineOfTheSeaGod_MessagePut(14, 0);
  Actor_SetModeById(14, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_ResetAndSetPosition(14, leader->x >> 16, leader->z >> 16);
  Actor_WaitForMoveById(14);
  ShrineOfTheSeaGod_ObjPlace(14, 0, 0);
  GameFlag_SetBit(0x873);
  Event_End();
}

void Func_02002384(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  Func_080d4568()->motionFlags = off;
  Camera_MoveWithinMapBounds(480 << 16, 32 << 16, 340 << 16, 1);
  Func_02003ac8();
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(15, 0x100, 40);
  Actor_SetSpritePriority(15, 1);
  Actor_GetById(15)->motionFlags = off;
  ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(15, 500, 344);
  ShrineOfTheSeaGod_ObjWalk(15, 580, 344);
  Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
  Event_Wait(20);
  GameFlag_SetBit(0x874);
  Event_End();
}

void Func_02002430(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(15);
  Func_080d4568()->motionFlags = off;
  Camera_MoveWithinMapBounds(564 << 16, 32 << 16, 344 << 16, 1);
  Func_02003ac8();
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(15, 0x100, 40);
  Actor_SetSpritePriority(15, 1);
  Actor_GetById(15)->motionFlags = off;
  ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(15, 616, 376);
  ShrineOfTheSeaGod_ObjWalk(15, 616, 460);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Event_Wait(20);
  GameFlag_SetBit(0x875);
  Event_End();
}

/// Actor 14, linked to the leader, walks to (280, 432); then flags 0x876 and
/// 0x877 are set.
void Func_020024e4(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(13);
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Value3((s32 (*)())Func_080d47b4, gMainRoutineWork.currentOwner, 0x100, 0);
  Func_080d4568()->motionFlags = 0;
  ShrineOfTheSeaGod_CamMove(328 << 16, 32 << 16, 552 << 16, 1);
  Func_02003ac8();
  /* FAKEMATCH(temporary): the angle in a local holding one constant, built
   * before the coordinates; as the constant 0xc000 in the call, 4 of 112
   * instructions differ in tla-en. */
  angle = 0xc000;
  ShrineOfTheSeaGod_ObjPlaceTurn(14, 328 << 16, 624 << 16, angle);
  wait(1);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 14);
  object->priorityFlags &= ~2;
  Actor_SetSpritePriority(14, 3);
  ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(14, 328, 568);
  ShrineOfTheSeaGod_ObjWalk(14, 328, 496);
  Actor_SetPositionAndReset(14, 280, 432);
  Actor_Stop(gMainRoutineWork.currentOwner);
  wait(1);
  angle = 0x4000;
  ShrineOfTheSeaGod_ObjPlaceTurn(14, 280 << 16, 328 << 16, angle);
  Actor_SetSpritePriority(14, 2);
  Actor_GetById(14)->priorityFlags |= 1;
  object->priorityFlags |= 2;
  wait(1);
  GameFlag_SetBit(0x876);
  GameFlag_SetBit(0x877);
  Event_End();
}

void Func_020025fc(void) {
  s32 done = ShrineOfTheSeaGod_FlagCheck(0x876);

  if (done == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Func_080d47b4(gMainRoutineWork.currentOwner, 0x100, 0);
    Func_080d4568()->motionFlags = done;
    Camera_MoveWithinMapBounds(328 << 16, 48 << 16, 552 << 16, 1);
    Func_02003ac8();
    /* FAKEMATCH(temporary): done, the flag result, reused for the angle, built
     * early; as the constant 0x4000 in the call, 2 of 98 instructions differ
     * in tla-en. */
    done = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(14, 424 << 16, 520 << 16, done);
    wait(1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 14);
    Actor_SetSpritePriority(14, 1);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(14, 380, 552);
    ShrineOfTheSeaGod_ObjWalk(14, 264, 552);
    Actor_SetPositionAndReset(14, 264, 540);
    Actor_Stop(gMainRoutineWork.currentOwner);
    wait(1);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Actor_SetSpritePriority(14, 2);
    Actor_GetById(14)->priorityFlags |= 1;
    wait(1);
    GameFlag_SetBit(0x876);
    Event_End();
  }
}

void Func_020026f8(void) {
  u8 off;

  if (GameFlag_Test(0x877)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    off = 0;
    Func_080d4568()->motionFlags = off;
    Camera_MoveWithinMapBounds(320 << 16, -1, 300 << 16, 1);
    ShrineOfTheSeaGod_Jump(14, 0x100, 40);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x13333, 0x9999);
    ShrineOfTheSeaGod_ObjWalk(14, 345, 296);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 296);
    Actor_WatchAndSetScript(14, gMainRoutineWork.currentOwner);
    Actor_GetById(14)->state = off;
    wait(10);
    GameFlag_SetBit(0x87c);
    Event_End();
  }
}

void Func_0200279c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_080d4568()->motionFlags = 0;
  Actor_Stop(14);
  wait(1);
  ShrineOfTheSeaGod_ObjSpeed(14, 0x26666, 0x13333);
}

void Func_020027d4(void) {
  Actor_WatchAndSetScript(14, gMainRoutineWork.currentOwner);
  Event_Wait(5);
  Event_End();
}

void Func_020027f8(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(392 << 16, 16 << 16, 216 << 16, 1);
  Actor_SetPositionAndReset(14, 392, 216);
  Actor_GetById(14)->state = 1;
  Func_020027d4();
}

void Func_02002830(void) {
  struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);
  u8 *state;
  s32 flag;

  Func_0200279c();
  ShrineOfTheSeaGod_CamMove(360 << 16, 16 << 16, 216 << 16, 1);
  ShrineOfTheSeaGod_ObjWalk(14, 360, 216);
  flag = GameFlag_Test(0x207);
  state = &block->state;
  if (flag) {
    block->facing = 0x8000;
    ShrineOfTheSeaGod_ObjEmote(14, 258);
    Event_Wait(20);
    *state = 4;
    GameFlag_SetBit(0x206);
  } else {
    Event_Wait(5);
    Camera_MoveWithinMapBounds(296 << 16, 16 << 16, 216 << 16, 1);
    Sound_Play(153);
    Actor_SetSingleChildFlags(Actor_GetById(14), 0);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
    block->speed = 0x80000;
    Actor_SetPositionAndCommit(14, 328, 216);
    Actor_SetSingleChildFlags(Actor_GetById(14), 1);
    Event_Wait(10);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(14, 296, 216);
    *state = 2;
  }
  Func_020027d4();
}

void Func_0200291c(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(280 << 16, 16 << 16, 296 << 16, 1);
  ShrineOfTheSeaGod_ObjWalk(14, 296, 248);
  ShrineOfTheSeaGod_ObjWalk(14, 280, 248);
  Actor_SetPositionAndReset(14, 280, 296);
  Actor_GetById(14)->state = 3;
  Func_020027d4();
}

void Func_0200296c(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(392 << 16, 16 << 16, 296 << 16, 1);
  Actor_SetPositionAndReset(14, 392, 296);
  Actor_GetById(14)->state = 0;
  Func_020027d4();
}

void Func_020029a4(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(280 << 16, 16 << 16, 296 << 16, 1);
  Actor_SetPositionAndReset(14, 280, 296);
  Actor_GetById(14)->state = 3;
  Func_020027d4();
}

void Func_020029dc(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(296 << 16, 16 << 16, 216 << 16, 1);
  ShrineOfTheSeaGod_ObjWalk(14, 280, 248);
  ShrineOfTheSeaGod_ObjWalk(14, 296, 248);
  Actor_SetPositionAndReset(14, 296, 216);
  Actor_GetById(14)->state = 2;
  Func_020027d4();
}

void Func_02002a2c(void) {
  struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);
  u8 *state;
  s32 flag;

  Func_0200279c();
  ShrineOfTheSeaGod_CamMove(328 << 16, 16 << 16, 216 << 16, 1);
  ShrineOfTheSeaGod_ObjWalk(14, 328, 216);
  flag = GameFlag_Test(0x207);
  state = &block->state;
  if (flag) {
    block->facing = 0;
    ShrineOfTheSeaGod_ObjEmote(14, 258);
    Event_Wait(20);
    *state = 5;
    GameFlag_SetBit(0x206);
  } else {
    Event_Wait(5);
    Camera_MoveWithinMapBounds(392 << 16, 16 << 16, 216 << 16, 1);
    Sound_Play(153);
    Actor_SetSingleChildFlags(Actor_GetById(14), 0);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
    block->speed = 0x80000;
    Actor_SetPositionAndCommit(14, 360, 216);
    Actor_SetSingleChildFlags(Actor_GetById(14), 1);
    Event_Wait(10);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 216);
    *state = 1;
  }
  Func_020027d4();
}

void Func_02002b18(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(392 << 16, 16 << 16, 296 << 16, 1);
  Actor_SetPositionAndReset(14, 392, 296);
  Actor_GetById(14)->state = 0;
  Func_020027d4();
}

void Func_02002b50(void) {
  struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);
  u8 *state;
  s32 flag;

  Func_0200279c();
  flag = GameFlag_Test(0x207);
  state = &block->state;
  if (flag) {
    block->facing = 0x8000;
    ShrineOfTheSeaGod_ObjEmote(14, 258);
    Event_Wait(20);
    *state = 4;
    GameFlag_SetBit(0x206);
  } else {
    Camera_MoveWithinMapBounds(296 << 16, 16 << 16, 216 << 16, 1);
    Sound_Play(153);
    Actor_SetSingleChildFlags(Actor_GetById(14), 0);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
    block->speed = 0x80000;
    Actor_SetPositionAndCommit(14, 328, 216);
    Actor_SetSingleChildFlags(Actor_GetById(14), 1);
    Event_Wait(10);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(14, 296, 216);
    *state = 2;
  }
  Func_020027d4();
}

void Func_02002c18(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(392 << 16, 16 << 16, 216 << 16, 1);
  Actor_SetPositionAndReset(14, 392, 216);
  Actor_GetById(14)->state = 1;
  Func_020027d4();
}

void Func_02002c50(void) {
  Func_0200279c();
  Camera_MoveWithinMapBounds(296 << 16, 16 << 16, 216 << 16, 1);
  Actor_SetPositionAndReset(14, 296, 216);
  Actor_GetById(14)->state = 2;
  Func_020027d4();
}

void Func_02002c88(void) {
  struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);
  u8 *state;
  s32 flag;

  Func_0200279c();
  flag = GameFlag_Test(0x207);
  state = &block->state;
  if (flag) {
    block->facing = 0;
    ShrineOfTheSeaGod_ObjEmote(14, 258);
    Event_Wait(20);
    *state = 5;
    GameFlag_SetBit(0x206);
  } else {
    Camera_MoveWithinMapBounds(392 << 16, 16 << 16, 216 << 16, 1);
    Sound_Play(153);
    Actor_SetSingleChildFlags(Actor_GetById(14), 0);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);
    block->speed = 0x80000;
    Actor_SetPositionAndCommit(14, 360, 216);
    Actor_SetSingleChildFlags(Actor_GetById(14), 1);
    Event_Wait(10);
    ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);
    ShrineOfTheSeaGod_ObjWalk(14, 392, 216);
    *state = 1;
  }
  Func_020027d4();
}

void Func_02002d50(void) {
  struct ShrineOfTheSeaGodEventWork *work;
  struct ShrineOfTheSeaGodObject *object;

  if (!GameFlag_Test(0x87c))
    return;
  if (GameFlag_Test(115))
    return;
  work = Ram_HeapSlots->eventWork;
  object = Actor_GetById(14);
  switch (work->exit) {
  case 13:
    if (object->state == 0)
      Func_020027f8();
    GameFlag_ClearBit(0x206);
    break;
  case 14:
    if (object->state == 0)
      Func_020029a4();
    break;
  case 15:
    {
      u8 current = object->state;

      if (current == 1)
        Func_02002830();
      else if (current == 4 && !GameFlag_Test(0x206))
        Func_02002b50();
    }
    break;
  case 16:
    {
      u8 current = object->state;

      if (current == 1)
        Func_02002b18();
      else if (current == 5)
        Func_02002c50();
    }
    break;
  case 17:
    {
      u8 current = object->state;

      if (current == 2)
        Func_0200291c();
      else if (current == 4)
        Func_02002c18();
    }
    break;
  case 18:
    {
      u8 current = object->state;

      if (current == 2)
        Func_02002a2c();
      else if (current == 5 && !GameFlag_Test(0x206))
        Func_02002c88();
    }
    break;
  case 19:
    if (object->state == 3)
      Func_0200296c();
    break;
  case 20:
    if (object->state == 3)
      Func_020029dc();
    GameFlag_ClearBit(0x206);
    break;
  }
}

extern const struct ShrineOfTheSeaGodSpot gData_02003cc0[];
extern const u8 gData_02003cbc[];
extern const u8 gData_02003c10[];

void Actor_SetPartAttribute(struct ShrineOfTheSeaGodObject *object, s32 value);
void Func_02003b20(s32 a, s32 b, s32 c, s32 d);
void Func_02003b30(const void *script);
void Func_02001128(void);

void Func_02002e70(void) {
  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 0);
  Func_02003aa0(9);
}

void Func_02002e8c(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->layer = 2;
}

void Func_02002ea8(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->layer = 0;
}

s32 Scene_Initialize(void) {
  s16 scene;

  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
    Func_02003094();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
    Func_02003134();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
    Func_0200320c();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
    Func_02003354();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
    Func_0200338c();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    Func_02003504();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
    Func_02003598();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
    Func_02003740();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    Func_0200377c();
  else if (scene == (s32)&SceneId_ShrineOfTheSeaGod11)
    Func_0200380c();
  return 0;
}

struct ShrineOfTheSeaGodObject *Func_02002f88(s32 id) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);

  object->priorityFlags |= 2;
  object->y = 0xc0000;
  Func_02003aa0(id);
  return object;
}

void Func_02002fb0(s32 id) {
  Func_02003aa0(id);
  Actor_SetPartAttribute(Actor_GetById(id), 15);
  Actor_SetSpritePriority(id, 1);
  Actor_GetById(id)->priorityFlags |= 2;
}

void Func_02002fe0(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(0x208) == 0)
      Func_0200123c();
  } else {
    GameFlag_SetBit(0x208);
  }
}

void Func_0200300c(void) {
  u32 i;

  for (i = 0; i <= 2; i++)
    Func_02002fb0(i + 9);
  Func_02002fe0();
}

void Func_02003024(void) {
  const struct ShrineOfTheSeaGodSpot *spot = gData_02003cc0;
  u32 i;

  for (i = 0; i <= 15; i++) {
    s32 id = i + 8;
    struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);
    struct ShrineOfTheSeaGodSprite *sprite;

    Func_02003aa0(id);
    Actor_SetModeById(id, 0);
    Actor_SetSpritePriority(id, 3);
    object->priorityFlags |= 2;
    Actor_SetPartAttribute(Actor_GetById(id), 12);
    sprite = object->sprite;
    object->x = spot->x;
    object->z = spot->z;
    object->field18 = spot->field18;
    sprite->rotation = spot->rotation;
    spot++;
  }
}

void Func_02003094(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;

  Func_02003854(8);
  Func_02003854(9);
  {
    s32 priority;

    map->mode17 = 1;
    priority = 0x480;
    Task_Add(Func_020010e8, priority);
  }
  Func_02003b18();
  Func_02003b20(0, 515, 10, 11);
  if (GameFlag_Test(0x872)) {
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  }
}

void Func_020030fc(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(11);

  if (object->x > leader->x)
    object->collisionFlags |= 8;
  else
    object->collisionFlags &= ~8;
}

void Func_02003134(void) {
  s32 angle;

  Func_02002f88(10)->y = -0x100000;
  Actor_SetSpritePriority(10, 3);
  Func_02003b30(gData_02003cbc);
  Func_02002e70();
  ShrineOfTheSeaGod_TaskAdd(Func_020011a0, 0x480);
  Func_02003aa0(11);
  Func_02003aa0(12);
  Actor_GetById(12)->motionFlags = 0;
  ShrineOfTheSeaGod_BlendStart(3, 15, 1, 1, 6, 15);
  if (GameFlag_Test(0x873)) {
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  } else if (GameFlag_Test(0x872)) {
    /* FAKEMATCH(temporary): the function-scope angle local holding one
     * constant, built early; as the constant 0x8000 in the call, 2 of 79
     * instructions differ in tla-en. */
    angle = 0x8000;
    ShrineOfTheSeaGod_ObjPlaceTurn(11, 328 << 16, 200 << 16, angle);
    wait(1);
    Actor_SetMovementTypeById(11, gData_02003c10);
    Actor_SetSingleChildFlags(Actor_GetById(11), 1);
    Actor_GetById(11)->collisionFlags = 1;
  }
  /* FAKEMATCH(temporary): SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE's local holding
   * the priority builds it before the task function loads; as
   * Task_Add(Func_020030fc, 0x480), 2 of 79 instructions differ in tla-en. */
  SHRINE_OF_THE_SEA_GOD_TASK_ADD_LATE(Func_020030fc, 0x480);
}

void Func_0200320c(void) {
  struct ShrineOfTheSeaGodObject *block;
  s32 angle;

  Func_02002f88(9);
  Func_02002f88(10);
  Func_02002f88(11);
  Func_02002f88(12);
  Func_02002f88(13);
  block = Func_02002f88(14);
  Actor_SetModeById(9, 0);
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 2);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Actor_SetSpritePriority(14, 1);
  ShrineOfTheSeaGod_TaskAdd(Func_02001088, 0x480);
  if (GameFlag_Test(0x230)) {
    ShrineOfTheSeaGod_ObjSetPos(block, 408 << 16, -(13 << 16), 336 << 16);
    ((struct ShrineOfTheSeaGodSprite *)block->sprite)->rotation = 0x1000;
    Actor_SetSpritePriority(14, 3);
    block->priorityFlags |= 2;
    ShrineOfTheSeaGod_BlendStart(0, 50, 3, 1, 24, 70);
  }
  if (GameFlag_Test(0x84c))
    ShrineOfTheSeaGod_ObjSetPos(Actor_GetById(8), 200 << 16, 0, 296 << 16);
  Func_02003b30(gData_02003cbc);
  if (GameFlag_Test(0x875))
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  else if (GameFlag_Test(0x874)) {
    /* FAKEMATCH(temporary): the function-scope angle local holding one
     * constant, built early; as the constant 0x4000 in the call, 4 of 127
     * instructions differ in tla-en. */
    angle = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(15, 580 << 16, 344 << 16, angle);
  }
  Func_02003b00(0);
  wait(1);
}

void Func_02003354(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;

  if (GameFlag_Test(0x876) == 0)
    Func_02003024();
  {
    s32 priority;

    map->mode17 = 1;
    priority = 0x480;
    Task_Add(Func_020010e8, priority);
  }
}

void Func_0200338c(void) {
  s32 angle;
  struct ShrineOfTheSeaGodObject *object = Func_02002f88(13);
  u8 *flags = &object->priorityFlags;

  if ((u16)(gMainRoutineWork.entrance - 2) <= 1) {
    Actor_SetSpritePriority(13, 3);
    /* FAKEMATCH(dead): the cancelling update ties the flag or to the
     * constant's register; as *flags |= 2, 4 of 153 instructions differ in
     * tla-en. */
    *flags = *flags + 2 - (*flags & 2);
  } else {
    Actor_SetSpritePriority(13, 1);
    *flags &= ~2;
  }
  Func_02000f18(10);
  Func_02000f18(11);
  Func_02000f18(12);
  if (GameFlag_Test(0x200))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 8, 18);
  if (GameFlag_Test(0x201))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 18);
  if (GameFlag_Test(0x202))
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 17);
  ShrineOfTheSeaGod_TaskAdd(Func_02001128, 0x480);
  Func_02003b30(gData_02003cbc);
  Func_02002e70();
  ShrineOfTheSeaGod_TaskAdd(Func_020011a0, 0x480);
  if (GameFlag_Test(115) || GameFlag_Test(0x340)) {
    GameFlag_SetBit(0x205);
  } else if (GameFlag_Test(0x87c)) {
    struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);
    s32 moved = GameFlag_Test(265);

    if (moved == 0) {
      block->state = moved;
      ShrineOfTheSeaGod_ObjPlace(14, 392 << 16, 296 << 16);
    }
    Actor_WatchAndSetScript(14, gMainRoutineWork.currentOwner);
  } else if (GameFlag_Test(0x877)) {
    /* FAKEMATCH(temporary): the function-scope angle local holding one
     * constant, built early; as the constant 0x4000 in the call, 4 of 153
     * instructions differ in tla-en. */
    angle = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(14, 280 << 16, 328 << 16, angle);
  }
}

void Func_02003504(void) {
  s32 open;

  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  open = GameFlag_Test(0x301);
  if (open) {
    ShrineOfTheSeaGod_ObjPlace(8, 248 << 16, 264 << 16);
    Actor_SetSpritePriority(8, 3);
    Actor_GetById(8)->motionFlags = 0;
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, 16);
    Func_02003aa0(8);
  } else {
    Actor_SetSpritePriority(8, 1);
    Actor_GetById(8)->motionFlags = open;
  }
  Func_0200300c();
}

/// The Shrine of the Sea God's prepare step: sets up actors 8 to 24 and,
/// for each of flags 0x231 and 0x232 that is set, copies its opened tiles
/// and places actor 15 or 16 where it rests.
void Func_02003598(void) {
  struct ShrineOfTheSeaGodObject *object;
  struct ShrineOfTheSeaGodObject *lastObject;

  ((struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork)->mode17 = 1;
  Func_02002f88(8);
  Func_02002f88(9);
  Func_02002f88(10);
  Func_02002f88(11);
  Func_02002f88(12);
  Func_02002f88(13);
  Func_02002f88(14);
  Func_02002f88(15);
  Func_02002f88(16);
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 0);
  Actor_SetModeById(15, 2);
  Actor_SetModeById(16, 2);
  if (GameFlag_Test(0x231)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
    Actor_SetPositionAndResetMotion(Actor_GetById(15), PIXELS(320), -PIXELS(21),
                                     PIXELS(534));
    Actor_SetSpritePriority(15, 3);
  }
  if (GameFlag_Test(0x232)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
    Actor_SetPositionAndResetMotion(Actor_GetById(16), PIXELS(400), -PIXELS(21),
                                     PIXELS(390));
    Actor_SetSpritePriority(16, 3);
  }
  Func_02003b18();
  Call4((void (*)())Func_02003b20, 0, 0x203, 17, 18);
  Func_02003b20(1, 0x204, 19, 20);
  Func_02003aa0(21);
  Actor_GetById(21)->motionFlags = 0;
  object = Actor_GetById(22);
  Func_02003aa0(22);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  Func_02003aa0(23);
  Actor_GetById(23)->motionFlags = 0;
  /* FAKEMATCH(temporary): actor 24 fetched into its own variable; as
   * object = Actor_GetById(24), 51 of 171 instructions differ in tla-en. */
  lastObject = Actor_GetById(24);
  object = lastObject;
  Func_02003aa0(24);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
}

void Func_02003740(void) {
  Func_02003854(8);
  Func_02003854(9);
  Func_02003854(10);
  if (gMainRoutineWork.entrance == 3 && GameFlag_Test(265) == 0)
    Func_02000994();
}

void Func_0200377c(void) {
  if (gMainRoutineWork.entrance == 1 && GameFlag_Test(265) == 0)
    Func_02000ce8();
}

void Func_020037a4(struct ShrineOfTheSeaGodObject *object) {
  struct ShrineOfTheSeaGodEffectParams params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      /* FAKEMATCH(temporary): the local holding one constant builds the mask
       * before the random draw; as 15 & Random16Far(), 7 of 46 instructions
       * differ in tla-en. */
      s32 mask = 15;

      dx = mask & Random16Far();
    }
    dy = Random16Far() & 7;
    params.kind = 673;
    Func_020000b8(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16),
                  object->z, 0, 0x8000, phase, 0x100000, &params);
  }
}

void Func_0200380c(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(64);
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[2];

  layer->x += 0x800000;
  Map_CenterOnFollowedPoint();
  wait(1);
  if (object != NULL) {
    object->motionFlags = 0;
    object->y = 0x420000;
    object->update = Func_020037a4;
  }
}

void Func_02003854(s32 id) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  Func_02003aa0(id);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  object->field18 = 0xcccc;
  object->field1c = 0xe666;
}

s32 Scene_PrepareMap(void) {
  s32 angle;

  if (GameFlag_Test(0x340)) {
    struct ShrineOfTheSeaGodPartyExtra *party =
        (struct ShrineOfTheSeaGodPartyExtra *)&gMainRoutineWork;

    if (party->value == 1) {
      if (GameFlag_Test(0x341))
        ShrineOfTheSeaGod_ObjPlaceAt(14, 360 << 16, 216 << 16, 0);
      else if (GameFlag_Test(0x342)) {
        /* FAKEMATCH(temporary): the function-scope angle local holding one
         * constant, built early; as the constant 0x8000 in the call, 4 of 56
         * instructions differ in tla-en. */
        angle = 0x8000;
        ShrineOfTheSeaGod_ObjPlaceTurn(14, 328 << 16, 216 << 16, angle);
      }
    }
    GameFlag_ClearBit(0x340);
    GameFlag_ClearBit(0x341);
    GameFlag_ClearBit(0x342);
  }
  return 0;
}
