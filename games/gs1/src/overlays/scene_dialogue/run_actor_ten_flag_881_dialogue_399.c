#include "types.h"

/* Scripted dialogue step for resource_399.  The imports name loader-relocated
 * call words and stay old-style because their arity varies between sites. */
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
