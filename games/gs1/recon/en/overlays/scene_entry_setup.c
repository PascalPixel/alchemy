#include "types.h"
#include "configured_effect_spawn.h"

/* Resource-local call aliases are bound to loader runtime destinations by
 * this owner's translation unit.  The five selectors are signed halfwords
 * in the shared scene record; switch tables and the final pool belong to
 * the complete entry setup function. */
struct Effect *Func_02000058(s32, s32, s32, s32);
void Func_02000558(void);
void Func_020007d8(void);
void Func_0200094c(u8 *);
void Func_020009c8(void);
void Func_020018f8(void);
void Func_020019c0(void);
void Func_02001a14(u8 *);
void Func_02001d48(void);
void Func_02001f60(void);
void Func_020022c8(s32);
void Func_02002b14(void);
void Func_0200300c(s32);
void Func_020042bc(void);
void Func_02004610(void);
void Func_020047c0(void);
void Func_02004a2c(void);
void Func_02004da4(s32);
void Func_02004dac(s32, s32);
void Func_02004e1c(s32, s32, s32, s32, s32, s32);
void Func_02004e24(void);
void Func_02004e2c(s32, s32, s32, s32, s32, s32);
void Func_02004e34(s32, s32, s32, s32, s32, s32);
void Func_02004e44(u8 *, s32);
s32 Func_02004e74(s32);
void Func_02004e7c(s32);
void Func_02004e84(s32);
void Func_02004e8c(s32);
u8 *Func_02004eac(s32);
void Func_02004ef4(s32, s32, s32);
void Func_02004efc(s32, s32);
void Func_02004f64(s32, s32);
void Func_02004fc4(void);
void Func_02004fd4(void);
void Func_02004fdc(s32);
void Func_02004fec(s32);
extern s16 Data_02000240[];
extern u8 Value_000000b5;
extern u8 Value_000000b6;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;

/* The six-argument scene operations share the calling form already verified
 * in this overlay's primary scene sequences. */
static __inline__ void Scene_Call6(void (*func)(), s32 a0, s32 a1,
    s32 a2, s32 a3, s32 a4, s32 a5)
{
    func(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Scene_Call3(void (*func)(), s32 a0, s32 a1, s32 a2)
{
    func(a0, a1, a2);
}

#define Func_02004ef4(a,b,c) Scene_Call3(Func_02004ef4,a,b,c)
#define Func_02004e1c(a,b,c,d,e,f) Scene_Call6(Func_02004e1c,a,b,c,d,e,f)
#define Func_02004e2c(a,b,c,d,e,f) Scene_Call6(Func_02004e2c,a,b,c,d,e,f)
#define Func_02004e34(a,b,c,d,e,f) Scene_Call6(Func_02004e34,a,b,c,d,e,f)

#define WORD(p, n) (*(s32 *)((p) + (n)))
#define HALF(p, n) (*(u16 *)((p) + (n)))
#define SCENE Data_02000240
#define WORK (*(u8 **)0x03001ebc)

s32 Scene_RunEntrySetup(void)
{
    u8 *actor;
    u32 i;
    s32 x, z;

    Func_02004e8c(1);
    if (Func_02004e74(0x109))
        Func_02000558();
    Func_02004e7c(0x110);
    WORD(WORK, 0x1c0) = 0x204;
    if (SCENE[224] == (s32)&Value_000000b5) {
        WORD(WORK, 0x1c0) = 0x100;
        if (!Func_02004e74(0x981))
            Func_0200300c(8);
        else
            Func_02004e34(7, 17, 2, 1, 7, 16);
        Func_0200300c(9);
        Func_0200300c(10);
        Func_0200300c(11);
        Func_02004f64(11, 2);
        Func_0200300c(12);
        Func_02004f64(12, 2);
        Func_0200300c(13);
        Func_0200300c(14);
    } else if (SCENE[224] == (s32)&Value_000000b6) {
        switch (SCENE[225]) {
        case 1: case 2:
            Func_0200300c(8);
            break;
        case 5:
            Func_02004e84(0x120);
        case 3: case 4: case 6:
            Func_0200300c(9);
            break;
        case 20: case 21:
            if (Func_02004e74(0x982)) {
                Func_02004e1c(103, 27, 89, 27, 7, 8);
                Func_02004e1c(41, 90, 27, 92, 3, 2);
                Func_02004e1c(41, 90, 29, 93, 3, 2);
                Func_02004e1c(41, 90, 27, 94, 3, 2);
                Func_02004e1c(41, 90, 27, 96, 3, 2);
                Func_02004e1c(41, 90, 29, 97, 3, 2);
                Func_02004e1c(41, 96, 25, 91, 3, 2);
                Func_02004e1c(41, 92, 25, 93, 3, 2);
                Func_02004e1c(41, 96, 25, 95, 3, 2);
                Func_02004e1c(41, 96, 25, 97, 3, 2);
                Func_02004e1c(41, 96, 27, 96, 3, 2);
                Func_02004e1c(41, 96, 29, 97, 3, 2);
            } else if (Func_02004e74(0x983)) {
                Func_02004e1c(111, 27, 89, 27, 7, 8);
                Func_02004e1c(41, 90, 25, 91, 3, 2);
                Func_02004e1c(41, 90, 25, 93, 3, 2);
                Func_02004e1c(41, 90, 25, 95, 3, 2);
                Func_02004e1c(41, 90, 25, 97, 3, 2);
                Func_02004e1c(41, 90, 27, 96, 3, 2);
                Func_02004e1c(41, 90, 29, 97, 3, 2);
                Func_02004e1c(41, 94, 27, 92, 3, 2);
                Func_02004e1c(41, 96, 29, 93, 3, 2);
                Func_02004e1c(41, 94, 27, 94, 3, 2);
                Func_02004e1c(41, 96, 27, 96, 3, 2);
                Func_02004e1c(41, 96, 29, 97, 3, 2);
            }
            break;
        case 26:
            Func_02004e84(0x121);
        case 22: case 23:
            Func_02004e84(0x12f);
            if (Func_02004e74(0x200))
                Func_02004e1c(44, 117, 41, 117, 3, 5);
            break;
        case 7: case 8: case 9: case 10: case 11:
            if (Func_02004e74(0x987)) {
                Func_02004ef4(12, 0x3680000, 0x580000);
                actor = Func_02004eac(12);
                WORD(actor, 12) = -0x180000;
                WORD(actor, 60) = 0x80000000;
            }
            break;
        }
    } else if (SCENE[224] == (s32)&Value_000000b7) {
        switch (SCENE[225]) {
        case 16:
            Func_02004e84(0x12f);
            break;
        case 21:
            Func_020042bc();
            break;
        case 20:
            Func_02004fec(170);
            if (!Func_02004e74(0x109))
                Func_020009c8();
            break;
        case 9: case 10:
            actor = Func_02004eac(11);
            Func_020019c0();
            if ((WORD(actor, 8) >> 20) == 8)
                Func_02001a14(actor);
            actor = Func_02004eac(12);
            if ((WORD(actor, 8) >> 20) == 7)
                Func_02001a14(actor);
            Func_02000058(0xce0000, 0, 0x1c10000, 223);
            Func_02000058(0xd20000, 0, 0x1c10000, 223);
            break;
        case 7: case 8:
            Func_02004fdc(0);
            Func_02004da4(2);
            actor = Func_02004eac(8);
            actor[85] = 0;
            WORD(actor, 108) = 0x020088c9;
            actor = Func_02004eac(9);
            actor[85] = 0;
            WORD(actor, 108) = 0x020088c9;
            actor = Func_02004eac(10);
            actor[85] = 0;
            WORD(actor, 108) = 0x020088c9;
            Func_020018f8();
            break;
        case 11: case 12: case 13: case 14: case 15:
            Func_02004fec(170);
            Func_02004fdc(0);
            Func_02004da4(2);
            if (Func_02004e74(0x300)) {
                Func_02004e1c(111, 5, 117, 5, 5, 2);
                Func_02004e1c(111, 10, 117, 10, 5, 2);
                Func_02004e1c(111, 7, 111, 5, 5, 2);
                Func_02004e1c(111, 7, 111, 10, 5, 2);
                Func_02004e2c(48, 3, 3, 10, 54, 3);
                Func_02004e2c(55, 26, 3, 10, 48, 3);
            }
            break;
        case 1: case 2: case 3: case 4: case 19:
            goto play_cue;
        }
    } else if (SCENE[224] == (s32)&Value_000000b8) {
        switch (SCENE[225]) {
        case 2:
            Func_020007d8();
            goto play_cue;
        case 4: case 6:
            Func_02004fdc(0);
            break;
        case 9: case 10:
            actor = Func_02004eac(8);
            actor[85] = 0;
            WORD(actor, 12) = 0;
            actor = Func_02004eac(9);
            actor[85] = 0;
            actor[89] = 0;
            if (Func_02004e74(0x301)) {
                Func_02004da4(1);
                Func_02004e1c(124, 41, 110, 41, 1, 2);
                Func_02004e34(46, 41, 1, 1, 46, 42);
                Func_02004ef4(9, 0x2e80000, 0x2d80000);
                actor[85] = 0;
                WORD(actor, 12) = -0x100000;
                Func_02004f64(9, 3);
                actor[35] = 2;
                Func_02004e34(45, 45, 1, 1, 46, 45);
                Func_02004efc(10, 7);
                Func_02004f64(10, 1);
                actor = Func_02004eac(10);
                actor[89] = 0;
                actor[35] = 2;
                Func_02004ef4(10, 0x2e70000, 0x2b80000);
                WORD(actor, 108) = 0x02008b99;
            }
            Func_02001d48();
            break;
        case 11:
            WORD(WORK, 0x1c0) = 0x202;
            WORD(Func_02004eac(0), 12) = -0x20000;
        case 7: case 8:
            Func_02004fec(170);
            Func_02004e24();
            *(volatile u16 *)0x04000050 = 0;
            if (Func_02004e74(0x300)) {
                Func_02004e1c(15, 96, 9, 96, 3, 3);
                Func_02004e1c(12, 96, 15, 96, 3, 3);
                Func_02004e1c(5, 50, 15, 32, 3, 4);
                Func_02004e1c(25, 45, 9, 32, 3, 4);
                Func_02004e34(15, 32, 3, 1, 9, 32);
                Func_02004e34(12, 32, 3, 1, 15, 32);
            }
            if (SCENE[225] == 11) {
                Func_02004fc4();
                Func_02004fd4();
                WORD(WORK, 0x1c0) = 0x204;
            }
            break;
        case 1:
            goto play_cue;
        }
    } else if (SCENE[224] == (s32)&Value_000000b9) {
        switch (SCENE[225]) {
        case 19:
            Func_02004610();
            break;
        case 9: case 10: case 11: case 12: case 13: case 14: case 20:
            Func_02004fec(170);
            if (Func_02004e74(0x306)) {
                Func_02004e2c(53, 12, 3, 13, 26, 12);
                Func_02004e1c(81, 41, 89, 14, 9, 2);
                Func_02004da4(1);
                Func_02004dac(0x0200c5f1, 3200);
            }
            if (Func_02004e74(0x307)) {
                Func_02004e2c(58, 12, 3, 13, 34, 12);
                Func_02004e1c(81, 41, 97, 14, 5, 2);
                Func_02004da4(1);
                Func_02004dac(0x0200c601, 3200);
            }
            if (SCENE[225] == 11)
                Func_020007d8();
            else if (SCENE[225] == 20)
                Func_020047c0();
            break;
        case 4: case 5:
            Func_020007d8();
            goto play_cue;
        case 15: case 16:
            actor = Func_02004eac(8);
            actor[85] = 0;
            WORD(actor, 12) = 0;
            Func_02004eac(9)[85] = 0;
            Func_02004eac(10)[85] = 0;
            Func_02004eac(11)[85] = 0;
            if (Func_02004e74(0x304)) {
                Func_02004da4(1);
                Func_02004e1c(111, 59, 109, 37, 1, 2);
                Func_02004e34(45, 37, 1, 1, 45, 38);
                if (Func_02004e74(0x302)) {
                    Func_02004ef4(9, 0x3080000, 0x2980000);
                    Func_02004ef4(10, 0x3480000, 0x2980000);
                    Func_02004ef4(11, 0x3080000, 0x2b80000);
                } else {
                    Func_02004ef4(9, 0x3480000, 0x2980000);
                    Func_02004ef4(10, 0x3080000, 0x2b80000);
                    Func_02004ef4(11, 0x3480000, 0x2b80000);
                }
                Func_02004f64(9, 3);
                actor = Func_02004eac(9);
                WORD(actor, 12) = -0x100000;
                actor[35] = 2;
                Func_02004f64(10, 3);
                actor = Func_02004eac(10);
                WORD(actor, 12) = -0x100000;
                actor[35] = 2;
                Func_02004f64(11, 3);
                actor = Func_02004eac(11);
                WORD(actor, 12) = -0x100000;
                actor[35] = 2;
                Func_02004efc(12, 7);
                Func_02004e44(Func_02004eac(12), 0);
                Func_02004f64(12, 1);
                actor = Func_02004eac(12);
                actor[89] = 0;
                actor[35] = 2;
                Func_02004ef4(12, 0x2d70000, 0x2780000);
                WORD(actor, 108) = 0x02008b99;
            }
            Func_02001f60();
            break;
        case 6:
            goto play_cue;
        }
    } else if (SCENE[224] == (s32)&Value_000000ba) {
        switch (SCENE[225]) {
        case 1: case 2:
            if (Func_02004e74(0x109)) {
                Func_020022c8(0);
                Func_0200094c(Func_02004eac(0));
                for (i = 0; i <= 3; i++) {
                    actor = Func_02004eac(i + 10);
                    x = WORD(actor, 8) >> 20;
                    if (x == 13) {
                        z = WORD(actor, 16) >> 20;
                        if (z == 7 && Func_02004e74(i + 0x200))
                            goto place_actor;
                    }
                }
            } else {
                actor = Func_02004eac(8);
                actor[85] = 0;
                WORD(actor, 12) = -0x300000;
                actor[35] |= 2;
                actor[89] &= 254;
                HALF(actor, 100) = 3;
                Func_02004f64(8, 1);
                actor = Func_02004eac(9);
                actor[85] = 0;
                WORD(actor, 12) = -0x300000;
                actor[35] |= 2;
                actor[89] &= 254;
                HALF(actor, 100) = 3;
                Func_02004f64(9, 1);
                actor = Func_02004eac(10);
                actor[85] = 0;
                HALF(actor, 100) = 0;
                Func_02004e44(Func_02004eac(10), 0);
                actor = Func_02004eac(11);
                actor[85] = 0;
                HALF(actor, 100) = 0;
                Func_02004e44(Func_02004eac(11), 0);
                actor = Func_02004eac(12);
                actor[85] = 0;
                HALF(actor, 100) = 0;
                Func_02004e44(Func_02004eac(12), 0);
                actor = Func_02004eac(13);
                actor[85] = 0;
                HALF(actor, 100) = 0;
                Func_02004e44(Func_02004eac(13), 0);
            }
            break;
        case 13: case 14:
play_cue:
            Func_02004fec(170);
            goto done;
        case 18: case 19:
            WORD(WORK, 0x1c0) = 0x202;
            WORD(Func_02004eac(0), 12) = -0x20000;
        case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 10: case 11: case 12: case 20:
            Func_02004eac(20)[85] = 4;
            Func_02004eac(20)[35] |= 2;
            WORD(Func_02004eac(20), 12) = -0x108000;
            Func_02004e24();
            *(volatile u16 *)0x04000050 = 0;
            if (Func_02004e74(0x306)) {
                Func_02004fec(170);
                Func_02004e1c(36, 81, 32, 81, 3, 2);
                Func_02004e1c(36, 83, 36, 81, 3, 2);
                Func_02004e34(36, 17, 3, 1, 32, 17);
                Func_02004e34(36, 18, 3, 1, 36, 17);
                Func_02004e1c(63, 29, 33, 20, 1, 1);
                Func_02004e1c(20, 56, 36, 17, 3, 4);
            }
            if (Func_02004e74(0x307)) {
                Func_02004e1c(44, 81, 48, 81, 3, 2);
                Func_02004e1c(44, 83, 44, 81, 3, 2);
                Func_02004e34(44, 17, 3, 1, 48, 17);
                Func_02004e34(44, 18, 3, 1, 44, 17);
                Func_02004e1c(63, 29, 49, 20, 1, 1);
                Func_02004e1c(41, 56, 44, 17, 3, 4);
            }
            if ((u16)(SCENE[225] - 18) <= 1) {
                Func_02004fc4();
                Func_02004fd4();
                WORD(WORK, 0x1c0) = 0x204;
            }
            if ((u16)SCENE[225] == 20)
                Func_02004a2c();
            break;
place_actor:
            actor[35] |= 2;
            actor[89] = 0;
            actor[85] = 0;
            Func_02004e34(4, 19, 1, 1, x, z);
            goto done;
        case 15: case 16:
            Func_02004da4(1);
            Func_02004dac(0x0200b051, 3200);
            actor = Func_02004eac(14);
            actor[85] = 0;
            WORD(actor, 12) = 0;
            Func_02004eac(15)[85] = 0;
            Func_02004eac(16)[85] = 0;
            Func_02004eac(17)[85] = 0;
            Func_02004eac(18)[85] = 0;
            if (Func_02004e74(0x308)) {
                Func_02004da4(1);
                Func_02004e1c(95, 56, 77, 35, 1, 2);
                Func_02004e34(13, 35, 1, 1, 13, 36);
                Func_02004ef4(15, 0x1080000, 0x2e80000);
                actor = Func_02004eac(15);
                WORD(actor, 12) = -0x100000;
                actor[35] = 2;
                Func_02004f64(15, 3);
                Func_02004ef4(16, 0xb80000, 0x2780000);
                actor = Func_02004eac(16);
                WORD(actor, 12) = -0x100000;
                actor[35] = 2;
                Func_02004f64(16, 3);
                Func_02004ef4(17, 0xe80000, 0x2b80000);
                actor = Func_02004eac(17);
                WORD(actor, 12) = -0x100000;
                actor[35] = 2;
                Func_02004f64(17, 3);
                Func_02004ef4(18, 0xb80000, 0x2980000);
                actor = Func_02004eac(18);
                WORD(actor, 12) = -0x100000;
                actor[35] = 2;
                Func_02004f64(18, 3);
                Func_02004efc(19, 7);
                Func_02004e44(Func_02004eac(19), 0);
                Func_02004f64(19, 1);
                actor = Func_02004eac(19);
                actor[89] = 0;
                actor[35] = 2;
                Func_02004ef4(19, 0xd70000, 0x2580000);
                WORD(actor, 108) = 0x02008b99;
            }
            Func_02002b14();
            break;
        }
    }
done:
    return 0;
}
