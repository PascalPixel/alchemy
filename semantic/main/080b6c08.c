typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Data_03001e74;

s32 Func_080b6c08(s32 arg0, u16 *arg1) {
    u8 *temp_r0;
    u8 *temp_r2;
    u8 *temp_ip;
    s32 temp_r3;
    u16 temp_r4;
    s32 temp_r5;
    s32 temp_r6;
    s32 temp_r7;

    temp_r6 = arg0;
    temp_r0 = Data_03001e74;
    temp_r3 = temp_r6 & 1;
    temp_r5 = 0;
    if (temp_r3 != 0) {
        temp_r3 = *(s16 *)(temp_r0 + 88);
        if (temp_r3 != 255) {
            temp_r2 = temp_r0 + 88;
loop1:
            temp_r7 = 0;
            temp_r4 = *(u16 *)temp_r2;
            temp_r3 = *(s16 *)(temp_r2 + temp_r7);
            if (temp_r3 != 254) {
                if (arg1 != 0) {
                    *arg1 = temp_r4;
                    arg1 += 1;
                }
                temp_r5 += 1;
            }
            temp_r2 += 2;
            temp_r4 = 0;
            temp_r3 = *(s16 *)(temp_r2 + temp_r4);
            if (temp_r3 != 255)
                goto loop1;
        }
    }

    temp_r3 = temp_r6 & 2;
    if (temp_r3 != 0) {
        temp_r2 = temp_r0 + 2;
        temp_r3 = *(s16 *)(temp_r2 + 100);
        temp_ip = temp_r2;
        if (temp_r3 != 255) {
            temp_r0 = (u8 *)100;
loop2:
            temp_r4 = *(u16 *)(temp_r2 + (s32)temp_r0);
            temp_r3 = *(s16 *)(temp_r2 + (s32)temp_r0);
            if (temp_r3 != 254) {
                if (arg1 != 0) {
                    *arg1 = temp_r4;
                    arg1 += 1;
                }
                temp_r5 += 1;
            }
            temp_r0 += 2;
            temp_r2 = temp_ip;
            temp_r3 = *(s16 *)(temp_r2 + (s32)temp_r0);
            if (temp_r3 != 255)
                goto loop2;
        }
    }

    if (arg1 != 0)
        *arg1 = 255;
    return temp_r5;
}
