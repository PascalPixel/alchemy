#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "event_inline.h"

void Func_0200120c(void);
void Func_02001214(void);
s32 Math_Cosine(s32 angle);
void Func_020003cc(void);
void Func_020003dc(struct Actor *object, s32 mode);
void Func_020001ec(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_020003f8(s32 unk0, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 unused[3]; // Unused: gives the stack frame its 12 bytes.
  struct EffectOptions options;
  struct FieldSprite *sprite;
  u32 i;

  if (actor->x >> 20 != 76)
    return;
  Func_0200120c();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
  if (Actor_GetById(15)->x >> 20 == 76) {
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    for (i = 0; i <= 17; i++) {
      System_Wait(1);
      sprite = actor->animation;
      sprite->rotation -= 256;
      actor->x -=
          Math_Cosine(((struct FieldSprite *)actor->animation)->rotation) / 2;
      actor->targetX = 0x80000000;
    }
    actor->update = Func_020003cc;
    Actor_SetSpeed(id, 0x30000, 0x18000);
    Actor_SetDestination(id, 1193, 530);
    actor->gravity = 0xcccc;
    actor->motionFlags = 3;
    actor->terrainId = 0;
    FieldActor_WaitForMoveById(id);
    Func_020003dc(actor, 0);
    Event_PlaySound(188);
    MapGrid_SetShake(0x50000, 0x50000, 0x10000);
    Event_PlaySound(141);
    MapGrid_SetShake(-1, -1, 0xe666);
    ((struct FieldSprite *)actor->animation)->rotation = 0x1000;
    // Cleared through a word: as a function pointer the store sinks below the
    // effect options.
    *(u32 *)&actor->update = 0;
    options.type = 276;
    options.startScaleX = 0x8000;
    options.targetScaleX = 0x18000;
    options.targetScaleY = 0x13333;
    options.startScaleY = 0xcccc;
    Func_020001ec(actor->x, actor->y, actor->z, 0, 0, 0,
                  EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                      EFFECT_SCALE_TO_TARGET,
                  &options);
    Event_PlaySound(154);
    Actor_SetAnimation(id, 3);
    MapGrid_WaitForShakeToSettle();
    Actor_GetById(15)->priorityFlags |= 2;
    Event_SetFlag(0x884);
  }
  Func_02001214();
}
