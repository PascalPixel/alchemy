#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;

extern void Func_0200760c(void);
extern void Func_0200741c(s32, s32);
extern void Func_0200757a(s32);
extern void Func_02007592(s32, s32);
extern s32 Func_0200748c(s32);
extern s32 Func_0200749c(s32);
extern s32 Func_0200747e(s32);
extern void Func_020074cc(s32);
extern void Func_020075ca(s32);
extern s32 Func_020075da(s32, s32);
extern s32 Func_0200753a(s32, s32);

s32 Func_02002a94(s32 a, s32 b)
{
    s32 v;
    s32 id;
    s32 r;

    Func_0200760c();
    Func_0200741c(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_0200757a(id);
    Func_02007592(a, 0);
    if (Func_0200748c(b + 512) != 0) {
        return 2;
    }
    if (Func_0200749c(b + 520) != 0) {
        r = Func_0200747e(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    Func_020074cc(b + 520);
    Func_020075ca((s32)&Value_0000207c);
    Func_020075da(a, 0);
    return Func_0200753a(0, 0);
}
