#include "TYPES.H"

struct RuntimeState_0808d8f0 {
    u8 padding0[0x19C];
    s16 delay;
};

struct GlobalData_0808d8f0 {
    u8 padding0[0x1F4];
    s32 selected_object;
};

extern void *gEventWork;
extern u8 gGameState;
#define PARTY_STATE gGameState
extern volatile u32 Data_03001ae8;

void BattleFx_RunRisingObjectSequence(s32, s32, s32);
void Field_TryJumpForward(void);
void FieldEffect_UpdateGridPlacement(void);
void battle_owner_69(void);

s32 Battle_DispatchInputEvent(s32 event)
{
    struct RuntimeState_0808d8f0 *state = (struct RuntimeState_0808d8f0 *)gEventWork;
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
            Field_TryJumpForward();
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
