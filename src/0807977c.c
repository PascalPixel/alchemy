typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct State_0807977c {
    u32 flags;
};

extern u8 Data_08084a8c[16];
struct State_0807977c *Func_08077330(s32);

s32 Func_0807977c(u8 *output) {
    u8 *source = Data_08084a8c;
    u8 *end = Data_08084a8c + 15;
    s32 count = 0;

    do {
        u8 value = *source++;
        if ((Func_08077330(0)->flags & (1 << value)) != 0) {
            *output++ = value;
            count++;
        }
    } while (source <= end);
    *output = 32;
    return count;
}
