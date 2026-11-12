#include "tundaria_tower.h"
#include "call.h"
#include "scene_tables.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"

void Func_020006d4(union FieldObject *object) {
  object->effect.x += object->effect.velocity.x;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  object->effect.velocity.y -= 0x4ccc;
}

// Scatters count sparks (effect type 279) around a position: each starts at
// the given scale, shrinks toward 0xcccc, turns to a random sixteenth of a
// turn and flies off with a random velocity through the overlay's effect
// spawner, falling under Func_020006d4.

void Func_02000708(struct TundariaTowerPosition *position, s32 count,
                   s32 scale) {
  struct EffectOptions options;
  s32 i;
  s32 velocityX;
  s32 velocityY;
  s32 x;

  options.priority = 1;
  options.type = 279;
  options.targetScaleY = 0xcccc;
  options.targetScaleX = 0xcccc;
  options.startScaleY = scale;
  options.startScaleX = scale;
  options.update = Func_020006d4;
  for (i = 0; i != count; i++) {
    s32 mask;

    options.rotation = (Random16Far() & 15) << 12;
    velocityX = Random16Far();
    // One constant for the loop's life: the listing keeps 15 in a register.
    mask = 15;
    velocityX &= mask;
    velocityX -= 8;
    velocityX <<= 14;
    velocityY = Random16Far() & mask;
    velocityY += 8;
    velocityY <<= 14;
    x = position->x - ((Random16Far() & 31) << 16) + 0xf0000;
    // The overlay's spawner is declared with this scene's view of the options.
    Func_020000b8(x, position->y + ((Random16Far() & mask) << 16), position->z,
                  velocityX, velocityY, 0, 0x15e0000,
                  (struct TundariaTowerEffectParams *)&options);
  }
}

void Func_020007dc(s32 step) {
  struct TundariaTowerPosition position;

  switch (step) {
  case 1:
    break;
  case 2:
    position.x = 0x15b0000;
    position.y = 0;
    position.z = 0x1780000;
    Func_02000708(&position, 1, 0xcccc);
    break;
  case 3:
    Map_CopyCellPictureIndicesRect(31, 85, 20, 85, 3, 4);
    position.x = 0x15b0000;
    position.y = 0;
    position.z = 0x1780000;
    Func_02000708(&position, 8, 0x18000);
    break;
  }
}

void Func_02000840(struct Actor *object) {
  struct TundariaTowerEffectParams params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      /* FAKEMATCH(temporary): a local holding the mask builds it before the
       * random draw; as Random16Far() & 15, 7 of 46 instructions differ in
       * tla-en. */
      s32 mask = 15;

      dx = mask & Random16Far();
    }
    dy = Random16Far() & 7;
    params.kind = 673;
    Func_020000b8(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16),
                  object->z, 0, 0x8000, phase, 0x100000, &params);
  }
}

void Func_020008a8(void) {
  struct Actor *object = Actor_GetById(8);

  Func_02003048();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x944);
  Call6((void (*)())Map_CopyCellAttributeRect, 11, 22, 3, 2, 20, 22);
  object->gravity = 0x8000;
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(68, PIXELS(344), PIXELS(372));
  Actor_GetById(68)->update = (void (*)(void))Func_02000840;
  Func_02003050();
}

void Func_02000920(void) {
  Func_02003048();
  Event_ResolvePendingActions(0);
  FieldMessage_Show(MsgThereSSomething, 1);
  if (GameFlag_Test(0x949)) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->unk15a = 1;
  }
  Func_02003050();
}

void Func_0200189c(void);

void Func_0200095c(void) { Func_0200189c(); }

extern const u8 gData_02003370[];

void Func_02002f90(struct Actor *object, const void *script);

/// Actor 8 is placed and given its script while the screen flashes, and three
/// map cells change.
void Func_02000964(void) {
  struct Actor *object = Actor_GetById(8);

  Func_02003048();
  Event_ResolvePendingActions(0);
  TundariaTower_FadeFrom(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(2);
  Sound_Play(132);
  TundariaTower_SetWork(0x50000, 0x70000, 0x10000);
  TundariaTower_SetWork(-1, -1, 0xe666);
  Event_Wait(2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(2);
  object->actionFlags = 0;
  object->update = 0;
  TundariaTower_ObjPlace(8, 424, 536);
  Func_02002f90(object, gData_02003370);
  Actor_WaitForMoveById(8);
  GameFlag_SetBit(0x200);
  Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
  MapCell_SetAttributes(0, 21, 33, 0x3200);
  MapCell_SetAttributes(0, 22, 33, 0x3200);
  MapCell_SetAttributes(0, 26, 33, 0xff00);
  Func_02003050();
}

void Func_02000338(struct Actor *object);

void Func_02000a48(s32 arg0, s32 arg1) {
  struct EffectOptions options;
  s16 *effectWork = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(effectWork[13]);
  s32 velocityX;

  if (arg1 < 8) {
    options.type = 279;
    options.startScaleY = 0x10000;
    options.startScaleX = 0x10000;
    options.priority = 0;
    options.targetScaleY = 0x8000;
    options.targetScaleX = 0x8000;
    options.rotation = (Random16Far() & 15) << 12;
    velocityX = ((Random16Far() & 15) - 8) << 13;
    Func_020000b8(object->x, object->y - ((arg1 * 3) << 16) + 0x100000,
                  object->z, velocityX, 0, 0, 0x5a0000, &options);
    object->scaleY -= 7936;
  }
  if (arg0 == 1) {
    object->scaleY = 0x10000;
    GameFlag_SetBit(object->action);
    Func_02000338(Actor_GetById(effectWork[13]));
    Map_CopyCellPictureIndicesRect(67, 23, (object->x >> 20) + 64,
                                   object->z >> 20, 1, 1);
    object->z = 0;
    object->y = 0;
    object->x = 0;
  }
}
