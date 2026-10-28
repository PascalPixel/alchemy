// Draft, differ 103 of 112 (111 before statements reordered 2026-10-26): register allocation differs throughout. The
// native keeps the position array's address in r7 and the group counter
// on the stack, computes each group's sprites as group * 80 inside the loop
// and spills the inner counter around every call (r2 via sp 0); this keeps
// the inner counter in r7 and the group's offset as a stack giv. The age
// tests are two compares in the native; written as one range test GCC folds
// them to an unsigned compare (differ 116), so the negative age continues
// apart here.
// BattleEffect_UpdateArcEffect, the arc effect's task (heap
// slot 0xf8, arc_effect.h). For 32 frames after a group starts, its two arc
// sprites rise from the target object by a pixel a frame and swell and shrink
// with the sine of the age; the second is drawn a pixel lower, flipped
// upside down, behind the target where the first is in front of it.
#include "types.h"
#include "fixed_math.h"
#include "object_runtime.h"
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
  struct ObjectRuntime *target;
};

void Camera_WorldToScreen(s32 *position);
s32 BattleEffect_GetObjectDepth(struct ObjectRuntime *object);
void Func_080eb01c(struct ArcSprite *sprite);

void BattleEffect_UpdateArcEffect(void) {
  s32 group;
  struct ArcWork *work = Ram_HeapSlots->block62;
  struct ObjectRuntime *target = work->target;
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
      y = position[2];
      x = position[0];
      sprite = work->sprites[group];
      for (i = 0; i < 2; i++, sprite++) {
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
