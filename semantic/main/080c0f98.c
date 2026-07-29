typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void **Func_080b7dd0(s32);

void Func_080c0f98(s32 arg0, s32 arg1) {
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r2;
    s32 var_r6;
    void **temp_r0;
    void **temp_r4;
    void **var_r1;
    void *temp_r0_2;
    void *temp_r4_2;

    temp_r0 = Func_080b7dd0(arg0);
    if ((temp_r0 != NULL) && (temp_r0_2 = *temp_r0, (temp_r0_2 != NULL))) {
        temp_r2 = 0xF & M2C_FIELD(temp_r0_2, u8 *, 0x54);
        switch (temp_r2) {
        case 1:
            temp_r4 = M2C_FIELD(temp_r0_2, void ***, 0x50);
            temp_r0_3 = (arg1 & 3) * 4;
            M2C_FIELD(temp_r4, u8 *, 5) = (u8) ((-0xD & M2C_FIELD(temp_r4, u8 *, 5)) | temp_r0_3);
            M2C_FIELD(temp_r4, u8 *, 0x11) = (u8) ((-0xD & M2C_FIELD(temp_r4, u8 *, 0x11)) | temp_r0_3);
            return;
        case 2:
            var_r1 = M2C_FIELD(temp_r0_2, void ***, 0x50);
            temp_r0_4 = (arg1 & 3) * 4;
            var_r6 = 0;
loop_7:
            temp_r4_2 = *var_r1;
            var_r1++;
            if (temp_r4_2 != NULL) {
                M2C_FIELD(temp_r4_2, u8 *, 5) = (u8) ((-0xD & M2C_FIELD(temp_r4_2, u8 *, 5)) | temp_r0_4);
                var_r6 += 1;
                M2C_FIELD(temp_r4_2, u8 *, 0x11) = (u8) ((-0xD & M2C_FIELD(temp_r4_2, u8 *, 0x11)) | temp_r0_4);
                if (var_r6 <= 3) {
                    goto loop_7;
                }
            }
            break;
        }
    }
}
