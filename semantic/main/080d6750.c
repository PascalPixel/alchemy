typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void *Func_08077008(s32);
extern void Func_080b5010(s16 *, s32);

void Func_080d6750(void *arg0)
{
    s16 arr[14];
    s32 count;
    s32 i;
    s16 *p;

    count = 0;
    if (*(s16 *)((u8 *)arg0 + 0x24) > 0x7F) {
        i = 0;
        p = arr;
        do {
            if (*(s16 *)((u8 *)Func_08077008(i + 0x80) + 0x38) > 0) {
                *p = (s16)(i + 0x80);
                count++;
                p++;
            }
            i++;
        } while (i != 6);
    } else {
        p = &arr[count];
        i = 0;
        do {
            if (*(s16 *)((u8 *)Func_08077008(i) + 0x38) > 0) {
                *p = (s16)i;
                count++;
                p++;
            }
            i++;
        } while (i != 8);
    }
    arr[count] = 0xFF;
    Func_080b5010(arr, 0);
}
