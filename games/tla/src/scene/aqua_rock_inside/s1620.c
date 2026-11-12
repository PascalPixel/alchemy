/// @file
/// Scene code of overlay 0x67c, in ROM order, from s1620's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "system/io_reg.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "main_routine_work.h"

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};
extern const u8 gData_02004034[], gData_02003fe8[], gData_02004004[],
    gData_02004058[], gData_02004014[], gData_0200407c[], gData_020040ac[];
u32 Random16Far(void);
void Scene_SetActorSpritePriority(void *object, s32 priority);
void Func_02003cc4(void *object, s32 mode);
void Actor_SetSingleChildFlags(void *object, u32 value);
void Animation_SetStateFlags(void *object, s32 flags);
void Func_02003ccc(void *object, const u8 *script);
void Func_02001598(void);
void Func_02003ebc(void);

void Func_02001620(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_02004058;
  Sound_Play(78);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  Camera_MoveWithinMapBounds((first->x >> 16) << 16, (first->y >> 16) << 16,
                             (first->z >> 16) << 16, 1);
  Sound_Play(141);
  /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x20000; as
   * a direct call, 17 of 402 instructions differ in tla-en. */
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(firstId, gData_02004034);
  Sound_Play(194);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(firstId, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = ((void *)Import_ActorCreate(672, first->x, first->y, first->z));
  Sound_Play(246);
  object->x += ((Random16Far() & 31) - 16) << 16;
  object->y += ((Random16Far() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Func_02003cc4(object, 7);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02003ccc(object, gData_02003fe8);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1; as the spiral alone, 174 of 402 instructions differ in tla-en. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  /* FAKEMATCH(wrapper): forwarded, so second->z takes r0 for its reload; as a
   * direct call, 105 of 402 instructions differ in tla-en. */
  spark = FieldActor_Create(672, second->x, second->y, second->z);
  Sound_Play(195);
  spark->action = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetPartAttribute(spark, 2);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02003cc4(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2
   * of 402 instructions differ in tla-en. */
  FieldVector_AddPolarOffset(spark->unk4c, spark->action, &spark->x);
  Func_02003ccc(spark, gData_02004004);
  if (i == 3) {
    /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2
     * of 402 instructions differ in tla-en. */
    Actor_SetSpriteColorById(secondId, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(secondId, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Sound_Play(220);
  wait(16);
  i = 2;
column:
  /* FAKEMATCH(wrapper): forwarded, so second->z takes r0 for its reload; as a
   * direct call, 116 of 402 instructions differ in tla-en. */
  riser = FieldActor_Create(672, second->x, second->y + i * 0x10000 - 0x80000,
                            second->z);
  riser->action = Random16Far();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  Actor_SetPartAttribute(riser, 2);
  Func_02003cc4(riser, 7);
  Actor_SetSingleChildFlags(riser, 0);
  Animation_SetStateFlags(riser, 1);
  Func_02003ccc(riser, gData_02004014);
  i += 2;
  if (i != 32)
    goto column;
  Sound_Play(220);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(firstId, 0);
  Actor_ConfigureSpriteColor(secondId, 0);
  Sound_Play(574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(80);
  Actor_SetMovementTypeById(firstId, gData_0200407c);
  Actor_SetMovementTypeById(secondId, gData_020040ac);
  second->update = Func_02001598;
  wait(120);
  second->update = 0;
  Sound_Play(390);
  Func_02003ebc();
  wait(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200124c();
void Func_02001620();
void Func_02003d7c();
void Func_02003d84();
void Func_02003de4();
void Party_LearnAbility();
extern u8 MsgCheckedTheTabletIt[];
extern u8 MsgWhatWentWrong[];

void Func_02001a1c(void) {
  s32 t4, t59;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)&MsgCheckedTheTabletIt, 1);
  t4 = Event_GetFlag(0x9ae);
  if (t4 == 0)
    goto L_02009a46;
  Func_02003d84();
  goto L_02009c56;
L_02009a46:
  FieldEvent_SetMessageId((s32)&MsgCheckedTheTabletIt);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
  Func_02003de4(7, gMainRoutineWork.currentOwner);
  FieldEvent_Wait(1);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 184);
  Actor_WalkTo(7, 274, 184);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(7);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(5);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkToAndWait(7, 264, 168);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Message_ShowCentered(((s32)&MsgCheckedTheTabletIt + 2), 1);
  Func_02001620(14, 7);
  Party_LearnAbility(7, 138);
  Event_SetFlag(0x9ae);
  Event_PlaySound(107);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Func_0200124c();
  Event_PlaySound(0x186);
  FieldEvent_Wait(30);
  FieldEvent_SetMessageId((s32)&MsgWhatWentWrong);
  Actor_ShowEmote(7, 0x105, 20);
  Actor_FaceEachOther(gMainRoutineWork.currentOwner, 7, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(7, 0x100, 20);
  Event_ShowMessage(7, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  t59 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t59 == 0)
    goto L_02009c42;
  Actor_SetDestination(7, FIELD_AT(t59, s16, 10), FIELD_AT(t59, s16, 18));
L_02009c42:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Func_02003d84();
L_02009c56:
}
