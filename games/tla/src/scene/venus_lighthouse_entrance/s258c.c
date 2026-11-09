#include "types.h"
#include "ram_buffer.h"

struct Unk_0200258cState {
  u8 unk00[24];
  s32 frame;
};

struct Unk_0200258cSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[30];
};

struct Unk_0200258cWork {
  u8 unk0000[0x800];
  struct Unk_0200258cState states[96];
  struct Unk_0200258cSprite sprites[96];
  u16 unk2180;
  u16 unk2182;
};

void Func_02002a14(struct Unk_0200258cSprite *sprite, struct Unk_0200258cState *state);
void Func_02002a1c(struct Unk_0200258cState *state, s32 a, s32 b);

void Func_0200258c(void) {
  struct Unk_0200258cWork *work = Ram_HeapSlots->menuRuntime;
  struct Unk_0200258cState *state = work->states;
  struct Unk_0200258cSprite *sprite = work->sprites;
  s32 value;
  s32 i;

  for (i = 0; i < 96; i++, sprite++, state++) {
    value = state->frame;
    if ((u32)value < 16) {
      sprite->tile = work->unk2182 + ((value >> 1) & 7) * 8;
      Func_02002a14(sprite, state);
      Func_02002a1c(state, 64, 0);
      state->frame++;
    }
  }
}
