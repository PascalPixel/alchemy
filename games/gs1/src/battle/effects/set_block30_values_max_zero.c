#include "types.h"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct Fields_08095214 {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void BattleEffect_SetBlock30ValuesMaxZero(void) {
    struct Fields_08095214 *state = (struct Fields_08095214 *)Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 0x7FFF;
    state->field_1f82 = 0;
}
