typedef signed char s8;
typedef signed int s32;
typedef unsigned int u32;

void Func_0808a500(s32);
u32 Func_08004458(void);
void Func_08009248(s32, u32);

void Func_080b2ffc(void)
{
    s32 var_r5;
    s32 var_r6;
    s32 temp_r7;
    s8 temp_r5;
    s32 offset;

    temp_r7 = *(s32 *)0x03001F2C;
    var_r5 = temp_r7 + 0x3B0;
    var_r6 = 0x17;
    do {
        var_r6 -= 1;
        Func_0808a500(var_r5);
        var_r5 += 0x48;
    } while (var_r6 >= 0);
    temp_r5 = *(s8 *)(temp_r7 + 0x3AB);
    if (temp_r5 != -1) {
        Func_08009248(*(s32 *)(temp_r7 + (offset = (temp_r5 * 4) + 0x114)), (Func_08004458() * 7) >> 16);
    }
}
