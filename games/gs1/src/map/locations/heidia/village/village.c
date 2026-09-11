#include "types.h"
#include "object_runtime.h"

/* map/locations/heidia/village/actor_pair_sequence.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000240d[];
extern u8 Data_03001ebc[];
void Func_020077f8();
void Func_0200783a();
void Func_02007862();
void Func_02007872_a();
void Func_02007872_b();
void Func_0200788a();
void Func_0200788c();
void Func_020078da();
void Func_020078e4();
void Func_02007904();
void Func_0200790c();
void Func_02007918();
void Func_0200791c();
void Func_02007920();
void Func_02007938();
void Func_0200794a();
void Func_02007990();
void Func_020079a8();
void Func_020079bc();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_020021c4(void)
{
    u32 i;
    s32 record;
    s32 base5_240d;

    Func_020077f8();
    Func_02007862(0, 0, 0);
    Func_0200783a(0, 1);
    Func_0200788a(0, 1);
    Func_02007990(113);
    Call3(Func_0200790c, 21, 0x100, 0);
    Call3(Func_02007918, 13, 0x100, 60);
    Func_020078da(21, 0, 0);
    Func_020078e4(13, 0, 0);
    base5_240d = (s32)Data_0000240d;
    Func_02007904(base5_240d);
    Func_0200791c(13, 0);
    Call3(Func_0200794a, 0, 0x102, 30);
    Func_02007920((base5_240d + 1));
    Func_02007938(13, 0);
    Func_020079bc();
    Func_02007872_a(60);
    Func_020079a8(60);
    Func_0200788c();
    Call1(Func_02007872_b, 0x225);
}

/* map/locations/heidia/village/actor_scenes.c */
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

extern u8 Data_03001e70[];
extern u32 Data_03001e40;
extern s16 Data_02000240[];
extern u8 Value_00002092;
struct ObjectRuntime *Func_020067ac();
struct ObjectRuntime *Func_020067b4();
s32 Func_020067a6();
s32 Func_020067de();
double Func_02006ce6(s32);
double Func_02006c7a(double, double);
s32 Func_02006d72(double);
s32 Func_02002314();
s32 Func_02002b3e();
s32 Func_02002b72();
void Func_020068a0();

struct ObjectRuntime *Func_020069e6();
void Func_02006afa();
s32 Func_020069dc();
s32 Func_02006a0e();
s32 Func_02002756();
s32 Func_02002dbc();
s32 Func_02002d5a();
s32 Func_02002d58();
void Func_02006a96();

struct ObjectRuntime *Func_02006b82();
s32 Func_02006b74();
s32 Func_02006ba6();
s32 Func_02002a96();
s32 Func_02002eec();
s32 Func_02002eea();
void Func_02006c28();

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

void Scene_StartActorTwelveTransition(void)
{
    Call3(Func_02006212, 12, 0x10000, 0x8000);
    Call3(Func_0200623e, 12, 248, 0x178);
    Func_02006264(12);
    Func_0200637a(215);
    Func_02006200(60);
    Func_02001728();
    Call1(Func_020061fa, 0x943);
}

void Scene_UpdateActorTwelveTransition(void)
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

void Scene_UpdateObjectPairA(void)
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

void Scene_UpdateObjectPairB(void)
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

void Scene_UpdateTableBObjectPair(void)
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

void Scene_UpdateObjectPairC(void)
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

static __inline__ void SetSceneValue(s16 *field, s32 value)
{
    *field = value;
}

void Scene_UpdateActorPairInteraction(void)
{
    struct ObjectRuntime *actor = Func_020067ac(9);
    struct ObjectRuntime *other = Func_020067b4(10);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Func_020067a6(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
        other->movement_state = 1;
    } else if (!Value1(Func_020067de, 0x214)) {
        actor->movement_state = 0;
        other->movement_state = 0;
        if (!Value1(Func_020067de, 0x214) && actor->movement_state == 0) {
            work[8] = Func_02006d72(Func_02006c7a(8912896.0, Func_02006ce6(actor->x)));
        }
        if (!Func_02002314()) {
            if (Data_02000240[294] != 0) {
                if (Func_02002b3e(9) && Data_02000240[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
                if (Func_02002b3e(10) && Data_02000240[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
            }
            if (Data_02000240[294] == 0) {
                if (Func_02002b72(9)) {
                    Func_020068a0(0x215);
                    Func_020068a0(0x214);
                }
                if (Func_02002b72(10)) {
                    Func_020068a0(0x215);
                    Func_020068a0(0x214);
                }
            }
            if (Value1(Func_020067de, 0x214)) {
                SetSceneValue(&scene[193], 91);
            }
        }
    }
}

void Scene_UpdateActorSeventeenInteraction(void)
{
    struct ObjectRuntime *actor = Func_020069e6(17);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    Func_02006afa(actor);
    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Func_020069dc(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Value1(Func_02006a0e, 0x214)) {
        actor->movement_state = 0;
        if (!Value1(Func_02006a0e, 0x214) && actor->movement_state == 0) {
            work[8] = 0x3400000 - actor->x;
            work[9] = 0x2400000 - actor->z;
        }
        if (!Func_02002756()) {
            Func_02002dbc(17);
            if (Func_02002d5a(17) && Data_02000240[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (Data_02000240[294] == 0) {
                if (Func_02002d58(17)) {
                    Func_02006a96(0x215);
                    Func_02006a96(0x214);
                }
            }
            if (Value1(Func_02006a0e, 0x214)) {
                SetSceneValue(&scene[193], 92);
            }
        }
    }
}

void Scene_UpdateActorEighteenInteraction(void)
{
    struct ObjectRuntime *actor = Func_02006b82(18);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Func_02006b74(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Value1(Func_02006ba6, 0x214)) {
        actor->movement_state = 0;
        if (!Value1(Func_02006ba6, 0x214) && actor->movement_state == 0) {
            work[8] = 0x2f00000 - actor->x;
            work[9] = 0x1f00000 - actor->z;
        }
        if (!Func_02002a96()) {
            if (Func_02002eec(18) && Data_02000240[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (Data_02000240[294] == 0) {
                if (Func_02002eea(18)) {
                    Func_02006c28(0x215);
                    Func_02006c28(0x214);
                }
            }
            if (Value1(Func_02006ba6, 0x214)) {
                SetSceneValue(&scene[193], 93);
            }
        }
    }
}

void Scene_SelectActorTwentyOneMessage(void)
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

void Scene_RunActorTwentyOneSequence(void)
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

/* map/locations/heidia/village/actor_setup.c */
extern u8 Data_03001ebc[];
void Func_0200a812();
s32 Func_0200a94c();
void Func_02005fbc();
void Func_0200a962();
void Func_0200a96a();
s32 Func_0200a968();
void Func_0200aa06();
void Func_0200a98c();
s32 Func_0200a982();
void Func_0200aa20();
void Func_02007658();
s32 Func_0200a99a();
void Func_0200aa38();
void Func_0200a9c0();
s32 Func_0200a9b6();
void Func_0200761a();
s32 Func_0200a9c4();
void Func_0200765c();
void Func_0200a9f0();
struct ObjectRuntime *Func_0200aa16();
struct ObjectRuntime *Func_0200aa2a();
struct ObjectRuntime *Village_GetActor();
struct ObjectRuntime *Func_0200aa52();
void Func_0200a9f6();
struct ObjectRuntime *Func_0200aa6c();
struct ObjectRuntime *Func_0200aa84();
void Func_0200aa22();
void Func_0200aa76();
void Village_FinishActorRestore();
s32 Func_0200aa86(s32);
void Func_02006360(s32);
s32 Func_0200aa96(s32);
void Func_02006370(s32);
s32 Func_0200aaa8(s32);
void Func_02006382(s32);
s32 Func_0200aac8(s32);
void Func_020062f2(s32);
s32 Func_0200aad8(s32);
void Func_02006302(s32);
s32 Func_0200aaf2(s32);
void Func_02006240(s32);
s32 Func_0200ab02(s32);
void Func_02006250(s32);
s32 Func_0200ab12(s32);
void Func_02006260(s32);

extern s16 Data_02000240[];
extern u8 Data_02009151[], Data_0200938d[], Data_02009a45[];
void Func_0200a598();
void Func_0200a7d0();
void Func_0200a7d8();
void Func_0200a7e0();
s32 Func_0200a716();
s32 Func_0200a72a();
s32 Func_0200a73e();
s32 Func_0200a752();
void Func_0200a7b4();
void Func_0200a7c8();
void Func_0200a7dc();
void Func_0200a7f0();
void Func_0200a7f8();
void Func_0200a816();
struct ObjectRuntime *Func_0200a874();
void Func_0200a812_setup();
s32 Func_0200a842();
void Func_0200a8e4();
void Func_0200a854();
void Func_0200a7f6();
void Func_0200a7f4();
void Func_0200a828();
void Func_0200a7fe();
s32 Func_0200a89c();
void Func_02007300();
void Func_02007314();
void Func_0200a844();
void Func_0200a8c4();
struct ObjectRuntime *Func_0200a922();
struct ObjectRuntime *Func_0200a92a();
void Func_0200a8c8();
void Func_0200a9f8();

extern u8 Data_02009719[], Data_0200975d[], Data_020097bd[];
extern u8 Data_02009529[], Data_020099e9[], Data_0200969d[];
void Func_0200a3d6();
s32 Func_0200a572();
s32 Func_0200a57c();
s32 Func_0200a586();
s32 Func_0200a5b2();
s32 Func_0200a5bc();
void Func_0200a5ba();
void Func_0200a5ee();
void Func_0200a5c4();
void Func_0200a620();
void Func_0200a650();
s32 Func_0200a602();
void Func_0200a682();
void Func_0200a772();
void Func_0200a77a();
void Func_0200a782();
void Func_0200a78a();
void Func_0200a792();
void Func_0200a640();

static __inline__ void Call1(void (*f)(), s32 value)
{
    f(value);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Scheduler_AddOrUpdateCallback returns an index even when it is ignored. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Scene_InstallSceneTasks(void)
{
    Func_0200a3d6();
    switch (Data_02000240[225]) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Value2(Func_0200a572, (s32)Data_02009719, 3200);
        Value2(Func_0200a57c, (s32)Data_0200975d, 3200);
        Value2(Func_0200a586, (s32)Data_020097bd, 3200);
        Func_0200a650(0xe00);
        break;
    case 12:
    case 19:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x209;
        Func_0200a650(0xc00);
        break;
    case 16:
    case 17:
    case 18:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Value2(Func_0200a5b2, (s32)Data_02009529, 3200);
        Value2(Func_0200a5bc, (s32)Data_020099e9, 3200);
        Func_0200a5ba(1);
        Func_0200a5ee();
        Func_0200a5c4(1);
        Call6(Func_0200a620, 101, 9, 10, 8, 110, 9);
        Func_0200a650(0xe00);
        break;
    case 13:
    case 14:
    case 15:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Value2(Func_0200a602, (s32)Data_0200969d, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a682(0xe00);
        break;
    }
    Func_0200a772(18, 1);
    Func_0200a77a(17, 1);
    Func_0200a782(21, 1);
    Func_0200a78a(12, 1);
    Func_0200a792(13, 1);
    Func_0200a640(1);
}

void Scene_SetupActorsForScene(void)
{
    struct ObjectRuntime *actor;

    Func_0200a598();
    Func_0200a7d0(9, 1);
    Func_0200a7d8(10, 1);
    Func_0200a7e0(17, 1);
    if (Func_0200a716(0x94c)) {
        Func_0200a7b4(15, 0, 0);
    }
    if (Func_0200a72a(0x949)) {
        Func_0200a7c8(11, 0, 0);
    }
    if (Func_0200a73e(0x94b)) {
        Func_0200a7dc(16, 0, 0);
    }
    if (Func_0200a752(0xf2e)) {
        Func_0200a7f0(8, 0, 0);
    }
    switch (Data_02000240[225]) {
    case 1:
    case 2:
    case 3:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a7f8(0xe00);
        Func_0200a7f6(Data_02009151, 3200);
        Func_0200a7f4(1);
        Func_0200a828();
        Func_0200a7fe(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x209;
        Func_0200a816(0xc00);
        Func_0200a812_setup(Func_0200a874(24), 0);
        if (Func_0200a842(0x314)) {
            Call3(Func_0200a8e4, 25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a854(0xe00);
        Func_0200a7f6(Data_0200938d, 3200);
        Func_0200a7f4(1);
        Func_0200a828();
        Func_0200a7fe(1);
        break;
    case 11:
    case 12:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        if (Func_0200a89c(0x94a)) {
            Func_02007300();
        }
        break;
    case 31:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_02007314();
        break;
    case 14:
    case 15:
    case 16:
        Func_0200a844(Data_02009a45, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a8c4(0xe00);
        break;
    }
    actor = Func_0200a922(8);
    Func_0200a8c8(Func_0200a92a(8), 0);
    Func_0200a9f8(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void Scene_RestoreActorsFromFlags(void)
{
    struct ObjectRuntime *actor;

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
    Func_0200a812();
    if (Func_0200a94c(0x943)) {
        Func_02005fbc();
    }
    Call1(Func_0200a962, 0x217);
    Call1(Func_0200a96a, 0x218);
    if (Func_0200a968(0x944)) {
        Func_0200aa06(8, 0, 0);
        Call1(Func_0200a98c, 0x217);
    }
    if (Func_0200a982(0x945)) {
        Func_0200aa20(9, 0, 0);
        Func_02007658();
    }
    if (Func_0200a99a(0x946)) {
        Func_0200aa38(10, 0, 0);
        Call1(Func_0200a9c0, 0x218);
    }
    if (Func_0200a9b6(0x947)) {
        Func_0200761a();
    }
    if (Func_0200a9c4(0x948)) {
        Func_0200765c();
    }
    Func_0200a9f0();
    actor = Func_0200aa16(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa2a(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Village_GetActor(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa52(11);
    if (actor != 0) {
        Func_0200a9f6(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Func_0200aa6c(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Func_0200aa22(Func_0200aa84(11), 0);
    Func_0200aa76();
    Village_FinishActorRestore(0xe00);
}

void Scene_ActivateThreeActorGroup(void)
{
    if (Func_0200aa86(0x35a)) {
        Func_02006360(0);
    }
    if (Func_0200aa96(0x35b)) {
        Func_02006370(1);
    }
    if (Func_0200aaa8(0x35c)) {
        Func_02006382(2);
    }
}

void Scene_ActivateTwoActorGroup(void)
{
    if (Func_0200aac8(0x358)) {
        Func_020062f2(0);
    }
    if (Func_0200aad8(0x359)) {
        Func_02006302(1);
    }
}

void Scene_ActivateAlternateActorGroup(void)
{
    if (Func_0200aaf2(0x355)) {
        Func_02006240(0);
    }
    if (Func_0200ab02(0x356)) {
        Func_02006250(1);
    }
    if (Func_0200ab12(0x357)) {
        Func_02006260(2);
    }
}

/* map/locations/heidia/village/actor_steps.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002424[];
extern u8 Data_03001ebc[];
void Func_02007330();
void Func_020073e2();
void Func_020073f0_a();
void Func_020073f0_b();
void Func_020073fa();
void Func_02007400();
void Func_02007416();
void Func_02007418();
void Func_0200742a();
void Func_0200742e_a();
void Func_0200742e_b();
void Func_02007446();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_02001cf0(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = (s32)Data_00002424;
    Func_020073e2(base6_2424);
    Func_020073fa(a0, 0);
    Func_02007330(120);
    Call3(Func_0200742a, a0, 0x101, 60);
    Func_02007400((base6_2424 + 1));
    Func_02007418(a0, 0);
    Func_020073f0_a(a0, 1);
    Func_02007416((base6_2424 + 2));
    Func_0200742e_a(a0, 0);
    Func_020073f0_b(a0, 4);
    Func_0200742e_b((base6_2424 + 3));
    Func_02007446(a0, 0);
}

/* map/locations/heidia/village/conditional_scene.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000244f[];
extern u8 Data_00002455[];
extern u8 Data_03001ebc[];
s32 Func_02009dae();
void Func_02009dc8();
s32 Func_02009dcc();
void Func_02009dec();
void Func_02009e0a();
void Func_02009e3e();
void Func_02009e4c();
void Func_02009e82();
void Func_02009e90();
void Func_02009ea8();
void Func_02009eae();
void Func_02009eba();
s32 Func_02009ebe();
s32 Func_02009ece();
void Func_02009ed4();
void Func_02009ed6();
void Func_02009eda();
void Func_02009ef2();
void Func_02009ef8();
void Func_02009f14();
void Func_02009f1c();
void Func_02009f34_a();
void Func_02009f34_b();
void Func_02009f38();
void Func_02009f40();
void Func_02009f46();
void Func_02009f4c();
void Func_02009f50();
void Func_02009f54();
void Func_02009f5e_a();
void Func_02009f5e_b();
void Func_02009f66();
s32 Func_02009f6e();
void Func_02009f78();
void Func_02009f7e();
void Func_02009f88_a();
void Func_02009f88_b();
void Func_02009f98_a();
void Func_02009f98_b();
void Func_02009f98_c();
void Func_02009fa8();
s32 Func_02009fb8();
void Func_02009fbe();
s32 Func_02009fce();
void Func_02009fd0();
void Func_02009fd6();
void Func_02009fe6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_02004794(void)
{
    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Func_02009dc8();
    if (Value1(Func_02009dae, 0x941) != 0) {
        Call1(Func_02009e90, 0x2566);
        Func_02009ea8(18, 0);
        Func_02009dec();
    } else {
        if (Value1(Func_02009dcc, 0x313) != 0) {
            Call1(Func_02009eae, 0x2457);
            Value2(Func_02009ebe, 25, 0);
            Func_02009e0a();
        } else {
            Call3(Func_02009ef8, 25, 0x102, 30);
            Func_02009eba(25, 0, 0);
            base5_244f = (s32)Data_0000244f;
            Func_02009eda(base5_244f);
            Func_02009ef2(25, 0);
            Func_02009ed4(25, 24, 0);
            Func_02009f54(24, 1);
            Func_02009f50();
            Func_02009e3e(60);
            Func_02009f66(0, 1);
            Func_02009e4c(20);
            Call3(Func_02009f46, 25, 0x105, 60);
            Func_02009f1c((base5_244f + 1));
            Func_02009f34_a(25, 0);
            Call3(Func_02009f5e_a, 25, 0x107, 60);
            Func_02009f34_b((base5_244f + 2));
            Func_02009f4c(25, 0);
            Func_02009e82(70);
            Call3(Func_02009f7e, 25, 0x100, 60);
            Func_02009f40(25, 0, 0);
            Func_02009f5e_b((base5_244f + 3));
            Value2(Func_02009f6e, 25, 0);
            if (Value2(Func_02009ece, 0, 0) == 0) {
                Func_02009f78((base5_244f + 4));
                Func_02009f88_a(25, 0);
            } else {
                Func_02009f88_b((base5_244f + 5));
                Func_02009f98_a(25, 0);
            }
            Func_02009ed6(60);
            Call3(Func_02009fd0, 25, 0x105, 60);
            base5_2455 = (s32)Data_00002455;
            Func_02009fa8(base5_2455);
            Value2(Func_02009fb8, 25, 0);
            Func_02009f98_b(25, 1);
            Func_02009fbe((base5_2455 + 1));
            Value2(Func_02009fce, 25, 0);
            Func_02009f98_c(25, 3);
            Func_02009fd6((base5_2455 + 2));
            Value2(Func_02009fe6, 25, 0);
            Call1(Func_02009f14, 0x313);
            Func_02009f38();
        }
    }
}

/* map/locations/heidia/village/main_scene.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002481[];
extern u8 Data_0000248e[];
extern u8 Data_000024a6[];
void Func_02003d0c();
void Func_02008646();
void Func_02008674_a();
void Func_02008674_b();
void Func_0200867a();
void Func_020086b0();
u8 *Scene_GetRecord_1();
void Func_0200873a();
void Func_02008754();
void Func_0200876e();
void Func_02008788();
void Func_0200879a();
void Func_020087a2();
void Func_020087ae();
void Func_020087bc();
void Func_020087c8();
void Func_020087d6();
void Func_020087e2();
void Func_020087e6();
void Func_020087ee();
void Func_020087f4();
void Func_020087f6();
void Func_020087fc();
void Func_02008810();
void Func_02008816();
void Func_02008830();
void Func_02008832();
void Func_0200884c();
void Func_0200884e();
void Func_02008860();
void Func_02008862();
void Func_02008868();
void Func_0200887a_a();
void Func_0200887a_b();
void Func_02008892();
void Func_02008896();
void Func_02008898();
void Func_020088a4();
void Func_020088b6();
void Func_020088bc_a();
void Func_020088bc_b();
void Func_020088c0();
void Func_020088c6_a();
void Func_020088c6_b();
void Func_020088ca();
void Func_020088cc();
void Func_020088d0();
void Func_020088d4_a();
void Func_020088d4_b();
void Func_020088dc();
void Func_020088fc();
void Func_02008900();
void Func_02008912();
void Func_0200891a();
void Func_02008928();
void Func_0200892a();
void Func_0200892e();
void Func_02008932();
void Func_02008938();
void Func_0200893c();
void Func_0200893e();
void Func_02008946_a();
void Func_02008946_b();
void Func_02008946_c();
void Func_02008954();
void Func_0200896c();
void Func_02008984();
s32 Func_0200898a();
void Func_0200898e();
void Func_02008996();
void Func_0200899a();
void Func_020089ae();
void Func_020089b2();
void Func_020089bc();
void Func_020089c2();
void Func_020089ca();
void Func_020089da();
void Func_020089de();
void Func_020089e2();
void Func_020089ec();
void Func_020089f6();
void Func_020089fa_a();
void Func_020089fa_b();
void Func_020089fc();
void Func_020089fe();
void Func_02008a00();
void Func_02008a06();
void Func_02008a0c();
void Func_02008a18();
void Func_02008a1a_a();
void Func_02008a1a_b();
s32 Func_02008a2a();
void Func_02008a38();
void Func_02008a3a();
void Func_02008a70();
void Func_02008a76();
void Func_02008a7a();
void Func_02008a84();
void Func_02008a88();
void Func_02008a8e_a();
void Func_02008a8e_b();
void Func_02008a9c();
void Func_02008a9e();
void Func_02008aae();
void Func_02008acc();
void Func_02008aea();
void Func_02008aec_a();
void Func_02008aec_b();
void Func_02008af2();
void Func_02008af6();
void Func_02008af8();
void Func_02008afa();
void Func_02008b00();
void Func_02008b02();
void Func_02008b04();
void Func_02008b06();
void Func_02008b0a();
void Func_02008b14();
void Func_02008b22();
void Func_02008b3a();
void Func_02008b44();
void Func_02008b4e();
void Func_02008b56();
void Func_02008b58();
void Func_02008b62();
void Func_02008b68();
void Func_02008b74();
void Func_02008b7a();
void Func_02008b86();
void Func_02008b98_a();
void Func_02008b98_b();
void Func_02008bb4();
void Func_02008bb6();
void Func_02008bb8();
void Func_02008bc6();
void Func_02008bd2();
void Func_02008bde();
void Func_02008be4_a();
void Func_02008be4_b();
void Func_02008c02();
void Func_02008c14();
void Func_02008c26();
void Func_02008c30();
void Func_02008c3c();
void Func_02008c48_a();
void Func_02008c48_b();
void Func_02008c50();
void Func_02008c52();
void Func_02008c58_a();
void Func_02008c58_b();
void Func_02008c60();
void Func_02008c62();
void Func_02008c68();
void Func_02008c6a();
void Func_02008c6c();
void Func_02008c7e_a();
void Func_02008c7e_b();
void Func_02008c80();
void Func_02008c84();
void Func_02008c88();
void Func_02008c96();
void Func_02008cac();
void Func_02008cba();
void Func_02008ccc();
void Func_02008cd8();
void Func_02008ce0();
void Func_02008ce6();
void Func_02008d1a();
void Func_02008d24();
void Func_02008d2e();
void Func_02008d38();
void Func_02008d42();
void Func_02008d4a();
void Func_02008d52();
void Func_02008d5a();
void Func_02008d64();
void Func_02008d6c();
void Func_02008d70();
void Func_02008d7c();
void Func_02008d88();
void Func_02008d8a();
void Func_02008d94_a();
void Func_02008d94_b();
void Func_02008da2();
void Func_02008da6();
void Func_02008db2();
void Func_02008db8();
void Func_02008dc4();
void Func_02008dd6();
void Func_02008dde_a();
void Func_02008dde_b();
void Func_02008de4();
void Func_02008dee();
void Func_02008df0();
void Func_02008dfc_a();
void Func_02008dfc_b();
void Func_02008dfe();
void Func_02008e02();
void Func_02008e0e();
void Func_02008e10();
void Func_02008e1a();
void Func_02008e2c();
void Func_02008e32();
void Func_02008e34();
void Func_02008e3c();
void Func_02008e4c();
void Func_02008e5a();
void Func_02008e7c();
void Func_02008e82();
void Func_02008e90();
void Func_02008e98();
void Func_02008e9e();
void Func_02008ea2();
void Func_02008eac();
void Func_02008eb8();
void Func_02008ec4();
void Func_02008ede();
void Func_02008ee6();
void Func_02008ef6();
void Func_02008ef8();
void Func_02008f02_a();
void Func_02008f02_b();
void Func_02008f0a();
void Func_02008f14();
void Func_02008f20();
void Func_02008f26();
void Func_02008f30();
void Func_02008f36_a();
void Func_02008f36_b();
void Func_02008f38();
void Func_02008f50();
void Func_02008f54();
void Func_02008f56();
void Func_02008f64();
void Func_02008f6e();
void Func_02008f76();
void Func_02008f78();
void Func_02008f90();
void Func_02008f96();
void Func_02008fa2();
void Func_02008fb4();
void Func_02008fc2();
void Func_02008fc6();
void Func_02008fcc();
void Func_02008fd6();
void Func_02008fd8();
void Func_02008fe2();
void Func_02008fe8();
void Func_02008ff8();
void Func_02008ffc();
void Func_02009006_a();
void Func_02009006_b();
void Func_02009010();
void Func_02009014();
void Func_0200901a();
void Func_0200901e_a();
void Func_0200901e_b();
void Func_02009032();
void Func_02009036_a();
void Func_02009036_b();
void Func_0200903c();
void Func_02009050();
void Func_02009052();
void Func_02009054();
void Func_02009058();
void Func_02009060();
void Func_02009068();
void Func_02009070();
void Func_02009078();
void Func_02009088();
void Func_0200908e();
void Func_0200909e();
void Func_020090b6();
void Func_020090c4();
void Func_020090d6();
void Func_020090dc();
void Func_020090f0();
void Func_020090f4_a();
void Func_020090f4_b();
void Func_020090fa();
void Func_020090fe();
void Func_02009112();
void Func_0200911a();
void Func_02009126();
void Func_02009132();
void Func_02009140();
void Func_02009146();
void Func_02009152();
void Func_0200915a();
void Func_0200915e();
void Func_02009160();
void Func_02009164_a();
void Func_02009164_b();
void Func_0200916e();
void Func_02009178_a();
void Func_02009178_b();
void Func_0200917a();
void Func_0200917c();
void Func_02009182();
void Func_0200918a();
void Func_02009190();
void Func_0200919a_a();
void Func_0200919a_b();
void Func_020091b2();
void Func_020091c6();
void Func_020091c8();
void Func_020091d2();
void Func_020091d6();
void Func_020091de();
void Func_020091f2();
void Func_020091f4_a();
void Func_020091f4_b();
void Func_020091fa();
void Func_02009208();
void Func_02009212();
void Func_0200921c();
void Func_02009222();
void Func_0200922c();
void Func_02009230();
void Func_0200923a();
void Func_0200923c();
void Func_0200924c();
void Func_02009256();
void Func_02009258();
void Func_02009260();
void Func_0200926a();
void Func_02009286();
void Func_0200929e();
s32 Func_020092ac();
void Func_020092b4();
void Func_020092b6();
void Func_020092bc();
void Func_020092d2();
void Func_020092d4();
void Func_020092d8();
void Func_020092ee();
void Func_020092f0();
void Func_020092f2();
void Func_020092f8();
void Func_020092fc();
void Func_02009300();
void Func_02009306();
s32 Func_02009308();
void Func_02009310();
void Func_02009320();
void Func_0200935e();
void Func_02009376_a();
void Func_02009376_b();
void Func_0200937c();
void Func_02009380();
void Func_0200938a();
void Func_02009394_a();
void Func_02009394_b();
void Func_0200939c();
void Func_0200939e();
void Func_020093a4();
void Func_020093a8();
void Func_020093b0();
void Func_020093b2();
void Func_020093ba();
void Func_020093ea();
void Func_020093ee_a();
void Func_020093ee_b();
void Func_020093fc();
void Func_02009406();
void Func_02009416();
void Func_0200941e();
void Func_02009422();
void Func_02009426();
void Func_02009430();
void Func_02009434();
void Func_0200943e();
void Func_02009448();
void Func_0200944a();
void Func_0200945a();
void Func_02009464();
void Func_0200946a_a();
void Func_0200946a_b();
void Func_0200947a();
void Func_02009482();
void Func_02009486();
void Func_02009488();
void Func_020094a0();
void Func_020094a6_a();
void Func_020094a6_b();
void Func_020094aa();
void Func_020094b4();
void Func_020094b6();
void Func_020094c2();
void Func_020094cc();
void Func_020094d6();
void Func_020094d8();
void Func_020094da();
void Func_020094de();
void Func_020094e0();
void Func_020094ec();
void Func_020094f0();
void Func_02009504();
void Func_02009516();
void Func_02009518();
void Func_0200951c();
void Func_02009520();
void Func_02009524();
void Func_02009526();
void Func_0200953e_a();
void Func_0200953e_b();
void Func_02009540();
void Func_02009550();
void Func_02009558_a();
void Func_02009558_b();
void Func_02009568();
void Func_02009570();
void Func_02009580();
void Func_02009582();
void Func_02009590_a();
void Func_02009590_b();
void Func_02009598();
void Func_020095a6_a();
void Func_020095a6_b();
void Func_020095b0();
void Func_020095b4();
void Func_020095bc();
void Func_020095c0();
void Func_020095c8_a();
void Func_020095c8_b();
void Func_020095e0();
void Func_020095ec();
void Func_020095fc();
void Func_02009604();
void Func_0200960a();
void Func_02009610_a();
void Func_02009610_b();
void Func_02009620();
void Func_02009622();
void Func_02009628();
void Func_0200962a();
void Func_0200962c();
void Func_02009632();
void Func_02009638();
void Func_0200963a();
void Func_02009644();
void Func_0200964a();
void Func_0200964e();
void Func_02009654();
void Func_0200968c();
void Func_02009694();
void Func_020096aa();
void Func_020096ac();
void Func_020096ae();
void Func_020096b0();
void Func_020096ba();
void Func_020096bc();
void Func_020096c2();
void Func_020096c6();
void Func_020096ca();
void Func_020096d2();
void Func_020096d6();
void Func_020096dc();
void Func_020096e6();
void Func_020096f0();
void Func_02009708();
void Func_02009716_a();
void Func_02009716_b();
void Func_0200972e();
void Func_0200973c();
void Func_02009754();
void Func_02009758();
void Func_02009762();
void Func_0200976c_a();
void Func_0200976c_b();
void Func_0200976c_c();
void Func_02009772();
void Func_02009776();
void Func_0200977e();
void Func_0200978a();
void Func_0200978c();
void Func_02009790();
void Func_02009792();
void Func_020097aa();
void Func_020097c8();
void Func_020097d4();
void Func_020097d6();
void Func_020097d8();
void Func_020097de_a();
void Func_020097de_b();
void Func_020097e0();
void Func_020097e8_a();
void Func_020097e8_b();
void Func_020097f0();
void Func_020097f2();
void Func_020097f4();
void Func_020097f8();
void Func_02009804();
void Func_02009806();
void Func_02009808();
void Func_02009820();
void Func_02009830();
void Func_02009834();
void Func_02009854();
void Func_02009868();
void Func_0200986a();
void Func_0200986c();
void Func_0200986e();
void Func_02009870_a();
void Func_02009870_b();
void Func_02009888();
void Func_0200988c();
void Func_0200988e();
void Func_02009898();
void Func_0200989e();
void Func_020098ae();
void Func_020098b6_a();
void Func_020098b6_b();
void Func_020098bc_a();
void Func_020098bc_b();
void Func_020098ce();
void Func_020098d4_a();
void Func_020098d4_b();
void Func_020098ec();
void Func_020098f8();
void Func_020098fc();
void Func_02009914();
void Func_02009916();
void Func_02009920();
void Func_0200992e();
void Func_02009938();
void Func_0200993e_a();
void Func_0200993e_b();
void Func_0200994a();
void Func_0200994c();
void Func_02009950_a();
void Func_02009950_b();
void Func_02009956();
void Func_0200995a();
void Func_0200996e();
void Func_02009984();
void Func_0200998c();
s32 Func_02009992();
void Func_0200999a();
void Func_0200999c();
void Func_020099a0();
void Func_020099aa();
void Func_020099b4();
void Func_020099b8();
void Func_020099c0();
void Func_020099c8();
void Func_020099ca();
void Func_020099d4();
void Func_020099d8();
void Func_020099dc();
void Func_020099ee();
void Func_020099f4_a();
void Func_020099f4_b();
u8 *Scene_GetRecord_2();
void Func_02009a04();
void Func_02009a0c();
void Func_02009a14();
void Func_02009a1c();
void Func_02009a22();
void Func_02009a24();
u8 *Scene_GetRecord_3();
s32 Func_02009a32();
void Func_02009a3a();
void Func_02009a3c();
void Func_02009a3e();
void Func_02009a44();
void Func_02009a52();
void Func_02009a56();
u8 *Motion_CommitPos_22();
void Func_02009a64();
void Func_02009a6a();
void Func_02009a72();
void Func_02009a82();
void Func_02009a90();
void Func_02009a9a_a();
void Func_02009a9a_b();
void Func_02009aa2();
void Func_02009aa8();
void Func_02009ab2();
void Func_02009ab8();
void Func_02009ac0();
void Func_02009ace();
void Func_02009ad2();
void Func_02009aec();
void Func_02009aee();
void Func_02009b04();
void Func_02009b22();
void Func_02009b24();
void Func_02009b2c();
void Func_02009b3a();
void Func_02009b42();
void Func_02009b5a();
void Func_02009b68();
void Func_02009b70_a();
void Func_02009b70_b();
void Func_02009b76();
void Func_02009b7a();
void Func_02009b80();
void Func_02009b88();
void Func_02009b96();
void Func_02009b98_a();
void Func_02009b98_b();
void Func_02009b98_c();
void Func_02009bb0();
void Func_02009bb6();
void Func_02009bc6();
void Func_02009bf4();
void Func_02009bf8();
void Func_02009bfa();
void Func_02009c02();
void Func_02009c06();
void Func_02009c08();
void Func_02009c10();
void Func_02009c12();
void Func_02009c14();
void Func_02009c16();
void Func_02009c18();
void Func_02009c1c();
void Func_02009c24();
void Func_02009c2c();
void Func_02009c36();
void Func_02009c40();
void Func_02009c46();
void Func_02009c4a();
void Func_02009c4c_a();
void Func_02009c4c_b();
void Func_02009c54();
void Func_02009c5e();
void Func_02009c66();
void Func_02009c68();
void Func_02009c72();
void Func_02009d7e();
void Func_02009d82();
void Func_02009d9c();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_Set_1(a0) Call1(Func_02008674_a, a0)
#define GameFlag_Set_2(a0) Call1(Func_0200867a, a0)
#define Battle_Reset_1(args...) Func_020086b0(args)
#define Scene_GetRecord_1(args...) Func_020086d6(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_0200873a, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_020087ae, a0, a1, a2)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_02008754, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_020087c8, a0, a1, a2)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Call3(Func_0200876e, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_020087e2, a0, a1, a2)
#define Motion_SetHPosTerrain_4(a0, a1, a2) Call3(Func_02008788, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_020087fc, a0, a1, a2)
#define Motion_SetHPosTerrain_5(a0, a1, a2) Call3(Func_020087a2, a0, a1, a2)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02008816, a0, a1, a2)
#define Motion_SetHPosTerrain_6(a0, a1, a2) Call3(Func_020087bc, a0, a1, a2)
#define Motion_ArmCb_6(a0, a1, a2) Call3(Func_02008830, a0, a1, a2)
#define Motion_SetHPosTerrain_7(a0, a1, a2) Call3(Func_020087d6, a0, a1, a2)
#define Motion_SetActionVariant_1(args...) Func_0200884e(args)
#define Object_SetModeById_1(args...) Func_020087ee(args)
#define Motion_SetHPosTerrain_8(a0, a1, a2) Call3(Func_020087f4, a0, a1, a2)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_02008868, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02008810(args)
#define Battle_WaitMode0_1(args...) Func_020088dc(args)
#define Battle_WaitMode0_2(args...) Func_0200879a(args)
#define SceneWork_SetStepValue_1(args...) Func_02008862(args)
#define BattleEv_RunWait_1(args...) Func_0200887a_a(args)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_020088a4, a0, a1, a2)
#define SceneWork_SetStepValue_2(args...) Func_0200887a_b(args)
#define BattleEv_RunWait_2(args...) Func_02008892(args)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_020088bc_a, a0, a1, a2)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_020088c6_a, a0, a1, a2)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_020088d0, a0, a1, a2)
#define Battle_WaitMode0_3(args...) Func_020087e6(args)
#define Motion_SetAngleToward_1(args...) Func_02008898(args)
#define Battle_WaitMode0_4(args...) Func_020087f6(args)
#define SceneWork_SetStepValue_3(args...) Func_020088bc_b(args)
#define BattleEv_RunWait_3(args...) Func_020088d4_a(args)
#define Motion_SetAngleToward_2(args...) Func_020088b6(args)
#define Motion_SetAngleToward_3(args...) Func_020088c0(args)
#define Motion_SetAngleToward_4(args...) Func_020088ca(args)
#define Motion_SetAngleToward_5(args...) Func_020088d4_b(args)
#define Battle_WaitMode0_5(args...) Func_02008832(args)
#define Motion_ResetPosMode2_1(a0, a1, a2) Call3(Func_02008896, a0, a1, a2)
#define Object_SetModeById_3(args...) Func_020088c6_b(args)
#define Battle_WaitMode0_6(args...) Func_0200884c(args)
#define SceneWork_SetStepValue_4(args...) Func_02008912(args)
#define BattleEv_RunWait_4(args...) Func_0200892a(args)
#define Battle_WaitMode0_7(args...) Func_02008860(args)
#define Motion_SetVarCb_1(args...) Func_02008900(args)
#define SceneWork_SetStepValue_5(args...) Func_0200892e(args)
#define BattleEv_RunWait_5(args...) Func_02008946_a(args)
#define Motion_SetAngleToward_6(args...) Func_02008928(args)
#define Motion_SetAngleToward_7(args...) Func_02008932(args)
#define Motion_SetAngleToward_8(args...) Func_0200893c(args)
#define Motion_SetAngleToward_9(args...) Func_02008946_b(args)
#define Motion_SetVarCb_2(args...) Func_0200893e(args)
#define SceneWork_SetStepValue_6(args...) Func_0200896c(args)
#define BattleEv_RunWait_6(args...) Func_02008984(args)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_020089ae, a0, a1, a2)
#define Object_SetModeById_4(args...) Func_02008946_c(args)
#define Battle_WaitMode0_8(args...) Func_020088cc(args)
#define Object_SetModeById_5(args...) Func_02008954(args)
#define SceneWork_SetStepValue_7(args...) Func_0200899a(args)
#define BattleEv_RunWait_7(args...) Func_020089b2(args)
#define BattleFx_SpawnLinked_6(a0, a1, a2) Call3(Func_020089de, a0, a1, a2)
#define Motion_SetVarCb_3(args...) Func_0200898e(args)
#define Battle_WaitMode0_9(args...) Func_020088fc(args)
#define SceneWork_SetStepValue_8(args...) Func_020089c2(args)
#define BattleEv_RunWait_8(args...) Func_020089da(args)
#define Motion_SetAngleToward_10(args...) Func_020089bc(args)
#define Battle_WaitMode0_10(args...) Func_0200891a(args)
#define SceneWork_SetStepValue_9(args...) Func_020089e2(args)
#define BattleEv_RunWait_9(args...) Func_020089fa_a(args)
#define Motion_SetVarCb_4(args...) Func_020089ca(args)
#define Battle_WaitMode0_11(args...) Func_02008938(args)
#define SceneWork_SetStepValue_10(args...) Func_02008a00(args)
#define BattleEv_RunWait_10(args...) Func_02008a18(args)
#define Motion_SetAngleToward_11(args...) Func_020089fa_b(args)
#define SceneWork_SetStepValue_11(args...) Func_02008a1a_a(args)
#define Motion_SetSpeed_1(a0, a1) Value2(Func_02008a2a, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_0200898a(args)
#define Object_SetModeById_6(args...) Func_020089f6(args)
#define Object_SetModeById_7(args...) Func_020089fe(args)
#define Object_SetModeById_8(args...) Func_02008a06(args)
#define Motion_SetAngleToward_12(args...) Func_02008a38(args)
#define Battle_WaitMode0_12(args...) Func_02008996(args)
#define SceneWork_SetStepValue_12(args...) Func_02008a76(args)
#define BattleEv_RunWait_11(args...) Func_02008a8e_a(args)
#define Motion_SetAngleToward_13(args...) Func_02008a70(args)
#define Motion_SetAngleToward_14(args...) Func_02008a7a(args)
#define Motion_SetAngleToward_15(args...) Func_02008a84(args)
#define Motion_SetAngleToward_16(args...) Func_02008a8e_b(args)
#define Battle_WaitMode0_13(args...) Func_020089ec(args)
#define Motion_SetAngleToward_17(args...) Func_02008a9e(args)
#define Battle_WaitMode0_14(args...) Func_020089fc(args)
#define Motion_SetAngleToward_18(args...) Func_02008aae(args)
#define Battle_WaitMode0_15(args...) Func_02008a0c(args)
#define Motion_CallWaitAnim_1(args...) Func_02008a9c(args)
#define Battle_WaitMode0_16(args...) Func_02008a1a_b(args)
#define Motion_SetAngleToward_19(args...) Func_02008acc(args)
#define SceneWork_SetStepValue_13(args...) Func_02008aec_a(args)
#define BattleEv_RunWait_12(args...) Func_02008b04(args)
#define Battle_WaitMode0_17(args...) Func_02008a3a(args)
#define Motion_SetAngleToward_20(args...) Func_02008aec_b(args)
#define Motion_SetAngleToward_21(args...) Func_02008af6(args)
#define Motion_SetAngleToward_22(args...) Func_02008b00(args)
#define Motion_SetAngleToward_23(args...) Func_02008b0a(args)
#define Object_SetModeById_9(args...) Func_02008aea(args)
#define Object_SetModeById_10(args...) Func_02008af2(args)
#define Object_SetModeById_11(args...) Func_02008afa(args)
#define Object_SetModeById_12(args...) Func_02008b02(args)
#define Battle_WaitMode0_18(args...) Func_02008a88(args)
#define Motion_SetAngleToward_24(args...) Func_02008b3a(args)
#define Motion_SetAngleToward_25(args...) Func_02008b44(args)
#define Motion_SetAngleToward_26(args...) Func_02008b4e(args)
#define Motion_SetAngleToward_27(args...) Func_02008b58(args)
#define Motion_SetAngleToward_28(args...) Func_02008b62(args)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02008af8, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02008b06, a0, a1, a2)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_02008b14, a0, a1, a2)
#define Motion_SetSpeed_5(a0, a1, a2) Call3(Func_02008b22, a0, a1, a2)
#define Motion_ResetPosMode2_2(a0, a1, a2) Call3(Func_02008b56, a0, a1, a2)
#define Motion_CommitPos_1(args...) Func_02008b74(args)
#define Motion_ResetPosMode2_3(a0, a1, a2) Call3(Func_02008b68, a0, a1, a2)
#define Motion_CommitPos_2(args...) Func_02008b86(args)
#define Motion_ResetPosMode2_4(a0, a1, a2) Call3(Func_02008b7a, a0, a1, a2)
#define Motion_CommitPos_3(args...) Func_02008b98_a(args)
#define Motion_ArmCb_8(a0, a1, a2) Call3(Func_02008c14, a0, a1, a2)
#define Motion_ResetPosMode2_5(a0, a1, a2) Call3(Func_02008b98_b, a0, a1, a2)
#define Motion_CommitPos_4(args...) Func_02008bb6(args)
#define Motion_ArmCb_9(args...) Func_02008c30(args)
#define Motion_ResetPosMode2_6(a0, a1, a2) Call3(Func_02008bb4, a0, a1, a2)
#define Motion_CommitPos_5(args...) Func_02008bd2(args)
#define Motion_ResetPosMode2_7(a0, a1, a2) Call3(Func_02008bc6, a0, a1, a2)
#define Motion_CommitPos_6(args...) Func_02008be4_a(args)
#define Motion_ArmCb_10(a0, a1, a2) Call3(Func_02008c60, a0, a1, a2)
#define Motion_ResetPosMode2_8(a0, a1, a2) Call3(Func_02008be4_b, a0, a1, a2)
#define Motion_CommitPos_7(args...) Func_02008c02(args)
#define Motion_ArmCb_11(a0, a1, a2) Call3(Func_02008c7e_a, a0, a1, a2)
#define Object_LinkPair_1(args...) Func_02008c58_a(args)
#define Object_LinkPair_2(args...) Func_02008c62(args)
#define Battle_WaitMode0_19(args...) Func_02008bb8(args)
#define Motion_CallWaitAnim_2(args...) Func_02008c48_a(args)
#define Object_SetModeById_13(args...) Func_02008c48_b(args)
#define Object_SetModeById_14(args...) Func_02008c50(args)
#define Object_SetModeById_15(args...) Func_02008c58_b(args)
#define Battle_WaitMode0_20(args...) Func_02008bde(args)
#define Audio_PlayCue_1(args...) Func_02008d64(args)
#define Object_SetModeById_16(args...) Func_02008c6c(args)
#define Motion_SetSpeed_6(a0, a1, a2) Call3(Func_02008c26, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_1(a0, a1, a2) Call3(Func_02008c6a, a0, a1, a2)
#define Motion_SetSpeed_7(a0, a1, a2) Call3(Func_02008c3c, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_2(a0, a1, a2) Call3(Func_02008c80, a0, a1, a2)
#define Motion_SetSpeed_8(a0, a1, a2) Call3(Func_02008c52, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_3(a0, a1, a2) Call3(Func_02008c96, a0, a1, a2)
#define Motion_SetSpeed_9(a0, a1, a2) Call3(Func_02008c68, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_4(a0, a1, a2) Call3(Func_02008cac, a0, a1, a2)
#define Motion_SetSpeed_10(a0, a1, a2) Call3(Func_02008c7e_b, a0, a1, a2)
#define Motion_SetSpeed_11(a0, a1, a2) Call3(Func_02008c88, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_5(a0, a1, a2) Call3(Func_02008ccc, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_6(a0, a1, a2) Call3(Func_02008cd8, a0, a1, a2)
#define Motion_CommitPos_8(args...) Func_02008ce6(args)
#define Audio_PlayCue_2(a0) Call1(Func_02008dfe, a0)
#define Battle_WaitMode0_21(args...) Func_02008c84(args)
#define Motion_ArmCb_12(a0, a1, a2) Call3(Func_02008d70, a0, a1, a2)
#define Motion_ArmCb_13(a0, a1, a2) Call3(Func_02008d7c, a0, a1, a2)
#define Motion_ArmCb_14(a0, a1, a2) Call3(Func_02008d88, a0, a1, a2)
#define Motion_ArmCb_15(a0, a1, a2) Call3(Func_02008d94_a, a0, a1, a2)
#define Battle_WaitMode0_22(args...) Func_02008cba(args)
#define Object_SetModeById_17(args...) Func_02008d42(args)
#define Object_SetModeById_18(args...) Func_02008d4a(args)
#define Object_SetModeById_19(args...) Func_02008d52(args)
#define Object_SetModeById_20(args...) Func_02008d5a(args)
#define Battle_WaitMode0_23(args...) Func_02008ce0(args)
#define Motion_SetSpeed_12(a0, a1, a2) Call3(Func_02008d1a, a0, a1, a2)
#define Motion_SetSpeed_13(a0, a1, a2) Call3(Func_02008d24, a0, a1, a2)
#define Motion_SetSpeed_14(a0, a1, a2) Call3(Func_02008d2e, a0, a1, a2)
#define Motion_SetSpeed_15(a0, a1, a2) Call3(Func_02008d38, a0, a1, a2)
#define Motion_ResetPosMode2_9(a0, a1, a2) Call3(Func_02008d6c, a0, a1, a2)
#define Motion_CommitPos_9(args...) Func_02008d8a(args)
#define Object_SetModeById_21(args...) Func_02008da2(args)
#define Motion_ArmCb_16(a0, a1, a2) Call3(Func_02008e0e, a0, a1, a2)
#define Motion_ResetPosMode2_10(a0, a1, a2) Call3(Func_02008d94_b, a0, a1, a2)
#define Motion_CommitPos_10(args...) Func_02008db2(args)
#define Motion_ResetPosMode2_11(a0, a1, a2) Call3(Func_02008da6, a0, a1, a2)
#define Motion_CommitPos_11(args...) Func_02008dc4(args)
#define Motion_ResetPosMode2_12(a0, a1, a2) Call3(Func_02008db8, a0, a1, a2)
#define Motion_CommitPos_12(args...) Func_02008dd6(args)
#define Object_SetModeById_22(args...) Func_02008dee(args)
#define Motion_ArmCb_17(a0, a1, a2) Call3(Func_02008e5a, a0, a1, a2)
#define Motion_ResetPosMode2_13(a0, a1, a2) Call3(Func_02008dde_a, a0, a1, a2)
#define Motion_CommitPos_13(args...) Func_02008dfc_a(args)
#define Motion_ResetPosMode2_14(a0, a1, a2) Call3(Func_02008df0, a0, a1, a2)
#define Motion_ResetPosMode2_15(a0, a1, a2) Call3(Func_02008dfc_b, a0, a1, a2)
#define Motion_CommitPos_14(args...) Func_02008e1a(args)
#define Object_SetModeById_23(args...) Func_02008e32(args)
#define Motion_ArmCb_18(a0, a1, a2) Call3(Func_02008e9e, a0, a1, a2)
#define Motion_CommitPos_15(args...) Func_02008e34(args)
#define Object_SetModeById_24(args...) Func_02008e4c(args)
#define Motion_ArmCb_19(a0, a1, a2) Call3(Func_02008eb8, a0, a1, a2)
#define Battle_WaitMode0_24(args...) Func_02008dde_b(args)
#define Battle_WaitMode0_25(args...) Func_02008de4(args)
#define SceneWork_SetStepValue_14(args...) Func_02008eac(args)
#define BattleEv_RunWait_13(args...) Func_02008ec4(args)
#define Object_SetModeById_25(args...) Func_02008e7c(args)
#define Battle_WaitMode0_26(args...) Func_02008e02(args)
#define Motion_SetVarCb_5(args...) Func_02008ea2(args)
#define Battle_WaitMode0_27(args...) Func_02008e10(args)
#define Object_SetModeById_26(args...) Func_02008e98(args)
#define SceneWork_SetStepValue_15(args...) Func_02008ede(args)
#define BattleEv_RunWait_14(args...) Func_02008ef6(args)
#define Battle_WaitMode0_28(args...) Func_02008e2c(args)
#define Object_LinkPair_3(args...) Func_02008ee6(args)
#define Battle_WaitMode0_29(args...) Func_02008e3c(args)
#define BattleFx_SpawnLinked_7(a0, a1, a2) Call3(Func_02008f36_a, a0, a1, a2)
#define Motion_SetAngleToward_29(args...) Func_02008ef8(args)
#define Motion_SetAngleToward_30(args...) Func_02008f02_a(args)
#define SceneWork_SetStepValue_16(args...) Func_02008f20(args)
#define BattleEv_RunWait_15(args...) Func_02008f38(args)
#define BattleFx_SpawnLinked_8(a0, a1, a2) Call3(Func_02008f64, a0, a1, a2)
#define Motion_SetVarCb_6(args...) Func_02008f14(args)
#define Battle_WaitMode0_30(args...) Func_02008e82(args)
#define Object_SetModeById_27(args...) Func_02008f0a(args)
#define Battle_WaitMode0_31(args...) Func_02008e90(args)
#define SceneWork_SetStepValue_17(args...) Func_02008f56(args)
#define BattleEv_RunWait_16(args...) Func_02008f6e(args)
#define Object_SetModeById_28(args...) Func_02008f26(args)
#define BattleFx_SpawnLinked_9(a0, a1, a2) Call3(Func_02008fa2, a0, a1, a2)
#define SceneWork_SetStepValue_18(args...) Func_02008f78(args)
#define BattleEv_RunWait_17(args...) Func_02008f90(args)
#define Motion_CallWaitAnim_3(args...) Func_02008f50(args)
#define BattleFx_SpawnLinked_10(a0, a1, a2) Call3(Func_02008fc2, a0, a1, a2)
#define Motion_ResetPosMode2_16(a0, a1, a2) Call3(Func_02008f36_b, a0, a1, a2)
#define Motion_CommitPos_16(args...) Func_02008f54(args)
#define Motion_SetAngleToward_31(args...) Func_02008f96(args)
#define SceneWork_SetStepValue_19(args...) Func_02008fb4(args)
#define BattleEv_RunWait_18(args...) Func_02008fcc(args)
#define Battle_WaitMode0_32(args...) Func_02008f02_b(args)
#define BattleFx_SpawnLinked_11(a0, a1, a2) Call3(Func_02008ffc, a0, a1, a2)
#define BattleFx_SpawnLinked_12(a0, a1, a2) Call3(Func_02009006_a, a0, a1, a2)
#define BattleFx_SpawnLinked_13(a0, a1, a2) Call3(Func_02009010, a0, a1, a2)
#define BattleFx_SpawnLinked_14(a0, a1, a2) Call3(Func_0200901a, a0, a1, a2)
#define Battle_WaitMode0_33(args...) Func_02008f30(args)
#define Motion_SetAngleToward_32(args...) Func_02008fe2(args)
#define BattleFx_SpawnLinked_15(a0, a1, a2) Call3(Func_02009036_a, a0, a1, a2)
#define Motion_SetAngleToward_33(args...) Func_02008ff8(args)
#define Object_SetModeById_29(args...) Func_02008fd8(args)
#define SceneWork_SetStepValue_20(args...) Func_0200901e_a(args)
#define BattleEv_RunWait_19(args...) Func_02009036_b(args)
#define BattleFx_SpawnLinked_16(a0, a1, a2) Call3(Func_02009060, a0, a1, a2)
#define Battle_WaitMode0_34(args...) Func_02008f76(args)
#define SceneWork_SetStepValue_21(args...) Func_0200903c(args)
#define BattleEv_RunWait_20(args...) Func_02009054(args)
#define Motion_ResetPosMode2_17(a0, a1, a2) Call3(Func_02008fe8, a0, a1, a2)
#define Motion_CommitPos_17(args...) Func_02009006_b(args)
#define Object_SetModeById_30(args...) Func_0200901e_b(args)
#define Motion_SetAngleToward_34(args...) Func_02009050(args)
#define SceneWork_SetStepValue_22(args...) Func_02009070(args)
#define BattleEv_RunWait_21(args...) Func_02009088(args)
#define Motion_SetVarCb_7(args...) Func_02009058(args)
#define Battle_WaitMode0_35(args...) Func_02008fc6(args)
#define Motion_SetAngleToward_35(args...) Func_02009078(args)
#define Battle_WaitMode0_36(args...) Func_02008fd6(args)
#define SceneWork_SetStepValue_23(args...) Func_0200909e(args)
#define BattleEv_RunWait_22(args...) Func_020090b6(args)
#define Object_SetModeById_31(args...) Func_0200908e(args)
#define Battle_WaitMode0_37(args...) Func_02009014(args)
#define SceneWork_SetStepValue_24(args...) Func_020090dc(args)
#define BattleEv_RunWait_23(args...) Func_020090f4_a(args)
#define Motion_SetVarCb_8(args...) Func_020090c4(args)
#define Battle_WaitMode0_38(args...) Func_02009032(args)
#define SceneWork_SetStepValue_25(args...) Func_020090fa(args)
#define BattleEv_RunWait_24(args...) Func_02009112(args)
#define Motion_SetAngleToward_36(args...) Func_020090f4_b(args)
#define Battle_WaitMode0_39(args...) Func_02009052(args)
#define SceneWork_SetStepValue_26(args...) Func_0200911a(args)
#define BattleEv_RunWait_25(args...) Func_02009132(args)
#define Battle_WaitMode0_40(args...) Func_02009068(args)
#define BattleFx_SpawnLinked_17(a0, a1, a2) Call3(Func_02009164_a, a0, a1, a2)
#define Motion_SetAngleToward_37(args...) Func_02009126(args)
#define SceneWork_SetStepValue_27(args...) Func_02009146(args)
#define BattleEv_RunWait_26(args...) Func_0200915e(args)
#define Motion_SetAngleToward_38(args...) Func_02009140(args)
#define SceneWork_SetStepValue_28(args...) Func_02009160(args)
#define BattleEv_RunWait_27(args...) Func_02009178_a(args)
#define Motion_SetAngleToward_39(args...) Func_0200915a(args)
#define Motion_SetAngleToward_40(args...) Func_02009164_b(args)
#define Motion_SetAngleToward_41(args...) Func_0200916e(args)
#define Motion_SetAngleToward_42(args...) Func_02009178_b(args)
#define Battle_WaitMode0_41(args...) Func_020090d6(args)
#define BattleFx_SpawnLinked_18(a0, a1, a2) Call3(Func_020091d2, a0, a1, a2)
#define Motion_SetVarCb_9(args...) Func_02009182(args)
#define Battle_WaitMode0_42(args...) Func_020090f0(args)
#define Motion_SetVarCb_10(args...) Func_02009190(args)
#define Battle_WaitMode0_43(args...) Func_020090fe(args)
#define SceneWork_SetStepValue_29(args...) Func_020091c6(args)
#define BattleEv_RunWait_28(args...) Func_020091de(args)
#define BattleFx_SpawnLinked_19(a0, a1, a2) Call3(Func_02009208, a0, a1, a2)
#define Motion_ResetPosMode2_18(a0, a1, a2) Call3(Func_0200917c, a0, a1, a2)
#define Motion_CommitPos_18(args...) Func_0200919a_a(args)
#define Object_SetModeById_32(args...) Func_020091b2(args)
#define SceneWork_SetStepValue_30(args...) Func_020091fa(args)
#define BattleEv_RunWait_29(args...) Func_02009212(args)
#define Motion_SetAngleToward_43(args...) Func_020091f4_a(args)
#define Battle_WaitMode0_44(args...) Func_02009152(args)
#define Motion_SetVarCb_11(args...) Func_020091f2(args)
#define SceneWork_SetStepValue_31(args...) Func_02009222(args)
#define BattleEv_RunWait_30(args...) Func_0200923a(args)
#define Motion_SetAngleToward_44(args...) Func_0200921c(args)
#define Battle_WaitMode0_45(args...) Func_0200917a(args)
#define Motion_SetAngleToward_45(args...) Func_0200922c(args)
#define Battle_WaitMode0_46(args...) Func_0200918a(args)
#define Motion_SetAngleToward_46(args...) Func_0200923c(args)
#define Battle_WaitMode0_47(args...) Func_0200919a_b(args)
#define Motion_SetAngleToward_47(args...) Func_0200924c(args)
#define Motion_SetAngleToward_48(args...) Func_02009256(args)
#define Motion_SetAngleToward_49(args...) Func_02009260(args)
#define Motion_SetAngleToward_50(args...) Func_0200926a(args)
#define Battle_WaitMode0_48(args...) Func_020091c8(args)
#define Motion_CallWaitAnim_4(args...) Func_02009258(args)
#define Battle_WaitMode0_49(args...) Func_020091d6(args)
#define SceneWork_SetStepValue_32(args...) Func_0200929e(args)
#define BattleEv_RunWait_31(args...) Func_020092b6(args)
#define Motion_SetVarCb_12(args...) Func_02009286(args)
#define Battle_WaitMode0_50(args...) Func_020091f4_b(args)
#define SceneWork_SetStepValue_33(args...) Func_020092bc(args)
#define BattleEv_RunWait_32(args...) Func_020092d4(args)
#define BattleFx_SpawnLinked_20(a0, a1, a2) Call3(Func_02009300, a0, a1, a2)
#define SceneWork_SetStepValue_34(args...) Func_020092d8(args)
#define BattleEv_RunWait_33(args...) Func_020092f0(args)
#define Motion_SetAngleToward_51(args...) Func_020092d2(args)
#define Battle_WaitMode0_51(args...) Func_02009230(args)
#define SceneWork_SetStepValue_35(args...) Func_020092f8(args)
#define Motion_SetSpeed_16(a0, a1) Value2(Func_02009308, a0, a1)
#define Motion_SetAngleToward_52(args...) Func_020092f2(args)
#define Motion_SetAngleToward_53(args...) Func_020092fc(args)
#define Motion_SetAngleToward_54(args...) Func_02009306(args)
#define Motion_SetAngleToward_55(args...) Func_02009310(args)
#define Motion_SetHPosTerrain_9(a0, a1, a2) Call3(Func_020092ee, a0, a1, a2)
#define Motion_SetSpeed_17(a0, a1, a2) Call3(Func_020092b4, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_2(args...) Func_020092ac(args)
#define Audio_PlayCue_3(args...) Func_02009416(args)
#define SceneWork_SetStepValue_36(args...) Func_0200935e(args)
#define BattleEv_RunWait_34(args...) Func_02009376_a(args)
#define Audio_PlayCue_4(args...) Func_02009434(args)
#define SceneWork_SetStepValue_37(args...) Func_0200937c(args)
#define BattleEv_RunWait_35(args...) Func_02009394_a(args)
#define Motion_SetAngleToward_56(args...) Func_02009376_b(args)
#define Motion_SetAngleToward_57(args...) Func_02009380(args)
#define Motion_SetAngleToward_58(args...) Func_0200938a(args)
#define Motion_SetAngleToward_59(args...) Func_02009394_b(args)
#define Motion_SetAngleToward_60(args...) Func_0200939e(args)
#define Motion_SetAngleToward_61(args...) Func_020093a8(args)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_02009422, a0, a1, a2, a3)
#define Motion_SetVarCb_13(args...) Func_020093b2(args)
#define Battle_WaitMode0_52(args...) Func_02009320(args)
#define Audio_PlayCue_5(args...) Func_020094a6_a(args)
#define SceneWork_SetStepValue_38(args...) Func_020093ee_a(args)
#define BattleEv_RunWait_36(args...) Func_02009406(args)
#define Motion_ResetPosMode2_19(a0, a1, a2) Call3(Func_0200939c, a0, a1, a2)
#define Motion_CommitPos_19(args...) Func_020093ba(args)
#define Motion_ResetPosMode2_20(a0, a1, a2) Call3(Func_020093b0, a0, a1, a2)
#define Motion_CamBounds_2(a0, a1, a2, a3) Call4(Func_0200947a, a0, a1, a2, a3)
#define SceneWork_SetStepValue_39(args...) Func_02009430(args)
#define BattleEv_RunWait_37(args...) Func_02009448(args)
#define Motion_CommitPos_20(args...) Func_020093ee_b(args)
#define Motion_ArmCb_20(a0, a1, a2) Call3(Func_0200946a_a, a0, a1, a2)
#define BattleFx_SpawnLinked_21(a0, a1, a2) Call3(Func_02009486, a0, a1, a2)
#define Object_SetModeById_33(args...) Func_0200941e(args)
#define Battle_WaitMode0_53(args...) Func_020093a4(args)
#define SceneWork_SetStepValue_40(args...) Func_0200946a_b(args)
#define BattleEv_RunWait_38(args...) Func_02009482(args)
#define Motion_SetVarCbObj_1(args...) Func_0200945a(args)
#define Motion_CallWaitAnim_5(args...) Func_0200944a(args)
#define SceneWork_SetStepValue_41(args...) Func_02009488(args)
#define BattleEv_RunWait_39(args...) Func_020094a0(args)
#define BattleFx_SpawnLinked_22(a0, a1, a2) Call3(Func_020094cc, a0, a1, a2)
#define Object_SetModeById_34(args...) Func_02009464(args)
#define Battle_WaitMode0_54(args...) Func_020093ea(args)
#define Motion_ArmCb_21(a0, a1, a2) Call3(Func_020094d6, a0, a1, a2)
#define Battle_WaitMode0_55(args...) Func_020093fc(args)
#define SceneWork_SetStepValue_42(args...) Func_020094c2(args)
#define BattleEv_RunWait_40(args...) Func_020094da(args)
#define Motion_SetVarCb_14(args...) Func_020094aa(args)
#define SceneWork_SetStepValue_43(args...) Func_020094d8(args)
#define BattleEv_RunWait_41(args...) Func_020094f0(args)
#define Battle_WaitMode0_56(args...) Func_02009426(args)
#define Motion_CallWaitAnim_6(args...) Func_020094b6(args)
#define Motion_SetAngleToward_62(args...) Func_020094e0(args)
#define Battle_WaitMode0_57(args...) Func_0200943e(args)
#define SceneWork_SetStepValue_44(args...) Func_02009504(args)
#define BattleEv_RunWait_42(args...) Func_0200951c(args)
#define Motion_SetVarCb_15(args...) Func_020094ec(args)
#define BattleFx_SpawnLinked_23(a0, a1, a2) Call3(Func_02009550, a0, a1, a2)
#define SceneWork_SetStepValue_45(args...) Func_02009526(args)
#define BattleEv_RunWait_43(args...) Func_0200953e_a(args)
#define BattleFx_SpawnLinked_24(a0, a1, a2) Call3(Func_02009568, a0, a1, a2)
#define SceneWork_SetStepValue_46(args...) Func_02009540(args)
#define BattleEv_RunWait_44(args...) Func_02009558_a(args)
#define Motion_CallWaitAnim_7(args...) Func_02009518(args)
#define SceneWork_SetStepValue_47(args...) Func_02009558_b(args)
#define BattleEv_RunWait_45(args...) Func_02009570(args)
#define Battle_WaitMode0_58(args...) Func_020094a6_b(args)
#define Battle_WaitMode0_59(a0, a1) Call2(Func_020095a6_a, a0, a1)
#define Battle_WaitMode0_60(args...) Func_020094b4(args)
#define Battle_WaitMode0_61(args...) Func_020095b4(args)
#define Motion_ResetPosMode2_21(a0, a1, a2) Call3(Func_02009520, a0, a1, a2)
#define Motion_CommitPos_21(args...) Func_0200953e_b(args)
#define Motion_SetAngleToward_63(args...) Func_02009580(args)
#define Battle_WaitMode0_62(args...) Func_020094de(args)
#define Motion_SetAngleToward_64(args...) Func_02009590_a(args)
#define SceneWork_SetStepValue_48(args...) Func_020095b0(args)
#define BattleEv_RunWait_46(args...) Func_020095c8_a(args)
#define Motion_SetVarCb_16(args...) Func_02009598(args)
#define SceneWork_SetStepValue_49(args...) Func_020095c8_b(args)
#define BattleEv_RunWait_47(args...) Func_020095e0(args)
#define Battle_WaitMode0_63(args...) Func_02009516(args)
#define Motion_CallWaitAnim_8(args...) Func_020095a6_b(args)
#define Battle_WaitMode0_64(args...) Func_02009524(args)
#define SceneWork_SetStepValue_50(args...) Func_020095ec(args)
#define BattleEv_RunWait_48(args...) Func_02009604(args)
#define Object_SetModeById_35(args...) Func_020095bc(args)
#define BattleFx_SpawnLinked_25(a0, a1, a2) Call3(Func_02009638, a0, a1, a2)
#define SceneWork_SetStepValue_51(args...) Func_02009610_a(args)
#define BattleEv_RunWait_49(args...) Func_02009628(args)
#define BattleFx_SpawnLinked_26(a0, a1, a2) Call3(Func_02009654, a0, a1, a2)
#define SceneWork_SetStepValue_52(args...) Func_0200962c(args)
#define BattleEv_RunWait_50(args...) Func_02009644(args)
#define Object_SetModeById_36(args...) Func_020095fc(args)
#define Battle_WaitMode0_65(args...) Func_02009582(args)
#define Object_SetModeById_37(args...) Func_0200960a(args)
#define Battle_WaitMode0_66(args...) Func_02009590_b(args)
#define Object_LinkPair_4(args...) Func_0200964a(args)
#define Object_SetModeById_38(args...) Func_02009622(args)
#define Object_SetModeById_39(args...) Func_0200962a(args)
#define Object_SetModeById_40(args...) Func_02009632(args)
#define Object_SetModeById_41(args...) Func_0200963a(args)
#define Battle_WaitMode0_67(args...) Func_020095c0(args)
#define BattleFx_SpawnLinked_27(a0, a1, a2) Call3(Func_020096bc, a0, a1, a2)
#define SceneWork_SetStepValue_53(args...) Func_02009694(args)
#define BattleEv_RunWait_51(args...) Func_020096ac(args)
#define BattleFx_SpawnLinked_28(a0, a1, a2) Call3(Func_020096d6, a0, a1, a2)
#define SceneWork_SetStepValue_54(args...) Func_020096ae(args)
#define BattleEv_RunWait_52(args...) Func_020096c6(args)
#define Object_LinkPair_5(args...) Func_020096b0(args)
#define Object_LinkPair_6(args...) Func_020096ba(args)
#define Battle_WaitMode0_68(args...) Func_02009610_b(args)
#define Motion_SetAngleToward_65(args...) Func_020096c2(args)
#define Battle_WaitMode0_69(args...) Func_02009620(args)
#define Motion_SetAngleToward_66(args...) Func_020096d2(args)
#define Motion_SetAngleToward_67(args...) Func_020096dc(args)
#define Motion_SetAngleToward_68(args...) Func_020096e6(args)
#define Motion_SetAngleToward_69(args...) Func_020096f0(args)
#define Battle_WaitMode0_70(args...) Func_0200964e(args)
#define SceneWork_SetStepValue_55(args...) Func_02009716_a(args)
#define BattleEv_RunWait_53(args...) Func_0200972e(args)
#define BattleFx_SpawnLinked_29(a0, a1, a2) Call3(Func_02009758, a0, a1, a2)
#define BattleFx_SpawnLinked_30(a0, a1, a2) Call3(Func_02009762, a0, a1, a2)
#define BattleFx_SpawnLinked_31(a0, a1, a2) Call3(Func_0200976c_a, a0, a1, a2)
#define BattleFx_SpawnLinked_32(a0, a1, a2) Call3(Func_02009776, a0, a1, a2)
#define Battle_WaitMode0_71(args...) Func_0200968c(args)
#define SceneWork_SetStepValue_56(args...) Func_02009754(args)
#define BattleEv_RunWait_54(args...) Func_0200976c_b(args)
#define Motion_SetVarCb_17(args...) Func_0200973c(args)
#define Battle_WaitMode0_72(args...) Func_020096aa(args)
#define SceneWork_SetStepValue_57(args...) Func_02009772(args)
#define BattleEv_RunWait_55(args...) Func_0200978a(args)
#define Motion_SetAngleToward_70(args...) Func_0200976c_c(args)
#define Battle_WaitMode0_73(args...) Func_020096ca(args)
#define SceneWork_SetStepValue_58(args...) Func_02009792(args)
#define BattleEv_RunWait_56(args...) Func_020097aa(args)
#define BattleFx_SpawnLinked_33(a0, a1, a2) Call3(Func_020097d4, a0, a1, a2)
#define BattleFx_SpawnLinked_34(a0, a1, a2) Call3(Func_020097de_a, a0, a1, a2)
#define BattleFx_SpawnLinked_35(a0, a1, a2) Call3(Func_020097e8_a, a0, a1, a2)
#define BattleFx_SpawnLinked_36(a0, a1, a2) Call3(Func_020097f2, a0, a1, a2)
#define Battle_WaitMode0_74(args...) Func_02009708(args)
#define Object_SetModeById_42(args...) Func_02009790(args)
#define Battle_WaitMode0_75(args...) Func_02009716_b(args)
#define SceneWork_SetStepValue_59(args...) Func_020097de_b(args)
#define BattleEv_RunWait_57(args...) Func_02009804(args)
#define BattleFx_SpawnLinked_37(a0, a1, a2) Call3(Func_02009830, a0, a1, a2)
#define SceneWork_SetStepValue_60(args...) Func_02009808(args)
#define BattleEv_RunWait_58(args...) Func_02009820(args)
#define Object_SetModeById_43(args...) Func_020097d8(args)
#define Object_SetModeById_44(args...) Func_020097e0(args)
#define Object_SetModeById_45(args...) Func_020097e8_b(args)
#define Object_SetModeById_46(args...) Func_020097f0(args)
#define Object_SetModeById_47(args...) Func_020097f8(args)
#define Battle_WaitMode0_76(args...) Func_0200977e(args)
#define Object_SetModeById_48(args...) Func_02009806(args)
#define Battle_WaitMode0_77(args...) Func_0200978c(args)
#define SceneWork_SetStepValue_61(args...) Func_02009854(args)
#define BattleEv_RunWait_59(args...) Func_0200986c(args)
#define BattleFx_SpawnLinked_38(a0, a1, a2) Call3(Func_02009898, a0, a1, a2)
#define SceneWork_SetStepValue_62(args...) Func_02009870_a(args)
#define BattleEv_RunWait_60(args...) Func_02009888(args)
#define Motion_SetAngleToward_71(args...) Func_0200986a(args)
#define Battle_WaitMode0_78(args...) Func_020097c8(args)
#define Motion_SetVarCb_18(args...) Func_02009868(args)
#define Battle_WaitMode0_79(args...) Func_020097d6(args)
#define SceneWork_SetStepValue_63(args...) Func_0200989e(args)
#define BattleEv_RunWait_61(args...) Func_020098b6_a(args)
#define Object_SetModeById_49(args...) Func_0200986e(args)
#define Battle_WaitMode0_80(args...) Func_020097f4(args)
#define SceneWork_SetStepValue_64(args...) Func_020098bc_a(args)
#define BattleEv_RunWait_62(args...) Func_020098d4_a(args)
#define Object_SetModeById_50(args...) Func_0200988c(args)
#define SceneWork_SetStepValue_65(args...) Func_020098d4_b(args)
#define BattleEv_RunWait_63(args...) Func_020098ec(args)
#define Motion_SetAngleToward_72(args...) Func_020098ce(args)
#define Motion_CallWaitAnim_9(args...) Func_020098b6_b(args)
#define Battle_WaitMode0_81(args...) Func_02009834(args)
#define SceneWork_SetStepValue_66(args...) Func_020098fc(args)
#define BattleEv_RunWait_64(args...) Func_02009914(args)
#define BattleFx_SpawnLinked_39(a0, a1, a2) Call3(Func_0200993e_a, a0, a1, a2)
#define SceneWork_SetStepValue_67(args...) Func_02009916(args)
#define BattleEv_RunWait_65(args...) Func_0200992e(args)
#define Motion_ArmCb_22(a0, a1, a2) Call3(Func_0200994a, a0, a1, a2)
#define Battle_WaitMode0_82(args...) Func_02009870_b(args)
#define SceneWork_SetStepValue_68(args...) Func_02009938(args)
#define BattleEv_RunWait_66(args...) Func_02009950_a(args)
#define Motion_SetVarCb_19(args...) Func_02009920(args)
#define Battle_WaitMode0_83(args...) Func_0200988e(args)
#define SceneWork_SetStepValue_69(args...) Func_02009956(args)
#define BattleEv_RunWait_67(args...) Func_0200996e(args)
#define Motion_SetAngleToward_73(args...) Func_02009950_b(args)
#define Battle_WaitMode0_84(args...) Func_020098ae(args)
#define Motion_CallWaitAnim_10(args...) Func_0200993e_b(args)
#define Battle_WaitMode0_85(args...) Func_020098bc_b(args)
#define SceneWork_SetStepValue_70(args...) Func_02009984(args)
#define BattleEv_RunWait_68(args...) Func_0200999c(args)
#define BattleFx_SpawnLinked_40(a0, a1, a2) Call3(Func_020099c8, a0, a1, a2)
#define SceneWork_SetStepValue_71(args...) Func_020099a0(args)
#define BattleEv_RunWait_69(args...) Func_020099b8(args)
#define Motion_SetAngleToward_74(args...) Func_0200999a(args)
#define Battle_WaitMode0_86(args...) Func_020098f8(args)
#define SceneWork_SetStepValue_72(args...) Func_020099c0(args)
#define BattleEv_RunWait_70(args...) Func_020099d8(args)
#define BattleFx_SpawnLinked_41(a0, a1, a2) Call3(Func_02009a04, a0, a1, a2)
#define SceneWork_SetStepValue_73(args...) Func_020099dc(args)
#define BattleEv_RunWait_71(args...) Func_020099f4_a(args)
#define Motion_CallWaitAnim_11(args...) Func_020099b4(args)
#define SceneWork_SetStepValue_74(args...) Func_020099f4_b(args)
#define BattleEv_RunWait_72(args...) Func_02009a0c(args)
#define Motion_SetAngleToward_75(args...) Func_020099ee(args)
#define Battle_WaitMode0_87(args...) Func_0200994c(args)
#define Object_SetModeById_51(args...) Func_020099d4(args)
#define Battle_WaitMode0_88(args...) Func_0200995a(args)
#define SceneWork_SetStepValue_75(args...) Func_02009a22(args)
#define Motion_SetSpeed_18(a0, a1) Value2(Func_02009a32, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_3(args...) Func_02009992(args)
#define SceneWork_SetStepValue_76(args...) Func_02009a3e(args)
#define BattleEv_RunWait_73(args...) Func_02009a56(args)
#define Battle_WaitMode0_89(args...) Func_0200998c(args)
#define Object_SetModeById_52(args...) Func_02009a14(args)
#define Object_SetModeById_53(args...) Func_02009a1c(args)
#define Object_SetModeById_54(args...) Func_02009a24(args)
#define Battle_WaitMode0_90(args...) Func_020099aa(args)
#define Object_LinkPair_7(args...) Func_02009a64(args)
#define Object_SetModeById_55(args...) Func_02009a3c(args)
#define Object_SetModeById_56(args...) Func_02009a44(args)
#define Battle_WaitMode0_91(args...) Func_020099ca(args)
#define Object_SetModeById_57(args...) Func_02009a52(args)
#define Scene_GetRecord_2(args...) Func_02009a00(args)
#define Motion_ResetAndSetPosition_1(args...) Func_02009a3a(args)
#define Motion_CommitPos_22(args...) Func_02009a60_a(args)
#define Motion_SetHPosTerrain_10(args...) Func_02009a72(args)
#define Object_SetModeById_58(args...) Func_02009a82(args)
#define Scene_GetRecord_3(args...) Func_02009a30(args)
#define Motion_ResetAndSetPosition_2(args...) Func_02009a6a(args)
#define Motion_CommitPos_23(args...) Func_02009a90(args)
#define Motion_SetHPosTerrain_11(args...) Func_02009aa2(args)
#define Object_SetModeById_59(args...) Func_02009ab2(args)
#define Scene_GetRecord_4(args...) Func_02009a60_b(args)
#define Motion_ResetAndSetPosition_3(args...) Func_02009a9a_a(args)
#define Motion_CommitPos_24(args...) Func_02009ac0(args)
#define Motion_SetHPosTerrain_12(args...) Func_02009ad2(args)
#define Battle_WaitMode0_92(args...) Func_02009a60_c(args)
#define Motion_SetSpeed_19(a0, a1, a2) Call3(Func_02009a9a_b, a0, a1, a2)
#define Motion_ResetPosMode2_22(a0, a1, a2) Call3(Func_02009ace, a0, a1, a2)
#define Motion_CommitPos_25(args...) Func_02009aec(args)
#define Object_SetModeById_60(args...) Func_02009b04(args)
#define Motion_ArmCb_23(a0, a1, a2) Call3(Func_02009b70_a, a0, a1, a2)
#define Motion_SetAngleToward_76(args...) Func_02009b42(args)
#define Object_SetModeById_61(args...) Func_02009b22(args)
#define Battle_WaitMode0_93(args...) Func_02009aa8(args)
#define Motion_SetAngleToward_77(args...) Func_02009b5a(args)
#define Battle_WaitMode0_94(args...) Func_02009ab8(args)
#define SceneWork_SetStepValue_77(args...) Func_02009b80(args)
#define BattleEv_RunWait_74(args...) Func_02009b98_a(args)
#define Motion_SetVarCbObj_2(args...) Func_02009b70_b(args)
#define SceneWork_SetStepValue_78(args...) Func_02009b98_b(args)
#define BattleEv_RunWait_75(args...) Func_02009bb0(args)
#define Object_SetModeById_62(args...) Func_02009b68(args)
#define Battle_WaitMode0_95(args...) Func_02009aee(args)
#define Motion_SetSpeed_20(a0, a1, a2) Call3(Func_02009b2c, a0, a1, a2)
#define Motion_SetSpeed_21(a0, a1, a2) Call3(Func_02009b3a, a0, a1, a2)
#define Motion_SetPosReset_1(args...) Func_02009c12(args)
#define Motion_ResetPosMode2_23(a0, a1, a2) Call3(Func_02009b76, a0, a1, a2)
#define Battle_WaitMode0_96(args...) Func_02009b24(args)
#define Motion_SetActionVariant_2(args...) Func_02009c14(args)
#define Motion_SetActionVariant_3(args...) Func_02009c1c(args)
#define Motion_ResetPosMode2_24(a0, a1, a2) Call3(Func_02009b98_c, a0, a1, a2)
#define Motion_CommitPos_26(args...) Func_02009bb6(args)
#define Motion_SetAngleToward_78(args...) Func_02009bf8(args)
#define Motion_CommitPos_27(args...) Func_02009bc6(args)
#define Motion_SetAngleToward_79(args...) Func_02009c08(args)
#define Motion_ArmCb_24(a0, a1, a2) Call3(Func_02009c4c_a, a0, a1, a2)
#define Object_SetModeById_63(args...) Func_02009bf4(args)
#define Battle_WaitMode0_97(args...) Func_02009b7a(args)
#define Object_SetModeById_64(args...) Func_02009c02(args)
#define Battle_WaitMode0_98(args...) Func_02009b88(args)
#define Object_SetModeById_65(args...) Func_02009c10(args)
#define Battle_WaitMode0_99(args...) Func_02009b96(args)
#define Motion_ResetPosMode2_25(a0, a1, a2) Call3(Func_02009bfa, a0, a1, a2)
#define Motion_ResetPosMode2_26(a0, a1, a2) Call3(Func_02009c06, a0, a1, a2)
#define Motion_CommitPos_28(args...) Func_02009c24(args)
#define Motion_ResetPosMode2_27(a0, a1, a2) Call3(Func_02009c18, a0, a1, a2)
#define Motion_CommitPos_29(args...) Func_02009c36(args)
#define Motion_ResetPosMode2_28(a0, a1, a2) Call3(Func_02009c2c, a0, a1, a2)
#define Motion_CommitPos_30(args...) Func_02009c4a(args)
#define Motion_ResetPosMode2_29(a0, a1, a2) Call3(Func_02009c40, a0, a1, a2)
#define Motion_CommitPos_31(args...) Func_02009c5e(args)
#define Motion_ResetPosMode2_30(a0, a1, a2) Call3(Func_02009c54, a0, a1, a2)
#define Motion_CommitPos_32(args...) Func_02009c72(args)
#define Motion_ResetPosMode2_31(a0, a1, a2) Call3(Func_02009c68, a0, a1, a2)
#define Battle_WaitMode0_100(args...) Func_02009c16(args)
#define Audio_PlayCue_6(args...) Func_02009d9c(args)
#define Battle_WaitMode0_101(args...) Func_02009c46(args)
#define Battle_WaitMode0_102(args...) Func_02009c4c_b(args)
#define Battle_SchedShoulder_1(args...) Func_02009c66(args)
u8 *Func_020086d6();
u8 *Func_02009a00();
u8 *Func_02009a60_a();
u8 *Func_02009a30();
u8 *Func_02009a60_b();
u8 *Func_02009a60_c();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* Long fixed sequence of setup, positioning, and per-actor animation calls
 * against actor slots 0-3, 11-15, driven by three script line tables, with
 * two two-way branches on the outcome of a query call. Ends by writing the
 * scene phase word and issuing a final batch of calls. */
void Scene_RunMainScriptSequence(void)
{
    u32 i;
    u8 *record;
    s32 script_a;
    s32 script_b;
    s32 script_c;

    GameFlag_Set_1(0x301);
    GameFlag_Set_2(0x941);
    Call6(Func_02008646, 16, 75, 7, 4, 26, 55);
    Func_02003d0c(4);
    Battle_Reset_1();
    record = Scene_GetRecord_1(12);
    Func_02008674_b(record, 1);
    Motion_SetHPosTerrain_1(0, 0x1c80000, 0xb80000);
    Motion_ArmCb_1(0, 0x4000, 0);
    Motion_SetHPosTerrain_2(1, 0x1b80000, 0xc00000);
    Motion_ArmCb_2(1, 0x4000, 0);
    Motion_SetHPosTerrain_3(3, 0x1e80000, 0xb80000);
    Motion_ArmCb_3(3, 0x4000, 0);
    Motion_SetHPosTerrain_4(2, 0x1d80000, 0xb80000);
    Motion_ArmCb_4(2, 0x4000, 0);
    Motion_SetHPosTerrain_5(12, 0x2080000, 0xe00000);
    Motion_ArmCb_5(12, 0xb000, 0);
    Motion_SetHPosTerrain_6(11, 0x1c00000, 0xed0000);
    Motion_ArmCb_6(11, 0x8000, 0);
    Motion_SetHPosTerrain_7(15, 0x1c00000, 0xee0000);
    Motion_SetActionVariant_1(15, 3);
    Object_SetModeById_1(11, 3);
    Motion_SetHPosTerrain_8(13, 0x1ca0000, 0xf30000);
    Motion_ArmCb_7(13, 0x4000, 0);
    Object_SetModeById_2(13, 5);
    Battle_WaitMode0_1();
    Battle_WaitMode0_2(120);
    /* Script line bases are overlay data symbols: an integer base would be
     * constant-propagated into every offset instead of staying in r5. */
    script_a = (s32)Data_00002481;
    SceneWork_SetStepValue_1(script_a);
    BattleEv_RunWait_1(13, 0);
    BattleFx_SpawnLinked_1(1, 0x101, 60);
    SceneWork_SetStepValue_2((script_a + 1));
    BattleEv_RunWait_2(1, 0);
    BattleFx_SpawnLinked_2(0, 0x101, 1);
    BattleFx_SpawnLinked_3(2, 0x101, 1);
    BattleFx_SpawnLinked_4(3, 0x101, 1);
    Battle_WaitMode0_3(60);
    Motion_SetAngleToward_1(12, 13, 0);
    Battle_WaitMode0_4(60);
    SceneWork_SetStepValue_3((script_a + 2));
    BattleEv_RunWait_3(12, 0);
    Motion_SetAngleToward_2(0, 12, 0);
    Motion_SetAngleToward_3(1, 12, 0);
    Motion_SetAngleToward_4(2, 12, 0);
    Motion_SetAngleToward_5(3, 12, 0);
    Battle_WaitMode0_5(60);
    Motion_ResetPosMode2_1(12, 0x200, 232);
    Object_SetModeById_3(12, 4);
    Battle_WaitMode0_6(60);
    SceneWork_SetStepValue_4((script_a + 3));
    BattleEv_RunWait_4(12, 0);
    Battle_WaitMode0_7(15);
    Motion_SetVarCb_1(13, 2);
    SceneWork_SetStepValue_5((script_a + 4));
    BattleEv_RunWait_5(13, 0);
    Motion_SetAngleToward_6(0, 13, 0);
    Motion_SetAngleToward_7(2, 13, 0);
    Motion_SetAngleToward_8(3, 13, 0);
    Motion_SetAngleToward_9(1, 13, 0);
    Motion_SetVarCb_2(2, 1);
    SceneWork_SetStepValue_6((script_a + 5));
    BattleEv_RunWait_6(2, 0);
    BattleFx_SpawnLinked_5(3, 0x105, 0);
    Object_SetModeById_4(3, 4);
    Battle_WaitMode0_8(80);
    Object_SetModeById_5(3, 1);
    SceneWork_SetStepValue_7((script_a + 6));
    BattleEv_RunWait_7(3, 0);
    BattleFx_SpawnLinked_6(12, 0x102, 65);
    Motion_SetVarCb_3(12, 2);
    Battle_WaitMode0_9(100);
    SceneWork_SetStepValue_8((script_a + 7));
    BattleEv_RunWait_8(13, 0);
    Motion_SetAngleToward_10(1, 0, 0);
    Battle_WaitMode0_10(10);
    SceneWork_SetStepValue_9((script_a + 8));
    BattleEv_RunWait_9(1, 0);
    Motion_SetVarCb_4(2, 1);
    Battle_WaitMode0_11(30);
    SceneWork_SetStepValue_10((script_a + 9));
    BattleEv_RunWait_10(2, 0);
    Motion_SetAngleToward_11(3, 0, 0);
    SceneWork_SetStepValue_11((script_a + 10));
    Motion_SetSpeed_1(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Object_SetModeById_6(2, 3);
        Object_SetModeById_7(1, 3);
        Object_SetModeById_8(3, 3);
        Motion_SetAngleToward_12(12, 13, 0);
        Battle_WaitMode0_12(60);
    } else {
        SceneWork_SetStepValue_12((script_a + 11));
        BattleEv_RunWait_11(3, 0);
        Motion_SetAngleToward_13(0, 12, 0);
        Motion_SetAngleToward_14(1, 12, 0);
        Motion_SetAngleToward_15(2, 12, 0);
        Motion_SetAngleToward_16(3, 12, 0);
        Battle_WaitMode0_13(20);
        Motion_SetAngleToward_17(12, 0, 0);
        Battle_WaitMode0_14(60);
        Motion_SetAngleToward_18(12, 13, 0);
        Battle_WaitMode0_15(80);
        Motion_CallWaitAnim_1(12, 3);
        Battle_WaitMode0_16(30);
        Motion_SetAngleToward_19(12, 0, 0);
        SceneWork_SetStepValue_13((script_a + 12));
        BattleEv_RunWait_12(12, 0);
        Battle_WaitMode0_17(60);
        Motion_SetAngleToward_20(2, 12, 0);
        Motion_SetAngleToward_21(1, 12, 0);
        Motion_SetAngleToward_22(3, 12, 0);
        Motion_SetAngleToward_23(0, 12, 0);
        Object_SetModeById_9(2, 3);
        Object_SetModeById_10(1, 3);
        Object_SetModeById_11(3, 3);
        Object_SetModeById_12(0, 3);
        Battle_WaitMode0_18(80);
        Motion_SetAngleToward_24(12, 13, 0);
        Motion_SetAngleToward_25(2, 13, 0);
        Motion_SetAngleToward_26(1, 13, 0);
        Motion_SetAngleToward_27(3, 13, 0);
        Motion_SetAngleToward_28(0, 13, 0);
    }
    Motion_SetSpeed_2(0, 0x8000, 0x4000);
    Motion_SetSpeed_3(1, 0x8000, 0x4000);
    Motion_SetSpeed_4(3, 0x8000, 0x4000);
    Motion_SetSpeed_5(2, 0x8000, 0x4000);
    Motion_ResetPosMode2_2(1, 0x1a0, 216);
    Motion_CommitPos_1(1);
    Motion_ResetPosMode2_3(1, 0x1a0, 248);
    Motion_CommitPos_2(1);
    Motion_ResetPosMode2_4(1, 0x1b8, 248);
    Motion_CommitPos_3(1);
    Motion_ArmCb_8(1, 0xc000, 0);
    Motion_ResetPosMode2_5(0, 0x1b8, 216);
    Motion_CommitPos_4(0);
    Motion_ArmCb_9(0, 0, 0);
    Motion_ResetPosMode2_6(3, 0x1e8, 248);
    Motion_CommitPos_5(3);
    Motion_ResetPosMode2_7(3, 0x1c8, 248);
    Motion_CommitPos_6(3);
    Motion_ArmCb_10(3, 0xc000, 0);
    Motion_ResetPosMode2_8(2, 0x1c8, 216);
    Motion_CommitPos_7(2);
    Motion_ArmCb_11(2, 0x8000, 0);
    Object_LinkPair_1(1, 0, 0);
    Object_LinkPair_2(3, 2, 0);
    Battle_WaitMode0_19(60);
    Motion_CallWaitAnim_2(0, 3);
    Object_SetModeById_13(2, 3);
    Object_SetModeById_14(1, 3);
    Object_SetModeById_15(3, 3);
    Battle_WaitMode0_20(100);
    Audio_PlayCue_1(226);
    Object_SetModeById_16(13, 7);
    Motion_SetSpeed_6(0, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_1(0, -24, 0);
    Motion_SetSpeed_7(1, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_2(1, -24, 0);
    Motion_SetSpeed_8(3, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_3(3, -24, 0);
    Motion_SetSpeed_9(2, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_4(2, -24, 0);
    Motion_SetSpeed_10(11, 0x1999, 0xccc);
    Motion_SetSpeed_11(15, 0x1999, 0xccc);
    Motion_OffsetPositionAndResetMotion_5(11, -24, 0);
    Motion_OffsetPositionAndResetMotion_6(15, -24, 0);
    Motion_CommitPos_8(0);
    Audio_PlayCue_2(0x120);
    Battle_WaitMode0_21(60);
    Motion_ArmCb_12(0, 0x4000, 0);
    Motion_ArmCb_13(1, 0xc000, 0);
    Motion_ArmCb_14(2, 0x4000, 0);
    Motion_ArmCb_15(3, 0xc000, 0);
    Battle_WaitMode0_22(60);
    Object_SetModeById_17(0, 3);
    Object_SetModeById_18(1, 3);
    Object_SetModeById_19(2, 3);
    Object_SetModeById_20(3, 3);
    Battle_WaitMode0_23(100);
    Motion_SetSpeed_12(0, 0xb333, 0x5999);
    Motion_SetSpeed_13(1, 0xb333, 0x5999);
    Motion_SetSpeed_14(2, 0xb333, 0x5999);
    Motion_SetSpeed_15(3, 0xb333, 0x5999);
    Motion_ResetPosMode2_9(0, 0x1c8, 184);
    Motion_CommitPos_9(0);
    Object_SetModeById_21(0, 1);
    Motion_ArmCb_16(0, 0x4000, 0);
    Motion_ResetPosMode2_10(1, 0x1d0, 0x100);
    Motion_CommitPos_10(1);
    Motion_ResetPosMode2_11(1, 0x1e0, 248);
    Motion_CommitPos_11(1);
    Motion_ResetPosMode2_12(1, 0x1b8, 192);
    Motion_CommitPos_12(1);
    Object_SetModeById_22(1, 1);
    Motion_ArmCb_17(1, 0x4000, 0);
    Motion_ResetPosMode2_13(3, 0x1e8, 248);
    Motion_CommitPos_13(3);
    Motion_ResetPosMode2_14(3, 0x1e8, 184);
    Motion_ResetPosMode2_15(2, 0x1d8, 184);
    Motion_CommitPos_14(2);
    Object_SetModeById_23(2, 1);
    Motion_ArmCb_18(2, 0x4000, 0);
    Motion_CommitPos_15(3);
    Object_SetModeById_24(3, 1);
    Motion_ArmCb_19(3, 0x4000, 0);
    Battle_WaitMode0_24(30);
    Battle_WaitMode0_25(60);
    script_b = (s32)Data_0000248e;
    SceneWork_SetStepValue_14(script_b);
    BattleEv_RunWait_13(1, 0);
    Object_SetModeById_25(13, 6);
    Battle_WaitMode0_26(120);
    Motion_SetVarCb_5(13, 2);
    Battle_WaitMode0_27(60);
    Object_SetModeById_26(13, 7);
    SceneWork_SetStepValue_15((script_b + 1));
    BattleEv_RunWait_14(13, 0);
    Battle_WaitMode0_28(20);
    Object_LinkPair_3(3, 2, 0);
    Battle_WaitMode0_29(10);
    BattleFx_SpawnLinked_7(3, 0x101, 80);
    Motion_SetAngleToward_29(3, 13, 0);
    Motion_SetAngleToward_30(2, 13, 0);
    SceneWork_SetStepValue_16((script_b + 2));
    BattleEv_RunWait_15(3, 0);
    BattleFx_SpawnLinked_8(13, 0x102, 70);
    Motion_SetVarCb_6(13, 2);
    Battle_WaitMode0_30(60);
    Object_SetModeById_27(13, 5);
    Battle_WaitMode0_31(70);
    SceneWork_SetStepValue_17((script_b + 3));
    BattleEv_RunWait_16(13, 0);
    Object_SetModeById_28(13, 7);
    BattleFx_SpawnLinked_9(2, 0x108, 40);
    SceneWork_SetStepValue_18((script_b + 4));
    BattleEv_RunWait_17(2, 0);
    Motion_CallWaitAnim_3(12, 3);
    BattleFx_SpawnLinked_10(1, 0x103, 60);
    Motion_ResetPosMode2_16(1, 0x1b8, 208);
    Motion_CommitPos_16(1);
    Motion_SetAngleToward_31(1, 0, 0);
    SceneWork_SetStepValue_19((script_b + 5));
    BattleEv_RunWait_18(1, 0);
    Battle_WaitMode0_32(20);
    BattleFx_SpawnLinked_11(0, 0x101, 0);
    BattleFx_SpawnLinked_12(2, 0x101, 0);
    BattleFx_SpawnLinked_13(3, 0x101, 0);
    BattleFx_SpawnLinked_14(12, 0x101, 0);
    Battle_WaitMode0_33(70);
    Motion_SetAngleToward_32(1, 2, 0);
    BattleFx_SpawnLinked_15(1, 0x102, 75);
    Motion_SetAngleToward_33(1, 0, 0);
    Object_SetModeById_29(1, 4);
    SceneWork_SetStepValue_20((script_b + 6));
    BattleEv_RunWait_19(1, 0);
    BattleFx_SpawnLinked_16(12, 0x101, 0);
    Battle_WaitMode0_34(60);
    SceneWork_SetStepValue_21((script_b + 7));
    BattleEv_RunWait_20(12, 0);
    Motion_ResetPosMode2_17(1, 0x1c0, 208);
    Motion_CommitPos_17(1);
    Object_SetModeById_30(1, 1);
    Motion_SetAngleToward_34(1, 12, 0);
    SceneWork_SetStepValue_22((script_b + 8));
    BattleEv_RunWait_21(1, 0);
    Motion_SetVarCb_7(12, 1);
    Battle_WaitMode0_35(60);
    Motion_SetAngleToward_35(1, 12, 0);
    Battle_WaitMode0_36(60);
    SceneWork_SetStepValue_23((script_b + 9));
    BattleEv_RunWait_22(12, 0);
    Object_SetModeById_31(1, 4);
    Battle_WaitMode0_37(60);
    SceneWork_SetStepValue_24((script_b + 10));
    BattleEv_RunWait_23(1, 0);
    Motion_SetVarCb_8(2, 1);
    Battle_WaitMode0_38(30);
    SceneWork_SetStepValue_25((script_b + 11));
    BattleEv_RunWait_24(2, 0);
    Motion_SetAngleToward_36(1, 3, 0);
    Battle_WaitMode0_39(20);
    SceneWork_SetStepValue_26((script_b + 12));
    BattleEv_RunWait_25(1, 0);
    Battle_WaitMode0_40(30);
    BattleFx_SpawnLinked_17(3, 0x100, 80);
    Motion_SetAngleToward_37(3, 1, 0);
    SceneWork_SetStepValue_27((script_b + 13));
    BattleEv_RunWait_26(3, 0);
    Motion_SetAngleToward_38(3, 13, 0);
    SceneWork_SetStepValue_28((script_b + 14));
    BattleEv_RunWait_27(3, 0);
    Motion_SetAngleToward_39(0, 13, 0);
    Motion_SetAngleToward_40(2, 13, 0);
    Motion_SetAngleToward_41(1, 13, 0);
    Motion_SetAngleToward_42(3, 13, 0);
    Battle_WaitMode0_41(120);
    BattleFx_SpawnLinked_18(13, 0x102, 30);
    Motion_SetVarCb_9(13, 1);
    Battle_WaitMode0_42(120);
    Motion_SetVarCb_10(12, 1);
    Battle_WaitMode0_43(60);
    SceneWork_SetStepValue_29((script_b + 15));
    BattleEv_RunWait_28(12, 0);
    BattleFx_SpawnLinked_19(1, 0x107, 110);
    Motion_ResetPosMode2_18(1, 0x1c8, 212);
    Motion_CommitPos_18(1);
    Object_SetModeById_32(1, 1);
    SceneWork_SetStepValue_30((script_b + 16));
    BattleEv_RunWait_29(1, 0);
    Motion_SetAngleToward_43(2, 1, 0);
    Battle_WaitMode0_44(60);
    Motion_SetVarCb_11(2, 1);
    SceneWork_SetStepValue_31((script_b + 17));
    BattleEv_RunWait_30(2, 0);
    Motion_SetAngleToward_44(1, 13, 0);
    Battle_WaitMode0_45(80);
    Motion_SetAngleToward_45(1, 2, 0);
    Battle_WaitMode0_46(60);
    Motion_SetAngleToward_46(1, 13, 0);
    Battle_WaitMode0_47(30);
    Motion_SetAngleToward_47(0, 13, 0);
    Motion_SetAngleToward_48(3, 13, 0);
    Motion_SetAngleToward_49(13, 13, 0);
    Motion_SetAngleToward_50(12, 13, 0);
    Battle_WaitMode0_48(80);
    Motion_CallWaitAnim_4(1, 3);
    Battle_WaitMode0_49(30);
    SceneWork_SetStepValue_32((script_b + 18));
    BattleEv_RunWait_31(1, 0);
    Motion_SetVarCb_12(13, 2);
    Battle_WaitMode0_50(70);
    SceneWork_SetStepValue_33((script_b + 19));
    BattleEv_RunWait_32(13, 0);
    BattleFx_SpawnLinked_20(3, 0x100, 60);
    SceneWork_SetStepValue_34((script_b + 20));
    BattleEv_RunWait_33(3, 0);
    Motion_SetAngleToward_51(2, 0, 0);
    Battle_WaitMode0_51(80);
    SceneWork_SetStepValue_35((script_b + 21));
    Motion_SetSpeed_16(2, 0);
    Motion_SetAngleToward_52(1, 0, 0);
    Motion_SetAngleToward_53(3, 0, 0);
    Motion_SetAngleToward_54(2, 0, 0);
    Motion_SetAngleToward_55(12, 0, 0);
    Motion_SetHPosTerrain_9(14, 0x1c80000, 0x1300000);
    Motion_SetSpeed_17(14, 0x8000, 0x4000);
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        Audio_PlayCue_3(19);
        SceneWork_SetStepValue_36((script_b + 22));
        BattleEv_RunWait_34(14, 0);
    } else {
        Audio_PlayCue_4(19);
        SceneWork_SetStepValue_37((script_b + 23));
        BattleEv_RunWait_35(14, 0);
    }
    Motion_SetAngleToward_56(0, 14, 0);
    Motion_SetAngleToward_57(1, 14, 0);
    Motion_SetAngleToward_58(3, 14, 0);
    Motion_SetAngleToward_59(2, 14, 0);
    Motion_SetAngleToward_60(12, 14, 0);
    Motion_SetAngleToward_61(13, 14, 0);
    Motion_CamBounds_1(0x1c80000, -1, 0xf00000, 1);
    Motion_SetVarCb_13(13, 1);
    Battle_WaitMode0_52(60);
    Audio_PlayCue_5(8);
    script_c = (s32)Data_000024a6;
    SceneWork_SetStepValue_38(script_c);
    BattleEv_RunWait_36(13, 0);
    Motion_ResetPosMode2_19(14, 0x1c8, 0x118);
    Motion_CommitPos_19(14);
    Motion_ResetPosMode2_20(14, 0x1b8, 0x100);
    Motion_CamBounds_2(0x1c80000, -1, 0xe00000, 1);
    SceneWork_SetStepValue_39((script_c + 1));
    BattleEv_RunWait_37(14, 0);
    Motion_CommitPos_20(14);
    Motion_ArmCb_20(14, 0xd000, 0);
    BattleFx_SpawnLinked_21(13, 0x102, 80);
    Object_SetModeById_33(14, 4);
    Battle_WaitMode0_53(89);
    SceneWork_SetStepValue_40((script_c + 2));
    BattleEv_RunWait_38(14, 0);
    Motion_SetVarCbObj_1(13, 2);
    Motion_CallWaitAnim_5(14, 3);
    SceneWork_SetStepValue_41((script_c + 3));
    BattleEv_RunWait_39(14, 0);
    BattleFx_SpawnLinked_22(13, 0x100, 80);
    Object_SetModeById_34(14, 4);
    Battle_WaitMode0_54(80);
    Motion_ArmCb_21(14, 0x3000, 0);
    Battle_WaitMode0_55(20);
    SceneWork_SetStepValue_42((script_c + 4));
    BattleEv_RunWait_40(14, 0);
    Motion_SetVarCb_14(13, 1);
    SceneWork_SetStepValue_43((script_c + 5));
    BattleEv_RunWait_41(13, 0);
    Battle_WaitMode0_56(30);
    Motion_CallWaitAnim_6(14, 3);
    Motion_SetAngleToward_62(14, 13, 0);
    Battle_WaitMode0_57(20);
    SceneWork_SetStepValue_44((script_c + 6));
    BattleEv_RunWait_42(14, 0);
    Motion_SetVarCb_15(13, 1);
    BattleFx_SpawnLinked_23(13, 0x102, 80);
    SceneWork_SetStepValue_45((script_c + 7));
    BattleEv_RunWait_43(13, 0);
    BattleFx_SpawnLinked_24(14, 0x103, 60);
    SceneWork_SetStepValue_46((script_c + 8));
    BattleEv_RunWait_44(14, 0);
    Motion_CallWaitAnim_7(14, 3);
    SceneWork_SetStepValue_47((script_c + 9));
    BattleEv_RunWait_45(14, 0);
    Battle_WaitMode0_58(20);
    Battle_WaitMode0_59(13, 0x101);
    Battle_WaitMode0_60(80);
    Battle_WaitMode0_61(13, 0);
    Motion_ResetPosMode2_21(14, 0x1f0, 240);
    Motion_CommitPos_21(14);
    Motion_SetAngleToward_63(14, 12, 0);
    Battle_WaitMode0_62(20);
    Motion_SetAngleToward_64(12, 14, 0);
    SceneWork_SetStepValue_48((script_c + 10));
    BattleEv_RunWait_46(14, 0);
    Motion_SetVarCb_16(12, 1);
    SceneWork_SetStepValue_49((script_c + 11));
    BattleEv_RunWait_47(12, 0);
    Battle_WaitMode0_63(40);
    Motion_CallWaitAnim_8(14, 3);
    Battle_WaitMode0_64(20);
    SceneWork_SetStepValue_50((script_c + 12));
    BattleEv_RunWait_48(14, 0);
    Object_SetModeById_35(14, 3);
    BattleFx_SpawnLinked_25(12, 0x102, 60);
    SceneWork_SetStepValue_51((script_c + 13));
    BattleEv_RunWait_49(12, 0);
    BattleFx_SpawnLinked_26(14, 0x100, 70);
    SceneWork_SetStepValue_52((script_c + 14));
    BattleEv_RunWait_50(14, 0);
    Object_SetModeById_36(12, 3);
    Battle_WaitMode0_65(140);
    Object_SetModeById_37(14, 3);
    Battle_WaitMode0_66(120);
    Object_LinkPair_4(0, 14, 0);
    Object_SetModeById_38(0, 3);
    Object_SetModeById_39(2, 3);
    Object_SetModeById_40(1, 3);
    Object_SetModeById_41(3, 3);
    Battle_WaitMode0_67(120);
    BattleFx_SpawnLinked_27(14, 0x108, 180);
    SceneWork_SetStepValue_53((script_c + 15));
    BattleEv_RunWait_51(14, 0);
    BattleFx_SpawnLinked_28(12, 0x101, 80);
    SceneWork_SetStepValue_54((script_c + 16));
    BattleEv_RunWait_52(12, 0);
    Object_LinkPair_5(0, 1, 0);
    Object_LinkPair_6(3, 2, 0);
    Battle_WaitMode0_68(80);
    Motion_SetAngleToward_65(14, 0, 0);
    Battle_WaitMode0_69(60);
    Motion_SetAngleToward_66(0, 14, 0);
    Motion_SetAngleToward_67(1, 14, 0);
    Motion_SetAngleToward_68(2, 14, 0);
    Motion_SetAngleToward_69(3, 14, 0);
    Battle_WaitMode0_70(60);
    SceneWork_SetStepValue_55((script_c + 17));
    BattleEv_RunWait_53(14, 0);
    BattleFx_SpawnLinked_29(0, 0x101, 0);
    BattleFx_SpawnLinked_30(1, 0x101, 0);
    BattleFx_SpawnLinked_31(2, 0x101, 0);
    BattleFx_SpawnLinked_32(3, 0x101, 0);
    Battle_WaitMode0_71(100);
    SceneWork_SetStepValue_56((script_c + 18));
    BattleEv_RunWait_54(1, 0);
    Motion_SetVarCb_17(2, 1);
    Battle_WaitMode0_72(60);
    SceneWork_SetStepValue_57((script_c + 19));
    BattleEv_RunWait_55(2, 0);
    Motion_SetAngleToward_70(14, 2, 0);
    Battle_WaitMode0_73(20);
    SceneWork_SetStepValue_58((script_c + 20));
    BattleEv_RunWait_56(14, 0);
    BattleFx_SpawnLinked_33(0, 0x101, 0);
    BattleFx_SpawnLinked_34(1, 0x101, 0);
    BattleFx_SpawnLinked_35(2, 0x101, 0);
    BattleFx_SpawnLinked_36(3, 0x101, 0);
    Battle_WaitMode0_74(120);
    Object_SetModeById_42(14, 4);
    Battle_WaitMode0_75(120);
    SceneWork_SetStepValue_59((script_c + 21));
    BattleEv_RunWait_57(14, 0);
    BattleFx_SpawnLinked_37(14, 0x102, 90);
    SceneWork_SetStepValue_60((script_c + 22));
    BattleEv_RunWait_58(14, 0);
    Object_SetModeById_43(0, 3);
    Object_SetModeById_44(2, 3);
    Object_SetModeById_45(3, 3);
    Object_SetModeById_46(1, 3);
    Object_SetModeById_47(12, 3);
    Battle_WaitMode0_76(80);
    Object_SetModeById_48(14, 4);
    Battle_WaitMode0_77(120);
    SceneWork_SetStepValue_61((script_c + 23));
    BattleEv_RunWait_59(14, 0);
    BattleFx_SpawnLinked_38(12, 0x100, 60);
    SceneWork_SetStepValue_62((script_c + 24));
    BattleEv_RunWait_60(12, 0);
    Motion_SetAngleToward_71(14, 12, 0);
    Battle_WaitMode0_78(20);
    Motion_SetVarCb_18(14, 1);
    Battle_WaitMode0_79(50);
    SceneWork_SetStepValue_63((script_c + 25));
    BattleEv_RunWait_61(14, 0);
    Object_SetModeById_49(1, 3);
    Battle_WaitMode0_80(70);
    SceneWork_SetStepValue_64((script_c + 26));
    BattleEv_RunWait_62(1, 0);
    Object_SetModeById_50(3, 3);
    SceneWork_SetStepValue_65((script_c + 27));
    BattleEv_RunWait_63(3, 0);
    Motion_SetAngleToward_72(14, 0, 0);
    Motion_CallWaitAnim_9(14, 3);
    Battle_WaitMode0_81(20);
    SceneWork_SetStepValue_66((script_c + 28));
    BattleEv_RunWait_64(14, 0);
    BattleFx_SpawnLinked_39(2, 0x101, 90);
    SceneWork_SetStepValue_67((script_c + 29));
    BattleEv_RunWait_65(2, 0);
    Motion_ArmCb_22(14, 0x5000, 0);
    Battle_WaitMode0_82(20);
    SceneWork_SetStepValue_68((script_c + 30));
    BattleEv_RunWait_66(14, 0);
    Motion_SetVarCb_19(12, 1);
    Battle_WaitMode0_83(60);
    SceneWork_SetStepValue_69((script_c + 31));
    BattleEv_RunWait_67(12, 0);
    Motion_SetAngleToward_73(14, 12, 0);
    Battle_WaitMode0_84(20);
    Motion_CallWaitAnim_10(14, 3);
    Battle_WaitMode0_85(20);
    SceneWork_SetStepValue_70((script_c + 32));
    BattleEv_RunWait_68(14, 0);
    BattleFx_SpawnLinked_40(1, 0x100, 70);
    SceneWork_SetStepValue_71((script_c + 33));
    BattleEv_RunWait_69(1, 0);
    Motion_SetAngleToward_74(14, 0, 0);
    Battle_WaitMode0_86(30);
    SceneWork_SetStepValue_72((script_c + 34));
    BattleEv_RunWait_70(14, 0);
    BattleFx_SpawnLinked_41(3, 0x100, 80);
    SceneWork_SetStepValue_73((script_c + 35));
    BattleEv_RunWait_71(3, 0);
    Motion_CallWaitAnim_11(14, 3);
    SceneWork_SetStepValue_74((script_c + 36));
    BattleEv_RunWait_72(14, 0);
    Motion_SetAngleToward_75(2, 0, 0);
    Battle_WaitMode0_87(30);
    Object_SetModeById_51(2, 3);
    Battle_WaitMode0_88(100);
    SceneWork_SetStepValue_75((script_c + 37));
    Motion_SetSpeed_18(2, 0);
    if (UiWork_WaitThenFinalizeCapacity_3(0, 0) != 0) {
        SceneWork_SetStepValue_76((script_c + 38));
        BattleEv_RunWait_73(14, 0);
    }
    Battle_WaitMode0_89(30);
    Object_SetModeById_52(1, 3);
    Object_SetModeById_53(2, 3);
    Object_SetModeById_54(3, 3);
    Battle_WaitMode0_90(100);
    Object_LinkPair_7(0, 12, 0);
    Object_SetModeById_55(0, 3);
    Object_SetModeById_56(12, 3);
    Battle_WaitMode0_91(100);
    Object_SetModeById_57(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_22(1);
    Motion_SetHPosTerrain_10(1, 0, 0);
    Object_SetModeById_58(2, 2);
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_2(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_23(2);
    Motion_SetHPosTerrain_11(2, 0, 0);
    Object_SetModeById_59(3, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_24(3);
    Motion_SetHPosTerrain_12(3, 0, 0);
    Battle_WaitMode0_92(30);
    Motion_SetSpeed_19(12, 0x6666, 0x3333);
    Motion_ResetPosMode2_22(12, 0x1d8, 184);
    Motion_CommitPos_25(12);
    Object_SetModeById_60(12, 1);
    Motion_ArmCb_23(12, 0x3000, 0);
    Motion_SetAngleToward_76(14, 0, 0);
    Object_SetModeById_61(14, 3);
    Battle_WaitMode0_93(20);
    Motion_SetAngleToward_77(14, 13, 0);
    Battle_WaitMode0_94(20);
    SceneWork_SetStepValue_77((script_c + 39));
    BattleEv_RunWait_74(14, 0);
    Motion_SetVarCbObj_2(13, 2);
    SceneWork_SetStepValue_78((script_c + 40));
    BattleEv_RunWait_75(13, 0);
    Object_SetModeById_62(0, 3);
    Battle_WaitMode0_95(30);
    Motion_SetSpeed_20(0, 0x10000, 0x8000);
    Motion_SetSpeed_21(12, 0x10000, 0x8000);
    Motion_SetPosReset_1(0, 1);
    Motion_ResetPosMode2_23(0, 0x1e0, 248);
    Battle_WaitMode0_96(40);
    Motion_SetActionVariant_2(12, 0);
    Motion_SetActionVariant_3(0, 0);
    Motion_ResetPosMode2_24(12, 0x1e0, 216);
    Motion_CommitPos_26(0);
    Motion_SetAngleToward_78(0, 14, 0);
    Motion_CommitPos_27(12);
    Motion_SetAngleToward_79(12, 14, 0);
    Motion_ArmCb_24(12, 0x3000, 0);
    Object_SetModeById_63(0, 3);
    Battle_WaitMode0_97(5);
    Object_SetModeById_64(12, 3);
    Battle_WaitMode0_98(100);
    Object_SetModeById_65(14, 3);
    Battle_WaitMode0_99(100);
    Motion_ResetPosMode2_25(12, 0x1e0, 248);
    Motion_ResetPosMode2_26(0, 0x1c8, 248);
    Motion_CommitPos_28(12);
    Motion_ResetPosMode2_27(12, 0x1c8, 248);
    Motion_CommitPos_29(0);
    Motion_ResetPosMode2_28(0, 0x1c8, 0x168);
    Motion_CommitPos_30(12);
    Motion_ResetPosMode2_29(12, 0x1c8, 0x168);
    Motion_CommitPos_31(0);
    Motion_ResetPosMode2_30(0, 0x160, 0x168);
    Motion_CommitPos_32(12);
    Motion_ResetPosMode2_31(12, 0x160, 0x168);
    Battle_WaitMode0_100(20);
    Audio_PlayCue_6(17);
    SCENE_PHASE = 0x203;
    Func_02009d7e();
    Battle_WaitMode0_101(1);
    Battle_WaitMode0_102(210);
    Func_02009d82(4);
    Battle_SchedShoulder_1();
}
#undef GameFlag_Set_1
#undef GameFlag_Set_2
#undef Battle_Reset_1
#undef Scene_GetRecord_1
#undef Motion_SetHPosTerrain_1
#undef Motion_ArmCb_1
#undef Motion_SetHPosTerrain_2
#undef Motion_ArmCb_2
#undef Motion_SetHPosTerrain_3
#undef Motion_ArmCb_3
#undef Motion_SetHPosTerrain_4
#undef Motion_ArmCb_4
#undef Motion_SetHPosTerrain_5
#undef Motion_ArmCb_5
#undef Motion_SetHPosTerrain_6
#undef Motion_ArmCb_6
#undef Motion_SetHPosTerrain_7
#undef Motion_SetActionVariant_1
#undef Object_SetModeById_1
#undef Motion_SetHPosTerrain_8
#undef Motion_ArmCb_7
#undef Object_SetModeById_2
#undef Battle_WaitMode0_1
#undef Battle_WaitMode0_2
#undef SceneWork_SetStepValue_1
#undef BattleEv_RunWait_1
#undef BattleFx_SpawnLinked_1
#undef SceneWork_SetStepValue_2
#undef BattleEv_RunWait_2
#undef BattleFx_SpawnLinked_2
#undef BattleFx_SpawnLinked_3
#undef BattleFx_SpawnLinked_4
#undef Battle_WaitMode0_3
#undef Motion_SetAngleToward_1
#undef Battle_WaitMode0_4
#undef SceneWork_SetStepValue_3
#undef BattleEv_RunWait_3
#undef Motion_SetAngleToward_2
#undef Motion_SetAngleToward_3
#undef Motion_SetAngleToward_4
#undef Motion_SetAngleToward_5
#undef Battle_WaitMode0_5
#undef Motion_ResetPosMode2_1
#undef Object_SetModeById_3
#undef Battle_WaitMode0_6
#undef SceneWork_SetStepValue_4
#undef BattleEv_RunWait_4
#undef Battle_WaitMode0_7
#undef Motion_SetVarCb_1
#undef SceneWork_SetStepValue_5
#undef BattleEv_RunWait_5
#undef Motion_SetAngleToward_6
#undef Motion_SetAngleToward_7
#undef Motion_SetAngleToward_8
#undef Motion_SetAngleToward_9
#undef Motion_SetVarCb_2
#undef SceneWork_SetStepValue_6
#undef BattleEv_RunWait_6
#undef BattleFx_SpawnLinked_5
#undef Object_SetModeById_4
#undef Battle_WaitMode0_8
#undef Object_SetModeById_5
#undef SceneWork_SetStepValue_7
#undef BattleEv_RunWait_7
#undef BattleFx_SpawnLinked_6
#undef Motion_SetVarCb_3
#undef Battle_WaitMode0_9
#undef SceneWork_SetStepValue_8
#undef BattleEv_RunWait_8
#undef Motion_SetAngleToward_10
#undef Battle_WaitMode0_10
#undef SceneWork_SetStepValue_9
#undef BattleEv_RunWait_9
#undef Motion_SetVarCb_4
#undef Battle_WaitMode0_11
#undef SceneWork_SetStepValue_10
#undef BattleEv_RunWait_10
#undef Motion_SetAngleToward_11
#undef SceneWork_SetStepValue_11
#undef Motion_SetSpeed_1
#undef UiWork_WaitThenFinalizeCapacity_1
#undef Object_SetModeById_6
#undef Object_SetModeById_7
#undef Object_SetModeById_8
#undef Motion_SetAngleToward_12
#undef Battle_WaitMode0_12
#undef SceneWork_SetStepValue_12
#undef BattleEv_RunWait_11
#undef Motion_SetAngleToward_13
#undef Motion_SetAngleToward_14
#undef Motion_SetAngleToward_15
#undef Motion_SetAngleToward_16
#undef Battle_WaitMode0_13
#undef Motion_SetAngleToward_17
#undef Battle_WaitMode0_14
#undef Motion_SetAngleToward_18
#undef Battle_WaitMode0_15
#undef Motion_CallWaitAnim_1
#undef Battle_WaitMode0_16
#undef Motion_SetAngleToward_19
#undef SceneWork_SetStepValue_13
#undef BattleEv_RunWait_12
#undef Battle_WaitMode0_17
#undef Motion_SetAngleToward_20
#undef Motion_SetAngleToward_21
#undef Motion_SetAngleToward_22
#undef Motion_SetAngleToward_23
#undef Object_SetModeById_9
#undef Object_SetModeById_10
#undef Object_SetModeById_11
#undef Object_SetModeById_12
#undef Battle_WaitMode0_18
#undef Motion_SetAngleToward_24
#undef Motion_SetAngleToward_25
#undef Motion_SetAngleToward_26
#undef Motion_SetAngleToward_27
#undef Motion_SetAngleToward_28
#undef Motion_SetSpeed_2
#undef Motion_SetSpeed_3
#undef Motion_SetSpeed_4
#undef Motion_SetSpeed_5
#undef Motion_ResetPosMode2_2
#undef Motion_CommitPos_1
#undef Motion_ResetPosMode2_3
#undef Motion_CommitPos_2
#undef Motion_ResetPosMode2_4
#undef Motion_CommitPos_3
#undef Motion_ArmCb_8
#undef Motion_ResetPosMode2_5
#undef Motion_CommitPos_4
#undef Motion_ArmCb_9
#undef Motion_ResetPosMode2_6
#undef Motion_CommitPos_5
#undef Motion_ResetPosMode2_7
#undef Motion_CommitPos_6
#undef Motion_ArmCb_10
#undef Motion_ResetPosMode2_8
#undef Motion_CommitPos_7
#undef Motion_ArmCb_11
#undef Object_LinkPair_1
#undef Object_LinkPair_2
#undef Battle_WaitMode0_19
#undef Motion_CallWaitAnim_2
#undef Object_SetModeById_13
#undef Object_SetModeById_14
#undef Object_SetModeById_15
#undef Battle_WaitMode0_20
#undef Audio_PlayCue_1
#undef Object_SetModeById_16
#undef Motion_SetSpeed_6
#undef Motion_OffsetPositionAndResetMotion_1
#undef Motion_SetSpeed_7
#undef Motion_OffsetPositionAndResetMotion_2
#undef Motion_SetSpeed_8
#undef Motion_OffsetPositionAndResetMotion_3
#undef Motion_SetSpeed_9
#undef Motion_OffsetPositionAndResetMotion_4
#undef Motion_SetSpeed_10
#undef Motion_SetSpeed_11
#undef Motion_OffsetPositionAndResetMotion_5
#undef Motion_OffsetPositionAndResetMotion_6
#undef Motion_CommitPos_8
#undef Audio_PlayCue_2
#undef Battle_WaitMode0_21
#undef Motion_ArmCb_12
#undef Motion_ArmCb_13
#undef Motion_ArmCb_14
#undef Motion_ArmCb_15
#undef Battle_WaitMode0_22
#undef Object_SetModeById_17
#undef Object_SetModeById_18
#undef Object_SetModeById_19
#undef Object_SetModeById_20
#undef Battle_WaitMode0_23
#undef Motion_SetSpeed_12
#undef Motion_SetSpeed_13
#undef Motion_SetSpeed_14
#undef Motion_SetSpeed_15
#undef Motion_ResetPosMode2_9
#undef Motion_CommitPos_9
#undef Object_SetModeById_21
#undef Motion_ArmCb_16
#undef Motion_ResetPosMode2_10
#undef Motion_CommitPos_10
#undef Motion_ResetPosMode2_11
#undef Motion_CommitPos_11
#undef Motion_ResetPosMode2_12
#undef Motion_CommitPos_12
#undef Object_SetModeById_22
#undef Motion_ArmCb_17
#undef Motion_ResetPosMode2_13
#undef Motion_CommitPos_13
#undef Motion_ResetPosMode2_14
#undef Motion_ResetPosMode2_15
#undef Motion_CommitPos_14
#undef Object_SetModeById_23
#undef Motion_ArmCb_18
#undef Motion_CommitPos_15
#undef Object_SetModeById_24
#undef Motion_ArmCb_19
#undef Battle_WaitMode0_24
#undef Battle_WaitMode0_25
#undef SceneWork_SetStepValue_14
#undef BattleEv_RunWait_13
#undef Object_SetModeById_25
#undef Battle_WaitMode0_26
#undef Motion_SetVarCb_5
#undef Battle_WaitMode0_27
#undef Object_SetModeById_26
#undef SceneWork_SetStepValue_15
#undef BattleEv_RunWait_14
#undef Battle_WaitMode0_28
#undef Object_LinkPair_3
#undef Battle_WaitMode0_29
#undef BattleFx_SpawnLinked_7
#undef Motion_SetAngleToward_29
#undef Motion_SetAngleToward_30
#undef SceneWork_SetStepValue_16
#undef BattleEv_RunWait_15
#undef BattleFx_SpawnLinked_8
#undef Motion_SetVarCb_6
#undef Battle_WaitMode0_30
#undef Object_SetModeById_27
#undef Battle_WaitMode0_31
#undef SceneWork_SetStepValue_17
#undef BattleEv_RunWait_16
#undef Object_SetModeById_28
#undef BattleFx_SpawnLinked_9
#undef SceneWork_SetStepValue_18
#undef BattleEv_RunWait_17
#undef Motion_CallWaitAnim_3
#undef BattleFx_SpawnLinked_10
#undef Motion_ResetPosMode2_16
#undef Motion_CommitPos_16
#undef Motion_SetAngleToward_31
#undef SceneWork_SetStepValue_19
#undef BattleEv_RunWait_18
#undef Battle_WaitMode0_32
#undef BattleFx_SpawnLinked_11
#undef BattleFx_SpawnLinked_12
#undef BattleFx_SpawnLinked_13
#undef BattleFx_SpawnLinked_14
#undef Battle_WaitMode0_33
#undef Motion_SetAngleToward_32
#undef BattleFx_SpawnLinked_15
#undef Motion_SetAngleToward_33
#undef Object_SetModeById_29
#undef SceneWork_SetStepValue_20
#undef BattleEv_RunWait_19
#undef BattleFx_SpawnLinked_16
#undef Battle_WaitMode0_34
#undef SceneWork_SetStepValue_21
#undef BattleEv_RunWait_20
#undef Motion_ResetPosMode2_17
#undef Motion_CommitPos_17
#undef Object_SetModeById_30
#undef Motion_SetAngleToward_34
#undef SceneWork_SetStepValue_22
#undef BattleEv_RunWait_21
#undef Motion_SetVarCb_7
#undef Battle_WaitMode0_35
#undef Motion_SetAngleToward_35
#undef Battle_WaitMode0_36
#undef SceneWork_SetStepValue_23
#undef BattleEv_RunWait_22
#undef Object_SetModeById_31
#undef Battle_WaitMode0_37
#undef SceneWork_SetStepValue_24
#undef BattleEv_RunWait_23
#undef Motion_SetVarCb_8
#undef Battle_WaitMode0_38
#undef SceneWork_SetStepValue_25
#undef BattleEv_RunWait_24
#undef Motion_SetAngleToward_36
#undef Battle_WaitMode0_39
#undef SceneWork_SetStepValue_26
#undef BattleEv_RunWait_25
#undef Battle_WaitMode0_40
#undef BattleFx_SpawnLinked_17
#undef Motion_SetAngleToward_37
#undef SceneWork_SetStepValue_27
#undef BattleEv_RunWait_26
#undef Motion_SetAngleToward_38
#undef SceneWork_SetStepValue_28
#undef BattleEv_RunWait_27
#undef Motion_SetAngleToward_39
#undef Motion_SetAngleToward_40
#undef Motion_SetAngleToward_41
#undef Motion_SetAngleToward_42
#undef Battle_WaitMode0_41
#undef BattleFx_SpawnLinked_18
#undef Motion_SetVarCb_9
#undef Battle_WaitMode0_42
#undef Motion_SetVarCb_10
#undef Battle_WaitMode0_43
#undef SceneWork_SetStepValue_29
#undef BattleEv_RunWait_28
#undef BattleFx_SpawnLinked_19
#undef Motion_ResetPosMode2_18
#undef Motion_CommitPos_18
#undef Object_SetModeById_32
#undef SceneWork_SetStepValue_30
#undef BattleEv_RunWait_29
#undef Motion_SetAngleToward_43
#undef Battle_WaitMode0_44
#undef Motion_SetVarCb_11
#undef SceneWork_SetStepValue_31
#undef BattleEv_RunWait_30
#undef Motion_SetAngleToward_44
#undef Battle_WaitMode0_45
#undef Motion_SetAngleToward_45
#undef Battle_WaitMode0_46
#undef Motion_SetAngleToward_46
#undef Battle_WaitMode0_47
#undef Motion_SetAngleToward_47
#undef Motion_SetAngleToward_48
#undef Motion_SetAngleToward_49
#undef Motion_SetAngleToward_50
#undef Battle_WaitMode0_48
#undef Motion_CallWaitAnim_4
#undef Battle_WaitMode0_49
#undef SceneWork_SetStepValue_32
#undef BattleEv_RunWait_31
#undef Motion_SetVarCb_12
#undef Battle_WaitMode0_50
#undef SceneWork_SetStepValue_33
#undef BattleEv_RunWait_32
#undef BattleFx_SpawnLinked_20
#undef SceneWork_SetStepValue_34
#undef BattleEv_RunWait_33
#undef Motion_SetAngleToward_51
#undef Battle_WaitMode0_51
#undef SceneWork_SetStepValue_35
#undef Motion_SetSpeed_16
#undef Motion_SetAngleToward_52
#undef Motion_SetAngleToward_53
#undef Motion_SetAngleToward_54
#undef Motion_SetAngleToward_55
#undef Motion_SetHPosTerrain_9
#undef Motion_SetSpeed_17
#undef UiWork_WaitThenFinalizeCapacity_2
#undef Audio_PlayCue_3
#undef SceneWork_SetStepValue_36
#undef BattleEv_RunWait_34
#undef Audio_PlayCue_4
#undef SceneWork_SetStepValue_37
#undef BattleEv_RunWait_35
#undef Motion_SetAngleToward_56
#undef Motion_SetAngleToward_57
#undef Motion_SetAngleToward_58
#undef Motion_SetAngleToward_59
#undef Motion_SetAngleToward_60
#undef Motion_SetAngleToward_61
#undef Motion_CamBounds_1
#undef Motion_SetVarCb_13
#undef Battle_WaitMode0_52
#undef Audio_PlayCue_5
#undef SceneWork_SetStepValue_38
#undef BattleEv_RunWait_36
#undef Motion_ResetPosMode2_19
#undef Motion_CommitPos_19
#undef Motion_ResetPosMode2_20
#undef Motion_CamBounds_2
#undef SceneWork_SetStepValue_39
#undef BattleEv_RunWait_37
#undef Motion_CommitPos_20
#undef Motion_ArmCb_20
#undef BattleFx_SpawnLinked_21
#undef Object_SetModeById_33
#undef Battle_WaitMode0_53
#undef SceneWork_SetStepValue_40
#undef BattleEv_RunWait_38
#undef Motion_SetVarCbObj_1
#undef Motion_CallWaitAnim_5
#undef SceneWork_SetStepValue_41
#undef BattleEv_RunWait_39
#undef BattleFx_SpawnLinked_22
#undef Object_SetModeById_34
#undef Battle_WaitMode0_54
#undef Motion_ArmCb_21
#undef Battle_WaitMode0_55
#undef SceneWork_SetStepValue_42
#undef BattleEv_RunWait_40
#undef Motion_SetVarCb_14
#undef SceneWork_SetStepValue_43
#undef BattleEv_RunWait_41
#undef Battle_WaitMode0_56
#undef Motion_CallWaitAnim_6
#undef Motion_SetAngleToward_62
#undef Battle_WaitMode0_57
#undef SceneWork_SetStepValue_44
#undef BattleEv_RunWait_42
#undef Motion_SetVarCb_15
#undef BattleFx_SpawnLinked_23
#undef SceneWork_SetStepValue_45
#undef BattleEv_RunWait_43
#undef BattleFx_SpawnLinked_24
#undef SceneWork_SetStepValue_46
#undef BattleEv_RunWait_44
#undef Motion_CallWaitAnim_7
#undef SceneWork_SetStepValue_47
#undef BattleEv_RunWait_45
#undef Battle_WaitMode0_58
#undef Battle_WaitMode0_59
#undef Battle_WaitMode0_60
#undef Battle_WaitMode0_61
#undef Motion_ResetPosMode2_21
#undef Motion_CommitPos_21
#undef Motion_SetAngleToward_63
#undef Battle_WaitMode0_62
#undef Motion_SetAngleToward_64
#undef SceneWork_SetStepValue_48
#undef BattleEv_RunWait_46
#undef Motion_SetVarCb_16
#undef SceneWork_SetStepValue_49
#undef BattleEv_RunWait_47
#undef Battle_WaitMode0_63
#undef Motion_CallWaitAnim_8
#undef Battle_WaitMode0_64
#undef SceneWork_SetStepValue_50
#undef BattleEv_RunWait_48
#undef Object_SetModeById_35
#undef BattleFx_SpawnLinked_25
#undef SceneWork_SetStepValue_51
#undef BattleEv_RunWait_49
#undef BattleFx_SpawnLinked_26
#undef SceneWork_SetStepValue_52
#undef BattleEv_RunWait_50
#undef Object_SetModeById_36
#undef Battle_WaitMode0_65
#undef Object_SetModeById_37
#undef Battle_WaitMode0_66
#undef Object_LinkPair_4
#undef Object_SetModeById_38
#undef Object_SetModeById_39
#undef Object_SetModeById_40
#undef Object_SetModeById_41
#undef Battle_WaitMode0_67
#undef BattleFx_SpawnLinked_27
#undef SceneWork_SetStepValue_53
#undef BattleEv_RunWait_51
#undef BattleFx_SpawnLinked_28
#undef SceneWork_SetStepValue_54
#undef BattleEv_RunWait_52
#undef Object_LinkPair_5
#undef Object_LinkPair_6
#undef Battle_WaitMode0_68
#undef Motion_SetAngleToward_65
#undef Battle_WaitMode0_69
#undef Motion_SetAngleToward_66
#undef Motion_SetAngleToward_67
#undef Motion_SetAngleToward_68
#undef Motion_SetAngleToward_69
#undef Battle_WaitMode0_70
#undef SceneWork_SetStepValue_55
#undef BattleEv_RunWait_53
#undef BattleFx_SpawnLinked_29
#undef BattleFx_SpawnLinked_30
#undef BattleFx_SpawnLinked_31
#undef BattleFx_SpawnLinked_32
#undef Battle_WaitMode0_71
#undef SceneWork_SetStepValue_56
#undef BattleEv_RunWait_54
#undef Motion_SetVarCb_17
#undef Battle_WaitMode0_72
#undef SceneWork_SetStepValue_57
#undef BattleEv_RunWait_55
#undef Motion_SetAngleToward_70
#undef Battle_WaitMode0_73
#undef SceneWork_SetStepValue_58
#undef BattleEv_RunWait_56
#undef BattleFx_SpawnLinked_33
#undef BattleFx_SpawnLinked_34
#undef BattleFx_SpawnLinked_35
#undef BattleFx_SpawnLinked_36
#undef Battle_WaitMode0_74
#undef Object_SetModeById_42
#undef Battle_WaitMode0_75
#undef SceneWork_SetStepValue_59
#undef BattleEv_RunWait_57
#undef BattleFx_SpawnLinked_37
#undef SceneWork_SetStepValue_60
#undef BattleEv_RunWait_58
#undef Object_SetModeById_43
#undef Object_SetModeById_44
#undef Object_SetModeById_45
#undef Object_SetModeById_46
#undef Object_SetModeById_47
#undef Battle_WaitMode0_76
#undef Object_SetModeById_48
#undef Battle_WaitMode0_77
#undef SceneWork_SetStepValue_61
#undef BattleEv_RunWait_59
#undef BattleFx_SpawnLinked_38
#undef SceneWork_SetStepValue_62
#undef BattleEv_RunWait_60
#undef Motion_SetAngleToward_71
#undef Battle_WaitMode0_78
#undef Motion_SetVarCb_18
#undef Battle_WaitMode0_79
#undef SceneWork_SetStepValue_63
#undef BattleEv_RunWait_61
#undef Object_SetModeById_49
#undef Battle_WaitMode0_80
#undef SceneWork_SetStepValue_64
#undef BattleEv_RunWait_62
#undef Object_SetModeById_50
#undef SceneWork_SetStepValue_65
#undef BattleEv_RunWait_63
#undef Motion_SetAngleToward_72
#undef Motion_CallWaitAnim_9
#undef Battle_WaitMode0_81
#undef SceneWork_SetStepValue_66
#undef BattleEv_RunWait_64
#undef BattleFx_SpawnLinked_39
#undef SceneWork_SetStepValue_67
#undef BattleEv_RunWait_65
#undef Motion_ArmCb_22
#undef Battle_WaitMode0_82
#undef SceneWork_SetStepValue_68
#undef BattleEv_RunWait_66
#undef Motion_SetVarCb_19
#undef Battle_WaitMode0_83
#undef SceneWork_SetStepValue_69
#undef BattleEv_RunWait_67
#undef Motion_SetAngleToward_73
#undef Battle_WaitMode0_84
#undef Motion_CallWaitAnim_10
#undef Battle_WaitMode0_85
#undef SceneWork_SetStepValue_70
#undef BattleEv_RunWait_68
#undef BattleFx_SpawnLinked_40
#undef SceneWork_SetStepValue_71
#undef BattleEv_RunWait_69
#undef Motion_SetAngleToward_74
#undef Battle_WaitMode0_86
#undef SceneWork_SetStepValue_72
#undef BattleEv_RunWait_70
#undef BattleFx_SpawnLinked_41
#undef SceneWork_SetStepValue_73
#undef BattleEv_RunWait_71
#undef Motion_CallWaitAnim_11
#undef SceneWork_SetStepValue_74
#undef BattleEv_RunWait_72
#undef Motion_SetAngleToward_75
#undef Battle_WaitMode0_87
#undef Object_SetModeById_51
#undef Battle_WaitMode0_88
#undef SceneWork_SetStepValue_75
#undef Motion_SetSpeed_18
#undef UiWork_WaitThenFinalizeCapacity_3
#undef SceneWork_SetStepValue_76
#undef BattleEv_RunWait_73
#undef Battle_WaitMode0_89
#undef Object_SetModeById_52
#undef Object_SetModeById_53
#undef Object_SetModeById_54
#undef Battle_WaitMode0_90
#undef Object_LinkPair_7
#undef Object_SetModeById_55
#undef Object_SetModeById_56
#undef Battle_WaitMode0_91
#undef Object_SetModeById_57
#undef Scene_GetRecord_2
#undef Motion_ResetAndSetPosition_1
#undef Motion_CommitPos_22
#undef Motion_SetHPosTerrain_10
#undef Object_SetModeById_58
#undef Scene_GetRecord_3
#undef Motion_ResetAndSetPosition_2
#undef Motion_CommitPos_23
#undef Motion_SetHPosTerrain_11
#undef Object_SetModeById_59
#undef Scene_GetRecord_4
#undef Motion_ResetAndSetPosition_3
#undef Motion_CommitPos_24
#undef Motion_SetHPosTerrain_12
#undef Battle_WaitMode0_92
#undef Motion_SetSpeed_19
#undef Motion_ResetPosMode2_22
#undef Motion_CommitPos_25
#undef Object_SetModeById_60
#undef Motion_ArmCb_23
#undef Motion_SetAngleToward_76
#undef Object_SetModeById_61
#undef Battle_WaitMode0_93
#undef Motion_SetAngleToward_77
#undef Battle_WaitMode0_94
#undef SceneWork_SetStepValue_77
#undef BattleEv_RunWait_74
#undef Motion_SetVarCbObj_2
#undef SceneWork_SetStepValue_78
#undef BattleEv_RunWait_75
#undef Object_SetModeById_62
#undef Battle_WaitMode0_95
#undef Motion_SetSpeed_20
#undef Motion_SetSpeed_21
#undef Motion_SetPosReset_1
#undef Motion_ResetPosMode2_23
#undef Battle_WaitMode0_96
#undef Motion_SetActionVariant_2
#undef Motion_SetActionVariant_3
#undef Motion_ResetPosMode2_24
#undef Motion_CommitPos_26
#undef Motion_SetAngleToward_78
#undef Motion_CommitPos_27
#undef Motion_SetAngleToward_79
#undef Motion_ArmCb_24
#undef Object_SetModeById_63
#undef Battle_WaitMode0_97
#undef Object_SetModeById_64
#undef Battle_WaitMode0_98
#undef Object_SetModeById_65
#undef Battle_WaitMode0_99
#undef Motion_ResetPosMode2_25
#undef Motion_ResetPosMode2_26
#undef Motion_CommitPos_28
#undef Motion_ResetPosMode2_27
#undef Motion_CommitPos_29
#undef Motion_ResetPosMode2_28
#undef Motion_CommitPos_30
#undef Motion_ResetPosMode2_29
#undef Motion_CommitPos_31
#undef Motion_ResetPosMode2_30
#undef Motion_CommitPos_32
#undef Motion_ResetPosMode2_31
#undef Battle_WaitMode0_100
#undef Audio_PlayCue_6
#undef Battle_WaitMode0_101
#undef Battle_WaitMode0_102
#undef Battle_SchedShoulder_1
#undef SCENE_PHASE

/* map/locations/heidia/village/party_animation.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007d7e();
void Func_02007d98();
void Func_02007db2_a(); void Func_02007db2_b();
void Func_02007dcc();
void Func_02007de6();
void Func_02007e04();
void Func_02007e16();
void Func_02007e28();
void Func_02007e80();
void Func_02007e92();
void Func_02007ea4();
void Func_02007eb0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_02002718(void)
{
    u32 i;
    s32 record;

    Call3(Func_02007d7e, 2, 0x10000, 0x8000);
    Call3(Func_02007db2_a, 2, 0x1f8, 216);
    Call3(Func_02007d98, 3, 0x10000, 0x8000);
    Call3(Func_02007dcc, 3, 0x1b8, 232);
    Call3(Func_02007db2_b, 1, 0x10000, 0x8000);
    Call3(Func_02007de6, 1, 0x1e0, 224);
    Func_02007e04(1);
    Call3(Func_02007e80, 1, 0xc000, 0);
    Func_02007e16(2);
    Call3(Func_02007e92, 2, 0xc000, 0);
    Func_02007e28(3);
    Call3(Func_02007ea4, 3, 0xc000, 0);
    Call3(Func_02007eb0, 0, 0xc000, 0);
}

/* map/locations/heidia/village/scene_party_placement.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02007b82();
s32 Func_02007b96();
s32 Func_02007baa();
void Func_02007bd4();
void Func_02007be0();
void Func_02007bea();
void Func_02007bf4();
void Func_02007c00();
void Func_02007c08_a();
void Func_02007c08_b();
void Func_02007c12();
void Func_02007c1e();
void Func_02007c34();
void Func_02007c52();
void Func_02007c62();
void Func_02007c68();
void Func_02007c7a();
void Func_02007c94();
void Func_02007caa();
void Func_02007cb4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_0200252c(void)
{
    u32 i;
    s32 record;

    record = Value1(Func_02007b82, 0);
    if (record != 0) {
        Func_02007be0(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02007b96, 0);
    if (record != 0) {
        Func_02007bf4(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02007baa, 0);
    if (record != 0) {
        Func_02007c08_a(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_02007c7a(0, 0, 0);
    Call3(Func_02007bd4, 2, 0xb333, 0x5999);
    Call3(Func_02007c08_b, 2, 0x1c8, 192);
    Call3(Func_02007bea, 3, 0xb333, 0x5999);
    Call3(Func_02007c1e, 3, 0x1b8, 184);
    Call3(Func_02007c00, 1, 0xb333, 0x5999);
    Call3(Func_02007c34, 1, 0x1c0, 240);
    Func_02007c52(2);
    Func_02007c94(2, 12, 0);
    Func_02007c62(1);
    Func_02007c68(3);
    Func_02007caa(1, 12, 0);
    Func_02007cb4(3, 12, 0);
    Func_02007c12(15);
}

/* map/locations/heidia/village/scene_sequence.c */
/* Audited retained supplemental scene body.
 * The complete production span preserves 49 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

/* Two id arguments recur across most of the calls below: 24 is passed as the
 * first argument throughout the main sequence, and 25 is introduced only
 * after Motion_SetAngleToward_2(25, 24, 0), whose second argument is 24 -- so 25 is
 * derived from 24 partway through the sequence. */
#define PRIMARY_ID 24
#define DERIVED_ID 25

s32 Func_02009fb6();
s32 Func_02009fc2();
s32 Func_02009fd0();
void Func_0200a00c();
void Func_0200a036();
void Func_0200a046();
void Func_0200a070();
void Func_0200a08a();
void Func_0200a090();
void Func_0200a098();
void Func_0200a09e();
void Func_0200a0a6();
void Func_0200a0b0();
void Func_0200a0b4();
void Func_0200a0b6();
void Func_0200a0cc();
void Func_0200a0ce();
void Func_0200a0d8();
void Func_0200a0e2();
void Func_0200a0fe();
void Func_0200a10c();
void Func_0200a112();
void Func_0200a118();
void Func_0200a122();
void Func_0200a124();
void Func_0200a12a();
void Func_0200a12c();
void Func_0200a132();
void Func_0200a13a_a();
void Func_0200a13a_b();
void Func_0200a142();
void Func_0200a154();
void Func_0200a15a();
void Func_0200a15c();
void Func_0200a162();
void Func_0200a16e();
void Func_0200a17c();
void Func_0200a18c_a();
void Func_0200a18c_b();
void Func_0200a1a4();
void Func_0200a1a6();
void Func_0200a1b4();
void Func_0200a1c2();
void Func_0200a1d6();
void Func_0200a1da();
void Func_0200a20a();
void Func_0200a21a();
void Func_0200a222();
void Func_0200a232();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_02009fb6, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02009fc2, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02009fd0, a0)
#define SceneWork_SetStepValue_1(a0) Call1(Func_0200a0b6, a0)
#define BattleEv_RunWait_1(a0, a1) Call2(Func_0200a0ce, a0, a1)
#define Motion_SetVarCbObj_1(a0, a1) Value2(Func_0200a0a6, a0, a1)
#define Battle_WaitMode0_1(a0) Value1(Func_0200a00c, a0)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_0200a046, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_1(a0, a1, a2) Value3(Func_0200a08a, a0, a1, a2)
#define Motion_CommitPos_1(a0) Value1(Func_0200a098, a0)
#define Object_SetModeById_1(a0, a1) Value2(Func_0200a0b0, a0, a1)
#define Battle_WaitMode0_2(a0) Value1(Func_0200a036, a0)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_0200a070, a0, a1, a2)
#define Motion_OffsetPositionAndResetMotion_2(a0, a1, a2) Call3(Func_0200a0b4, a0, a1, a2)
#define Motion_SetAngleToward_1(a0, a1, a2) Value3(Func_0200a0fe, a0, a1, a2)
#define Motion_CommitPos_2(a0) Value1(Func_0200a0cc, a0)
#define SceneWork_SetStepValue_2(a0) Value1(Func_0200a122, a0)
#define BattleEv_RunWait_2(a0, a1) Call2(Func_0200a13a_a, a0, a1)
#define Motion_SetVarCbObj_2(a0, a1) Call2(Func_0200a112, a0, a1)
#define Motion_SetAngleToward_2(a0, a1, a2) Value3(Func_0200a124, a0, a1, a2)
#define SceneWork_SetStepValue_3(a0) Value1(Func_0200a142, a0)
#define BattleEv_RunWait_3(a0, a1) Value2(Func_0200a15a, a0, a1)
#define Battle_WaitMode0_3(a0) Value1(Func_0200a090, a0)
#define Object_SetModeById_2(a0, a1) Value2(Func_0200a118, a0, a1)
#define Battle_WaitMode0_4(a0) Value1(Func_0200a09e, a0)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_0200a0d8, a0, a1, a2)
#define Motion_ResetPosMode2_1(a0, a1, a2) Value3(Func_0200a10c, a0, a1, a2)
#define Motion_CommitPos_3(a0) Value1(Func_0200a12a, a0)
#define Motion_ArmCb_1(a0, a1, a2) Value3(Func_0200a1a6, a0, a1, a2)
#define SceneWork_SetStepValue_4(a0) Value1(Func_0200a18c_a, a0)
#define BattleEv_RunWait_4(a0, a1) Call2(Func_0200a1a4, a0, a1)
#define Object_SetModeById_3(a0, a1) Value2(Func_0200a15c, a0, a1)
#define Battle_WaitMode0_5(a0) Value1(Func_0200a0e2, a0)
#define Motion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Func_0200a154, a0, a1, a2)
#define Motion_CommitPos_4(a0) Value1(Func_0200a162, a0)
#define Object_SetModeById_4(a0, a1) Value2(Func_0200a17c, a0, a1)
#define SceneWork_SetStepValue_5(a0) Value1(Func_0200a1c2, a0)
#define BattleEv_RunWait_5(a0, a1) Call2(Func_0200a1da, a0, a1)
#define Motion_ResetPosMode2_2(a0, a1, a2) Value3(Func_0200a16e, a0, a1, a2)
#define Motion_CommitPos_5(a0) Value1(Func_0200a18c_b, a0)
#define Object_LinkPair_1(a0, a1, a2) Value3(Func_0200a1d6, a0, a1, a2)
#define Battle_WaitMode0_6(a0) Value1(Func_0200a12c, a0)
#define Object_SetModeById_5(a0, a1) Call2(Func_0200a1b4, a0, a1)
#define Battle_WaitMode0_7(a0) Value1(Func_0200a13a_b, a0)
#define GameFlag_Set_1(a0) Value1(Func_0200a132, a0)
#define SceneWork_SetStepValue_6(a0) Call1(Func_0200a20a, a0)
#define BattleEv_RunWait_6(a0, a1) Call2(Func_0200a222, a0, a1)
#define SceneWork_SetStepValue_7(a0) Call1(Func_0200a21a, a0)
#define BattleEv_RunWait_7(a0, a1) Call2(Func_0200a232, a0, a1)

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

static __inline__ void Call0(void (*f)())
{
    f();
}

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

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); a sequence id counting up from
 * 9569 is threaded through four of the calls one apart. Each of the two
 * outer gating checks has its own short fallback branch on PRIMARY_ID. */
void Scene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet_1(2369) != 0) {
        if (GameFlag_IsSet_2(2382) == 0 && GameFlag_IsSet_3(788) == 0) {
        sequence_id = 9569;
        SceneWork_SetStepValue_1(sequence_id);
        BattleEv_RunWait_1(PRIMARY_ID, 0);
        Motion_SetVarCbObj_1(PRIMARY_ID, 1);
        Battle_WaitMode0_1(30);
        Motion_SetSpeed_1(PRIMARY_ID, 6553, 3276);
        Motion_OffsetPositionAndResetMotion_1(PRIMARY_ID, -4, 0);
        Motion_CommitPos_1(PRIMARY_ID);
        Object_SetModeById_1(PRIMARY_ID, 3);
        Battle_WaitMode0_2(60);
        Motion_SetSpeed_2(PRIMARY_ID, 13107, 6553);
        Motion_OffsetPositionAndResetMotion_2(PRIMARY_ID, -6, 0);
        Motion_SetAngleToward_1(PRIMARY_ID, 0, 0);
        Motion_CommitPos_2(PRIMARY_ID);
        SceneWork_SetStepValue_2(sequence_id + 1);
        BattleEv_RunWait_2(PRIMARY_ID, 0);
        Motion_SetVarCbObj_2(PRIMARY_ID, 1);
        Motion_SetAngleToward_2(DERIVED_ID, PRIMARY_ID, 0);
        SceneWork_SetStepValue_3(sequence_id + 2);
        BattleEv_RunWait_3(PRIMARY_ID, 0);
        Battle_WaitMode0_3(70);
        Object_SetModeById_2(DERIVED_ID, 3);
        Battle_WaitMode0_4(60);
        Motion_SetSpeed_3(DERIVED_ID, 26214, 13107);
        Motion_ResetPosMode2_1(DERIVED_ID, 880, 112);
        Motion_CommitPos_3(DERIVED_ID);
        Motion_ArmCb_1(DERIVED_ID, 53248, 0);
        SceneWork_SetStepValue_4(sequence_id + 3);
        BattleEv_RunWait_4(PRIMARY_ID, 0);
        Object_SetModeById_3(PRIMARY_ID, 3);
        Battle_WaitMode0_5(70);
        Motion_OffsetPositionAndResetMotion_3(PRIMARY_ID, 8, 0);
        Motion_CommitPos_4(PRIMARY_ID);
        Object_SetModeById_4(PRIMARY_ID, 5);
        SceneWork_SetStepValue_5(sequence_id + 4);
        BattleEv_RunWait_5(PRIMARY_ID, 0);
        Motion_ResetPosMode2_2(0, 896, 120);
        Motion_CommitPos_5(0);
        Object_LinkPair_1(0, DERIVED_ID, 0);
        Battle_WaitMode0_6(60);
        Object_SetModeById_5(DERIVED_ID, 3);
        Battle_WaitMode0_7(30);
        GameFlag_Set_1(788);
        } else {
            SceneWork_SetStepValue_6(9575);
            BattleEv_RunWait_6(PRIMARY_ID, 0);
        }
    } else {
        SceneWork_SetStepValue_7(0x244d);
        BattleEv_RunWait_7(PRIMARY_ID, 0);
    }
}
#undef PRIMARY_ID
#undef DERIVED_ID
#undef GameFlag_IsSet_1
#undef GameFlag_IsSet_2
#undef GameFlag_IsSet_3
#undef SceneWork_SetStepValue_1
#undef BattleEv_RunWait_1
#undef Motion_SetVarCbObj_1
#undef Battle_WaitMode0_1
#undef Motion_SetSpeed_1
#undef Motion_OffsetPositionAndResetMotion_1
#undef Motion_CommitPos_1
#undef Object_SetModeById_1
#undef Battle_WaitMode0_2
#undef Motion_SetSpeed_2
#undef Motion_OffsetPositionAndResetMotion_2
#undef Motion_SetAngleToward_1
#undef Motion_CommitPos_2
#undef SceneWork_SetStepValue_2
#undef BattleEv_RunWait_2
#undef Motion_SetVarCbObj_2
#undef Motion_SetAngleToward_2
#undef SceneWork_SetStepValue_3
#undef BattleEv_RunWait_3
#undef Battle_WaitMode0_3
#undef Object_SetModeById_2
#undef Battle_WaitMode0_4
#undef Motion_SetSpeed_3
#undef Motion_ResetPosMode2_1
#undef Motion_CommitPos_3
#undef Motion_ArmCb_1
#undef SceneWork_SetStepValue_4
#undef BattleEv_RunWait_4
#undef Object_SetModeById_3
#undef Battle_WaitMode0_5
#undef Motion_OffsetPositionAndResetMotion_3
#undef Motion_CommitPos_4
#undef Object_SetModeById_4
#undef SceneWork_SetStepValue_5
#undef BattleEv_RunWait_5
#undef Motion_ResetPosMode2_2
#undef Motion_CommitPos_5
#undef Object_LinkPair_1
#undef Battle_WaitMode0_6
#undef Object_SetModeById_5
#undef Battle_WaitMode0_7
#undef GameFlag_Set_1
#undef SceneWork_SetStepValue_6
#undef BattleEv_RunWait_6
#undef SceneWork_SetStepValue_7
#undef BattleEv_RunWait_7

/* map/locations/heidia/village/scene_transition.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007bfa();
void Func_02007c0e();
void Func_02007c3c();
void Func_02007c3e();
void Func_02007c4c();
void Func_02007cb6();
void Func_02007d70();
void Func_02007d7c();
void Func_02007d88();
void Func_02007d94();
s32 Func_02007da0();
void Func_02007db8();
void Func_02007dc6();
void Func_02007dd2();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_020025f8(void)
{
    u32 i;
    s32 record;

    Call3(Func_02007bfa, 0x40000, 0x40000, 0x10000);
    Func_02007db8(141);
    Func_02007c3e(80);
    Call1(Func_02007dc6, 0x120);
    Func_02007c4c(5);
    Func_02007dd2(145);
    Call6(Func_02007c0e, 16, 75, 7, 4, 26, 55);
    Call3(Func_02007c3c, -1, -1, 0xe666);
    Call3(Func_02007d70, 0, 0x100, 0);
    Call3(Func_02007d7c, 1, 0x100, 0);
    Call3(Func_02007d88, 2, 0x100, 0);
    Call3(Func_02007d94, 3, 0x100, 0);
    Value3(Func_02007da0, 12, 0x100, 0);
    Func_02007cb6(60);
}

/* map/locations/heidia/village/supplemental_scene.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
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
void Func_020075de_a();
void Func_020075de_b();
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
void Func_0200764c_a();
void Func_0200764c_b();
void Func_02007650();
void Func_02007656();
void Func_02007662_a();
void Func_02007662_b();
void Func_02007662_c();
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

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Object_SetModeById_1(args...) Func_02007544(args)
#define Object_SetModeById_2(args...) Func_0200754c(args)
#define Object_SetModeById_3(args...) Func_02007554(args)
#define Object_SetModeById_4(args...) Func_0200755c(args)
#define Audio_PlayCue_1(args...) Func_02007662_a(args)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_020075de_a, a0, a1, a2)
#define Battle_WaitMode0_1(args...) Func_020074f4(args)
#define Motion_SetAngleToward_1(args...) Func_020075a6(args)
#define SceneWork_SetStepValue_1(args...) Func_020075c6(args)
#define BattleEv_RunWait_1(args...) Func_020075de_b(args)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_0200760a, a0, a1, a2)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_02007616, a0, a1, a2)
#define Motion_SetAngleToward_2(args...) Func_020075d8(args)
#define Motion_SetAngleToward_3(args...) Func_020075e2(args)
#define Motion_SetAngleToward_4(args...) Func_020075ec(args)
#define Battle_WaitMode0_2(args...) Func_0200754a(args)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02007636, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02007642, a0, a1, a2)
#define SceneWork_SetStepValue_2(args...) Func_02007628(args)
#define BattleEv_RunWait_2(args...) Func_02007640(args)
#define Motion_CallWaitAnim_1(args...) Func_02007600(args)
#define SceneWork_SetStepValue_3(args...) Func_0200763e(args)
#define BattleEv_RunWait_3(args...) Func_02007656(args)
#define SceneWork_SetStepValue_4(args...) Func_0200764c_a(args)
#define BattleEv_RunWait_4(args...) Func_02007664(args)
#define Motion_SetVarCbObj_1(args...) Func_0200763c(args)
#define SceneWork_SetStepValue_5(args...) Func_02007662_b(args)
#define BattleEv_RunWait_5(args...) Func_0200767a(args)
#define Motion_CallWaitAnim_2(args...) Func_0200763a(args)
#define SceneWork_SetStepValue_6(args...) Func_02007678(args)
#define BattleEv_RunWait_6(args...) Func_02007690(args)
#define Motion_CallWaitAnim_3(args...) Func_02007650(args)
#define Battle_WaitMode0_3(args...) Func_020075ce(args)
#define Motion_SetAngleToward_5(args...) Func_02007680(args)
#define Motion_SetAngleToward_6(args...) Func_0200768a(args)
#define Battle_WaitMode0_4(args...) Func_020075e8(args)
#define Motion_ResetPosMode2_1(a0, a1, a2) Call3(Func_0200764c_b, a0, a1, a2)
#define Motion_CommitPos_1(args...) Func_0200766a(args)
#define Motion_SetAngleToward_7(args...) Func_020076ac(args)
#define Motion_CallWaitAnim_4(args...) Func_02007694(args)
#define Battle_WaitMode0_5(args...) Func_02007614(args)
#define SceneWork_SetStepValue_7(args...) Func_020076da(args)
#define BattleEv_RunWait_7(args...) Func_020076f2(args)
#define SharedWorkData_SetFirstAndSecondFields_1(args...) Func_02007778(args)
#define BattleFx_ComputeWeightedResultAndDispatch_1(args...) Func_02007782(args)
#define Motion_SetHPosTerrain_1(args...) Func_020076cc(args)
#define Motion_SetHPosTerrain_2(args...) Func_020076d6(args)
#define Motion_SetHPosTerrain_3(args...) Func_020076e0(args)
#define Battle_SchedShoulder_1(args...) Func_0200767c(args)
#define GameFlag_Set_1(a0) Call1(Func_02007662_c, a0)

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at Data_00002438 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at Data_02000240
 * before handing off to the next step. */
void Scene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;
    s32 sequence_2438;
    s32 status_record_2000240;

    Object_SetModeById_1(0, 1);
    Object_SetModeById_2(12, 1);
    Object_SetModeById_3(13, 1);
    Object_SetModeById_4(14, 1);
    Audio_PlayCue_1(113);
    BattleFx_SpawnLinked_1(12, 0x100, 0);
    Battle_WaitMode0_1(30);
    Motion_SetAngleToward_1(12, 0, 0);
    sequence_2438 = (s32)Data_00002438;
    SceneWork_SetStepValue_1(sequence_2438);
    BattleEv_RunWait_1(12, 0);
    BattleFx_SpawnLinked_2(13, 0x100, 0);
    BattleFx_SpawnLinked_3(14, 0x100, 0);
    Motion_SetAngleToward_2(13, 0, 0);
    Motion_SetAngleToward_3(14, 0, 0);
    Motion_SetAngleToward_4(0, 13, 0);
    Battle_WaitMode0_2(65);
    Motion_ArmCb_1(13, 0x5000, 0);
    Motion_ArmCb_2(14, 0xd000, 0);
    SceneWork_SetStepValue_2((sequence_2438 + 1));
    BattleEv_RunWait_2(13, 0);
    Motion_CallWaitAnim_1(14, 3);
    SceneWork_SetStepValue_3((sequence_2438 + 2));
    BattleEv_RunWait_3(14, 0);
    SceneWork_SetStepValue_4((sequence_2438 + 3));
    BattleEv_RunWait_4(12, 0);
    Motion_SetVarCbObj_1(13, 1);
    SceneWork_SetStepValue_5((sequence_2438 + 4));
    BattleEv_RunWait_5(13, 0);
    Motion_CallWaitAnim_2(14, 3);
    SceneWork_SetStepValue_6((sequence_2438 + 5));
    BattleEv_RunWait_6(14, 0);
    Motion_CallWaitAnim_3(14, 3);
    Battle_WaitMode0_3(60);
    Motion_SetAngleToward_5(13, 0, 0);
    Motion_SetAngleToward_6(14, 0, 0);
    Battle_WaitMode0_4(70);
    Motion_ResetPosMode2_1(12, 0x2a0, 88); /* object_id 12, x 0x2a0, z 88 */
    Motion_CommitPos_1(12);
    Motion_SetAngleToward_7(12, 0, 0);
    Motion_CallWaitAnim_4(12, 3);
    Battle_WaitMode0_5(30);
    SceneWork_SetStepValue_7((sequence_2438 + 6));
    BattleEv_RunWait_7(12, 0);
    SCENE_PHASE = 0x200;
    SharedWorkData_SetFirstAndSecondFields_1((s32)Data_000000a1, 31);
    status_record_2000240 = (s32)Data_02000240;
    /* Status byte at +0x22b of the record. */
    *(u8 *)((status_record_2000240 + 0x22b)) = 3;
    BattleFx_ComputeWeightedResultAndDispatch_1(98, 3);
    Motion_SetHPosTerrain_1(12, 0, 0);
    Motion_SetHPosTerrain_2(13, 0, 0);
    Motion_SetHPosTerrain_3(14, 0, 0);
    Battle_SchedShoulder_1();
    GameFlag_Set_1(0x94a); /* main:080770c8 */
}
#undef Object_SetModeById_1
#undef Object_SetModeById_2
#undef Object_SetModeById_3
#undef Object_SetModeById_4
#undef Audio_PlayCue_1
#undef BattleFx_SpawnLinked_1
#undef Battle_WaitMode0_1
#undef Motion_SetAngleToward_1
#undef SceneWork_SetStepValue_1
#undef BattleEv_RunWait_1
#undef BattleFx_SpawnLinked_2
#undef BattleFx_SpawnLinked_3
#undef Motion_SetAngleToward_2
#undef Motion_SetAngleToward_3
#undef Motion_SetAngleToward_4
#undef Battle_WaitMode0_2
#undef Motion_ArmCb_1
#undef Motion_ArmCb_2
#undef SceneWork_SetStepValue_2
#undef BattleEv_RunWait_2
#undef Motion_CallWaitAnim_1
#undef SceneWork_SetStepValue_3
#undef BattleEv_RunWait_3
#undef SceneWork_SetStepValue_4
#undef BattleEv_RunWait_4
#undef Motion_SetVarCbObj_1
#undef SceneWork_SetStepValue_5
#undef BattleEv_RunWait_5
#undef Motion_CallWaitAnim_2
#undef SceneWork_SetStepValue_6
#undef BattleEv_RunWait_6
#undef Motion_CallWaitAnim_3
#undef Battle_WaitMode0_3
#undef Motion_SetAngleToward_5
#undef Motion_SetAngleToward_6
#undef Battle_WaitMode0_4
#undef Motion_ResetPosMode2_1
#undef Motion_CommitPos_1
#undef Motion_SetAngleToward_7
#undef Motion_CallWaitAnim_4
#undef Battle_WaitMode0_5
#undef SceneWork_SetStepValue_7
#undef BattleEv_RunWait_7
#undef SharedWorkData_SetFirstAndSecondFields_1
#undef BattleFx_ComputeWeightedResultAndDispatch_1
#undef Motion_SetHPosTerrain_1
#undef Motion_SetHPosTerrain_2
#undef Motion_SetHPosTerrain_3
#undef Battle_SchedShoulder_1
#undef GameFlag_Set_1
#undef SCENE_PHASE
