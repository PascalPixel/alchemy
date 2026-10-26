// Draft, differ 43 of 106: starts the next of the 96 sparks at a point
// around (x, y, z), in a random direction and speed. Left: the native keeps
// the work in r8 and the source in r10, this draft the other way round.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

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
s32 Func_02002a24(struct ObjectRuntime *object);
s32 Func_02002a2c(struct ObjectRuntime *object);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);

void Func_0200260c(struct ObjectRuntime *source, s32 x, s32 y, s32 z) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  s32 angle = Random16Far();
  s16 *next = &work->next;
  struct SparkSprite *sprite = &work->sprites[*next];
  struct SparkState *state = &work->states[*next];
  s32 speed;

  sprite->priority = Func_02002a24(source);
  sprite->unk1e = Func_02002a2c(source);
  state->frame = 0;
  state->x = x;
  state->y = y;
  state->z = z;
  Vector_AddPolarOffsetFar(Random16Far() * 2 + 0x20000, angle, &state->x);
  state->velocityX = 0;
  speed = (Random16Far() >> 1) + 0xc000;
  state->velocityZ = 0;
  state->velocityY = speed;
  Vector_AddPolarOffsetFar(Random16Far() >> 2, angle, &state->velocityX);
  *next = *next + 1;
  *next = *next % 96;
}
