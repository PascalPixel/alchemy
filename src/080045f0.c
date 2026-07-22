typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

void Func_080045f0(u32 value) {
    u8 *buffer = (u8 *)0x03001F70;
    const u8 *digits = (const u8 *)0x0800795C;
    s32 index = 7;

    do {
        buffer[index] = digits[value & 0xF];
        value >>= 4;
        index--;
    } while (index >= 0);

    {
        u8 *terminator = (u8 *)0x03001F70;
        terminator[8] = 0;
    }
}
