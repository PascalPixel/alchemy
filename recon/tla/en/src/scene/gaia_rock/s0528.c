#include "types.h"
#include "call.h"

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 priorityFlags;
  u8 unk24[4];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

u32 Random16Far(void);
void Func_02000e5c(struct OrbitEffect *effect, s32 priority);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
struct OrbitEffect *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_02000474(struct OrbitEffect *effect);

s32 Func_02000528(struct OrbitEffect *source) {
  s32 i;
  s32 timer;

  i = 0;
  timer = 20;
loop:
  {
    struct OrbitEffect *effect;
    s32 radius;
    s32 position[3];

    position[0] = source->x;
    position[1] = source->y + i * SPLIT_CONSTANT(0xcc00, 204) + 0x40000;
    position[2] = source->z;
    radius = Random16Far();
    Vector_AddPolarOffsetFar(radius * 5 + 0x30000, Random16Far(), position);
    effect = (struct OrbitEffect *)Value4(
        (s32 (*)())Actor_Spawn, 308, position[0], position[1], position[2]);
    if (effect != NULL) {
      effect->update = Func_02000474;
      effect->timer = timer;
      effect->pause = 0;
      effect->speed = SPLIT_CONSTANT(0x1900, 153);
      effect->scale = 0x30000;
      effect->angle = Random16Far();
      effect->priorityFlags = 0;
      Func_02000e5c(effect, 2);
    }
  }
  timer -= 2;
  i++;
  if (i < 16)
    goto loop;
  return 0;
}
