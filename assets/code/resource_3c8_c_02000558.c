typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;
extern void Func_020054b8(s32 *, s32);
extern u8 Value_00000212;

void Func_02000558(void)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;
    s32 lim = (s32)&Value_00000212;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];
        u32 h = *(u16 *)((u8 *)p + 100);
        s32 t = h << 16;

        if ((t >> 20) == lim) {
            s32 m = 15;
            m &= h;
            Func_020054b8(p, m);
        }
    }
}
