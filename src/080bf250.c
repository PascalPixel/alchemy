typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

u8 *Func_08077008(void);
s32 Func_080bf208(s32, u8, s32);

s32 Func_080bf250(s32 arg0) {
    u8 *base = Func_08077008();
    u8 *value = base + 0x132;

    if (*value != 0) {
        s32 zero;
        (*value)--;
        zero = 0;
        if ((s8)*value == 0) {
            base[0x133] = zero;
            return 1;
        }
        if ((s8)base[0x133] < 0) {
            if (Func_080bf208(arg0, *value, 30) != 0) {
                base[0x133] = zero;
                *value = zero;
                return 1;
            }
        }
    }
    return 0;
}
