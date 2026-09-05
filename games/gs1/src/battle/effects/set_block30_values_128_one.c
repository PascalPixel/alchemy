#include "types.h"

#define BattleEffect_SetBlock30Values128One Func_08095268

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct Fields_08095268 {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void BattleEffect_SetBlock30Values128One(void) {
    struct Fields_08095268 *state = (struct Fields_08095268 *)Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 128;
    state->field_1f82 = 1;
}
