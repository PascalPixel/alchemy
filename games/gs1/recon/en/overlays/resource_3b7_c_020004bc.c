#include "types.h"

#define FieldScene_RunScene3b7SequenceA Func_020004bc

void Func_02001d9e();
void Func_02001db6();
void Func_02001dd4();
void Func_02001de2();
void Func_02001de4();
void Func_02001dee();
void Func_02001df2();
void Func_02001e00();
void Func_02001e10();
void Func_02001e1c();
void Func_02001e2e();
void Func_02001e3a();
void Func_02001e4c();
void Func_02001e58();
void Func_02001e62();
void Func_02001e6a();
void Func_02001e76();
void Func_02001e88();
void Func_02001e94();
void Func_02001e98();
void Func_02001ea6();
void Func_02001eb2();
void Func_02001ec4();
void Func_02001ed0();
void Func_02001ee2();
void Func_02001ee4();
void Func_02001eee();
void Func_02001f00();
void Func_02001f02();
void Func_02001f0c();
void Func_02001f1e();
void Func_02001f20();
void Func_02001f2a();
void Func_02001f3c();
void Func_02001f3e();
void Func_02001f48();
void Func_02001f5c();
void Func_02001f66();
void Func_02001f76();
void Func_02001f7a();
void Func_02001f84();
void Func_02001f94();
void Func_02001f98();
void Func_02001fa2();
void Func_02001fb2();
void Func_02001fb6();
void Func_02001fc0();
void Func_02001fd0();
void Func_02001fd4();
void Func_02001fdc();
void Func_02001fee();
void Func_02001fee_a();
void Func_02001ff2();
void Func_02001ffa();
void Func_0200200c();
void Func_02002010();
void Func_02002018();
void Func_0200202a();
void Func_0200202e();
void Func_02002036();
void Func_02002048();
void Func_0200204c();
void Func_02002054();
void Func_0200205e();
void Func_02002066();
void Func_0200206a();
void Func_02002072();
void Func_02002076();
void Func_02002084();
void Func_02002090();
void Func_020020a2();
void Func_020020a4();
void Func_020020ae();
void Func_020020c0();
void Func_020020c2();
void Func_020020cc();
void Func_020020de();
void Func_020020e0();
void Func_020020ea();
void Func_020020fc();
void Func_020020fe();
void Func_02002108();
void Func_0200211c();
void Func_02002126();
void Func_0200213a();
void Func_02002144();
void Func_02002158();
void Func_02002162();
void Func_02002176();
void Func_02002180();
void Func_02002194();
void Func_02002194_a();
void Func_020021b2();
void Func_020021d0();
void Func_020021ee();
void Func_0200220c();
void Func_0200222a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene3b7SequenceA(s32 a0)
{
    u32 i;
    s32 record;
    s32 v6;

    Func_02001de4();
    Func_02001de2(30);
    Func_02001e98(148);
    Func_02001dee(100);
    Call3(Func_02001e62, 0, 0xc000, 0);
    v6 = 3;
    Func_02001e00(40);
    Call6(Func_02001d9e, 82, 20, 70, 0, v6, 8);
    Func_02001e1c(3);
    Call6(Func_02001db6, 85, 20, 70, 0, v6, 8);
    Func_02001ee4(154);
    Func_02001e3a(8);
    Call6(Func_02001dd4, 88, 20, 70, 0, v6, 8);
    Func_02001f02(154);
    Func_02001e58(8);
    Call6(Func_02001df2, 91, 20, 70, 0, v6, 8);
    Func_02001f20(154);
    Func_02001e76(8);
    Call6(Func_02001e10, 94, 20, 70, 0, v6, 8);
    Func_02001f3e(154);
    Func_02001e94(8);
    Call6(Func_02001e2e, 97, 20, 70, 0, v6, 8);
    Func_02001f5c(154);
    Func_02001eb2(8);
    Call6(Func_02001e4c, 100, 20, 70, 0, v6, 8);
    Func_02001f7a(154);
    Func_02001ed0(8);
    Call6(Func_02001e6a, 79, 29, 70, 0, v6, 8);
    Func_02001f98(154);
    Func_02001eee(8);
    Call6(Func_02001e88, 82, 29, 70, 0, v6, 8);
    Func_02001fb6(154);
    Func_02001f0c(8);
    Call6(Func_02001ea6, 85, 29, 70, 0, v6, 8);
    Func_02001fd4(154);
    Func_02001f2a(8);
    Call6(Func_02001ec4, 88, 29, 70, 0, v6, 8);
    Func_02001ff2(154);
    Func_02001f48(8);
    Call6(Func_02001ee2, 91, 29, 70, 0, v6, 8);
    Func_02002010(154);
    Func_02001f66(8);
    Call6(Func_02001f00, 94, 29, 70, 0, v6, 8);
    Func_0200202e(154);
    Func_02001f84(8);
    Call6(Func_02001f1e, 97, 29, 70, 0, v6, 8);
    Func_0200204c(154);
    Func_02001fa2(8);
    Call6(Func_02001f3c, 100, 29, 70, 0, v6, 8);
    Func_0200206a(154);
    Func_02001fc0(70);
    Func_02002076(126);
    Func_0200205e(a0, 3);
    Func_02001fee(a0, 0);
    Func_02001fdc(20);
    Call6(Func_02001f76, 97, 29, 70, 0, v6, 8);
    Func_020020a4(154);
    Func_02001ffa(8);
    Call6(Func_02001f94, 94, 29, 70, 0, v6, 8);
    Func_020020c2(154);
    Func_02002018(8);
    Call6(Func_02001fb2, 91, 29, 70, 0, v6, 8);
    Func_020020e0(154);
    Func_02002036(8);
    Call6(Func_02001fd0, 88, 29, 70, 0, v6, 8);
    Func_020020fe(154);
    Func_02002054(8);
    Call6(Func_02001fee_a, 85, 29, 70, 0, v6, 8);
    Func_0200211c(154);
    Func_02002072(8);
    Call6(Func_0200200c, 82, 29, 70, 0, v6, 8);
    Func_0200213a(154);
    Func_02002090(8);
    Call6(Func_0200202a, 100, 20, 70, 0, v6, 8);
    Func_02002158(154);
    Func_020020ae(8);
    Call6(Func_02002048, 97, 20, 70, 0, v6, 8);
    Func_02002176(154);
    Func_020020cc(8);
    Call6(Func_02002066, 94, 20, 70, 0, v6, 8);
    Func_02002194(154);
    Func_020020ea(8);
    Call6(Func_02002084, 91, 20, 70, 0, v6, 8);
    Func_020021b2(154);
    Func_02002108(8);
    Call6(Func_020020a2, 88, 20, 70, 0, v6, 8);
    Func_020021d0(154);
    Func_02002126(8);
    Call6(Func_020020c0, 85, 20, 70, 0, v6, 8);
    Func_020021ee(154);
    Func_02002144(8);
    Call6(Func_020020de, 82, 20, 70, 0, v6, 8);
    Func_0200220c(154);
    Func_02002162(8);
    Call6(Func_020020fc, 79, 20, 70, 0, v6, 8);
    Func_0200222a(154);
    Func_02002180(8);
    Func_02002194_a();
}
