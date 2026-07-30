typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void *Func_08077008(void *);
s32 Func_08077058(void *, s32);
extern u8 Value_00000200;

s32 Func_080a40ac(void *arg0)
{
    void *object = Func_08077008(arg0);
    u32 value = *(u16 *)((u8 *)object + 0xD8);
    s32 result = 0;
    s32 index = 0;
    u16 *entry = (u16 *)((u8 *)object + 0xD8);

    goto condition;
body:
    {
        u32 current = *entry;
        if ((current & (u32)&Value_00000200) == 0) {
            s32 high = current >> 11;
            s32 count = high + 1;
            if (high == 0)
                count = 1;
            if (count != 0) {
                do {
                    result = Func_08077058(arg0, index);
                    count--;
                } while (count != 0);
            }
            if (result != 2)
                goto return_zero;
            goto return_one;
        }
        index++;
        entry++;
        if (index > 14)
            goto return_result;
        value = *entry;
    }
condition:
    if (value != 0)
        goto body;
return_one:
    result = 1;
return_result:
    return result;
return_zero:
    return 0;
}
