#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02001e94

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000000a1[];
extern u8 Data_00002438[];
extern u8 Data_02000240[];
void Func_020074f4();
void Func_02007544();
void Func_0200754a();
void Func_0200754c();
void Func_02007554();
void Func_0200755c();
void Func_020075a6();
void Func_020075c6();
void Func_020075ce();
void Func_020075d8();
void Func_020075de();
void Func_020075e2();
void Func_020075e8();
void Func_020075ec();
void Func_02007600();
void Func_0200760a();
void Func_02007614();
void Func_02007616();
void Func_02007628();
void Func_02007636();
void Func_0200763a();
void Func_0200763c();
void Func_0200763e();
void Func_02007640();
void Func_02007642();
void Func_0200764c();
void Func_02007650();
void Func_02007656();
void Func_02007662();
void Func_02007664();
void Func_0200766a();
void Func_02007678();
void Func_0200767a();
void Func_0200767c();
void Func_02007680();
void Func_0200768a();
void Func_02007690();
void Func_02007694();
void Func_020076ac();
void Func_020076cc();
void Func_020076d6();
void Func_020076da();
void Func_020076e0();
void Func_020076f2();
void Func_02007778();
void Func_02007782();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;
    s32 base5_2438;
    s32 base3_2000240;

    Func_02007544(0, 1);
    Func_0200754c(12, 1);
    Func_02007554(13, 1);
    Func_0200755c(14, 1);
    Func_02007662(113);
    Call3(Func_020075de, 12, 0x100, 0);
    Func_020074f4(30);
    Func_020075a6(12, 0, 0);
    base5_2438 = (s32)Data_00002438;
    Func_020075c6(base5_2438);
    Func_020075de(12, 0);
    Call3(Func_0200760a, 13, 0x100, 0);
    Call3(Func_02007616, 14, 0x100, 0);
    Func_020075d8(13, 0, 0);
    Func_020075e2(14, 0, 0);
    Func_020075ec(0, 13, 0);
    Func_0200754a(65);
    Call3(Func_02007636, 13, 0x5000, 0);
    Call3(Func_02007642, 14, 0xd000, 0);
    Func_02007628((base5_2438 + 1));
    Func_02007640(13, 0);
    Func_02007600(14, 3);
    Func_0200763e((base5_2438 + 2));
    Func_02007656(14, 0);
    Func_0200764c((base5_2438 + 3));
    Func_02007664(12, 0);
    Func_0200763c(13, 1);
    Func_02007662((base5_2438 + 4));
    Func_0200767a(13, 0);
    Func_0200763a(14, 3);
    Func_02007678((base5_2438 + 5));
    Func_02007690(14, 0);
    Func_02007650(14, 3);
    Func_020075ce(60);
    Func_02007680(13, 0, 0);
    Func_0200768a(14, 0, 0);
    Func_020075e8(70);
    Call3(Func_0200764c, 12, 0x2a0, 88);
    Func_0200766a(12);
    Func_020076ac(12, 0, 0);
    Func_02007694(12, 3);
    Func_02007614(30);
    Func_020076da((base5_2438 + 6));
    Func_020076f2(12, 0);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x200;
    Func_02007778((s32)Data_000000a1, 31);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02007782(98, 3);
    Func_020076cc(12, 0, 0);
    Func_020076d6(13, 0, 0);
    Func_020076e0(14, 0, 0);
    Func_0200767c();
    Call1(Func_02007662, 0x94a);
}
