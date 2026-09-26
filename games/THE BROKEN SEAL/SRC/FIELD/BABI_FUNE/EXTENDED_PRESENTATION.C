#include "TYPES.H"

extern u8 Data_03001e70[];
extern u8 Value_00000000;

void Main_0808a018();
void Main_0808a210();
void Main_080091c0();
void Main_0808a170();
void Main_0808a570();
void Main_0808a0e8();
void Main_0808a010();
void Main_0808a138();
void Main_0808a180();
void Main_0808a110();
void Main_0808a1b8();
void Main_0808a100();
s32 Main_0808a178();
s32 Main_0808a070();
void Main_0808a1e8();
void Main_080f9010();
void Main_0808a090();
void Main_0808a098();
void Main_0808a0a0();
void Main_0808a580();
void Main_0808a218();
void Main_080f9020();
void Local_020012ac();
void * Main_0808a080();
void Main_0808a208();
void Main_0808a368();
void Main_0808a370();
void Main_080770c8();
void Main_0808a238();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call5(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    f(a0, a1, a2, a3, a4);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Scene_RunExtendedPresentationSequence(void)
{
    u8 *runtime;
    u8 *scene;
    s32 blank;
    void *p176;

    Main_0808a018();
    Call4(Main_0808a210, -1, -1, -1, 0);
    Call6(Main_080091c0, 18, 0, 3, 1, 18, 12);
    Call1(Main_0808a170, 10494);
    Call4(Main_0808a570, 1, -10, 16, 49152);
    Call4(Main_0808a570, 3, 0, 24, 49152);
    Call4(Main_0808a570, 2, 10, 16, 49152);
    Call1(Main_0808a0e8, 1);
    Call1(Main_0808a010, 50);
    Call2(Main_0808a138, 1, 2);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a180, 1, 0);
    Call1(Main_0808a010, 10);
    Call2(Main_0808a110, 2, 3);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a180, 2, 0);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 3, 16384, 0);
    Call1(Main_0808a010, 40);
    Call2(Main_0808a180, 3, 0);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 0, 16384, 0);
    Call3(Main_0808a1b8, 1, 16384, 0);
    Call3(Main_0808a1b8, 2, 16384, 0);
    Call1(Main_0808a010, 60);
    Call2(Main_0808a100, 0, 3);
    Call2(Main_0808a100, 1, 3);
    Call2(Main_0808a110, 2, 3);
    Call1(Main_0808a010, 60);
    Call2(Main_0808a138, 1, 2);
    Call1(Main_0808a010, 30);
    Call3(Main_0808a1b8, 1, 57344, 0);
    Call1(Main_0808a010, 20);
    Main_0808a178(1, 0);
    if (Main_0808a070(0, 0) == 0) {
        Call1(Main_0808a010, 30);
        Call3(Main_0808a1e8, 1, 258, 50);
        Call2(Main_0808a180, 1, 0);
        (*(u16 *)(*(u8 **)0x03001ebc + 472))++;
    } else {
        Call1(Main_0808a010, 30);
        Call3(Main_0808a1e8, 1, 258, 50);
        (*(u16 *)(*(u8 **)0x03001ebc + 472))++;
        Call2(Main_0808a180, 1, 0);
    }
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 2, 32768, 0);
    Call1(Main_0808a010, 30);
    Call3(Main_0808a1e8, 2, 257, 50);
    Call2(Main_0808a180, 2, 0);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 1, 0, 0);
    Call1(Main_0808a010, 30);
    Call3(Main_0808a1e8, 1, 257, 60);
    Call2(Main_0808a138, 3, 2);
    Call1(Main_0808a010, 30);
    Call3(Main_0808a1b8, 3, 40960, 0);
    Call1(Main_0808a010, 30);
    Call2(Main_0808a180, 3, 0);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1e8, 1, 256, 50);
    Call2(Main_0808a180, 1, 0);
    Call1(Main_0808a010, 10);
    Call2(Main_0808a138, 2, 2);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a180, 2, 0);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 3, 49152, 0);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a110, 3, 4);
    Call1(Main_0808a010, 20);
    Main_0808a178(3, 0);
    if (Main_0808a070(0, 0) == 0) {
        Call1(Main_0808a010, 20);
        Call2(Main_0808a138, 1, 2);
        Call1(Main_0808a010, 20);
        Call3(Main_0808a1b8, 1, 57344, 0);
        Call1(Main_0808a010, 20);
        Call2(Main_0808a180, 1, 0);
        (*(u16 *)(*(u8 **)0x03001ebc + 472))++;
    } else {
        Call1(Main_0808a010, 20);
        Call2(Main_0808a138, 1, 2);
        Call1(Main_0808a010, 20);
        Call3(Main_0808a1b8, 1, 57344, 0);
        Call1(Main_0808a010, 20);
        (*(u16 *)(*(u8 **)0x03001ebc + 472))++;
        Call2(Main_0808a180, 1, 0);
    }
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1e8, 0, 258, 50);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 2, 40960, 0);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a110, 2, 3);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a180, 2, 0);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a180, 1, 0);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a110, 0, 3);
    Call1(Main_0808a010, 30);
    Call1(Main_0808a010, 10);
    Call2(Main_0808a110, 3, 3);
    Call1(Main_0808a010, 20);
    Main_0808a178(3, 0);
    Call3(Main_0808a1b8, 1, 57344, 0);
    Call3(Main_0808a1b8, 3, 49152, 0);
    Call3(Main_0808a1b8, 2, 40960, 0);
    Call1(Main_0808a010, 20);
    if (Main_0808a070(0, 0) == 0) {
        Call1(Main_0808a010, 30);
        Call2(Main_0808a110, 3, 3);
        Call1(Main_0808a010, 20);
        Call2(Main_0808a180, 3, 0);
        (*(u16 *)(*(u8 **)0x03001ebc + 472))++;
    } else {
        Call1(Main_0808a010, 30);
        Call2(Main_0808a110, 3, 3);
        Call1(Main_0808a010, 20);
        (*(u16 *)(*(u8 **)0x03001ebc + 472))++;
        Call2(Main_0808a180, 3, 0);
    }
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 1, 49152, 0);
    Call1(Main_0808a010, 30);
    Call2(Main_0808a180, 1, 0);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 0, 49152, 0);
    Call1(Main_0808a010, 30);
    Call1(Main_0808a010, 10);
    Call3(Main_0808a1b8, 2, 49152, 0);
    Call1(Main_0808a010, 30);
    Call2(Main_0808a110, 2, 3);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a180, 2, 0);
    Call1(Main_0808a010, 10);
    Call2(Main_0808a110, 3, 3);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a180, 3, 0);
    Call1(Main_080f9010, 17);
    Call1(Main_0808a010, 10);
    Call2(Main_0808a100, 0, 3);
    Call2(Main_0808a100, 1, 3);
    Call2(Main_0808a110, 2, 3);
    Call1(Main_0808a010, 40);
    Call3(Main_0808a090, 0, 78643, 39321);
    Call3(Main_0808a090, 1, 78643, 39321);
    Call3(Main_0808a090, 2, 78643, 39321);
    Call3(Main_0808a090, 3, 78643, 39321);
    Call2(Main_0808a098, 0, 33593376);
    Call1(Main_0808a010, 50);
    Call4(Main_0808a210, 17301504, -1, 13107200, 1);
    Call2(Main_0808a098, 1, 33593568);
    Call1(Main_0808a010, 50);
    Call2(Main_0808a098, 2, 33593740);
    Call1(Main_0808a0a0, 2);
    Call3(Main_0808a1b8, 0, 16384, 0);
    Call3(Main_0808a1b8, 1, 16384, 0);
    Call3(Main_0808a1b8, 2, 16384, 0);
    Call3(Main_0808a580, 3, 0, -32);
    Call1(Main_0808a010, 30);
    Call3(Main_0808a1b8, 3, 16384, 0);
    Call1(Main_0808a010, 60);
    Call3(Main_0808a1b8, 3, 32768, 0);
    Call1(Main_0808a010, 20);
    Call2(Main_0808a098, 3, 33593932);
    Call1(Main_0808a0a0, 3);
    Call3(Main_0808a1b8, 3, 49152, 0);
    Call1(Main_0808a010, 20);
    Call4(Main_0808a210, 14155776, -1, 11010048, 1);
    Main_0808a218();
    Call1(Main_0808a010, 20);
    Call3(Main_0808a1b8, 0, 32768, 0);
    Call3(Main_0808a1b8, 1, 32768, 0);
    Call3(Main_0808a1b8, 2, 32768, 0);
    Call3(Main_0808a1b8, 3, 32768, 0);
    Call1(Main_0808a010, 40);
    Call2(Main_0808a100, 0, 3);
    Call2(Main_0808a100, 1, 3);
    Call2(Main_0808a100, 3, 3);
    Call2(Main_0808a110, 2, 3);
    Call1(Main_0808a010, 30);
    Call1(Main_080f9010, 67);
    Call1(Main_080f9020, 240);
    Local_020012ac();
    Call1(Main_0808a010, 80);
    scene = *(u8 **)Data_03001e70;
    p176 = Main_0808a080(8);
    *(s32 *)(p176 + 52) = 131;
    *(s32 *)(p176 + 48) = 131072;
    *(s32 *)(scene + 284) = -0x3000;
    Call3(Main_0808a580, 8, 60, 0);
    *(s32 *)(scene + 284) = -0x6000;
    Call3(Main_0808a580, 8, 60, 0);
    Call1(Main_0808a010, 80);
    Call1(Main_0808a010, 100);
    Call2(Main_0808a208, 144179, 655);
    Call4(Main_0808a210, 52953088, -1, 11010048, 1);
    Call1(Main_0808a010, 300);
    runtime = *(u8 **)(Data_03001e70 + 76);
    *(u32 *)(runtime + 448) = 256;
    /* FAKEMATCH: keep the backdrop zero before its address calculation. */
    do {
        blank = 0;
    } while (0);
    *(u16 *)0x05000000 = blank;
    *(u32 *)(runtime + 456) = 96;
    Main_0808a368();
    Main_0808a370();
    Call1(Main_0808a010, 30);
    Call1(Main_080770c8, 282);
    Call2(Main_0808a238, (s32)&Value_00000000, 9);
}
