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

struct Resource3c8Actor *Func_0808a080();
s32 Func_080770c0();
s32 Func_02000098();

void Func_0808a018();
void Func_080091c0();
void Func_0808a1e0();
void Func_080091e0();
void Func_0200094c();
void Func_080f9010();
void Func_080090d0();
void Func_0808a0f0();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a010();
void Func_08009178();
void Func_080770c8();
void Func_0808a020();
void Func_02001d48(void)
{
    struct Resource3c8Actor *actor9;
    struct Resource3c8Actor *actor0;
    struct Resource3c8Actor *actor10;
    s32 serviceResult;
    s32 x;
    s32 y;

    serviceResult = 0;
    actor9 = Func_0808a080(9);
    actor0 = Func_0808a080(0);
    Func_0808a018();

    Func_080091c0(109, 43, 7, 5, 45, 43);
    if ((actor9->flags23 & 2) != 0) {
        Func_080091c0(45, 45, 1, 1, 46, 45);
    } else {
        Func_080091c0(
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
    if (x == 46 && y == 45 && Func_080770c0(0x301) == 0) {
        if ((actor0->positionY >> 20) <= 45) {
            serviceResult = Func_02000098(
                0x02e80000,
                0,
                0x02c00000,
                20
            );
            Func_0808a1e0(0, 3);
        }

        Func_080091e0(Func_0808a080(9), 0);
        actor9->flags22 = 0;
        actor9->mode55 = 3;
        actor9->state48 = 0x1999;
        actor9->state44 = 0;
        Func_080091c0(43, 45, 1, 1, x, y);
        Func_0200094c(actor9);
        Func_080f9010(188);
        actor9->mode55 = 0;
        actor9->state0c = -0x100000;
        Func_0808a1e0(9, 3);
        actor9->flags23 = 2;
        Func_080091c0(45, 45, 1, 1, x, y);

        actor0 = Func_0808a080(0);
        actor0->flags23 |= 1;
        Func_080090d0(serviceResult);
        actor9->state59 = 0;
        actor9->flags23 |= 2;

        actor10 = Func_0808a080(10);
        actor10->state59 = 0;
        actor10 = Func_0808a080(10);
        actor10->flags23 |= 2;
        Func_0808a0f0(10, 0x03280000, 0x02d80000);

        /* Both pooled 0x0200dxxx words are direct arguments, not pointer slots. */
        Func_0808a098(10, (void *)0x0200d3c4);
        Func_0808a0a0(10);
        Func_0808a010(30);
        Func_080f9010(158);
        Func_08009178((void *)0x0200dce8, 110, 41);
        Func_080091c0(46, 41, 1, 1, x, 42);
        Func_080770c8(0x301);
    }

    Func_0808a020();
}
