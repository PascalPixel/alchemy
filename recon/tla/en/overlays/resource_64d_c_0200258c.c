// Draft, differ 22 of 57 (was 54), registers only: the Venus Lighthouse
// entrance's spark animation, each live spark (frame below 16) stepping its
// tile through eight frames from the tile base. One local carrying both the
// frame and the new tile keeps the tile base's address inside the loop, as
// the native does (plain, loop.c hoists it into r8 and the counter moves to
// r7: 54). Left: the native keeps the frame in r3 and the tile in r1, with
// separate pseudos; here the shared local takes r0 for both. Measured: a
// frame local apart from the tile (32, the tile base hoisting returns in
// part), the frame pinned to r3 (28, or 15 with the frame read again for the
// shift), five minutes of psynergy permute (nothing under 22).
#include "types.h"
#include "ram_buffer.h"

struct SparkState {
  u8 unk00[24];
  s32 frame;
};

struct SparkSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[30];
};

struct SparkWork {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite sprites[96];
  u16 unk2180;
  u16 tileBase;
};

void Func_02002a14(struct SparkSprite *sprite, struct SparkState *state);
void Func_02002a1c(struct SparkState *state, s32 a, s32 b);

void Func_0200258c(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  s32 value;
  s32 i;

  for (i = 0; i < 96; i++) {
    struct SparkSprite *sprite = &work->sprites[i];
    struct SparkState *state = &work->states[i];

    value = state->frame;
    if ((u32)value < 16) {
      value = work->tileBase + ((state->frame >> 1) & 7) * 8;
      sprite->tile = value;
      Func_02002a14(sprite, state);
      Func_02002a1c(state, 64, 0);
      state->frame++;
    }
  }
}
