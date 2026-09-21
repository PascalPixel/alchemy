#include "TYPES.H"

struct RuntimeState_0808d8f0 {
    u8 padding0[0x19C];
    s16 delay;
};

struct GlobalData_0808d8f0 {
    u8 padding0[0x1F4];
    s32 selected_object;
};

extern void *Data_03001ebc;
extern u8 Data_02000240;
#define PARTY_STATE Data_02000240
extern volatile u32 Data_03001ae8;

void Func_08092708(s32, s32, s32);
#define BattleFx_RunRisingObjectSequence Func_08092708
void Func_08093c00(void);
void Func_08093e28(void);
#define FieldEffect_UpdateGridPlacement Func_08093e28
void Func_08093fa0(void);
#define battle_owner_69 Func_08093fa0

s32 Battle_DispatchInputEvent(s32 event)
{
    struct RuntimeState_0808d8f0 *state = (struct RuntimeState_0808d8f0 *)Data_03001ebc;
    s32 selected_object = ((struct GlobalData_0808d8f0 *)&PARTY_STATE)->selected_object;

    switch (event) {
    case 0xFC:
        if (state->delay > 12 && (Data_03001ae8 & 0x80)) {
            BattleFx_RunRisingObjectSequence(selected_object, 6, 0);
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
                FieldEffect_UpdateGridPlacement();
            } else if (Data_03001ae8 & 0x40) {
                battle_owner_69();
            }
            state->delay = 0;
        }
        break;
    }
    return 0;
}
