typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_0800307c(s32 arg0, s32 arg1, const void *arg2);

void Func_08021e28(void) {
    u32 address = 0x04000012;
    s32 zero = 0;

    *(volatile u16 *)address = zero;
    address = 0x08021E15;
    Func_0800307c(2, 136, (const void *)address);
}
