/// @file
/// The fifth part of Func_02001738's group: follows its owner like the others, throws effect 403 every eighth frame and marks the ground beneath.
#include "actor.h"
#include "system/field_effect.h"
#include "system/main_routine_work.h"
#include "types.h"

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[4];
  void (*unk24)(void);
};

extern u32 gFrameCount;
extern u32 gData_02003d50[];
void Func_020011b8(void);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);

union Position {
  s32 word;
  s16 half[2];
};
#define EFFECT_Y(object) (((union Position *)&(object)->effect.y)->word)

void Func_020015e4(union FieldObject *object) {
  struct Actor *watched = (struct Actor *)object->effect.countdown;
  struct MagmaRockEffectConfig config;
  s32 step;
  s32 mask;

  if (watched->action >= 35 && watched->action <= 42) {
    EFFECT_Y(object) = watched->y + 0x180000;
    EFFECT_Y(object) = EFFECT_Y(object) + (watched->action << 19) + (-34 << 19);
  } else {
    if (watched->action >= 9 && watched->action <= 331) {
      EFFECT_Y(object) = watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
    } else if (watched->action > 332 &&
               watched->action < (object->effect.unk5a[8] << 1) + 333) {
      EFFECT_Y(object) = watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
      EFFECT_Y(object) -= (watched->action << 19) - (332 << 19);
    } else if ((object->effect.unk5a[8] << 1) + 333 < watched->action) {
      object->effect.y = object->actor.terrainHeight;
      object->effect.update = 0;
    }
  }

  mask = 7;
  step = gFrameCount & mask;
  if (step == 0) {
    config.id = 403;
    config.variant = 10;
    config.kind = 2;
    config.unk24 = Func_020011b8;
    config.data = gData_02003d50;
    Func_0200015c(object->effect.x + 0x10000, object->effect.y - 0x40000,
                  object->effect.z, ((Random16Far() & mask) - 3) << 14,
                  -0x20000, step, 0x1330000, &config);
  }
  Func_020005cc(watched->terrainId, watched->x >> 20, watched->z >> 20,
                (object->effect.y >> 19) + 1);
}
