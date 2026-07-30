typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern const s16 Data_080367e4[];
extern const s16 Data_0803680c[];

s32 Func_08019d2c(u32 value)
{
    s32 result = -1;
    s32 index = 0;
    s32 key;

    if (value <= 19) {
        const u8 *low = (const u8 *)Data_080367e4;
        key = *(const s16 *)low;
        if (key != -1) {
            if (key == value) {
                index = 2;
                result = *(const s16 *)(low + index);
            } else {
loop_low:
                index += 2;
                {
                    const u8 *entry = low + index * 2;
                    key = *(const s16 *)entry;
                }
                if (key != -1) {
                    if (key == value) {
                        index++;
                        {
                            const u8 *entry = low + index * 2;
                            result = *(const s16 *)entry;
                        }
                    } else {
                        goto loop_low;
                    }
                }
            }
        }
    } else {
        const u8 *high = (const u8 *)Data_0803680c;
        s32 sentinel = -1;
        key = *(const s16 *)high;
        if (key != sentinel) {
            if (key == value) {
                index = 2;
                result = *(const s16 *)(high + index);
                goto add_base;
            }
loop_high:
            index += 2;
            {
                const u8 *entry = high + index * 2;
                key = *(const s16 *)entry;
            }
            if (key != -1) {
                if (key == value) {
                    index++;
                    {
                        const u8 *entry = high + index * 2;
                        result = *(const s16 *)entry;
                    }
add_base:
                    result += 128;
                } else {
                    goto loop_high;
                }
            }
        }
    }
    return result;
}
