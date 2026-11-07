#include "types.h"
#include "fixed_math.h"
#include "actor.h"
#include "ram_buffer.h"

struct ArcSprite {
  u8 unk00[12];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  s16 depth;
  u8 unk20[8];
};

struct ArcWork {
  struct ArcSprite sprites[2][2];
  s16 age[2];
  s16 resource;
  s16 group;
  struct Actor *target;
};

void Camera_WorldToScreen(s32 *position);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
void Func_080eb01c(struct ArcSprite *sprite);

void BattleEffect_UpdateArcEffect(void) {
  struct ArcWork *work = Ram_HeapSlots->block62;
  struct Actor *target = work->target;
  s32 group;
  s32 position[3];
  s32 i;

  for (group = 0; group < 2; group++) {
    s32 scale = Trig_Sin(work->age[group] << 10);

    if (work->age[group] < 0)
      continue;
    if (work->age[group] < 32) {
      struct ArcSprite *sprite;
      s32 x, y;

      position[0] = target->x;
      position[1] = target->y + (work->age[group] << 16);
      position[2] = target->z;
      Camera_WorldToScreen(position);
      x = position[0];
      y = position[2];
      for (i = 0; i < 2; i++) {
        sprite = &work->sprites[group][i];
        sprite->x = x;
        sprite->y = y;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        if (i == 0) {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) - 1;
        } else {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) + 1;
          sprite->y += 0xffff0000;
          sprite->scaleY = -sprite->scaleY;
        }
        Func_080eb01c(sprite);
      }
      work->age[group]++;
    }
  }
}
