#include "types.h"

struct RuntimeState_0808d8f0 {
    u8 padding0[0x19C];
    s16 delay;
};

struct GlobalData_0808d8f0 {
    u8 padding0[0x1F4];
    s32 selected_object;
};

extern struct RuntimeState_0808d8f0 *Data_03001ebc;
extern struct GlobalData_0808d8f0 Data_02000240;
extern volatile u32 Data_03001ae8;

void Func_08092708(s32, s32, s32);
void Func_08093c00(void);
void Func_08093e28(void);
void Func_08093fa0(void);

s32 Func_0808d8f0(s32 event)
{
    struct RuntimeState_0808d8f0 *state = Data_03001ebc;
    s32 selected_object = Data_02000240.selected_object;

    switch (event) {
    case 0xFC:
        if (state->delay > 12 && (Data_03001ae8 & 0x80)) {
            Func_08092708(selected_object, 6, 0);
            state->delay = 0;
        }
        break;
    case 0xF9:
    case 0xFE:
        if (state->delay > 12) {
            Func_08093c00();
            state->delay = 0;
        }
        break;
    case 0xFD:
        if (state->delay > 12) {
            if (Data_03001ae8 & 0x80) {
                Func_08093e28();
            } else if (Data_03001ae8 & 0x40) {
                Func_08093fa0();
            }
            state->delay = 0;
        }
        break;
    }
    return 0;
}
