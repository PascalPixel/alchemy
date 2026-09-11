#include "types.h"

/* Overlay resource_37b, scene script at 02000614 (1656 bytes).
 *
 * The routine writes 0x100 and 32 into the shared scene work record at
 * Data_03001ebc offsets 0x1c0 and 0x1c8, issues one long straight-line run of
 * 167 scene calls, then writes 0x204 and 16 into the same two fields before its
 * final call and returns. The call arguments select four subjects - 8, 5, 1 and
 * 0 - and carry fixed-point coordinate pairs and small mode/step numbers. There
 * are no loops and no switch tables; the only control flow is one test of a
 * value-returning call at 02000a4e that selects constant 0x1010 or 0x1011, and
 * two null guards at 02000be6 and 02000c16 that forward a probed record's s16
 * fields at +10 and +18 into a following call.
 *
 * The work-record pointer at Data_03001ebc is read twice, once at the head and
 * once at the tail, because the frame saves only lr: nothing can hold the
 * pointer across the intervening calls.
 *
 * Uncertain: the roles of the individual calls. Their runtime entry points are
 * unnamed apart from one - the eighteen sites whose loader binding is 0200a3a4
 * reach Scene_RunSplitPairSteps (resource 020023a4) - so the wording above
 * describes argument shapes, not proven behaviour. Every site nonetheless keeps
 * its legacy Func_<pre-relocation-word> spelling, including those eighteen:
 * alchemy binds an overlay call from that spelling, and refuses a project name
 * ("no stable overlay call binding") until the owner's translation unit
 * declares its runtime symbol. The meaning of work-record offsets 0x1c0 and
 * 0x1c8 is inherited from the sibling overlay scene scripts in the corpus and
 * is not independently established here.
 *
 * Measurement status: this owner has no entry in source-paths.json and no
 * translation unit, so `alchemy score` refuses this file outright with
 * "ambiguous overlay call identity" - thirteen legacy names below reach two or
 * three different runtime targets from different sites and cannot be bound
 * without the absolute_symbols table listed further down. Measured on an
 * alias-substituted diagnostic copy (twenty-five ambiguous sites respelled to a
 * different legacy name with the same runtime target, which emits identical
 * bytes; the two sites with no same-target alias deliberately pointed
 * elsewhere), the owner scores candidate=1656 reference=1656
 * differing_halfwords=2, and the complete aligned diff holds exactly those two
 * deliberate call-target differences. This file is therefore expected to be
 * BYTE-EXACT once the unit exists: route it to adoption verification rather
 * than retaining it as a draft.
 */

#define FieldScene_RunStagedActorScene Func_02000614

extern u8 Data_03001ebc[];

void Func_02002a02();
void Func_02002a52();
void Func_02002a6c();
void Func_02002a88();
void Func_02002a9c();
void Func_02002ae2();
void Func_02002aee();
void Func_02002af2();
void Func_02002af8();
void Func_02002b08();
void Func_02002b08_a();
void Func_02002b1a();
void Func_02002b24();
void Func_02002b32();
void Func_02002b36();
void Func_02002b3c();
void Func_02002b56();
void Func_02002b5c();
void Func_02002b64();
void Func_02002b6a();
void Func_02002b76();
void Func_02002b76_a();
void Func_02002b7a();
void Func_02002b7a_a();
void Func_02002b82();
void Func_02002b8a();
void Func_02002b92();
void Func_02002ba0();
void Func_02002bb6();
void Func_02002bbc();
void Func_02002bc6();
void Func_02002bce();
void Func_02002be2();
void Func_02002bea();
void Func_02002bf0();
void Func_02002c02();
void Func_02002c02_a();
void Func_02002c0e();
void Func_02002c10();
void Func_02002c18();
void Func_02002c18_a();
void Func_02002c1c();
void Func_02002c20();
void Func_02002c28();
void Func_02002c30();
void Func_02002c3a();
void Func_02002c4e();
void Func_02002c54();
void Func_02002c58();
void Func_02002c62();
void Func_02002c6c();
void Func_02002c7c();
void Func_02002c82();
void Func_02002c8a();
void Func_02002c92();
void Func_02002c98();
void Func_02002cae();
void Func_02002cb2();
void Func_02002cbc();
void Func_02002cbc_a();
void Func_02002cc4();
void Func_02002cce();
void Func_02002cce_a();
void Func_02002cce_b();
void Func_02002cd2();
void Func_02002cda();
void Func_02002cde();
void Func_02002cea();
void Func_02002d00();
void Func_02002d08();
void Func_02002d1e();
void Func_02002d24();
void Func_02002d34();
void Func_02002d3e();
void Func_02002d68();
void Func_02002d68_a();
void Func_02002d6e();
void Func_02002d82();
void Func_02002d84();
void Func_02002d8c();
void Func_02002d8e();
void Func_02002d98();
void Func_02002da4();
void Func_02002daa();
void Func_02002db0();
void Func_02002dbc();
void Func_02002dbe();
void Func_02002dc2();
void Func_02002dc8();
void Func_02002dd4();
void Func_02002dd4_a();
void Func_02002de6();
void Func_02002dfc();
void Func_02002e00();
void Func_02002e0e();
void Func_02002e14();
void Func_02002e18();
void Func_02002e18_a();
void Func_02002e20();
void Func_02002e24();
void Func_02002e30();
void Func_02002e3e();
void Func_02002e4a();
void Func_02002e4e();
void Func_02002e64();
void Func_02002e70();
void Func_02002e76();
void Func_02002e9e();
void Func_02002ea2();
void Func_02002ea8();
void Func_02002eb2();
void Func_02002eb2_a();
s32 Func_02002eb4();
void Func_02002eca();
s32 Func_02002ed0();
void Func_02002ede();
void Func_02002ee4();
void Func_02002eec();
void Func_02002ef8();
void Func_02002f04();
void Func_02002f08();
void Func_02002f24();
void Func_02002f36();
void Func_02002f3c();
void Func_02002f3e();
void Func_02002f4c();
void Func_02002f4e();
void Func_02002f56();
void Func_02002f5c();
void Func_02002f66();
void Func_02002f6a();
void Func_02002f72();
void Func_02002f82();
void Func_02002f84();
void Func_02002f90();
void Func_02002f9e();
void Func_02002faa();
void Func_02002fdc();
void Func_02002fde();
void Func_02002fee();
void Func_02002ff4();
void Func_02002ff8();
void Func_02002ffc();
void Func_0200300c();
void Func_02003012();
void Func_02003026();
void Func_0200303c();
void Func_0200304a();
s32 Func_02003050();
void Func_0200305e();
void Func_0200306e();
void Func_02003072();
void Func_02003076();
s32 Func_02003080();
void Func_02003080_a();
void Func_0200308a();
void Func_0200308a_a();
void Func_02003090();
void Func_02003098();
void Func_020030a2();
void Func_020030a4();
void Func_020030aa();
void Func_020030b0();
void Func_020030b2();
void Func_020030ba();
void Func_020030c8();
void Func_020030da();

/* Thirteen of the legacy call names below reach two or three different loader
 * bindings from different sites in this one owner, so each extra site needs its
 * own declaration and its runtime address declared in the owner's translation
 * unit. Required absolute_symbols entries:
 *
 *   Func_02002b08    0x0200a44c      Func_02002b08_a  0x0200a3a4
 *   Func_02002b76    0x0200a544      Func_02002b76_a  0x0200a44c
 *   Func_02002b7a    0x0200a514      Func_02002b7a_a  0x0200a4c4
 *   Func_02002c02    0x0200a51c      Func_02002c02_a  0x0200a44c
 *   Func_02002c18    0x0200a4f4      Func_02002c18_a  0x0200a4ac
 *   Func_02002cbc    0x0200a4d4      Func_02002cbc_a  0x0200a44c
 *   Func_02002cce    0x0200a4cc      Func_02002cce_a  0x0200a4bc
 *                                    Func_02002cce_b  0x0200a3a4
 *   Func_02002d68    0x0200a514      Func_02002d68_a  0x0200a3a4
 *   Func_02002dd4    0x0200a4f4      Func_02002dd4_a  0x0200a44c
 *   Func_02002e18    0x0200a4c4      Func_02002e18_a  0x0200a4b4
 *   Func_02002eb2    0x0200a504      Func_02002eb2_a  0x0200a4d4
 *   Func_02003080    0x0200a46c      Func_02003080_a  0x0200a444
 *   Func_0200308a    0x0200a514      Func_0200308a_a  0x0200a4ac
 *
 * Every entry is thumb. Without them alchemy score refuses the owner with
 * "ambiguous overlay call identity"; with them the body below links byte for
 * byte against the reference.
 */

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

void FieldScene_RunStagedActorScene(void)
{
    u8 *work;
    s32 record;

    Func_02002a6c();
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Func_02002b76();
    Func_02002b8a();
    Func_02002a88(20);
    Call3(Func_02002aee, 8, 0x2400000, 0x1280000);
    Func_02002a9c(1);
    Call1(Func_02002b32, 0x1004);
    Func_02002a02(8, 6);
    Call2(Func_02002b7a, 0xcccc, 0x1999);
    Call4(Func_02002b92, 0x23e0000, -1, 0xb40000, 1);
    Call3(Func_02002af8, 8, 0x10000, 0x8000);
    Call3(Func_02002b24, 8, 0x240, 216);
    Func_02002ae2(20);
    Func_02002b5c(5, 2, 0);
    Func_02002af2(30);
    Func_02002a52(5, 6);
    Func_02002b7a_a(8, 2);
    Func_02002b08(6);
    Call3(Func_02002bbc, 8, 0x9000, 0);
    Func_02002b1a(10);
    Call2(Func_02002bea, 0x59999, 0xb333);
    Call4(Func_02002c02, 0x11f0000, -1, 0xb00000, 1);
    Func_02002c0e();
    Func_02002b3c(60);
    Call4(Func_02002c1c, 0x23e0000, -1, 0xb40000, 1);
    Func_02002c28();
    Func_02002b56(20);
    Func_02002bc6(8, 3);
    Func_02002b64(10);
    Call3(Func_02002c18, 8, 0xc000, 0);
    Func_02002b76_a(10);
    Func_02002bf0(8, 6, 0);
    Call3(Func_02002bb6, 8, 0x30000, 0x20000);
    Call3(Func_02002be2, 8, 0x240, 184);
    Func_02002ba0(40);
    Func_02002c10(8, 3);
    Func_02002b08_a(8, 6);
    Func_02002c18_a(1, 3);
    Func_02002c20(5, 3);
    Func_02002c30(0, 3);
    Func_02002bce(40);
    Func_02002c4e(8, 3);
    Func_02002b36(8, 6);
    Func_02002c58(0, 2, 0);
    Func_02002c62(1, 2, 0);
    Func_02002c6c(5, 2, 0);
    Func_02002c02_a(30);
    Func_02002c82(1, 2);
    Func_02002b6a(1, 6);
    Func_02002c92(8, 1);
    Func_02002c8a(8, 4);
    Func_02002b82(8, 6);
    Func_02002cbc(0, 5, 0);
    Func_02002c3a(40);
    Func_02002cc4(8, 0, 0);
    Func_02002cce(8, 5, 0);
    Func_02002c54(40);
    Func_02002cce_a(8, 6, 0);
    Call3(Func_02002cb2, 8, 0x240, 216);
    Call3(Func_02002d1e, 8, 0x8000, 0);
    Func_02002c7c(10);
    Call3(Func_02002cae, 8, 0x13333, 0x9999);
    Call3(Func_02002cd2, 8, 0x1b0, 200);
    Func_02002c98(20);
    Call2(Func_02002d68, 0x26666, 0x4ccc);
    Call4(Func_02002d82, 0x1200000, -1, 0xab0000, 1);
    Func_02002d8e();
    Func_02002cbc_a(80);
    Func_02002d24(8, 1);
    Call4(Func_02002da4, 0x23e0000, -1, 0xb40000, 1);
    Func_02002cda(20);
    Func_02002d8c(8, 0, 0);
    Func_02002cea(30);
    Call3(Func_02002d3e, 8, 0x240, 216);
    Call3(Func_02002daa, 8, 0xc000, 0);
    Func_02002d08(10);
    Call3(Func_02002dbc, 0, 0x4000, 0);
    Call3(Func_02002dc8, 5, 0x4000, 0);
    Call3(Func_02002dd4, 1, 0x4000, 0);
    Call2(Func_02002dfc, 0xcccc, 0x1999);
    Call4(Func_02002e14, 0x23e0000, -1, 0xab0000, 1);
    Func_02002dbe(8, 6, 0);
    Call3(Func_02002d84, 8, 0x30000, 0x20000);
    Call3(Func_02002db0, 8, 0x240, 184);
    Func_02002d6e(80);
    Func_02002cce_b(8, 6);
    Func_02002de6(8, 4);
    Func_02002cde(8, 20);
    Call3(Func_02002e4a, 5, 0x102, 0);
    Func_02002d98(40);
    Func_02002e18(5, 2);
    Func_02002d00(5, 6);
    Func_02002e18_a(8, 3);
    Call3(Func_02002e64, 8, 0x8000, 0);
    Func_02002dc2(40);
    Call3(Func_02002e76, 8, 0xc000, 0);
    Func_02002dd4_a(30);
    Func_02002d34(8, 6);
    Call3(Func_02002e9e, 0, 0x101, 0);
    Call3(Func_02002ea8, 1, 0x101, 0);
    Call3(Func_02002eb2, 5, 0x101, 0);
    Func_02002e00(60);
    Func_02002e70(8, 4);
    Func_02002d68_a(8, 6);
    Func_02002ea2(1, 0, 0);
    Func_02002e20(40);
    Func_02002eb2_a(5, 0, 0);
    Func_02002e30(40);
    Value2(Func_02002ed0, 8, 0);
    Call3(Func_02002eec, 0, 0x4000, 0);
    Call3(Func_02002ef8, 5, 0x4000, 0);
    Call3(Func_02002f04, 1, 0x4000, 0);
    if (Value2(Func_02002eb4, 0, 0) == 0) {
        Call1(Func_02002f36, 0x1010);
    } else {
        Call1(Func_02002f3e, 0x1011);
    }
    Func_02002e0e(8, 6);
    Call1(Func_02002f4c, 0x1012);
    Func_02002f3c(8, 2);
    Func_02002e24(8, 6);
    Call3(Func_02002f90, 1, 0x102, 0);
    Func_02002ede(60);
    Func_02002e3e(1, 6);
    Func_02002f56(8, 3);
    Func_02002e4e(8, 6);
    Call3(Func_02002faa, 8, 0x4000, 0);
    Func_02002f08(20);
    Func_02002f82(8, 6, 0);
    Call3(Func_02002f66, 8, 0x240, 216);
    Func_02002f24(40);
    Call2(Func_02002ff4, 0xcccc, 0x1999);
    Call4(Func_0200300c, 0x23e0000, -1, 0xbf0000, 1);
    Call3(Func_02002f72, 8, 0x10000, 0x8000);
    Call3(Func_02002f9e, 8, 0x240, 232);
    Func_02002f5c(40);
    Func_02002fdc(8, 2);
    Func_02002f6a(40);
    Func_02002eca(8, 6);
    Call3(Func_02003026, 8, 0xc000, 0);
    Func_02002f84(30);
    Func_02002ee4(8, 6);
    Func_02002ffc(8, 3);
    Call4(Func_02003076, 0x2400000, -1, 0xd70000, 1);
    Call3(Func_02002ff8, 8, 0x23e, 0x143);
    Func_02003012(8, 0, 0);
    Call2(Func_0200308a, 0x39999, 0x7333);
    Call4(Func_020030a4, 0x2400000, -1, 0x880000, 1);
    Func_020030b0();
    Func_02002fde(20);
    Func_02003090(5, 0, 0);
    Func_02002fee(10);
    Func_02002f4e(5, 6);
    Func_0200305e(1, 3);
    Func_0200306e(5, 3);
    Call3(Func_0200303c, 1, 0x10000, 0x8000);
    Call3(Func_0200304a, 5, 0x10000, 0x8000);
    Func_0200308a_a(5, 2);
    record = Value1(Func_02003050, 0);
    if (record != 0) {
        Func_02003072(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003098(5);
    Func_020030aa(5, 0, 0);
    Func_020030ba(1, 2);
    record = Value1(Func_02003080, 0);
    if (record != 0) {
        Func_020030a2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020030c8(1);
    Func_020030da(1, 0, 0);
    Call1(Func_02003080_a, 0x12f);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x204;
    *(s32 *)(work + 0x1c8) = 16;
    Func_020030b2();
}
