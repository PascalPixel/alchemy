#include "types.h"

/* External Func aliases name loader-relocated call words, not runtime addresses. */
void Func_0200613a(s32, s32, s32, s32, s32, s32);
void Func_0200614c(s32, s32, s32, s32, s32, s32);
u8 *Func_020061ca(s32);
void Func_0200616e(u8 *, s32);
void Func_02000c68(void);
extern u8 Data_03001ebc[];
void Func_02001728();
void Func_020061fa();
void Func_02006200();
void Func_02006212();
void Func_0200623e();
void Func_02006264();
void Func_0200637a();
void Func_020061fc();
void Func_0200620a();
void Func_0200620c();
void Func_0200621e();
void Func_02006240();
s32 Func_02006244();
void Func_0200627e();
s32 Func_0200629c();
void Func_020063c8();
extern s32 Data_0200f714[];
void Func_0200627c(s32, s32, s32, s32, s32, s32);
void Func_0200628e(s32, s32, s32, s32, s32, s32);
void Func_020062a2(s32, s32, s32, s32, s32, s32);
void Func_020019a4();
s32 Func_020062f2();
s32 Func_02006316();
void Func_0200631a();
void Func_02006324();
void Func_02006356();
void Func_020064c2();
void Func_02001b44();
s32 Func_020063de();
void Func_020063f6();
void Func_02006400();
void Func_02006432();
void Func_0200659e();
extern s32 Data_0200f754[];
void Func_0200641c(s32, s32, s32, s32, s32, s32);
void Func_0200642e(s32, s32, s32, s32, s32, s32);
void Func_02006442(s32, s32, s32, s32, s32, s32);
extern s32 Data_0200f764[];
void Func_020064cc(s32, s32, s32, s32, s32, s32);
void Func_020064de(s32, s32, s32, s32, s32, s32);
void Func_020064fa(s32, s32, s32, s32, s32, s32);
void Func_02001e34();
s32 Func_02006542();
void Func_0200655a();
void Func_02006564();
void Func_02006596();
void Func_02006702();
s32 Func_02006492();
void Func_02001cd4();
void Func_02006652();
void Func_020064aa();
void Func_020064b4();
void Func_020064e6();

extern u32 Data_0200dfa4;
void Func_02009d70();
void Func_02009d88();
void Func_02009da6();
void Func_02009db8();
void Func_02009dca();
void Func_02009ccc();
void Func_02009cde();
void Func_02009cf0();
void Func_02009db6();
void Func_02009dce();

extern u8 Data_00002411[];
void Func_02009d48();
void Func_02009d5a();
void Func_02009d6c();
void Func_02009d8a();
void Func_02009d9e();
void Func_02009e04();
void Func_02009e22();
void Func_02009e26();
void Func_02009e34_a();
void Func_02009e34_b();
void Func_02009e46();
void Func_02009e4c();
void Func_02009e64();
void Func_02009e76();
void Func_02009e7c();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void ConfigureSceneActor12(void)
{
    s32 actor_slot = 15;
    u8 *actor;

    Func_0200613a(15, 20, 1, 1, actor_slot, 22);
    Func_0200614c(17, 23, 1, 3, actor_slot, 23);
    actor = Func_020061ca(12);
    if (actor != 0) {
        Func_0200616e(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}

void RunSceneObjectSetup(void)
{
    Func_02000c68();
}

void FieldScene_StartActorTwelveTransition(void)
{
    Call3(Func_02006212, 12, 0x10000, 0x8000);
    Call3(Func_0200623e, 12, 248, 0x178);
    Func_02006264(12);
    Func_0200637a(215);
    Func_02006200(60);
    Func_02001728();
    Call1(Func_020061fa, 0x943);
}

void FieldScene_UpdateActorTwelveTransition(void)
{
    u8 *rec7;
    s32 record;

    record = Value1(Func_02006244, 12);
    if ((*(volatile s32 *)(record + 16) >> 20) > 22) {
        Call3(Func_020061fc, 0x40000, 0x40000, 0x10000);
        Call3(Func_0200620a, -1, -1, 0xe666);
        Func_020063c8(144);
        Call6(Func_0200620c, 15, 20, 1, 1, 15, 22);
        Call6(Func_0200621e, 17, 23, 1, 3, 15, 23);
        rec7 = Value1(Func_0200629c, 12);
        if ((s32)rec7 != 0) {
            Func_02006240((s32)rec7, 0);
            rec7[35] = 2;
        }
        Call1(Func_0200627e, 0x943);
    }
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{
    s32 position_x = Data_0200f714[table_index * 2];
    s32 position_z = Data_0200f714[table_index * 2 + 1];

    Func_0200627c(0, 0x4d, 1, 3, position_x, position_z);
    Func_0200628e(1, 0x4d, 1, 1, position_x + 1, position_z);
    Func_020062a2(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void FieldScene_UpdateObjectPairA(void)
{
    s32 v6;
    s32 raw;
    s32 d;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    v6 = 1;
    if (Func_020062f2(234) != -v6) {
        raw = *(s16 *)(((s32)p5 + 0x16c));
        d = raw - 40;
        if (Value1(Func_02006316, 0x941) != 0) {
            if (d == 4) {
                goto done;
            }
        }
        Func_020019a4(d);
        Func_020064c2(157);
        Call3(Func_0200631a, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006324, -v6, -v6, 0xe666);
        Func_02006356(raw + 0x328);
    }
done:;
}

void FieldScene_UpdateObjectPairB(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Func_020063de(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Func_02001b44(field - 40);
        Func_0200659e(157);
        Call3(Func_020063f6, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006400, -1, -1, 0xe666);
        Func_02006432(field + 0x32d);
    }
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{
    s32 position_x = Data_0200f754[table_index * 2];
    s32 position_z = Data_0200f754[table_index * 2 + 1];

    Func_0200641c(0x37, 0x79, 1, 3, position_x, position_z);
    Func_0200642e(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Func_02006442(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateTableBObjectPair(void)
{
    u8 *work;
    s16 field;

    work = *(volatile s32 *)Data_03001ebc;
    if (Func_02006492(234) != -1) {
        field = *(s16 *)((s32)work + 0x16c);
        Func_02001cd4(field - 40);
        Func_02006652(157);
        Call3(Func_020064aa, 0x30000, 0x30000, 0x10000);
        Call3(Func_020064b4, -1, -1, 0xe666);
        Func_020064e6(field + 0x330);
    }
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{
    s32 position_x = Data_0200f764[table_index * 2];
    s32 position_z = Data_0200f764[table_index * 2 + 1];

    Func_020064cc(1, 0x50, 1, 3, position_x, position_z);
    Func_020064de(2, 0x50, 1, 1, position_x + 1, position_z);
    Func_020064fa(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateObjectPairC(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Func_02006542(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Func_02001e34(field - 40);
        Func_02006702(157);
        Call3(Func_0200655a, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006564, -1, -1, 0xe666);
        Func_02006596(field + 0x332);
    }
}

void FieldScene_SelectActorTwentyOneMessage(void)
{
    switch (Data_0200dfa4) {
    case 0:
        Func_02009d70(0x2414);
        Func_02009d88(21, 0);
        break;
    case 1:
        Func_02009d70(0x2415);
        Func_02009d88(21, 0);
        break;
    case 2:
        Func_02009d70(0x2416);
        Func_02009d88(21, 0);
        break;
    case 3:
        Func_02009d70(0x2417);
        Func_02009d88(21, 0);
        break;
    case 4:
        Func_02009d70(0x2418);
        Func_02009d88(21, 0);
        break;
    case 6:
        Func_02009d70(0x241a);
        Func_02009d88(21, 0);
        break;
    case 7:
        Func_02009d70(0x241b);
        Func_02009d88(21, 0);
        break;
    case 5:
        Call3(Func_02009da6, 21, 0xd000, 0);
        Func_02009ccc(50);
        Call3(Func_02009db8, 21, 0xb000, 0);
        Func_02009cde(50);
        Call3(Func_02009dca, 21, 0x5000, 0);
        Func_02009cf0(50);
        Func_02009db6(0x2419);
        Func_02009dce(21, 0);
        break;
    }
}

void FieldScene_RunActorTwentyOneSequence(void)
{
    s32 base5_2411;

    Call3(Func_02009e26, 21, 0x101, 30);
    Call3(Func_02009e22, 21, 0xd000, 0);
    Func_02009d48(50);
    Call3(Func_02009e34_a, 21, 0xb000, 0);
    Func_02009d5a(50);
    Call3(Func_02009e46, 21, 0x5000, 0);
    Func_02009d6c(50);
    base5_2411 = (s32)Data_00002411;
    Func_02009e34_b(base5_2411);
    Func_02009e4c(21, 0);
    Func_02009e04(21, 4);
    Func_02009d8a(60);
    Call3(Func_02009e76, 21, 0xb000, 0);
    Func_02009d9e(40);
    Func_02009e64((base5_2411 + 1));
    Func_02009e7c(21, 0);
}
