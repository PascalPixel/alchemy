#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "call.h"

struct Actor *Actor_GetById(u32 id);
void Func_0200120c(void);
void Func_02001214(void);
void Func_020012e4(s32 skip);
void Func_020011d4(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void wait(s32 frames);
s32 Math_Cosine(s32 angle);
void Func_020003cc(void);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_ResetAndSetPosition(u32 id, s32 x, s32 z);
void Actor_WaitForMoveById(u32 id);
void Func_020003dc(struct Actor *object, s32 mode);
void Func_0200131c(s32 cue);
void Func_020011f4(s32 a, s32 b, s32 c);
void Func_020001ec(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Actor_SetModeById(u32 id, s32 mode);
void Func_020011fc(void);
void GameFlag_SetBit(s32 flag);

void Func_020003f8(s32 unk0, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 unused[3]; // Unused: gives the stack frame its 12 bytes.
  struct EffectOptions options;
  struct FieldSprite *sprite;
  u32 i;

  if (actor->x >> 20 != 76)
    return;
  Func_0200120c();
  Func_020012e4(0);
  Call6((void (*)())Func_020011d4, 74, 31, 1, 1, 74, 33);
  if (Actor_GetById(15)->x >> 20 == 76) {
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    for (i = 0; i <= 17; i++) {
      wait(1);
      sprite = actor->animation;
      sprite->rotation -= 256;
      actor->x -=
          Math_Cosine(((struct FieldSprite *)actor->animation)->rotation) / 2;
      actor->targetX = 0x80000000;
    }
    actor->update = Func_020003cc;
    Call3((void (*)())Actor_SetSpeedParameters, id, 0x30000, 0x18000);
    Call3((void (*)())Actor_ResetAndSetPosition, id, SPLIT_CONSTANT(0x4a9), SPLIT_CONSTANT(0x212));
    actor->gravity = 0xcccc;
    actor->motionFlags = 3;
    actor->terrainId = 0;
    Actor_WaitForMoveById(id);
    Func_020003dc(actor, 0);
    Func_0200131c(188);
    Call3((void (*)())Func_020011f4, 0x50000, 0x50000, 0x10000);
    Func_0200131c(141);
    Call3((void (*)())Func_020011f4, -1, -1, SPLIT_CONSTANT(0xe666));
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
    Func_0200131c(154);
    Actor_SetModeById(id, 3);
    Func_020011fc();
    Actor_GetById(15)->priorityFlags |= 2;
    GameFlag_SetBit(SPLIT_CONSTANT(0x884));
  }
  Func_02001214();
}
