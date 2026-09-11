#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define Battle_Reset_1() Call0(Func_02000bd6)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02000c04, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02000c12, a0, a1, a2)
#define Motion_ResetPosMode2_1(a0, a1, a2) Call3(Func_02000c2e, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02000c42, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02000cbe, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02000cca, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02000c82, a0, a1)
#define Battle_SchedShoulder_1() Call0(Func_02000c5c)
#define Scene_SetActor13Value1A Func_02000030
#define SceneData_GetPrimaryTable Func_02000040
#define SceneData_ReturnZero Func_02000080
#define SceneData_GetTable8B48 Func_02000084
#define SceneData_GetSecondaryTable Func_0200008c
#define SceneData_GetTertiaryTable Func_020000e4
#define Dialogue_ShowMessages8fbAnd8fc Func_020002f8
#define Scene_RunPrimarySequence Func_02000334
#define Scene_RunScene3c3SequenceA Func_020003c4
#define Scene_RunActorNinePromptDialogue Func_02000730
#define Scene_RunActorTenRepeatedMotion Func_020007ac

extern u8 Value_000000aa;
extern u8 Value_000000ab;
extern u8 Data_02008a40[];
extern u8 Data_02008ad0[];
extern u8 Data_02008998[];
extern u8 Value_000000a9;
extern u8 Data_02008ba8[];
extern u8 Data_02008c98[];
extern u8 Data_02008c50[];
extern u8 Data_02008b90[];
extern u8 Data_02008ddc[];
extern u8 Data_02008e54[];
extern u8 Data_02008d10[];
extern u8 Data_00000069[];
extern u8 Data_000000a9[];
extern u8 Data_000000aa[];

void Func_0200099c(s32, s32);
s32 Func_020009a8();
s32 Func_020009da();
s32 Func_020009fc();
s32 Func_02000a16();
void Func_02000a40();
s32 Func_02000a4c();
void Func_02000a4e();
void Func_02000a54();
void Func_02000a58();
void Func_02000a80();
void Func_02000ab6();
s32 Func_02000ae6();
void Func_02000b00();
void Func_02000b08();
void Func_02000b10();
void Func_02000b18();
void Func_02000b1e();
void Func_02000b20();
void Func_02000b20_a();
void Func_02000b34();
void Func_02000b36();
s32 Func_02000b52();
s32 Func_02000bc0();
void Func_02000c16();
void Func_02000c46();
void Func_02000c78(s32);
void Func_02000b9e(s32);
void Func_02000ba4(s32);
void Func_02000c7a(s32);
void Func_02000bd6();
void Func_02000c04();
void Func_02000c0e();
void Func_02000c12();
void Func_02000c20();
void Func_02000c2e();
void Func_02000c42();
void Func_02000c5c();
void Func_02000c82();
void Func_02000cbe();
void Func_02000cca();
void Func_02000c62();
void Func_02000c66();
void Func_02000c90();
s32 Func_02000ca6();
void Func_02000cb4();
void Func_02000ccc();
void Func_02000cec();
void Func_02000cee();
void Func_02000d30();
void Func_02000d6a();
void Func_02000d6a_a();
void Func_02000d74();
void Func_02000d7e();
void Func_02000d82();
void Func_02000d88();
void Func_02000d8c();
void Func_02000d96();
void Func_02000d98();
void Func_02000d98_a();
void Func_02000da2();
void Func_02000daa();
void Func_02000dc4();
void Func_02000dc8();
void Func_02000dce();
void Func_02000ddc();
void Func_02000df0();
s32 Func_02000df6();
s32 Func_02000df6_a();
void Func_02000dfc();
void Func_02000dfe();
void Func_02000e14();
void Func_02000e14_a();
void Func_02000e16();
void Func_02000e28();
void Func_02000e2e();
void Func_02000e44();
void Func_02000e46();
void Func_02000e4c();
void Func_02000e58();
void Func_02000e62();
void Func_02000e76();
void Func_02000e92();
void Func_02000ea0();
void Func_02000eae();
void Func_02000ec4();
void Func_02000ece();
void Func_02000ece_a();
void Func_02000ed8();
void Func_02000edc();
void Func_02000ee8();
void Func_02000ef6();
void Func_02000f00();
void Func_02000f06();
void Func_02000f12();
void Func_02000f20();
void Func_02000f22();
void Func_02000f24();
void Func_02000f28();
void Func_02000f36();
void Func_02000f4a();
void Func_02000f4c();
void Func_02000f4e();
void Func_02000f5c();
void Func_02000f68();
s32 Func_02000f6e();
s32 Func_02000f6e_a();
s32 Func_02000f6e_b();
void Func_02000f76();
void Func_02000f80();
void Func_02000f8c();
void Func_02000f90();
void Func_02000f92();
void Func_02000f98();
void Func_02000fa0();
void Func_02000fa8();
void Func_02000fb0();
void Func_02000fb2();
void Func_02000fc8();
void Func_02000fca();
void Func_02000fe0();
void Func_02000fe6();
void Func_02000fd0();
void Func_02001060();
void Func_02001068();
s32 Func_02001078();
s32 Func_02001008();
void Func_02001094();
void Func_0200106c();
void Func_020010a4();
void Func_020010be();
void Func_0200103a();
void Func_0200104c(void);
void Func_020010d2(s32);
void Func_020010f4(s32, s32, s32);
void Func_020010de(s32, s32);
void Func_0200104a(u8 *, s32);
u8 *Func_02001084(s32);
void Func_02001038(s32);
void Func_020010f8(s32, s32);
void Func_02001064(u8 *, s32);
u8 *Func_0200109e(s32);
void Func_02001054(s32);
void Func_0200111a(s32, s32);
void Func_02001086(u8 *, s32);
u8 *Func_020010c0(s32);
void Func_02001074(s32);
void Func_02001134(s32, s32);
void Func_020010a0(u8 *, s32);
u8 *Func_020010da(s32);
void Func_02001090(s32);
void Func_0200112e(s32, s32, s32);
void Func_020010cc(s32);
void Func_020010f0(void);

/*
 * The eight-byte owner at 0x02000084 includes its one pool word, which holds
 * the returned table address 0x02008b48.
 */

/*
 * Select a table from the scene id.  The 88-byte owner at 0x0200008c includes
 * its seven-word literal pool at 0x020000c4-0x020000e3.  Element 224 of the
 * cross-overlay block Data_02000240 is the signed scene id this overlay keys
 * on.  The compared constants are spelled as the addresses of Value_000000aa
 * and its neighbours: that is how a small integer is pooled here, and the
 * names are not references to symbols.
 */

/* Old-style declarations: overlay import arities vary per call site. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * Engine calls: each pseudo symbol is the per-site call word the overlay image
 * holds -- one word can serve two sites with different targets -- and the
 * macro names the engine function the site reaches through the overlay veneer
 * and the main-image veneer island, keeping the site's own calling form.
 * Unbound names are provisional.
 */

/*
 * Runs a fixed sequence of setup calls, mostly in mirrored pairs for entities
 * 8 and 9, followed by two six-argument calls whose second and last arguments
 * match (entity 6/27 and entity 9/26).
 */

                        /* Test a story flag; used in a condition. */

                        /* Open a scripted scene. */

                        /* Close a scripted scene. */

                        /* Dialogue prompt; the result selects the branch. */

                        /* Scene-presentation request. */

                        /* Show a dialogue line by id. */

                        /* Dialogue-line variant with a mode word. */

                        /* Wait for the slot's action to finish. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

s32 Func_0200092a();    /* Raw encoded call destination. */

void Scene_SetActor13Value1A(void)
{
    Func_0200099c(0xD, 0x1A);
}

s32 SceneData_GetPrimaryTable(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)Data_02008a40;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)Data_02008ad0;
    }
    return (s32)Data_02008998;
}

s32 SceneData_ReturnZero(void)
{
    extern s16 Data_02000240[];

    return 0;
}

u8 *SceneData_GetTable8B48(void)
{
    return (u8 *)0x02008b48;
}

s32 SceneData_GetSecondaryTable(void)
{
    extern s16 Data_02000240[];

    s16 scene = Data_02000240[224];

    if (scene == (s32)&Value_000000aa) {
        return (s32)Data_02008ba8;
    }
    if (scene == (s32)&Value_000000a9) {
        if (Func_0200092a(0x96f) != 0) {
            return (s32)Data_02008c98;
        }
        return (s32)Data_02008c50;
    }
    return (s32)Data_02008b90;
}

s32 SceneData_GetTertiaryTable(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)Data_02008ddc;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)Data_02008e54;
    }
    return (s32)Data_02008d10;
}

s32 Func_02000124(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    s16 *q;
    s16 *p;
    s16 *r;
    s32 v;

    if (Value1(Func_020009a8, 0x89f) != 0) {
        s32 a = (s32)Data_00000069;

        q = (s16 *)Data_02000240;
        q[226] = a;
        {
            s16 *t = q + 227;
            s32 shown = 10;

            *t = shown;
        }
    }
    p = (s16 *)Data_02000240;
    v = p[224];
    if (v == (s32)Data_000000a9) {
        if (Value1(Func_020009da, 0x897) != 0) {
            Func_02000a58(10, 0, 0);
        }
        if (p[225] == 3) {
            if (Value1(Func_020009fc, 0x8fb) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 1;

                    *t = shown;
                }
            }
            if (Value1(Func_02000a16, 0x8fc) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 5;

                    *t = shown;
                }
            }
            Call1(Func_02000a40, 0x12f);
        }
        r = (s16 *)Data_02000240;
        if (r[225] == 1) {
            Call1(Func_02000a4e, 0x8fb);
            if (Value1(Func_02000a4c, 0x96f) == 0) {
                Call6(Func_02000a54, 6, 0, 2, 1, 8, 27);
            }
        }
        if (r[225] != 5) {
            goto L_0200024c;
        }
        Call1(Func_02000a80, 0x8fc);
    } else {
        if (v == (s32)Data_000000aa) {
            Func_02000b00(8, 4);
            Func_02000b08(9, 4);
            Func_02000b10(10, 3);
            Func_02000b18(11, 4);
            Func_02000b20(12, 3);
            *(volatile s32 *)(Func_02000ae6(15) + 28) = 0x19999;
            Call6(Func_02000ab6, 108, 38, 1, 1, 102, 56);
        }
    }
    L_0200024c:;
    return 0;
}

void Func_02000288(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    s32 arg;
    s32 v5;

    arg = *(s16 *)(*(u8 **)Data_03001ebc + 0x16c);
    *(u8 *)(Func_02000b52(0) + 85) = 0;
    v5 = 2;
    Func_02000c16(158);
    Call6(Func_02000b20_a, 66, 36, 71, 8, v5, v5);
    Func_02000b1e(4);
    Call6(Func_02000b36, 68, 36, 71, 8, v5, v5);
    Func_02000b34(4);
    Value3(Func_02000bc0, 0, 3, -16);
    Func_02000c46(arg);
}

void Dialogue_ShowMessages8fbAnd8fc(void)
{
    extern u8 *Data_03001ebc;

    s16 token = *(s16 *)(Data_03001ebc + 364);

    Func_02000c78(123);
    Func_02000b9e(0x8FB);
    Func_02000ba4(0x8FC);
    Func_02000c7a(token);
}

void Scene_RunPrimarySequence(void)
{
    Battle_Reset_1();
    Motion_SetSpeed_1(8, 65536, 32768);
    Motion_SetSpeed_2(9, 65536, 32768);
    Motion_ResetPosMode2_1(8, 136, 384);
    Motion_SetPosReset_1(9, 152, 384);
    Motion_ArmCb_1(8, 16384, 0);
    Motion_ArmCb_2(9, 16384, 0);
    Object_SetModeById_1(8, 1);
    Call6(Func_02000c0e, 6, 27, 1, 1, 7, 27);
    Call6(Func_02000c20, 9, 26, 2, 1, 7, 26);
    Battle_SchedShoulder_1();
}

void Scene_RunScene3c3SequenceA(void)
{
    extern u8 Data_03001ebc[];
    void Func_02000fb6();

    u32 i;
    s32 record;
    s32 v5;

    Func_02000c66();
    Call3(Func_02000c90, 0, 0x19999, 0xcccc);
    Call3(Func_02000cb4, 0, 120, 0x1b6);
    Call3(Func_02000d30, 0, 0xc000, 0);
    record = Value1(Func_02000ca6, 0);
    if (record != 0) {
        Func_02000cec(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_02000c62(1);
    Call3(Func_02000ccc, 11, 0x19999, 0xcccc);
    Call3(Func_02000cee, 11, 108, 0x1af);
    Call3(Func_02000d6a, 11, 0xd000, 10);
    Call3(Func_02000d7e, 11, 0x100, 20);
    Call3(Func_02000d82, 11, 0xd000, 20);
    Func_02000d8c(11, 0, 40);
    Call3(Func_02000d98, 11, 0xd000, 40);
    Func_02000da2(11, 0, 20);
    Func_02000d6a_a(11, 2);
    Call1(Func_02000d88, 0x2654);
    Func_02000daa(11, 0, 40);
    Call3(Func_02000dce, 8, 0x100, 0);
    Func_02000d96(8, 2);
    Func_02000dc8(8, 0, 10);
    Call3(Func_02000d74, 11, 132, 0x1a4);
    Call3(Func_02000df0, 11, 0xd000, 0);
    Call3(Func_02000dfc, 0, 0xe000, 0);
    Call3(Func_02000d98_a, 11, 138, 0x1a0);
    Call3(Func_02000e14, 11, 0xb000, 10);
    Func_02000ddc(11, 2);
    Func_02000e16(11, 0, 40);
    ((void (*)())Func_02000df6)(8, 2);
    Func_02000e28(8, 0, 40);
    Call3(Func_02000e4c, 9, 0x100, 20);
    Func_02000e14_a(9, 2);
    Func_02000e46(9, 0, 20);
    Call3(Func_02000e62, 0, 0xc000, 0);
    Call3(Func_02000dfe, 11, 144, 0x1a4);
    Func_02000dc4(20);
    Func_02000e44(9, 2);
    Func_02000e76(9, 0, 20);
    Call2(Func_02000ea0, 9, 0x102);
    Func_02000e58(9, 3);
    Func_02000e92(9, 0, 20);
    Call3(Func_02000eae, 11, 0x5000, 20);
    Func_02000eae_a(11, 0);
    if (Value1(Func_02000df6_a, 0x9b0) != 0) {
        Call3(Func_02000ece, 11, 0xd000, 40);
        Call2(Func_02000ee8, 11, 0x102);
        Func_02000e2e(40);
        Func_02000ed8(11, 0, 10);
    } else {
        bump_step(1);
    }
    Call3(Func_02000f06, 11, 0x5000, 10);
    Func_02000f00(11, 0, 40);
    Call3(Func_02000f24, 11, 0x100, 40);
    Call3(Func_02000f28, 11, 0xb000, 10);
    Func_02000f22(11, 0, 10);
    Call3(Func_02000ece_a, 11, 138, 0x1a0);
    Call3(Func_02000f4a, 11, 0xb000, 20);
    Func_02000f12(8, 2);
    Func_02000f4c(8, 0, 10);
    Call2(Func_02000f76, 11, 0x102);
    Func_02000f36(11, 1);
    Func_02000ec4(20);
    ((void (*)())Func_02000f6e)(11, 0, 20);
    Func_02000f4e(9, 2);
    Func_02000edc(20);
    Call3(Func_02000f98, 0, 0xe000, 10);
    Func_02000f68(9, 1);
    Func_02000ef6(20);
    Func_02000fa0(9, 0, 10);
    Func_02000f80(8, 2);
    Func_02000fb2(8, 0, 20);
    Func_02000f92(11, 1);
    Func_02000f20(20);
    Func_02000fca(11, 0, 20);
    Call3(Func_02000fe6, 11, 0x5000, 10);
    Func_02000fe0(11, 0, 10);
    Func_02000fa8(11, 2);
    record = Value1(Func_02000f6e_a, 0);
    if (record != 0) {
        Func_02000f90(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02000fb6(11);
    v5 = 7;
    Func_02000fc8(11, 0, 0);
    Call6(Func_02000f5c, 6, 27, 1, 1, v5, 27);
    Call6(Func_02000f6e_b, 9, 26, 2, 1, v5, 26);
    Call1(Func_02000f8c, 0x89f);
    Func_02000fb0();
}

/*
 * Dialogue bracket at 0x02000730.  The 124-byte owner includes its four-word
 * literal pool at 0x0200079c-0x020007ab.  The tail is shared through a label
 * rather than copied into each arm: copying it would add a fourth call site
 * where there are three.  The skip-beat counter sits at byte offset 472 off
 * the Data_03001ebc pointer cell, which costs one dereference.  The guard is
 * tested against zero at this site; the polarity is read per call site.
 */
void Scene_RunActorNinePromptDialogue(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000fb6();
    s32 Func_02000fb6_a();

    u8 *work;

    Func_02000fd0();

    if (Func_02000fb6_a(0x89f) != 0) {
        Func_02001060(0x2668);
        goto close;
    }

    Func_02001068(0x264e);
    {
        s32 mode = 0;
        s32 no = 9;

        Func_02001078(no, mode);
    }

    if (Func_02001008(0, 0) != 0) {
        goto skip;
    }

    Func_02001094(9, 0);
    Func_0200106c(9, 4);

close:
    Func_020010a4(9, 0);
    goto done;

skip:
    /* Skip-beat counter, two beats' worth. */
    work = Data_03001ebc;
    *(u16 *)(work + 472) += 2;
    Func_020010be(9, 0);

done:
    Func_0200103a();
}

void Scene_RunActorTenRepeatedMotion(void)
{
    extern u8 *Data_03001ebc;
    s32 Func_02000fb6();

    unsigned int beat;

    Func_0200104c();

    Func_020010d2(0x266d);
    Func_020010f4(10, 0, 10);

    beat = 0;
    do {
        Func_020010de(10, 0);
        Func_0200104a(Func_02001084(10), 1);
        Func_02001038(4);

        Func_020010f8(10, 15);
        Func_02001064(Func_0200109e(10), 0);
        beat++;
        Func_02001054(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Func_0200111a(10, 0);
        Func_02001086(Func_020010c0(10), 1);
        Func_02001074(2);

        Func_02001134(10, 15);
        Func_020010a0(Func_020010da(10), 0);
        beat++;
        Func_02001090(2);
    } while (beat <= 11);

    Func_0200112e(10, 0, 0);

    Func_020010cc(0x897);

    Func_020010f0();
}
