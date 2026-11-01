// Matches: differ 0 (tla-en, 2026-11-01)
// Each frame, every particle younger than 20 frames picks one of eight sprite
// tiles from its age (a new tile every 4 frames), then is drawn and stepped.
// Forms that decide the match, all plain C with no device:
// - goto loop: keeps 0x1602 and -1 rebuilt in the body.
// - `base` then `mask`: the address's reload is done before 7 is live, so the
//   second spill register is r2, not r0; `mask` also gives the shift chain r3.
// - `tile` narrowed to u16 before `attr` loads, `attr` first in the OR: the
//   u16 bitfield store gives a subreg operand that regmove ties to -1024
//   (differ 36 at best).
// - `step` set outside the loop: gcse copies the constant into the argument,
//   so r2 loads last; written inline it is precomputed and loads first
//   (differ 2).
#include "types.h"
#include "system/ram_buffer.h"

struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 age;
};

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void Func_02003c3c(struct ParticleSprite *sprite, struct Particle *particle);
void Func_02003c44(struct Particle *particle, s32 unk1, s32 step);

void Func_0200379c(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuRuntime;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  s32 step = -0x8000;

  i = 63;
next:
  if ((u32)particle->age <= 19) {
    u16 *base = &work->tile;
    s32 mask = 7;
    s32 tile = (u16)((*base + ((particle->age >> 2) & mask) * 8) & 0x3ff);
    s32 attr = sprite->attr;

    sprite->attr = (attr & ~0x3ff) | tile;
    Func_02003c3c(sprite, particle);
    Func_02003c44(particle, 63, step);
    particle->age++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}
