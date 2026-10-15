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

void Func_0200258c(void)
{
    struct SparkWork *work = Ram_HeapSlots->menuRuntime;
    s32 i;

    for (i = 0; i < 96; i++) {
        struct SparkState *state = &work->states[i];
        struct SparkSprite *sprite = &work->sprites[i];

        if ((u32)state->frame < 16) {
            sprite->tile = work->tileBase + ((state->frame >> 1) & 7) * 8;
            Func_02002a14(sprite, state);
            Func_02002a1c(state, 64, 0);
            state->frame++;
        }
    }
}
