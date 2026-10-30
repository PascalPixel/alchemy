// Near miss, differ 2 of 106 (tla-en, 2026-10-30), with no device: starts the
// next of the 96 sparks at a point around (x, y, z), in a random direction and
// speed. The counter's step and wrap are two statements (as one, 49). The
// speed needs no temporary any more: stored straight to velocityY before
// velocityZ is cleared it differs 2 as before. Left: the parameter spills:
// the native stores y (r2) before z (r3), here z first and y after the heap
// slot's address is formed (movs r3, #192). In sched2, z's spill takes its
// priority from the anti dependence on movs r3, #192, three steps above the
// heap slot load, and y's only from that load, which it may alias
// (-mthumb-slow-loads's store rule, cost 2), so y's spill ranks one lower; the
// native's order needs them tied, as the parameter order then breaks the tie.
// Overlay 675's Func_020009f8 (resource_675_c_020009f8.c) stops on the same
// pair of spills. Tried 2026-10-30: a one-pass block before the body ties the
// spills but holds source's and x's copies (mov sl, r0; adds r7, r1, #0)
// above it and hoists the state base's constant (6), also with source and x
// copied to locals after it (6). Before: no barrier or statement move reaches
// it.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct SparkState {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct SparkSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkWork {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite sprites[96];
  s16 next;
  u16 tileBase;
};

u32 Random16Far(void);
s32 Func_02002a24(struct Actor *object);
s32 Func_02002a2c(struct Actor *object);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);

void Func_0200260c(struct Actor *source, s32 x, s32 y, s32 z) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  s32 angle = Random16Far();
  s16 *next = &work->next;
  struct SparkSprite *sprite = &work->sprites[*next];
  struct SparkState *state = &work->states[*next];

  sprite->priority = Func_02002a24(source);
  sprite->unk1e = Func_02002a2c(source);
  state->frame = 0;
  state->x = x;
  state->y = y;
  state->z = z;
  Vector_AddPolarOffsetFar(Random16Far() * 2 + 0x20000, angle, &state->x);
  state->velocityX = 0;
  state->velocityY = (Random16Far() >> 1) + 0xc000;
  state->velocityZ = 0;
  Vector_AddPolarOffsetFar(Random16Far() >> 2, angle, &state->velocityX);
  *next = *next + 1;
  *next = *next % 96;
}
