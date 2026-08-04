typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 Data_02000240;

void Func_08092c40(s32);
s32 Func_08091c7c(void *, s32);
void Func_08092f84(s32, s32);

s32 Func_08093054(s32 first, s32 second)
{
    s32 result;
    u8 *state;
    u8 *table;

    Func_08092c40(first);
    table = &Data_02000240;
    result = Func_08091c7c(*(void **)(table + 500), 0);
    if (result == 0) {
        Func_08092f84(first, second);
        state = *(u8 **)0x03001ebc;
        *(u16 *)(state + 472) += 1;
    } else {
        state = *(u8 **)0x03001ebc;
        *(u16 *)(state + 472) += 1;
        Func_08092f84(first, second);
    }
    return result;
}

void Func_080930b8(void)
{
}
