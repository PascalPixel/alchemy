#include "object_lookup.h"
#include "types.h"

struct BattleEffect16GlobalState {
    u8 unknown_000[0x1F4];
    u32 active_object_id;
};

extern struct BattleEffect16GlobalState Data_02000240;
extern u32 Data_03001e40;
s32 UnsignedModulo(u32, s32);

void BattleEffect_UpdateEffect16State(void) {
    s32 phase;
    u8 *effect_state;
    u8 *child_state;
    register u8 *state_byte;
    register u8 state_value;

    effect_state = *(u8 **)(ObjectTable_Get(Data_02000240.active_object_id) + 0x50);
    child_state = *(u8 **)(effect_state + 0x28);
    phase = UnsignedModulo(Data_03001e40, 5);
    if (phase == 0) {
        state_byte = effect_state + 0x25;
        *state_byte = 1;
        state_byte++;
        state_value = 3;
        goto write_value;
    }
    if (phase == 2) {
        state_byte = effect_state + 0x25;
        child_state[5] = 0;
        state_value = 1;
        *state_byte = state_value;
        state_byte++;
write_value:
        *state_byte = state_value;
    }
}
