#include "types.h"

#define SceneDialogue_ShowLine1571Or152F Func_02000254
#define SceneDialogue_RunActor9Line Func_0200028c
#define SceneDialogue_RunActorTenFlag881Dialogue Func_020002b8
#define SceneDialogue_RunActor12Line Func_02000510
#define SceneDialogue_RunActor18Line Func_02000530
#define SceneDialogue_RunActor20BranchScene Func_02000550
#define SceneDialogue_RunActor20FlaggedLine Func_020005a4
#define SceneDialogue_RunActorEightBranchedDialogue Func_02000608
#define SceneDialogue_ShowLine156E Func_02000668
#define SceneDialogue_ShowLine1573Or155A Func_02000688
void Func_02002488(void);
s32 Func_0200246e(s32);
void Func_02002568(s32);
void Func_02002570(s32);
void Func_02002580(s32, s32);
void Func_020024b4(void);
s32 Func_020024c0(void);
s32 Func_0200257a(s32, s32, s32);
s32 Func_020025a0(s32);
s32 Func_020025c0(s32, s32);
s32 Func_020024e4(void);
void Func_020024ec();
s32 Func_020024d2();
void Func_020025cc();
void Func_020025dc_a();
void Func_020025dc_b();
void Func_02002616();
void Func_0200250c();
void Func_020025a4();
void Func_0200251a();
s32 Func_0200261c();
void Func_02002630_a();
void Func_02002630_b();
void Func_02002630_c();
void Func_020025d0();
void Func_02002620();
void Func_0200266a();
void Func_02002560();
void Func_020025f8();
void Func_0200256e();
void Func_02002660();
void Func_02002684();
void Func_02002624();
void Func_020025a8();
s32 Func_02002744();
s32 Func_0200281a();
s32 Func_0200283a();
s32 Func_0200275e();
s32 Func_02002764();
s32 Func_0200283a_a();
s32 Func_0200285a();
s32 Func_0200277e();
s32 Func_02002784();
s32 Func_0200276a();
s32 Func_02002864();
s32 Func_02002874_a();
s32 Func_02002874_b();
s32 Func_02002894();
s32 Func_0200279a();
s32 Func_020027a0();
s32 Func_020027c4();
void Func_020027d8(void);
s32 Func_020027be(s32);
void Func_020028b8(s32);
void Func_020028c0(s32);
void Func_020028d0(s32, s32);
void Func_02002804(void);
void Func_0200283c();
s32 Func_02002822();
void Func_0200291c();
s32 Func_02002834();
void Func_0200292e();
void Func_02002936();
void Func_02002946();
void Func_0200287a();
void Func_0200289c();
void Func_02002972();
void Func_02002982();
void Func_020028b6();
void Func_020028bc();
s32 Func_020028a2();
void Func_0200299c();
void Func_020029a4();
void Func_020029b4();
void Func_020028e8();

/* Scripted dialogue step for resource_399.  The imports name loader-relocated
 * call words and stay old-style because their arity varies between sites. */

/* Typed wrappers around the two angle calls.  Their parameters are what force
 * the 0x3000 argument to be built from an immediate and a shift at each call. */
static __inline__ void Call_02002630(s32 channel, s32 angle, s32 frames)
{
    Func_02002630_a(channel, angle, frames);
}

static __inline__ void Call_02002684(s32 channel, s32 angle, s32 frames)
{
    Func_02002684(channel, angle, frames);
}

void SceneDialogue_ShowLine1571Or152F(void)
{
    Func_02002488();
    if (Func_0200246e(3) != 0) {
        Func_02002568(0x1571);
    } else {
        Func_02002570(0x152f);
    }
    Func_02002580(8, 0);
    Func_020024b4();
}

void SceneDialogue_RunActor9Line(void)
{
    Func_020024c0();
    Func_0200257a(9, 0, 10);
    Func_020025a0(0x152a);
    Func_020025c0(9, 0);
    Func_020024e4();
}

/*
 * One scripted section, bracketed by an open and a close call, in which story
 * flag 0x881 picks between two arms on channel 10.  The arms differ only in the
 * message id and one step call, and stay separate so that every call is written
 * once.  258 is a pose id, 0x3000 three sixteenths of a turn.  Func_0200261c's
 * unused s32 return is what fixes that call's argument order.
 */
void SceneDialogue_RunActorTenFlag881Dialogue(void)
{
    Func_020024ec();

    if (Func_020024d2(0x881) != 0) {
        Func_020025cc(0x163c);
        Func_020025dc_a(10, 0);
        Func_02002616(10, 258);
        Func_0200250c(40);
        Func_020025a4(10, 1);
        Func_0200251a(20);
        Func_020025dc_b(10, 0, 20);
        Func_0200261c(10, 0);
        Call_02002630(10, 0x3000, 10);
        Func_020025d0(10, 9);
    } else {
        Func_02002620(0x152d);
        Func_02002630_b(10, 0);
        Func_0200266a(10, 258);
        Func_02002560(40);
        Func_020025f8(10, 1);
        Func_0200256e(20);
        Func_02002630_c(10, 0, 20);
        Func_02002660(10, 0);
        Call_02002684(10, 0x3000, 10);
        Func_02002624(10, 9);
    }

    Func_020025a8();
}

void SceneDialogue_RunActor12Line(void)
{
    Func_02002744();
    Func_0200281a(0x153f);
    Func_0200283a(12, 0);
    Func_0200275e();
}

void SceneDialogue_RunActor18Line(void)
{
    Func_02002764();
    Func_0200283a_a(0x154d);
    Func_0200285a(18, 0);
    Func_0200277e();
}

void SceneDialogue_RunActor20BranchScene(void)
{
    Func_02002784();
    if (Func_0200276a(3) != 0) {
        Func_02002864(0x1574);
        Func_02002874_a(20, 0);
    } else {
        Func_02002874_b(0x1557);
        Func_02002894(20, 0);
        Func_0200279a(0x82a);
        Func_020027a0(0x82c);
    }
    Func_020027c4();
}

void SceneDialogue_RunActor20FlaggedLine(void)
{
    Func_020027d8();
    if (Func_020027be(3) != 0) {
        Func_020028b8(0x1575);
    } else {
        Func_020028c0(0x155B);
    }
    Func_020028d0(20, 0);
    Func_02002804();
}

void SceneDialogue_RunActorEightBranchedDialogue(void)
{
    Func_0200283c();
    if (Func_02002822(0x82b) != 0) {
        Func_0200291c(0x156f);
    } else if (Func_02002834(0x82c) != 0) {
        Func_0200292e(0x153b);
    } else {
        Func_02002936(0x1533);
    }
    Func_02002946(8, 0);
    Func_0200287a();
}

void SceneDialogue_ShowLine156E(void)
{
    Func_0200289c();
    Func_02002972(0x156e);
    Func_02002982(10, 0);
    Func_020028b6();
}

void SceneDialogue_ShowLine1573Or155A(void)
{
    Func_020028bc();
    if (Func_020028a2(3) != 0) {
        Func_0200299c(0x1573);
    } else {
        Func_020029a4(0x155a);
    }
    Func_020029b4(19, 0);
    Func_020028e8();
}
