// Near miss, differ 28 of 124, instruction order only: the prologue's
// constants, the 0x300000 magnitude's shift, the delay's reload around
// the spin store and the end's two offsets; the literal pool also lands one
// instruction earlier in the native (before the sprite's y store). The spin
// as a conditional expression measured 28, as if/else 35.
// The 96-particle burst's task (heap slot 0x5c work,
// started by Func_080e2acc). Each particle whose delay counter is between 0
// and 63 shows the picture (delay / 8) % 6 of the burst's tiles 48 units out
// along its heading, then turns by its spin, rises by its speed and spins
// eight units faster; every counter advances. The work's flash flag is set
// on frame 30 only, and the burst stops being active on frame 112.
#include "types.h"
#include "ram_buffer.h"

struct BurstSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 unk08_10 : 6;
  u8 unk0a[2];
  s32 x;
  s32 y;
  u8 unk14[20];
};

struct BurstParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 heading;
  s32 speed;
  s32 spin;
  s32 delay;
};

struct BurstWork {
  u8 tiles[384];
  struct BurstSprite sprites[96];
  struct BurstParticle particles[96];
  s16 resource;
  s16 vram;
  s16 flash;
  s16 frame;
  s16 active;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);
void Func_080eb01c(struct BurstSprite *sprite);

void Func_080e2c38(void) {
  struct BurstWork *work = (struct BurstWork *)Ram_HeapSlots->battleFxWork[0];
  struct BurstParticle *particle = work->particles;
  struct BurstSprite *sprite = work->sprites;
  s32 position[3];
  s32 i;

  for (i = 0; i < 96; i++, sprite++, particle++) {
    if ((u32)particle->delay < 64) {
      sprite->tile = work->vram + (particle->delay >> 3) % 6 * 2;
      position[0] = particle->x;
      position[1] = particle->y;
      position[2] = particle->z;
      Vector_AddPolarOffset(0x300000, particle->heading, position);
      Camera_WorldToScreen(position);
      sprite->x = position[0];
      sprite->y = position[2];
      Func_080eb01c(sprite);
      particle->heading += particle->spin;
      particle->y += particle->speed;
      particle->spin =
          particle->spin >= 0 ? particle->spin + 8 : particle->spin - 8;
    }
    particle->delay++;
  }
  work->flash = 0;
  if (work->frame == 30)
    work->flash = 1;
  if (work->frame == 112)
    work->active = 0;
  work->frame++;
}
