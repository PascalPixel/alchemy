#include "types.h"

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

extern Ctl Data_02001000;
extern u8 *Data_03001f3c;
extern u8 Data_0200bef1[];

extern s32 Func_02008e56(void);
extern void Func_02008e26(s32, s32);
extern s32 Func_02008f44(s32);
extern void Func_02008dfa(s32, s32);

void Func_0200457c(void)
{
    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_02008e26(Func_02008e56(), (s32)(state + 240));
    if (Func_02008f44(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_02008dfa((s32)Data_0200bef1, e);
    }
}
