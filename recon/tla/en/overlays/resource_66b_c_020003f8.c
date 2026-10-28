// Draft, differ 63 of 169 (was 81): when actor `id` stands at x 76 and actor
// 15 has reached x 76 too, inside an event: the actor rocks over for 18
// frames, is thrown down to (1193, 530) with the screen shaking (cues 188 and
// 141), lands in a cloud (type 276, Func_020001ec), plays cue 154 and mode 3;
// actor 15 is made to stand in front and flag 0x884 is set. The calls whose
// constants the native builds afresh go through CallN. Left: the native keeps
// the sprite's rotation address and 0xcccc in high registers, reuses the zero
// in r5 for the cleared update and orders the options' stores otherwise.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "lib/call.h"

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

void Func_020003f8(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
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
      sprite->rotation += -256;
      actor->x -= Math_Cosine(((struct FieldSprite *)actor->animation)->rotation) / 2;
      actor->targetX = 0x80000000;
    }
    actor->update = Func_020003cc;
    Actor_SetSpeedParameters(id, 0x30000, 0x18000);
    Actor_ResetAndSetPosition(id, 1193, 530);
    actor->gravity = 0xcccc;
    actor->motionFlags = 3;
    actor->terrainId = 0;
    Actor_WaitForMoveById(id);
    Func_020003dc(actor, 0);
    Func_0200131c(188);
    Call3((void (*)())Func_020011f4, 0x50000, 0x50000, 0x10000);
    Func_0200131c(141);
    Call3((void (*)())Func_020011f4, -1, -1, 0xe666);
    ((struct FieldSprite *)actor->animation)->rotation = 0x1000;
    options.type = 276;
    actor->update = 0;
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
    GameFlag_SetBit(0x884);
  }
  Func_02001214();
}
