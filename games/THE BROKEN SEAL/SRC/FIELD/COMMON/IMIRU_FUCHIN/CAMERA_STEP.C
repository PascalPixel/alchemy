#include "TYPES.H"

void FieldScene_RunScene39a_02001ad0();
s32 Engine_GameFlagIsSet();


extern u8 Data_0000003f[];
extern u8 Data_00000040[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void ImiruFuchin_NudgeCameraByStep(void)
{
    s32 step;

    step = *(s16 *)(*(u8 **)0x03001ebc + 0x16c);
    if (Data_02000240_t[224][0] == (s32)Data_0000003f) {
        if (step == 17)
            FieldScene_RunScene39a_02001ad0(0, -32);
        else
            FieldScene_RunScene39a_02001ad0(-32, 0);
    }
    if (Data_02000240_t[224][0] == (s32)Data_00000040 && step == 25 && Value1(Engine_GameFlagIsSet, 0x309) != 0)
        FieldScene_RunScene39a_02001ad0(0, 32);
}
