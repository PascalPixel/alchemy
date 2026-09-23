#include "TYPES.H"


/*
 * Exact 2026-09-23 (832 bytes), a tagged fake match. The callback passed
 * to the target-and-callback call (main Object_SetTargetAndCallback) is the
 * data symbol Data_0200ac00, as in HAIDIA_IE/ELDER_AID_EVENT.C; as a plain
 * integer its pool load was scheduled before the zero actor argument.
 * The do/while around the 0x22b area-state store holds the linked scene
 * load after the store, as in the reference.
 */

struct SceneWork {
    u8 pad000[0x22b];
    u8 area_state;
};

extern const u8 Data_00000005[];
extern u8 Data_0200ac00[];
extern struct SceneWork Data_02000240;
void Func_020035c2();
s32 Func_02003600();
s32 Func_020036de();
void Func_020036fc();
s32 Func_02003704();
void Func_02003722();
s32 Func_02003732();
s32 Func_02003754();
s32 Func_02003778();
void Func_02003792();
s32 Func_0200379e();
s32 Func_0200379e_a();
void Func_02003888();
void Func_020038e0();
s32 Func_0200394a();
void Func_0200399c();
void Func_020039aa();
void Func_020039d4();
void Func_020039da();
void Func_020039f8();
void Func_02003a06();
void Func_02003a12();
void Func_02003a18();
void Func_02003a20();
void Func_02003a4c();
void Func_02003a58();
void Func_02003a5a();
void Func_02003a5e();
void Func_02003a6a();
void Func_02003a6a_a();
s32 Func_02003a70();
void Func_02003a80();
void Func_02003a92();
void Func_02003a98();
void Func_02003a98_a();
void Func_02003a9a();
void Func_02003aa0();
void Func_02003aa2();
void Func_02003ab0();
void Func_02003ab2();
void Func_02003abe();
void Func_02003ac2();
void Func_02003ad0();
void Func_02003ad4();
void Func_02003ad4_a();
void Func_02003aec();
void Func_02003afa();
void Func_02003b02();
void Func_02003b12();
s32 Func_02003b2a();
void Func_02003b44();
void Func_02003b4c();
s32 Func_02003b54();
void Func_02003b68();
void Func_02003b84();
void Func_02003b90();
void Func_02003b92();
void Func_02003b98();
void Func_02003b9a(s32, s32, s32);
void Func_02003ba0();
void Func_02003ba6();
void Func_02003ba8();
void Func_02003bae();
void Func_02003bc4();
void Func_02003bca();
void Func_02003bcc();
void Func_02003bda();
void Func_02003be8();
void Func_02003bf2();
void Func_02003bf6();
void Func_02003c02();
s32 Func_02003c0e();
void Func_02003c1a();
void Func_02003c1c();
void Func_02003c84();
void Func_02003c8e();
s32 Func_02003d12(const void *, s32);
void Func_02003d1a();
void Func_02003d22(const void *, s32);

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Stages the two moving actors around actor 25, then advances the area's
 * scene state after the final message and sound cue. */
void FieldScene_RunGroupChoreography(void)
{
    s32 record;
    s32 walk_speed;
    s32 turn_speed;
    s32 approach_speed;

    Func_020039d4(25, 15);
    record = Func_0200394a(25);
    Func_020038e0(record, 0);
    Call3(Func_020039aa, 25, 0, 0x14b0000);
    Func_02003888(1);
    Call2(Func_02003a18, 0x1019, 0);
    Call3(Func_02003a4c, 23, 0x100, 0);
    Call3(Func_02003a58, 24, 0x100, 40);
    Func_020039da(25, 0, 0);
    Call3(Func_02003a5e, 23, 0x5000, 0);
    walk_speed = 0x5000;
    Func_020035c2(24, walk_speed, 40);
    Call2(Func_02003a9a, 0x18000, 0x3000);
    Call4(Func_02003ab2, 0x590000, 0xb00000, 0x1390000, 1);
    Func_02003abe();
    Func_0200399c(40);
    Call3(Func_02003aa0, 23, 0xe000, 0);
    Value3(Func_02003600, 24, 0x7000, 40);
    Call2(Func_02003ad4, 0xcccc, 0x1999);
    Call4(Func_02003aec, 0x640000, 0x900000, 0x14d0000, 1);
    Call3(Func_02003a12, 23, 0x10000, 0x8000);
    Call3(Func_02003a20, 24, 0x10000, 0x8000);
    Call3(Func_02003a5a, 23, 105, 0x149);
    Func_020039f8(10);
    Call3(Func_02003a6a, 24, 124, 0x149);
    Func_02003a80(23);
    Func_02003a98(23, 1);
    Func_02003b12(23, walk_speed, 0);
    Func_02003a98_a(24);
    Func_02003ab0(24, 1);
    Value3(Func_02003b2a, 24, walk_speed, 0);
    Func_02003afa(25, 0);
    record = Func_02003a70(25);
    Func_02003a06(record, 1);
    Call3(Func_02003ad0, 25, 0, 0x14b0000);
    Call3(Func_02003a92, 25, 0x13333, 0x9999);
    Call3(Func_02003ad4_a, 25, 37, 0x153);
    Func_02003a6a_a(20);
    Func_02003b02(23, 3);
    Value2(Func_02003b54, 23, 0);
    Call3(Func_02003b90, 25, 0x101, 0);
    approach_speed = 0xd000;
    Value3(Func_020036de, 0, approach_speed, 10);
    Func_02003ac2(0, 0);
    Func_02003aa2(40);
    Func_02003ba6(23, 0, 0);
    turn_speed = 0x8000;
    Value3(Func_02003704, 24, turn_speed, 20);
    Func_02003b68(25, 2);
    Call2(Func_020036fc, 0x1019, 10);
    Call3(Func_02003b9a, 0, 0x10019, (s32)Data_0200ac00);
    Call3(Func_02003b4c, 25, 93, 0x169);
    Value3(Func_02003732, 25, approach_speed, 40);
    Func_02003722(25, 20);
    Func_02003b44(0);
    Func_02003bf6(23, 0, 0);
    Value3(Func_02003754, 24, turn_speed, 15);
    Func_02003b98(23, 3);
    Func_02003ba8(24, 3);
    Func_02003c1a(23, walk_speed, 0);
    Value3(Func_02003778, 24, walk_speed, 30);
    Func_02003bc4(24, 4);
    Call2(Func_02003c1c, 0x2018, 0);
    Func_02003792(0, approach_speed, 30);
    Value3(Func_0200379e, 0, 0x4000, 40);
    Func_02003c02(23, 2);
    Func_02003bf2(23, 3);
    ((void (*)())Func_0200379e_a)(23, 20);
    Call2(Func_02003c84, 0, 0x102);
    Call2(Func_02003c8e, 25, 0x102);
    Func_02003b84(80);
    Call2(Func_02003bcc, 24, 0x200a8e8);
    Func_02003b92(6);
    Call2(Func_02003bda, 23, 0x200a940);
    Func_02003ba0(20);
    Call2(Func_02003be8, 0, 0x200a998);
    Func_02003bae(6);
    Value2(Func_02003c0e, 25, 0x200a9f0);
    /* FAKEMATCH: the do/while loads the linked scene after the store. */
    do {
        Data_02000240.area_state = 2;
    } while (0);
    {
        const void *message = Data_00000005;

        Func_02003d12(message, 19);
        Func_02003d22(message, 19);
    }
    Func_02003d1a(12, 4);
    Call1(Func_02003bca, 0x11a);
}
