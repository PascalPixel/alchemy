typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

extern volatile s16 Data_0200200c;

s32 Func_080056cc(void);
s32 Func_08005c68(void);
void Func_08005cf8(void);

s32 Func_0801f77c(void)
{
    s32 initial;
    s32 count;
    s32 result;

    initial = Func_080056cc();
    count = 0;
    result = -9;
    if (initial == 0) {
        s32 temp_r0;
        s16 *second;
        s32 temp_r3;
        void *var_r1;
        s32 first;

        temp_r0 = Func_08005c68();
        var_r1 = (void *)*(volatile s32 *)0x03001F1C;
        second = (s16 *)&Data_0200200c;
        temp_r3 = 0x02002010;
        *(volatile s16 *)temp_r3 = 0;
        first = temp_r3;
        temp_r3 = 0x1070;
        *second = 0;
        result = temp_r0;
        var_r1 += temp_r3;
        for (temp_r0 = 2; temp_r0 >= 0; temp_r0--, var_r1 += 64) {
            if (*(s8 *)((s8 *)var_r1 + 1) != 0) {
                *(volatile s16 *)first = 1;
                count++;
            }
            if (*(s8 *)((s8 *)var_r1 + 2) != 0) {
                Data_0200200c = 1;
            }
        }

        if ((*(volatile s32 *)0x03001AE8 & 0x120) != 0x120) {
            *(volatile s16 *)0x02002010 = 0;
        }
    }
    Func_08005cf8();
    if (result != 0 && count == result) {
        return result + 100;
    }
    return result;
}
