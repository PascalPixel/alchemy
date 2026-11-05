#include "types.h"
#include "ram_buffer.h"
#include "effect_sprite.h"

// One mote: a position chasing its target (Motion_StepTowardTarget's struct
// Chase) and a frame count, negative before it starts.
struct Mote {
  s32 x;
  s32 z;
  s32 unk08;
  s32 targetX;
  s32 targetZ;
  s32 unk14;
  s32 timer;
};

struct MoteWork {
  u8 unk0000[0x800];
  struct Mote sparks[32];
  struct EffectSprite sparkSprites[32];
  struct Mote motes[48];
  struct EffectSprite moteSprites[48];
  s32 originX;
  s32 originZ;
  s32 angle;
  s32 radius;
  s32 spacing;
  s8 state;
  s8 moving;
  u16 sparkTile;
  s32 targetX;
  s32 targetZ;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void Motion_StepTowardTarget(struct Mote *mote, s32 speed);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct Mote *mote);
void BattleEffect_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);

void Func_080e9f68(void) {
  struct MoteWork *work = Ram_HeapSlots->battleFxWork[0];
  s32 position[3];
  struct Mote *mote;
  struct EffectSprite *sprite;
  s32 i;
  u16 *tile;

  if (work->moving != 0) {
    position[0] = work->originX;
    position[2] = work->originZ;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    mote = work->motes;
    mote->targetX = position[0];
    mote->targetZ = position[2];
    work->angle += 0x600;
    for (i = 0; i < 48; i++, mote++) {
      if (mote->timer >= 0) {
        if (i != 0) {
          mote->targetX = position[0];
          mote->targetZ = position[2];
        }
        position[0] = mote->x;
        position[2] = mote->z;
        Vector_AddPolarOffset(work->spacing, 0, position);
      }
      mote->timer++;
    }
  }

  mote = work->motes;
  sprite = work->moteSprites;
  for (i = 47; i >= 0; i--, sprite++, mote++) {
    if (work->state == 0) {
      if (mote->timer >= 0) {
        Motion_StepTowardTarget(mote, 0x40000);
        sprite->x = mote->x;
        sprite->y = mote->z;
        Func_080eb01c(sprite);
      }
      mote->timer++;
    } else if (work->state == 2) {
      Motion_StepTowardTarget(mote, 0x60000);
      sprite->x = mote->x;
      sprite->y = mote->z;
      Func_080eb01c(sprite);
    } else if (work->state == 3) {
      if (mote->x != work->targetX || mote->z != work->targetZ) {
        Motion_StepTowardTarget(mote, 0x60000);
        sprite->x = mote->x;
        sprite->y = mote->z;
        Func_080eb01c(sprite);
      }
    }
  }

  mote = work->sparks;
  tile = &work->sparkTile;
  sprite = work->sparkSprites;
  for (i = 31; i >= 0; i--, sprite++, mote++) {
    if ((u32)mote->timer < 20) {
      sprite->oam.index = *tile + (mote->timer / 5 & 7) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, mote);
      BattleEffect_IntegrateVector3(&mote->x, 63, -0x8000);
      mote->timer++;
    }
  }
}
