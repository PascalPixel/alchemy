#include "TYPES.H"

#define FieldScene_RunSceneEntryHook Func_02002500

extern u8 Data_02000240[];

extern u8 Value_00000044;
extern u8 Value_00000045;
extern u8 Value_00000046;

extern u8 Data_03001ebc[];

s32 Func_020052d0();
void Func_020052f0();
void Func_0200523e();
void Func_0200537e();
void Func_0200535c();
void Func_020053a8();
void Func_02005342();
void Func_02005364();
void Func_02005420();
void Func_0200542c();
void Func_02005382();
void Func_02005338();
void Func_020053ec();
void Func_020053bc();
void Func_02005352();
void Func_02003354();
void Func_020033e2();
u8 *Func_02005394();
void Func_0200533a();
void Func_02005378();
void Func_02005386();
void Func_0200539a();
s32 Func_02005380();
s32 Func_0200538e();
void Func_02005434();
void Func_02005412();
u8 *Func_020053e2();
void Func_02005428();
s32 Func_020053d0();
u8 *Func_0200540a();
void Func_02005346();
void Func_0200545c();
s32 Func_020053fa();
void Func_02005284();
void Func_020054a4();
u8 *Func_0200544a();
void Func_020053f0();
u8 *Func_02005456();
void Func_020053fc();
void Func_02002f66();
s32 Func_0200543a();
void Func_02002f78();
void Func_020053ac();
void Func_020054c2();
void Func_020054d2();
void Func_02005436();
s32 Func_020054b4();
void Func_02005558();
u8 *Func_020054f6();
void Func_0200549c();
s32 Func_020054d4();
void Func_0200557a();
s32 Func_020054ec();
void Func_0200302a();
void Func_0200545e();
void Func_02005574();
void Func_02005584();
u8 *Func_0200554a();
void Func_020054f4();
s32 Func_02002a00(u32, s32, s32, u32, u32, s32);
void Func_02003082();
void Func_02003088();
u8 *Func_0200558a();
u8 *Func_02005592();
s32 Func_02002a32(u32, s32, s32, u32, u32, s32);
void Func_020054e0();
void Func_02005620();
s32 Func_0200559e();
void Func_02005644();
u8 *Func_020055e2();
void Func_02005588();
void Func_0200563a();
void Func_02005648();
void Func_020055d4();
s32 Func_0200567a();
void Func_0200571e();
u8 *Func_020056bc();
void Func_02005662();
u8 *Func_020056c8();
void Func_0200566e();
s32 Func_020056a6();
void Func_0200574a();
u8 *Func_020056e8();
void Func_0200568e();
u8 *Func_020056f4();
void Func_0200569a();
s32 Func_020056d0();
void Func_02005776();
u8 *Func_02005714();
void Func_020056ba();
u8 *Func_02005720();
void Func_020056c6();
u8 *Func_0200572e();
u8 *Func_02005736();
s32 Func_02002bd6(u32, s32, s32, u32, u32, s32);
void Func_020057be();
void Func_0200568c();
u8 *Func_02005762();
void Func_02003276();
u8 *Func_02003414(s32, s32, s32, s32);
u8 *Func_02003420(s32, s32, s32, s32);
void Func_0200329a();
void Func_020032a0();
s32 Func_02005772();
void Func_020032b0();
void Func_020056e4();
void Func_020057fc();
void Func_02005800();
u8 *Func_020057ce();
void Func_0200577c();
void Func_020057cc();
void Func_0200583a();
u8 *Func_02005808();
void Func_020057ae();
s32 Func_020057e4();
void Func_02003322();
void Func_02005756();
void Func_0200586e();
void Func_02005870();
u8 *Func_0200583e();
void Func_020057e8();
u8 *Func_0200585e();
u8 *Func_02005866();
s32 Func_02002d06(u32, s32, s32, u32, u32, s32);
void Func_020058ee();
void Func_020057bc();
u8 *Func_02005892();
s32 Func_02005872();
void Func_02005916();
u8 *Func_020058b4();
void Func_0200585a();
s32 Func_02005890();
void Func_0200590a();
void Func_02005918();
void Func_0200475c();
void Func_020058fe();
s32 Func_020058c4();
s32 Func_020058ce();
void Func_020058e0();
void Func_020058e8();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Stores 0x204 in the scene work word at 448, then by scene (0x44, 0x45 or
 * 0x46) and entry number applies the flag-dependent actor placements and states
 * for that entry. Returns 0. */
s32 FieldScene_RunSceneEntryHook(void)
{
    u8 *rec;
    s32 mode;
    s32 step;
    s32 pos;
    s32 tmp;

    {
        u8 *work = *(u8 **)Data_03001ebc;
        s32 off = 448;

        *(s32 *)(work + off) = 0x204;
        mode = *(s16 *)(Data_02000240 + off);
    }
    if (mode == (s32)&Value_00000044) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (Value1(Func_020052d0, 0x89c) == 0) {
                Func_020052f0();
                Func_0200523e(1);
                Func_0200537e(10, 1);
                Call3(Func_0200535c, 10, 0x5c0000, 0x780000);
                Call3(Func_020053a8, 10, 0xd000, 0);
                Call3(Func_02005342, 0, 0x6666, 0x3333);
                Call3(Func_02005364, 0, 136, 64);
                Func_02005420();
                Func_0200542c();
                Func_02005382(0);
                Func_02005338(30);
                Call3(Func_020053ec, 10, 256, 0);
                Func_020053bc(10, 2);
                Func_02005352(30);
                Func_02003354(10, 136, 116, 0x70000);
                Func_020033e2(10);
                Call2(Func_02005434, 10, 15);
                Func_0200533a(Func_02005394(10), 0);
                Call1(Func_02005378, 0x89c);
                Func_02005386(60);
                Func_0200539a();
            }
            if (Func_02005380(0x109) == 0) {
                break;
            }
            if (Func_0200538e(768) != 0) {
                break;
            }
            Func_02005434(10, 15);
            Call3(Func_02005412, 10, 0x880000, 0x740000);
            break;

        case 7:
        case 8:
        case 9:
            rec = Func_020053e2(0);
            if (rec != 0) {
                Func_02005428(16, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
            }
            rec = Func_02005394(16);
            *(s32 *)(rec + 108) = 0;
            if (Func_020053d0(0x109) != 0) {
                rec = Func_0200540a(16);
                *(s32 *)(rec + 12) = 0x200000;
            }
            Func_02005346(1);
            Call3(Func_0200545c, 16, 0x2780000, 0x1b80000);
            if (Func_020053fa(0xfd4) == 0) {
                Func_02005284(16);
            }
            Func_020054a4(11, 15);
            Call2(Func_020054a4, 12, 15);
            Func_020053f0(Func_0200544a(11), 0);
            Func_020053fc(Func_02005456(12), 0);
            Func_02002f66(8);
            if (Func_0200543a(784) == 0) {
                Func_02002f78(9);
                break;
            }
            Func_020053ac(1);
            Call3(Func_020054c2, 9, 0x2100000, 0x1980000);
            Func_020054d2(9, 4);
            Call6(Func_02005436, 38, 27, 4, 2, 31, 25);
            *(u8 *)(Func_0200544a(9) + 35) = 2;
            break;
        }
    } else if (mode == (s32)&Value_00000045) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (Func_020054b4(0x303) == 0) {
                Call2(Func_02005558, 12, 15);
                Func_0200549c(Func_020054f6(12), 0);
            }
            if (Func_020054d4(772) != 0) {
                break;
            }
            Call2(Func_0200557a, 13, 15);
            Func_020056c6(Func_02005720(13), 0);
            break;

        case 10:
        case 11:
        case 12:
            if (Func_020054ec(0x311) == 0) {
                Func_0200302a(10);
            } else {
                s32 attr = 0;

                Func_0200545e(1);
                Call3(Func_02005574, 10, 0x2280000, 0x1fe0000);
                Call2(Func_02005584, 10, 4);
                *(u8 *)(Func_0200554a(10) + 35) = 2;
                Call6(Func_020054f4, 44, 30, 2, 4, 34, 30);
                Func_02002a00(0, 35, 29, 1, 4, attr);
            }
            Func_02003082(8);
            Func_02003088(9);
            pos = *(s32 *)(Func_0200558a(11) + 8);
            tmp = *(s32 *)(Func_02005592(11) + 16);
            pos >>= 20;
            Func_02002a32(2, pos, tmp >> 20, 1, 1, 255);
            Func_020054e0(1);
            Func_02005620(11, 6);
            {
                u8 *obj = Func_0200558a(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Func_0200559e(0x306) != 0) {
                break;
            }
            Call2(Func_02005644, 14, 15);
            Func_02005588(Func_020055e2(14), 0);
            if (Func_0200559e(0x305) == 0) {
                break;
            }
            Call3(Func_0200563a, 14, 0x1a80000, 0x1e00000);
            Call3(Func_02005648, 17, 0x1a80000, 0x1e00000);
            break;
        }
    } else if (mode == (s32)&Value_00000046) {
        {
            s32 off = 450;

            step = *(s16 *)(Data_02000240 + off);
        }
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            Func_020055d4(1);
            if (Func_0200567a(0x307) == 0) {
                Call2(Func_0200571e, 15, 15);
                Func_02005662(Func_020056bc(15), 0);
                Func_0200566e(Func_020056c8(19), 0);
            }
            if (Func_020056a6(776) == 0) {
                Call2(Func_0200574a, 16, 15);
                Func_0200568e(Func_020056e8(16), 0);
                Func_0200569a(Func_020056f4(20), 0);
            }
            if (Func_020056d0(0x309) != 0) {
                break;
            }
            Call2(Func_02005776, 17, 15);
            Func_020056ba(Func_02005714(17), 0);
            Func_020056c6(Func_02005720(21), 0);
            break;

        case 7:
            pos = *(s32 *)(Func_0200572e(13) + 8);
            tmp = *(s32 *)(Func_02005736(13) + 16);
            pos >>= 20;
            Func_02002bd6(2, pos, tmp >> 20, 1, 1, 255);
            Call2(Func_020057be, 13, 6);
            Func_0200568c(1);
            {
                u8 *obj = Func_02005762(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            Func_02003276(8);
            break;

        case 8:
        case 9:
        case 10:
        case 11:
            Func_02003414(0x2de0000, 0, 0x1720000, 223);
            Func_02003420(0x2f20000, 0, 0x1720000, 223);
            Func_0200329a(10);
            Func_020032a0(12);
            if (Func_02005772(0x312) == 0) {
                Func_020032b0(9);
            } else {
                Func_020056e4(1);
                Func_020057fc(9, 4);
                Call3(Func_02005800, 9, 0x2ba0000, 0x18e0000);
                {
                    u8 *obj = Func_020057ce(9);
                    u32 mask = 2;
                    mask = mask | obj[35];
                    obj[35] = mask;
                }
                Call6(Func_0200577c, 26, 20, 2, 4, 42, 23);
                Func_020057cc(532);
                Call3(Func_0200583a, 14, 0x2780000, 0x1b80000);
                Func_020057ae(Func_02005808(14), 0);
            }
            if (Func_020057e4(0x313) == 0) {
                Func_02003322(11);
            } else {
                Func_02005756(1);
                Func_0200586e(11, 4);
                Call3(Func_02005870, 11, 0x29a0000, 0x2260000);
                *(u8 *)(Func_0200583e(11) + 35) = 2;
                Call6(Func_020057e8, 26, 20, 2, 4, 40, 32);
            }
            pos = *(s32 *)(Func_0200585e(14) + 8);
            tmp = *(s32 *)(Func_02005866(14) + 16);
            pos >>= 20;
            Func_02002d06(2, pos, tmp >> 20, 1, 1, 255);
            Call2(Func_020058ee, 14, 6);
            Func_020057bc(1);
            {
                u8 *obj = Func_02005892(9);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Func_02005872(0x30b) == 0) {
                Call2(Func_02005916, 18, 15);
                Func_0200585a(Func_020058b4(18), 0);
                if (Func_02005890(0x30a) != 0) {
                    Call3(Func_0200590a, 22, 0x2e80000, 0x1f80000);
                    Call3(Func_02005918, 18, 0x2e80000, 0x1f80000);
                }
            }
            Func_0200475c();
            break;

        case 12:
        case 13:
            Func_020058fe(18, 0x200b084);
            if (Func_020058c4(0x893) == 0) {
                break;
            }
            if (Func_020058ce(0x89e) == 0) {
                break;
            }
            Func_020058e0(0x88f);
            break;

        case 15:
            Func_020058e8(0x89e);
            break;
        }
    }
    return 0;
}
