/// @file
/// Flashes an actor, then raises eight puffs over another and sets a flag.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "fixed_math.h"

void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
u32 Random16Far(void);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
s32 Func_02003d80(void *sprite, s32 frame);
void Func_02003df8(s32 cue);
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

  Func_02003df8(125);
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
    object = Func_02003bb0(285, (i << 17) + leader->x - Q16_ONE * 8,
                           y - Q16_ONE * 2, leader->z);
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
  FieldEvent_Wait(20);
}
