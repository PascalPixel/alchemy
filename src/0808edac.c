typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080091a8(s32, s32, s32);
s32 Func_0808ec14();

void Func_0808edac(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r0;
    s32 temp_r0_3;
    s32 var_r6;
    s32 var_r7;
    void *temp_r0_2;
    void *temp_r3;
    void *temp_r5;

    var_r7 = arg2;
    var_r6 = arg1;
    temp_r0 = Func_0808ec14();
    if (temp_r0 != -1) {
        temp_r3 = *(s32 *)0x03001EBC + (temp_r0 * 8);
        temp_r0_2 = temp_r3 + 0x11C;
        temp_r5 = M2C_FIELD(temp_r3, void **, 0x11C);
        if (temp_r5 != NULL) {
            if (var_r6 == -1) {
                var_r6 = (M2C_FIELD(temp_r0_2, u8 *, 6) << 0x14) + 0x80000;
            }
            if (var_r7 == -1) {
                var_r7 = (M2C_FIELD(temp_r0_2, u8 *, 7) << 0x14) + 0x80000;
            }
            M2C_FIELD(temp_r5, s32 *, 8) = var_r6;
            M2C_FIELD(temp_r5, s32 *, 0x10) = var_r7;
            temp_r0_3 = Func_080091a8(0, var_r6, var_r7);
            M2C_FIELD(temp_r5, s32 *, 0x14) = temp_r0_3;
            M2C_FIELD(temp_r5, s32 *, 0xC) = temp_r0_3;
        }
    }
}
