typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;

extern u16 Data_080b413c[];

void Func_080b06ec(s32 arg0, s32 arg1, u32 arg2) {
    u8 *source = (u8 *)(0x080B3D40 + (arg0 << 5));
    u8 *destination = (u8 *)(arg1 + Data_080b413c[arg2] + 2);
    s32 count = 3;

    do {
        if (*source != 0) {
            destination[0] = *source++;
            if (*source != 0) {
                destination[1] = *source++;
                if (*source != 0) {
                    destination[30] = *source++;
                    if (*source != 0) destination[31] = *source++;
                }
            }
        }
        destination += 4;
        count--;
    } while (count >= 0);
}
