#include "types.h"
#include "call.h"
#include "field_coordinate.h"
#include "field_sprite.h"

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

struct PositionedActor *Actor_GetById(s32 id);
void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);
void Func_02001c10(s32 id, s32 a1, s32 a2);
s32 GameFlag_Test(s32 flag);

void Func_02000a88(void) {
  struct PositionedActor *actor9 = Actor_GetById(9);
  struct PositionedActor *actor10 = Actor_GetById(10);
  struct PositionedActor *actor23 = Actor_GetById(23);
  struct FieldSprite *sprite = actor23->animation;

  Call3((void (*)())Func_02001c10, 24, 0x2000000, 0xf80000);
  Call3((void (*)())Func_02001c10, 23, 0x2000000, 0xf80000);
  Call3((void (*)())Func_02001c10, 9, 0x1d80000, 0x1000000);
  Call3((void (*)())Func_02001c10, 10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (GameFlag_Test(SPLIT_CONSTANT(0xa0a))) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_GetById(9), 4);
    Animation_ApplyChildValues(Actor_GetById(11), 4);
  }
  if (Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa0b))) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_GetById(10), 4);
    Animation_ApplyChildValues(Actor_GetById(12), 4);
  }
  if (GameFlag_Test(SPLIT_CONSTANT(0xa0a)) && GameFlag_Test(SPLIT_CONSTANT(0xa0b))) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}
