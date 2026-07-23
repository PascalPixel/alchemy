typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02000240[];
extern u8 Value_0000097d;

extern s32 Func_08077148(s32);
extern s32 Func_080772d8(u8, s32);
extern void Func_08015120(s32, s32);
extern void Func_08015040(void *, s32);

s32 Func_080919d8(s32 arg0)
{
    s32 count;
    s32 value;
    s32 offset = 0;
    s32 remaining;
    s32 total = offset;
    u8 *entry;

    count = Func_08077148(arg0);
    if (total < count) {
        offset = 252;
        offset <<= 1;
        entry = Data_02000240 + offset;
        remaining = count;
        do {
            value = Func_080772d8(*entry, arg0);
            remaining--;
            entry++;
            total += value;
        } while (remaining != 0);
    }

    if (total >= count * 30) {
        Func_08015120(arg0, 2);
        Func_08015040(&Value_0000097d, 1);
        Func_08015120(arg0, 2);
        Func_08015040(&Value_0000097d + 1, 1);
        return -1;
    }
    return 0;
}
