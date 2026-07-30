typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

extern u8 *Data_03001f2c;

s32 Func_08077148(void);
void Func_08009008(void *, s32 *, s32 *, s32);

void Func_080a19a0(void)
{
    u8 *temp_r5;
    s32 sp4[2];
    s32 spC[4];
    s32 *temp_r8;
    s16 *temp_r7;
    volatile s32 *temp_r6;
    s32 var_r4;
    s32 temp_sl;

    temp_r5 = Data_03001f2c;
    temp_sl = (u16)Func_08077148();
    var_r4 = 0;
    if (var_r4 < temp_sl) {
        temp_r8 = sp4;
        temp_r6 = spC;
        temp_r7 = (s16 *)(temp_r5 + 308);
        temp_r5 += 276;
        do {
            void *object;
            s32 top;

            top = 0x01e20000 - (temp_r7[8] << 16);
            object = *(void **)temp_r5;
            if (object != 0) {
                *((s8 *)object + 9) &= -13;
                sp4[0] = *(s32 *)(temp_r5 + 64);
                temp_r8[1] = *(s32 *)(temp_r5 + 64);
                temp_r6[1] = top;
                temp_r6[0] = temp_r7[0] << 16;
                temp_r6[2] = (temp_r7[8] << 16) + top;
                temp_r6[3] = 0;
                Func_08009008(object, (s32 *)temp_r6, temp_r8, 0x4000);
            }
            var_r4++;
            temp_r7++;
            temp_r5 += 4;
        } while (var_r4 < temp_sl);
    }
}
