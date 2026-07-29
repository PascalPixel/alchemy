typedef signed int s32;
typedef unsigned int u32;

extern u32 Data_020092d0;
extern u32 Data_020099d0;

extern void Func_020020da(s32, s32);
extern void Func_02001f4e(void);

void Func_02000fb4(s32 a)
{
    u32 *p = (u32 *)0x03001ed0;
    u32 b = *p;
    u32 src;
    u32 c;

    if (a != 0) {
        p = (u32 *)0x040000D4;
        src = (u32)&Data_020099d0;
    } else {
        p = (u32 *)0x040000D4;
        src = (u32)&Data_020092d0;
    }
    c = 0x840000e0;
    p[0] = src;
    p[1] = b;
    p[2] = c;
    Func_020020da(0x10000, 0);
    Func_02001f4e();
}
