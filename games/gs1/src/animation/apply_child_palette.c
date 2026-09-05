#include "effect_runtime.h"

struct Item0808e0b0 {
    u8 padding0[5];
    u8 value;
    u8 padding6[10];
    void *field_10;
};

struct Inner0808e0b0 {
    u8 padding0[37];
    u8 dirty;
    u8 padding26;
    u8 cnt;
    struct Item0808e0b0 *items[1];
};

struct Outer0808e0b0 {
    u8 padding0[80];
    struct Inner0808e0b0 *state;
    u8 mode;
};

extern u32 Data_03001e40;
extern u8 Data_0809e6b8[];

void Animation_ApplyChildPalette(struct Outer0808e0b0 *obj, s32 palette)
{
    struct Inner0808e0b0 *state;
    struct Item0808e0b0 **p;
    struct Item0808e0b0 *current;
    s32 cnt;
    s32 replacement;
    u8 n;

    if ((obj->mode & 15) == 1) {
        state = obj->state;
        replacement = palette - 1;
        if (palette == 0)
            replacement = Data_0809e6b8[(Data_03001e40 >> 1) & 7];

        n = state->cnt;
        if (n != 0) {
            p = state->items;
            cnt = n;
            do {
                current = *p++;
                if (current != 0 && current->field_10 != 0 &&
                    current->value != 15)
                    current->value = replacement;
                cnt--;
            } while (cnt != 0);
        }
        state->dirty = 1;
    }
}
