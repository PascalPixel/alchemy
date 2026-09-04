#include "types.h"

#define FieldScene_RunScene391_02000c68 Func_02000c68

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240[];
s32 Func_02000ef4();
void Func_02001530();
void Func_02001582();
void Func_02001588();
void Func_02001a22();
s32 Func_0200386c();
void Func_02003878();
s32 Func_020038a6();
void Func_020038a8();
s32 Func_020038d2();
s32 Func_020038da();
s32 Func_020038e0();
void Func_02003976();
void Func_02003980();
void Func_0200398a();
void Func_02003994();
void Func_0200399e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene-transition phase flag in the field-scene table. */
#define SceneTransition_Phase Data_02000240[225]

s32 FieldScene_RunScene391_02000c68(void)
{
    s32 record;
    s32 zero;

    Func_02001530(10);
    if (Value1(Func_0200386c, 0x200) != 0) {
        zero = 0;
        *(u8 *)(Func_020038a6(10) + 35) = 2;
        Call6(Func_02003878, 0, 17, 2, 4, 19, 17);
        record = Func_02000ef4(2, 20, 17, 1, 4, zero);
        record = Func_020038da(10);
        Func_020038a8(record, 0);
    }
    Func_02001582(8);
    Func_02001588(9);
    if (SceneTransition_Phase == 4) {
        if (Value1(Func_020038d2, 0x843) == 0) {
            Func_02001a22();
        }
    }
    if (Value1(Func_020038e0, 0x845) != 0) {
        Func_02003976(17, 0, 0);
        Func_02003980(18, 0, 0);
        Func_0200398a(19, 0, 0);
        Func_02003994(20, 0, 0);
        Func_0200399e(21, 0, 0);
    }
    return 0;
}
