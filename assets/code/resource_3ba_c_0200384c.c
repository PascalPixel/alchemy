typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

extern Ctl Data_02001000;
extern u8 *Data_03001f3c;
extern u8 Data_0200b1c1[];

extern s32 Func_020073ee(void);
extern void Func_020073be(s32, s32);
extern s32 Func_020074bc(s32);
extern void Func_02007392(s32, s32);

void Func_0200384c(void)
{
    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_020073be(Func_020073ee(), (s32)(state + 240));
    if (Func_020074bc(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_02007392((s32)Data_0200b1c1, e);
    }
}
