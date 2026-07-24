typedef unsigned char u8;
typedef signed int s32;

s32 Func_0801db70(void);
s32 Func_080207c4(void);
void Func_0801776c(s32, s32);
s32 Func_0801d4cc(void);

s32 Func_08029504(void)
{
    s32 result;

retry:
    result = Func_0801db70();
    if (result == -1) {
        return -1;
    }
    if (result == 0) {
        if (Func_080207c4() == -1) {
            goto retry;
        }
    } else if (result == 1) {
        Func_0801776c(0xc2a, 1);
        *(u8 *)0x03001cc8 = result;
    } else if (result == 2) {
        if (Func_0801d4cc() == -1) {
            goto retry;
        }
    }
    return 0;
}
