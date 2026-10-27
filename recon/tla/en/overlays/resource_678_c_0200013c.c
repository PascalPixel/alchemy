// Draft, differ 149 of 196: when actor `id` stands at x 12, inside an event: the
// actor drops in place, then for 25 frame steps of three frames charges up
// (its sprite's byte 0x11) and every fourth step throws up a splash of type
// 14 at a random spot within 2 pixels (Func_02002808), then releases two
// objects of type 269 15 frames apart as Func_02002144 does, settles
// (Func_020028d0) and sets flag 0x8e5. Left: the native keeps the step in r9, the sprite in r10 and the id in r11, takes the splash's priority through a bitfield shift pair and lays the loops out with their tests at the bottom; the second loop matches Func_02002144's form.
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern u8 gData_02002ae8[];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02002898(void);
void Func_02002968(s32 mode);
void Func_020028a0(void);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
u16 Random16Far(void);
struct ObjectRuntime *Func_02002808(s32 type, s32 x, s32 y, s32 z);
void Func_02002800(struct ObjectRuntime *object, const u8 *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void Animation_ApplyChildValuesToRecord(void *sprite, s32 value);
void Animation_ApplyChildValue(void *sprite, s32 value);
void Func_020028d0(s32 id, s32 a, s32 b);
void GameFlag_SetBit(s32 flag);

void Func_0200013c(s32 unused, s32 id) {
  struct ObjectRuntime *actor = Object_GetById(id);
  struct ObjectRuntime *splash;
  struct FieldSprite *sprite, *splashSprite;
  u8 *raw, *kind;
  s32 i, x, z, a, b;

  if (actor->x >> 20 != 12)
    return;
  Func_02002898();
  Func_02002968(0);
  actor->motionFlags = 3;
  WaitFrames(5);
  actor->motionFlags = 0;
  sprite = actor->animation;
  raw = (u8 *)sprite;
  actor->velocityY = 0;
  actor->y = 0x40000;
  for (i = 0; i <= 24; i++) {
    raw[17] = (raw[17] & 3) | (i << 2);
    raw[26] = (raw[26] | 8) & ~1;
    raw[25] = 1;
    if ((i & 3) == 0) {
      a = Random16Far();
      b = Random16Far();
      x = actor->x + a * 2 - b * 2;
      a = Random16Far();
      b = Random16Far();
      z = actor->z + a * 2 - b * 2;
      splash = Func_02002808(14, x, actor->y, z);
      if (splash != 0) {
        splashSprite = splash->animation;
        splash->terrainHeight = actor->terrainHeight;
        Func_02002800(splash, gData_02002ae8);
        splash->motionFlags = 0;
        if (splashSprite != 0) {
          Animation_ApplyChildArgument(splashSprite, 1);
          ((u8 *)splashSprite)[26] = 0;
          splashSprite->priority = sprite->priority;
          Animation_ApplyChildValuesToRecord(splashSprite, 13);
          Animation_ApplyChildValue(splashSprite, 8);
        }
      }
    }
    WaitFrames(3);
  }
  kind = &actor->animationKind;
  *kind = 0;
  actor->y = 0;
  for (i = 1; i >= 0; i--) {
    splash = Func_02002808(269, actor->x, actor->y, actor->z);
    if (splash != 0) {
      splashSprite = splash->animation;
      Func_02002800(splash, gData_02002ae8);
      splash->motionFlags = 0;
      splash->terrainId = 1;
      if (splashSprite != 0) {
        Animation_ApplyChildArgument(splashSprite, 2);
        ((u8 *)splashSprite)[26] = 0;
        splashSprite->priority = 2;
      }
    }
    WaitFrames(15);
  }
  FieldEvent_WaitFrames(30);
  *kind = 1;
  Func_020028d0(id, 0, 0);
  Func_020028a0();
  GameFlag_SetBit(0x8e5);
}
