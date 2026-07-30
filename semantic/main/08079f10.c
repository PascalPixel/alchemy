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

    if (Func_08079ef8(action) != 0 && M2C_FIELD(state, s16 *, 0x38) != 0) {
        return 0;
    }

    if (action == 3 && M2C_FIELD(state, s8 *, 0x131) == 0) {
        return 0;
    }

    if (action == 4 &&
        M2C_FIELD(state, u8 *, 0x138) == 0 &&
        M2C_FIELD(state, u8 *, 0x139) == 0 &&
        M2C_FIELD(state, u8 *, 0x13A) == 0 &&
        M2C_FIELD(state, u8 *, 0x13B) == 0 &&
        M2C_FIELD(state, u8 *, 0x13C) == 0 &&
        M2C_FIELD(state, u8 *, 0x13D) == 0 &&
        M2C_FIELD(state, u8 *, 0x141) == 0) {
        return 0;
    }

    if (action == 0x40 &&
        M2C_FIELD(state, s8 *, 0x131) == 0 &&
        M2C_FIELD(state, u8 *, 0x138) == 0 &&
        M2C_FIELD(state, u8 *, 0x139) == 0 &&
        M2C_FIELD(state, u8 *, 0x13A) == 0 &&
        M2C_FIELD(state, u8 *, 0x13B) == 0 &&
        M2C_FIELD(state, u8 *, 0x13C) == 0 &&
        M2C_FIELD(state, u8 *, 0x13D) == 0 &&
        M2C_FIELD(state, u8 *, 0x140) == 0 &&
        M2C_FIELD(state, u8 *, 0x141) == 0) {
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
    return 0;
}
