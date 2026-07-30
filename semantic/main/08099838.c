#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef void (*FrameCallback)(void);

extern u8 *Data_03001ebc;
struct GlobalState_08099838 {
    u8 padding_000[0x1f4];
    u32 selected_object;
    u8 padding_1f8[0x54];
    u16 callback_active;
};
extern struct GlobalState_08099838 Data_02000240;

struct SavedState_08099838 {
    u8 *first;
    void *runtime;
};

void Func_08099838(void)
{
    struct SavedState_08099838 saved;
    u32 cycle;
    void *object;
    void *effect;
    void *control;
    FrameCallback callback;

    saved.runtime = Data_03001ebc;
    object = Func_0808ba1c(Data_02000240.selected_object);
    effect = FIELD(object, void *, 0x50);
    control = FIELD(effect, void *, 0x28);
    Func_080f9010(130);
    Func_08009080(object, 0);
    FIELD(object, s32, 0x6c) = 0;
    cycle = 0;
    saved.first = effect + 0x25;

    do {
        FIELD(control, s8, 5) = 7;
        FIELD(effect, s8, 0x25) = 1;
        FIELD(effect, s8, 0x26) = 2;
        Func_080030f8(2);
        FIELD(effect, s8, 0x25) = 1;
        FIELD(effect, s8, 0x26) = 0;
        Func_080030f8(2);
        cycle++;
    } while (cycle <= 9);

    cycle = 0;
    FIELD(control, s8, 5) = cycle;
    FIELD(effect, s8, 0x26) = 2;
    *saved.first = 1;
    callback = (FrameCallback)0x08099679;
    Func_080041d8((s32)callback, 0xc80);
    Data_02000240.callback_active = 1;
    callback();

    if (FIELD(saved.runtime, s16, 0x17e) == 0x2092) {
        Func_08099738();
        FIELD(saved.runtime, u16, 0x17e) = cycle;
    }
}
