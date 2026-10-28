// Draft, differ 24 of 199 (was 69): the leader and actor 14: Func_02001340
// lines them up, flag 0x230 is set, a burst of 17 effects plays and actor 14
// sinks. The sinking loop's 0x200 turn is a variable set before it (found by
// a permute search; a temporary device on adoption, though the native still
// builds 0x200 in the loop). Left: register choice: the native keeps the
// leader in r10 and the options' address in r8, here the other way round,
// and forms the velocity's address after the cosine call. A barrier after
// the turn gives 22.
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"
#include "call.h"

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
void Effect_AdvanceMotion(void *object);
void Func_02001340(struct ShrineOfTheSeaGodObject *object,
                   struct ShrineOfTheSeaGodObject *other);
void Func_02001464(void);
void Func_020039b8(s32 a, s32 b, s32 c);
void Func_020039c0(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Task_RemoveFar(void (*func)(void));

void Func_02001474(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *stone = Actor_GetById(14);
  struct ShrineOfTheSeaGodSprite *sprite = stone->sprite;
  struct EffectOptions options;
  u32 i;
  s32 spin;

  Func_020039d8();
  Func_02003b08(0);
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
  Func_02003b48(227);
  Call3((void (*)())Func_020039b8, 0x10000, 0x20000, 0x10000);
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
  Call3((void (*)())Func_020039b8, -1, -1, 0xe666);
  Task_RemoveFar(Func_02001464);
  spin = 0x200;
  for (i = 0; i <= 7;) {
    sprite->rotation += spin;
    stone->z += 0x10000;
    i++;
    wait(1);
  }
  Func_020039c0();
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
  leader->layer = 0;
  wait(10);
  Func_020039e0();
}
