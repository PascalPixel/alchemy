typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080a10d0(s32 *, s32, s32, s32, s32, s32);
s32 Func_080a1778(s32, s32, s32);

s32 Func_080a1814(void *arg0)
{
    s32 handle;
    s32 zero = 0;
    s32 state;
    void *object;

    M2C_FIELD(arg0, s32 *, 0x10) = zero;
    Func_080a10d0((s32 *)((u8 *)arg0 + 0x10), zero, zero, 13, 5, 2);
    handle = M2C_FIELD(arg0, s32 *, 0x10);
    object = (void *)Func_080a1778(handle, -8, 11);
    M2C_FIELD(object, u8 *, 5) = 13;
    M2C_FIELD(arg0, u8 *, 0x1C) = 255;
    M2C_FIELD(arg0, u8 *, 0x1D) = zero;
    M2C_FIELD(arg0, void **, 0x14) = object;
    state = 254;
    M2C_FIELD(object, u8 *, 0x0F) = state;
    state -= 255;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x18), s8 *, 0x0F) = state;
    return handle;
}
