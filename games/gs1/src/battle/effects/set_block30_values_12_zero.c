#include "types.h"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct Fields_08095240 {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void BattleEffect_SetBlock30Values12Zero(void) {
    struct Fields_08095240 *state = (struct Fields_08095240 *)Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 12;
    state->field_1f82 = 0;
}
