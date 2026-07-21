typedef signed char s8;
typedef signed int s32;
typedef unsigned char u8;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_0801e3c8(s32 enabled) {
    s32 countdown;
    s32 reverse_countdown;
    s8 *byte;
    s8 *reverse_byte;
    void *state;

    state = *(void **)0x03001E8C;
    if (enabled != 0) {
        M2C_FIELD(state, s8 *, 0xEA2) = 1;
        enabled = 0;
        for (countdown = 0x80, byte = state + 0xE20; countdown <= 0xFF; countdown += 1) {
            *byte = enabled;
            byte += 1;
        }
        return;
    }
    M2C_FIELD(state, s8 *, 0xEA2) = 0;
    enabled = 0;
    reverse_byte = state + 0xE20;
    reverse_countdown = 0x7F;
    do {
        reverse_countdown -= 1;
        *reverse_byte = enabled;
        reverse_byte += 1;
    } while (reverse_countdown >= 0);
}
