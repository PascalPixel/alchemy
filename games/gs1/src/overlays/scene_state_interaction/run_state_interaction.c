#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;

extern void Func_02006084(void);
extern void Func_02005ebc(s32, s32);
extern void Func_02006002(s32);
extern void Func_0200601a(s32, s32);
extern s32 Func_02005f2c(s32);
extern s32 Func_02005f3c(s32);
extern s32 Func_02005f1e(s32);
extern void Func_02005f6c(s32);
extern void Func_02006052(s32);
extern s32 Func_02006062(s32, s32);
extern s32 Func_02005fda(s32, s32);

s32 Func_02001ffc(s32 a, s32 b)
{
    s32 v;
    s32 id;
    s32 r;

    Func_02006084();
    Func_02005ebc(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02006002(id);
    Func_0200601a(a, 0);
    if (Func_02005f2c(b + 512) != 0) {
        return 2;
    }
    if (Func_02005f3c(b + 520) != 0) {
        r = Func_02005f1e(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    Func_02005f6c(b + 520);
    Func_02006052((s32)&Value_0000207c);
    Func_02006062(a, 0);
    return Func_02005fda(0, 0);
}
