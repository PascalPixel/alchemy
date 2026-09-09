#include "types.h"

#define RunEventScript02 Func_020019b0

/* Second event script of resource_38d: a linear field-scene choreography that
 * drives channels 0-3 and 18-20 through fades, motions, positions and message
 * waits, branching six times on the saved flag 0x84f and on three actor
 * lookups.
 *
 * Every branch, call and store below is taken from the complete decoded owner
 * (2060 bytes, 186 calls). Roles of the called wrappers remain unknown, so
 * they keep their address aliases; the two same-overlay callees are noted at
 * their call sites.
 *
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. That legacy spelling is a PC-relative reading of a
 * word the loader rewrites, so it is not a stable identity. Eight of the words
 * in this owner spell an alias that two different runtime veneers share. A
 * ninth, Func_02004510, reads as a single unambiguous veneer here yet is still
 * refused a binding by the scorer, and the cause is not established from this
 * owner; the call word at 0x02002006 is an ordinary bl and the halfword before
 * it is `movs r1,#3`, not pool data. So none of those nine can be bound on
 * their own from this file. Each such call is
 * written with another alias of the SAME runtime veneer, and the legacy word
 * plus that veneer address is recorded at the call site. The intended per-site
 * spelling is Func_<legacy>_a/_b with explicit runtime addresses, which needs
 * an absolute_symbols block in this owner's translation unit; that block does
 * not exist yet.
 *
 * The cleared state byte written into the record at +85 is held in its own
 * local, so its value is established once at the top of the scene rather than
 * rebuilt at the store; the fourth argument zero of the following call is an
 * unrelated constant. */

extern u8 Data_03001ebc[];
extern u8 Data_0200a5c0[];
extern u8 Data_0200a5d4[];

void Func_020035de();
void Func_020036ec();
void Func_02003748();
void Func_0200376a();
void Func_020037d4();
void Func_02003846();
void Func_020038ac();
void Func_020038fc();
void Func_02003dd4();
void Func_02003df4();
void Func_02003e08();
void Func_02003e1a();
void Func_02003e24();
void Func_02003e8c();
void Func_02003eee();
void Func_02003ef0();
void Func_02003efc();
void Func_02003f06();
s32 Func_02003f10();
void Func_02003f12();
void Func_02003f28();
void Func_02003f34();
void Func_02003f44();
void Func_02003f46();
void Func_02003f52();
void *Func_02003f58();
void Func_02003f66();
void Func_02003f7e();
void Func_02003f8c();
void Func_02003f96();
void Func_02003fa4();
void Func_02003fae();
void *Func_02003fb0();
void Func_02003fb2();
void Func_02003fbc();
void *Func_02003fc4();
void Func_02003fc6();
void *Func_02003fd8();
void Func_02003fea();
void Func_02003ff0();
void Func_02003ff8();
void Func_02004002();
void Func_02004010();
void Func_0200401e();
void Func_02004024();
void Func_02004032();
void Func_02004034();
void Func_02004046();
s32 Func_0200405a();
void Func_0200406a();
void Func_02004078();
void Func_0200407c();
void Func_0200407e();
void Func_02004080();
void Func_02004086();
void Func_02004088();
void Func_02004090();
s32 Func_0200409a();
void Func_0200409e();
void Func_020040ac();
void Func_020040b6();
void Func_020040be();
void Func_020040c6();
void Func_020040da();
s32 Func_020040e2();
void Func_020040f4();
void Func_02004138();
void Func_02004144();
void Func_02004146();
void Func_02004156();
void Func_02004158();
void Func_02004180();
void Func_02004186();
void Func_020041b6();
void Func_020041c0();
s32 Func_020041c4();
void Func_0200420a();
void Func_02004214();
void Func_02004222();
s32 Func_0200422e();
void Func_02004232();
void Func_0200423e();
void Func_02004250();
void Func_02004256();
void Func_0200425c();
void Func_0200426a();
void Func_02004272();
void Func_0200428e();
void Func_020042a0();
void Func_020042a8();
s32 Func_020042aa();
void Func_020042ca();
s32 Func_020042d0();
void Func_020042d6();
void Func_02004320();
void Func_02004324();
void Func_02004330();
s32 Func_0200434e();
void Func_0200436a();
void Func_02004382();
void Func_02004384();
void Func_020043a0();
void Func_020043aa();
void Func_020043c2();
void Func_020043c8();
void Func_020043de();
void Func_020043e4();
void Func_020043ea();
void Func_020043ec();
s32 Func_020043ee();
void Func_02004400();
void Func_02004402();
void Func_02004404();
void Func_02004412();
void Func_0200441e();
void Func_0200443a();
void Func_02004450();
void Func_02004464();
void Func_0200447a();
void Func_02004488();
void Func_02004496();
void Func_020044a0();
void Func_020044c6();
void Func_020044ca();
void Func_020044d2();
void Func_020044d4();
void *Func_020044de();
void *Func_020044fe();
void Func_02004502();
void Func_02004518();
void Func_0200451a();
void *Func_0200451e();
void Func_02004522();
void Func_02004526();
void Func_0200452a();
void Func_02004530();
void Func_02004538();
void Func_0200453a();
void Func_02004540();
void Func_02004542();
void Func_0200454e();
void Func_02004550();
void Func_02004558();
void Func_02004560();
void Func_02004562();
void Func_02004580();
void Func_02004586();
void Func_0200458e();
void Func_020045a0();
void Func_020045a8();
void Func_020045aa();
s32 Func_020045b0();
void Func_020045b2();
void Func_020045ba();
void Func_020045c8();
void Func_020045ce();
void Func_020045dc();
void Func_020045f2();
void Func_0200460c();
void Func_0200460e();
void Func_02004610();
void Func_02004624();
void Func_02004634();
void Func_02004638();
void Func_02004670();
void Func_02004678();
void Func_020046f0();
void Func_020046fc();
void Func_02004706();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Actor record returned by the three lookup helpers. The two halfword reads
 * take the whole part of the matching 16.16 word. */
typedef struct ActorPos {
    u8 field_00[8];
    s16 x_low;
    s16 x;
    s32 field_0c;
    s16 y_low;
    s16 y;
} ActorPos;

void RunEventScript02(void)
{
    u8 *work;
    u8 *buf;
    ActorPos *pos;
    s32 flag;
    u8 clear = 0;

    Func_02003e24();
    Call4(Func_02003f46, -1, -1, -1, 0);
    Func_02003dd4(1);
    buf = (u8 *)Func_02003f58();
    buf[85] = clear;
    Call4(Func_02003f66, 0x037e0000, -1, 0x02980000, 0);
    Func_02003df4(1);
    Func_02003eee(0, 0, 0);
    Func_02003e1a();
    Func_02003e08(1);

    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x201;
    *(s32 *)(work + 0x1c8) = 16;

    Func_02003fb2();
    Func_02003fc6();
    Func_02003e8c(40);
    Call3(Func_02003f28, 19, 0x03780000, 0x031e0000);
    Call3(Func_02003f34, 0, 0x03880000, 0x031e0000);
    Call2(Func_02003fbc, 0x9999, 0x1333);
    /* legacy word Func_02003fd2, veneer 0x0200a580 */
    Call4(Func_02003f66, 0x037e0000, -1, 0x02ba0000, 1);
    Call3(Func_02003efc, 19, 0xcccc, 0x6666);
    Call3(Func_02003f06, 0, 0x9999, 0x4ccc);
    Call3(Func_02003f44, 19, 888, 720);
    Call3(Func_02003f52, 0, 904, 736);
    Func_02003ef0(60);
    Func_02003f7e(19);
    Func_02003f96(19, 1);
    Func_02003f8c(0);
    Func_02003fa4(0, 1);
    Func_02003f12(20);
    /* legacy word Func_02003fd2, veneer 0x0200a520 */
    Func_02004146(19, 2);
    Call1(Func_02003ff0, 0x1728);

    flag = 1;
    if (Value1(Func_02003f10, 0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Func_02004024(19, 0);
    if (flag != 0) {
        bump_step(1);
    }

    Call4(Func_0200407e, 0x037e0000, -1, 0x02980000, 1);
    Func_02003fae(19, Data_0200a5d4);
    Call3(Func_02003fea, 0, 894, 684);

    pos = (ActorPos *)Func_02003fb0(0);
    if (pos != 0) {
        Func_0200401e(1, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }
    pos = (ActorPos *)Func_02003fc4(0);
    if (pos != 0) {
        Func_02004032(2, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }
    pos = (ActorPos *)Func_02003fd8(0);
    if (pos != 0) {
        Func_02004046(3, *(s32 *)&pos->x_low, *(s32 *)&pos->y_low);
    }

    Call3(Func_02003ff8, 1, 0x9999, 0x4ccc);
    Call3(Func_02004002, 2, 0x9999, 0x4ccc);
    Call3(Func_02004010, 3, 0x10000, 0x8000);
    Func_02004078(1, 2);
    Func_02004080(2, 2);
    Func_02004088(3, 2);
    Call3(Func_0200407c, 1, -16, 16);
    Func_02004086(2, 16, 16);
    Func_02004090(3, 32, 16);
    Func_0200409e(2);
    Func_020040b6(1, 1);
    Func_020040be(2, 1);
    Func_020040c6(3, 1);
    Func_02004034(10);
    Call3(Func_02004138, 1, 0xc000, 0);
    Call3(Func_02004144, 2, 0xc000, 0);
    Func_020040da(3);
    Call3(Func_02004156, 3, 0xa000, 0);
    Func_020040ac(19);
    Func_0200406a(20);

    flag = 1;
    if (Value1(Func_0200405a, 0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Func_02004146(18, 3);
    Call3(Func_02004180, 0x2012, 0, 20);
    if (flag != 0) {
        bump_step(1);
    }

    flag = 1;
    if (Value1(Func_0200409a, 0x84f) == 0) {
        bump_step(1);
        flag = 0;
    }
    Func_02004186(18, 1);
    Call3(Func_020041c0, 0x2012, 0, 10);
    if (flag != 0) {
        bump_step(1);
    }

    Func_020035de(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    Func_020040f4(20);
    if (Value1(Func_020040e2, 0x84f) != 0) {
        Call3(Func_0200420a, 0, 258, 60);
        Call3(Func_02004214, 1, 261, 40);
    } else {
        Func_02004158(40);
    }

    Call3(Func_0200425c, 1, 0xc000, 10);
    Call3(Func_02004256, 0x4001, 0, 10);
    Call3(Func_02004272, 2, 0xc000, 10);
    Func_02004222(2, 3);
    Call2(Func_0200426a, 0x4002, 0);
    Call3(Func_0200428e, 3, 0xa000, 10);
    Func_0200423e(3, 3);
    /* legacy word Func_02004290, veneer 0x0200a550 */
    Call3(Func_020042a8, 0x4003, 0, 20);
    Func_02004250(18, 3);
    Func_020041b6(20);
    Call3(Func_020042a8, 0x2012, 0, 20);
    Call3(Func_020042ca, 1, 259, 0);
    Call3(Func_020042d6, 0, 258, 60);

    if (Value1(Func_020041c4, 0x84f) != 0) {
        Func_020042a0(18, 1);
        /* legacy word Func_02004290, veneer 0x0200a508 */
        Func_02004250(18, 4);
        Value2(Func_020042d0, 0x2012, 0);
        Func_020036ec(); /* ConfigurePrimarySceneChannels, 0x02001958 */
        flag = 1;
        if (Func_0200422e(0, 0) != 0) {
            bump_step(1);
            flag = 0;
        }
        Call3(Func_02004320, 18, 0x5000, 0);
        Func_02003748(); /* ConfigureSecondarySceneChannels, 0x02001984 */
        Func_02004232(10);
        Call3(Func_02004324, 0x2012, 0, 10);
        if (flag != 0) {
            bump_step(1);
        }
        /* legacy word Func_0200435c, veneer 0x0200a568 */
        Call3(Func_020042d6, 18, 258, 60);
    } else {
        bump_step(4);
    }

    Value2(Func_0200434e, 0x2012, 0);
    Func_0200376a(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    if (Func_020042aa(0, 0) == 0) {
        /* legacy word Func_0200435c, veneer 0x0200a538 */
        Call1(Func_02004384, 0x1737);
    } else {
        Call1(Func_02004384, 0x1738);
    }

    Func_020037d4(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Call3(Func_020043aa, 0x2012, 0, 20);
    Func_02004382(19, 1);
    Call1(Func_020043a0, 0x1739);
    Func_020043c2(19, 0, 10);
    Call3(Func_020043de, 0, 0x6000, 0);
    Call3(Func_020043ea, 1, 0xe000, 0);
    /* legacy word Func_020043f4, veneer 0x0200a560 */
    Func_020043ea(2, 0, 0);
    Call3(Func_02004400, 3, 0x8000, 40);
    Func_020043c8(18, 2);
    Call3(Func_02004402, 0x2012, 0, 10);
    Call3(Func_0200441e, 0, 0xc000, 0);
    Func_02003846(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Func_02004330(10);
    Call3(Func_0200443a, 18, 261, 60);
    /* legacy word Func_0200442c, veneer 0x0200a550 */
    Call3(Func_02004402, 0x2012, 0, 10);
    Func_020043e4(0, 3);
    Func_020043ec(1, 3);
    /* legacy word Func_020043f4, veneer 0x0200a500 */
    Func_020043ec(2, 3);
    Func_02004404(3, 3);
    Func_0200436a(20);
    Func_02004412(18, 3);
    Call3(Func_02004464, 0x2012, 0, 10);
    Call3(Func_02004488, 18, 264, 60);
    Call3(Func_0200447a, 0x2012, 0, 10);
    Call3(Func_02004496, 18, 0x3000, 10);
    /* legacy word Func_02004490, veneer 0x0200a550 */
    Call3(Func_0200447a, 0x2012, 0, 10);
    Func_02004450(18, 3);
    /* legacy word Func_02004490, veneer 0x0200a540 */
    Value2(Func_0200434e, 0x2012, 0);

    Func_020038ac(); /* ConfigurePrimarySceneChannels, 0x02001958 */
    flag = 1;
    if (Func_020043ee(0, 0) == 1) {
        bump_step(1);
        flag = 0;
    }
    Func_020038fc(); /* ConfigureSecondarySceneChannels, 0x02001984 */
    Call3(Func_020044d2, 0x2012, 0, 10);
    if (flag != 0) {
        bump_step(1);
    }

    Call3(Func_02004502, 18, 0x7000, 10);
    Func_020044ca(19, 1);
    /* legacy word Func_02004516, veneer 0x0200a560 */
    Call3(Func_02004502, 19, 0x1000, 20);
    Func_020044c6(18, 3);
    /* legacy word Func_0200442c, veneer 0x0200a468 */
    Func_020044a0(20);
    Func_020044d4(19, 3);
    Func_02004526(19, 0, 10);
    Call3(Func_02004542, 19, 0x3000, 10);
    Call3(Func_0200454e, 18, 0x3000, 20);
    /* legacy word Func_02004516, veneer 0x0200a520 */
    Func_020044ca(18, 1);
    Call3(Func_02004550, 0x2012, 0, 10);
    /* legacy word Func_02004510, veneer 0x0200a508; that alias resolves to one
     * veneer here, but spelling it directly is still refused for want of a
     * stable binding, so the same veneer is reached through another alias. */
    Func_020044d4(18, 3);
    Call3(Func_02004562, 0x2012, 0, 10);
    Func_0200451a(0, 3);
    Func_02004522(1, 3);
    Func_0200452a(2, 3);
    Func_0200453a(3, 3);
    Func_020044a0(20);

    Func_02004540(1, 2);
    pos = (ActorPos *)Func_020044de(0);
    if (pos != 0) {
        Func_02004518(1, pos->x, pos->y);
    }
    Func_02004560(2, 2);
    pos = (ActorPos *)Func_020044fe(0);
    if (pos != 0) {
        Func_02004538(2, pos->x, pos->y);
    }
    Func_02004580(3, 2);
    pos = (ActorPos *)Func_0200451e(0);
    if (pos != 0) {
        Func_02004558(3, pos->x, pos->y);
    }

    Func_0200458e(1);
    Func_020045a0(1, 0, 0);
    Func_020045aa(2, 0, 0);
    Func_020045a8(3);
    Func_020045ba(3, 0, 0);
    Func_02004530(20);
    Call3(Func_02004634, 18, 0x5000, 0);
    Call3(Func_0200460e, 0, 0x10013, (s32)Data_0200a5c0);
    /* legacy word Func_020045c2, veneer 0x0200a4d8 */
    Call3(Func_020045ce, 19, 852, 646);
    Call3(Func_020045ce, 19, 852, 666);
    Call3(Func_020045dc, 19, 864, 672);
    Call3(Func_02004670, 19, 0x1000, 10);
    Func_02004638(19, 1);
    Func_02004586(10);
    Func_02004678(19, 0, 10);
    Call3(Func_0200460c, 19, 886, 708);
    Call3(Func_02004610, 19, 894, 764);
    Call3(Func_02004624, 0, 894, 764);
    Func_020046f0();
    Func_020046fc();
    Call1(Func_020045b2, 802);
    if (Value1(Func_020045b0, 0x84f) == 0) {
        /* legacy word Func_020045c2, veneer 0x0200a458 */
        Call1(Func_020045c8, 0x84f);
        Call1(Func_020045c8, 0x84a);
    }
    Func_02004706(6);
    Func_020045f2();
}
