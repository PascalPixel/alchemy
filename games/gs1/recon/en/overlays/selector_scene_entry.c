#include "types.h"

#define Scene_RunSelectorEntry Func_02003d20
#define SCENE Data_02000240
#define WORD(p, offset) (*(s32 *)((u8 *)(p) + (offset)))
#define WORK (*(u8 **)0x03001ebc)

extern s16 Data_02000240[];
void Func_02005a8c();
void Func_02004fcc();
void Func_02005c1c();
s32 Func_02005a84();
void Func_02005984();
void Func_02000dfc();
void Func_02005a1c();
void Func_02005c54();
u8 *Func_02005ad4();
void Func_02000030();
void Func_0200597c();
void Func_02005b24();
void Func_02005a94();
void Func_02000ba4();
void Func_02005a04();
void Func_020059e4();
void Func_02005b94();
void Func_02005c64();
void Func_020017a8();
void Func_02003788();
void Func_02002030();
void Func_02001db4();
void Func_02005b9c();
void Func_02005ab4();
void Func_02005b6c();
void Func_02005a2c();
void Func_02005bf4();
void Func_02005c04();
void Func_02005aac();
void Func_02005a3c();
void Func_02005a44();
void Func_02005bcc();
void Func_02005abc();
void Func_02005be4();
void Func_02005bdc();
void Func_02005bec();
void Func_02005b2c();
void Func_02005a5c();
void Func_02005a14();
void Func_02005bb4();
u8 *Func_02005bc4();
void Func_02004c68();
void Func_02005a0c();

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene number is reloaded after each group: called scripts may change it.
 * All three selector tables and their shared case tails belong to this owner. */
s32 Scene_RunSelectorEntry(void)
{
    u8 *actor;
    u32 i;
    s32 x;
    s32 flag;

    Func_02005a8c(0x111);
    SCENE[289] = 11;
    SCENE[288] = 0x39;
    WORD(WORK, 0x1c0) = 0x204;
    *(volatile u16 *)0x04000050 = 0x3f40;
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_02004fcc(21, 0x02001000);
    Func_02005c1c(0);
    if (Func_02005a84(0x875)) {
        Func_02005984(0x02008d59, 0xc80);
    } else {
        Func_02000dfc();
    }
    if (SCENE[224] == 0x36) {
        switch (SCENE[225]) {
        case 1:
        case 2:
        case 3:
            if (Func_02005a84(0x875)) {
                Call6(Func_02005a1c, 66, 5, 27, 23, 2, 5);
            }
            break;
        case 5:
        case 6:
            Func_02005c54(170);
            break;
        case 15:
            Func_02000030(Func_02005ad4(0), 1);
            /* fall through */
        case 4:
            if (Func_02005a84(0x876)) {
                Func_0200597c(1);
                Call3(Func_02005b24, 9, 0x2a00000, 0x800000);
                Call3(Func_02005b24, 10, 0x2c00000, 0xc00000);
                Call3(Func_02005b24, 11, 0x2880000, 0xf00000);
            } else if (!Func_02005a84(0x109)) {
                Func_02005a94(0x310);
                Func_02005a94(0x311);
                Func_02005a94(0x312);
                Func_02005a94(0x313);
            }
            Func_02000030(Func_02005ad4(9), 1);
            Func_02000030(Func_02005ad4(10), 1);
            Func_02000030(Func_02005ad4(11), 1);
            Func_02000ba4(9);
            Func_02000ba4(10);
            Func_02000ba4(11);
            Func_02000030(Func_02005ad4(12), 1);
            if (!Func_02005a84(0x310)) {
                break;
            }
            Call6(Func_02005a04, 119, 9, 109, 11, 1, 1);
            if (Func_02005a84(0x311)) {
                Call6(Func_02005a04, 118, 9, 104, 13, 1, 1);
            }
            Func_020059e4();
            Func_0200597c(1);
            break;
        case 9:
        case 10:
            if (!Func_02005a84(0x873)) {
                Call3(Func_02005b24, 3, 174 << 18, 158 << 18);
                Call3(Func_02005b94, 3, 0, 0);
            } else {
                Call3(Func_02005b24, 8, 194 << 18, 158 << 18);
                Call6(Func_02005a1c, 110, 39, 5, 1, 46, 39);
            }
            break;
        }
    }
    if (SCENE[224] == 0x37) {
        switch (SCENE[225]) {
        case 1:
        case 2:
            Func_02000030(Func_02005ad4(8), 1);
            Func_02000030(Func_02005ad4(9), 1);
            if (Func_02005a84(0x302)) {
                Func_0200597c(1);
                Func_02005c64(211);
                Call3(Func_02005b24, 8, 184 << 16, 132 << 18);
                Call6(Func_02005a1c, 11, 31, 1, 4, 9, 31);
                Call6(Func_02005a1c, 7, 30, 1, 4, 11, 31);
                Call6(Func_02005a04, 74, 58, 70, 32, 1, 2);
                Call6(Func_02005a04, 74, 59, 70, 34, 1, 1);
                Call6(Func_02005a04, 76, 60, 74, 38, 3, 1);
                Call6(Func_02005a04, 77, 60, 76, 38, 2, 1);
                Call6(Func_02005a04, 75, 58, 86, 41, 1, 3);
                Call6(Func_02005a04, 75, 59, 86, 43, 1, 2);
                Call6(Func_02005a04, 76, 59, 80, 49, 2, 1);
                Call6(Func_02005a04, 77, 59, 82, 49, 2, 1);
            }
            break;
        case 3:
        case 4:
            Func_020059e4();
            Func_0200597c(1);
            if (Func_02005a84(0x109) && Func_02005a84(0x256)) {
                Call6(Func_02005a04, 5, 2, 5, 11, 1, 1);
                Call6(Func_02005a04, 9, 1, 9, 7, 1, 2);
            }
            if (Func_02005a84(0x874)) {
                Call3(Func_02005b24, 11, 176 << 15, 216 << 16);
                WORD(Func_02005ad4(11), 12) -= 0x20000;
                actor = Func_02005ad4(11);
                WORD(actor, 60) = WORD(Func_02005ad4(11), 12);
                Call6(Func_02005a04, 9, 1, 9, 7, 1, 2);
                Call6(Func_02005a04, 5, 2, 5, 11, 1, 1);
                Call6(Func_02005a1c, 9, 5, 1, 1, 9, 10);
            }
            break;
        case 7:
        case 8:
        case 9:
            Func_02000030(Func_02005ad4(10), 1);
            if (Func_02005a84(0x306)) {
                Func_020017a8(0);
                Call6(Func_02005a1c, 42, 41, 4, 1, 42, 39);
                Call6(Func_02005a1c, 42, 40, 4, 1, 42, 41);
                Call3(Func_02005b24, 10, 176 << 18, 160 << 18);
            }
            break;
        }
    }
    if (SCENE[224] == 0x38) {
        switch (SCENE[225]) {
        case 4:
        case 5:
        case 6:
            Func_02000030(Func_02005ad4(15), 1);
            Func_02000030(Func_02005ad4(16), 1);
            Func_02000030(Func_02005ad4(17), 1);
            Func_02000030(Func_02005ad4(18), 1);
            Func_02000030(Func_02005ad4(19), 1);
            Func_02000030(Func_02005ad4(0), 1);
            x = 158 << 18;
            flag = 204 << 2;
            for (i = 0; i < 4; i++, x += 0x400000, flag += 2) {
                if (Func_02005a84(flag)) {
                    Call3(Func_02005b24, i + 15, x, 176 << 15);
                } else if (Func_02005a84(flag + 1)) {
                    Call3(Func_02005b24, i + 15, x + 0x200000, 176 << 15);
                }
            }
            if (Func_02005a84(0x338)) {
                Call3(Func_02005b24, 19, 230 << 18, 176 << 15);
            } else if (Func_02005a84(0x339)) {
                Call3(Func_02005b24, 19, 238 << 18, 176 << 15);
            } else {
                break;
            }
            Call6(Func_02005a1c, 53, 10, 1, 1, 58, 7);
            break;
        case 10:
        case 11:
            if (Func_02005a84(0x348)) {
                Call3(Func_02005b24, 20, 228 << 17, 164 << 17);
                Call6(Func_02005a1c, 29, 20, 1, 1, 31, 20);
            }
            break;
        case 3:
        case 13:
            Func_02003788();
            /* fall through */
        case 1:
        case 2:
        case 12:
            Func_02005c54(170);
            break;
        case 7:
        case 8:
        case 9:
            if (Func_02005a84(0x109) && Func_02005a84(0x256)) {
                WORD(Func_02005ad4(0), 12) = -0x20000;
                actor = Func_02005ad4(0);
                WORD(actor, 60) = WORD(Func_02005ad4(0), 12);
                Call6(Func_02005a04, 6, 29, 10, 23, 1, 1);
                Call6(Func_02005a04, 10, 28, 10, 18, 1, 2);
            }
            if (Func_02005a84(0x878)) {
                Call3(Func_02005b24, 8, 168 << 16, 188 << 17);
                WORD(Func_02005ad4(8), 12) -= 0x20000;
                actor = Func_02005ad4(8);
                WORD(actor, 60) = WORD(Func_02005ad4(8), 12);
                Call6(Func_02005a04, 6, 29, 10, 23, 1, 1);
                Call6(Func_02005a04, 10, 28, 10, 18, 1, 2);
                Call6(Func_02005a1c, 10, 16, 1, 1, 10, 19);
                Func_020059e4();
            }
            break;
        case 16:
            Func_0200597c(1);
            Call3(Func_02005b24, 10, 204 << 18, 152 << 18);
            Call3(Func_02005b24, 11, 194 << 18, 144 << 18);
            Func_02000030(Func_02005ad4(0), 1);
            Func_02002030(0);
            Func_02001db4(1);
            /* fall through */
        case 14:
            if (SCENE[225] == 14) {
                Func_02005c64(211);
            }
            Func_02000030(Func_02005ad4(9), 1);
            Func_02005b9c(10, 2);
            Func_02005ad4(10)[34] = 2;
            Func_02000030(Func_02005ad4(11), 1);
            Func_02000030(Func_02005ad4(12), 1);
            Func_02000030(Func_02005ad4(13), 1);
            Func_02000030(Func_02005ad4(14), 1);
            Func_02000ba4(10);
            Func_02000ba4(11);
            if (SCENE[225] != 14) {
                break;
            }
            if (!Func_02005a84(0x109)) {
                Func_02005a94(0x318);
                Func_02005a94(0x319);
                Func_02005a94(0x31a);
                Func_02005a94(0x31b);
                break;
            }
            Func_02002030(0);
            if (Func_02005a84(0x319)) {
                Func_02001db4(2);
                if ((WORD(Func_02005ad4(9), 16) >> 20) == 44) {
                    Func_02005984(0x0200a2c5, 0xc80);
                }
            } else if (Func_02005a84(0x31a)) {
                Func_02001db4(1);
            } else if (!Func_02005a84(0x31b)) {
                Func_02001db4(0);
            }
            break;
        case 15:
            Func_02005ab4();
            Func_02000030(Func_02005ad4(8), 1);
            Func_02005b6c(0, 15);
            Func_02005a2c(Func_02005ad4(0), 0);
            WORD(Func_02005ad4(8), 12) = 0x900000;
            Func_02005ad4(8)[85] = 0;
            WORD(Func_02005ad4(8), 68) = 0;
            WORD(Func_02005ad4(8), 72) = 0x4ccc;
            WORD(WORK, 0x1c0) = 0x100;
            Func_02005bf4();
            Func_02005c04();
            Func_02005ad4(8)[85] = 3;
            Func_02005c64(189);
            Func_02005aac(32);
            Func_02005c64(188);
            Func_02000030(Func_02005ad4(8), 2);
            Func_02005a3c(0x30000, 0x30000, 0x10000);
            Func_02005a3c(-1, -1, 0xe666);
            Func_02005a44();
            Func_02005aac(60);
            Func_02005bcc(16);
            Func_02005abc();
            break;
        }
    }
    if (SCENE[224] == 0x39) {
        switch (SCENE[225]) {
        case 10:
            Func_02005984(0x0200adcd, 0xc80);
            if (Func_02005a84(0x109)) {
                break;
            }
            Func_02003788();
            Func_02005c54(170);
            Func_02005be4(0x10000, 0);
            Func_02005bdc(0x10003, 1);
            Func_02005bec(30);
            Func_02005c04();
            Func_02005b2c(0, 1);
            Func_02005aac(30);
            Func_02005a5c(0x1633, 0, 0);
            Func_02005bdc(0x10000, 0);
            Func_02005bec(30);
            break;
        case 15:
            Call3(Func_02005b24, 3, 0, 0);
            Func_02005a14();
            *(volatile u16 *)0x04000050 = 0;
            WORD(Func_02005ad4(9), 24) = -0x10000;
            Func_02005b9c(14, 1);
            Func_02005b9c(15, 1);
            Func_02005b9c(16, 1);
            flag = Func_02005a84(0x109);
            if (flag) {
                Call6(Func_02005a1c, 104, 34, 5, 4, 40, 34);
                Call6(Func_02005a04, 45, 91, 40, 91, 5, 4);
                break;
            }
            Call3(Func_02005b24, 10, 206 << 18, 150 << 18);
            Call6(Func_02005a1c, 116, 36, 3, 4, 52, 36);
            Func_02005ad4(10)[85] = flag;
            Func_02000030(Func_02005ad4(10), 1);
            Func_02005ab4();
            Func_02005bb4(-1, -1, -1, 0);
            Func_02005bc4()[85] = flag;
            Func_02005b9c(0, 1);
            Func_02005b9c(13, 1);
            Func_02004c68();
            Func_02005a0c();
            Func_02005abc();
            break;
        case 12:
            Func_02005aac(1);
            Func_02000030(Func_02005ad4(0), 1);
            /* fall through */
        case 11:
            Call6(Func_02005a1c, 104, 34, 5, 4, 40, 34);
            Call6(Func_02005a04, 45, 91, 40, 91, 5, 4);
            if (!Func_02005a84(0x881)) {
                Call6(Func_02005a04, 30, 45, 50, 45, 5, 6);
                Call6(Func_02005a04, 50, 105, 50, 109, 5, 3);
                Func_020059e4();
                Func_0200597c(1);
            } else {
                Func_02005b9c(14, 1);
                Func_02005b9c(15, 1);
                Func_02005b9c(16, 1);
            }
            WORD(Func_02005ad4(9), 24) = -0x10000;
            if (!Func_02005a84(0x82b)) {
                Call3(Func_02005b24, 3, 0, 0);
                Call3(Func_02005b24, 10, 206 << 18, 150 << 18);
                Func_02005b9c(10, 1);
                Call6(Func_02005a1c, 116, 37, 3, 3, 52, 37);
                Call6(Func_02005a04, 126, 35, 116, 35, 1, 2);
                Func_02005984(0x0200a649, 0xc80);
                break;
            }
            if (!Func_02005a84(0x871)) {
                if (!Func_02005a84(0x870)) {
                    Call3(Func_02005b94, 3, 0, 0);
                    Func_02005b2c(3, 16);
                } else {
                    Call3(Func_02005b24, 3, 210 << 18, 158 << 18);
                    Call3(Func_02005b94, 3, 0xc000, 0);
                }
                Func_02000030(Func_02005ad4(3), 1);
                Call6(Func_02005a04, 126, 35, 116, 35, 1, 2);
                Func_02005984(0x0200a649, 0xc80);
            } else {
                Call3(Func_02005b24, 3, 0, 0);
                Call3(Func_02005b24, 10, 206 << 18, 150 << 18);
                Call6(Func_02005a1c, 116, 36, 3, 4, 52, 36);
                Func_02005ad4(10)[89] = 254;
                Func_02005aac(1);
            }
            Func_02005ad4(10)[85] = 0;
            Func_02000030(Func_02005ad4(10), 1);
            break;
        }
    }
    return 0;
}
