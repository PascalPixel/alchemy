typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080f7e34(s32 arg0) {
    s32 temp_r0;
    s32 temp_r1;
    s32 temp_r3;
    s32 temp_r4;
    void *temp_r2;

    temp_r1 = *(s32 *)0x02004C00;
    temp_r3 = arg0 * 12;
    temp_r4 = temp_r3 + 4;
    temp_r0 = *(s32 *)(temp_r1 + temp_r4);
    if (temp_r0 != 0) {
        temp_r2 = *(void **)(temp_r1 + temp_r3);
        if (temp_r2 != 0) {
            *(s32 *)((u8 *)temp_r2 + 4) = temp_r0;
        }
        **(s32 **)(temp_r1 + temp_r4) = *(s32 *)(temp_r1 + temp_r3);
    }
}
