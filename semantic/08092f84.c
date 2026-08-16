#include "types.h"

struct RuntimeState {
    u8 padding_000[0x1CC];
    s32 busy;
};

extern struct RuntimeState *Data_03001ebc;
extern volatile u32 Data_03001ae8;

s32 Func_08092c40(s32 key);
void Func_080030f8(s32 frames);
s32 Func_08092ba8(s32 key);
void *Func_0808d394(s32 index);
void Func_08015100(s32 value);
s32 Func_08015050(s32 handle);
void Func_08015140(void);

void Func_08092f84(s32 key, s32 unused)
{
    struct RuntimeState *state = Data_03001ebc;
    s32 handle = Func_08092c40(key);
    s32 index;
    s32 value;
    u32 waitCount = 0;

    Func_080030f8(1);
    value = Func_08092ba8(key);
    if (key <= 7) {
        index = key & 0xFFF;
        if (Func_0808d394(index) == 0)
            value = index;
    }
    Func_08015100(value);

    if (state->busy == 0) {
        while (Func_08015050(handle) == 0) {
            Func_080030f8(1);
            waitCount++;
            if (waitCount > 600 ||
                ((Data_03001ae8 & 4) != 0 &&
                 (Data_03001ae8 & 0x100) != 0 &&
                 (Data_03001ae8 & 0x200) != 0 &&
                 (Data_03001ae8 & 1) != 0)) {
                Func_08015140();
            }
        }
    }

    Func_080030f8(1);
}
