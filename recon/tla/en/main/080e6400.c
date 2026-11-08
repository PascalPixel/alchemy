// Matches: differ 0 (tla-en, 2026-11-08)
// All six editions build the same 1,224 bytes but for the calls' offsets
// (checked by assembling the draft per edition against each ROM's copy:
// tla-ja 080e63bc, tla-de 080f3400, tla-es 080ef3f4, tla-fr 080eb3f4, tla-it
// 080e63f4, each call landing on that edition's callee); the diff tool cannot
// measure those scaffold sections itself (no Thumb marker).
// One tagged device: FAKEMATCH(barrier) on `phase`. Review also the copy's
// return type, which is untagged as the routine's true type.
// How the two sched2 ties closed (-fsched-verbose=6 on the sched2 dump):
// - r1 = 32 before r0 = picture at the copy: the two moves tie on priority
//   and dependents, so RTL order put r0 first. With the copy typed as
//   returning a value (call_value sets r0), the later r0 = &pictureSprite
//   depends on the call instead of on r0 = picture, which drops to two
//   dependents and loses to r1. copy_strided.s does return the source's end
//   in r0 on every path; the other three call sites (f021a84.c, f0e4a34.c,
//   f0e92b4.c) compile the same with either type.
// - the 0x06010000 pool load before `movs r4, #200`: both have priority 20
//   (each reaches `adds r3, r7, r4` at cost 3), and after the tile ldrsh,
//   which reads r3, the pool load is anti-dependent (class 2) and the movs
//   independent (class 3). No C reshapes that: the reload round-robin
//   (0x400 in r0, 0x192e r1, scratch r2, pool r3, 0x192a r4, scratch r0)
//   shows the native's order is picture, tile, vram, timer, as here, and a
//   once-only block at the tile load would pull the picture ahead of it. The
//   asm on `phase` (r8, untouched until the re-test) is a no-code insn with
//   no unit, so it issues in the ldrsh's stall; after it both candidates are
//   class 3 and the pool load wins on three dependents to two. Without it,
//   2 differ. An asm on the pool constant instead lowers its priority (a link
//   into an asm costs nothing): 2. Milder kinds cannot add a no-code insn.
// Earlier rounds (differ 4): vram before picture (24), the copy size in one
// local (4), vram after the ripple (99); vram as a shift, an integer sum or
// two steps, split, reversed, indexed or from a constant local, typed copy
// parameters, 14 + timer, the picture as work->picture or void *, the copy
// through a local (all 4); once-only blocks (12, 14, 22 with the tile address
// alone inside); the ripple phase read first (124); 34 permuter minutes.
// The frame callback of Func_080e68d0's effect, a state machine on its phase
// (0x1928) and timer (0x192a) in one of four modes (0x1938). Mode 0: the
// second object appears full width, the first takes mode 3 and rises for 16
// frames, then a picture rippled each frame (Func_080e62f4) is copied to
// sprite VRAM and drawn while 18 particles grow
// (BattleEffect_UpdateGrowingParticleSprite) until frame 50; mode 3 does the
// same with 64 rising, jittering sparks until frame 90; modes 1 and 2 only
// raise the first object (mode 4) for 16 frames, mode 2 with 20 growing
// particles. The phase parks at 999 when done.
// The shape that set the registers: one `sprite` variable, first the picture's
// sprite and then `&work->sprites[i]` in modes 0 and 3, so loop strength
// reduction keeps the copy at the top of each pass (sprite = r0/r8 in r5) and
// the work stays in r7; mode 2 steps both pointers and counts up, and GCC
// reverses its counter to 19..0.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"

struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct RiseWork {
  u8 tiles[0x400];
  u8 picture[0x400];
  struct EffectSprite sprites[64];
  struct EffectParticle particles[64];
  struct EffectSprite pictureSprite;
  s16 phase;
  s16 timer;
  s16 particleTile;
  s16 pictureTile;
  struct Actor *object;
  struct Actor *other;
  s16 mode;
};

/// Render_CopyStrided as the heap holds it: copies rows 8 bytes at a time and
/// returns the end of the source.
typedef const u8 *(*CopyStridedFn)(const u8 *source, s32 stride, s32 rows,
                                   void *destination);

void Func_080e62f4(u8 *source, u8 *destination, s32 phase);
void BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite *sprite,
                                              struct EffectParticle *particle,
                                              s32 tiles);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct EffectParticle *particle);
u32 Random16(void);

#define RISE_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e6400(void) {
  struct RiseWork *work;
  struct Actor *object;
  struct EffectSprite *sprite;
  struct EffectParticle *particle;
  u8 *vram;
  u8 *picture;
  s16 *phase;
  s32 jitter;
  s32 i;

  work = (struct RiseWork *)Ram_HeapSlots->battleFxWork[0];
  object = work->object;
  if (work->mode == 0) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = 0x10000;
      if (work->timer == 4)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(object, 3);
      if (work->timer <= 15)
        object->y += 0x24ccc;
      if (work->timer == 28)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 25)
        work->other->scaleX = 0;
      if (work->timer == 50)
        work->phase = 999;
      break;
    }
    phase = &work->phase;
    if (*phase == 2) {
      /* FAKEMATCH(barrier): an empty asm on phase, which issues in the tile
       * load's stall so the vram base's pool load wins its tie with the timer
       * offset; as no asm, 2 of 596 instructions differ in tla-en. */
      asm("" : "+r"(phase));
      picture = work->picture;
      vram = (u8 *)0x06010000 + work->pictureTile * 32;
      Func_080e62f4(work->tiles, picture, work->timer + 14);
      // The heap copy of Render_CopyStrided (copy_strided.s) hands back the
      // source's end in r0; the call is scheduled as one that returns it.
      ((CopyStridedFn)Ram_HeapSlots->unk4c[2])(picture, 32, 32, vram);
      sprite = &work->pictureSprite;
      Func_080eb01c(sprite);
      if (*phase == 2) {
        for (i = 0, particle = work->particles; i < 18; i++, particle++) {
          sprite = &work->sprites[i];
          if (particle->timer >= 0 && particle->timer < 32)
            BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                     work->particleTile);
          particle->timer++;
        }
      }
    }
  } else if (work->mode == 3) {
    switch (work->phase) {
    case 0:
      if (work->timer == 1)
        work->other->scaleX = 0x10000;
      if (work->timer == 4)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 8)
        Actor_SetMode(work->object, 3);
      if (work->timer <= 15)
        object->y += 0x24ccc;
      if (work->timer == 28)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 70)
        work->other->scaleX = 0;
      if (work->timer == 90)
        work->phase = 999;
      break;
    }
    if (work->phase == 2) {
      for (i = 0, particle = work->particles; i < 64; i++, particle++) {
        if (particle->timer >= 0 && particle->timer < 45) {
          sprite = &work->sprites[i];
          EffectSprite_DrawAtWorldPosition(sprite, particle);
          if (particle->timer < 42) {
            particle->y -= 0x10000;
            jitter = Random16();
            particle->x += jitter - (s32)Random16();
          }
        }
        particle->timer++;
      }
    }
  } else if (work->mode == 1) {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->object, 4);
      object->y += 0x24ccc;
      if (work->timer == 15)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 20)
        work->phase = 999;
      break;
    }
  } else {
    switch (work->phase) {
    case 0:
      if (work->timer == 8)
        RISE_NEXT_PHASE(work);
      break;
    case 1:
      if (work->timer == 0)
        Actor_SetMode(work->object, 4);
      object->y += 0x24ccc;
      if (work->timer == 15)
        RISE_NEXT_PHASE(work);
      break;
    case 2:
      if (work->timer == 30)
        work->phase = 999;
      break;
    }
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 20; i++, sprite++, particle++) {
      if (particle->timer >= 0 && particle->timer < 32)
        BattleEffect_UpdateGrowingParticleSprite(sprite, particle,
                                                 work->particleTile);
      particle->timer++;
    }
  }
  work->timer++;
}
