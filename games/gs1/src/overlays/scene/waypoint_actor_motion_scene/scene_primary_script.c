#include "types.h"

#define FieldScene_RunScene39f_02000d90 Func_02000d90
#define FieldScene_RunScriptedSteps0And17E6 Func_0200112c
#define FieldScene_RunActor10WaypointSequence Func_02001150
#define FieldScene_RunActorElevenPresentationBeat Func_02001244
#define FieldScene_RunStepFD4WithActor181 Func_02001420
#define FieldScene_RunProbedActorEightOrTenScene Func_02001454
#define FieldScene_RunActorThirteenPresentationBeat Func_020016f0
#define FieldScene_RunScene39f_02001818 Func_02001818
#define FieldScene_RunSupplementalSequenceOne Func_02001c34
#define FieldScene_RunScene39fSequenceA Func_02001d04
#define FieldScene_RunSlot16WaypointSequence Func_02001de0
#define FieldScene_RunActor17CameraSequence Func_02001ef0
#define FieldScene_RunScene39f_02002004 Func_02002004
#define FieldScene_RunActorEighteenEffectSequence Func_02002078
#define FieldScene_RunScene39f_020021b0 Func_020021b0
/* The probe record is passed on by value: four words in r0-r3 and the last two
 * copied to the outgoing stack area by an `ldmia`/`stmia` pair. */
struct Resource39fProbe {
    s32 word[6];
};

extern u8 Data_03001ebc[];

void Func_02003b36();
void Func_02003b5e();
s32 Func_02003b62();
void Func_02003b82();
void Func_02003bb8();
void Func_02003bec();
void Func_02003bee();
void Func_02003c78();
void Func_02003ed4(void);
void Func_02003f34(s32 a, s32 b);
void Func_02003ebc(s32 a, s32 b);
void Func_02003ef0(void);
u8 *Func_02003f14(s32);
void Func_02003f02(void);
void Func_02001c7a(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02003fe6(void);
void Func_0200200e(s32, s32);
void Func_02001f04(s32, s32, s32, s32);
void Func_02003fee(s32, s32);
void Func_02003fc0(s32, s32, s32);
void Func_02003f46(s32);
void Func_02003fb6_b(s32, s32);
void Func_02003fb6_c(s32);
void Func_0200400a(s32, s32);
void Func_02003f62(s32);
void Func_02001f62(s32, s32, s32, s32);
void Func_02003ff0(s32, s32, s32);
void Func_02003f7e(s32);
void Func_02001f7e(s32, s32, s32, s32);
void Func_0200400c(s32, s32, s32);
void Func_02003f9a(s32);
void Func_02001f9a(s32, s32, s32, s32);
void Func_02004028(s32, s32, s32);
void Func_02003fb6_a(s32);
void Func_02004020(s32, s32, s32);
void Func_0200402a(s32, s32, s32);
u8 *Func_02004008();
void Func_02003ff6();
void Func_02002102();
void Func_02001ffc();
void Func_02001d72();
void Func_020040e6();
void Func_020040b8_a();
void Func_020040b8_b();
void Func_0200403e();
void Func_020040ae();
void Func_020040f8();
void Func_0200416e();
void Func_0200405c();
u8 *Func_02004082_a();
u8 *Func_0200408c();
void Func_02002070();
void Func_02004082_b();
void Func_02004080();
void Func_020040ea();
void Func_02004184();
void Func_020041c8_a(s32 a);
void Func_020041c8_b(s32 a);
s32 Func_02004222(s32 a, s32 b, s32 c);
void Func_020042e0(s32 a, s32 b);
s32 Func_020041f8(s32 a, s32 b);
void Func_020041f4(void);
void Func_020041fe(void);
s32 Func_020018d6(struct Resource39fProbe *);
void Func_02001a82(struct Resource39fProbe);
void Func_020041e8(s32, s32, s32, s32, s32, s32);
void Func_02004246_a(s32);
void Func_020042b6(s32, s32);
void Func_020042aa(s32, s32, s32);
void Func_02004268(s32);
void Func_020042d0(s32, s32);
void Func_0200438e(s32);
u8 *Func_0200429c(s32);
void Func_02004246_b(s32, s32, s32, s32, s32, s32);
s32 Func_02001752(s32, s32, s32, s32, s32, s32);
void Func_020042be(void);
u8 *Func_020044b4(s32);
void Func_020044a2(void);
void Func_020025ae(s32, s32);
void Func_020024a6(s32, s32, s32, s32);
void Func_020044b8(s32);
void Func_02002222(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004596(s32, s32);
void Func_02004568(s32, s32, s32);
void Func_020044ee(s32);
void Func_0200455e(s32, s32);
void Func_020045b0(s32, s32);
void Func_02004506(s32);
void Func_0200250a(s32, s32, s32, s32);
void Func_02004598(s32, s32, s32);
void Func_02004526(s32);
void Func_02002528(s32, s32, s32, s32);
void Func_020045b6(s32, s32, s32);
void Func_02004546(s32);
void Func_0200254a(s32, s32, s32, s32);
void Func_020045d8(s32, s32, s32);
void Func_02004566(s32);
void Func_02002568(s32, s32, s32, s32);
void Func_020045f6(s32, s32, s32);
void Func_02004584(s32);
void Func_020045e6(s32, s32, s32);
void Func_0200458e(s32);
void Func_020045f8(s32, s32, s32);
void Func_020045b4(void);
void Func_020025c4();
void Func_02002658();
void Func_020026cc();
void Func_020045b0_a();
void Func_020045c0();
void Func_020045d6();
void Func_020045f4();
void Func_020045f6_a();
s32 Func_0200460a();
void Func_0200461e();
void Func_02004662();
void Func_0200466c();
s32 Func_02001eb0();
void Func_02001ec2();
void Func_02001ed2();
s32 Func_02001ee2();
s32 Func_02001ef4();
void Func_02001f30();
void Func_02002834();
void Func_02002abc();
void Func_02002b32();
void Func_02002bc4();
void Func_020049e4();
s32 Func_02004a02();
s32 Func_02004a0a();
void Func_02004a6c();
s32 Func_02004a72();
s32 Func_02004a80();
void Func_02004a9c();
void Func_02004ab8();
s32 Func_02004ac8();
void Func_02004ace();
void Func_02004b00();
void Func_02004b1e();
void Func_02004b42();
u8 *Func_02004b44();
void Func_02004b70();
void Func_02004b7a();
void Func_02004ba8();
void Func_02004bba();
void Func_02004c30();
void Func_02004c3c();
s32 Func_02004b44_a();
s32 Func_02004b44_b();
void Func_02004b70_a();
void Func_02004b70_b();
u8 *Func_02004ba4(s32);
void Func_02004b92(void);
void Func_02002c9e(s32, s32);
void Func_02002b96(s32, s32, s32, s32);
void Func_02004ba8_a(s32);
void Func_02002912(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004c86(s32, s32);
void Func_02004c58(s32, s32, s32);
void Func_02004bde(s32);
void Func_02004c4e(s32, s32);
void Func_02004ca0(s32, s32);
void Func_02004bf8(s32);
void Func_02002bfc(s32, s32, s32, s32);
void Func_02004c8c(s32, s32, s32);
void Func_02004c1c(s32);
void Func_02002c1c(s32, s32, s32, s32);
void Func_02004caa(s32, s32, s32);
void Func_02004c38(s32);
void Func_02002c38(s32, s32, s32, s32);
void Func_02004cc6(s32, s32, s32);
void Func_02004c54(s32);
void Func_02004d14(s32, s32);
void Func_02004cbe(s32, s32, s32);
void Func_02004c6c_a(s32);
void Func_02004c6c_b(s32);
void Func_02004cd6(s32, s32, s32);
void Func_02004c92(void);
u8 *Func_02004cb4(s32);
void Func_02004ca4(void);
void Func_02002db0(s32, s32);
void Func_02002ca8(s32, s32, s32, s32);
void Func_02004cba(s32);
void Func_02002a24(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004d98_a(s32, s32);
void Func_02004d6a(s32, s32, s32);
void Func_02004cf0(s32);
void Func_02004d60(s32, s32);
void Func_02004db2(s32, s32);
void Func_02004d08(s32);
void Func_02002d0a(s32, s32, s32, s32);
void Func_02004d98_b(s32, s32, s32);
void Func_02004d28(s32);
void Func_02002d2c(s32, s32, s32, s32);
void Func_02004dba(s32, s32, s32);
void Func_02004d48(s32);
void Func_02002d4a(s32, s32, s32, s32);
void Func_02004dd8(s32, s32, s32);
void Func_02004d66(s32);
void Func_02004e26(s32, s32);
void Func_02004dd0(s32, s32, s32);
void Func_02004d7e(s32);
void Func_02004d7c(s32);
void Func_02004de6(s32, s32, s32);
void Func_02004da2(void);
void Func_02002dc4();
void Func_02002e52();
void Func_02002eb8();
void Func_02004daa();
void Func_02004dac();
void Func_02004dee();
void Func_02004df0();
s32 Func_02004e04();
void Func_02004e18();
void Func_02004e5c();
void Func_02004e66();
void Func_02004e86();
u8 *Func_02004e40(s32);
void Func_02004e30(void);
void Func_02002f3c(s32, s32);
void Func_02002e38(s32, s32, s32, s32);
void Func_02004e4a(s32);
void Func_02002bb4(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004f28_a(s32, s32);
void Func_02004efa(s32, s32, s32);
void Func_02004e80(s32);
void Func_02004ef0(s32, s32);
void Func_02004f42(s32, s32);
void Func_02004e98(s32);
void Func_02002e9a(s32, s32, s32, s32);
void Func_02004f28_b(s32, s32, s32);
void Func_02004eb8(s32);
void Func_02002ebc(s32, s32, s32, s32);
void Func_02004f4a(s32, s32, s32);
void Func_02004ed8(s32);
void Func_02002edc(s32, s32, s32, s32);
void Func_02004f6a(s32, s32, s32);
void Func_02004ef8(s32);
void Func_02002efa(s32, s32, s32, s32);
void Func_02004f88(s32, s32, s32);
void Func_02004f16(s32);
void Func_02004fd6(s32, s32);
void Func_02004f80(s32, s32, s32);
void Func_02004f2e(s32);
void Func_02004f2c(s32);
void Func_02004f48(void);
void Func_02002cf0();
void Func_02002f74();
void Func_02002fce();
void Func_02002ff0();
void Func_0200300e();
void Func_0200307c();
void Func_02004f62();
s32 Func_02004f74();
void Func_02004f86();
void Func_02004fc0();
void Func_02004fec();
void Func_0200500c();
void Func_0200502a();
void Func_02005032();
void Func_02005040();
void Func_02005042();
void Func_02005048();
void Func_0200505c_a();
void Func_0200505c_b();
void Func_0200506a();
void Func_0200507e();
void Func_02005082();
void Func_02005094();
void Func_0200509c();
void Func_020050ea();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* 0x02003fb6 serves the two-argument presentation select and, twice later,
 * one-argument imports; the `_b` alias carries the two-argument one. */

/* An EWRAM global, not overlay data: the module is linked above this address,
 * so it cannot be in-image. */

/* Returns a slot record.  Its fields are named by position and not verified:
 * +8/+12/+16 read as the position and +10/+18 as signed halfwords. */

/* Four register arguments plus four stack words. */

/* 0x02004246 serves two imports: the one-argument post at site 5 and the
 * six-argument message at site 12. */

/* 0x02004d98 serves two imports: the two-argument setter at site 7 and the
 * three-argument one at site 14. */

/* 0x02004f28 serves two imports: the two-argument setter at site 7 and the
 * three-argument step at site 14. */

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_02001818(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    return f(a0);
}

static __inline__ void Call3_02001c34(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ void Call3_02001d04(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    s32 Func_02004b4e();

    f(a0, a1, a2);
}

static __inline__ void Call3_02002004(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3_020021b0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 rec7;
    s32 record;

    p10 = a2;
    p9 = a3;
    p8 = a1;
    rec7 = Func_02003b62();
    Func_02003bec(a0, 1);
    Call3(Func_02003b82, a0, 0x30000, 0x18000);
    Func_02003c78(152);
    *(volatile s32 *)(rec7 + 40) = p9;
    *(volatile s32 *)(rec7 + 72) = 0x8000;
    *(volatile s32 *)(rec7 + 68) = 0;
    Func_02003b36(rec7, 0);
    Func_02003bb8(a0, p8, p10);
    p8b = ((s32)p8 << 16);
    p10b = ((s32)p10 << 16);
    Func_02003bee(a0, p8b, p10b);
    Func_02003b5e(rec7, 1);
    *(volatile s32 *)(rec7 + 72) = 0x10000;
    p10c = a0;
}

void FieldScene_RunScriptedSteps0And17E6(void)
{
    Func_02003ed4();
    Func_02003f34(0, 1);
    Func_02003ebc(6118, 1);
    Func_02003ef0();
}

void FieldScene_RunActor10WaypointSequence(void)
{
    u8 *slot;

    slot = Func_02003f14(10);

    /* r0 still holds the record returned above. */
    Func_02003f02();

    Func_0200200e(10, 1);
    Func_02001f04(10, 88, 120, 0x60000);        /* 192 << 11 */

    Func_02001c7a(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_02003fee(10, 1);
    Func_02003fc0(10, 0, 0);
    Func_02003f46(20);
    Func_02003fb6_b(10, 2);
    Func_0200400a(10, 258);                     /* 129 << 1 */
    Func_02003f62(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    Func_02001f62(10, 88, 152, 0x30000);
    Func_02003ff0(0, 10, 0);
    Func_02003f7e(10);

    Func_02001f7e(10, 120, 192, 0x30000);
    Func_0200400c(0, 10, 0);
    Func_02003f9a(10);

    Func_02001f9a(10, 120, 240, 0x30000);
    Func_02004028(0, 10, 0);
    Func_02003fb6_a(10);

    Func_02003fb6_c(768);                       /* 192 << 2 */
    Func_02004020(13, 0, 0);
    Func_0200402a(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02003fe6();
}

/*
 * A full cutscene beat for slot 11: opens the slot, places it at (408, 456),
 * publishes an eight-argument piece, runs the presentation, then re-places the
 * slot on the party's current heading readings and sets the engine byte at
 * Data_02000240 + 0x22b to 3.  The 228-byte owner includes an alignment
 * halfword and its four pool words.
 */
void FieldScene_RunActorElevenPresentationBeat(void)
{
    extern s16 Data_02000240[];

    u8 *slot;
    s32 offset;

    slot = Func_02004008(11);

    /* Reads the record left in r0 by the call above; it must not be respelled
     * as a fresh fetch. */
    Func_02003ff6();

    Func_02002102(11, 0);
    Func_02001ffc(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Func_02001d72(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_020040e6(11, 1);
    Func_020040b8_a(11, 0, 0);
    Func_0200403e(30);
    Func_020040ae(11, 2);
    Func_020040f8(11, 0x103, 0);
    Func_0200416e(147);
    Func_0200405c(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    Func_02002070(11,
                  *(s16 *)(Func_02004082_a(0) + 10),
                  *(s16 *)(Func_0200408c(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Func_02004082_b(10);
    Func_02004080(0x301);
    Func_020040ea(14, 0, 0);

    offset = 0x22b;
    ((u8 *)Data_02000240)[offset] = 3;

    Func_02004184(53, 0);

    /* Common exit; no argument registers are set. */
    Func_020040b8_b();
}

void FieldScene_RunStepFD4WithActor181(s32 a)
{
    Func_020041c8_a(a);
    Func_02004222(16, 0, 0);
    Func_020041c8_b(4052);
    Func_020042e0(181, 3);
    Func_020041f8(181, 0);
    Func_020041f4();
}

void FieldScene_RunProbedActorEightOrTenScene(void)
{
    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;
    s32 height;
    s32 value;

    /* No argument register is written before this branch. */
    Func_020041fe();

    if (Func_020018d6(&probe) != 0) {
        Func_02001a82(probe);

        if (probe.word[1] == 8 && (probe.word[4] >> 20) == 23) {
            fifth = 35;
            sixth = 68;
            Func_020041e8(35, 67, 4, 1, fifth, sixth);
        } else if (probe.word[1] == 10 && (probe.word[2] >> 20) == 35) {
            /* Written here, not at the call: the reference keeps it in a
             * callee-saved register across the whole sequence. */
            value = 0;
            Func_02004246_a(0x311);
            Func_020042b6(10, 3);
            Func_020042aa(10, -16, 6);
            Func_02004268(30);
            Func_020042d0(10, 8);
            Func_0200438e(240);

            Func_0200429c(10)[35] = 2;

            fifth = 34;
            sixth = 30;
            Func_02004246_b(44, 30, 2, 4, fifth, sixth);
            height = 4;
            Func_02001752(2, 35, 30, 1, height, value);
        }
    }

    /* Common exit; no argument registers are set. */
    Func_020042be();
}

void FieldScene_RunActorThirteenPresentationBeat(void)
{
    u8 *slot;

    slot = Func_020044b4(13);

    /* r0 still holds the record returned above. */
    Func_020044a2();

    Func_020025ae(13, 1);
    Func_020024a6(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Func_020044b8(10);

    Func_02002222(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004596(13, 1);
    Func_02004568(13, 0, 0);
    Func_020044ee(20);
    Func_0200455e(13, 2);
    Func_020045b0(13, 258);                     /* 129 << 1 */
    Func_02004506(60);

    Func_0200250a(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Func_02004598(0, 13, 0);
    Func_02004526(6);

    Func_02002528(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Func_020045b6(0, 13, 0);
    Func_02004546(6);

    Func_0200254a(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Func_020045d8(0, 13, 0);
    Func_02004566(6);

    Func_02002568(13, 584, 136, 0x38000);       /* 146 << 2 */
    Func_020045f6(0, 13, 0);
    Func_02004584(6);

    Func_020045e6(13, 0, 0);
    Func_0200458e(772);                         /* 193 << 2 */
    Func_020045f8(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_020045b4();
}

void FieldScene_RunScene39f_02001818(void)
{
    u32 i;
    s32 record;

    Func_020045c0();
    Func_020026cc(14, 1);
    Call4(Func_020025c4, 14, 0x1a8, 0x1e0, 0x79999);
    Func_020045d6(2);
    Func_02002658(14);
    Func_0200466c(14, 15);
    record = Func_0200460a(14);
    Func_020045b0_a(record, 0);
    Func_020045f6_a(30);
    Call1(Func_020045f4, 0x305);
    Call3_02001818(Func_02004662, 17, 0x1a80000, 0x1e00000);
    Func_0200461e();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    extern u8 Data_02000240[];
    u8 *Func_02004b4e();

    s32 one;
    s32 x;
    s32 y;
    u8 *record;

    Func_020049e4();
    record = Value1(Func_02004a02, 14);
    x = *(volatile s32 *)((s32)record + 8);
    record = Value1(Func_02004a0a, 14);
    y = *(volatile s32 *)((s32)record + 16);
    x >>= 20;
    y >>= 20;
    one = 1;
    Func_02001eb0(2, x, y, 1, one, 255);
    Call6(Func_02001ec2, 2, x + 1, y, 1, one, 0);
    Call6(Func_02001ed2, 2, x - 1, y, 1, one, 0);
    Func_02001ee2(2, x, y + 1, 1, one, 0);
    Func_02001ef4(2, x, y - 1, 1, one, 0);
    record = Value1(Func_02004a72, 14);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) == 27) {
        record = Value1(Func_02004a80, 14);
        record[85] = 0;
        *(volatile s32 *)((s32)record + 20) = -0x20000;
        *(volatile s32 *)((s32)record + 12) = -0x20000;
        Call1(Func_02004a6c, 0x214);
        Call6(Func_02001f30, 2, 43, 23, 1, one, 255);
    }
    Func_02004a9c();
}

/*
 * Actor presentation beat for overlay resource_39f.  The twin at 0x02001d04
 * is the same beat for slot 15.
 */
void FieldScene_RunScene39fSequenceA(void)
{
    extern u8 Data_02000240[];
    s32 Func_02004b4e();

    s32 rec7;
    s32 big;
    s32 first;
    s32 shown;
    s32 second;
    s32 base3_2000240;

    rec7 = Func_02004ac8(15);
    big = 0x80000;
    Func_02004ab8();
    Func_02002bc4(15, 0);
    Call4(Func_02002abc, 15, 0x1d8, 104, big);
    Func_02004ace(10);
    Func_02002834(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + big), 0, 0, 0, 1, 0);
    Func_02004ba8(15, 1);
    Func_02004b7a(15, 0, 0);
    Func_02004b00(30);
    Func_02004b70_a(15, 2);
    Call3_02001d04(Func_02004bba, 15, 0x103, 0);
    Func_02004c30(147);
    Func_02004b1e(60);
    first = Func_02004b44_a(0);
    shown = *(s16 *)(first + 10);
    second = Func_02004b4e(0);
    Call4(Func_02002b32, 15, shown, *(s16 *)(second + 18), 0x60000);
    ((s64 (*)())Func_02004b44_b)(10);
    Call1(Func_02004b42, 0x307);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02004c3c(53, 0);
    Func_02004b70_b();
}

void FieldScene_RunSlot16WaypointSequence(void)
{
    u8 *slot;

    slot = Func_02004ba4(16);

    /* r0 still holds the record returned above. */
    Func_02004b92();

    Func_02002c9e(16, 1);
    Func_02002b96(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Func_02004ba8_a(10);

    Func_02002912(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004c86(16, 1);
    Func_02004c58(16, 0, 0);
    Func_02004bde(20);
    Func_02004c4e(16, 2);
    Func_02004ca0(16, 258);                     /* 129 << 1 */
    Func_02004bf8(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    Func_02002bfc(16, 448, 192, 0x30000);       /* 224 << 1 */
    Func_02004c8c(0, 16, 0);
    Func_02004c1c(6);

    Func_02002c1c(16, 424, 208, 0x30000);       /* 212 << 1 */
    Func_02004caa(0, 16, 0);
    Func_02004c38(6);

    Func_02002c38(16, 424, 224, 0x30000);
    Func_02004cc6(0, 16, 0);
    Func_02004c54(6);

    Func_02004d14(0, 1);
    Func_02004cbe(16, 0, 0);
    Func_02004c6c_a(30);
    Func_02004c6c_b(776);                         /* 194 << 2 */
    Func_02004cd6(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004c92();
}

void FieldScene_RunActor17CameraSequence(void)
{
    u8 *slot;

    slot = Func_02004cb4(17);

    /* r0 still holds the record returned above. */
    Func_02004ca4();

    Func_02002db0(17, 1);
    Func_02002ca8(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Func_02004cba(10);

    Func_02002a24(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004d98_a(17, 1);
    Func_02004d6a(17, 0, 0);
    Func_02004cf0(20);
    Func_02004d60(17, 2);
    Func_02004db2(17, 258);                     /* 129 << 1 */
    Func_02004d08(60);

    Func_02002d0a(17, 376, 152, 0x60000);       /* 188 << 1 */
    Func_02004d98_b(0, 17, 0);
    Func_02004d28(10);

    Func_02002d2c(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Func_02004dba(0, 17, 0);
    Func_02004d48(6);

    Func_02002d4a(17, 296, 160, 0x30000);       /* 148 << 1 */
    Func_02004dd8(0, 17, 0);
    Func_02004d66(6);

    Func_02004e26(0, 1);
    Func_02004dd0(17, 0, 0);
    Func_02004d7e(30);
    Func_02004d7c(0x309);
    Func_02004de6(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004da2();
}

void FieldScene_RunScene39f_02002004(void)
{
    u32 i;
    s32 record;

    Func_02004dac();
    Func_02002eb8(18, 1);
    Call4(Func_02004e86, 0x2e80000, -1, 0x1f80000, 1);
    Call4(Func_02002dc4, 18, 0x2e8, 0x1f8, 0x90000);
    Func_02002e52(18);
    Func_02004e66(18, 15);
    record = Func_02004e04(18);
    Func_02004daa(record, 0);
    Func_02004df0(30);
    Call1(Func_02004dee, 0x30a);
    Call3_02002004(Func_02004e5c, 22, 0x2e80000, 0x1f80000);
    Func_02004e18();
}

void FieldScene_RunActorEighteenEffectSequence(void)
{
    u8 *slot;

    slot = Func_02004e40(18);

    /* r0 still holds the record returned above. */
    Func_02004e30();

    Func_02002f3c(18, 1);
    Func_02002e38(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Func_02004e4a(10);

    Func_02002bb4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004f28_a(18, 1);
    Func_02004efa(18, 0, 0);
    Func_02004e80(20);
    Func_02004ef0(18, 2);
    Func_02004f42(18, 258);                     /* 129 << 1 */
    Func_02004e98(60);

    Func_02002e9a(18, 712, 568, 0x60000);       /* 142 << 2 */
    Func_02004f28_b(0, 18, 0);
    Func_02004eb8(10);

    Func_02002ebc(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Func_02004f4a(0, 18, 0);
    Func_02004ed8(6);

    Func_02002edc(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Func_02004f6a(0, 18, 0);
    Func_02004ef8(6);

    Func_02002efa(18, 736, 704, 0x30000);       /* 176 << 2 */
    Func_02004f88(0, 18, 0);
    Func_02004f16(6);

    Func_02004fd6(0, 1);
    Func_02004f80(18, 0, 0);
    Func_02004f2e(30);
    Func_02004f2c(0x30b);

    /* Common exit; no argument registers are set. */
    Func_02004f48();
}

void FieldScene_RunScene39f_020021b0(void)
{
    s32 rec7;

    rec7 = Value1(Func_02004f74, 18);
    Func_02004f62();
    Call3_020021b0(Func_02004fc0, 18, 0x880000, 0x1680000);
    Func_0200307c(18, 1);
    Func_02002f74(18, 136, 0x198, 0x80000);
    Func_02004f86(10);
    Func_02002cf0(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Call3_020021b0(Func_02005048, 18, 0xc000, 40);
    Call2(Func_0200506a, 18, 0x102);
    Func_02005032(18, 2);
    Func_02005082(18, 1);
    Func_02002fce(18, 136, 0x1b8, 0x60000);
    Func_0200505c_a(0, 18, 0);
    Func_02004fec(10);
    Func_02002ff0(18, 136, 0x1d8, 0x30000);
    Func_0200507e(0, 18, 0);
    Func_0200500c(6);
    Func_0200300e(18, 136, 0x1f8, 0x30000);
    Func_0200509c(0, 18, 0);
    Func_0200502a(6);
    Func_020050ea(0, 1);
    Func_02005094(18, 0, 0);
    Func_02005042(60);
    Call1(Func_02005040, 0x89d);
    Func_0200505c_b();
}
