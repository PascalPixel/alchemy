// Draft, differ 140 of 159: a charge-up step for the controlled character in
// Taopo Swamp: holding the direction pad raises gSceneState's charge
// (faster with B held), shown in the sprite's byte 0x11; at 0x1400 it
// releases two objects of type 269 (Func_02002808) 15 frames apart, waits,
// puts the character back at the saved place and clears the charge. Left: registers from the start (the native keeps gSceneState in r9, the actor in r8 and the release count in r11) and the charge's arithmetic order.
#include "types.h"
#include "object_runtime.h"
#include "input.h"

extern u8 gSceneState[];
extern u8 gData_02002ae8[];
s32 Func_02002998(void);
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02002898(void);
void Func_02002968(s32 mode);
void Func_020028a0(void);
struct ObjectRuntime *Func_02002808(s32 type, s32 x, s32 y, s32 z);
void Func_02002800(struct ObjectRuntime *object, const u8 *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);

void Func_02002144(void) {
  u8 *state = gSceneState;
  struct ObjectRuntime *actor = Object_GetById(Func_02002998());
  u8 *sprite = actor->animation;
  struct ObjectRuntime *spark;
  u8 *sparkSprite, *kind;
  s32 charge, step, i;

  if (*(u8 **)(sprite + 44) != 0)
    (*(u8 **)(sprite + 44))[6] = 1;
  sprite[26] = (sprite[26] | 8) & ~1;
  sprite[25] = 1;
  if (gInput.held & 0xf0) {
    step = 0x1400;
    if (gInput.held & 2)
      step = step * 3 >> 1;
    *(u16 *)(state + 2) += step / 96 + 1;
  }
  charge = *(s16 *)(state + 2);
  sprite[17] = (sprite[17] & 3) | ((charge / 256) << 2);
  *(u16 *)state = 1;
  if (charge < 0x1400)
    return;
  Func_02002898();
  Func_02002968(0);
  kind = &actor->animationKind;
  *kind = 0;
  for (i = 1; i >= 0; i--) {
    spark = Func_02002808(269, actor->x, actor->y, actor->z);
    if (spark != 0) {
      sparkSprite = spark->animation;
      Func_02002800(spark, gData_02002ae8);
      spark->motionFlags = 0;
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite[26] = 0;
        sparkSprite[9] = (sparkSprite[9] & ~12) | 8;
      }
    }
    WaitFrames(15);
  }
  FieldEvent_WaitFrames(30);
  *kind = 1;
  sprite[17] &= 3;
  sprite[26] = 0;
  Func_020028a0();
  actor->x = *(s32 *)(state + 4);
  actor->z = *(s32 *)(state + 8);
  *(u16 *)state = 0;
  *(u16 *)(state + 2) = 0;
}
