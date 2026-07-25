typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

u16 Func_020012c8(u32 count, u8 *p) {
    u32 crc;
    u32 i;
    u32 j;

    i = 0;
    crc = 0xFFFF;
    if (count != 0) {
        do {
            crc ^= (u32)*p << 8;
            j = 0;
            do {
                if ((crc & 0x8000) != 0) {
                    crc = (crc << 1) + 0xFFFFEFDF;
                } else {
                    crc = crc << 1;
                }
                j++;
            } while (j != 8);
            i++;
            p++;
        } while (i != count);
    }
    return ~crc;
}
