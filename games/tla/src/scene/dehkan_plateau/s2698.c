/// @file
/// Scene code of overlay 0x655, in ROM order, from s2698's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "field_sprite.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
s32 Func_02003d80(void *sprite, s32 frame);
void Effect_AdvanceMotion(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

extern const u8 gData_02003f14[];

/// Flashes actor id, then raises eight puffs over actor center, then sets
/// flag and waits 20 frames.
void Func_02002698(s32 id, s32 center, s32 flag) {
  struct Actor *leader;
  s32 frame;
  u32 i;

  Sound_Play(125);
  Actor_SetPartAttribute(Actor_GetById(id), 7);
  wait(2);
  Actor_SetModeById(id, 0);
  Actor_SetPartAttribute(Actor_GetById(id), 7);
  wait(4);
  Actor_SetPartAttribute(Actor_GetById(id), 0);
  leader = Actor_GetById(center);
  frame = 0;
  for (i = 0; i <= 7; i++) {
    struct Actor *object;
    struct EffectView *effect;
    s32 y;

    /* FAKEMATCH(temporary): the form kept; as one call argument, 10 of 129
     * instructions differ in tla-en. */
    y = leader->y - ((((Random16Far() * 7) << 1) >> 16) << 16);
    object = ((struct Actor *)Import_ActorCreate(
        285, (i << 17) + leader->x - Q16_ONE * 8, y - Q16_ONE * 2, leader->z));
    if (object != NULL) {
      effect = (struct EffectView *)object;
      frame = Func_02003d80(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      Func_02003ba0(object, 2);
      Func_02003ba8(object, gData_02003f14);
      object->scaleX = 0xb333;
      object->scaleY = 0xb333;
      effect->sprite->priority = 1;
      effect->velocityX = 0;
      effect->velocityZ = 0;
      effect->velocityY =
          -((((u32)Random16Far() << 14) >> 16) + Q16_ONE * 3 / 8);
      effect->spin = (((u32)Random16Far() << 9) >> 16) - 256;
      effect->scaleRateX = -(Q16_ONE / 32);
      effect->scaleRateY = -(Q16_ONE * 3 / 128);
      effect->update = Effect_AdvanceMotion;
    }
  }
  GameFlag_SetBit(flag);
  Event_Wait(20);
}

void Func_020021b4(void);
void Func_02003c28(void);
void Func_02003c30();
void Func_02003cf0();

void Func_020027e0(void) {
  struct Actor *actor;

  actor = Actor_GetById(25);
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  Func_080d4568()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(616), -1, PIXELS(280), 1);
  Func_02003cf0();
  Actor_SetAngleToward(25, gMainRoutineWork.currentOwner, 0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Func_080d47b4(25, 0x100, 40);
  Actor_SetSpritePriority(25, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 632, 280);
  Actor_SetSpeed(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 664, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(21, 25, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 696, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 712, 280);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(712), -1, PIXELS(136), 1);
  Map_CopyCellPictureIndicesRect(112, 6, 110, 4, 1, 1);
  Actor_SetSpritePriority(25, 1);
  Actor_WalkToAndWait(25, 712, 200);
  Actor_SetPositionAndReset(25, 712, 168);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_ArmCallbackAndRefresh(25, FACING_EAST);
  Actor_SetSpeedParameters(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 168);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(24, 25, 0x216);
  Actor_ArmCallbackAndRefresh(25, FACING_NORTH);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 136);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_WalkToAndWait(25, 744, 72);
  Map_CopyCellPictureIndicesRect(66, 51, 110, 4, 1, 1);
  Sound_Play(229);
  Func_020021b4();
  Event_Wait(10);
  Actor_Jump(25, 6, 16);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Event_Wait(20);
  GameFlag_SetBit(0x870);
  Func_02003c30();
}

void Func_02002bcc(void);

void Func_02002a5c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;
  s32 angle;

  Func_02003c28();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(temporary): the angle named before the call, so it is built
   * before the two coordinates; as the literal argument FACING_NORTH, 4 of
   * 102 instructions differ in tla-en. */
  angle = FACING_NORTH;
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.currentOwner, PIXELS(248), PIXELS(136), angle);
  Map_CopyCellPictureIndicesRect(65, 64, 15, 8, 1, 1);
  GameFlag_SetBit(0x245);
  Sound_Play(161);
  Func_020021b4();
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(9, 0x100, 20);
  Func_02002bcc();
  Event_Wait(20);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  Event_Wait(16);
  for (i = 0; i < 40; i++) {
    if (i == 5)
      Sound_Play(204);
    leader->scaleX -= 0x400;
    leader->scaleY -= 0x300;
    leader->y -= 0x9999;
    wait(1);
  }
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(23);
}

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};

void Func_02002b68(void) {
  if (((struct EffectWork7c4 *)Ram_HeapSlots->effectWork)->active != 0) {
    Sound_Play(229);
    Map_CopyCellPictureIndicesRect(64, 64, 15, 8, 1, 1);
    Map_CopyCellPictureIndicesRect(64, 66, 15, 72, 1, 2);
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
  }
}
