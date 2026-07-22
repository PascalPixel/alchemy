typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u32 Func_08004938(s32 arg0)
{
    s32 stateAddress = 0x03001E50;
    u32 temp_r0_2;
    u32 temp_r3;
    u32 var_r2;
    u32 units = ((u32)arg0 + 3) >> 2;

    var_r2 = M2C_FIELD((void *)stateAddress, u32 *, 4);
    arg0 = (s32)(units << 2);
    temp_r3 = var_r2 + (u32)arg0;
    if (temp_r3 > 0x030077FFU) {
        var_r2 = M2C_FIELD((void *)stateAddress, u32 *, 0);
        temp_r0_2 = var_r2 + (u32)arg0;
        if (temp_r0_2 >= 0x02040000U) {
            return 0U;
        }
        M2C_FIELD((void *)stateAddress, u32 *, 0) = temp_r0_2;
        goto block_5;
    }
    M2C_FIELD((void *)stateAddress, u32 *, 4) = temp_r3;
block_5:
    return var_r2;
}
