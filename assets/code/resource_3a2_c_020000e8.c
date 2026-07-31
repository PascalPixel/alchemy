typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Data_03001ebc;

extern void Func_020015dc(void);
extern void Func_02001672(s32);
/* Returns a value: the reference sets r1 before r0 at this site, which only a
 * value-returning callee does. */
extern s32 Func_02001682(s32, s32);
extern s32 Func_02001602(s32, s32);
extern void Func_020015f4(s32);
extern void Func_020016a4(s32, s32);
extern void Func_020016ce(s32, s32);
extern void Func_0200162a(void);

void Func_020000e8(void)
{
    u8 *workspace;

    Func_020015dc();
    Func_02001672(0x1958);
    Func_02001682(10, 0);

    if (Func_02001602(0, 0) == 1) {
        Func_020015f4(20);
        Func_020016a4(10, 0);
    } else {
        workspace = Data_03001ebc;
        *(u16 *) (workspace + 472) += 1;
        Func_020016ce(10, 0);
    }

    Func_0200162a();
}
