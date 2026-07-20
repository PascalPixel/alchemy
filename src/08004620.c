typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
extern s32 Func_080022f4(u32, s32);
extern u8 Data_08007970[];
extern u8 Data_03001f70[];

void Func_08004620(s32 arg0) {
    u32 *tbl;
    s8 *out;
    s32 count;
    u32 word;
    s32 result;
    s32 val;
    s8 sign;

    result = arg0;
    val = result;
    tbl = (u32 *)Data_08007970;
    sign = 0x20;
    out = (s8 *)Data_03001f70;
    if (val < 0) {
        val = -val;
        sign = 0x2D;
    }
    word = *tbl++;
    count = 9;
    if ((u32) val < word) {
        do {
            count -= 1;
            *out++ = 0x20;
            if (count == 0) break;
            word = *tbl++;
        } while ((u32) val < word);
    }
    *out++ = sign;
    tbl -= 1;
    if (count != 0) {
        do {
            word = *tbl++;
            result = Func_080022f4((u32) val, word);
            *out++ = result + 0x30;
            val -= result * word;
            count -= 1;
        } while (count != 0);
    }
    out[0] = val + 0x30;
    out[1] = 0;

}
