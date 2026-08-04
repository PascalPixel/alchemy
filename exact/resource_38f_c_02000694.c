#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000027;
extern u8 Value_00000026;
#define SCENE_POINTER (*(u8 *volatile *)0x03001ebc)

extern u8 Data_0200add8[];
extern u8 Data_0200b2d8[];

void Func_02002fbc(void);
s32 Func_0200311e();
void Func_020030dc();
s32 Func_0200312a();
void Func_020030e8();
s32 Func_02003136();
void Func_020030f4();
s32 Func_02003142();
void Func_02003100();
void Func_0200316a();
void Func_02003172();
void Func_0200317a();
void Func_02003182();
s32 Func_02003138();
s32 Func_0200317c();
void Func_0200313c();
void Func_0200314c();
void Func_0200315e();
s32 Func_02003190();
void Func_02001078(void);
s32 Func_020031ae();
void Func_020031f8();
void Func_020031fe();
void Func_02003204();
void Func_0200320a();
void Func_02003210();
void Func_02003216();
void Func_0200321c();
void Func_02003222();
void Func_02003228();

s32 Func_02000694(void)
{
    s16 step;
    u32 actor;
    u8 *tbl;

    step = Data_02000240[224];

    if (step == (s32)&Value_00000027) {
        Func_02002fbc();
        return 0;
    }

    if (step == (s32)&Value_00000026) {
        *(s32 *)(SCENE_POINTER + 0x1c0) = 0x204;
        return 0;
    }

    Func_020030dc(Func_0200311e(23), 0);
    Func_020030e8(Func_0200312a(24), 0);
    Func_020030f4(Func_02003136(25), 0);
    Func_02003100(Func_02003142(26), 0);

    tbl = Data_0200add8;
    Func_0200316a(23, tbl);
    Func_02003172(24, tbl);
    Func_0200317a(25, tbl);
    Func_02003182(26, tbl);

    if (Func_02003138(0x845) == 0) {
        for (actor = 8; actor <= 16; actor++) {
            Func_0200313c(Func_0200317c(actor), 0);
        }
        Func_0200314c(13, 9, 1, 1, 13, 8);
        Func_0200315e(13, 9, 1, 1, 15, 8);
        {
            s32 m = 14;
            s32 n = 9;
            Func_02003172(13, 9, 1, 1, m, n);
        }
    }

    if (Func_02003190(0x843) == 0) {
        if (Data_02000240[225] == 1) {
            Func_02001078();
        }
    }

    if (Func_020031ae(0x843) != 0) {
        Func_020031f8(1);
        Func_020031fe(2);
        Func_02003204(3);
        Func_0200320a(17);
        Func_02003210(18);
        Func_02003216(19);
        Func_0200321c(20);
        Func_02003222(21);
        Func_02003228(22);
        Func_02003216(Data_0200b2d8);
    }

    return 0;
}
