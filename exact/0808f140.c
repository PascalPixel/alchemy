#include "object_lookup.h"
#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080091e0(void *, s32);
void Func_08009098(void *, s32);
void Func_080030f8(u32);
void Func_0808f0d8(struct Object_0808f0d8 *object);
void Func_08009080(void *, s32);

struct GlobalState {
    u8 padding_000[500];
    u32 selected_object;
};

extern struct GlobalState Data_02000240;

void Func_0808f140(void *object, s32 flags) {
    void *other;

    if (object != NULL) {
        other = Func_0808ba1c(Data_02000240.selected_object);
        if (flags & 1) {
            Func_080091e0(object, 0);
            Func_08009098(object, 0x0809E6C0);
            FIELD(object, u32, 0x28) = 0x20000;
            FIELD(object, u32, 0x48) = 0x4000;
            FIELD(object, s32, 0x6C) = 0x0808EEE5;
        }
        if (flags == 3) {
            Func_080030f8(60);
        }
        if (flags & 2) {
            Func_0808f0d8(object);
        }
        if (flags == 3) {
            Func_080030f8(80);
        }
        Func_08009080(other, 1);
    }
}
