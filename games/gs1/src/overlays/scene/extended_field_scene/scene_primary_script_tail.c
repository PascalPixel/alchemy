#include "types.h"

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_0200306e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02002f34, a0, a1, a2)
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1_0200084c(a0) Call1(Func_0200309e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_0200084c(a0, a1, a2) Call3(Func_02002f62, a0, a1, a2)
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1_0200087c(a0) Call1(Func_020030ce, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_0200087c(a0, a1, a2) Call3(Func_02002f92, a0, a1, a2)
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1_020008ac(a0) Call1(Func_020030fe, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_020008ac(a0, a1, a2) Call3(Func_02002fc4, a0, a1, a2)
#define NULL ((void *)0)
#define FieldScene_RunSupplementalSequenceFour Func_0200081c
#define FieldScene_RunSupplementalSequenceFive Func_0200084c
#define FieldScene_RunSupplementalSequenceSix Func_0200087c
#define FieldScene_RunSupplementalSequenceSeven Func_020008ac
#define FieldScene_RunStep7BThen8 Func_020008dc
#define FieldScene_RunStep15 Func_02002580
#define FieldScene_RunStep17 Func_02002590
#define FieldScene_RunStep20 Func_020025a0

extern u8 Value_0200beb4;

void Func_02000f52();
void Func_02002e68();
void Func_02002f34();
void Func_0200306e();
void Func_02000f80();
void Func_02002e98();
void Func_02002f62();
void Func_0200309e();
void Func_02000fb0();
void Func_02002ec8();
void Func_02002f92();
void Func_020030ce();
void Func_02000fe2();
void Func_02002ef8();
void Func_02002fc4();
void Func_020030fe();
void Func_0200312e(s32);
void Func_02000ffc(s32);
void Func_02004c4a(s32);
void Func_020048be(void);
void Func_02004c5a(s32);
void Func_020048ce(void);
void Func_02004c6a(s32);
void Func_0200491a(void);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

static __inline__ s32 Value2_0200084c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_0200087c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_020008ac(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value_0200beb4 with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void FieldScene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02002e68, (s32)&Value_0200beb4, 52, 18); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Call1(Func_02000f52, 4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value_0200beb4
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void FieldScene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue_1_0200084c(158);
    Call3(Func_02002e98, (s32)&Value_0200beb4, 41, 32); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_0200084c(0, 200, 0x222);
    Call1(Func_02000f80, 5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value_0200beb4, another 3-argument call, and a closing single-argument
 * call. */
void FieldScene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue_1_0200087c(158);
    Call3(Func_02002ec8, (s32)&Value_0200beb4, 35, 36); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_0200087c(0, 102, 0x263); /* object_id 0, x 102, z 611 */
    Call1(Func_02000fb0, 6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void FieldScene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue_1_020008ac(158);
    Call3(Func_02002ef8, (s32)&Value_0200beb4, 51, 39); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_020008ac(0, 358, 0x29e);
    Call1(Func_02000fe2, 7);
}

void FieldScene_RunStep7BThen8(void)
{
    Func_0200312e(123);
    Func_02000ffc(8);
}

void FieldScene_RunStep15(void)
{
    Func_02004c4a(15);
    Func_020048be();
}

void FieldScene_RunStep17(void)
{
    Func_02004c5a(17);
    Func_020048ce();
}

void FieldScene_RunStep20(void)
{
    Func_02004c6a(20);
    Func_0200491a();
}
