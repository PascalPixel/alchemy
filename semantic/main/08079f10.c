#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
u8 *Func_08077394(s32);
s32 Func_0807987c(s32, s32);
s32 Func_08079be8(void);
s32 Func_08079d7c(s32);
s32 Func_08079e9c(u8 *, s32);
s32 Func_08079ef8(s32);

s32 Func_08079f10(s32 current, s32 target, s32 category, s32 action, s32 scale) {
    u8 *state = Func_08077394(target);
    s32 attempts = 1;
    s32 score;
    s32 attempt;
    s8 *flag131;
    u8 *flag138;
    u8 *flag139;
    u8 *flag13A;
    u8 *flag13B;
    u8 *flag13C;

    if (Func_08079ef8(action) != 0 && M2C_FIELD(state, s16 *, 0x38) != 0) {
        return 0;
    }

    if (action == 3 && M2C_FIELD(state, s8 *, 0x131) == 0) {
        goto fail;
    }

    goto action4_check;
action4_tail:
    if (state[0x13B] == 0 && state[0x13C] == 0 &&
        state[0x13D] == 0 && state[0x141] == 0) {
        goto fail;
    }
    goto action4_done;
action4_check:
    if (action == 4) {
        if (state[0x138] == 0 && state[0x139] == 0 && state[0x13A] == 0) {
            goto action4_tail;
        }
    }

action4_done:
    flag131 = (s8 *)(state + 0x131);
    flag138 = state + 0x138;
    flag139 = state + 0x139;
    flag13A = state + 0x13A;
    flag13B = state + 0x13B;
    flag13C = state + 0x13C;
    if (action == 0x40 &&
        *flag131 == 0 &&
        *flag138 == 0 &&
        *flag139 == 0 &&
        *flag13A == 0 &&
        *flag13B == 0 &&
        *flag13C == 0 &&
        M2C_FIELD(state, u8 *, 0x13D) == 0 &&
        M2C_FIELD(state, u8 *, 0x141) == 0 &&
        M2C_FIELD(state, u8 *, 0x140) == 0) {
        return 0;
    }

    if (action == 0x1C && M2C_FIELD(state, u8 *, 0x141) == 1) {
        return 0;
    }

    score = Func_08079d7c(action);
    if (score > 0) {
        s32 difference = Func_0807987c(current, category) -
            Func_0807987c(target, category) -
            (M2C_FIELD(state, u8 *, 0x42) >> 1);
        score += difference * 3;
        if (Func_08079e9c(state, action) != 0) {
            score += 25;
        }
    } else {
        score = -score;
    }

    if (action == 0x43) {
        attempts = 3;
    }

    for (attempt = 0; attempt < attempts; attempt++) {
        if (Func_080022ec(score * scale, 100) >= Func_08079be8()) {
            return 1;
        }
    }
fail:
    return 0;
}
