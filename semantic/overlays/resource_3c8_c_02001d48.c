typedef signed int s32;
typedef unsigned char u8;

struct Resource3c8Actor {
    u8 unknown_00[8];
    s32 positionX;
    s32 state0c;
    s32 positionY;
    u8 unknown_14[14];
    u8 flags22;
    u8 flags23;
    u8 unknown_24[32];
    s32 state44;
    s32 state48;
    u8 unknown_4c[9];
    u8 mode55;
    u8 unknown_56[3];
    u8 state59;
};

struct Resource3c8Actor *Func_02006c0c();
struct Resource3c8Actor *Func_02006c14();
struct Resource3c8Actor *Func_02006d22();
struct Resource3c8Actor *Func_02006d48();
struct Resource3c8Actor *Func_02006d52();
s32 Func_02001e96();
s32 Func_02006c50();
s32 Func_02006cba();
void Func_0200278e();
void Func_02006bb6();
void Func_02006bd8();
void Func_02006bf2();
void Func_02006c02();
void Func_02006c58();
void Func_02006c70();
void Func_02006c72();
void Func_02006ca4();
void Func_02006d28();
void Func_02006d5c();
void Func_02006d6c();
void Func_02006d76();
void Func_02006d80();
void Func_02006d8e();
void Func_02006d9a();
void Func_02006db0();
void Func_02006dba();
void Func_02006cf4();
void Func_02006e44();
void Func_02006ed2();

/*
 * The resident calls at 0x02001e96 and 0x0200278e share this package's
 * numeric namespace; neither names a local continuation owned by this C file.
 */
void Func_02001d48(void)
{
    struct Resource3c8Actor *actor9;
    struct Resource3c8Actor *actor0;
    struct Resource3c8Actor *actor10;
    s32 serviceResult;
    s32 x;
    s32 y;

    serviceResult = 0;
    actor9 = Func_02006c0c(9);
    actor0 = Func_02006c14(0);
    Func_02006c02();

    Func_02006bb6(109, 43, 7, 5, 45, 43);
    if ((actor9->flags23 & 2) != 0) {
        Func_02006bd8(45, 45, 1, 1, 46, 45);
    } else {
        Func_02006bf2(
            48,
            42,
            1,
            1,
            actor9->positionX >> 20,
            actor9->positionY >> 20
        );
    }

    x = actor9->positionX >> 20;
    y = actor9->positionY >> 20;
    if (x == 46 && y == 45 && Func_02006c50(0x301) == 0) {
        if ((actor0->positionY >> 20) <= 45) {
            serviceResult = Func_02001e96(
                0x02e80000,
                0,
                0x02c00000,
                20
            );
            Func_02006d6c(0, 3);
        }

        Func_02006c58(Func_02006cba(9), 0);
        actor9->flags22 = 0;
        actor9->mode55 = 3;
        actor9->state48 = 0x1999;
        actor9->state44 = 0;
        Func_02006c70(43, 45, 1, 1, x, y);
        Func_0200278e(actor9);
        Func_02006e44(188);
        actor9->mode55 = 0;
        actor9->state0c = -0x100000;
        Func_02006dba(9, 3);
        actor9->flags23 = 2;
        Func_02006ca4(45, 45, 1, 1, x, y);

        actor0 = Func_02006d22(0);
        actor0->flags23 |= 1;
        Func_02006c72(serviceResult);
        actor9->state59 = 0;
        actor9->flags23 |= 2;

        actor10 = Func_02006d48(10);
        actor10->state59 = 0;
        actor10 = Func_02006d52(10);
        actor10->flags23 |= 2;
        Func_02006db0(10, 0x03280000, 0x02d80000);

        /* Both pooled 0x0200dxxx words are direct arguments, not pointer slots. */
        Func_02006d80(10, (void *)0x0200d3c4);
        Func_02006d8e(10);
        Func_02006d5c(30);
        Func_02006ed2(158);
        Func_02006cf4((void *)0x0200dce8, 110, 41);
        Func_02006d28(46, 41, 1, 1, x, 42);
        Func_02006d76(0x301);
    }

    Func_02006d9a();
}
