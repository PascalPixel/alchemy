#include "object_lookup.h"
#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080091e0(void *, s32);
void Object_SetCallback(void *, s32);
void WaitFrames(u32);
void Func_0808f0d8(struct Object_0808f0d8 *object);
void Object_SetMode(void *, s32);

struct GlobalState {
    u8 padding_000[500];
    u32 selected_object;
};

extern struct GlobalState Data_02000240;
extern const u8 Data_0809e6c0[];
extern u8 Data_0808eee5;

void Func_0808f140(void *object, s32 flags) {
    void *other;

    if (object != NULL) {
        other = Func_0808ba1c(Data_02000240.selected_object);
        if (flags & 1) {
            Func_080091e0(object, 0);
            Object_SetCallback(object, (s32)Data_0809e6c0);
            FIELD(object, u32, 0x28) = 0x20000;
            FIELD(object, u32, 0x48) = 0x4000;
            FIELD(object, s32, 0x6C) = (s32)&Data_0808eee5;
        }
        if (flags == 3) {
            WaitFrames(60);
        }
        if (flags & 2) {
            Func_0808f0d8(object);
        }
        if (flags == 3) {
            WaitFrames(80);
        }
        Object_SetMode(other, 1);
    }
}
