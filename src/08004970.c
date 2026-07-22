typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s16 *Func_08004970(s32 arg0)
{
    s32 stateAddress = 0x03001E50;
    u32 temp_r0_2;
    u32 temp_r0;
    u32 temp_r3;
    u32 var_r2;
    u32 units = ((u32)arg0 + 3) >> 2;

    var_r2 = M2C_FIELD((void *)stateAddress, u32 *, 0);
    arg0 = (s32)(units << 2);
    temp_r3 = var_r2 + (u32)arg0;
    if (temp_r3 >= 0x02040000U) {
        var_r2 = M2C_FIELD((void *)stateAddress, u32 *, 4);
        temp_r0_2 = var_r2 + (u32)arg0;
        if (temp_r0_2 > 0x030077FFU) {
            return NULL;
        }
        M2C_FIELD((void *)stateAddress, u32 *, 4) = temp_r0_2;
        goto done;
    }
    M2C_FIELD((void *)stateAddress, u32 *, 0) = temp_r3;
done:
    return (s16 *)var_r2;
}

void Func_080049a8(void)
{
}
