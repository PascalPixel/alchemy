typedef signed int s32;
typedef unsigned int u32;

extern u32 Data_020097b0;
extern u32 Data_020090b0;

extern void Func_02001a92(s32, s32);
extern void Func_020017fe(void);

void Func_02000c0c(s32 a)
{
    u32 *p = (u32 *)0x03001ed0;
    u32 b = *p;
    u32 src;
    u32 c;

    if (a != 0) {
        p = (u32 *)0x040000D4;
        src = (u32)&Data_020097b0;
    } else {
        p = (u32 *)0x040000D4;
        src = (u32)&Data_020090b0;
    }
    c = 0x840000e0;
    p[0] = src;
    p[1] = b;
    p[2] = c;
    Func_02001a92(0x10000, 0);
    Func_020017fe();
}
