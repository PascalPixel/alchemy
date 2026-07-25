typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 Data_0200ce3c[];

void Func_02003c94(u8 *out, u32 value) {
    s32 i;

    out += 8;
    *out = 0;
    out--;
    for (i = 7; i >= 0; i--) {
        *out = Data_0200ce3c[value & 15];
        value >>= 4;
        out--;
    }
}
