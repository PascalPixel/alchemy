// Draft, differ 2 of 57 (tla-en, 5 November), no device: the two pointer steps at the loop's end in the other order (native adds r6, #40 ahead of adds r5, #28). Their initial order is already the native's.
#include "types.h"
#include "call.h"
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
      sprite->tile = work->tileBase + ((value >> 1) & 7) * 8;
      Func_02002a14(sprite, state);
      Func_02002a1c(state, 64, 0);
      state->frame++;
    }
  }
}
