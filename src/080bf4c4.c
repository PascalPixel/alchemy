typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_08077008(s32);
s32 Func_080bf208(s32, s32, s32);

s32 Func_080bf4c4(s32 arg0)
{
    s32 temp_r3_2;
    s32 var_r0;
    u8 temp_r3_3;
    s32 temp_r3;
    s32 var_r2;
    void *temp_r0;

    temp_r0 = Func_08077008(arg0);
    var_r2 = M2C_FIELD(temp_r0, u8 *, 0x13D);
    temp_r3 = var_r2 & 0xFF;
    if (temp_r3 != 0) {
        if ((u32)temp_r3 > 7U) {
            temp_r3 += 0xF8;
            M2C_FIELD(temp_r0, u8 *, 0x13D) = temp_r3;
            var_r2 = temp_r3;
        }
        if (var_r2 & 7) {
            temp_r3_2 = var_r2 + 0xFF;
            M2C_FIELD(temp_r0, u8 *, 0x13D) = temp_r3_2;
            var_r2 = temp_r3_2;
        }
        var_r0 = 1;
        temp_r3_3 = var_r2;
        if (temp_r3_3 != 0) {
            if ((u32)temp_r3_3 <= 7U &&
                Func_080bf208(arg0, M2C_FIELD(temp_r0, u8 *, 0x13D), 0x1E) != 0) {
                M2C_FIELD(temp_r0, u8 *, 0x13D) = 0U;
                return 1;
            }
            goto block_9;
        }
        return var_r0;
    }
block_9:
    var_r0 = 0;
    return var_r0;
}
